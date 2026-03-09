from sqlalchemy import String, Float, DateTime, Integer, ForeignKey, Enum as SAEnum
from sqlalchemy.orm import Mapped, mapped_column, relationship
from datetime import datetime, timezone
import enum
from app.database import Base


class AlertStatus(str, enum.Enum):
    ACTIVE = "active"
    ACKNOWLEDGED = "acknowledged"
    RESOLVED = "resolved"


class AlertType(str, enum.Enum):
    SUHU_HIGH = "suhu_high"
    KELEMBABAN_LOW = "kelembaban_low"
    CAHAYA_LOW = "cahaya_low"
    TANAH_LOW = "tanah_low"
    ANOMALY = "anomaly"
    DEVICE_OFFLINE = "device_offline"


class Alert(Base):
    __tablename__ = "alerts"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, index=True)
    device_id: Mapped[str] = mapped_column(String(50), ForeignKey("devices.device_id"), index=True)
    sensor_reading_id: Mapped[int | None] = mapped_column(
        Integer, ForeignKey("sensor_readings.id"), nullable=True
    )
    alert_type: Mapped[str] = mapped_column(
        SAEnum(AlertType, name="alert_type_enum"), nullable=False
    )
    message: Mapped[str] = mapped_column(String(500), nullable=False)
    threshold_value: Mapped[float | None] = mapped_column(Float, nullable=True)
    actual_value: Mapped[float | None] = mapped_column(Float, nullable=True)
    status: Mapped[str] = mapped_column(
        SAEnum(AlertStatus, name="alert_status_enum"),
        default=AlertStatus.ACTIVE,
        index=True,
    )
    acknowledged_by: Mapped[str | None] = mapped_column(String(100), nullable=True)
    created_at: Mapped[datetime] = mapped_column(
        DateTime(timezone=True), default=lambda: datetime.now(timezone.utc), index=True
    )
    updated_at: Mapped[datetime] = mapped_column(
        DateTime(timezone=True),
        default=lambda: datetime.now(timezone.utc),
        onupdate=lambda: datetime.now(timezone.utc),
    )

    device = relationship("Device", back_populates="alerts")
    sensor_reading = relationship("SensorReading", back_populates="alerts")
