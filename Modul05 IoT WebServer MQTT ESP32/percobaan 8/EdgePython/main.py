#!/usr/bin/env python3
"""
EdgePython — Percobaan 8: IoT Edge Bridge + Command Validator
Praktikum Mekatronika dan Robotika

Fungsi:
  1. Subscribe esp32/command/# → validasi JSON schema → republish jika valid
  2. Rate limiter per actuator type (max 10 cmd/menit, menggunakan collections.deque)
  3. Log semua perintah ke command_audit.log dengan timestamp
  4. Subscribe esp32/ack/# → tampilkan ACK di terminal dengan warna (colorama)
  5. Subscribe esp32/sensor/# → tampilkan live sensor di terminal
"""

import json
import os
import sys
import time
import logging
import threading
from collections import deque
from datetime import datetime, timezone

import paho.mqtt.client as mqtt
from dotenv import load_dotenv
from colorama import Fore, Style, init as colorama_init
from jsonschema import validate, ValidationError

# ─── Init ─────────────────────────────────────────────────
colorama_init(autoreset=True)
load_dotenv(os.path.join(os.path.dirname(__file__), ".env"))

# ─── Env config ───────────────────────────────────────────
MQTT_BROKER   = os.getenv("MQTT_BROKER", "localhost")
MQTT_PORT     = int(os.getenv("MQTT_PORT", "1883"))
MQTT_USERNAME = os.getenv("MQTT_USERNAME", "")
MQTT_PASSWORD = os.getenv("MQTT_PASSWORD", "")
MQTT_CLIENT_ID = os.getenv("MQTT_CLIENT_ID", f"edge-percobaan8-{int(time.time())}")
RATE_LIMIT_MAX = int(os.getenv("RATE_LIMIT_MAX", "10"))
RATE_LIMIT_WINDOW_S = int(os.getenv("RATE_LIMIT_WINDOW_SECONDS", "60"))
LOG_FILE = os.getenv("LOG_FILE", "command_audit.log")

# ─── Logging to file ──────────────────────────────────────
log_dir = os.path.dirname(os.path.abspath(LOG_FILE)) if os.path.dirname(LOG_FILE) else "."
os.makedirs(log_dir, exist_ok=True)

file_handler = logging.FileHandler(LOG_FILE, encoding="utf-8")
file_handler.setFormatter(logging.Formatter("%(asctime)s | %(levelname)s | %(message)s"))
audit_logger = logging.getLogger("audit")
audit_logger.setLevel(logging.INFO)
audit_logger.addHandler(file_handler)

# ─── JSON Schemas ─────────────────────────────────────────
ACTUATOR_SCHEMA = {
    "type": "object",
    "required": ["state"],
    "properties": {
        "state": {"type": "string", "enum": ["on", "off", "toggle"]},
        "led":   {"type": "integer", "minimum": 1, "maximum": 3},
        "ts":    {"type": "number"},
    },
    "additionalProperties": True,
}

LED_SCHEMA = {
    "type": "object",
    "required": ["state", "led"],
    "properties": {
        "state": {"type": "string", "enum": ["on", "off", "toggle"]},
        "led":   {"type": "integer", "minimum": 1, "maximum": 3},
        "ts":    {"type": "number"},
    },
    "additionalProperties": True,
}

SCHEMA_MAP: dict[str, dict] = {
    "led1":  LED_SCHEMA,
    "led2":  LED_SCHEMA,
    "led3":  LED_SCHEMA,
    "relay": ACTUATOR_SCHEMA,
    "all":   ACTUATOR_SCHEMA,
}

# ─── Rate Limiter ─────────────────────────────────────────
class RateLimiter:
    """Per-actuator token bucket using a deque of timestamps."""

    def __init__(self, max_calls: int, window_seconds: int):
        self.max_calls = max_calls
        self.window_s  = window_seconds
        self._buckets: dict[str, deque] = {}
        self._lock = threading.Lock()

    def is_allowed(self, key: str) -> bool:
        now = time.monotonic()
        cutoff = now - self.window_s
        with self._lock:
            bucket = self._buckets.setdefault(key, deque())
            # Remove timestamps older than window
            while bucket and bucket[0] < cutoff:
                bucket.popleft()
            if len(bucket) >= self.max_calls:
                return False
            bucket.append(now)
            return True

    def remaining(self, key: str) -> int:
        now = time.monotonic()
        cutoff = now - self.window_s
        with self._lock:
            bucket = self._buckets.get(key, deque())
            count = sum(1 for ts in bucket if ts >= cutoff)
            return max(0, self.max_calls - count)


