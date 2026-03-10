from sqlalchemy import String, Float, DateTime, Integer, ForeignKey
from sqlalchemy.orm import Mapped, mapped_column, relationship
from datetime import datetime, timezone
from app.database import Base


class SensorReading(Base):
    __tablename__ = "sensor_readings"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, index=True)
    device_id: Mapped[str] = mapped_column(String(50), ForeignKey("devices.device_id"), index=True)
    suhu: Mapped[float] = mapped_column(Float, nullable=False)
    kelembaban: Mapped[float] = mapped_column(Float, nullable=False)
    cahaya: Mapped[float] = mapped_column(Float, nullable=False)
    tanah: Mapped[float] = mapped_column(Float, nullable=False)
    fan_state: Mapped[bool] = mapped_column(default=False)
    pump_state: Mapped[bool] = mapped_column(default=False)
    auto_mode: Mapped[bool] = mapped_column(default=True)
    rssi: Mapped[int] = mapped_column(Integer, nullable=True)
    created_at: Mapped[datetime] = mapped_column(
        DateTime(timezone=True), default=lambda: datetime.now(timezone.utc), index=True
    )

    device = relationship("Device", back_populates="sensor_readings")
    alerts = relationship("Alert", back_populates="sensor_reading")
