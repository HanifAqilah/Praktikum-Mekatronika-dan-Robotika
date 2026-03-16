// ============================================================
//  percobaan1.ino – ESP32 HTTP WebServer Client
//  Mengirim data sensor ke Node.js server via HTTP POST
//  dan membaca status LED via HTTP GET.
// ============================================================

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

// ── State ────────────────────────────────────────────────────
unsigned long lastPublish   = 0;
unsigned long lastLedCheck  = 0;
bool          currentLed    = false;

// ── WiFi helpers ─────────────────────────────────────────────
void connectWiFi() {
  Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - t > 15000) {
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
    Serial.println("[WiFi] Disconnected – reconnecting...");
    WiFi.disconnect();
    connectWiFi();
  }
}

// ── Simulated sensor readings ─────────────────────────────────
float readSuhu() {
  // Simulate temperature 25–35 °C
  return 25.0f + (random(0, 1000) / 100.0f);
}

float readKelembaban() {
  // Simulate humidity 40–80 %
  return 40.0f + (random(0, 4000) / 100.0f);
}

int readCahaya() {
  // Simulate light ADC value 0–4095
  return random(0, 4096);
}

// ── POST /api/sensor ─────────────────────────────────────────
void publishSensor() {
  float suhu       = readSuhu();
  float kelembaban = readKelembaban();
  int   cahaya     = readCahaya();

  String url = String("http://") + SERVER_IP + ":" + SERVER_PORT + "/api/sensor";

  // Build JSON payload
  StaticJsonDocument<128> doc;
  doc["suhu"]       = suhu;
  doc["kelembaban"] = kelembaban;
  doc["cahaya"]     = cahaya;

  String body;
  serializeJson(doc, body);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int code = http.POST(body);
  if (code == 200) {
    Serial.printf("[POST] OK – suhu=%.2f°C  kel=%.2f%%  cahaya=%d\n",
                  suhu, kelembaban, cahaya);
  } else {
    Serial.printf("[POST] Error – HTTP %d  url=%s\n", code, url.c_str());
  }
  http.end();
}

// ── GET /api/led – check & apply LED state ────────────────────
void syncLED() {
  String url = String("http://") + SERVER_IP + ":" + SERVER_PORT + "/api/led";

  // We use a simple GET trick: reading /api/data is safer (no side-effects)
  // Use /api/data to get the current ledState without toggling.
  String dataUrl = String("http://") + SERVER_IP + ":" + SERVER_PORT + "/api/data";

  HTTPClient http;
  http.begin(dataUrl);
  int code = http.GET();
  if (code == 200) {
    String payload = http.getString();
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (!err) {
      bool ledState = doc["ledState"].as<bool>();
      if (ledState != currentLed) {
        currentLed = ledState;
        digitalWrite(LED_PIN, currentLed ? HIGH : LOW);
        Serial.printf("[LED] State changed → %s\n", currentLed ? "ON" : "OFF");
      }
    }
  } else {
    Serial.printf("[LED] GET /api/data error – HTTP %d\n", code);
  }
  http.end();
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n╔══════════════════════════════════════╗");
  Serial.println("║  ESP32 WebServer Client – P1         ║");
  Serial.println("╚══════════════════════════════════════╝");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  connectWiFi();
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  ensureWiFi();

  unsigned long now = millis();

  // Publish sensor data every PUBLISH_INTERVAL ms
  if (now - lastPublish >= PUBLISH_INTERVAL) {
    lastPublish = now;
    publishSensor();
  }

  // Sync LED state every 1 second
  if (now - lastLedCheck >= 1000) {
    lastLedCheck = now;
    syncLED();
  }

  delay(10);
}
