from pydantic import BaseModel
from datetime import datetime
from typing import Optional


class SensorReadingOut(BaseModel):
    id: int
    device_id: str
    suhu: float
    kelembaban: float
    cahaya: float
    tanah: float
    fan_state: bool
    pump_state: bool
    auto_mode: bool
    rssi: Optional[int] = None
    created_at: datetime

    model_config = {"from_attributes": True}


class SensorStats(BaseModel):
    device_id: str
    suhu_avg: float
    suhu_min: float
    suhu_max: float
    kelembaban_avg: float
    kelembaban_min: float
    kelembaban_max: float
    cahaya_avg: float
    tanah_avg: float
    total_readings: int
    period_hours: int


class SensorCreate(BaseModel):
    device_id: str
    suhu: float
    kelembaban: float
    cahaya: float
    tanah: float
    fan_state: bool = False
    pump_state: bool = False
    auto_mode: bool = True
    rssi: Optional[int] = None


class SensorHistoryParams(BaseModel):
    device_id: str
    hours: int = 24
    limit: int = 500
