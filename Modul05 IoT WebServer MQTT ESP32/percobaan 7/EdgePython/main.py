"""
EdgePython – Data Aggregator & Alert Rules Engine
==================================================
Subscribes to sensor/data, evaluates rules from rules.json,
publishes alerts to sensor/alert, and rolling 5-min averages
to sensor/agg.  All logs are written in structured JSON format.

Default rules.json (created automatically if missing):
[
  {"field": "suhu",      "op": ">",  "value": 35,   "action": "alert", "message": "Suhu kritis!"},
  {"field": "suhu",      "op": "<",  "value": 10,   "action": "alert", "message": "Suhu terlalu rendah!"},
  {"field": "kelembaban","op": ">",  "value": 90,   "action": "alert", "message": "Kelembaban sangat tinggi!"},
  {"field": "kelembaban","op": "<",  "value": 20,   "action": "alert", "message": "Kelembaban sangat rendah!"},
  {"field": "cahaya",    "op": ">",  "value": 4000, "action": "alert", "message": "Cahaya berlebih!"},
  {"field": "tanah",     "op": ">",  "value": 3000, "action": "alert", "message": "Tanah terlalu basah!"},
  {"field": "tanah",     "op": "<",  "value": 500,  "action": "alert", "message": "Tanah terlalu kering!"}
]

Usage:
  cp .env.example .env && python main.py
"""

import asyncio
import json
import logging
import logging.handlers
import os
import time
from collections import deque
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Deque

import aiomqtt
import structlog
from dotenv import load_dotenv

load_dotenv()

# ─── Config ──────────────────────────────────────────────────────────────────
MQTT_HOST        = os.getenv("MQTT_HOST", "localhost")
MQTT_PORT        = int(os.getenv("MQTT_PORT", "1883"))
MQTT_USERNAME    = os.getenv("MQTT_USERNAME", "")
MQTT_PASSWORD    = os.getenv("MQTT_PASSWORD", "")
TOPIC_DATA       = os.getenv("MQTT_TOPIC_DATA", "sensor/data")
TOPIC_ALERT      = os.getenv("MQTT_TOPIC_ALERT", "sensor/alert")
TOPIC_AGG        = os.getenv("MQTT_TOPIC_AGG", "sensor/agg")
RULES_FILE       = Path(os.getenv("RULES_FILE", "rules.json"))
LOG_FILE         = os.getenv("LOG_FILE", "edge_agent.log")
ROLLING_WINDOW_S = 300   # 5 minutes

DEFAULT_RULES = [
    {"field": "suhu",       "op": ">",  "value": 35,   "action": "alert", "message": "Suhu kritis!"},
    {"field": "suhu",       "op": "<",  "value": 10,   "action": "alert", "message": "Suhu terlalu rendah!"},
    {"field": "kelembaban", "op": ">",  "value": 90,   "action": "alert", "message": "Kelembaban sangat tinggi!"},
    {"field": "kelembaban", "op": "<",  "value": 20,   "action": "alert", "message": "Kelembaban sangat rendah!"},
    {"field": "cahaya",     "op": ">",  "value": 4000, "action": "alert", "message": "Cahaya berlebih!"},
    {"field": "tanah",      "op": ">",  "value": 3000, "action": "alert", "message": "Tanah terlalu basah!"},
    {"field": "tanah",      "op": "<",  "value": 500,  "action": "alert", "message": "Tanah terlalu kering!"},
]

# ─── Logging ─────────────────────────────────────────────────────────────────
_file_handler = logging.handlers.RotatingFileHandler(
    LOG_FILE, maxBytes=5 * 1024 * 1024, backupCount=3, encoding="utf-8"
)

structlog.configure(
    processors=[
        structlog.stdlib.add_log_level,
        structlog.processors.TimeStamper(fmt="iso"),
        structlog.processors.JSONRenderer(),
    ],
    wrapper_class=structlog.stdlib.BoundLogger,
    logger_factory=structlog.WriteLoggerFactory(file=_file_handler.stream),
)
_console = logging.getLogger("edge")
_console.setLevel(logging.DEBUG)
_console.addHandler(logging.StreamHandler())

log = structlog.get_logger("edge_agent")


# ─── Rules ───────────────────────────────────────────────────────────────────
def load_rules() -> list[dict]:
    if not RULES_FILE.exists():
        RULES_FILE.write_text(json.dumps(DEFAULT_RULES, indent=2))
        _console.info("Created default rules.json")
    with RULES_FILE.open() as f:
        rules = json.load(f)
    _console.info("Loaded %d rules from %s", len(rules), RULES_FILE)
    return rules


OPS = {
    ">":  lambda a, b: a > b,
    "<":  lambda a, b: a < b,
    ">=": lambda a, b: a >= b,
    "<=": lambda a, b: a <= b,
    "==": lambda a, b: a == b,
    "!=": lambda a, b: a != b,
}


