import asyncio
import json
import os
from contextlib import asynccontextmanager
from typing import Dict, List

import aiosqlite
from dotenv import load_dotenv
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from passlib.context import CryptContext

from app.routers import auth, sensors
from app.routers.auth import verify_token

load_dotenv()

PORT = int(os.getenv("PORT", 8004))
DB_FILE = os.getenv("DB_FILE", "sensor.db")

pwd_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

# ------------------------------------------------------------------
# WebSocket connection manager
# ------------------------------------------------------------------
class ConnectionManager:
    def __init__(self):
        self.active: List[WebSocket] = []

    async def connect(self, ws: WebSocket):
        await ws.accept()
        self.active.append(ws)

    def disconnect(self, ws: WebSocket):
        if ws in self.active:
            self.active.remove(ws)

    async def broadcast(self, data: dict):
        dead: List[WebSocket] = []
        for ws in list(self.active):
            try:
                await ws.send_text(json.dumps(data))
            except Exception:
                dead.append(ws)
        for ws in dead:
            self.disconnect(ws)


ws_manager = ConnectionManager()


async def init_db():
    async with aiosqlite.connect(DB_FILE) as db:
        await db.execute("""
            CREATE TABLE IF NOT EXISTS users (
                id              INTEGER PRIMARY KEY AUTOINCREMENT,
                username        TEXT    UNIQUE NOT NULL,
                hashed_password TEXT    NOT NULL,
                created_at      DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        """)
        await db.execute("""
            CREATE TABLE IF NOT EXISTS sensor_readings (
                id          INTEGER PRIMARY KEY AUTOINCREMENT,
                device_id   TEXT    NOT NULL,
                suhu        REAL    NOT NULL,
                kelembaban  REAL    NOT NULL,
                cahaya      INTEGER NOT NULL,
                created_at  DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        """)
        # Seed default admin user
        cursor = await db.execute(
            "SELECT id FROM users WHERE username = ?", ("admin",)
        )
        if not await cursor.fetchone():
            hashed = pwd_context.hash("admin123")
            await db.execute(
                "INSERT INTO users (username, hashed_password) VALUES (?, ?)",
                ("admin", hashed),
            )
        await db.commit()


@asynccontextmanager
async def lifespan(app: FastAPI):
    await init_db()
    yield


app = FastAPI(title="IoT JWT WebSocket API", version="1.0.0", lifespan=lifespan)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:9004"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(auth.router, prefix="/api")
app.include_router(sensors.router, prefix="/api")

# Expose manager so sensor router can broadcast
app.state.ws_manager = ws_manager


@app.websocket("/api/ws/{token}")
async def websocket_endpoint(websocket: WebSocket, token: str):
    try:
        verify_token(token)
    except Exception:
        await websocket.close(code=4001)
        return

    await ws_manager.connect(websocket)
    try:
        while True:
            await asyncio.sleep(30)
    except WebSocketDisconnect:
        ws_manager.disconnect(websocket)


if __name__ == "__main__":
    import uvicorn
    uvicorn.run("main:app", host="0.0.0.0", port=PORT, reload=True)
