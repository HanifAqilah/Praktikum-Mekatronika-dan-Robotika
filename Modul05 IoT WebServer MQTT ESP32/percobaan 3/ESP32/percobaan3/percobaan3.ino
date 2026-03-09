#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

unsigned long lastPostTime = 0;

void connectWiFi() {
  Serial.printf("[WiFi] Menghubungkan ke SSID: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[WiFi] Terhubung! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[WiFi] Gagal terhubung. Restart...");
    ESP.restart();
  }
}

float simulateSuhu() {
  return SUHU_MIN + (random(0, 1000) / 1000.0f) * (SUHU_MAX - SUHU_MIN);
}

float simulateKelembaban() {
  return KELEMBABAN_MIN + (random(0, 1000) / 1000.0f) * (KELEMBABAN_MAX - KELEMBABAN_MIN);
}

int simulateCahaya() {
  return random(CAHAYA_MIN, CAHAYA_MAX + 1);
}

void postSensorData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HTTP] WiFi tidak terhubung, melewati pengiriman.");
    return;
  }

  float suhu       = simulateSuhu();
  float kelembaban = simulateKelembaban();
  int   cahaya     = simulateCahaya();

  StaticJsonDocument<256> doc;
  doc["device_id"]  = DEVICE_ID;
  doc["suhu"]       = suhu;
  doc["kelembaban"] = kelembaban;
  doc["cahaya"]     = cahaya;

  String payload;
  serializeJson(doc, payload);

  String url = String("http://") + SERVER_HOST + ":" + SERVER_PORT + API_ENDPOINT;

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.printf("[HTTP] POST %s  Status: %d\n", url.c_str(), httpCode);
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
      Serial.printf("       Suhu=%.2f°C  Kelembaban=%.2f%%  Cahaya=%d\n",
                    suhu, kelembaban, cahaya);
      Serial.printf("       Response: %s\n", http.getString().c_str());
    }
  } else {
    Serial.printf("[HTTP] Error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);
  Serial.println("\n=== Percobaan 3 – SSE Sensor Publisher ===");
  randomSeed(esp_random());
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Koneksi terputus. Menghubungkan ulang...");
    connectWiFi();
  }

  unsigned long now = millis();
  if (now - lastPostTime >= POST_INTERVAL_MS) {
    lastPostTime = now;
    postSensorData();
  }
}
