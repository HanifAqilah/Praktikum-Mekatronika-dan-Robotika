// ============================================================
//  Percobaan 6 – ESP32 IoT MQTT Client
//  File  : percobaan6.ino
//  Board : ESP32 Dev Module (any variant)
//  Libs  : WiFi (built-in), PubSubClient, ArduinoJson
// ============================================================
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

// ── Pin definitions ──────────────────────────────────────────
#define LED_PIN 2          // Built-in LED on most ESP32 boards

// ── MQTT Topics ──────────────────────────────────────────────
#define TOPIC_SUHU       "esp32/sensor/suhu"
#define TOPIC_KELEMBABAN "esp32/sensor/kelembaban"
#define TOPIC_CAHAYA     "esp32/sensor/cahaya"
#define TOPIC_STATUS     "esp32/status"
#define TOPIC_CMD_LED    "esp32/command/led"

// ── Publish interval ─────────────────────────────────────────
#define PUBLISH_INTERVAL_MS 3000UL

// ── Globals ──────────────────────────────────────────────────
WiFiClient   wifiClient;
PubSubClient mqttClient(wifiClient);

bool     ledState      = false;
uint32_t lastPublishMs = 0;

// ── Simulated sensor readings (replace with real sensors) ────
// If you have a DHT22 or BH1750, replace these functions.
float readSuhu() {
  // Simulate: 25–40 °C with slow drift
  static float base = 28.0f;
  base += (random(-10, 11) * 0.05f);
  if (base < 25.0f) base = 25.0f;
  if (base > 42.0f) base = 42.0f;
  return base;
}

float readKelembaban() {
  // Simulate: 40–90 %RH
  static float base = 60.0f;
  base += (random(-10, 11) * 0.1f);
  if (base < 40.0f) base = 40.0f;
  if (base > 90.0f) base = 90.0f;
  return base;
}

int readCahaya() {
  // Simulate: 0–1023 lux (analogue read or ADC)
  return (int)analogRead(34);   // GPIO34 (ADC1_CH6) – change to your LDR pin
}

// ── WiFi ─────────────────────────────────────────────────────
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.printf("[WiFi] Connecting to \"%s\"", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
    if (++attempts >= 40) {
      // Restart if unable to connect after 20 s
      Serial.println("\n[WiFi] Timeout – restarting…");
      ESP.restart();
    }
  }
  Serial.printf("\n[WiFi] Connected!  IP: %s  RSSI: %d dBm\n",
                WiFi.localIP().toString().c_str(), WiFi.RSSI());
}

// ── MQTT callback ─────────────────────────────────────────────
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  // Copy payload to a null-terminated string
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0';

  Serial.printf("[MQTT] Received  topic=%-30s  payload=%s\n", topic, msg);

  if (strcmp(topic, TOPIC_CMD_LED) == 0) {
    if (strcasecmp(msg, "ON") == 0) {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);
      Serial.println("[LED] Turned ON");
    } else if (strcasecmp(msg, "OFF") == 0) {
      ledState = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("[LED] Turned OFF");
    } else {
      Serial.printf("[LED] Unknown command: %s\n", msg);
    }
  }
}

// ── MQTT connect / reconnect ──────────────────────────────────
void connectMQTT() {
  if (mqttClient.connected()) return;

  mqttClient.setServer(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
  mqttClient.setCallback(onMqttMessage);
  mqttClient.setKeepAlive(30);
  mqttClient.setSocketTimeout(15);

  Serial.printf("[MQTT] Connecting to %s:%d as \"%s\"…\n",
                MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_CLIENT_ID);

  bool ok;
  const char* lwt_payload = "offline";

#if defined(MQTT_USERNAME) && defined(MQTT_PASSWORD) \
    && (sizeof(MQTT_USERNAME) > 1) && (sizeof(MQTT_PASSWORD) > 1)
  ok = mqttClient.connect(
         MQTT_CLIENT_ID,
         MQTT_USERNAME, MQTT_PASSWORD,
         TOPIC_STATUS, 1 /*QoS*/, true /*retain*/, lwt_payload);
#else
  ok = mqttClient.connect(
         MQTT_CLIENT_ID,
         nullptr, nullptr,
         TOPIC_STATUS, 1 /*QoS*/, true /*retain*/, lwt_payload);
#endif

  if (ok) {
    Serial.println("[MQTT] Connected!");
    mqttClient.subscribe(TOPIC_CMD_LED, 1);
    Serial.printf("[MQTT] Subscribed to %s\n", TOPIC_CMD_LED);

    // Announce online
    mqttClient.publish(TOPIC_STATUS,
                       "{\"state\":\"online\"}", true /*retain*/);
  } else {
    Serial.printf("[MQTT] Failed, rc=%d – retry in 5 s\n",
                  mqttClient.state());
    delay(5000);
  }
}

// ── Publish all sensors + status ─────────────────────────────
void publishData() {
  char buf[128];

  // Suhu
  float suhu = readSuhu();
  snprintf(buf, sizeof(buf), "{\"suhu\":%.2f,\"unit\":\"C\"}", suhu);
  mqttClient.publish(TOPIC_SUHU, buf);
  Serial.printf("[PUB] %s  →  %s\n", TOPIC_SUHU, buf);

  // Kelembaban
  float rh = readKelembaban();
  snprintf(buf, sizeof(buf), "{\"kelembaban\":%.2f,\"unit\":\"%%RH\"}", rh);
  mqttClient.publish(TOPIC_KELEMBABAN, buf);
  Serial.printf("[PUB] %s  →  %s\n", TOPIC_KELEMBABAN, buf);

  // Cahaya
  int lux = readCahaya();
  snprintf(buf, sizeof(buf), "{\"cahaya\":%d,\"unit\":\"lux\"}", lux);
  mqttClient.publish(TOPIC_CAHAYA, buf);
  Serial.printf("[PUB] %s  →  %s\n", TOPIC_CAHAYA, buf);

  // Status (JSON with IP, RSSI, uptime)
  StaticJsonDocument<200> doc;
  doc["state"]  = "online";
  doc["ip"]     = WiFi.localIP().toString();
  doc["rssi"]   = WiFi.RSSI();
  doc["uptime"] = millis() / 1000;
  doc["led"]    = ledState ? "ON" : "OFF";
  serializeJson(doc, buf, sizeof(buf));
  mqttClient.publish(TOPIC_STATUS, buf, true /*retain*/);
  Serial.printf("[PUB] %s  →  %s\n", TOPIC_STATUS, buf);
}

// ── setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Percobaan 6 – ESP32 MQTT IoT ===");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  randomSeed(esp_random());

  connectWiFi();
  connectMQTT();
}

// ── loop ──────────────────────────────────────────────────────
void loop() {
  // Ensure WiFi is alive
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Lost connection – reconnecting…");
    connectWiFi();
  }

  // Ensure MQTT is alive
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  mqttClient.loop();

  // Publish on interval
  uint32_t now = millis();
  if (now - lastPublishMs >= PUBLISH_INTERVAL_MS) {
    lastPublishMs = now;
    publishData();
  }
}
