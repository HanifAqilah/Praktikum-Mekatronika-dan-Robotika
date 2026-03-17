from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy import select, desc
from sqlalchemy.ext.asyncio import AsyncSession
from typing import Optional

from app.database import get_db
from app.dependencies import get_current_user
from app.models.alert import Alert, AlertStatus
from app.models.user import User
from app.schemas.alert import AlertOut, AlertAcknowledge
from app.services.alert_service import acknowledge_alert, resolve_alert

router = APIRouter()


@router.get("/", response_model=list[AlertOut])
async def list_alerts(
    status: Optional[AlertStatus] = Query(None),
    device_id: Optional[str] = Query(None),
    limit: int = Query(100, le=500),
    offset: int = Query(0, ge=0),
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    query = select(Alert).order_by(desc(Alert.created_at)).limit(limit).offset(offset)
    if status:
        query = query.where(Alert.status == status)
    if device_id:
        query = query.where(Alert.device_id == device_id)
    result = await db.execute(query)
    return result.scalars().all()


@router.get("/{alert_id}", response_model=AlertOut)
async def get_alert(
    alert_id: int,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalar_one_or_none()
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    return alert


@router.post("/{alert_id}/acknowledge", response_model=AlertOut)
async def ack_alert(
    alert_id: int,
    body: AlertAcknowledge,
    db: AsyncSession = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    alert = await acknowledge_alert(db, alert_id, body.acknowledged_by or current_user.username)
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    return alert


@router.post("/{alert_id}/resolve", response_model=AlertOut)
async def resolve_alert_endpoint(
    alert_id: int,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    alert = await resolve_alert(db, alert_id)
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    return alert


@router.delete("/{alert_id}", status_code=204)
async def delete_alert(
    alert_id: int,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalar_one_or_none()
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    await db.delete(alert)
    await db.commit()