def evaluate_rules(payload: dict, rules: list[dict]) -> list[dict]:
    """Return list of triggered rule results."""
    triggered = []
    for rule in rules:
        field   = rule.get("field", "")
        op      = rule.get("op", ">")
        value   = rule.get("value")
        message = rule.get("message", f"{field} {op} {value}")

        if field not in payload:
            continue
        fn = OPS.get(op)
        if fn is None:
            _console.warning("Unknown op '%s' in rule", op)
            continue
        try:
            actual = float(payload[field])
        except (TypeError, ValueError):
            continue

        if fn(actual, float(value)):
            triggered.append({
                "field":   field,
                "op":      op,
                "value":   value,
                "actual":  actual,
                "message": message,
                "device":  payload.get("device", "unknown"),
                "ts":      payload.get("ts", int(time.time())),
            })
    return triggered


# ─── Rolling Window ──────────────────────────────────────────────────────────
# Per-device deque of (timestamp, reading_dict)
_rolling: dict[str, Deque[tuple[float, dict]]] = {}


def update_rolling(device: str, payload: dict) -> dict | None:
    """
    Add payload to the rolling window; return 5-min average if at least
    one full minute of data is available, else None.
    """
    now = time.time()
    if device not in _rolling:
        _rolling[device] = deque()

    _rolling[device].append((now, payload))

    # Purge entries older than ROLLING_WINDOW_S
    while _rolling[device] and now - _rolling[device][0][0] > ROLLING_WINDOW_S:
        _rolling[device].popleft()

    window = _rolling[device]
    if len(window) < 2:
        return None

    # Only publish an aggregate every ~60 s (when window >= 60 s old)
    oldest_ts = window[0][0]
    if now - oldest_ts < 60:
        return None

    fields = ["suhu", "kelembaban", "cahaya", "tanah"]
    averages = {}
    for f in fields:
        vals = [r[f] for _, r in window if f in r]
        if vals:
            averages[f] = round(sum(vals) / len(vals), 2)

    return {
        "device":  device,
        "window_s": ROLLING_WINDOW_S,
        "samples":  len(window),
        "averages": averages,
        "ts":       int(now),
    }


# ─── Main ────────────────────────────────────────────────────────────────────
async def run():
    rules = load_rules()
    reconnect_delay = 5

    while True:
        try:
            kwargs: dict[str, Any] = {}
            if MQTT_USERNAME:
                kwargs["username"] = MQTT_USERNAME
            if MQTT_PASSWORD:
                kwargs["password"] = MQTT_PASSWORD

            async with aiomqtt.Client(
                hostname=MQTT_HOST,
                port=MQTT_PORT,
                **kwargs,
            ) as client:
                _console.info("Connected to MQTT %s:%d", MQTT_HOST, MQTT_PORT)
                reconnect_delay = 5

                await client.subscribe(TOPIC_DATA)
                _console.info("Subscribed to %s", TOPIC_DATA)

                async for message in client.messages:
                    topic = str(message.topic)
                    raw   = message.payload.decode("utf-8", errors="replace")

                    try:
                        payload = json.loads(raw)
                    except json.JSONDecodeError:
                        log.warning("invalid_json", raw=raw)
                        continue

                    device = payload.get("device", "unknown")
                    log.info("received", device=device, topic=topic, payload=payload)

                    # ── Rules engine ─────────────────────────────────────────
                    triggered = evaluate_rules(payload, rules)
                    for t in triggered:
                        alert_json = json.dumps(t)
                        await client.publish(TOPIC_ALERT, alert_json)
                        log.warning("alert_published", **t)
                        _console.warning("[ALERT] %s", t["message"])

                    # ── Rolling aggregation ───────────────────────────────────
                    agg = update_rolling(device, payload)
                    if agg is not None:
                        agg_json = json.dumps(agg)
                        await client.publish(TOPIC_AGG, agg_json)
                        log.info("agg_published", device=device, averages=agg["averages"])
                        _console.info("[AGG] %s → %s", device, agg["averages"])

        except aiomqtt.MqttError as exc:
            _console.warning("MQTT error: %s – retry in %ds", exc, reconnect_delay)
            await asyncio.sleep(reconnect_delay)
            reconnect_delay = min(reconnect_delay * 2, 60)
        except asyncio.CancelledError:
            _console.info("Edge agent stopped")
            raise
        except Exception as exc:
            _console.exception("Unexpected error: %s", exc)
            await asyncio.sleep(reconnect_delay)


if __name__ == "__main__":
    try:
        asyncio.run(run())
    except KeyboardInterrupt:
        _console.info("Edge agent interrupted by user")
    finally:
        _file_handler.close()
