# Jobsheet Modul 6: WebServer MQTT ESP32
**Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi**  
**Dosen: Rofiq Cahyo Prayogo, S.T., M.T.**  
**Praktikum Mekatronika — 2 SKS | Durasi: 5 Jam 40 Menit**

---

## INFORMASI PRAKTIKUM

| Aspek             | Keterangan                                 |
|-------------------|--------------------------------------------|
| Modul             | 6 — WebServer MQTT ESP32                   |
| Durasi            | 5 jam 40 menit                             |
| Prasyarat         | Modul 1-5 sudah selesai                    |
| Hardware          | ESP32 DevKit, DHT22, LED, Resistor         |
| Software          | Arduino IDE 2.x, Mosquitto, Node-RED       |
| Protokol          | HTTP, MQTT, WebSocket, JSON                |

---

## TUJUAN PEMBELAJARAN

Setelah menyelesaikan praktikum ini, mahasiswa mampu:

1. **Memahami** konsep IoT, protokol MQTT, dan WebServer pada ESP32
2. **Menginstal** dan mengkonfigurasi seluruh software yang diperlukan
3. **Merangkai** hardware ESP32 dengan sensor DHT22 dan LED
4. **Memprogram** WebServer REST API pada ESP32 menggunakan AsyncWebServer
5. **Mengimplementasikan** MQTT Publisher dan Subscriber pada ESP32
6. **Mengintegrasikan** WebServer dan MQTT dalam satu sistem bersamaan
7. **Membangun** dashboard real-time menggunakan Node-RED
8. **Menerapkan** FreeRTOS untuk multitasking pada sistem IoT
9. **Melakukan** troubleshooting dan debugging sistem IoT

---

## ALAT DAN BAHAN

### Hardware

| No | Komponen             | Jumlah | Spesifikasi           |
|----|----------------------|--------|-----------------------|
| 1  | ESP32 DevKit V1      | 1 unit | 38 pin, USB-C/micro   |
| 2  | Kabel USB            | 1 buah | Sesuai port ESP32     |
| 3  | Sensor DHT22         | 1 unit | Suhu -40~80°C, ±0.5°C |
| 4  | LED Merah            | 1 buah | 5mm, 2V 20mA          |
| 5  | Resistor 220 Ω       | 1 buah | ±5%, ¼ Watt           |
| 6  | Resistor 10 kΩ       | 1 buah | Pull-up DHT22         |
| 7  | Breadboard           | 1 buah | 400/830 tie-point     |
| 8  | Kabel jumper M-M     | 10 buah | 20cm                 |
| 9  | Laptop/PC            | 1 unit | RAM min. 4GB          |
| 10 | Router WiFi          | 1 unit | 2.4 GHz b/g/n         |

### Software (Semua Gratis)

| No | Software              | Versi Min | Download                              |
|----|-----------------------|-----------|---------------------------------------|
| 1  | Arduino IDE           | 2.2.0     | arduino.cc/en/software                |
| 2  | ESP32 Board Package   | 2.0.14    | Via Board Manager                     |
| 3  | Mosquitto Broker      | 2.0.x     | mosquitto.org/download                |
| 4  | MQTT Explorer (GUI)   | 0.4.0     | mqtt-explorer.com                     |
| 5  | Node.js               | 18 LTS    | nodejs.org                            |
| 6  | Node-RED              | 3.x       | npm install -g node-red               |
| 7  | Postman               | Latest    | postman.com (untuk test API)          |
| 8  | Git (opsional)        | Latest    | git-scm.com                           |

---

## BAGIAN A: INSTALASI DAN KONFIGURASI SOFTWARE

### A.1 Instalasi Arduino IDE 2.x

**Langkah-langkah:**

1. Buka browser, akses `https://www.arduino.cc/en/software`
2. Pilih installer sesuai OS (Windows Installer / Linux AppImage)
3. Jalankan installer, ikuti wizard instalasi
4. Buka Arduino IDE setelah instalasi selesai

**Menambahkan Board ESP32:**

1. Klik menu **File → Preferences** (Ctrl+,)
2. Pada kolom "Additional Boards Manager URLs", masukkan:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Klik **OK**
4. Klik menu **Tools → Board → Boards Manager**
5. Ketik `esp32` pada kolom pencarian
6. Pilih **esp32 by Espressif Systems**, klik **Install**
7. Tunggu proses download dan instalasi (~200 MB)

**Verifikasi Instalasi Board:**
1. Hubungkan ESP32 ke laptop via USB
2. Klik **Tools → Board → esp32 → ESP32 Dev Module**
3. Klik **Tools → Port** → pilih port COM yang muncul
   - Windows: `COM3` atau `COM4` dll.
   - Linux: `/dev/ttyUSB0` atau `/dev/ttyACM0`
   - macOS: `/dev/cu.usbserial-xxx`

**Jika Port Tidak Muncul — Instalasi Driver:**
- Windows: Download driver CH340 dari `wch.cn` atau CP2102 dari Silicon Labs
- Linux: `sudo usermod -a -G dialout $USER` lalu logout-login
- macOS: Driver biasanya sudah terinstal otomatis

### A.2 Instalasi Library Arduino

Buka **Sketch → Include Library → Manage Libraries** (Ctrl+Shift+I)

Instalasi library berikut satu per satu:

