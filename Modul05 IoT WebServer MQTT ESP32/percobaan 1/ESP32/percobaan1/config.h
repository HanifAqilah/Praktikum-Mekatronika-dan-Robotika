// ============================================================
//  config.h – Konfigurasi untuk percobaan1.ino
//  Sesuaikan nilai-nilai di bawah dengan jaringan dan
//  alamat IP laptop/server Anda.
// ============================================================
#pragma once

// WiFi credentials
#define WIFI_SSID     "NamaWiFiAnda"
#define WIFI_PASSWORD "PasswordWiFiAnda"

// IP laptop/server yang menjalankan Node.js
// Cek dengan: ipconfig (Windows) atau ip addr (Linux/Mac)
#define SERVER_IP   "192.168.1.100"
#define SERVER_PORT 3001

// GPIO pin untuk LED built-in ESP32
#define LED_PIN 2

// Interval pengiriman data sensor (milliseconds)
#define PUBLISH_INTERVAL 3000
