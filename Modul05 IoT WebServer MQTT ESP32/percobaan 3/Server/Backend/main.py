import asyncio
import json
import os
from contextlib import asynccontextmanager
from datetime import datetime
from typing import AsyncGenerator

import aiosqlite
from dotenv import load_dotenv
from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse
from pydantic import BaseModel

load_dotenv()

PORT = int(os.getenv("PORT", 8003))
DB_FILE = os.getenv("DB_FILE", "sensor.db")
CORS_ORIGINS = os.getenv("CORS_ORIGINS", "http://localhost:9003").split(",")

latest_readings: dict = {}


async def init_db():
    async with aiosqlite.connect(DB_FILE) as db:
        await db.execute("""
            CREATE TABLE IF NOT EXISTS sensor_readings (
                id          INTEGER PRIMARY KEY AUTOINCREMENT,
                device_id   TEXT NOT NULL,
                suhu        REAL NOT NULL,
                kelembaban  REAL NOT NULL,
                cahaya      INTEGER NOT NULL,
                created_at  DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        """)
        await db.commit()


@asynccontextmanager
async def lifespan(app: FastAPI):
    await init_db()
    yield


app = FastAPI(title="Sensor SSE API", version="1.0.0", lifespan=lifespan)

app.add_middleware(
    CORSMiddleware,
    allow_origins=CORS_ORIGINS,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


class SensorPayload(BaseModel):
    device_id: str
    suhu: float
    kelembaban: float
    cahaya: int


@app.post("/api/sensor")
async def post_sensor(payload: SensorPayload):
    async with aiosqlite.connect(DB_FILE) as db:
        cursor = await db.execute(
            "INSERT INTO sensor_readings (device_id, suhu, kelembaban, cahaya) VALUES (?, ?, ?, ?)",
            (payload.device_id, payload.suhu, payload.kelembaban, payload.cahaya),
        )
        await db.commit()
        row_id = cursor.lastrowid

    latest_readings[payload.device_id] = {
        "id": row_id,
        "device_id": payload.device_id,
        "suhu": payload.suhu,
        "kelembaban": payload.kelembaban,
        "cahaya": payload.cahaya,
        "created_at": datetime.utcnow().isoformat(),
    }

    return {"id": row_id, "message": "Data berhasil disimpan"}


@app.get("/api/latest")
async def get_latest():
    return latest_readings


@app.get("/api/history")
async def get_history():
    async with aiosqlite.connect(DB_FILE) as db:
        db.row_factory = aiosqlite.Row
        cursor = await db.execute(
            "SELECT * FROM sensor_readings ORDER BY id DESC LIMIT 100"
        )
        rows = await cursor.fetchall()
    return [dict(row) for row in rows]


@app.get("/api/stats")
async def get_stats():
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


async def sse_generator() -> AsyncGenerator[str, None]:
    while True:
        data = json.dumps(latest_readings)
        yield f"data: {data}\n\n"
        await asyncio.sleep(2)


@app.get("/api/stream")
async def stream_sensor(request: Request):
    async def event_stream():
        async for chunk in sse_generator():
            if await request.is_disconnected():
                break
            yield chunk

    return StreamingResponse(
        event_stream(),
        media_type="text/event-stream",
        headers={
            "Cache-Control": "no-cache",
            "X-Accel-Buffering": "no",
            "Connection": "keep-alive",
        },
    )


if __name__ == "__main__":
    import uvicorn
    uvicorn.run("main:app", host="0.0.0.0", port=PORT, reload=True)
