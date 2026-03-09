-- Percobaan 8 - IoT Database Schema

CREATE TABLE IF NOT EXISTS sensor_logs (
  id          SERIAL PRIMARY KEY,
  device_id   VARCHAR(64)     NOT NULL DEFAULT 'esp32-p8',
  suhu        NUMERIC(6,2),
  kelembaban  NUMERIC(6,2),
  cahaya      INTEGER,
  tanah       INTEGER,
  raw_payload JSONB,
  created_at  TIMESTAMPTZ     NOT NULL DEFAULT NOW()
);

CREATE INDEX IF NOT EXISTS idx_sensor_logs_created_at
  ON sensor_logs (created_at DESC);

CREATE INDEX IF NOT EXISTS idx_sensor_logs_device_id
  ON sensor_logs (device_id, created_at DESC);

CREATE TABLE IF NOT EXISTS command_logs (
  id            SERIAL PRIMARY KEY,
  command_type  VARCHAR(64)   NOT NULL,
  target        VARCHAR(64)   NOT NULL,
  payload       JSONB         NOT NULL,
  status        VARCHAR(32)   NOT NULL DEFAULT 'sent',
  created_at    TIMESTAMPTZ   NOT NULL DEFAULT NOW()
);

CREATE INDEX IF NOT EXISTS idx_command_logs_created_at
  ON command_logs (created_at DESC);
