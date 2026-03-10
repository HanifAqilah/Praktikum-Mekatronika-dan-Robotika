import os
from datetime import datetime

import aiosqlite
from dotenv import load_dotenv
from fastapi import APIRouter, Depends, HTTPException, Request, status
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer
from pydantic import BaseModel

from app.routers.auth import verify_token

load_dotenv()

DB_FILE = os.getenv("DB_FILE", "sensor.db")

router = APIRouter(prefix="/sensors", tags=["sensors"])
security = HTTPBearer()


def get_current_user(credentials: HTTPAuthorizationCredentials = Depends(security)) -> dict:
    return verify_token(credentials.credentials)


class SensorPayload(BaseModel):
    device_id: str
    suhu: float
    kelembaban: float
    cahaya: int


@router.post("/data")
async def post_sensor_data(payload: SensorPayload, request: Request):
    async with aiosqlite.connect(DB_FILE) as db:
        cursor = await db.execute(
            "INSERT INTO sensor_readings (device_id, suhu, kelembaban, cahaya) VALUES (?, ?, ?, ?)",
            (payload.device_id, payload.suhu, payload.kelembaban, payload.cahaya),
        )
        await db.commit()
        row_id = cursor.lastrowid

    data = {
        "id": row_id,
        "device_id": payload.device_id,
        "suhu": payload.suhu,
        "kelembaban": payload.kelembaban,
        "cahaya": payload.cahaya,
        "created_at": datetime.utcnow().isoformat(),
    }

    # Broadcast to all WebSocket clients
    manager = request.app.state.ws_manager
    await manager.broadcast(data)

    return {"id": row_id, "message": "Data berhasil disimpan"}


@router.get("/latest")
async def get_latest(_user: dict = Depends(get_current_user)):
    async with aiosqlite.connect(DB_FILE) as db:
        db.row_factory = aiosqlite.Row
        cursor = await db.execute("""
            SELECT * FROM sensor_readings
            WHERE id IN (
                SELECT MAX(id) FROM sensor_readings GROUP BY device_id
            )
        """)
        rows = await cursor.fetchall()
    return [dict(row) for row in rows]


@router.get("/history")
async def get_history(limit: int = 50, _user: dict = Depends(get_current_user)):
    if limit > 200:
        limit = 200
    async with aiosqlite.connect(DB_FILE) as db:
        db.row_factory = aiosqlite.Row
        cursor = await db.execute(
            "SELECT * FROM sensor_readings ORDER BY id DESC LIMIT ?", (limit,)
        )
        rows = await cursor.fetchall()
    return [dict(row) for row in rows]


@router.get("/stats")
async def get_stats(_user: dict = Depends(get_current_user)):
    async with aiosqlite.connect(DB_FILE) as db:
        db.row_factory = aiosqlite.Row
        cursor = await db.execute("""
            SELECT
                ROUND(AVG(suhu), 2)        AS avg_suhu,
                ROUND(AVG(kelembaban), 2)  AS avg_kelembaban,
                ROUND(AVG(cahaya), 2)      AS avg_cahaya,
                ROUND(MAX(suhu), 2)        AS max_suhu,
                ROUND(MIN(suhu), 2)        AS min_suhu,
                COUNT(*)                   AS count
            FROM sensor_readings
            WHERE created_at >= datetime('now', '-24 hours')
        """)
        row = await cursor.fetchone()
    return dict(row) if row else {}