rate_limiter = RateLimiter(RATE_LIMIT_MAX, RATE_LIMIT_WINDOW_S)

# ─── Helpers ──────────────────────────────────────────────
def now_iso() -> str:
    return datetime.now(tz=timezone.utc).isoformat()

def print_header(text: str, color: str = Fore.CYAN):
    print(f"\n{color}{'─' * 60}{Style.RESET_ALL}")
    print(f"{color}  {text}{Style.RESET_ALL}")
    print(f"{color}{'─' * 60}{Style.RESET_ALL}")

def extract_actuator(topic: str) -> str:
    """Extract actuator name from topic like esp32/command/led1 → led1"""
    parts = topic.split("/")
    return parts[-1] if parts else topic

# ─── Message handlers ─────────────────────────────────────
def handle_command(client: mqtt.Client, topic: str, payload_str: str):
    """Validate command payload, apply rate limiting, and republish if valid."""
    actuator = extract_actuator(topic)
    ts = now_iso()

    # 1. Parse JSON
    try:
        payload = json.loads(payload_str)
    except json.JSONDecodeError as e:
        msg = f"REJECTED [JSON parse error] topic={topic} payload={payload_str!r} error={e}"
        print(f"{Fore.RED}⛔  {msg}{Style.RESET_ALL}")
        audit_logger.warning(msg)
        return

    # 2. Validate schema
    schema = SCHEMA_MAP.get(actuator, ACTUATOR_SCHEMA)
    try:
        validate(instance=payload, schema=schema)
    except ValidationError as e:
        msg = f"REJECTED [schema invalid] topic={topic} error={e.message} payload={payload}"
        print(f"{Fore.YELLOW}⚠️   {msg}{Style.RESET_ALL}")
        audit_logger.warning(msg)
        return

    # 3. Rate limit check
    if not rate_limiter.is_allowed(actuator):
        remaining_wait = RATE_LIMIT_WINDOW_S
        msg = (
            f"REJECTED [rate limit] topic={topic} "
            f"max={RATE_LIMIT_MAX}/{RATE_LIMIT_WINDOW_S}s payload={payload}"
        )
        print(f"{Fore.MAGENTA}🚫  {msg}{Style.RESET_ALL}")
        audit_logger.warning(msg)
        return

    # 4. Re-publish validated command (add edge metadata)
    payload["_edge_validated"] = True
    payload["_edge_ts"] = ts
    republish_payload = json.dumps(payload)

    client.publish(topic, republish_payload, qos=1)
    remaining = rate_limiter.remaining(actuator)

    msg = (
        f"ACCEPTED topic={topic} state={payload.get('state')} "
        f"remaining_rate={remaining}/{RATE_LIMIT_MAX}"
    )
    print(f"{Fore.GREEN}✅  {msg}{Style.RESET_ALL}")
    audit_logger.info(msg)


def handle_ack(topic: str, payload_str: str):
    """Display ACK from ESP32 with color in terminal."""
    actuator = extract_actuator(topic)
    try:
        data = json.loads(payload_str)
        status = data.get("status", "?")
        state  = data.get("state", "?")
        color = Fore.GREEN if status in ("ok", "success") else Fore.RED
        icon  = "✔" if status in ("ok", "success") else "✘"
        print(
            f"{color}{icon} ACK [{actuator}] "
            f"status={status} state={state} "
            f"ts={data.get('ts', '?')}{Style.RESET_ALL}"
        )
        audit_logger.info(f"ACK topic={topic} status={status} state={state}")
    except json.JSONDecodeError:
        print(f"{Fore.YELLOW}⚠️  ACK [{actuator}] raw={payload_str!r}{Style.RESET_ALL}")


