import json
import logging
from typing import List

from fastapi import APIRouter, Depends, Query
from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.models import Actuator
from app.schemas import ActuatorCommand, ActuatorOut

logger = logging.getLogger(__name__)

router = APIRouter(prefix="/api/v1/actuators", tags=["actuators"])

_mqtt_client = None


def set_mqtt_client(client) -> None:
    global _mqtt_client
    _mqtt_client = client


@router.post("/command", response_model=ActuatorOut)
async def send_command(
    payload: ActuatorCommand,
    db: AsyncSession = Depends(get_db),
):
    result = await db.execute(
        select(Actuator)
        .where(Actuator.device_id == payload.device_id)
        .where(Actuator.name == payload.actuator)
    )
    actuator = result.scalar_one_or_none()

    new_state = payload.action == "on" if payload.action in ("on", "off") else (
        not actuator.state if actuator else False
    )

    if actuator is None:
        actuator = Actuator(
            device_id=payload.device_id,
            name=payload.actuator,
            state=new_state,
            mode=payload.mode or "manual",
            last_command=payload.action,
        )
        db.add(actuator)
    else:
        actuator.state = new_state
        actuator.mode = payload.mode or actuator.mode
        actuator.last_command = payload.action

    await db.commit()
    await db.refresh(actuator)

    if _mqtt_client:
        topic = f"greenhouse/{payload.device_id}/command/{payload.actuator}"
        msg = json.dumps({"action": payload.action, "mode": payload.mode})
        try:
            await _mqtt_client.publish(topic, msg)
        except Exception as exc:
            logger.warning("MQTT publish failed: %s", exc)

    return ActuatorOut.model_validate(actuator)


@router.get("/status", response_model=List[ActuatorOut])
async def get_status(
    device_id: str = Query("esp32-greenhouse"),
    db: AsyncSession = Depends(get_db),
):
    result = await db.execute(
        select(Actuator).where(Actuator.device_id == device_id)
    )
    return [ActuatorOut.model_validate(a) for a in result.scalars().all()]
