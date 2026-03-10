from fastapi import APIRouter, Depends, HTTPException, Query
from pydantic import BaseModel
from sqlalchemy import select, desc
from sqlalchemy.ext.asyncio import AsyncSession
from typing import Optional, Any
from datetime import datetime

from app.database import get_db
from app.dependencies import get_current_user
from app.models.device import Device
from app.models.user import User
from app.services.mqtt_service import send_threshold_update, send_mode_change, send_actuator_command

router = APIRouter()


class DeviceOut(BaseModel):
    id: int
    device_id: str
    name: str
    description: Optional[str] = None
    location: Optional[str] = None
    status: str
    last_seen: Optional[datetime] = None
    auto_mode: bool
    fan_state: bool
    pump_state: bool
    thresholds: dict[str, Any]
    firmware_version: Optional[str] = None
    created_at: datetime

    model_config = {"from_attributes": True}


class DeviceCreate(BaseModel):
    device_id: str
    name: str
    description: Optional[str] = None
    location: Optional[str] = None


class DeviceUpdate(BaseModel):
    name: Optional[str] = None
    description: Optional[str] = None
    location: Optional[str] = None
    thresholds: Optional[dict[str, Any]] = None


class ThresholdUpdate(BaseModel):
    suhu_max: Optional[float] = None
    kelembaban_min: Optional[float] = None
    cahaya_min: Optional[float] = None
    tanah_min: Optional[float] = None


class ActuatorCommand(BaseModel):
    actuator: str
    state: bool


@router.get("/", response_model=list[DeviceOut])
async def list_devices(
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Device).order_by(Device.name))
    return result.scalars().all()


@router.post("/", response_model=DeviceOut, status_code=201)
async def create_device(
    body: DeviceCreate,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Device).where(Device.device_id == body.device_id))
    if result.scalar_one_or_none():
        raise HTTPException(status_code=409, detail="Device ID already exists")
    device = Device(**body.model_dump())
    db.add(device)
    await db.commit()
    await db.refresh(device)
    return device


@router.get("/{device_id}", response_model=DeviceOut)
async def get_device(
    device_id: str,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Device).where(Device.device_id == device_id))
    device = result.scalar_one_or_none()
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")
    return device


@router.patch("/{device_id}", response_model=DeviceOut)
async def update_device(
    device_id: str,
    body: DeviceUpdate,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Device).where(Device.device_id == device_id))
    device = result.scalar_one_or_none()
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")

    for field, value in body.model_dump(exclude_none=True).items():
        setattr(device, field, value)

    await db.commit()
    await db.refresh(device)
    return device


@router.delete("/{device_id}", status_code=204)
async def delete_device(
    device_id: str,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Device).where(Device.device_id == device_id))
    device = result.scalar_one_or_none()
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")
    await db.delete(device)
    await db.commit()


@router.post("/{device_id}/thresholds", response_model=DeviceOut)
async def update_thresholds(
    device_id: str,
    body: ThresholdUpdate,
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Device).where(Device.device_id == device_id))
    device = result.scalar_one_or_none()
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")

    thresholds = dict(device.thresholds or {})
    updates = body.model_dump(exclude_none=True)
    thresholds.update(updates)
    device.thresholds = thresholds

    await db.commit()
    await db.refresh(device)

    await send_threshold_update(device_id, thresholds)
    return device


@router.post("/{device_id}/mode")
async def set_device_mode(
    device_id: str,
    auto_mode: bool = Query(...),
    db: AsyncSession = Depends(get_db),
    _: User = Depends(get_current_user),
):
    result = await db.execute(select(Device).where(Device.device_id == device_id))
    device = result.scalar_one_or_none()
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")
    device.auto_mode = auto_mode
    await db.commit()
    await send_mode_change(device_id, auto_mode)
    return {"device_id": device_id, "auto_mode": auto_mode}


@router.post("/{device_id}/actuator")
async def control_actuator(
    device_id: str,
    body: ActuatorCommand,
    _: User = Depends(get_current_user),
):
    success = await send_actuator_command(device_id, body.actuator, body.state)
    if not success:
        raise HTTPException(status_code=503, detail="MQTT broker not available")
    return {"device_id": device_id, "actuator": body.actuator, "state": body.state}
