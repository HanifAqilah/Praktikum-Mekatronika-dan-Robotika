"""SQLAlchemy ORM models for sensor readings and alerts."""

import datetime

from sqlalchemy import Integer, Float, String, DateTime, Boolean, func
from sqlalchemy.orm import Mapped, mapped_column

from app.database import Base


class SensorReading(Base):
    __tablename__ = "sensor_readings"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, index=True)
    device_id: Mapped[str] = mapped_column(String(64), index=True)
    suhu: Mapped[float] = mapped_column(Float)
    kelembaban: Mapped[float] = mapped_column(Float)
    cahaya: Mapped[int] = mapped_column(Integer)
    tanah: Mapped[int] = mapped_column(Integer)
    created_at: Mapped[datetime.datetime] = mapped_column(
        DateTime(timezone=True),
        server_default=func.now(),
        index=True,
    )


class Alert(Base):
    __tablename__ = "alerts"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, index=True)
    device_id: Mapped[str] = mapped_column(String(64), index=True)
    field: Mapped[str] = mapped_column(String(32))
    message: Mapped[str] = mapped_column(String(255))
    value: Mapped[float] = mapped_column(Float)
    threshold: Mapped[float] = mapped_column(Float)
    resolved: Mapped[bool] = mapped_column(Boolean, default=False)
    created_at: Mapped[datetime.datetime] = mapped_column(
        DateTime(timezone=True), server_default=func.now()
    )
    resolved_at: Mapped[datetime.datetime | None] = mapped_column(
        DateTime(timezone=True), nullable=True
    )
