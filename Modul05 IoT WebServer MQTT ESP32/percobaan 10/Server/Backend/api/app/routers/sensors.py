from typing import Optional

from fastapi import APIRouter, Depends, Query
from fastapi.responses import StreamingResponse
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.schemas import SensorReadingOut, SensorHistoryPage
from app.services import cache_service, sensor_service
import io

router = APIRouter(prefix="/api/v1", tags=["sensors"])


@router.get("/sensors/realtime", response_model=SensorReadingOut)
async def get_realtime(
    device_id: str = Query("esp32-greenhouse"),
    db: AsyncSession = Depends(get_db),
):
    cached = await cache_service.get_realtime(f"sensor:{device_id}")
    if cached:
        return cached
    reading = await sensor_service.get_latest(db, device_id)
    if reading is None:
        from fastapi import HTTPException
        raise HTTPException(status_code=404, detail="No sensor data found")
    out = SensorReadingOut.model_validate(reading)
    await cache_service.set_realtime(f"sensor:{device_id}", out.model_dump())
    return out


@router.get("/sensors/history", response_model=SensorHistoryPage)
async def get_history(
    device_id: str = Query("esp32-greenhouse"),
    cursor: Optional[int] = Query(None),
    limit: int = Query(20, ge=1, le=100),
    db: AsyncSession = Depends(get_db),
):
    return await sensor_service.get_history(db, device_id, cursor, limit)


@router.get("/export/sensors")
async def export_sensors(
    device_id: str = Query("esp32-greenhouse"),
    db: AsyncSession = Depends(get_db),
):
    csv_data = await sensor_service.export_csv(db, device_id)
    return StreamingResponse(
        io.StringIO(csv_data),
        media_type="text/csv",
        headers={"Content-Disposition": f'attachment; filename="sensors_{device_id}.csv"'},
    )
