// config.h – Percobaan 7: WiFi + MQTT + Sensor Configuration
// Copy this file and rename to config.h, then fill in your credentials.

#pragma once

// ─── WiFi ────────────────────────────────────────────────────────────────────
#define WIFI_SSID        "YourWiFiSSID"
#define WIFI_PASSWORD    "YourWiFiPassword"
#define WIFI_TIMEOUT_MS  15000   // 15 s connection timeout

// ─── MQTT ────────────────────────────────────────────────────────────────────
#define MQTT_SERVER      "192.168.1.100"  // IP of your MQTT broker
#define MQTT_PORT        1883
#define MQTT_USERNAME    ""               // leave empty if no auth
#define MQTT_PASSWORD    ""
#define MQTT_CLIENT_ID   "esp32-percobaan7"
#define MQTT_TOPIC_DATA  "sensor/data"
#define MQTT_TOPIC_ALERT "sensor/alert"

// ─── Device ──────────────────────────────────────────────────────────────────
#define DEVICE_ID        "esp32-01"

// ─── GPIO Pins ───────────────────────────────────────────────────────────────
#define PIN_DHT          4          // DHT22 data pin
#define DHT_TYPE         DHT22

#define PIN_LDR          34         // LDR voltage divider → ADC1_CH6
#define PIN_SOIL         35         // Soil moisture sensor → ADC1_CH7
#define PIN_STATUS_LED   2          // On-board blue LED

// ─── Timing ──────────────────────────────────────────────────────────────────
#define PUBLISH_INTERVAL_MS   2000  // Publish every 2 seconds
#define WATCHDOG_TIMEOUT_S    30    // WDT resets if loop hangs > 30 s
#define MQTT_RECONNECT_MS     5000  // Wait between MQTT reconnect attempts

// ─── ADC ─────────────────────────────────────────────────────────────────────
#define ADC_SAMPLES           10    // Oversample ADC for noise reduction
#define ADC_MAX               4095
