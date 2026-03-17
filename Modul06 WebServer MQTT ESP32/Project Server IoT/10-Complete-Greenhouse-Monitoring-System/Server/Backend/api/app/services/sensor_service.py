import csv
import io
import datetime
from typing import List, Optional

from sqlalchemy import select, func, delete
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import SensorReading
from app.schemas import SensorReadingOut, SensorHistoryPage, HourlyAggregate, DailyAggregate


async def get_latest(db: AsyncSession, device_id: str) -> Optional[SensorReading]:
    result = await db.execute(
        select(SensorReading)
        .where(SensorReading.device_id == device_id)
        .order_by(SensorReading.created_at.desc())
        .limit(1)
    )
    return result.scalar_one_or_none()


async def get_history(
    db: AsyncSession,
    device_id: str,
    cursor: Optional[int],
    limit: int = 20,
) -> SensorHistoryPage:
    query = select(SensorReading).where(SensorReading.device_id == device_id)
    if cursor is not None:
        query = query.where(SensorReading.id < cursor)
    query = query.order_by(SensorReading.created_at.desc()).limit(limit + 1)

    result = await db.execute(query)
    rows = result.scalars().all()

    next_cursor: Optional[int] = None
    if len(rows) > limit:
        next_cursor = rows[limit - 1].id
        rows = rows[:limit]

    count_result = await db.execute(
        select(func.count()).select_from(SensorReading)
        .where(SensorReading.device_id == device_id)
    )
    total = count_result.scalar_one()

    return SensorHistoryPage(
        data=[SensorReadingOut.model_validate(r) for r in rows],
        next_cursor=next_cursor,
        total=total,
    )


async def get_hourly_aggregates(
    db: AsyncSession,
    device_id: str,
    hours: int = 24,
) -> List[HourlyAggregate]:
    since = datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(hours=hours)
    trunc = func.date_trunc("hour", SensorReading.created_at).label("hour")
    result = await db.execute(
        select(
            trunc,
            func.avg(SensorReading.suhu).label("avg_suhu"),
            func.avg(SensorReading.kelembaban).label("avg_kelembaban"),
            func.avg(SensorReading.cahaya).label("avg_cahaya"),
            func.avg(SensorReading.tanah).label("avg_tanah"),
        )
        .where(SensorReading.device_id == device_id)
        .where(SensorReading.created_at >= since)
        .group_by(trunc)
        .order_by(trunc)
    )
    return [
        HourlyAggregate(
            hour=row.hour,
            avg_suhu=round(row.avg_suhu, 2) if row.avg_suhu is not None else None,
            avg_kelembaban=round(row.avg_kelembaban, 2) if row.avg_kelembaban is not None else None,
            avg_cahaya=round(row.avg_cahaya, 2) if row.avg_cahaya is not None else None,
            avg_tanah=round(row.avg_tanah, 2) if row.avg_tanah is not None else None,
        )
        for row in result.all()
    ]


async def get_daily_aggregates(
    db: AsyncSession,
    device_id: str,
    days: int = 30,
) -> List[DailyAggregate]:
    since = datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(days=days)
    trunc = func.date_trunc("day", SensorReading.created_at).label("day")
    result = await db.execute(
        select(
            trunc,
            func.avg(SensorReading.suhu).label("avg_suhu"),
            func.avg(SensorReading.kelembaban).label("avg_kelembaban"),
            func.avg(SensorReading.cahaya).label("avg_cahaya"),
            func.avg(SensorReading.tanah).label("avg_tanah"),
            func.min(SensorReading.suhu).label("min_suhu"),
            func.max(SensorReading.suhu).label("max_suhu"),
        )
        .where(SensorReading.device_id == device_id)
        .where(SensorReading.created_at >= since)
        .group_by(trunc)
        .order_by(trunc)
    )
    return [
        DailyAggregate(
            day=row.day.date(),
            avg_suhu=round(row.avg_suhu, 2) if row.avg_suhu is not None else None,
            avg_kelembaban=round(row.avg_kelembaban, 2) if row.avg_kelembaban is not None else None,
            avg_cahaya=round(row.avg_cahaya, 2) if row.avg_cahaya is not None else None,
            avg_tanah=round(row.avg_tanah, 2) if row.avg_tanah is not None else None,
            min_suhu=round(row.min_suhu, 2) if row.min_suhu is not None else None,
            max_suhu=round(row.max_suhu, 2) if row.max_suhu is not None else None,
        )
        for row in result.all()
    ]


async def export_csv(db: AsyncSession, device_id: str) -> str:
    result = await db.execute(
        select(SensorReading)
        .where(SensorReading.device_id == device_id)
        .order_by(SensorReading.created_at.asc())
    )
    rows = result.scalars().all()

    output = io.StringIO()
    writer = csv.writer(output)
    writer.writerow(["id", "device_id", "suhu", "kelembaban", "cahaya", "tanah", "created_at"])
    for r in rows:
        writer.writerow([r.id, r.device_id, r.suhu, r.kelembaban, r.cahaya, r.tanah, r.created_at.isoformat()])
    return output.getvalue()


async def purge_old_readings(db: AsyncSession, days: int = 30) -> int:
    cutoff = datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(days=days)
    result = await db.execute(
        delete(SensorReading).where(SensorReading.created_at < cutoff)
    )
    await db.commit()
    return result.rowcount
