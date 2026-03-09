import asyncio
import logging
from contextlib import asynccontextmanager

from fastapi import FastAPI, WebSocket, WebSocketDisconnect, Query, Depends
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy import func, select, desc
from sqlalchemy.ext.asyncio import AsyncSession

from app.config import settings
from app.database import create_tables, get_db
from app.mqtt_handler import mqtt_listener
from app.routers import auth, sensors, alerts, devices
from app.websocket_manager import manager
from app.dependencies import get_current_user
from app.models.sensor import SensorReading
from app.models.alert import Alert, AlertStatus
from app.models.device import Device
from app.services.auth_service import decode_token

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
)
logger = logging.getLogger(__name__)


@asynccontextmanager
async def lifespan(app: FastAPI):
    logger.info("Starting up: creating database tables...")
    await create_tables()
    logger.info("Database ready. Starting MQTT listener...")
    mqtt_task = asyncio.create_task(mqtt_listener())
    logger.info(f"Server running on port {settings.BACKEND_PORT}")
    yield
    logger.info("Shutting down: cancelling MQTT listener...")
    mqtt_task.cancel()
    try:
        await mqtt_task
    except asyncio.CancelledError:
        pass
    logger.info("Shutdown complete")


app = FastAPI(
    title="Percobaan 9 - IoT WebServer MQTT API",
    description="Backend API for IoT sensor monitoring with MQTT, WebSocket, and ML anomaly detection",
    version="1.0.0",
    lifespan=lifespan,
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(auth.router, prefix="/api/auth", tags=["Authentication"])
app.include_router(sensors.router, prefix="/api/sensors", tags=["Sensors"])
app.include_router(alerts.router, prefix="/api/alerts", tags=["Alerts"])
app.include_router(devices.router, prefix="/api/devices", tags=["Devices"])


@app.get("/api/dashboard/summary")
async def dashboard_summary(
    db: AsyncSession = Depends(get_db),
    _=Depends(get_current_user),
):
    total_devices = await db.scalar(select(func.count(Device.id)))
    online_devices = await db.scalar(select(func.count(Device.id)).where(Device.status == "online"))
    active_alerts = await db.scalar(
        select(func.count(Alert.id)).where(Alert.status == AlertStatus.ACTIVE)
    )
    total_readings = await db.scalar(select(func.count(SensorReading.id)))

    latest_readings = await db.execute(
        select(SensorReading).order_by(desc(SensorReading.created_at)).limit(5)
    )
    readings = latest_readings.scalars().all()

    devices_result = await db.execute(select(Device).order_by(desc(Device.last_seen)).limit(10))
    device_list = devices_result.scalars().all()

    return {
        "total_devices": total_devices or 0,
        "online_devices": online_devices or 0,
        "active_alerts": active_alerts or 0,
        "total_readings": total_readings or 0,
        "latest_readings": [
            {
                "id": r.id,
                "device_id": r.device_id,
                "suhu": r.suhu,
                "kelembaban": r.kelembaban,
                "cahaya": r.cahaya,
                "tanah": r.tanah,
                "fan_state": r.fan_state,
                "pump_state": r.pump_state,
                "auto_mode": r.auto_mode,
                "created_at": r.created_at.isoformat(),
            }
            for r in readings
        ],
        "devices": [
            {
                "device_id": d.device_id,
                "name": d.name,
                "status": d.status,
                "last_seen": d.last_seen.isoformat() if d.last_seen else None,
                "auto_mode": d.auto_mode,
            }
            for d in device_list
        ],
    }


@app.websocket("/api/ws")
async def websocket_endpoint(websocket: WebSocket, token: str = Query(None)):
    if token is None:
        await websocket.close(code=1008, reason="Missing token")
        return
    try:
        decode_token(token)
    except ValueError:
        await websocket.close(code=1008, reason="Invalid token")
        return

    await manager.connect(websocket)
    try:
        while True:
            data = await websocket.receive_text()
            if data == "ping":
                await websocket.send_text("pong")
    except WebSocketDisconnect:
        manager.disconnect(websocket)
    except Exception as e:
        logger.error(f"WebSocket error: {e}")
        manager.disconnect(websocket)


@app.get("/health")
async def health():
    return {"status": "ok", "version": "1.0.0"}


if __name__ == "__main__":
    import uvicorn
    uvicorn.run("main:app", host="0.0.0.0", port=settings.BACKEND_PORT, reload=True)
