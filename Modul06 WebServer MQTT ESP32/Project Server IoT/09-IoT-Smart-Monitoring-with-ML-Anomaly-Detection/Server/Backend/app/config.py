from functools import lru_cache
from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    model_config = SettingsConfigDict(env_file=".env", env_file_encoding="utf-8", extra="ignore")

    DATABASE_URL: str = "postgresql+asyncpg://iot_user:iot_password@localhost:5432/iot_db"
    MQTT_HOST: str = "localhost"
    MQTT_PORT: int = 1883
    MQTT_USERNAME: str = ""
    MQTT_PASSWORD: str = ""
    MQTT_CLIENT_ID: str = "percobaan9_server"

    SECRET_KEY: str = "change-this-to-a-long-random-secret-key-in-production"
    ALGORITHM: str = "HS256"
    ACCESS_TOKEN_EXPIRE_MINUTES: int = 30
    REFRESH_TOKEN_EXPIRE_DAYS: int = 7

    BACKEND_PORT: int = 8009

    SUHU_THRESHOLD: float = 30.0
    KELEMBABAN_THRESHOLD: float = 40.0
    CAHAYA_THRESHOLD: float = 500.0
    TANAH_THRESHOLD: float = 30.0


@lru_cache
def get_settings() -> Settings:
    return Settings()


settings = get_settings()
