# MATERI MODUL 04: IoT WebServer MQTT ESP32

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – IoT WebServer MQTT ESP32  
**Hardware:** ESP32 DevKit / ESP32-S2  
**Software:** Arduino IDE / PlatformIO, Mosquitto MQTT Broker  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan – Internet of Things (IoT)](#1-pendahuluan--internet-of-things-iot)
2. [Arsitektur IoT](#2-arsitektur-iot)
3. [ESP32 – Mikrokontroler IoT](#3-esp32--mikrokontroler-iot)
4. [WiFi pada ESP32](#4-wifi-pada-esp32)
5. [Web Server pada ESP32](#5-web-server-pada-esp32)
6. [Protokol MQTT](#6-protokol-mqtt)
7. [MQTT Broker – Mosquitto](#7-mqtt-broker--mosquitto)
8. [PubSubClient Library untuk ESP32](#8-pubsubclient-library-untuk-esp32)
9. [Integrasi WebServer dan MQTT](#9-integrasi-webserver-dan-mqtt)
10. [Dashboard IoT](#10-dashboard-iot)
11. [Keamanan IoT Dasar](#11-keamanan-iot-dasar)
12. [Referensi](#12-referensi)

---

## 1. PENDAHULUAN – INTERNET OF THINGS (IoT)

### 1.1 Definisi IoT

**Internet of Things (IoT)** adalah konsep di mana perangkat fisik (things) terhubung ke internet dan dapat saling bertukar data tanpa intervensi manusia secara langsung. Dalam konteks robotika, IoT memungkinkan:

- **Monitoring jarak jauh** — memantau status robot dari browser/smartphone
- **Kontrol remote** — mengirim perintah ke robot via internet
- **Data logging** — menyimpan data sensor ke cloud
- **Interoperabilitas** — robot berkomunikasi dengan perangkat lain

### 1.2 Contoh Penerapan IoT dalam Robotika

| Aplikasi | Deskripsi |
|---------|-----------|
| Smart Factory | Robot industri melaporkan status via MQTT ke dashboard |
| Agriculture Robot | Sensor kelembaban mengirim data ke cloud, robot menyiram otomatis |
| Autonomous Vehicle | Telemetri kendaraan dikirim real-time ke server |
| Home Automation | Robot pembersih dikendalikan dari smartphone |
| Warehouse Robot | Fleet management via WebSocket/MQTT |

### 1.3 Stack Teknologi IoT

```
┌─────────────────────────────────────┐
│          APPLICATION LAYER          │
│  Dashboard, Mobile App, Analytics   │
├─────────────────────────────────────┤
│          PLATFORM LAYER             │
│  Cloud, MQTT Broker, Database       │
├─────────────────────────────────────┤
│          NETWORK LAYER              │
│  WiFi, Bluetooth, LoRa, Cellular   │
├─────────────────────────────────────┤
│          PERCEPTION LAYER           │
│  Sensors, Actuators, MCU (ESP32)    │
└─────────────────────────────────────┘
```

---

## 2. ARSITEKTUR IoT

### 2.1 Arsitektur Publish-Subscribe vs Request-Response

| Aspek | Request-Response (HTTP) | Publish-Subscribe (MQTT) |
|-------|------------------------|--------------------------|
| Model | Client → Server → Client | Publisher → Broker → Subscriber |
| Koneksi | Per-request (stateless) | Persistent (stateful) |
| Overhead | Tinggi (header HTTP besar) | Rendah (header 2 byte) |
| Real-time | Polling (tidak efisien) | Push notification (efisien) |
| Use case | Web page, REST API | Sensor data, real-time control |
| Banyak penerima | Harus kirim ke masing-masing | Broadcast via topic |

### 2.2 Arsitektur Sistem Praktikum

```
[ESP32 + Sensor] ──WiFi──> [MQTT Broker (Mosquitto)]
       │                           │
       │                    ┌──────┴──────┐
       │                    │  Subscriber  │
       │                    │  (Dashboard) │
       │                    └─────────────┘
       │
       └──WiFi──> [Web Server ESP32]
                        │
                  ┌─────┴─────┐
                  │  Browser  │
                  │  (Client) │
                  └───────────┘
```

---

## 3. ESP32 – MIKROKONTROLER IoT

### 3.1 Spesifikasi ESP32

| Parameter | Nilai |
|-----------|-------|
| CPU | Dual-core Xtensa LX6, 240 MHz |
| RAM | 520 KB SRAM |
| Flash | 4 MB (standar) |
| WiFi | 802.11 b/g/n, 2.4 GHz |
| Bluetooth | v4.2 BR/EDR + BLE |
| GPIO | 34 pin (tergantung modul) |
| ADC | 2 × 12-bit SAR ADC, 18 channel |
| DAC | 2 × 8-bit |
| PWM | 16 channel LED PWM |
| I2C | 2 bus |
| SPI | 4 bus |
| UART | 3 port |
| Tegangan operasi | 3.3V |
| Arus deep sleep | ~10 µA |

### 3.2 Pinout ESP32 DevKit

```
                    ┌───────────┐
             EN  ──│1        38│── GND
            VP(36)──│2        37│── GPIO23 (MOSI)
            VN(39)──│3        36│── GPIO22 (SCL)
           GPIO34 ──│4        35│── GPIO1  (TX0)
           GPIO35 ──│5        34│── GPIO3  (RX0)
           GPIO32 ──│6        33│── GPIO21 (SDA)
           GPIO33 ──│7        32│── GND
           GPIO25 ──│8        31│── GPIO19 (MISO)
           GPIO26 ──│9        30│── GPIO18 (SCK)
           GPIO27 ──│10       29│── GPIO5  (SS)
           GPIO14 ──│11       28│── GPIO17 (TX2)
           GPIO12 ──│12       27│── GPIO16 (RX2)
            GND   ──│13       26│── GPIO4
            VIN   ──│14       25│── GPIO0  (BOOT)
           GPIO13 ──│15       24│── GPIO2  (LED)
            SD2   ──│16       23│── GPIO15
            SD3   ──│17       22│── SD1
            CMD   ──│18       21│── SD0
            3V3   ──│19       20│── CLK
                    └───────────┘
```

### 3.3 Arduino Core untuk ESP32

Instalasi di Arduino IDE:
1. **File > Preferences > Additional Board URLs:**
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
2. **Tools > Board > Boards Manager** → cari `esp32` → Install
3. Pilih board: `ESP32 Dev Module` atau sesuai modul

---

## 4. WiFi PADA ESP32

### 4.1 Mode WiFi ESP32

| Mode | Fungsi |
|------|--------|
| **Station (STA)** | Terhubung ke router/access point yang ada |
| **Access Point (AP)** | ESP32 menjadi hotspot sendiri |
| **STA + AP** | Kombinasi keduanya |

### 4.2 Kode Koneksi WiFi (Station Mode)

```cpp
#include <WiFi.h>

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
```

### 4.3 Kode Access Point Mode

```cpp
#include <WiFi.h>

const char* ap_ssid = "Robot_IoT";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {}
```

---

## 5. WEB SERVER PADA ESP32

### 5.1 Konsep Web Server Embedded

ESP32 dapat menjalankan **HTTP server** ringan yang melayani halaman web langsung dari flash memory. Client (browser) mengakses IP ESP32 untuk melihat data sensor atau mengontrol aktuator.

### 5.2 Library WebServer

```cpp
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Robot IoT Dashboard</h1>";
  html += "<p>Suhu: " + String(readTemperature()) + " C</p>";
  html += "<p><a href='/led/on'>LED ON</a></p>";
  html += "<p><a href='/led/off'>LED OFF</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleLedOn() {
  digitalWrite(2, HIGH);
  server.send(200, "text/html", "<p>LED ON</p><a href='/'>Back</a>");
}

void handleLedOff() {
  digitalWrite(2, LOW);
  server.send(200, "text/html", "<p>LED OFF</p><a href='/'>Back</a>");
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  
  WiFi.begin("SSID", "PASS");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.begin();
  Serial.println("Server started: " + WiFi.localIP().toString());
}

void loop() {
  server.handleClient();
}
```

### 5.3 Async Web Server (ESPAsyncWebServer)

Lebih efisien daripada WebServer blocking:

```cpp
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void setup() {
  WiFi.begin("SSID", "PASS");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<h1>Async Server ESP32</h1>");
  });
  
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"suhu\": 25.5, \"kelembaban\": 60}";
    request->send(200, "application/json", json);
  });
  
  server.begin();
}

void loop() {}
```

### 5.4 SPIFFS/LittleFS untuk Hosting File HTML

ESP32 dapat menyimpan file HTML, CSS, JavaScript di flash memory:

1. Install plugin **ESP32 Sketch Data Upload**
2. Buat folder `data/` di project
3. Letakkan file `index.html`, `style.css`, `script.js`
4. Upload via **Tools > ESP32 Sketch Data Upload**
5. Akses dari kode:

```cpp
#include <SPIFFS.h>

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/index.html", "text/html");
});
```

---

## 6. PROTOKOL MQTT

### 6.1 Apa itu MQTT?

**MQTT (Message Queuing Telemetry Transport)** adalah protokol komunikasi ringan berbasis publish-subscribe yang dirancang untuk perangkat IoT dengan bandwidth terbatas dan latensi tinggi.

### 6.2 Komponen MQTT

| Komponen | Fungsi |
|---------|--------|
| **Publisher** | Perangkat yang mengirim pesan ke topic |
| **Subscriber** | Perangkat yang menerima pesan dari topic |
| **Broker** | Server penengah yang mengelola routing pesan |
| **Topic** | Alamat/kanal pesan (hierarki: `robot/sensor/suhu`) |
| **Payload** | Isi pesan (string, JSON, binary) |

### 6.3 Cara Kerja MQTT

```
[ESP32 Publisher]                    [Broker]                [Dashboard Subscriber]
       │                                │                           │
       │── CONNECT ──────────────────>  │                           │
       │<── CONNACK ──────────────────  │                           │
       │                                │  <── SUBSCRIBE ───────────│
       │                                │      (robot/sensor/#)     │
       │── PUBLISH ──────────────────>  │                           │
       │   topic: robot/sensor/suhu     │── PUBLISH ──────────────> │
       │   payload: "25.5"              │   topic: robot/sensor/suhu│
       │                                │   payload: "25.5"         │
```

### 6.4 Quality of Service (QoS)

| QoS | Nama | Pengiriman | Use Case |
|-----|------|-----------|----------|
| 0 | At most once | Fire and forget | Sensor data non-kritis |
| 1 | At least once | Acknowledged, mungkin duplikat | Kontrol aktuator |
| 2 | Exactly once | Full handshake, tanpa duplikat | Transaksi kritis |

### 6.5 Topic Naming Convention

```
robot/sensor/suhu          → data suhu
robot/sensor/jarak/depan   → jarak HC-SR04 depan
robot/aktuator/motor/kiri  → kontrol motor kiri
robot/aktuator/led         → kontrol LED
robot/status               → status koneksi robot
```

Wildcard:
- `+` → single level: `robot/sensor/+` → semua sensor
- `#` → multi level: `robot/#` → semua data robot

### 6.6 Retain Message dan Last Will

- **Retain** → broker menyimpan pesan terakhir, subscriber baru langsung dapat
- **Last Will and Testament (LWT)** → pesan otomatis saat client disconnect

```
LWT Topic: robot/status
LWT Payload: "offline"
LWT QoS: 1
LWT Retain: true
```

---

## 7. MQTT BROKER – MOSQUITTO

### 7.1 Apa itu Mosquitto?

**Eclipse Mosquitto** adalah MQTT broker open-source yang ringan dan banyak digunakan di IoT. Dapat dijalankan di:
- Ubuntu/Linux server
- Raspberry Pi
- Windows (development)
- Docker container

### 7.2 Instalasi Mosquitto di Ubuntu

```bash
sudo apt update
sudo apt install -y mosquitto mosquitto-clients
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
```

### 7.3 Konfigurasi Dasar

File: `/etc/mosquitto/mosquitto.conf`

```
listener 1883
allow_anonymous true
```

Restart:
```bash
sudo systemctl restart mosquitto
```

### 7.4 Test dengan CLI

Terminal 1 (Subscribe):
```bash
mosquitto_sub -h localhost -t "robot/sensor/#" -v
```

Terminal 2 (Publish):
```bash
mosquitto_pub -h localhost -t "robot/sensor/suhu" -m "25.5"
```

### 7.5 Public Broker untuk Testing

| Broker | Host | Port |
|--------|------|------|
| Eclipse | mqtt.eclipseprojects.io | 1883 |
| HiveMQ | broker.hivemq.com | 1883 |
| EMQX | broker.emqx.io | 1883 |

---

## 8. PUBSUBCLIENT LIBRARY UNTUK ESP32

### 8.1 Instalasi Library

Di Arduino IDE: **Sketch > Include Library > Manage Libraries** → cari `PubSubClient` → Install

### 8.2 Kode ESP32 MQTT Publisher

```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SSID";
const char* password = "PASS";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("ESP32Robot")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  
  float suhu = 25.5; // ganti dengan pembacaan sensor
  char msg[50];
  snprintf(msg, 50, "%.1f", suhu);
  client.publish("robot/sensor/suhu", msg);
  
  delay(2000);
}
```

### 8.3 Kode ESP32 MQTT Subscriber

```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Topic: " + String(topic) + " | Msg: " + message);
  
  if (String(topic) == "robot/aktuator/led") {
    if (message == "ON") digitalWrite(2, HIGH);
    if (message == "OFF") digitalWrite(2, LOW);
  }
}

void setup() {
  // ... WiFi setup ...
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Robot")) {
      client.subscribe("robot/aktuator/#");
    } else {
      delay(5000);
    }
  }
}
```

---

## 9. INTEGRASI WEBSERVER DAN MQTT

### 9.1 Arsitektur Gabungan

ESP32 bisa menjalankan Web Server dan MQTT client **secara bersamaan**:

```
Browser ──HTTP──> [ESP32 Web Server] ──MQTT──> [Broker]
                       │                          │
                  Kontrol LED               Subscriber lain
                  Lihat sensor              (Dashboard, App)
```

### 9.2 Contoh Integrasi

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>

WebServer server(80);
WiFiClient espClient;
PubSubClient mqtt(espClient);

float sensorValue = 0;

void handleRoot() {
  String html = "<html><body><h1>Robot Dashboard</h1>";
  html += "<p>Sensor: " + String(sensorValue) + "</p>";
  html += "<p><a href='/publish'>Publish to MQTT</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handlePublish() {
  char msg[50];
  snprintf(msg, 50, "%.1f", sensorValue);
  mqtt.publish("robot/sensor/data", msg);
  server.send(200, "text/html", "<p>Published!</p><a href='/'>Back</a>");
}

void setup() {
  WiFi.begin("SSID", "PASS");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  mqtt.setServer("broker.hivemq.com", 1883);
  
  server.on("/", handleRoot);
  server.on("/publish", handlePublish);
  server.begin();
}

void loop() {
  server.handleClient();
  if (!mqtt.connected()) {
    mqtt.connect("ESP32Robot");
  }
  mqtt.loop();
  sensorValue = analogRead(34) * 3.3 / 4095.0;
}
```

---

## 10. DASHBOARD IoT

### 10.1 Opsi Dashboard

| Platform | Keterangan | Harga |
|---------|-----------|-------|
| **Node-RED** | Flow-based, mendukung MQTT native | Gratis |
| **Grafana** | Visualisasi data time-series | Gratis (self-hosted) |
| **ThingsBoard** | Platform IoT lengkap | Community gratis |
| **HTML + JavaScript** | Custom dashboard di ESP32 | Gratis |
| **MQTT Explorer** | Desktop client untuk debug MQTT | Gratis |

### 10.2 HTML Dashboard di ESP32


```html
<!DOCTYPE html>
<html>
<head>
  <title>Robot IoT</title>
  <script>
    setInterval(function() {
      fetch('/sensor')
        .then(response => response.json())
        .then(data => {
          document.getElementById('suhu').innerHTML = data.suhu;
          document.getElementById('jarak').innerHTML = data.jarak;
        });
    }, 1000);
  </script>
</head>
<body>
  <h1>Robot IoT Dashboard</h1>
  <p>Suhu: <span id="suhu">--</span> °C</p>
  <p>Jarak: <span id="jarak">--</span> cm</p>
  <button onclick="fetch('/led/on')">LED ON</button>
  <button onclick="fetch('/led/off')">LED OFF</button>
</body>
</html>
```

---

## 11. KEAMANAN IoT DASAR

### 11.1 Risiko Keamanan

| Risiko | Dampak |
|--------|--------|
| Open broker tanpa auth | Siapa saja bisa subscribe/publish |
| Data tidak terenkripsi | Sniffing data sensor |
| Firmware tanpa update | Eksploitasi vulnerability |
| Credential hardcoded | Password bocor dari source code |

### 11.2 Langkah Mitigasi

1. **Aktifkan autentikasi MQTT** (username/password)
2. **Gunakan TLS/SSL** untuk enkripsi (port 8883)
3. **Batasi topic access** dengan ACL di broker
4. **Gunakan unique client ID** per device
5. **Jangan hardcode credential** — gunakan config file atau NVS

---

## 12. REFERENSI

1. OASIS. (2014). *MQTT Version 3.1.1 Specification*. http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html
2. Espressif. (2024). *ESP32 Technical Reference Manual*. https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf
3. Espressif. (2024). *ESP32 Arduino Core Documentation*. https://docs.espressif.com/projects/arduino-esp32/en/latest/
4. O'Leary, N. (2024). *PubSubClient Library*. https://pubsubclient.knolleary.net/
5. Espressif. (2024). *ESP32 HTTP Server*. https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_server.html
6. Eclipse Foundation. (2024). *Mosquitto MQTT Broker*. https://mosquitto.org/
7. Schaffner, B. (2023). *Practical IoT with MQTT*. O'Reilly Media.

---

Dokumen ini menjadi acuan utama materi Modul 04 IoT WebServer MQTT ESP32.