| Library               | Versi    | Fungsi                      |
|-----------------------|----------|-----------------------------|
| AsyncTCP              | 1.1.1    | TCP async ESP32             |
| ESPAsyncWebServer     | 1.2.3    | Web server asinkron         |
| PubSubClient          | 2.8.0    | MQTT client                 |
| ArduinoJson           | 6.21.x   | JSON parsing/serializing    |
| DHT sensor library    | 1.4.x    | Baca DHT11/DHT22            |
| Adafruit Unified Sensor| 1.1.x   | Dependensi DHT              |
| WiFiManager           | 2.0.x    | Portal config WiFi          |

> **Catatan:** ESPAsyncWebServer tidak tersedia di Library Manager. Install manual:
> 1. Download ZIP dari: `github.com/me-no-dev/ESPAsyncWebServer`
> 2. **Sketch → Include Library → Add .ZIP Library**
> 3. Lakukan hal yang sama untuk AsyncTCP

### A.3 Instalasi Mosquitto MQTT Broker

**Windows:**
```
1. Download installer dari mosquitto.org/download
2. Jalankan installer, pilih "Install as Service"
3. Edit file C:\Program Files\mosquitto\mosquitto.conf:
   Tambahkan baris:
     listener 1883
     allow_anonymous true
4. Restart service: Services → Mosquitto Broker → Restart
```

**Linux (Ubuntu/Debian):**
```bash
# Install
sudo apt update && sudo apt install -y mosquitto mosquitto-clients

# Edit konfigurasi
sudo nano /etc/mosquitto/conf.d/default.conf
```
Isi file konfigurasi:
```
listener 1883
allow_anonymous true
```
```bash
# Restart dan enable
sudo systemctl restart mosquitto
sudo systemctl enable mosquitto

# Cek status
sudo systemctl status mosquitto
```

**Verifikasi Mosquitto Berjalan:**

Buka **dua terminal** terpisah:

Terminal 1 (Subscriber):
```bash
mosquitto_sub -h localhost -t "test/topic" -v
```

Terminal 2 (Publisher):
```bash
mosquitto_pub -h localhost -t "test/topic" -m "Hello MQTT!"
```

Jika terminal 1 menampilkan `test/topic Hello MQTT!`, broker berfungsi dengan benar. ✓

### A.4 Instalasi Node.js dan Node-RED

```bash
# Install Node.js LTS
# Windows/macOS: download installer dari nodejs.org
# Linux:
curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
sudo apt install -y nodejs

# Verifikasi
node --version   # v18.x.x
npm --version    # 9.x.x

# Install Node-RED
npm install -g --unsafe-perm node-red

# Jalankan Node-RED
node-red

# Akses di browser: http://localhost:1880
```

**Install Node-RED Dashboard:**
Buka terminal baru, jalankan:
```bash
cd ~/.node-red
npm install node-red-dashboard
```

Restart Node-RED, akses `http://localhost:1880/ui` untuk dashboard.

---

## BAGIAN B: RANGKAIAN HARDWARE

### B.1 Skema Rangkaian

```
ESP32 DevKit V1               DHT22                LED
─────────────────          ─────────────          ────
3V3  ─────────────────────── VCC (pin 1)
GPIO4 ──[10kΩ]──┬─────────── DATA (pin 2)
                │
               GND
               3V3
GND  ─────────────────────── GND (pin 4)
                            
GPIO2 ──[220Ω]──────────────────────────── (+) LED (-) ── GND
                                           Anode      Katode
```

**Tabel Koneksi:**

| ESP32 Pin | Koneksi      | Keterangan             |
|-----------|--------------|------------------------|
| 3V3       | VCC DHT22    | Power DHT22            |
| GND       | GND DHT22    | Ground                 |
| GPIO4     | DATA DHT22   | Dengan pull-up 10kΩ    |
| GPIO2     | LED (+) via R| Anode LED, R=220Ω      |
| GND       | LED (-)      | Katode LED             |

### B.2 Foto Rangkaian (Referensi)

```
┌─────────────────────────────────────────┐
│  BREADBOARD                             │
│                                         │
│  [+] ─── 3V3 ESP32                     │
│  [-] ─── GND ESP32                     │
│                                         │
│  DHT22:  VCC─[+]  DATA─GPIO4  GND─[-] │
│  Pull-up: GPIO4─[10kΩ]─[+]            │
│                                         │
│  LED:  GPIO2─[220Ω]─[LED(+)]─[LED(-)]─[-] │
└─────────────────────────────────────────┘
```

### B.3 Pengecekan Hardware

Sebelum memulai program, lakukan pengecekan:
- [ ] ESP32 terdeteksi di Device Manager / port serial
- [ ] Polaritas LED benar (kaki panjang = anode = +)
- [ ] DHT22 terhubung ke 3V3, bukan 5V
- [ ] Resistor pull-up 10kΩ terpasang di jalur DATA DHT22
- [ ] Tidak ada short circuit (cek dengan multimeter)

---

## BAGIAN C: PRAKTIKUM 1 — WEBSERVER DASAR

**Estimasi waktu: 45 menit**

### C.1 Tujuan
Membuat WebServer sederhana pada ESP32 yang menampilkan halaman HTML dan REST API JSON.

### C.2 Kode Program — WebServer Hello World

Buat file baru di Arduino IDE: `praktikum1_webserver.ino`

