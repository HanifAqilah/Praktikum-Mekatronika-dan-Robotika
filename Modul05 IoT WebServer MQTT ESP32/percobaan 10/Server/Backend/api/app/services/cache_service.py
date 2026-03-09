import json
import logging
from typing import Any, Optional

import redis.asyncio as aioredis
from app.config import settings

logger = logging.getLogger(__name__)

_redis: Optional[aioredis.Redis] = None

REALTIME_TTL = 5      # seconds – live sensor data
DEFAULT_TTL  = 60     # seconds – general cache


async def get_redis() -> aioredis.Redis:
    global _redis
    if _redis is None:
        _redis = aioredis.from_url(settings.REDIS_URL, decode_responses=True)
    return _redis


async def close_redis() -> None:
    global _redis
    if _redis:
        await _redis.aclose()
        _redis = None


# ── Realtime helpers (short TTL) ───────────────────────────────────────────────

async def get_realtime(key: str) -> Optional[Any]:
    r = await get_redis()
    try:
        raw = await r.get(f"rt:{key}")
        return json.loads(raw) if raw else None
    except Exception as exc:
        logger.warning("Redis get_realtime error: %s", exc)
        return None


async def set_realtime(key: str, value: Any) -> None:
    r = await get_redis()
    try:
        await r.set(f"rt:{key}", json.dumps(value, default=str), ex=REALTIME_TTL)
    except Exception as exc:
        logger.warning("Redis set_realtime error: %s", exc)


# ── General cache helpers ──────────────────────────────────────────────────────

async def get_cache(key: str) -> Optional[Any]:
    r = await get_redis()
    try:
        raw = await r.get(f"cache:{key}")
        return json.loads(raw) if raw else None
    except Exception as exc:
        logger.warning("Redis get_cache error: %s", exc)
        return None


async def set_cache(key: str, value: Any, ttl: int = DEFAULT_TTL) -> None:
    r = await get_redis()
    try:
        await r.set(f"cache:{key}", json.dumps(value, default=str), ex=ttl)
    except Exception as exc:
        logger.warning("Redis set_cache error: %s", exc)


async def invalidate(key: str) -> None:
    r = await get_redis()
    try:
        await r.delete(f"cache:{key}")
    except Exception as exc:
        logger.warning("Redis invalidate error: %s", exc)
