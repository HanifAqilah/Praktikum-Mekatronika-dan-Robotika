#!/usr/bin/env python3
"""
Percobaan 6 – Edge MQTT Subscriber with Alerts
================================================
Subscribes to esp32/# on the configured MQTT broker, prints every
incoming message with a timestamp, fires a red-colored warning when
temperature exceeds 35 °C, and writes a rotating log file.

Usage:
    Copy .env.example → .env and fill in broker settings, then:
        python3 main.py

Dependencies (see requirements.txt):
    paho-mqtt  colorama  python-dotenv
"""

import json
import logging
import os
import sys
import time
from datetime import datetime
from logging.handlers import RotatingFileHandler

import paho.mqtt.client as mqtt
from colorama import Fore, Style, init as colorama_init
from dotenv import load_dotenv

# ── Colorama (Windows compatibility) ────────────────────────────────────────
colorama_init(autoreset=True)

# ── Load .env ────────────────────────────────────────────────────────────────
load_dotenv()

MQTT_BROKER_HOST: str = os.getenv("MQTT_BROKER_HOST", "localhost")
MQTT_BROKER_PORT: int = int(os.getenv("MQTT_BROKER_PORT", "1883"))
MQTT_USERNAME:    str = os.getenv("MQTT_USERNAME", "")
MQTT_PASSWORD:    str = os.getenv("MQTT_PASSWORD", "")
MQTT_CLIENT_ID:   str = os.getenv("MQTT_CLIENT_ID", f"edge-sub-{int(time.time())}")
SUBSCRIBE_TOPIC:  str = os.getenv("SUBSCRIBE_TOPIC", "esp32/#")

SUHU_ALERT_THRESHOLD: float = float(os.getenv("SUHU_ALERT_THRESHOLD", "35.0"))
LOG_FILE:             str   = os.getenv("LOG_FILE", "edge_log.txt")
LOG_MAX_BYTES:        int   = int(os.getenv("LOG_MAX_BYTES", str(1 * 1024 * 1024)))  # 1 MB
LOG_BACKUP_COUNT:     int   = int(os.getenv("LOG_BACKUP_COUNT", "3"))

# ── Logging setup ─────────────────────────────────────────────────────────────
_file_handler = RotatingFileHandler(
    LOG_FILE,
    maxBytes=LOG_MAX_BYTES,
    backupCount=LOG_BACKUP_COUNT,
    encoding="utf-8",
)
_file_handler.setFormatter(
    logging.Formatter("%(asctime)s | %(levelname)-8s | %(message)s",
                      datefmt="%Y-%m-%dT%H:%M:%S")
)

_stream_handler = logging.StreamHandler(sys.stdout)
_stream_handler.setFormatter(logging.Formatter("%(message)s"))

logger = logging.getLogger("edge")
logger.setLevel(logging.DEBUG)
logger.addHandler(_file_handler)
logger.addHandler(_stream_handler)


# ── Helper: parse numeric value from payload ──────────────────────────────────
def extract_float(payload: str, key: str) -> float | None:
    """Try to parse a float from a JSON payload by key, or parse the whole string."""
    try:
        obj = json.loads(payload)
        if isinstance(obj, dict) and key in obj:
            return float(obj[key])
        if isinstance(obj, (int, float)):
            return float(obj)
    except (json.JSONDecodeError, ValueError, TypeError):
        pass
    # Try raw float
    try:
        return float(payload.strip())
    except ValueError:
        return None


# ── MQTT Callbacks ────────────────────────────────────────────────────────────
def on_connect(client: mqtt.Client, userdata, flags, rc: int, properties=None):
    if rc == 0:
        logger.info(
            f"{Fore.GREEN}[MQTT] Connected to {MQTT_BROKER_HOST}:{MQTT_BROKER_PORT}{Style.RESET_ALL}"
        )
        client.subscribe(SUBSCRIBE_TOPIC, qos=1)
        logger.info(f"[MQTT] Subscribed to: {SUBSCRIBE_TOPIC}")
    else:
        logger.error(f"[MQTT] Connection failed – return code {rc}")


def on_disconnect(client: mqtt.Client, userdata, rc: int, properties=None):
    if rc == 0:
        logger.info("[MQTT] Cleanly disconnected")
    else:
        logger.warning(
            f"{Fore.YELLOW}[MQTT] Unexpected disconnect (rc={rc}), will retry…{Style.RESET_ALL}"
        )


def on_message(client: mqtt.Client, userdata, msg: mqtt.MQTTMessage):
    ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    topic   = msg.topic
    payload = msg.payload.decode("utf-8", errors="replace")

    # ── Print to console + log file
    line = f"[{ts}]  {topic}: {payload}"
    logger.info(line)

    # ── Temperature alert
    if "suhu" in topic:
        value = extract_float(payload, "suhu")
        if value is not None and value > SUHU_ALERT_THRESHOLD:
            alert = (
                f"{Fore.RED}{Style.BRIGHT}"
                f"⚠  PERINGATAN SUHU TINGGI!  "
                f"Suhu = {value:.1f} °C  (ambang: {SUHU_ALERT_THRESHOLD} °C)"
                f"{Style.RESET_ALL}"
            )
            logger.warning(alert)
            # Also write plain alert to log file
            _file_handler.stream.write(
                f"{ts} | WARNING  | ALERT: Suhu {value:.1f} °C > {SUHU_ALERT_THRESHOLD} °C\n"
            )
            _file_handler.stream.flush()


def on_subscribe(client, userdata, mid, granted_qos, properties=None):
    logger.debug(f"[MQTT] Subscription confirmed – mid={mid}, qos={granted_qos}")


# ── Main ──────────────────────────────────────────────────────────────────────
def main():
    logger.info("=" * 60)
    logger.info(" Percobaan 6 – Edge MQTT Subscriber")
    logger.info(f" Broker  : {MQTT_BROKER_HOST}:{MQTT_BROKER_PORT}")
    logger.info(f" Topic   : {SUBSCRIBE_TOPIC}")
    logger.info(f" Log     : {LOG_FILE}  (max {LOG_MAX_BYTES // 1024} KB × {LOG_BACKUP_COUNT} backups)")
    logger.info(f" Alert   : suhu > {SUHU_ALERT_THRESHOLD} °C")
    logger.info("=" * 60)

    client = mqtt.Client(
        client_id=MQTT_CLIENT_ID,
        protocol=mqtt.MQTTv5,
        callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
    )

    if MQTT_USERNAME:
        client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD or None)

    client.on_connect    = on_connect
    client.on_disconnect = on_disconnect
    client.on_message    = on_message
    client.on_subscribe  = on_subscribe

    # Enable automatic reconnection
    client.reconnect_delay_set(min_delay=1, max_delay=60)

    try:
        client.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT, keepalive=60)
    except Exception as exc:
        logger.error(
            f"{Fore.RED}[MQTT] Cannot connect: {exc}{Style.RESET_ALL}"
        )
        sys.exit(1)

    logger.info(f"{Fore.CYAN}Listening… Press Ctrl+C to quit{Style.RESET_ALL}")
    try:
        client.loop_forever()
    except KeyboardInterrupt:
        logger.info("\n[SYS] Interrupted by user – disconnecting…")
        client.disconnect()


if __name__ == "__main__":
    main()