```cpp
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ====================== KONFIGURASI ======================
const char* WIFI_SSID = "NAMA_WIFI_ANDA";
const char* WIFI_PASS = "PASSWORD_WIFI";

#define DHT_PIN     4
#define DHT_TYPE    DHT22
#define LED_PIN     2
// =========================================================

DHT dht(DHT_PIN, DHT_TYPE);
AsyncWebServer server(80);

// Halaman HTML yang akan ditampilkan
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 WebServer</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; background: #f0f0f0; }
    .card { background: white; border-radius: 10px; padding: 20px; 
            margin: 20px auto; max-width: 400px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
    h1 { color: #007bff; }
    .value { font-size: 2em; font-weight: bold; color: #333; }
    button { padding: 10px 25px; background: #007bff; color: white; 
             border: none; border-radius: 5px; cursor: pointer; font-size: 1em; }
    button:hover { background: #0056b3; }
  </style>
</head>
<body>
  <div class="card">
    <h1>🌡️ ESP32 Sensor Dashboard</h1>
    <p>Temperature: <span class="value" id="temp">--</span> °C</p>
    <p>Humidity: <span class="value" id="hum">--</span> %</p>
    <button onclick="toggleLed()">Toggle LED</button>
  </div>
  
  <script>
    // Update data setiap 3 detik
    setInterval(async function() {
      const res = await fetch('/api/sensor');
      const data = await res.json();
      document.getElementById('temp').textContent = data.temperature.toFixed(1);
      document.getElementById('hum').textContent = data.humidity.toFixed(1);
    }, 3000);
    
    async function toggleLed() {
      const res = await fetch('/api/led/toggle', { method: 'POST' });
      const data = await res.json();
      alert('LED sekarang: ' + (data.state ? 'ON' : 'OFF'));
    }
  </script>
</body>
</html>
)rawliteral";

// Variabel global LED state
bool ledState = false;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  
  // Koneksi WiFi
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n✓ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // ===== ROUTE DEFINITIONS =====
  
  // Route: GET / → Halaman HTML utama
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", HTML_PAGE);
  });
  
  // Route: GET /api/sensor → Data JSON sensor
  server.on("/api/sensor", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();
    
    StaticJsonDocument<200> doc;
    doc["temperature"] = isnan(temp) ? 0.0 : temp;
    doc["humidity"]    = isnan(hum)  ? 0.0 : hum;
    doc["led"]         = ledState;
    doc["uptime"]      = millis() / 1000;
    doc["rssi"]        = WiFi.RSSI();
    
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
  });
  
  // Route: POST /api/led/toggle → Toggle LED
  server.on("/api/led/toggle", HTTP_POST, [](AsyncWebServerRequest *request) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    
    StaticJsonDocument<100> doc;
    doc["state"] = ledState;
    doc["message"] = ledState ? "LED ON" : "LED OFF";
    
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
  });
  
  // Route: POST /api/led → Set LED state via JSON body
  server.on("/api/led", HTTP_POST, 
    [](AsyncWebServerRequest *request){},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<100> doc;
      DeserializationError err = deserializeJson(doc, data, len);
      
      if (!err) {
        ledState = doc["state"].as<bool>();
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        request->send(200, "application/json", "{\"status\":\"ok\",\"state\":" + 
                      String(ledState ? "true" : "false") + "}");
      } else {
        request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      }
    }
  );
  
  // Handle 404
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "application/json", "{\"error\":\"Not Found\"}");
  });
  
  server.begin();
  Serial.println("✓ WebServer started at http://" + WiFi.localIP().toString());
  Serial.println("→ Buka browser dan akses: http://" + WiFi.localIP().toString());
}

void loop() {
  // Tidak perlu kode di sini, AsyncWebServer berjalan di background
  // Monitor WiFi reconnect
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.reconnect();
    delay(5000);
  }
}
```

### C.3 Upload dan Test

1. Ganti `NAMA_WIFI_ANDA` dan `PASSWORD_WIFI` dengan kredensial WiFi Anda
2. Klik **Verify** (✓) untuk compile, pastikan tidak ada error
3. Klik **Upload** (→) untuk upload ke ESP32
4. Buka **Serial Monitor** (Ctrl+Shift+M), set baudrate **115200**
5. Tunggu hingga muncul IP Address
6. Buka browser, akses `http://[IP_ADDRESS]/`

**Test dengan Postman atau curl:**
```bash
# GET data sensor
curl http://[IP]/api/sensor

# POST toggle LED
curl -X POST http://[IP]/api/led/toggle

# POST set LED ON
curl -X POST http://[IP]/api/led \
  -H "Content-Type: application/json" \
  -d '{"state": true}'
```

### C.4 Pertanyaan Praktikum 1

1. Berapa nilai RTT yang Anda ukur saat mengakses REST API dari browser?
2. Berapa efisiensi bandwidth HTTP untuk request GET `/api/sensor`?
3. Coba akses dari dua device berbeda secara bersamaan — apakah server crash?

---

## BAGIAN D: PRAKTIKUM 2 — MQTT PUBLISHER

**Estimasi waktu: 45 menit**

### D.1 Tujuan
Memprogram ESP32 sebagai MQTT Publisher yang mengirim data sensor ke broker Mosquitto.

### D.2 Kode Program

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ====================== KONFIGURASI ======================
const char* WIFI_SSID   = "NAMA_WIFI_ANDA";
const char* WIFI_PASS   = "PASSWORD_WIFI";
const char* MQTT_BROKER = "192.168.X.X";  // Ganti dengan IP laptop Anda
const int   MQTT_PORT   = 1883;
const char* CLIENT_ID   = "ESP32_Publisher_01";

