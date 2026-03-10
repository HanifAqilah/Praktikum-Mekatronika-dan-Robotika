#!/usr/bin/env python3
"""
EdgePython main.py – Real-time anomaly detection with IsolationForest.

Subscribes to MQTT esp32/status, extracts feature vector [suhu, kelembaban, cahaya, tanah],
runs IsolationForest prediction, and if anomaly is detected:
  1. Publishes to esp32/anomaly (MQTT)
  2. POSTs to backend REST API /api/alerts (HTTP)

Usage:
    python main.py

Environment variables (or .env file):
    MQTT_HOST       MQTT broker host (default: localhost)
    MQTT_PORT       MQTT broker port (default: 1883)
    MQTT_USERNAME   MQTT username (optional)
    MQTT_PASSWORD   MQTT password (optional)
    API_URL         Backend API URL (default: http://localhost:8009)
    API_TOKEN       JWT token for REST API authentication
    MODEL_PATH      Path to anomaly_model.pkl (default: models/anomaly_model.pkl)
    PREDICTION_INTERVAL_S  Minimum seconds between anomaly alerts per device (default: 60)
"""
import json
import logging
import os
import sys
import time
from pathlib import Path
from typing import Any

import joblib
import numpy as np
import paho.mqtt.client as mqtt
import requests
from dotenv import load_dotenv

load_dotenv()

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
)
logger = logging.getLogger("edge_anomaly")

# ── Config ────────────────────────────────────────────────────────────────
MQTT_HOST   = os.getenv("MQTT_HOST", "localhost")
MQTT_PORT   = int(os.getenv("MQTT_PORT", "1883"))
MQTT_USER   = os.getenv("MQTT_USERNAME", "")
MQTT_PASS   = os.getenv("MQTT_PASSWORD", "")
API_URL     = os.getenv("API_URL", "http://localhost:8009")
API_TOKEN   = os.getenv("API_TOKEN", "")
MODEL_PATH  = Path(os.getenv("MODEL_PATH", Path(__file__).parent / "models" / "anomaly_model.pkl"))
PRED_INTERVAL = int(os.getenv("PREDICTION_INTERVAL_S", "60"))

FEATURES = ["suhu", "kelembaban", "cahaya", "tanah"]

# ── Load model ────────────────────────────────────────────────────────────
def load_model() -> dict[str, Any]:
    if not MODEL_PATH.exists():
        logger.error(f"Model not found at {MODEL_PATH}. Run train_model.py first.")
        sys.exit(1)
    bundle = joblib.load(MODEL_PATH)
    logger.info(f"Model loaded from {MODEL_PATH} (trained on {bundle['training_size']} samples)")
    return bundle


# ── Anomaly detection ─────────────────────────────────────────────────────
def predict_anomaly(model_bundle: dict, payload: dict) -> tuple[bool, float, dict]:
    """Returns (is_anomaly, score, features_dict)."""
    features = {f: float(payload.get(f, 0)) for f in FEATURES}
    X = np.array([[features[f] for f in FEATURES]])
    pipe = model_bundle["model"]
    pred = pipe.predict(X)[0]          # 1 = normal, -1 = anomaly
    score = pipe.decision_function(X)[0]  # negative = more anomalous
    return pred == -1, float(score), features


# ── MQTT publish anomaly ──────────────────────────────────────────────────
def publish_anomaly(client: mqtt.Client, device_id: str, score: float, features: dict) -> None:
    payload = {
        "device_id": device_id,
        "anomaly_score": round(score, 6),
        "features": features,
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
    }
    topic = "esp32/anomaly"
    client.publish(topic, json.dumps(payload), qos=1)
    logger.info(f"Anomaly published to {topic}: {payload}")


