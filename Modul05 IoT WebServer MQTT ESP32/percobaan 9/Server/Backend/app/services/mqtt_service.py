import logging
import json
from app.config import settings

logger = logging.getLogger(__name__)

_mqtt_client = None


def set_mqtt_client(client) -> None:
    global _mqtt_client
    _mqtt_client = client


async def publish_command(topic: str, payload: dict) -> bool:
    if _mqtt_client is None:
        logger.warning("MQTT client not available, cannot publish command")
        return False
    try:
        await _mqtt_client.publish(topic, json.dumps(payload))
        logger.info(f"Published to {topic}: {payload}")
        return True
    except Exception as e:
        logger.error(f"Failed to publish MQTT command: {e}")
        return False


async def send_device_command(device_id: str, command: str, value=None) -> bool:
    payload = {"command": command, "device_id": device_id}
    if value is not None:
        payload["value"] = value
    return await publish_command(f"esp32/command/{device_id}", payload)


async def send_threshold_update(device_id: str, thresholds: dict) -> bool:
    payload = {"type": "threshold_update", "device_id": device_id, **thresholds}
    return await publish_command("esp32/config", payload)


async def send_mode_change(device_id: str, auto_mode: bool) -> bool:
    payload = {"command": "set_mode", "device_id": device_id, "auto_mode": auto_mode}
    return await publish_command(f"esp32/command/{device_id}", payload)


async def send_actuator_command(device_id: str, actuator: str, state: bool) -> bool:
    payload = {"command": "set_actuator", "device_id": device_id, "actuator": actuator, "state": state}
    return await publish_command(f"esp32/command/{device_id}", payload)
