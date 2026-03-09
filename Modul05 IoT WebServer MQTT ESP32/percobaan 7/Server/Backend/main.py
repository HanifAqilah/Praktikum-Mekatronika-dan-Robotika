"""
Percobaan 7 – IoT Dashboard with FastAPI + aiomqtt + SQLite
Entry point: uvicorn main:app --host 0.0.0.0 --port 8007
"""

from contextlib import asynccontextmanager
import asyncio

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from app.database import init_db
from app.mqtt_handler import start_mqtt_listener
from app.routers import sensors, alerts


@asynccontextmanager
async def lifespan(app: FastAPI):
    # Startup
    await init_db()
    mqtt_task = asyncio.create_task(start_mqtt_listener())
    yield
    # Shutdown
    mqtt_task.cancel()
    try:
        await mqtt_task
    except asyncio.CancelledError:
        pass


app = FastAPI(
    title="Percobaan 7 – IoT Sensor Dashboard",
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

app.include_router(sensors.router, prefix="/api")
app.include_router(alerts.router, prefix="/api")


@app.get("/api/health")
async def health():
    return {"status": "ok", "service": "percobaan7-backend"}