def handle_sensor(topic: str, payload_str: str):
    """Display live sensor data in terminal."""
    try:
        data = json.loads(payload_str)
        suhu      = data.get("suhu", "?")
        kelembaban = data.get("kelembaban", "?")
        cahaya    = data.get("cahaya", "?")
        tanah     = data.get("tanah", "?")
        device    = data.get("device_id", topic.split("/")[-1])
        print(
            f"{Fore.CYAN}📡 SENSOR [{device}] "
            f"suhu={Fore.YELLOW}{suhu}°C{Fore.CYAN} "
            f"kel={Fore.BLUE}{kelembaban}%{Fore.CYAN} "
            f"cahaya={Fore.WHITE}{cahaya}{Fore.CYAN} "
            f"tanah={Fore.GREEN}{tanah}{Style.RESET_ALL}"
        )
    except json.JSONDecodeError:
        print(f"{Fore.CYAN}📡 SENSOR raw={payload_str!r}{Style.RESET_ALL}")

# ─── MQTT Callbacks ───────────────────────────────────────
def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print(f"{Fore.GREEN}✅ Connected to MQTT broker {MQTT_BROKER}:{MQTT_PORT}{Style.RESET_ALL}")
        client.subscribe("esp32/command/#", qos=1)
        client.subscribe("esp32/ack/#",     qos=0)
        client.subscribe("esp32/sensor/#",  qos=0)
        print(f"{Fore.CYAN}📥 Subscribed to esp32/command/#, esp32/ack/#, esp32/sensor/#")
    else:
        print(f"{Fore.RED}❌ Connection failed reason_code={reason_code}{Style.RESET_ALL}")


def on_disconnect(client, userdata, flags, reason_code, properties):
    if reason_code != 0:
        print(f"{Fore.YELLOW}⚡ Disconnected (rc={reason_code}). Reconnecting...{Style.RESET_ALL}")


def on_message(client, userdata, msg):
    topic       = msg.topic
    payload_str = msg.payload.decode("utf-8", errors="replace")

    if topic.startswith("esp32/command/"):
        handle_command(client, topic, payload_str)
    elif topic.startswith("esp32/ack/"):
        handle_ack(topic, payload_str)
    elif topic.startswith("esp32/sensor/"):
        handle_sensor(topic, payload_str)

# ─── Main ─────────────────────────────────────────────────
def main():
    print_header("Percobaan 8 — EdgePython Bridge", Fore.CYAN)
    print(f"  Broker  : {MQTT_BROKER}:{MQTT_PORT}")
    print(f"  ClientID: {MQTT_CLIENT_ID}")
    print(f"  Rate    : max {RATE_LIMIT_MAX} cmd/{RATE_LIMIT_WINDOW_S}s per actuator")
    print(f"  Log     : {LOG_FILE}")
    print()

    client = mqtt.Client(
        mqtt.CallbackAPIVersion.VERSION2,
        client_id=MQTT_CLIENT_ID,
        clean_session=True,
    )
    client.on_connect    = on_connect
    client.on_disconnect = on_disconnect
    client.on_message    = on_message

    if MQTT_USERNAME:
        client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)

    client.reconnect_delay_set(min_delay=1, max_delay=30)

    try:
        client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
    except ConnectionRefusedError:
        print(f"{Fore.RED}❌ Cannot connect to {MQTT_BROKER}:{MQTT_PORT}. Is the broker running?{Style.RESET_ALL}")
        sys.exit(1)
    except Exception as e:
        print(f"{Fore.RED}❌ Connection error: {e}{Style.RESET_ALL}")
        sys.exit(1)

    print(f"{Fore.WHITE}Press Ctrl+C to stop.{Style.RESET_ALL}\n")
    audit_logger.info(f"EdgePython started. Broker={MQTT_BROKER}:{MQTT_PORT}")

    try:
        client.loop_forever()
    except KeyboardInterrupt:
        print(f"\n{Fore.YELLOW}⏹  Stopping EdgePython...{Style.RESET_ALL}")
        audit_logger.info("EdgePython stopped by user.")
        client.disconnect()
        sys.exit(0)


if __name__ == "__main__":
    main()
