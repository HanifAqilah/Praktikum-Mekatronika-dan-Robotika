// ============================================================
//  percobaan2.ino – ESP32 HTTP DataLogger Client
//  Mengirim data sensor ke Node.js/SQLite server setiap 5 detik.
//  Menangani validasi error HTTP 422.
// ============================================================

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

// ── State ────────────────────────────────────────────────────
unsigned long lastPublish = 0;
int           postCount   = 0;
int           errorCount  = 0;

// ── WiFi helpers ─────────────────────────────────────────────
void connectWiFi() {
  Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > 15000) {
      Serial.println("\n[WiFi] Timeout – restarting...");
      ESP.restart();
    }
    delay(500);
    Serial.print('.');
  }
  Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
}

void ensureWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Reconnecting...");
    WiFi.disconnect();
    connectWiFi();
  }
}

// ── LED blink helper ──────────────────────────────────────────
void blinkLED(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    if (i < times - 1) delay(delayMs);
  }
}

// ── Simulated sensor readings ─────────────────────────────────
float readSuhu() {
  // Simulate 22–38 °C (slightly wider range than percobaan 1)
  return 22.0f + (random(0, 1600) / 100.0f);
}

float readKelembaban() {
  // Simulate 35–85 %
  return 35.0f + (random(0, 5000) / 100.0f);
}

int readCahaya() {
  // Simulate 12-bit ADC 0–4095
  return random(0, 4096);
}

// ── POST /api/sensor ─────────────────────────────────────────
void publishSensor() {
  float suhu       = readSuhu();
  float kelembaban = readKelembaban();
  int   cahaya     = readCahaya();

  // Build JSON
  StaticJsonDocument<192> doc;
  doc["suhu"]       = suhu;
  doc["kelembaban"] = kelembaban;
  doc["cahaya"]     = cahaya;
  doc["device_id"]  = DEVICE_ID;

  String body;
  serializeJson(doc, body);

  String url = String("http://") + SERVER_IP + ":" + SERVER_PORT + "/api/sensor";

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);

  int code = http.POST(body);

  if (code == 201) {
    postCount++;
    Serial.printf("[POST #%d] OK – suhu=%.2f°C  kel=%.2f%%  cahaya=%d\n",
                  postCount, suhu, kelembaban, cahaya);
    // Blink LED 2 times on success
    blinkLED(2, 100);

  } else if (code == 422) {
    errorCount++;
    // Validation error – print detail from server
    String resp = http.getString();
    StaticJsonDocument<256> errDoc;
    DeserializationError parseErr = deserializeJson(errDoc, resp);
    if (!parseErr) {
      const char* detail = errDoc["detail"] | "unknown";
      Serial.printf("[POST] Validation Error (422): %s\n", detail);
    } else {
      Serial.printf("[POST] Validation Error (422): %s\n", resp.c_str());
    }
    // Blink LED 5 times fast on error
    blinkLED(5, 80);

  } else {
    errorCount++;
    Serial.printf("[POST] HTTP Error – code=%d  url=%s\n", code, url.c_str());
    blinkLED(3, 200);
  }

  http.end();

  Serial.printf("[STATS] Total posted=%d  errors=%d  WiFi RSSI=%d dBm\n",
                postCount, errorCount, WiFi.RSSI());
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n╔══════════════════════════════════════╗");
  Serial.println("║  ESP32 DataLogger Client – P2        ║");
  Serial.println("╠══════════════════════════════════════╣");
  Serial.printf ("║  Device ID  : %-22s║\n", DEVICE_ID);
  Serial.printf ("║  Server     : %s:%-6d    ║\n", SERVER_IP, SERVER_PORT);
  Serial.printf ("║  Interval   : %-4d ms                ║\n", PUBLISH_INTERVAL);
  Serial.println("╚══════════════════════════════════════╝");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  connectWiFi();

  // Signal ready: 3 quick blinks
  blinkLED(3, 150);
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  ensureWiFi();

  unsigned long now = millis();
  if (now - lastPublish >= PUBLISH_INTERVAL) {
    lastPublish = now;
    publishSensor();
  }

  delay(10);
}
