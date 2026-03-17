from pydantic_settings import BaseSettings
from pydantic import field_validator
from typing import List


class Settings(BaseSettings):
    DATABASE_URL: str = "postgresql+asyncpg://greenhouse:greenhouse_pass@postgres:5432/greenhouse_db"
    REDIS_URL: str = "redis://redis:6379/0"

    MQTT_BROKER: str = "mosquitto"
    MQTT_PORT: int = 1883
    MQTT_USERNAME: str = ""
    MQTT_PASSWORD: str = ""
    MQTT_CLIENT_ID: str = "fastapi-backend"

    SECRET_KEY: str = "change-me-in-production"
    ALGORITHM: str = "HS256"
    ACCESS_TOKEN_EXPIRE_MINUTES: int = 60
    REFRESH_TOKEN_EXPIRE_DAYS: int = 7

    APP_ENV: str = "production"
    ALLOWED_ORIGINS: str = "http://localhost,http://localhost:3000"

    @property
    def origins_list(self) -> List[str]:
        return [o.strip() for o in self.ALLOWED_ORIGINS.split(",")]

    class Config:
        env_file = ".env"


settings = Settings()
