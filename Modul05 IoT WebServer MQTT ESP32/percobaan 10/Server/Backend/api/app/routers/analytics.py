from typing import List

from fastapi import APIRouter, Depends, Query
from sqlalchemy import func, select
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.models import Alert, Actuator, SensorReading
from app.schemas import DailyAggregate, DashboardSummary, HourlyAggregate, SensorReadingOut, ActuatorOut
from app.services import sensor_service
import datetime

router = APIRouter(prefix="/api/v1", tags=["analytics"])


@router.get("/analytics/hourly", response_model=List[HourlyAggregate])
async def hourly(
    device_id: str = Query("esp32-greenhouse"),
    hours: int = Query(24, ge=1, le=168),
    db: AsyncSession = Depends(get_db),
):
    return await sensor_service.get_hourly_aggregates(db, device_id, hours)


@router.get("/analytics/daily", response_model=List[DailyAggregate])
async def daily(
    device_id: str = Query("esp32-greenhouse"),
    days: int = Query(30, ge=1, le=365),
    db: AsyncSession = Depends(get_db),
):
    return await sensor_service.get_daily_aggregates(db, device_id, days)


@router.get("/dashboard/summary", response_model=DashboardSummary)
async def dashboard_summary(
    device_id: str = Query("esp32-greenhouse"),
    db: AsyncSession = Depends(get_db),
):
    latest = await sensor_service.get_latest(db, device_id)

    active_count_result = await db.execute(
        select(func.count()).select_from(Alert)
        .where(Alert.device_id == device_id)
        .where(Alert.status == "active")
    )
    active_alerts = active_count_result.scalar_one()

    today = datetime.datetime.now(datetime.timezone.utc).replace(
        hour=0, minute=0, second=0, microsecond=0
    )
    readings_result = await db.execute(
        select(func.count()).select_from(SensorReading)
        .where(SensorReading.device_id == device_id)
        .where(SensorReading.created_at >= today)
    )
    total_readings_today = readings_result.scalar_one()

    actuators_result = await db.execute(
        select(Actuator).where(Actuator.device_id == device_id)
    )
    actuators = [ActuatorOut.model_validate(a) for a in actuators_result.scalars().all()]

    return DashboardSummary(
        latest=SensorReadingOut.model_validate(latest) if latest else None,
        active_alerts=active_alerts,
        total_readings_today=total_readings_today,
        actuators=actuators,
    )
