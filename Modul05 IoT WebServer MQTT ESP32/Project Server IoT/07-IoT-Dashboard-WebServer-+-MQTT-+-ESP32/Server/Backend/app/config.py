"""Pydantic-settings config loaded from .env"""

from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    model_config = SettingsConfigDict(env_file=".env", env_file_encoding="utf-8", extra="ignore")

    MQTT_HOST: str = "localhost"
    MQTT_PORT: int = 1883
    MQTT_USERNAME: str = ""
    MQTT_PASSWORD: str = ""
    MQTT_TOPIC_DATA: str = "sensor/data"
    MQTT_TOPIC_ALERT: str = "sensor/alert"
    MQTT_TOPIC_AGG: str = "sensor/agg"

    DATABASE_URL: str = "sqlite+aiosqlite:///./sensor_data.db"

    HOST: str = "0.0.0.0"
    PORT: int = 8007

    # Alert thresholds
    SUHU_MAX: float = 35.0
    SUHU_MIN: float = 10.0
    KELEMBABAN_MAX: float = 90.0
    KELEMBABAN_MIN: float = 20.0
    CAHAYA_MAX: int = 4000
    TANAH_MAX: int = 3000
    TANAH_MIN: int = 500


settings = Settings()
