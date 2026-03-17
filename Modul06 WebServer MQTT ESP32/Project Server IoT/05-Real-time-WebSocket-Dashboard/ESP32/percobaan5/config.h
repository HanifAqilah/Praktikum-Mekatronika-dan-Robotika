#ifndef CONFIG_H
#define CONFIG_H

// ─── WiFi ──────────────────────────────────────────────────────────────────
#define WIFI_SSID       "YourSSID"
#define WIFI_PASSWORD   "YourPassword"

// ─── Server ────────────────────────────────────────────────────────────────
#define SERVER_IP       "192.168.1.100"   // IP komputer yang menjalankan backend
#define SERVER_PORT     8005

// ─── Hardware ──────────────────────────────────────────────────────────────
#define LED_PIN         2                 // Built-in LED pada ESP32
#define LIGHT_SENSOR_PIN 34               // ADC pin untuk sensor cahaya (opsional)

// ─── Timing (ms) ───────────────────────────────────────────────────────────
#define SENSOR_INTERVAL   3000            // Kirim data sensor setiap 3 detik
#define LED_POLL_INTERVAL 2000            // Poll status LED setiap 2 detik
#define WIFI_RETRY_DELAY  500             // Delay antar retry WiFi (ms)
#define WIFI_MAX_RETRY    20              // Maksimum percobaan koneksi WiFi

// ─── Device ────────────────────────────────────────────────────────────────
#define DEVICE_ID       "esp32-05"

#endif // CONFIG_H
