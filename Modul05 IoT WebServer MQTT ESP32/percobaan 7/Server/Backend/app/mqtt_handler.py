"""
aiomqtt subscriber running as an asyncio background task.
Receives JSON sensor payloads, persists to DB, and triggers alert checks.
"""

import asyncio
import json
import logging
from datetime import datetime, timezone

import aiomqtt

from app.config import settings
from app.database import AsyncSessionLocal
from app.models import SensorReading, Alert

logger = logging.getLogger("mqtt_handler")

# Shared in-memory cache: latest reading per device (used by SSE)
latest_readings: dict[str, dict] = {}


async def _check_and_manage_alerts(session, payload: dict, device_id: str):
    """Auto-create or auto-resolve alerts based on configured thresholds."""
    from sqlalchemy import select
    from datetime import datetime, timezone

    checks = [
        ("suhu",      payload["suhu"],      settings.SUHU_MAX,      ">",  f"Suhu tinggi {payload['suhu']:.1f}°C > {settings.SUHU_MAX}°C"),
        ("suhu",      payload["suhu"],      settings.SUHU_MIN,      "<",  f"Suhu rendah {payload['suhu']:.1f}°C < {settings.SUHU_MIN}°C"),
        ("kelembaban",payload["kelembaban"],settings.KELEMBABAN_MAX, ">",  f"Kelembaban tinggi {payload['kelembaban']:.1f}% > {settings.KELEMBABAN_MAX}%"),
        ("kelembaban",payload["kelembaban"],settings.KELEMBABAN_MIN, "<",  f"Kelembaban rendah {payload['kelembaban']:.1f}% < {settings.KELEMBABAN_MIN}%"),
        ("cahaya",    payload["cahaya"],    settings.CAHAYA_MAX,     ">",  f"Cahaya berlebih {payload['cahaya']} > {settings.CAHAYA_MAX}"),
        ("tanah",     payload["tanah"],     settings.TANAH_MAX,      ">",  f"Kelembaban tanah tinggi {payload['tanah']} > {settings.TANAH_MAX}"),
        ("tanah",     payload["tanah"],     settings.TANAH_MIN,      "<",  f"Kelembaban tanah rendah {payload['tanah']} < {settings.TANAH_MIN}"),
    ]

    for field, value, threshold, op, message in checks:
        alert_key = f"{device_id}:{field}:{op}"
        triggered = (op == ">" and value > threshold) or (op == "<" and value < threshold)

        # Find existing open alert for this device+field+op combination
        result = await session.execute(
            select(Alert).where(
                Alert.device_id == device_id,
                Alert.field == alert_key,
                Alert.resolved == False,  # noqa: E712
            )
        )
        existing = result.scalars().first()

        if triggered and existing is None:
            new_alert = Alert(
                device_id=device_id,
                field=alert_key,
                message=message,
                value=float(value),
                threshold=float(threshold),
                resolved=False,
            )
            session.add(new_alert)
            logger.warning("Alert created: %s", message)

        elif not triggered and existing is not None:
            existing.resolved = True
            existing.resolved_at = datetime.now(timezone.utc)
            logger.info("Alert resolved: %s", existing.message)

    await session.commit()


async def _handle_message(raw: str):
    try:
        payload = json.loads(raw)
    except json.JSONDecodeError:
        logger.error("Invalid JSON payload: %s", raw)
        return

    required = {"suhu", "kelembaban", "cahaya", "tanah"}
    if not required.issubset(payload):
        logger.error("Missing fields in payload: %s", payload)
        return

    device_id = payload.get("device", "esp32-unknown")

    reading = SensorReading(
        device_id=device_id,
        suhu=float(payload["suhu"]),
        kelembaban=float(payload["kelembaban"]),
        cahaya=int(payload["cahaya"]),
        tanah=int(payload["tanah"]),
    )

    async with AsyncSessionLocal() as session:
        session.add(reading)
        await session.commit()
        await session.refresh(reading)
        await _check_and_manage_alerts(session, payload, device_id)

    # Update in-memory cache for SSE
    latest_readings[device_id] = {
        "id": reading.id,
        "device_id": device_id,
        "suhu": reading.suhu,
        "kelembaban": reading.kelembaban,
        "cahaya": reading.cahaya,
        "tanah": reading.tanah,
        "created_at": reading.created_at.isoformat() if reading.created_at else datetime.now(timezone.utc).isoformat(),
    }
    logger.debug("Stored reading from %s: suhu=%.1f", device_id, reading.suhu)


async def start_mqtt_listener():
    """Long-running asyncio task that subscribes to MQTT topics."""
    reconnect_delay = 5

    while True:
        try:
            kwargs = {}
            if settings.MQTT_USERNAME:
                kwargs["username"] = settings.MQTT_USERNAME
            if settings.MQTT_PASSWORD:
                kwargs["password"] = settings.MQTT_PASSWORD

            async with aiomqtt.Client(
                hostname=settings.MQTT_HOST,
                port=settings.MQTT_PORT,
                **kwargs,
            ) as client:
                logger.info("MQTT connected to %s:%d", settings.MQTT_HOST, settings.MQTT_PORT)
                reconnect_delay = 5

                await client.subscribe(settings.MQTT_TOPIC_DATA)
                await client.subscribe(settings.MQTT_TOPIC_AGG)
                logger.info(
                    "Subscribed to %s and %s",
                    settings.MQTT_TOPIC_DATA,
                    settings.MQTT_TOPIC_AGG,
                )

                async for message in client.messages:
                    topic = str(message.topic)
                    payload_str = message.payload.decode("utf-8", errors="replace")
                    logger.debug("MQTT [%s]: %s", topic, payload_str)
                    await _handle_message(payload_str)

        except aiomqtt.MqttError as exc:
            logger.warning("MQTT error: %s – retrying in %ds", exc, reconnect_delay)
            await asyncio.sleep(reconnect_delay)
            reconnect_delay = min(reconnect_delay * 2, 60)
        except asyncio.CancelledError:
            logger.info("MQTT listener cancelled")
            raise
        except Exception as exc:
            logger.exception("Unexpected MQTT error: %s", exc)
            await asyncio.sleep(reconnect_delay)
