#ifndef CONFIG_H
#define CONFIG_H

// WiFi credentials
#define WIFI_SSID       "YourWiFiSSID"
#define WIFI_PASSWORD   "YourWiFiPassword"

// Backend server (no auth needed for sensor data submission)
#define SERVER_HOST     "192.168.1.100"
#define SERVER_PORT     8004
#define API_ENDPOINT    "/api/sensors/data"

// Device identity
#define DEVICE_ID       "esp32-04"

// Sensor simulation ranges
#define SUHU_MIN        20.0f
#define SUHU_MAX        40.0f
#define KELEMBABAN_MIN  30.0f
#define KELEMBABAN_MAX  90.0f
#define CAHAYA_MIN      0
#define CAHAYA_MAX      4095

// Timing
#define POST_INTERVAL_MS  3000

// Serial baud rate
#define SERIAL_BAUD     115200

#endif // CONFIG_H
