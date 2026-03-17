import asyncio
import datetime
import json
import logging
from contextlib import asynccontextmanager

import aiomqtt
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from app.config import settings
from app.database import create_tables, AsyncSessionLocal
from app.models import SensorReading
from app.routers import actuators as actuators_router
from app.routers.actuators import set_mqtt_client
from app.routers.sensors import router as sensors_router
from app.routers.alerts import router as alerts_router
from app.routers.analytics import router as analytics_router
from app.routers.auth import router as auth_router
from app.services import alert_service, cache_service

logger = logging.getLogger("greenhouse")
logging.basicConfig(level=logging.INFO)

_mqtt_task: asyncio.Task | None = None
_cleanup_task: asyncio.Task | None = None


async def _mqtt_subscriber() -> None:
    """Subscribe to greenhouse/# and persist sensor readings."""
    broker_kwargs: dict = dict(hostname=settings.MQTT_BROKER, port=settings.MQTT_PORT)
    if settings.MQTT_USERNAME:
        broker_kwargs["username"] = settings.MQTT_USERNAME
        broker_kwargs["password"] = settings.MQTT_PASSWORD

    while True:
        try:
            async with aiomqtt.Client(**broker_kwargs, identifier=settings.MQTT_CLIENT_ID) as client:
                set_mqtt_client(client)
                await client.subscribe("greenhouse/#")
                logger.info("MQTT subscribed to greenhouse/#")
                async for message in client.messages:
                    await _handle_message(str(message.topic), message.payload)
        except Exception as exc:
            logger.warning("MQTT connection lost: %s – reconnecting in 5s", exc)
            set_mqtt_client(None)
            await asyncio.sleep(5)


async def _handle_message(topic: str, raw: bytes) -> None:
    try:
        payload = json.loads(raw)
    except Exception:
        return

    parts = topic.split("/")
    if len(parts) < 3:
        return

    device_id = parts[1]
    msg_type = parts[2]

    if msg_type != "sensors":
        return

    reading_data = {k: payload.get(k) for k in ("suhu", "kelembaban", "cahaya", "tanah")}
    reading_data["device_id"] = device_id

    async with AsyncSessionLocal() as db:
        reading = SensorReading(**reading_data)
        db.add(reading)
        await db.commit()
        await db.refresh(reading)
        await cache_service.set_realtime(
            f"sensor:{device_id}",
            {**reading_data, "id": reading.id, "created_at": reading.created_at.isoformat()},
        )
        await alert_service.check_thresholds(db, device_id, reading_data)


async def _midnight_cleanup() -> None:
    """Delete readings older than 30 days, runs at midnight each day."""
    while True:
        now = datetime.datetime.now(datetime.timezone.utc)
        tomorrow_midnight = (now + datetime.timedelta(days=1)).replace(
            hour=0, minute=0, second=0, microsecond=0
        )
        await asyncio.sleep((tomorrow_midnight - now).total_seconds())
        async with AsyncSessionLocal() as db:
            deleted = await alert_service.__module__ and __import__(
                "app.services.sensor_service", fromlist=["purge_old_readings"]
            ).purge_old_readings(db, 30)
            logger.info("Cleanup: removed %s old readings", deleted)


async def _run_cleanup() -> None:
    from app.services.sensor_service import purge_old_readings
    while True:
        now = datetime.datetime.now(datetime.timezone.utc)
        tomorrow = (now + datetime.timedelta(days=1)).replace(
            hour=0, minute=0, second=0, microsecond=0
        )
        await asyncio.sleep((tomorrow - now).total_seconds())
        async with AsyncSessionLocal() as db:
            deleted = await purge_old_readings(db, 30)
            logger.info("Nightly cleanup: removed %d old readings", deleted)


@asynccontextmanager
async def lifespan(app: FastAPI):
    await create_tables()
    global _mqtt_task, _cleanup_task
    _mqtt_task = asyncio.create_task(_mqtt_subscriber())
    _cleanup_task = asyncio.create_task(_run_cleanup())
    logger.info("Greenhouse backend started")
    yield
    _mqtt_task.cancel()
    _cleanup_task.cancel()
    await cache_service.close_redis()
    logger.info("Greenhouse backend stopped")


app = FastAPI(
    title="Greenhouse Monitoring API",
    version="1.0.0",
    lifespan=lifespan,
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=settings.origins_list,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(sensors_router)
app.include_router(actuators_router.router)
app.include_router(alerts_router)
app.include_router(analytics_router)
app.include_router(auth_router)


@app.get("/health")
async def health():
    return {"status": "ok"}
