"""
Aggregation service: computes per-minute, per-hour, and per-day statistics
from the sensor_readings table.
"""

from datetime import datetime, timezone, timedelta
from typing import List

from sqlalchemy import select, func, text
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import SensorReading
from app.schemas import AggregatedReading


async def get_last_60min_per_minute(
    session: AsyncSession, device_id: str | None = None
) -> List[AggregatedReading]:
    """Return one row per minute for the last 60 minutes."""
    since = datetime.now(timezone.utc) - timedelta(hours=1)

    # SQLite strftime groups by minute bucket
    bucket_expr = func.strftime("%Y-%m-%dT%H:%M:00", SensorReading.created_at)

    query = (
        select(
            bucket_expr.label("bucket"),
            func.avg(SensorReading.suhu).label("suhu_avg"),
            func.min(SensorReading.suhu).label("suhu_min"),
            func.max(SensorReading.suhu).label("suhu_max"),
            func.avg(SensorReading.kelembaban).label("kelembaban_avg"),
            func.avg(SensorReading.cahaya).label("cahaya_avg"),
            func.avg(SensorReading.tanah).label("tanah_avg"),
            func.count().label("count"),
        )
        .where(SensorReading.created_at >= since)
        .group_by("bucket")
        .order_by("bucket")
    )

    if device_id:
        query = query.where(SensorReading.device_id == device_id)

    result = await session.execute(query)
    rows = result.all()
    return [
        AggregatedReading(
            bucket=row.bucket or "",
            suhu_avg=round(row.suhu_avg or 0, 2),
            suhu_min=round(row.suhu_min or 0, 2),
            suhu_max=round(row.suhu_max or 0, 2),
            kelembaban_avg=round(row.kelembaban_avg or 0, 2),
            cahaya_avg=round(row.cahaya_avg or 0, 2),
            tanah_avg=round(row.tanah_avg or 0, 2),
            count=row.count,
        )
        for row in rows
    ]


async def get_last_24h_per_hour(
    session: AsyncSession, device_id: str | None = None
) -> List[AggregatedReading]:
    """Return one row per hour for the last 24 hours."""
    since = datetime.now(timezone.utc) - timedelta(hours=24)

    bucket_expr = func.strftime("%Y-%m-%dT%H:00:00", SensorReading.created_at)

    query = (
        select(
            bucket_expr.label("bucket"),
            func.avg(SensorReading.suhu).label("suhu_avg"),
            func.min(SensorReading.suhu).label("suhu_min"),
            func.max(SensorReading.suhu).label("suhu_max"),
            func.avg(SensorReading.kelembaban).label("kelembaban_avg"),
            func.avg(SensorReading.cahaya).label("cahaya_avg"),
            func.avg(SensorReading.tanah).label("tanah_avg"),
            func.count().label("count"),
        )
        .where(SensorReading.created_at >= since)
        .group_by("bucket")
        .order_by("bucket")
    )

    if device_id:
        query = query.where(SensorReading.device_id == device_id)

    result = await session.execute(query)
    rows = result.all()
    return [
        AggregatedReading(
            bucket=row.bucket or "",
            suhu_avg=round(row.suhu_avg or 0, 2),
            suhu_min=round(row.suhu_min or 0, 2),
            suhu_max=round(row.suhu_max or 0, 2),
            kelembaban_avg=round(row.kelembaban_avg or 0, 2),
            cahaya_avg=round(row.cahaya_avg or 0, 2),
            tanah_avg=round(row.tanah_avg or 0, 2),
            count=row.count,
        )
        for row in rows
    ]


async def get_today_stats(
    session: AsyncSession, device_id: str | None = None
) -> dict:
    """Return min/max/avg for today's data."""
    today_start = datetime.now(timezone.utc).replace(hour=0, minute=0, second=0, microsecond=0)

    query = select(
        func.avg(SensorReading.suhu).label("suhu_avg"),
        func.min(SensorReading.suhu).label("suhu_min"),
        func.max(SensorReading.suhu).label("suhu_max"),
        func.avg(SensorReading.kelembaban).label("kelembaban_avg"),
        func.min(SensorReading.kelembaban).label("kelembaban_min"),
        func.max(SensorReading.kelembaban).label("kelembaban_max"),
        func.avg(SensorReading.cahaya).label("cahaya_avg"),
        func.avg(SensorReading.tanah).label("tanah_avg"),
        func.count().label("total"),
    ).where(SensorReading.created_at >= today_start)

    if device_id:
        query = query.where(SensorReading.device_id == device_id)

    result = await session.execute(query)
    row = result.one()

    return {
        "suhu": {"avg": round(row.suhu_avg or 0, 2), "min": round(row.suhu_min or 0, 2), "max": round(row.suhu_max or 0, 2)},
        "kelembaban": {"avg": round(row.kelembaban_avg or 0, 2), "min": round(row.kelembaban_min or 0, 2), "max": round(row.kelembaban_max or 0, 2)},
        "cahaya": {"avg": round(row.cahaya_avg or 0, 2)},
        "tanah": {"avg": round(row.tanah_avg or 0, 2)},
        "total_readings": row.total,
    }