// Topics
const char* TOPIC_SENSOR  = "lab/esp32/sensor";
const char* TOPIC_STATUS  = "lab/esp32/status";

#define DHT_PIN   4
#define DHT_TYPE  DHT22
#define LED_PIN   2
// =========================================================

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long lastPublish = 0;
const long    PUBLISH_INTERVAL = 5000;  // 5 detik

void connectWiFi() {
  Serial.print("Connecting WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\n✓ WiFi: " + WiFi.localIP().toString());
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting MQTT...");
    
    if (mqttClient.connect(CLIENT_ID, NULL, NULL,
        TOPIC_STATUS, 1, true, "offline")) {  // LWT: offline
      
      Serial.println(" ✓ Connected!");
      
      // Publish status online
      mqttClient.publish(TOPIC_STATUS, "online", true);
      Serial.println("→ Published: " + String(TOPIC_STATUS) + " = online");
      
    } else {
      Serial.print(" ✗ Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retry in 5s...");
      delay(5000);
    }
  }
}

void publishSensorData() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  
  if (isnan(temp) || isnan(hum)) {
    Serial.println("✗ DHT22 Read Error!");
    return;
  }
  
  // Buat JSON
  StaticJsonDocument<256> doc;
  doc["device_id"]    = CLIENT_ID;
  doc["temperature"]  = round(temp * 10) / 10.0;  // 1 desimal
  doc["humidity"]     = round(hum * 10) / 10.0;
  doc["rssi"]         = WiFi.RSSI();
  doc["uptime_s"]     = millis() / 1000;
  doc["timestamp"]    = millis();
  
  char buffer[256];
  size_t n = serializeJson(doc, buffer);
  
  // Publish ke broker
  bool success = mqttClient.publish(TOPIC_SENSOR, buffer, n);
  
  if (success) {
    Serial.printf("✓ Published [%s]: T=%.1f°C H=%.1f%% RSSI=%d\n",
                  TOPIC_SENSOR, temp, hum, WiFi.RSSI());
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("✗ Publish failed!");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("\n========== ESP32 MQTT Publisher ==========");
  
  connectWiFi();
  
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setBufferSize(512);  // Perbesar buffer jika perlu
  
  connectMQTT();
  
  Serial.println("→ Subscribe untuk monitor: mosquitto_sub -h " + 
                 String(MQTT_BROKER) + " -t 'lab/#' -v");
}

void loop() {
  // Pastikan koneksi MQTT tetap aktif
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
  
  // Publish setiap PUBLISH_INTERVAL
  unsigned long now = millis();
  if (now - lastPublish >= PUBLISH_INTERVAL) {
    lastPublish = now;
    publishSensorData();
  }
}
```

### D.3 Monitoring dengan MQTT Explorer

1. Buka **MQTT Explorer**
2. Klik **+** tambah koneksi baru:
   - Protocol: `mqtt://`
   - Host: `localhost` (atau IP laptop)
   - Port: `1883`
3. Klik **Connect**
4. Setelah ESP32 publish, lihat topic `lab/esp32/sensor` muncul di tree
5. Klik topic untuk lihat payload JSON

### D.4 Monitoring di Terminal

```bash
# Subscribe semua topic di lab/
mosquitto_sub -h localhost -t "lab/#" -v

# Subscribe dengan format lebih detail
mosquitto_sub -h localhost -t "lab/#" -v --pretty
```

---

## BAGIAN E: PRAKTIKUM 3 — MQTT SUBSCRIBER

**Estimasi waktu: 30 menit**

### E.1 Tujuan
Memprogram ESP32 sebagai MQTT Subscriber yang menerima perintah kontrol dari broker.

### E.2 Kode Program (Tambahan ke Praktikum 2)

Tambahkan/modifikasi kode Praktikum 2 dengan bagian subscriber:

```cpp
// Tambahkan di atas setup():
const char* TOPIC_CONTROL = "lab/esp32/control";  // Topic untuk terima perintah

// Callback function — dipanggil saat pesan diterima
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);
  
  // Buat null-terminated string dari payload
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0';
  
  Serial.printf("← Received [%s]: %s\n", topic, msg);
  
  // Parse JSON payload
  StaticJsonDocument<200> doc;
  DeserializationError err = deserializeJson(doc, msg);
  
  if (err) {
    Serial.println("✗ JSON parse error: " + String(err.c_str()));
    return;
  }
  
  // Handler berdasarkan topic
  if (topicStr == TOPIC_CONTROL) {
    
    // Kontrol LED
    if (doc.containsKey("led")) {
      bool ledState = doc["led"].as<bool>();
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      Serial.println("→ LED set to: " + String(ledState ? "ON" : "OFF"));
      
      // Konfirmasi balik ke broker
      StaticJsonDocument<100> ack;
      ack["action"] = "led_set";
      ack["state"]  = ledState;
      ack["device"] = CLIENT_ID;
      char ackBuf[100];
      serializeJson(ack, ackBuf);
      mqttClient.publish("lab/esp32/ack", ackBuf);
    }
    
    // Kontrol interval publish
    if (doc.containsKey("interval")) {
      // (Opsional: ubah PUBLISH_INTERVAL dinamis)
      Serial.println("→ New interval: " + String(doc["interval"].as<int>()) + "ms");
    }
    
    // Reset ESP32
    if (doc.containsKey("reset") && doc["reset"].as<bool>()) {
      Serial.println("→ Reset command received!");
      mqttClient.publish("lab/esp32/status", "restarting", true);
      delay(500);
      ESP.restart();
    }
  }
}

// Di connectMQTT(), tambahkan setelah connect berhasil:
// mqttClient.setCallback(mqttCallback);
// mqttClient.subscribe(TOPIC_CONTROL);

// Di setup(), tambahkan sebelum connectMQTT():
// mqttClient.setCallback(mqttCallback);
```

