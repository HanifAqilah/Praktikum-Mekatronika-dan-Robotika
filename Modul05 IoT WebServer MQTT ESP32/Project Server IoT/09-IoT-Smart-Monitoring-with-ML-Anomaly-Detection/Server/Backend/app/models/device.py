from sqlalchemy import String, Boolean, DateTime, Integer, Float, JSON
from sqlalchemy.orm import Mapped, mapped_column, relationship
from datetime import datetime, timezone
from typing import Any
from app.database import Base


class Device(Base):
    __tablename__ = "devices"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, index=True)
    device_id: Mapped[str] = mapped_column(String(50), unique=True, index=True, nullable=False)
    name: Mapped[str] = mapped_column(String(100), nullable=False)
    description: Mapped[str | None] = mapped_column(String(500), nullable=True)
    location: Mapped[str | None] = mapped_column(String(200), nullable=True)
    status: Mapped[str] = mapped_column(String(20), default="offline")
    last_seen: Mapped[datetime | None] = mapped_column(DateTime(timezone=True), nullable=True)
    auto_mode: Mapped[bool] = mapped_column(Boolean, default=True)
    fan_state: Mapped[bool] = mapped_column(Boolean, default=False)
    pump_state: Mapped[bool] = mapped_column(Boolean, default=False)
    thresholds: Mapped[dict[str, Any]] = mapped_column(
        JSON,
        default=lambda: {
            "suhu_max": 30.0,
            "kelembaban_min": 40.0,
            "cahaya_min": 500.0,
            "tanah_min": 30.0,
        },
    )
    firmware_version: Mapped[str | None] = mapped_column(String(20), nullable=True)
    created_at: Mapped[datetime] = mapped_column(
        DateTime(timezone=True), default=lambda: datetime.now(timezone.utc)
    )

    sensor_readings = relationship("SensorReading", back_populates="device")
    alerts = relationship("Alert", back_populates="device")
