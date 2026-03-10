"""Alert management router."""

from typing import List, Optional

from fastapi import APIRouter, Depends, Query, HTTPException
from sqlalchemy import select, desc, update
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.models import Alert
from app.schemas import AlertOut, AlertCreate

router = APIRouter()


@router.get("/alerts", response_model=List[AlertOut])
async def list_alerts(
    resolved: Optional[bool] = Query(None, description="Filter by resolved status"),
    device: Optional[str] = Query(None),
    limit: int = Query(50, le=500),
    db: AsyncSession = Depends(get_db),
):
    """List alerts, optionally filtered by resolved status and device."""
    query = select(Alert).order_by(desc(Alert.created_at)).limit(limit)
    if resolved is not None:
        query = query.where(Alert.resolved == resolved)
    if device:
        query = query.where(Alert.device_id == device)
    result = await db.execute(query)
    return result.scalars().all()


@router.get("/alerts/{alert_id}", response_model=AlertOut)
async def get_alert(alert_id: int, db: AsyncSession = Depends(get_db)):
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalars().first()
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    return alert


@router.post("/alerts", response_model=AlertOut, status_code=201)
async def create_alert(payload: AlertCreate, db: AsyncSession = Depends(get_db)):
    """Manually create an alert (e.g. from EdgePython)."""
    alert = Alert(**payload.model_dump())
    db.add(alert)
    await db.commit()
    await db.refresh(alert)
    return alert


@router.patch("/alerts/{alert_id}/resolve", response_model=AlertOut)
async def resolve_alert(alert_id: int, db: AsyncSession = Depends(get_db)):
    """Manually resolve an alert."""
    from datetime import datetime, timezone

    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalars().first()
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    alert.resolved = True
    alert.resolved_at = datetime.now(timezone.utc)
    await db.commit()
    await db.refresh(alert)
    return alert


@router.delete("/alerts/{alert_id}", status_code=204)
async def delete_alert(alert_id: int, db: AsyncSession = Depends(get_db)):
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalars().first()
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    await db.delete(alert)
    await db.commit()
