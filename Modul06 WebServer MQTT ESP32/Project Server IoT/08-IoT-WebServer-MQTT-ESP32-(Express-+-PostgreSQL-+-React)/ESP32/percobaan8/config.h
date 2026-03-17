// config.h — Percobaan 8: IoT WebServer MQTT ESP32
// Edit sesuai jaringan dan broker MQTT Anda

#ifndef CONFIG_H
#define CONFIG_H

// ─── WiFi ─────────────────────────────────────────────────
#define WIFI_SSID       "NamaWiFiAnda"
#define WIFI_PASSWORD   "PasswordWiFiAnda"

// ─── MQTT Broker ──────────────────────────────────────────
#define MQTT_BROKER     "192.168.1.100"   // IP broker MQTT (misal: Mosquitto di laptop)
#define MQTT_PORT       1883
#define MQTT_USERNAME   ""                // Kosongkan jika tidak ada auth
#define MQTT_PASSWORD_V ""
#define MQTT_CLIENT_ID  "esp32-percobaan8"

// ─── MQTT Topics ──────────────────────────────────────────
#define TOPIC_SENSOR    "esp32/sensor/data"
#define TOPIC_CMD_LED1  "esp32/command/led1"
#define TOPIC_CMD_LED2  "esp32/command/led2"
#define TOPIC_CMD_LED3  "esp32/command/led3"
#define TOPIC_CMD_RELAY "esp32/command/relay"
#define TOPIC_CMD_ALL   "esp32/command/all"
#define TOPIC_ACK_BASE  "esp32/ack/"
#define TOPIC_CMD_SUB   "esp32/command/#"

// ─── GPIO Pin Definitions ─────────────────────────────────
#define PIN_LED1        2    // Biru
#define PIN_LED2        4    // Hijau
#define PIN_LED3        5    // Kuning
#define PIN_RELAY       15   // Relay simulation (LED merah cerah)

// ─── Sensor Pins (analog / simulated) ─────────────────────
// Jika menggunakan DHT22, ubah sesuai pinout
#define PIN_DHT         23   // DHT22 data pin (opsional)
#define USE_DHT         false  // set true jika DHT22 terpasang

// Simulasi sensor menggunakan nilai acak jika DHT tidak terpasang
#define SIMULATE_SENSORS true

// ─── Timing ───────────────────────────────────────────────
#define SENSOR_INTERVAL_MS  3000   // Kirim data sensor setiap 3 detik
#define MQTT_KEEPALIVE_S    60
#define RECONNECT_DELAY_MS  5000

// ─── Device ID ────────────────────────────────────────────
#define DEVICE_ID       "esp32-p8"

#endif // CONFIG_H
