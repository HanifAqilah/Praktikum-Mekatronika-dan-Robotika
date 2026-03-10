import logging
import json
from typing import TYPE_CHECKING
from fastapi import WebSocket

if TYPE_CHECKING:
    pass

logger = logging.getLogger(__name__)


class WebSocketManager:
    def __init__(self):
        self.active_connections: list[WebSocket] = []

    async def connect(self, websocket: WebSocket) -> None:
        await websocket.accept()
        self.active_connections.append(websocket)
        logger.info(f"WebSocket connected. Total connections: {len(self.active_connections)}")

    def disconnect(self, websocket: WebSocket) -> None:
        if websocket in self.active_connections:
            self.active_connections.remove(websocket)
        logger.info(f"WebSocket disconnected. Total connections: {len(self.active_connections)}")

    async def broadcast(self, data: dict) -> None:
        if not self.active_connections:
            return
        message = json.dumps(data, default=str)
        dead_connections = []
        for connection in self.active_connections:
            try:
                await connection.send_text(message)
            except Exception as e:
                logger.warning(f"Failed to send WS message: {e}")
                dead_connections.append(connection)
        for conn in dead_connections:
            self.disconnect(conn)

    async def send_personal_message(self, data: dict, websocket: WebSocket) -> None:
        try:
            await websocket.send_text(json.dumps(data, default=str))
        except Exception as e:
            logger.warning(f"Failed to send personal WS message: {e}")
            self.disconnect(websocket)

    async def broadcast_sensor(self, reading: dict) -> None:
        await self.broadcast({"type": "sensor", "data": reading})

    async def broadcast_alert(self, alert: dict) -> None:
        await self.broadcast({"type": "alert", "data": alert})

    async def broadcast_device_status(self, device: dict) -> None:
        await self.broadcast({"type": "device_status", "data": device})


manager = WebSocketManager()
