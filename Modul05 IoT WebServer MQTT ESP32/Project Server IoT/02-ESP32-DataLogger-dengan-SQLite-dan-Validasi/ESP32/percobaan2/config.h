// ============================================================
//  config.h – Konfigurasi untuk percobaan2.ino
//  Sesuaikan nilai-nilai di bawah dengan jaringan Anda.
// ============================================================
#pragma once

// WiFi credentials
#define WIFI_SSID     "NamaWiFiAnda"
#define WIFI_PASSWORD "PasswordWiFiAnda"

// IP laptop/server yang menjalankan Node.js backend
// Windows: ipconfig  |  Linux/Mac: ip addr atau ifconfig
#define SERVER_IP   "192.168.1.100"
#define SERVER_PORT 3002

// ID perangkat (dikirim ke server dalam payload JSON)
#define DEVICE_ID   "esp32-02"

// GPIO pin untuk LED built-in ESP32
#define LED_PIN 2

// Interval pengiriman data sensor (milliseconds)
#define PUBLISH_INTERVAL 5000
