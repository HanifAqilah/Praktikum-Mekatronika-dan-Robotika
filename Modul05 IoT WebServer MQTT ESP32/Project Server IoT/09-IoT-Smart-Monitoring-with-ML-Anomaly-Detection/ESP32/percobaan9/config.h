#pragma once

// ── WiFi ──────────────────────────────────────────────────────────────────
#define WIFI_SSID        "YOUR_WIFI_SSID"
#define WIFI_PASSWORD    "YOUR_WIFI_PASSWORD"

// ── MQTT Broker ───────────────────────────────────────────────────────────
#define MQTT_SERVER      "192.168.1.100"   // change to your broker IP
#define MQTT_PORT        1883
#define MQTT_USERNAME    ""                // leave empty if no auth
#define MQTT_PASSWORD    ""

// ── Device Identity ───────────────────────────────────────────────────────
#define DEVICE_ID        "esp32-01"
#define FIRMWARE_VERSION "1.0.0"

// ── MQTT Topics ───────────────────────────────────────────────────────────
#define TOPIC_STATUS     "esp32/status"
#define TOPIC_CONFIG     "esp32/config"
#define TOPIC_ANOMALY    "esp32/anomaly"
#define TOPIC_CMD_PREFIX "esp32/command/"   // + DEVICE_ID

// ── Sensor Pins ───────────────────────────────────────────────────────────
#define DHT_PIN          4          // DHT22 data pin
#define DHT_TYPE         DHT22      // or DHT11
#define LDR_PIN          34         // analog LDR pin (ADC1)
#define SOIL_PIN         35         // analog soil moisture pin (ADC1)

// ── Actuator Pins ─────────────────────────────────────────────────────────
#define FAN_RELAY_PIN    26         // relay IN1 – active LOW
#define PUMP_RELAY_PIN   27         // relay IN2 – active LOW

// ── Button Pin ────────────────────────────────────────────────────────────
#define MODE_BTN_PIN     0          // BOOT button – press to toggle auto/manual

// ── Default Thresholds (overridable via MQTT esp32/config) ────────────────
#define DEFAULT_SUHU_MAX          30.0f
#define DEFAULT_KELEMBABAN_MIN    40.0f
#define DEFAULT_CAHAYA_MIN        500.0f
#define DEFAULT_TANAH_MIN         30.0f

// ── Publish interval ──────────────────────────────────────────────────────
#define STATUS_INTERVAL_MS        5000     // 5 seconds