### E.3 Kirim Perintah dari Terminal

```bash
# Nyalakan LED
mosquitto_pub -h localhost -t "lab/esp32/control" -m '{"led":true}'

# Matikan LED
mosquitto_pub -h localhost -t "lab/esp32/control" -m '{"led":false}'

# Reset ESP32
mosquitto_pub -h localhost -t "lab/esp32/control" -m '{"reset":true}'

# Monitor acknowledgment
mosquitto_sub -h localhost -t "lab/esp32/ack" -v
```

---

## BAGIAN F: PRAKTIKUM 4 — INTEGRASI WEBSERVER + MQTT + FREERTOS

**Estimasi waktu: 90 menit**

### F.1 Tujuan
Mengintegrasikan WebServer, MQTT, dan FreeRTOS dalam satu program ESP32.

### F.2 Arsitektur Program

```
┌─────────────────────────────────────────────────────┐
│                    ESP32 Program                     │
│                                                      │
│  ┌───────────────┐    ┌──────────────────────────┐  │
│  │  Core 0       │    │  Core 1                  │  │
│  │  (Protocol)   │    │  (Application)           │  │
│  │               │    │                          │  │
│  │  WiFi Stack   │    │ ┌──────────────────────┐ │  │
│  │  MQTT Stack   │    │ │ Task: SensorRead     │ │  │
│  │               │    │ │ Prio: 1, Period: 5s  │ │  │
│  │               │    │ └──────────┬───────────┘ │  │
│  │               │    │           │ Mutex        │  │
│  │               │    │ ┌──────────▼───────────┐ │  │
│  │               │    │ │ Task: MQTTPublish    │ │  │
│  │               │    │ │ Prio: 2, Period: 5s  │ │  │
│  │               │    │ └──────────────────────┘ │  │
│  │               │    │                          │  │
│  │               │    │ ┌──────────────────────┐ │  │
│  │               │    │ │ AsyncWebServer       │ │  │
│  │               │    │ │ (event-driven)       │ │  │
│  │               │    │ └──────────────────────┘ │  │
│  └───────────────┘    └──────────────────────────┘  │
└─────────────────────────────────────────────────────┘
```

### F.3 Kode Program Lengkap

