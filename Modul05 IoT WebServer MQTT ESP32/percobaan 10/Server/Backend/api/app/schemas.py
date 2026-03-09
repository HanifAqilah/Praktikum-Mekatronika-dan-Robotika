import datetime
from typing import Optional, List
from pydantic import BaseModel, ConfigDict


# ── SensorReading ──────────────────────────────────────────────────────────────

class SensorReadingBase(BaseModel):
    device_id: str
    suhu: Optional[float] = None
    kelembaban: Optional[float] = None
    cahaya: Optional[float] = None
    tanah: Optional[float] = None


class SensorReadingCreate(SensorReadingBase):
    pass


class SensorReadingOut(SensorReadingBase):
    model_config = ConfigDict(from_attributes=True)
    id: int
    created_at: datetime.datetime


class SensorHistoryPage(BaseModel):
    data: List[SensorReadingOut]
    next_cursor: Optional[int] = None
    total: int


# ── Alert ──────────────────────────────────────────────────────────────────────

class AlertBase(BaseModel):
    device_id: str
    field: str
    condition: str
    value: float
    threshold: float
    severity: str = "warning"


class AlertCreate(AlertBase):
    pass


class AlertOut(AlertBase):
    model_config = ConfigDict(from_attributes=True)
    id: int
    status: str
    acknowledged_at: Optional[datetime.datetime] = None
    resolved_at: Optional[datetime.datetime] = None
    created_at: datetime.datetime


class AlertAcknowledge(BaseModel):
    alert_ids: List[int]


# ── Actuator ───────────────────────────────────────────────────────────────────

class ActuatorBase(BaseModel):
    device_id: str
    name: str
    state: bool = False
    mode: str = "manual"


class ActuatorOut(ActuatorBase):
    model_config = ConfigDict(from_attributes=True)
    id: int
    last_command: Optional[str] = None
    created_at: datetime.datetime


class ActuatorCommand(BaseModel):
    device_id: str
    actuator: str
    action: str  # "on" | "off" | "toggle"
    mode: Optional[str] = "manual"


# ── User / Auth ────────────────────────────────────────────────────────────────

class UserCreate(BaseModel):
    username: str
    password: str


class UserOut(BaseModel):
    model_config = ConfigDict(from_attributes=True)
    id: int
    username: str
    created_at: datetime.datetime


class Token(BaseModel):
    access_token: str
    refresh_token: str
    token_type: str = "bearer"


class TokenRefresh(BaseModel):
    refresh_token: str


# ── Analytics ─────────────────────────────────────────────────────────────────

class HourlyAggregate(BaseModel):
    hour: datetime.datetime
    avg_suhu: Optional[float] = None
    avg_kelembaban: Optional[float] = None
    avg_cahaya: Optional[float] = None
    avg_tanah: Optional[float] = None


class DailyAggregate(BaseModel):
    day: datetime.date
    avg_suhu: Optional[float] = None
    avg_kelembaban: Optional[float] = None
    avg_cahaya: Optional[float] = None
    avg_tanah: Optional[float] = None
    min_suhu: Optional[float] = None
    max_suhu: Optional[float] = None


class DashboardSummary(BaseModel):
    latest: Optional[SensorReadingOut] = None
    active_alerts: int
    total_readings_today: int
    actuators: List[ActuatorOut]


# ── Settings ──────────────────────────────────────────────────────────────────

class SettingOut(BaseModel):
    model_config = ConfigDict(from_attributes=True)
    id: int
    key: str
    value: str
    updated_at: datetime.datetime
