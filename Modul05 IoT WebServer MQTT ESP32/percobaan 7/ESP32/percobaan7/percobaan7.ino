/*
 * Percobaan 7 – IoT Dashboard: ESP32 Firmware
 * ============================================
 * Sensors:
 *   - DHT22  → GPIO4  (temperature + humidity)
 *   - LDR    → GPIO34 (light intensity, ADC)
 *   - Soil   → GPIO35 (soil moisture, ADC)
 *
 * Publishes JSON to MQTT topic "sensor/data" every 2 seconds.
 * Status LED (GPIO2):
 *   - Double-blink on successful publish
 *   - Fast continuous blink on MQTT error
 * Watchdog timer resets device if loop hangs > 30 s.
 *
 * Libraries required: PubSubClient, DHT, ArduinoJson
 * (see ESP32/libraries.txt)
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <esp_task_wdt.h>

#include "config.h"

// ─── Objects ─────────────────────────────────────────────────────────────────
WiFiClient   wifiClient;
PubSubClient mqttClient(wifiClient);
DHT          dht(PIN_DHT, DHT_TYPE);

// ─── State ───────────────────────────────────────────────────────────────────
unsigned long lastPublishMs   = 0;
unsigned long lastMqttRetryMs = 0;
bool          mqttError       = false;

// ─── Helpers ─────────────────────────────────────────────────────────────────
void ledBlink(int times, int onMs, int offMs) {
    for (int i = 0; i < times; i++) {
        digitalWrite(PIN_STATUS_LED, HIGH);
        delay(onMs);
        digitalWrite(PIN_STATUS_LED, LOW);
        if (i < times - 1) delay(offMs);
    }
}

int adcAverage(int pin) {
    long sum = 0;
    for (int i = 0; i < ADC_SAMPLES; i++) {
        sum += analogRead(pin);
        delayMicroseconds(200);
    }
    return (int)(sum / ADC_SAMPLES);
}

// ─── WiFi ────────────────────────────────────────────────────────────────────
void connectWiFi() {
    if (WiFi.status() == WL_CONNECTED) return;

    Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        esp_task_wdt_reset();
        if (millis() - start > WIFI_TIMEOUT_MS) {
            Serial.println("\n[WiFi] Timeout – restarting");
            ESP.restart();
        }
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\n[WiFi] Connected: %s\n", WiFi.localIP().toString().c_str());
}

// ─── MQTT Callback ───────────────────────────────────────────────────────────
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
    Serial.printf("[MQTT] Received [%s]: %s\n", topic, msg.c_str());
}

// ─── MQTT Connect ────────────────────────────────────────────────────────────
bool connectMqtt() {
    if (mqttClient.connected()) return true;

    unsigned long now = millis();
    if (now - lastMqttRetryMs < MQTT_RECONNECT_MS) return false;
    lastMqttRetryMs = now;

    Serial.printf("[MQTT] Connecting to %s:%d…\n", MQTT_SERVER, MQTT_PORT);

    bool ok;
    if (strlen(MQTT_USERNAME) > 0) {
        ok = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    } else {
        ok = mqttClient.connect(MQTT_CLIENT_ID);
    }

    if (ok) {
        Serial.println("[MQTT] Connected");
        mqttClient.subscribe(MQTT_TOPIC_ALERT);
        mqttError = false;
    } else {
        Serial.printf("[MQTT] Failed, state=%d\n", mqttClient.state());
        mqttError = true;
    }
    return ok;
}

// ─── Read & Publish ──────────────────────────────────────────────────────────
void readAndPublish() {
    // DHT22
    float suhu      = dht.readTemperature();
    float kelembaban = dht.readHumidity();

    if (isnan(suhu) || isnan(kelembaban)) {
        Serial.println("[DHT] Read failed – skipping publish");
        return;
    }

    // ADC sensors
    int cahaya = adcAverage(PIN_LDR);
    int tanah  = adcAverage(PIN_SOIL);

    // Build JSON
    StaticJsonDocument<256> doc;
    doc["suhu"]      = round(suhu * 10.0f) / 10.0f;
    doc["kelembaban"]= round(kelembaban * 10.0f) / 10.0f;
    doc["cahaya"]    = cahaya;
    doc["tanah"]     = tanah;
    doc["device"]    = DEVICE_ID;
    doc["ts"]        = (unsigned long)(millis() / 1000);  // relative seconds

    char buf[256];
    size_t len = serializeJson(doc, buf);

    if (mqttClient.publish(MQTT_TOPIC_DATA, buf, len)) {
        Serial.printf("[MQTT] Published: %s\n", buf);
        ledBlink(2, 80, 80);   // double-blink = success
    } else {
        Serial.println("[MQTT] Publish failed");
        mqttError = true;
    }
}

// ─── Setup ───────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Percobaan 7 – IoT Dashboard ===");

    pinMode(PIN_STATUS_LED, OUTPUT);
    digitalWrite(PIN_STATUS_LED, LOW);

    // Watchdog
    esp_task_wdt_init(WATCHDOG_TIMEOUT_S, true);
    esp_task_wdt_add(NULL);

    dht.begin();
    analogReadResolution(12);   // 0-4095
    analogSetAttenuation(ADC_11db);

    connectWiFi();

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(60);
    mqttClient.setBufferSize(512);

    connectMqtt();
    Serial.println("[Setup] Done");
}

// ─── Loop ────────────────────────────────────────────────────────────────────
void loop() {
    esp_task_wdt_reset();

    // Keep WiFi alive
    connectWiFi();

    // Keep MQTT alive
    if (!mqttClient.connected()) {
        mqttError = true;
        connectMqtt();
    }
    mqttClient.loop();

    // Error indication: fast blink if MQTT not connected
    if (mqttError && !mqttClient.connected()) {
        digitalWrite(PIN_STATUS_LED, (millis() / 150) % 2 == 0 ? HIGH : LOW);
    }

    // Publish at interval
    unsigned long now = millis();
    if (now - lastPublishMs >= PUBLISH_INTERVAL_MS) {
        lastPublishMs = now;
        if (mqttClient.connected()) {
            readAndPublish();
        }
    }
}
