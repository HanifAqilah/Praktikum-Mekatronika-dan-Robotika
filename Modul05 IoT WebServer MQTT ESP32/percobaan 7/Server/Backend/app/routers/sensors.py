"""Sensors REST + SSE router."""

import asyncio
import json
from typing import AsyncGenerator, List, Optional

from fastapi import APIRouter, Depends, Query
from fastapi.responses import StreamingResponse
from sqlalchemy import select, desc
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.models import SensorReading
from app.schemas import SensorReadingOut, AggregatedReading
from app.services.aggregation import (
    get_last_60min_per_minute,
    get_last_24h_per_hour,
    get_today_stats,
)
from app.mqtt_handler import latest_readings

router = APIRouter()


@router.get("/sensors/latest", response_model=List[SensorReadingOut])
async def get_latest_readings(
    device: Optional[str] = Query(None),
    limit: int = Query(20, le=200),
    db: AsyncSession = Depends(get_db),
):
    """Return the most recent sensor readings."""
    query = select(SensorReading).order_by(desc(SensorReading.created_at)).limit(limit)
    if device:
        query = query.where(SensorReading.device_id == device)
    result = await db.execute(query)
    return result.scalars().all()


@router.get("/sensors/aggregate/minutes", response_model=List[AggregatedReading])
async def aggregate_per_minute(
    device: Optional[str] = Query(None),
    db: AsyncSession = Depends(get_db),
):
    """Aggregated per-minute data for the last 60 minutes."""
    return await get_last_60min_per_minute(db, device)


@router.get("/sensors/aggregate/hours", response_model=List[AggregatedReading])
async def aggregate_per_hour(
    device: Optional[str] = Query(None),
    db: AsyncSession = Depends(get_db),
):
    """Aggregated per-hour data for the last 24 hours."""
    return await get_last_24h_per_hour(db, device)


@router.get("/sensors/stats/today")
async def today_stats(
    device: Optional[str] = Query(None),
    db: AsyncSession = Depends(get_db),
):
    """Min/max/avg statistics for today."""
    return await get_today_stats(db, device)


async def _sse_event_generator() -> AsyncGenerator[str, None]:
    """Yield SSE events with latest sensor data every 2 seconds."""
    while True:
        data = {
            "devices": list(latest_readings.values()),
            "count": len(latest_readings),
        }
        yield f"data: {json.dumps(data)}\n\n"
        await asyncio.sleep(2)


@router.get("/stream")
async def sensor_stream():
    """
    Server-Sent Events endpoint.
    Connect with EventSource('/api/stream') in the browser.
    """
    return StreamingResponse(
        _sse_event_generator(),
        media_type="text/event-stream",
        headers={
            "Cache-Control": "no-cache",
            "X-Accel-Buffering": "no",
        },
    )