```cpp
// ================================================================
// Modul 6 - Integrasi WebServer + MQTT + FreeRTOS
// ================================================================
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ====================== KONFIGURASI ======================
const char* WIFI_SSID   = "NAMA_WIFI_ANDA";
const char* WIFI_PASS   = "PASSWORD_WIFI";
const char* MQTT_BROKER = "192.168.X.X";
const int   MQTT_PORT   = 1883;
const char* DEVICE_ID   = "ESP32_Lab_01";

#define DHT_PIN       4
#define LED_PIN       2
#define DHT_TYPE      DHT22
#define PUBLISH_MS    5000
// =========================================================

// ===== Objek Global =====
DHT dht(DHT_PIN, DHT_TYPE);
AsyncWebServer server(80);
AsyncEventSource events("/events");
WiFiClient wifiNetClient;
PubSubClient mqttClient(wifiNetClient);

// ===== Shared Data (Thread-Safe) =====
SemaphoreHandle_t dataMutex;
float  g_temp      = 0.0;
float  g_humidity  = 0.0;
bool   g_ledState  = false;
long   g_rssi      = 0;

// ===== HTML Dashboard =====
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>IoT Dashboard - ESP32</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body { font-family: 'Segoe UI', sans-serif; background: #0d1117; color: #c9d1d9; }
    header { background: #161b22; padding: 15px 20px; border-bottom: 1px solid #30363d; }
    header h1 { color: #58a6ff; font-size: 1.4em; }
    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 15px; padding: 20px; }
    .card { background: #161b22; border-radius: 10px; padding: 20px; text-align: center; border: 1px solid #30363d; }
    .card-title { font-size: 0.85em; color: #8b949e; margin-bottom: 8px; }
    .card-value { font-size: 2em; font-weight: bold; }
    .temp { color: #ff7b72; }
    .hum  { color: #58a6ff; }
    .led  { color: #7ee787; }
    .rssi { color: #ffa657; }
    .chart-container { padding: 0 20px 20px; }
    .controls { padding: 0 20px 20px; display: flex; gap: 10px; }
    button { padding: 10px 20px; border: none; border-radius: 6px; cursor: pointer; font-size: 0.9em; transition: opacity 0.2s; }
    button:hover { opacity: 0.8; }
    .btn-on  { background: #2ea043; color: white; }
    .btn-off { background: #d73a49; color: white; }
    .status { background: #161b22; border: 1px solid #30363d; border-radius: 6px; padding: 8px 15px; font-size: 0.85em; color: #7ee787; }
  </style>
</head>
<body>
  <header><h1>🌐 ESP32 IoT Dashboard</h1></header>
  
  <div class="grid">
    <div class="card"><div class="card-title">🌡️ Temperature</div>
      <div class="card-value temp" id="tempVal">--</div><small>°C</small></div>
    <div class="card"><div class="card-title">💧 Humidity</div>
      <div class="card-value hum" id="humVal">--</div><small>%</small></div>
    <div class="card"><div class="card-title">💡 LED Status</div>
      <div class="card-value led" id="ledVal">--</div></div>
    <div class="card"><div class="card-title">📶 WiFi RSSI</div>
      <div class="card-value rssi" id="rssiVal">--</div><small>dBm</small></div>
  </div>
  
  <div class="controls">
    <button class="btn-on"  onclick="setLed(true)">LED ON</button>
    <button class="btn-off" onclick="setLed(false)">LED OFF</button>
    <div class="status" id="connStatus">Connecting...</div>
  </div>
  
  <div class="chart-container">
    <canvas id="sensorChart" height="80"></canvas>
  </div>
  
  <script>
    const MAX_POINTS = 30;
    const labels=[], tempData=[], humData=[];
    
    const chart = new Chart(document.getElementById('sensorChart').getContext('2d'), {
      type: 'line',
      data: {
        labels,
        datasets: [
          { label: 'Suhu (°C)',      data: tempData, borderColor:'#ff7b72', backgroundColor:'rgba(255,123,114,0.1)', fill:true, tension:0.4 },
          { label: 'Kelembaban (%)', data: humData,  borderColor:'#58a6ff', backgroundColor:'rgba(88,166,255,0.1)', fill:true, tension:0.4 }
        ]
      },
      options: { animation:false, scales: { x: { ticks: { color:'#8b949e' } }, y: { ticks: { color:'#8b949e' }, min:0, max:100 } }, plugins: { legend: { labels: { color:'#c9d1d9' } } } }
    });
    
    // Server-Sent Events
    const evtSource = new EventSource('/events');
    evtSource.addEventListener('sensor', e => {
      const d = JSON.parse(e.data);
      document.getElementById('tempVal').textContent = d.temperature.toFixed(1);
      document.getElementById('humVal').textContent  = d.humidity.toFixed(1);
      document.getElementById('ledVal').textContent  = d.led ? 'ON' : 'OFF';
      document.getElementById('rssiVal').textContent = d.rssi;
      document.getElementById('connStatus').textContent = '● Connected';
      
      const t = new Date().toLocaleTimeString();
      labels.push(t); tempData.push(d.temperature); humData.push(d.humidity);
      if(labels.length > MAX_POINTS){ labels.shift(); tempData.shift(); humData.shift(); }
      chart.update('none');
    });
    evtSource.onerror = () => {
      document.getElementById('connStatus').textContent = '● Disconnected';
      document.getElementById('connStatus').style.color = '#ff7b72';
    };
    
    async function setLed(state) {
      const res = await fetch('/api/led', {
        method: 'POST',
        headers: {'Content-Type':'application/json'},
        body: JSON.stringify({state})
      });
      const data = await res.json();
      document.getElementById('ledVal').textContent = data.state ? 'ON' : 'OFF';
    }
  </script>
</body>
</html>
)rawliteral";

// ===== MQTT Callback =====
void onMQTTMessage(char* topic, byte* payload, unsigned int len) {
  char msg[len + 1];
  memcpy(msg, payload, len);
  msg[len] = '\0';
  
  Serial.printf("← MQTT [%s]: %s\n", topic, msg);
  
  StaticJsonDocument<200> doc;
  if (deserializeJson(doc, msg) == DeserializationError::Ok) {
    if (doc.containsKey("led")) {
      bool st = doc["led"].as<bool>();
      xSemaphoreTake(dataMutex, portMAX_DELAY);
      g_ledState = st;
      xSemaphoreGive(dataMutex);
      digitalWrite(LED_PIN, st ? HIGH : LOW);
    }
  }
}

// ===== Task: Baca Sensor =====
void sensorTask(void *pvParams) {
  for (;;) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    if (!isnan(t) && !isnan(h)) {
      xSemaphoreTake(dataMutex, portMAX_DELAY);
      g_temp      = t;
      g_humidity  = h;
      g_rssi      = WiFi.RSSI();
      xSemaphoreGive(dataMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// ===== Task: Publish MQTT + SSE =====
void publishTask(void *pvParams) {
  for (;;) {
    // Reconnect MQTT jika putus
    if (!mqttClient.connected()) {
      if (mqttClient.connect(DEVICE_ID, NULL, NULL, "lab/esp32/status", 1, true, "offline")) {
        mqttClient.publish("lab/esp32/status", "online", true);
        mqttClient.subscribe("lab/esp32/control");
        Serial.println("✓ MQTT Reconnected");
      } else {
        vTaskDelay(pdMS_TO_TICKS(5000));
        continue;
      }
    }
    mqttClient.loop();
    
    // Ambil data thread-safe
    float t, h; long rssi; bool led;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    t = g_temp; h = g_humidity; rssi = g_rssi; led = g_ledState;
    xSemaphoreGive(dataMutex);
    
    // Buat JSON
    StaticJsonDocument<256> doc;
    doc["device_id"]    = DEVICE_ID;
    doc["temperature"]  = (float)round(t * 10) / 10;
    doc["humidity"]     = (float)round(h * 10) / 10;
    doc["led"]          = led;
    doc["rssi"]         = rssi;
    doc["uptime"]       = millis() / 1000;
    
    char buf[256];
    serializeJson(doc, buf);
    
    // Publish MQTT
    mqttClient.publish("lab/esp32/sensor", buf);
    
    // Kirim ke WebServer via SSE
    events.send(buf, "sensor", millis());
    
    Serial.printf("→ Published: T=%.1f H=%.1f RSSI=%ld\n", t, h, rssi);
    
    vTaskDelay(pdMS_TO_TICKS(PUBLISH_MS));
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  
  dataMutex = xSemaphoreCreateMutex();
  
  Serial.println("\n========== Modul 6: Integrasi WebServer MQTT ==========");
  
  // Koneksi WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\n✓ WiFi: http://" + WiFi.localIP().toString());
  
  // MQTT Setup
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(onMQTTMessage);
  mqttClient.setBufferSize(512);
  
  // WebServer Routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send_P(200, "text/html", DASHBOARD_HTML);
  });
  
  server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest *req) {
    StaticJsonDocument<200> doc;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    doc["temperature"] = g_temp;
    doc["humidity"]    = g_humidity;
    doc["led"]         = g_ledState;
    doc["rssi"]        = g_rssi;
    xSemaphoreGive(dataMutex);
    String json; serializeJson(doc, json);
    req->send(200, "application/json", json);
  });
  
  server.on("/api/led", HTTP_POST,
    [](AsyncWebServerRequest *req){},
    NULL,
    [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t idx, size_t total) {
      StaticJsonDocument<100> doc;
      if (deserializeJson(doc, data, len) == DeserializationError::Ok) {
        bool st = doc["state"].as<bool>();
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        g_ledState = st;
        xSemaphoreGive(dataMutex);
        digitalWrite(LED_PIN, st ? HIGH : LOW);
        req->send(200, "application/json", 
                  "{\"state\":" + String(st?"true":"false") + "}");
      } else {
        req->send(400, "application/json", "{\"error\":\"Bad JSON\"}");
      }
    }
  );
  
  events.onConnect([](AsyncEventSourceClient *client) {
    client->send("connected", NULL, millis(), 3000);
  });
  server.addHandler(&events);
  server.begin();
  Serial.println("✓ WebServer running");
  
  // Buat Tasks (di Core 1)
  xTaskCreatePinnedToCore(sensorTask,  "SensorTask",  4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(publishTask, "PublishTask", 8192, NULL, 2, NULL, 1);
  
  Serial.println("✓ FreeRTOS Tasks created");
  Serial.println("→ Akses dashboard: http://" + WiFi.localIP().toString());
}

void loop() {
  // Kosong - semua di task
}
```

