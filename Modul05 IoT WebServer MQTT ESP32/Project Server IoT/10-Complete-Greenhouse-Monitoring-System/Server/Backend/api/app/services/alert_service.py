import datetime
from typing import List, Optional

from sqlalchemy import select, update, delete
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import Alert
from app.schemas import AlertCreate, AlertOut

THRESHOLDS: dict = {
    "suhu":      {"high": 35.0, "low": 15.0},
    "kelembaban": {"high": 90.0, "low": 40.0},
    "cahaya":    {"high": 10000.0, "low": 100.0},
    "tanah":     {"high": 80.0, "low": 20.0},
}


async def create_alert(db: AsyncSession, payload: AlertCreate) -> Alert:
    alert = Alert(**payload.model_dump())
    db.add(alert)
    await db.commit()
    await db.refresh(alert)
    return alert


async def list_alerts(
    db: AsyncSession,
    status: Optional[str] = None,
    device_id: Optional[str] = None,
    limit: int = 50,
    offset: int = 0,
) -> List[Alert]:
    query = select(Alert)
    if status:
        query = query.where(Alert.status == status)
    if device_id:
        query = query.where(Alert.device_id == device_id)
    query = query.order_by(Alert.created_at.desc()).limit(limit).offset(offset)
    result = await db.execute(query)
    return result.scalars().all()


async def acknowledge_alert(db: AsyncSession, alert_id: int) -> Optional[Alert]:
    now = datetime.datetime.now(datetime.timezone.utc)
    await db.execute(
        update(Alert)
        .where(Alert.id == alert_id)
        .values(status="acknowledged", acknowledged_at=now)
    )
    await db.commit()
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    return result.scalar_one_or_none()


async def bulk_acknowledge(db: AsyncSession, alert_ids: List[int]) -> int:
    now = datetime.datetime.now(datetime.timezone.utc)
    result = await db.execute(
        update(Alert)
        .where(Alert.id.in_(alert_ids))
        .values(status="acknowledged", acknowledged_at=now)
    )
    await db.commit()
    return result.rowcount


async def resolve_alert(db: AsyncSession, alert_id: int) -> Optional[Alert]:
    now = datetime.datetime.now(datetime.timezone.utc)
    await db.execute(
        update(Alert)
        .where(Alert.id == alert_id)
        .values(status="resolved", resolved_at=now)
    )
    await db.commit()
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    return result.scalar_one_or_none()


async def delete_alert(db: AsyncSession, alert_id: int) -> bool:
    result = await db.execute(delete(Alert).where(Alert.id == alert_id))
    await db.commit()
    return result.rowcount > 0


async def check_thresholds(
    db: AsyncSession,
    device_id: str,
    reading: dict,
) -> List[Alert]:
    created: List[Alert] = []
    for field, limits in THRESHOLDS.items():
        value = reading.get(field)
        if value is None:
            continue
        for condition, threshold in [("high", limits["high"]), ("low", limits["low"])]:
            violated = (condition == "high" and value > threshold) or (
                condition == "low" and value < threshold
            )
            if not violated:
                continue
            severity = "critical" if abs(value - threshold) / threshold > 0.2 else "warning"
            alert = await create_alert(
                db,
                AlertCreate(
                    device_id=device_id,
                    field=field,
                    condition=condition,
                    value=value,
                    threshold=threshold,
                    severity=severity,
                ),
            )
            created.append(alert)
    return created
