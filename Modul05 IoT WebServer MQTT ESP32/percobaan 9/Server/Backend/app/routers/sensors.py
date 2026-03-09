import csv
import io
import logging
from datetime import datetime, timedelta, timezone
from functools import lru_cache
from typing import Optional

from fastapi import APIRouter, Depends, HTTPException, Query, WebSocket, WebSocketDisconnect
from fastapi.responses import StreamingResponse
from sqlalchemy import func, select, desc
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.dependencies import get_current_user
from app.models.sensor import SensorReading
from app.models.user import User
from app.schemas.sensor import SensorReadingOut, SensorStats
from app.websocket_manager import manager
from app.services.auth_service import decode_token

logger = logging.getLogger(__name__)
router = APIRouter()


@router.get("/", response_model=list[SensorReadingOut])
async def get_sensors(
    device_id: Optional[str] = Query(None),
    limit: int = Query(100, le=1000),
    offset: int = Query(0, ge=0),
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    query = select(SensorReading).order_by(desc(SensorReading.created_at)).limit(limit).offset(offset)
    if device_id:
        query = query.where(SensorReading.device_id == device_id)
    result = await db.execute(query)
    return result.scalars().all()


@router.get("/history", response_model=list[SensorReadingOut])
async def get_sensor_history(
    device_id: str = Query(...),
    hours: int = Query(24, ge=1, le=168),
    limit: int = Query(500, le=2000),
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    since = datetime.now(timezone.utc) - timedelta(hours=hours)
    query = (
        select(SensorReading)
        .where(SensorReading.device_id == device_id, SensorReading.created_at >= since)
        .order_by(desc(SensorReading.created_at))
        .limit(limit)
    )
    result = await db.execute(query)
    readings = result.scalars().all()
    return list(reversed(readings))


@router.get("/stats", response_model=SensorStats)
async def get_sensor_stats(
    device_id: str = Query(...),
    hours: int = Query(24, ge=1, le=168),
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    since = datetime.now(timezone.utc) - timedelta(hours=hours)
    result = await db.execute(
        select(
            func.avg(SensorReading.suhu).label("suhu_avg"),
            func.min(SensorReading.suhu).label("suhu_min"),
            func.max(SensorReading.suhu).label("suhu_max"),
            func.avg(SensorReading.kelembaban).label("kelembaban_avg"),
            func.min(SensorReading.kelembaban).label("kelembaban_min"),
            func.max(SensorReading.kelembaban).label("kelembaban_max"),
            func.avg(SensorReading.cahaya).label("cahaya_avg"),
            func.avg(SensorReading.tanah).label("tanah_avg"),
            func.count(SensorReading.id).label("total"),
        ).where(SensorReading.device_id == device_id, SensorReading.created_at >= since)
    )
    row = result.one()
    if row.total == 0:
        raise HTTPException(status_code=404, detail="No data found for this device")

    return SensorStats(
        device_id=device_id,
        suhu_avg=round(row.suhu_avg or 0, 2),
        suhu_min=round(row.suhu_min or 0, 2),
        suhu_max=round(row.suhu_max or 0, 2),
        kelembaban_avg=round(row.kelembaban_avg or 0, 2),
        kelembaban_min=round(row.kelembaban_min or 0, 2),
        kelembaban_max=round(row.kelembaban_max or 0, 2),
        cahaya_avg=round(row.cahaya_avg or 0, 2),
        tanah_avg=round(row.tanah_avg or 0, 2),
        total_readings=row.total,
        period_hours=hours,
    )


@router.get("/export/csv")
async def export_csv(
    device_id: str = Query(...),
    hours: int = Query(24, ge=1, le=720),
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    since = datetime.now(timezone.utc) - timedelta(hours=hours)
    result = await db.execute(
        select(SensorReading)
        .where(SensorReading.device_id == device_id, SensorReading.created_at >= since)
        .order_by(SensorReading.created_at)
    )
    readings = result.scalars().all()

    output = io.StringIO()
    writer = csv.writer(output)
    writer.writerow(
        ["id", "device_id", "suhu", "kelembaban", "cahaya", "tanah", "fan_state", "pump_state", "auto_mode", "rssi", "created_at"]
    )
    for r in readings:
        writer.writerow(
            [r.id, r.device_id, r.suhu, r.kelembaban, r.cahaya, r.tanah, r.fan_state, r.pump_state, r.auto_mode, r.rssi, r.created_at]
        )

    output.seek(0)
    filename = f"sensor_{device_id}_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
    return StreamingResponse(
        iter([output.getvalue()]),
        media_type="text/csv",
        headers={"Content-Disposition": f'attachment; filename="{filename}"'},
    )


@router.get("/latest", response_model=SensorReadingOut)
async def get_latest_reading(
    device_id: str = Query(...),
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(
        select(SensorReading)
        .where(SensorReading.device_id == device_id)
        .order_by(desc(SensorReading.created_at))
        .limit(1)
    )
    reading = result.scalar_one_or_none()
    if not reading:
        raise HTTPException(status_code=404, detail="No readings found for this device")
    return reading


@router.websocket("/ws")
async def websocket_sensor_feed(websocket: WebSocket, token: str = Query(None)):
    if token is None:
        await websocket.close(code=1008)
        return
    try:
        decode_token(token)
    except ValueError:
        await websocket.close(code=1008)
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
