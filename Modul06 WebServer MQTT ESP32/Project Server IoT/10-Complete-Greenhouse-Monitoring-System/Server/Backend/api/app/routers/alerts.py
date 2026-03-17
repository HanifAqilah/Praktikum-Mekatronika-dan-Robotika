from typing import List, Optional

from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.schemas import AlertAcknowledge, AlertCreate, AlertOut
from app.services import alert_service

router = APIRouter(prefix="/api/v1/alerts", tags=["alerts"])


@router.get("", response_model=List[AlertOut])
async def list_alerts(
    status: Optional[str] = Query(None),
    device_id: Optional[str] = Query(None),
    limit: int = Query(50, ge=1, le=200),
    offset: int = Query(0, ge=0),
    db: AsyncSession = Depends(get_db),
):
    return await alert_service.list_alerts(db, status, device_id, limit, offset)


@router.post("", response_model=AlertOut, status_code=201)
async def create_alert(payload: AlertCreate, db: AsyncSession = Depends(get_db)):
    return await alert_service.create_alert(db, payload)


@router.post("/acknowledge", response_model=dict)
async def bulk_acknowledge(payload: AlertAcknowledge, db: AsyncSession = Depends(get_db)):
    count = await alert_service.bulk_acknowledge(db, payload.alert_ids)
    return {"acknowledged": count}


@router.patch("/{alert_id}/acknowledge", response_model=AlertOut)
async def acknowledge(alert_id: int, db: AsyncSession = Depends(get_db)):
    alert = await alert_service.acknowledge_alert(db, alert_id)
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    return alert


@router.patch("/{alert_id}/resolve", response_model=AlertOut)
async def resolve(alert_id: int, db: AsyncSession = Depends(get_db)):
    alert = await alert_service.resolve_alert(db, alert_id)
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    return alert


@router.delete("/{alert_id}", status_code=204)
async def delete_alert(alert_id: int, db: AsyncSession = Depends(get_db)):
    deleted = await alert_service.delete_alert(db, alert_id)
    if not deleted:
        raise HTTPException(status_code=404, detail="Alert not found")
