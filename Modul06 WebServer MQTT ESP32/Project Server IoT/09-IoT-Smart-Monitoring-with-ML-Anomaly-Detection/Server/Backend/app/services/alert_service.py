import logging
from datetime import datetime, timezone
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select, update
from app.models.alert import Alert, AlertStatus, AlertType
from app.models.device import Device
from app.websocket_manager import manager

logger = logging.getLogger(__name__)


async def create_alert(
    db: AsyncSession,
    device_id: str,
    alert_type: AlertType,
    message: str,
    threshold_value: float | None = None,
    actual_value: float | None = None,
    sensor_reading_id: int | None = None,
) -> Alert:
    alert = Alert(
        device_id=device_id,
        alert_type=alert_type,
        message=message,
        threshold_value=threshold_value,
        actual_value=actual_value,
        sensor_reading_id=sensor_reading_id,
        status=AlertStatus.ACTIVE,
    )
    db.add(alert)
    await db.commit()
    await db.refresh(alert)

    await manager.broadcast_alert(
        {
            "id": alert.id,
            "device_id": alert.device_id,
            "alert_type": alert.alert_type,
            "message": alert.message,
            "threshold_value": alert.threshold_value,
            "actual_value": alert.actual_value,
            "status": alert.status,
            "created_at": alert.created_at.isoformat(),
        }
    )
    logger.info(f"Alert created: {alert_type} for device {device_id}")
    return alert


async def check_and_create_alerts(
    db: AsyncSession,
    device: Device,
    reading_id: int,
    suhu: float,
    kelembaban: float,
    cahaya: float,
    tanah: float,
) -> list[Alert]:
    thresholds = device.thresholds or {}
    suhu_max = thresholds.get("suhu_max", 30.0)
    kelembaban_min = thresholds.get("kelembaban_min", 40.0)
    cahaya_min = thresholds.get("cahaya_min", 500.0)
    tanah_min = thresholds.get("tanah_min", 30.0)

    created_alerts = []

    if suhu > suhu_max:
        alert = await create_alert(
            db,
            device.device_id,
            AlertType.SUHU_HIGH,
            f"Suhu tinggi: {suhu:.1f}°C melebihi batas {suhu_max}°C",
            threshold_value=suhu_max,
            actual_value=suhu,
            sensor_reading_id=reading_id,
        )
        created_alerts.append(alert)

    if kelembaban < kelembaban_min:
        alert = await create_alert(
            db,
            device.device_id,
            AlertType.KELEMBABAN_LOW,
            f"Kelembaban rendah: {kelembaban:.1f}% di bawah batas {kelembaban_min}%",
            threshold_value=kelembaban_min,
            actual_value=kelembaban,
            sensor_reading_id=reading_id,
        )
        created_alerts.append(alert)

    if cahaya < cahaya_min:
        alert = await create_alert(
            db,
            device.device_id,
            AlertType.CAHAYA_LOW,
            f"Cahaya rendah: {cahaya:.0f} lux di bawah batas {cahaya_min} lux",
            threshold_value=cahaya_min,
            actual_value=cahaya,
            sensor_reading_id=reading_id,
        )
        created_alerts.append(alert)

    if tanah < tanah_min:
        alert = await create_alert(
            db,
            device.device_id,
            AlertType.TANAH_LOW,
            f"Kelembaban tanah rendah: {tanah:.1f}% di bawah batas {tanah_min}%",
            threshold_value=tanah_min,
            actual_value=tanah,
            sensor_reading_id=reading_id,
        )
        created_alerts.append(alert)

    return created_alerts


async def resolve_alerts_for_device(db: AsyncSession, device_id: str) -> None:
    await db.execute(
        update(Alert)
        .where(Alert.device_id == device_id, Alert.status == AlertStatus.ACTIVE)
        .values(status=AlertStatus.RESOLVED, updated_at=datetime.now(timezone.utc))
    )
    await db.commit()


async def acknowledge_alert(db: AsyncSession, alert_id: int, username: str) -> Alert | None:
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalar_one_or_none()
    if not alert:
        return None
    alert.status = AlertStatus.ACKNOWLEDGED
    alert.acknowledged_by = username
    alert.updated_at = datetime.now(timezone.utc)
    await db.commit()
    await db.refresh(alert)
    return alert


async def resolve_alert(db: AsyncSession, alert_id: int) -> Alert | None:
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalar_one_or_none()
    if not alert:
        return None
    alert.status = AlertStatus.RESOLVED
    alert.updated_at = datetime.now(timezone.utc)
    await db.commit()
    await db.refresh(alert)
    return alert