### F.4 Verifikasi Sistem

Cek poin-poin berikut setelah upload:

- [ ] Serial Monitor menampilkan IP Address
- [ ] Dashboard terbuka di browser
- [ ] Grafik suhu dan kelembaban update otomatis tiap 5 detik
- [ ] Tombol LED ON/OFF berfungsi mengubah status LED fisik
- [ ] MQTT Explorer menampilkan data di topic `lab/esp32/sensor`
- [ ] Kirim perintah via `mosquitto_pub` mengubah LED

---

## BAGIAN G: PRAKTIKUM 5 — NODE-RED DASHBOARD

**Estimasi waktu: 60 menit**

### G.1 Tujuan
Membangun dashboard profesional menggunakan Node-RED yang terhubung ke MQTT broker.

### G.2 Setup Node-RED

1. Jalankan Node-RED: `node-red` di terminal
2. Buka `http://localhost:1880`

### G.3 Import Flow Node-RED

Klik menu **≡ (hamburger) → Import → Paste JSON below:**

```json
[
  {
    "id": "mqtt-broker-1",
    "type": "mqtt-broker",
    "name": "Mosquitto Local",
    "broker": "localhost",
    "port": "1883",
    "clientid": "nodered-sub-01",
    "autoConnect": true
  },
  {
    "id": "mqtt-in-sensor",
    "type": "mqtt in",
    "name": "Sensor Data",
    "topic": "lab/esp32/sensor",
    "qos": "1",
    "broker": "mqtt-broker-1",
    "wires": [["json-parse"]]
  },
  {
    "id": "json-parse",
    "type": "json",
    "name": "Parse JSON",
    "wires": [["set-temp", "set-hum", "chart-temp", "debug-1"]]
  },
  {
    "id": "set-temp",
    "type": "change",
    "name": "Extract Temp",
    "rules": [{"t": "set", "p": "payload", "to": "payload.temperature"}],
    "wires": [["gauge-temp"]]
  },
  {
    "id": "set-hum",
    "type": "change",
    "name": "Extract Humidity",
    "rules": [{"t": "set", "p": "payload", "to": "payload.humidity"}],
    "wires": [["gauge-hum"]]
  }
]
```

Klik **Deploy**.

### G.4 Menambahkan Node Manual

Drag-drop node-node berikut ke canvas:

1. **mqtt in** → konfigurasi ke broker localhost:1883, topic `lab/esp32/sensor`
2. **json** → untuk parse JSON
3. **function** → untuk ekstrak field
4. **ui_gauge** × 2 → untuk suhu dan kelembaban
5. **ui_chart** → untuk grafik historis
6. **ui_button** → untuk kontrol LED
7. **mqtt out** → untuk publish perintah ke `lab/esp32/control`

