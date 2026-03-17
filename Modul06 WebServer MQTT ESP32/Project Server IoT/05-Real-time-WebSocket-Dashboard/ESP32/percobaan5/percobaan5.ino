/*
 * Percobaan 5 — ESP32 IoT WebServer MQTT
 * Mekatronika dan Robotika
 *
 * Fungsi:
 *   - Kirim data sensor (suhu, kelembaban, cahaya) ke backend via HTTP POST
 *   - Poll LED state dari backend via HTTP GET, kontrol LED onboard
 *   - Auto-reconnect WiFi jika koneksi putus
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

// ─── Forward declarations ──────────────────────────────────────────────────
void connectWiFi();
void sendSensorData();
void pollLedState();
float readSuhu();
float readKelembaban();
int   readCahaya();

// ─── Timing ───────────────────────────────────────────────────────────────
unsigned long lastSensorSend = 0;
unsigned long lastLedPoll    = 0;

// ─── Setup ────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("\n========================================");
  Serial.println("  Percobaan 5 — Real-time Sensor Node  ");
  Serial.println("========================================");
  Serial.printf("  Device ID : %s\n", DEVICE_ID);
  Serial.printf("  Server    : http://%s:%d\n", SERVER_IP, SERVER_PORT);
  Serial.println("========================================\n");

  connectWiFi();
}

// ─── Loop ─────────────────────────────────────────────────────────────────
void loop() {
  // Auto-reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Koneksi terputus. Mencoba ulang...");
    connectWiFi();
  }

  unsigned long now = millis();

  if (now - lastSensorSend >= SENSOR_INTERVAL) {
    lastSensorSend = now;
    sendSensorData();
  }

  if (now - lastLedPoll >= LED_POLL_INTERVAL) {
    lastLedPoll = now;
    pollLedState();
  }

  delay(10);
}

// ─── WiFi ─────────────────────────────────────────────────────────────────
void connectWiFi() {
  Serial.printf("[WiFi] Menghubungkan ke SSID: %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < WIFI_MAX_RETRY) {
    delay(WIFI_RETRY_DELAY);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Terhubung!");
    Serial.printf("[WiFi] IP Address : %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WiFi] RSSI       : %d dBm\n\n", WiFi.RSSI());
  } else {
    Serial.println("\n[WiFi] GAGAL terhubung. Akan coba lagi di loop berikutnya.");
  }
}

// ─── Sensor readers ───────────────────────────────────────────────────────
/*
 * Gunakan sensor DHT22/DHT11 jika tersedia dengan library DHT.h.
 * Kode di bawah mensimulasikan nilai sensor untuk pengujian.
 * Ganti isi fungsi ini dengan pembacaan sensor nyata Anda.
 */

float readSuhu() {
  // Simulasi: suhu antara 25.0 – 35.0 °C
  return 25.0f + (random(0, 1000) / 100.0f);
}

float readKelembaban() {
  // Simulasi: kelembaban antara 40.0 – 80.0 %
  return 40.0f + (random(0, 4000) / 100.0f);
}

int readCahaya() {
  // Baca ADC dari pin sensor cahaya (0–4095)
  // Jika tidak ada sensor, kembalikan nilai simulasi
#ifdef LIGHT_SENSOR_PIN
  return analogRead(LIGHT_SENSOR_PIN);
#else
  return random(0, 4096);
#endif
}

// ─── HTTP POST sensor data ─────────────────────────────────────────────────
void sendSensorData() {
  if (WiFi.status() != WL_CONNECTED) return;

  float suhu       = readSuhu();
  float kelembaban = readKelembaban();
  int   cahaya     = readCahaya();

  // Build JSON payload
  StaticJsonDocument<256> doc;
  doc["device_id"]  = DEVICE_ID;
  doc["suhu"]       = round(suhu * 10.0f) / 10.0f;
  doc["kelembaban"] = round(kelembaban * 10.0f) / 10.0f;
  doc["cahaya"]     = cahaya;

  String payload;
  serializeJson(doc, payload);

  String url = String("http://") + SERVER_IP + ":" + SERVER_PORT + "/api/sensor";

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(payload);

  if (httpCode == HTTP_CODE_CREATED || httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.printf("[POST] Suhu=%.1f°C  Lembab=%.1f%%  Cahaya=%d  → HTTP %d\n",
                  suhu, kelembaban, cahaya, httpCode);
  } else {
    Serial.printf("[POST] GAGAL — HTTP %d  URL: %s\n", httpCode, url.c_str());
  }

  http.end();
}

// ─── HTTP GET LED state ────────────────────────────────────────────────────
void pollLedState() {
  if (WiFi.status() != WL_CONNECTED) return;

  String url = String("http://") + SERVER_IP + ":" + SERVER_PORT + "/api/led";

  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();

    StaticJsonDocument<64> doc;
    DeserializationError err = deserializeJson(doc, body);

    if (!err) {
      bool state = doc["state"].as<bool>();
      digitalWrite(LED_PIN, state ? HIGH : LOW);
      Serial.printf("[LED]  State dari server: %s\n", state ? "ON" : "OFF");
    }
  } else {
    Serial.printf("[LED]  GET gagal — HTTP %d\n", httpCode);
  }

  http.end();
}