# ── REST API alert ────────────────────────────────────────────────────────
def post_alert(device_id: str, score: float, features: dict) -> None:
    if not API_TOKEN:
        logger.warning("API_TOKEN not set, skipping REST alert POST")
        return
    headers = {"Authorization": f"Bearer {API_TOKEN}", "Content-Type": "application/json"}
    body = {
        "device_id": device_id,
        "alert_type": "anomaly",
        "message": (
            f"Anomali ML terdeteksi. Skor: {score:.4f}. "
            f"suhu={features.get('suhu','?')}, "
            f"kelembaban={features.get('kelembaban','?')}, "
            f"cahaya={features.get('cahaya','?')}, "
            f"tanah={features.get('tanah','?')}"
        ),
    }
    try:
        resp = requests.post(f"{API_URL}/api/alerts", json=body, headers=headers, timeout=10)
        resp.raise_for_status()
        logger.info(f"Alert posted to API: id={resp.json().get('id')}")
    except requests.RequestException as e:
        logger.error(f"Failed to post alert to API: {e}")


# ── MQTT client setup ─────────────────────────────────────────────────────
_model_bundle: dict | None = None
_last_alert: dict[str, float] = {}  # device_id -> last alert timestamp


def on_connect(client: mqtt.Client, userdata: Any, flags: dict, rc: int) -> None:
    if rc == 0:
        logger.info(f"Connected to MQTT broker {MQTT_HOST}:{MQTT_PORT}")
        client.subscribe("esp32/status")
        client.subscribe("esp32/status/#")
        logger.info("Subscribed to esp32/status")
    else:
        logger.error(f"MQTT connection failed, rc={rc}")


def on_message(client: mqtt.Client, userdata: Any, msg: mqtt.MQTTMessage) -> None:
    global _model_bundle, _last_alert

    try:
        payload = json.loads(msg.payload.decode())
    except (json.JSONDecodeError, UnicodeDecodeError) as e:
        logger.warning(f"Failed to parse message on {msg.topic}: {e}")
        return

    device_id = payload.get("device_id", "unknown")

    # Check all features are present
    if not all(f in payload for f in FEATURES):
        logger.debug(f"Incomplete payload from {device_id}, skipping")
        return

    if _model_bundle is None:
        return

    is_anomaly, score, features = predict_anomaly(_model_bundle, payload)

    logger.debug(
        f"[{device_id}] suhu={features['suhu']:.1f} kel={features['kelembaban']:.1f} "
        f"cahaya={features['cahaya']:.0f} tanah={features['tanah']:.1f} "
        f"→ {'ANOMALY' if is_anomaly else 'normal'} (score={score:.4f})"
    )

    if is_anomaly:
        now = time.time()
        last = _last_alert.get(device_id, 0)
        if now - last < PRED_INTERVAL:
            logger.debug(f"Rate-limiting anomaly alert for {device_id}")
            return
        _last_alert[device_id] = now

        logger.warning(f"ANOMALY detected for {device_id}: score={score:.4f} features={features}")
        publish_anomaly(client, device_id, score, features)
        post_alert(device_id, score, features)


def on_disconnect(client: mqtt.Client, userdata: Any, rc: int) -> None:
    if rc != 0:
        logger.warning(f"Unexpected MQTT disconnect (rc={rc}), will auto-reconnect...")


def main() -> None:
    global _model_bundle
    _model_bundle = load_model()

    client = mqtt.Client(client_id=f"edge_anomaly_{int(time.time())}", protocol=mqtt.MQTTv311)
    client.on_connect    = on_connect
    client.on_message    = on_message
    client.on_disconnect = on_disconnect

    if MQTT_USER:
        client.username_pw_set(MQTT_USER, MQTT_PASS)

    client.reconnect_delay_set(min_delay=1, max_delay=30)

    logger.info(f"Connecting to MQTT broker {MQTT_HOST}:{MQTT_PORT}...")
    client.connect(MQTT_HOST, MQTT_PORT, keepalive=60)

    logger.info("Edge anomaly detector running. Press Ctrl+C to stop.")
    client.loop_forever()


if __name__ == "__main__":
    main()
