from pydantic import BaseModel
from datetime import datetime
from typing import Optional
from app.models.alert import AlertStatus, AlertType


class AlertOut(BaseModel):
    id: int
    device_id: str
    sensor_reading_id: Optional[int] = None
    alert_type: AlertType
    message: str
    threshold_value: Optional[float] = None
    actual_value: Optional[float] = None
    status: AlertStatus
    acknowledged_by: Optional[str] = None
    created_at: datetime
    updated_at: datetime

    model_config = {"from_attributes": True}


class AlertCreate(BaseModel):
    device_id: str
    alert_type: AlertType
    message: str
    threshold_value: Optional[float] = None
    actual_value: Optional[float] = None
    sensor_reading_id: Optional[int] = None


class AlertAcknowledge(BaseModel):
    acknowledged_by: str


class AlertListParams(BaseModel):
    status: Optional[AlertStatus] = None
    device_id: Optional[str] = None
    limit: int = 100
    offset: int = 0
