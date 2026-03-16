/*
 * Percobaan 9 – IoT Smart Monitoring with MQTT, Auto/Manual Control
 * ESP32 Firmware v1.0.0
 *
 * Sensors : DHT22 (suhu + kelembaban), LDR (cahaya), Soil Moisture (tanah)
 * Actuators: Fan relay, Pump relay
 * Features : Auto mode (threshold-based), Manual mode (MQTT commanded),
 *            Mode toggle via physical button, persistent mode via Preferences,
 *            threshold updates via MQTT esp32/config
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <Preferences.h>

#include "config.h"

// ── Globals ──────────────────────────────────────────────────────────────
WiFiClient   wifiClient;
PubSubClient mqtt(wifiClient);
DHT          dht(DHT_PIN, DHT_TYPE);
Preferences  prefs;

// Runtime thresholds (loaded from Preferences, updated via MQTT)
float g_suhuMax       = DEFAULT_SUHU_MAX;
float g_kelembabanMin = DEFAULT_KELEMBABAN_MIN;
float g_cahayaMin     = DEFAULT_CAHAYA_MIN;
float g_tanahMin      = DEFAULT_TANAH_MIN;

bool  g_autoMode  = true;    // true = auto, false = manual
bool  g_fanState  = false;
bool  g_pumpState = false;

unsigned long g_lastPublish  = 0;
unsigned long g_lastReconnect = 0;

volatile bool g_btnPressed = false;  // set in ISR

// ── Helper: relay control (active-LOW relay board) ───────────────────────
void setFan(bool on) {
  g_fanState = on;
  digitalWrite(FAN_RELAY_PIN, on ? LOW : HIGH);
}

void setPump(bool on) {
  g_pumpState = on;
  digitalWrite(PUMP_RELAY_PIN, on ? LOW : HIGH);
}

// ── ISR for mode toggle button ────────────────────────────────────────────
void IRAM_ATTR onModeBtn() {
  g_btnPressed = true;
}

// ── Persistence helpers ──────────────────────────────────────────────────
void loadPreferences() {
  prefs.begin("p9", true);  // read-only
  g_autoMode      = prefs.getBool("autoMode", true);
  g_suhuMax       = prefs.getFloat("suhuMax",    DEFAULT_SUHU_MAX);
  g_kelembabanMin = prefs.getFloat("kelMin",     DEFAULT_KELEMBABAN_MIN);
  g_cahayaMin     = prefs.getFloat("cahayaMin",  DEFAULT_CAHAYA_MIN);
  g_tanahMin      = prefs.getFloat("tanahMin",   DEFAULT_TANAH_MIN);
  prefs.end();
}

void savePreferences() {
  prefs.begin("p9", false);  // read-write
  prefs.putBool("autoMode",  g_autoMode);
  prefs.putFloat("suhuMax",   g_suhuMax);
  prefs.putFloat("kelMin",    g_kelembabanMin);
  prefs.putFloat("cahayaMin", g_cahayaMin);
  prefs.putFloat("tanahMin",  g_tanahMin);
  prefs.end();
}

// ── WiFi ─────────────────────────────────────────────────────────────────
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());
}

// ── MQTT callback ─────────────────────────────────────────────────────────
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String topicStr(topic);
  String payloadStr;
  payloadStr.reserve(length);
  for (unsigned int i = 0; i < length; i++) payloadStr += (char)payload[i];

  Serial.print("MQTT RX [");
  Serial.print(topicStr);
  Serial.print("]: ");
  Serial.println(payloadStr);

  StaticJsonDocument<512> doc;
  if (deserializeJson(doc, payloadStr) != DeserializationError::Ok) {
    Serial.println("JSON parse error");
    return;
  }

  // ── esp32/config : threshold updates ──────────────────────────────────
  if (topicStr == TOPIC_CONFIG) {
    // Accept if no device_id filter, or if it matches ours
    const char* target = doc["device_id"] | "";
    if (strlen(target) > 0 && strcmp(target, DEVICE_ID) != 0) return;

    if (doc.containsKey("suhu_max"))       g_suhuMax       = doc["suhu_max"].as<float>();
    if (doc.containsKey("kelembaban_min")) g_kelembabanMin = doc["kelembaban_min"].as<float>();
    if (doc.containsKey("cahaya_min"))     g_cahayaMin     = doc["cahaya_min"].as<float>();
    if (doc.containsKey("tanah_min"))      g_tanahMin      = doc["tanah_min"].as<float>();

    savePreferences();
    Serial.println("Thresholds updated");
    return;
  }

  // ── esp32/command/<device_id> : control commands ──────────────────────
  String cmdTopic = String(TOPIC_CMD_PREFIX) + DEVICE_ID;
  if (topicStr == cmdTopic) {
    const char* cmd = doc["command"] | "";

    if (strcmp(cmd, "set_mode") == 0) {
      g_autoMode = doc["auto_mode"].as<bool>();
      savePreferences();
      Serial.print("Mode set to: ");
      Serial.println(g_autoMode ? "AUTO" : "MANUAL");
    }
    else if (strcmp(cmd, "set_actuator") == 0 && !g_autoMode) {
      const char* actuator = doc["actuator"] | "";
      bool state = doc["state"].as<bool>();
      if (strcmp(actuator, "fan") == 0)  setFan(state);
      if (strcmp(actuator, "pump") == 0) setPump(state);
    }
    return;
  }
}

// ── MQTT connect / reconnect ──────────────────────────────────────────────
void mqttConnect() {
  String clientId = String("esp32-") + DEVICE_ID + "-" + String(millis());
  bool connected;

  if (strlen(MQTT_USERNAME) > 0) {
    connected = mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD);
  } else {
    connected = mqtt.connect(clientId.c_str());
  }

  if (connected) {
    Serial.println("MQTT connected");
    mqtt.subscribe(TOPIC_CONFIG);
    String cmdTopic = String(TOPIC_CMD_PREFIX) + DEVICE_ID;
    mqtt.subscribe(cmdTopic.c_str());
    Serial.print("Subscribed to: ");
    Serial.print(TOPIC_CONFIG);
    Serial.print(", ");
    Serial.println(cmdTopic);
  } else {
    Serial.print("MQTT connect failed, rc=");
    Serial.println(mqtt.state());
  }
}

// ── Sensor read helpers ───────────────────────────────────────────────────
float readSuhu() {
  float v = dht.readTemperature();
  return isnan(v) ? -999.0f : v;
}

float readKelembaban() {
  float v = dht.readHumidity();
  return isnan(v) ? -999.0f : v;
}

float readCahaya() {
  // LDR: lower ADC value = more light; map to 0-1023 lux approximation
  int raw = analogRead(LDR_PIN);
  return map(raw, 0, 4095, 1023, 0);
}

float readTanah() {
  // Soil sensor: lower ADC = wet; map to 0-100%
  int raw = analogRead(SOIL_PIN);
  return map(raw, 4095, 1500, 0, 100);  // adjust 1500 for your sensor
}

// ── Auto-control logic ────────────────────────────────────────────────────
void autoControl(float suhu, float kelembaban) {
  if (!g_autoMode) return;
  setFan(suhu > g_suhuMax);
  setPump(kelembaban < g_kelembabanMin);
}

// ── Publish status to MQTT ────────────────────────────────────────────────
void publishStatus(float suhu, float kelembaban, float cahaya, float tanah) {
  StaticJsonDocument<384> doc;
  doc["device_id"]  = DEVICE_ID;
  doc["firmware"]   = FIRMWARE_VERSION;
  doc["suhu"]       = round(suhu * 10) / 10.0;
  doc["kelembaban"] = round(kelembaban * 10) / 10.0;
  doc["cahaya"]     = (int)cahaya;
  doc["tanah"]      = round(tanah * 10) / 10.0;
  doc["fan"]        = g_fanState;
  doc["pump"]       = g_pumpState;
  doc["auto_mode"]  = g_autoMode;
  doc["rssi"]       = WiFi.RSSI();

  char buf[384];
  serializeJson(doc, buf);
  mqtt.publish(TOPIC_STATUS, buf, true);

  Serial.print("Published: ");
  Serial.println(buf);
}

// ── setup ─────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(100);

  // GPIO init
  pinMode(FAN_RELAY_PIN,  OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(MODE_BTN_PIN,   INPUT_PULLUP);
  digitalWrite(FAN_RELAY_PIN,  HIGH);  // relay off (active-LOW)
  digitalWrite(PUMP_RELAY_PIN, HIGH);

  // Button ISR
  attachInterrupt(digitalPinToInterrupt(MODE_BTN_PIN), onModeBtn, FALLING);

  // Load saved settings
  loadPreferences();
  Serial.print("Auto mode: ");
  Serial.println(g_autoMode ? "YES" : "NO");

  // DHT
  dht.begin();

  // WiFi
  connectWiFi();

  // MQTT
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  mqtt.setBufferSize(512);
  mqttConnect();
}

// ── loop ──────────────────────────────────────────────────────────────────
void loop() {
  // WiFi watchdog
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost, reconnecting...");
    connectWiFi();
  }

  // MQTT reconnect with back-off
  if (!mqtt.connected()) {
    unsigned long now = millis();
    if (now - g_lastReconnect > 5000) {
      g_lastReconnect = now;
      mqttConnect();
    }
  }
  mqtt.loop();

  // Button: toggle auto/manual
  if (g_btnPressed) {
    g_btnPressed = false;
    g_autoMode = !g_autoMode;
    savePreferences();
    Serial.print("Button: mode toggled to ");
    Serial.println(g_autoMode ? "AUTO" : "MANUAL");
    if (!g_autoMode) {
      // Turn off actuators when entering manual mode
      setFan(false);
      setPump(false);
    }
  }

  // Periodic sensor read & publish
  unsigned long now = millis();
  if (now - g_lastPublish >= STATUS_INTERVAL_MS) {
    g_lastPublish = now;

    float suhu       = readSuhu();
    float kelembaban = readKelembaban();
    float cahaya     = readCahaya();
    float tanah      = readTanah();

    if (suhu == -999.0f || kelembaban == -999.0f) {
      Serial.println("DHT read failed, skipping publish");
      return;
    }

    autoControl(suhu, kelembaban);

    if (mqtt.connected()) {
      publishStatus(suhu, kelembaban, cahaya, tanah);
    }
  }
}
