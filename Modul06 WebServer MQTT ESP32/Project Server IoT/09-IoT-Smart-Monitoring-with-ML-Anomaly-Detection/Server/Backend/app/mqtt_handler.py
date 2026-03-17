import asyncio
import json
import logging
from datetime import datetime, timezone

import aiomqtt

from app.config import settings
from app.database import AsyncSessionLocal
from app.models.device import Device
from app.models.sensor import SensorReading
from app.services.alert_service import check_and_create_alerts
from app.services import mqtt_service
from app.websocket_manager import manager
from sqlalchemy import select

logger = logging.getLogger(__name__)

RECONNECT_INTERVAL = 5


async def _process_status_message(payload: dict) -> None:
    device_id = payload.get("device_id")
    if not device_id:
        logger.warning("Received status without device_id")
        return

    suhu = float(payload.get("suhu", 0))
    kelembaban = float(payload.get("kelembaban", 0))
    cahaya = float(payload.get("cahaya", 0))
    tanah = float(payload.get("tanah", 0))
    fan_state = bool(payload.get("fan", False))
    pump_state = bool(payload.get("pump", False))
    auto_mode = bool(payload.get("auto_mode", True))
    rssi = payload.get("rssi")
    firmware = payload.get("firmware")

    async with AsyncSessionLocal() as db:
        result = await db.execute(select(Device).where(Device.device_id == device_id))
        device = result.scalar_one_or_none()

        if device is None:
            device = Device(
                device_id=device_id,
                name=f"ESP32-{device_id}",
                status="online",
                last_seen=datetime.now(timezone.utc),
                auto_mode=auto_mode,
                fan_state=fan_state,
                pump_state=pump_state,
                firmware_version=firmware,
            )
            db.add(device)
            await db.flush()
        else:
            device.status = "online"
            device.last_seen = datetime.now(timezone.utc)
            device.auto_mode = auto_mode
            device.fan_state = fan_state
            device.pump_state = pump_state
            if firmware:
                device.firmware_version = firmware

        reading = SensorReading(
            device_id=device_id,
            suhu=suhu,
            kelembaban=kelembaban,
            cahaya=cahaya,
            tanah=tanah,
            fan_state=fan_state,
            pump_state=pump_state,
            auto_mode=auto_mode,
            rssi=rssi,
        )
        db.add(reading)
        await db.flush()

        await db.commit()
        await db.refresh(reading)

        await check_and_create_alerts(db, device, reading.id, suhu, kelembaban, cahaya, tanah)

        await manager.broadcast_sensor(
            {
                "id": reading.id,
                "device_id": device_id,
                "suhu": suhu,
                "kelembaban": kelembaban,
                "cahaya": cahaya,
                "tanah": tanah,
                "fan_state": fan_state,
                "pump_state": pump_state,
                "auto_mode": auto_mode,
                "rssi": rssi,
                "created_at": reading.created_at.isoformat(),
            }
        )

        await manager.broadcast_device_status(
            {
                "device_id": device_id,
                "status": "online",
                "last_seen": device.last_seen.isoformat(),
                "auto_mode": auto_mode,
                "fan_state": fan_state,
                "pump_state": pump_state,
            }
        )


async def _process_anomaly_message(payload: dict) -> None:
    device_id = payload.get("device_id", "unknown")
    score = payload.get("anomaly_score", 0)
    features = payload.get("features", {})

    async with AsyncSessionLocal() as db:
        from app.services.alert_service import create_alert
        from app.models.alert import AlertType

        await create_alert(
            db,
            device_id,
            AlertType.ANOMALY,
            f"Anomali terdeteksi oleh ML model. Skor: {score:.4f}. "
            f"Fitur: suhu={features.get('suhu', '?')}, "
            f"kelembaban={features.get('kelembaban', '?')}, "
            f"cahaya={features.get('cahaya', '?')}, "
            f"tanah={features.get('tanah', '?')}",
        )


async def mqtt_listener() -> None:
    while True:
        try:
            kwargs = {"hostname": settings.MQTT_HOST, "port": settings.MQTT_PORT}
            if settings.MQTT_USERNAME:
                kwargs["username"] = settings.MQTT_USERNAME
                kwargs["password"] = settings.MQTT_PASSWORD

            async with aiomqtt.Client(**kwargs) as client:
                mqtt_service.set_mqtt_client(client)
                logger.info(f"Connected to MQTT broker {settings.MQTT_HOST}:{settings.MQTT_PORT}")

                await client.subscribe("esp32/status")
                await client.subscribe("esp32/status/#")
                await client.subscribe("esp32/anomaly")
                logger.info("Subscribed to MQTT topics")

                async for message in client.messages:
                    topic = str(message.topic)
                    try:
                        payload = json.loads(message.payload.decode())
                    except (json.JSONDecodeError, UnicodeDecodeError) as e:
                        logger.warning(f"Failed to parse MQTT payload on {topic}: {e}")
                        continue

                    try:
                        if topic.startswith("esp32/status"):
                            await _process_status_message(payload)
                        elif topic == "esp32/anomaly":
                            await _process_anomaly_message(payload)
                    except Exception as e:
                        logger.error(f"Error processing MQTT message on {topic}: {e}")

        except aiomqtt.MqttError as e:
            mqtt_service.set_mqtt_client(None)
            logger.error(f"MQTT connection error: {e}. Reconnecting in {RECONNECT_INTERVAL}s...")
            await asyncio.sleep(RECONNECT_INTERVAL)
        except asyncio.CancelledError:
            logger.info("MQTT listener cancelled")
            return
        except Exception as e:
            logger.error(f"Unexpected MQTT error: {e}. Reconnecting in {RECONNECT_INTERVAL}s...")
            await asyncio.sleep(RECONNECT_INTERVAL)
