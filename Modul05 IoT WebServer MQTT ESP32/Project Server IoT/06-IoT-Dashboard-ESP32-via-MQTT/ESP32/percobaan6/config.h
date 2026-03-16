// ============================================================
//  Percobaan 6 – ESP32 MQTT Configuration
//  File  : config.h
//  !! DO NOT commit real credentials to version control !!
// ============================================================
#ifndef CONFIG_H
#define CONFIG_H

// ── WiFi ─────────────────────────────────────────────────────
#define WIFI_SSID     "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ── MQTT Broker ───────────────────────────────────────────────
// Use the LAN IP of your computer running Mosquitto / Node-RED
#define MQTT_BROKER_HOST "192.168.1.100"
#define MQTT_BROKER_PORT  1883

// MQTT credentials (set to "" if broker has no authentication)
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""

// Unique client ID – change if running multiple ESP32 boards
#define MQTT_CLIENT_ID "esp32-percobaan6"

#endif // CONFIG_H