**Function Node — Extract & Process:**
```javascript
// Ekstrak data dan buat output multiple
const data = msg.payload;

// Output 1: Temperature
const tempMsg = { payload: data.temperature, topic: 'temperature' };
// Output 2: Humidity  
const humMsg  = { payload: data.humidity, topic: 'humidity' };
// Output 3: Full data untuk chart
const chartMsg = { payload: { temperature: data.temperature, humidity: data.humidity } };

return [tempMsg, humMsg, chartMsg];
```

**Button Node — Toggle LED:**
```javascript
// Saat tombol ditekan, kirim JSON ke MQTT
msg.payload = JSON.stringify({ led: !context.get('ledState') || false });
context.set('ledState', !context.get('ledState'));
return msg;
```

### G.5 Konfigurasi Dashboard

1. Klik **Dashboard** di panel kanan
2. Tambah **Tab** baru: "ESP32 Monitor"
3. Tambah **Group**: "Sensor Data", "Charts", "Control"
4. Assign setiap UI node ke group yang sesuai
5. Klik **Deploy**
6. Akses dashboard di `http://localhost:1880/ui`

---

## BAGIAN H: TROUBLESHOOTING

### H.1 Masalah Umum dan Solusi

| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| Port COM tidak muncul | Driver belum terinstal | Install driver CH340 atau CP2102 |
| Upload gagal "timed out" | Mode upload salah | Tekan tombol **BOOT** saat proses upload |
| Upload gagal "brownout" | Power supply tidak stabil | Ganti kabel USB, gunakan port USB 3.0 |
| WiFi tidak connect | SSID/pass salah atau 5GHz | Cek kredensial, pastikan WiFi 2.4GHz |
| MQTT tidak connect | Broker tidak berjalan | `sudo systemctl status mosquitto` |
| MQTT error rc=-2 | IP broker salah | Cek IP laptop di WiFi dengan `ipconfig` |
| MQTT error rc=5 | Auth diperlukan | Cek file konfigurasi mosquitto.conf |
| DHT22 "nan" | Kabel atau pull-up | Cek wiring, tambahkan resistor 10kΩ |
| Heap memory low | Leak atau alokasi besar | Kurangi buffer, gunakan stack lebih kecil |
| WebServer lambat | Blocking di loop | Pindahkan ke task atau gunakan async |
| ESP32 crash/restart | Stack overflow | Perbesar stack size pada xTaskCreate |

### H.2 Debug Kode

```cpp
// Monitor heap memory
Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());

// Monitor WiFi status
Serial.println("WiFi status: " + String(WiFi.status()));
// 0=WL_IDLE, 3=WL_CONNECTED, 6=WL_DISCONNECTED

// Monitor MQTT state
// -4=TIMEOUT, -3=LOST, -2=FAILED, -1=DISCONNECTED, 0=CONNECTED

// Enable ESP32 debug level
#define CORE_DEBUG_LEVEL 5  // 0=None, 1=Error, 2=Warn, 5=Verbose
```

### H.3 Checklist Verifikasi Akhir

**Hardware:**
- [ ] ESP32 terdeteksi di port serial
- [ ] LED berkedip saat publish data
- [ ] DHT22 membaca nilai suhu & kelembaban yang masuk akal

**Software:**
- [ ] Arduino IDE dapat compile tanpa error
- [ ] Serial Monitor menampilkan IP Address
- [ ] Browser dapat membuka dashboard ESP32
- [ ] MQTT Explorer menampilkan data realtime
- [ ] Node-RED dashboard menampilkan gauge dan chart

**Integrasi:**
- [ ] WebServer dan MQTT berjalan bersamaan
- [ ] Perintah dari `mosquitto_pub` mengubah LED
- [ ] Tombol di browser dashboard mengubah LED
- [ ] Data sensor update setiap 5 detik di semua platform

---

## BAGIAN I: LAPORAN PRAKTIKUM

### I.1 Format Laporan

Buat laporan dalam format PDF dengan struktur:

1. **Cover** — NIM, Nama, Judul Praktikum, Tanggal
2. **Tujuan Praktikum** — 5 poin tujuan
3. **Alat dan Bahan** — List lengkap
4. **Dasar Teori** — Ringkasan 1-2 halaman (HTTP, MQTT, ESP32)
5. **Langkah Percobaan** — Screenshot setiap langkah
6. **Hasil Percobaan** — Screenshot hasil, data yang dikumpulkan
7. **Analisis dan Pembahasan** — Jawab pertanyaan di setiap bagian
8. **Kesimpulan** — Minimum 3 poin
9. **Daftar Pustaka** — Minimal 5 referensi

### I.2 Data yang Harus Dikumpulkan

| No | Data                        | Cara Pengambilan             |
|----|-----------------------------|------------------------------|
| 1  | Screenshot dashboard browser| Ctrl+Shift+S                 |
| 2  | Log Serial Monitor          | Copy-paste dari Serial Monitor|
| 3  | Screenshot MQTT Explorer    | Tampilkan topic dan payload  |
| 4  | Screenshot Node-RED dashboard| Tampilkan gauge dan chart   |
| 5  | Hasil pengukuran RSSI       | Serial Monitor + rumus FSPL  |
| 6  | Perhitungan latency MQTT    | Rumus dari Materi Bab 4      |
| 7  | Perhitungan battery life    | Rumus dari Materi Bab 12     |

---

*Jobsheet ini dibuat untuk mendukung pelaksanaan Praktikum Mekatronika Modul 6. Ikuti setiap langkah dengan seksama dan catat semua hasil pengamatan.*
