/*
 * Percobaan 8 — IoT WebServer MQTT ESP32
 * Praktikum Mekatronika dan Robotika
 *
 * Fitur:
 *  - 3 LED aktuator (GPIO2, GPIO4, GPIO5)
 *  - 1 Relay simulasi (GPIO15, LED merah)
 *  - Publish data sensor ke esp32/sensor/data setiap 3 detik
 *  - Subscribe esp32/command/# dan proses perintah led1/led2/led3/relay/all
 *  - Publish ACK ke esp32/ack/<aktuator> setelah eksekusi perintah
 *
 * Library yang dibutuhkan:
 *  - PubSubClient by Nick O'Leary
 *  - ArduinoJson by Benoit Blanchon
 *  - WiFi (bawaan ESP32)
 *
 * Pasang nilai SSID / broker di config.h
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

// ─── Objek WiFi & MQTT ────────────────────────────────────
WiFiClient   wifiClient;
PubSubClient mqttClient(wifiClient);

// ─── State aktuator ───────────────────────────────────────
bool stateLed1  = false;
bool stateLed2  = false;
bool stateLed3  = false;
bool stateRelay = false;

// ─── Timing ───────────────────────────────────────────────
unsigned long lastSensorPublish = 0;

// ─── Fungsi utilitas ──────────────────────────────────────
void setLed(uint8_t pin, bool state) {
  digitalWrite(pin, state ? HIGH : LOW);
}

void applyActuators() {
  setLed(PIN_LED1,  stateLed1);
  setLed(PIN_LED2,  stateLed2);
  setLed(PIN_LED3,  stateLed3);
  setLed(PIN_RELAY, stateRelay);
}

// Publish ACK ke broker
void publishAck(const char* actuator, bool success, const char* state) {
  char topic[64];
  snprintf(topic, sizeof(topic), "%s%s", TOPIC_ACK_BASE, actuator);

  StaticJsonDocument<128> doc;
  doc["actuator"] = actuator;
  doc["status"]   = success ? "ok" : "fail";
  doc["state"]    = state;
  doc["ts"]       = millis();

  char buf[128];
  serializeJson(doc, buf);
  mqttClient.publish(topic, buf, false);
  Serial.printf("[mqtt] ACK -> %s : %s\n", topic, buf);
}

// ─── Callback MQTT ────────────────────────────────────────
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String topicStr(topic);
  String msgStr;
  for (unsigned int i = 0; i < length; i++) msgStr += (char)payload[i];

  Serial.printf("[mqtt] Received [%s]: %s\n", topic, msgStr.c_str());

  // Parse JSON payload
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, msgStr);
  if (err) {
    Serial.printf("[mqtt] JSON parse error: %s\n", err.c_str());
    return;
  }

  const char* stateVal = doc["state"] | "";
  bool turnOn = (strcmp(stateVal, "on") == 0);
  bool isToggle = (strcmp(stateVal, "toggle") == 0);

  // ── led1 ──
  if (topicStr == TOPIC_CMD_LED1) {
    stateLed1 = isToggle ? !stateLed1 : turnOn;
    setLed(PIN_LED1, stateLed1);
    publishAck("led1", true, stateLed1 ? "on" : "off");
  }
  // ── led2 ──
  else if (topicStr == TOPIC_CMD_LED2) {
    stateLed2 = isToggle ? !stateLed2 : turnOn;
    setLed(PIN_LED2, stateLed2);
    publishAck("led2", true, stateLed2 ? "on" : "off");
  }
  // ── led3 ──
  else if (topicStr == TOPIC_CMD_LED3) {
    stateLed3 = isToggle ? !stateLed3 : turnOn;
    setLed(PIN_LED3, stateLed3);
    publishAck("led3", true, stateLed3 ? "on" : "off");
  }
  // ── relay ──
  else if (topicStr == TOPIC_CMD_RELAY) {
    stateRelay = isToggle ? !stateRelay : turnOn;
    setLed(PIN_RELAY, stateRelay);
    publishAck("relay", true, stateRelay ? "on" : "off");
  }
  // ── all ──
  else if (topicStr == TOPIC_CMD_ALL) {
    stateLed1  = turnOn;
    stateLed2  = turnOn;
    stateLed3  = turnOn;
    stateRelay = turnOn;
    applyActuators();
    publishAck("all", true, turnOn ? "on" : "off");
  }
  else {
    Serial.printf("[mqtt] Unknown topic: %s\n", topic);
  }
}

// ─── Koneksi WiFi ─────────────────────────────────────────
void connectWiFi() {
  Serial.printf("\n[wifi] Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[wifi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[wifi] Failed to connect. Will retry in loop.");
  }
}

// ─── Koneksi MQTT ─────────────────────────────────────────
bool connectMqtt() {
  Serial.printf("[mqtt] Connecting to %s:%d as %s\n", MQTT_BROKER, MQTT_PORT, MQTT_CLIENT_ID);

  bool connected;
  if (strlen(MQTT_USERNAME) > 0) {
    connected = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD_V);
  } else {
    connected = mqttClient.connect(MQTT_CLIENT_ID);
  }

  if (connected) {
    Serial.println("[mqtt] Connected!");
    mqttClient.subscribe(TOPIC_CMD_SUB, 1);
    Serial.printf("[mqtt] Subscribed to %s\n", TOPIC_CMD_SUB);

    // Publish pesan online ke broker
    StaticJsonDocument<128> doc;
    doc["device_id"] = DEVICE_ID;
    doc["status"]    = "online";
    doc["ip"]        = WiFi.localIP().toString();
    char buf[128];
    serializeJson(doc, buf);
    mqttClient.publish("esp32/status", buf, true);  // retain=true
    return true;
  } else {
    Serial.printf("[mqtt] Failed, rc=%d. Retry in %d ms\n", mqttClient.state(), RECONNECT_DELAY_MS);
    return false;
  }
}

// ─── Baca/simulasi sensor ─────────────────────────────────
struct SensorData {
  float suhu;
  float kelembaban;
  int   cahaya;
  int   tanah;
};

SensorData readSensors() {
  SensorData data;

#if SIMULATE_SENSORS
  // Simulasi nilai sensor dengan nilai acak yang realistis
  data.suhu       = 25.0f + (float)(random(-50, 100)) / 10.0f;  // 20–35 °C
  data.kelembaban = 60.0f + (float)(random(-200, 200)) / 10.0f; // 40–80%
  data.cahaya     = random(100, 1023);
  data.tanah      = random(200, 800);
#else
  // Jika USE_DHT = true, ganti dengan pembacaan DHT22 sesungguhnya
  // #include <DHT.h>
  // DHT dht(PIN_DHT, DHT22);
  // data.suhu       = dht.readTemperature();
  // data.kelembaban = dht.readHumidity();
  data.suhu       = 0;
  data.kelembaban = 0;
  data.cahaya     = analogRead(34);
  data.tanah      = analogRead(35);
#endif

  return data;
}

// ─── Publish data sensor ──────────────────────────────────
void publishSensorData() {
  SensorData s = readSensors();

  StaticJsonDocument<256> doc;
  doc["device_id"]  = DEVICE_ID;
  doc["suhu"]       = serialized(String(s.suhu, 2));
  doc["kelembaban"] = serialized(String(s.kelembaban, 2));
  doc["cahaya"]     = s.cahaya;
  doc["tanah"]      = s.tanah;
  doc["uptime_ms"]  = millis();

  char buf[256];
  serializeJson(doc, buf);

  bool ok = mqttClient.publish(TOPIC_SENSOR, buf, false);
  if (ok) {
    Serial.printf("[mqtt] Sensor published: suhu=%.1f kel=%.1f cahaya=%d tanah=%d\n",
                  s.suhu, s.kelembaban, s.cahaya, s.tanah);
  } else {
    Serial.println("[mqtt] Sensor publish FAILED");
  }
}

// ─── Setup ────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Percobaan 8 — IoT MQTT ESP32 ===");

  // Inisialisasi pin output
  pinMode(PIN_LED1,  OUTPUT);
  pinMode(PIN_LED2,  OUTPUT);
  pinMode(PIN_LED3,  OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  applyActuators();  // Semua OFF saat boot

  connectWiFi();

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(onMqttMessage);
  mqttClient.setKeepAlive(MQTT_KEEPALIVE_S);
  mqttClient.setBufferSize(512);

  connectMqtt();
}

// ─── Loop ─────────────────────────────────────────────────
void loop() {
  // Pastikan WiFi terhubung
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[wifi] Disconnected. Reconnecting...");
    connectWiFi();
    return;
  }

  // Pastikan MQTT terhubung
  if (!mqttClient.connected()) {
    Serial.println("[mqtt] Disconnected. Reconnecting...");
    connectMqtt();
    delay(RECONNECT_DELAY_MS);
    return;
  }

  mqttClient.loop();

  // Publish sensor setiap SENSOR_INTERVAL_MS
  unsigned long now = millis();
  if (now - lastSensorPublish >= SENSOR_INTERVAL_MS) {
    lastSensorPublish = now;
    publishSensorData();
  }
}
