"""Pydantic schemas for request/response validation."""

import datetime
from typing import Optional

from pydantic import BaseModel, Field


class SensorPayload(BaseModel):
    suhu: float = Field(..., description="Temperature in °C")
    kelembaban: float = Field(..., description="Humidity in %")
    cahaya: int = Field(..., description="Light intensity (ADC 0-4095)")
    tanah: int = Field(..., description="Soil moisture (ADC 0-4095)")
    device: str = Field(default="esp32-01", description="Device identifier")
    ts: Optional[int] = Field(default=None, description="Unix timestamp from device")


class SensorReadingOut(BaseModel):
    id: int
    device_id: str
    suhu: float
    kelembaban: float
    cahaya: int
    tanah: int
    created_at: datetime.datetime

    model_config = {"from_attributes": True}


class AggregatedReading(BaseModel):
    bucket: str
    suhu_avg: float
    suhu_min: float
    suhu_max: float
    kelembaban_avg: float
    cahaya_avg: float
    tanah_avg: float
    count: int


class AlertOut(BaseModel):
    id: int
    device_id: str
    field: str
    message: str
    value: float
    threshold: float
    resolved: bool
    created_at: datetime.datetime
    resolved_at: Optional[datetime.datetime]

    model_config = {"from_attributes": True}


class AlertCreate(BaseModel):
    device_id: str
    field: str
    message: str
    value: float
    threshold: float
