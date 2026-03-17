# JOBSHEET MODUL 04: IoT WebServer MQTT ESP32

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – IoT WebServer MQTT ESP32  
**Pertemuan:** 7–8 (2 × 2 SKS)  
**Tanggal:** ___________________  
**Nama Kelompok:** ___________________  
**Anggota:**

| No | Nama | NIM |
|----|------|-----|
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:

1. Menghubungkan ESP32 ke jaringan WiFi dalam mode Station dan Access Point
2. Membangun Web Server pada ESP32 untuk menampilkan data sensor dan mengontrol aktuator
3. Memahami protokol MQTT (publish, subscribe, topic, QoS)
4. Mengonfigurasi dan menggunakan MQTT broker Mosquitto
5. Mengimplementasikan MQTT publisher dan subscriber pada ESP32
6. Mengintegrasikan Web Server dan MQTT pada satu ESP32
7. Membuat dashboard IoT berbasis HTML/JavaScript

---

## B. ALAT DAN BAHAN

| No | Komponen | Jumlah | Keterangan |
|----|---------|--------|-----------|
| 1 | ESP32 DevKit | 1 | Mikrokontroler utama |
| 2 | Kabel USB Micro/Type-C | 1 | Pemrograman dan power |
| 3 | Breadboard | 1 | Prototyping |
| 4 | LED 5mm + Resistor 220Ω | 3 | Aktuator output |
| 5 | Push Button + Resistor 10kΩ | 2 | Input digital |
| 6 | Sensor DHT22/DHT11 | 1 | Sensor suhu dan kelembaban |
| 7 | Sensor LDR + Resistor 10kΩ | 1 | Sensor cahaya (voltage divider) |
| 8 | Potensiometer 10kΩ | 1 | Input analog |
| 9 | Laptop dengan Arduino IDE | 1 | Pemrograman |
| 10 | Koneksi WiFi (router/hotspot) | 1 | Jaringan lokal |

### Software

| No | Software | Fungsi |
|----|---------|--------|
| 1 | Arduino IDE / PlatformIO | Pemrograman ESP32 |
| 2 | Mosquitto (Ubuntu) | MQTT Broker |
| 3 | MQTT Explorer | Desktop MQTT client untuk debug |
| 4 | Browser (Chrome/Firefox) | Akses Web Server |

---

## C. DASAR TEORI SINGKAT

### C.1 Alur Kerja IoT dengan ESP32

```
[Sensor] → [ESP32] → WiFi → [Web Server / MQTT Broker]
                                    │
                              [Browser / Dashboard / App]
```

### C.2 Library yang Digunakan

| Library | Fungsi |
|---------|--------|
| `WiFi.h` | Koneksi WiFi |
| `WebServer.h` | HTTP server |
| `PubSubClient.h` | MQTT client |
| `DHT.h` | Sensor DHT11/22 |
| `ArduinoJson.h` | Format JSON |

---

## D. LANGKAH KERJA

---

### PERCOBAAN 1: KONEKSI WiFi STATION MODE

**Estimasi waktu: 20 menit**

**Rangkaian:** ESP32 saja (tanpa komponen eksternal).

**Kode Program:**

```cpp
#include <WiFi.h>

const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost! Reconnecting...");
    WiFi.reconnect();
    delay(5000);
  }
  delay(10000);
  Serial.println("WiFi OK, IP: " + WiFi.localIP().toString());
}
```

**Yang harus diamati:**
1. IP Address yang didapat dari router
2. Nilai RSSI (kekuatan sinyal, semakin mendekati 0 semakin baik)
3. Waktu yang dibutuhkan untuk terkoneksi

**Checkpoint ✅:** ESP32 berhasil terhubung WiFi, IP address tampil di Serial Monitor.

---

### PERCOBAAN 2: ESP32 ACCESS POINT MODE

**Estimasi waktu: 15 menit**

**Kode Program:**

```cpp
#include <WiFi.h>

const char* ap_ssid = "Robot_IoT_AP";
const char* ap_password = "robot1234";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access Point Started!");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Serial.print("Devices connected: ");
  Serial.println(WiFi.softAPgetStationNum());
  delay(5000);
}
```

**Yang harus diamati:**
1. SSID muncul di daftar WiFi smartphone/laptop
2. IP address Access Point (default: 192.168.4.1)
3. Jumlah device yang terkoneksi

**Checkpoint ✅:** Smartphone/laptop berhasil terhubung ke AP ESP32.

---

### PERCOBAAN 3: WEB SERVER – TAMPILKAN DATA SENSOR

**Estimasi waktu: 30 menit**

**Rangkaian:** DHT22 → GPIO4, LDR → GPIO34 (ADC)

```
DHT22:  VCC→3.3V, DATA→GPIO4 (pull-up 10kΩ), GND→GND
LDR:    VCC→3.3V ── LDR ── GPIO34 ── R10kΩ ── GND
```

**Kode Program:**

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define LDR_PIN 34

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

const char* ssid = "SSID";
const char* password = "PASS";

void handleRoot() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();
  int cahaya = analogRead(LDR_PIN);
  
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>Robot Sensor</title></head><body>";
  html += "<h1>Dashboard Sensor Robot</h1>";
  html += "<table border='1'><tr><th>Sensor</th><th>Nilai</th></tr>";
  html += "<tr><td>Suhu</td><td>" + String(suhu) + " °C</td></tr>";
  html += "<tr><td>Kelembaban</td><td>" + String(kelembaban) + " %</td></tr>";
  html += "<tr><td>Cahaya (ADC)</td><td>" + String(cahaya) + "</td></tr>";
  html += "</table></body></html>";
  
  server.send(200, "text/html", html);
}

void handleJson() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();
  int cahaya = analogRead(LDR_PIN);
  
  String json = "{\"suhu\":" + String(suhu) + ",\"kelembaban\":" + String(kelembaban) + ",\"cahaya\":" + String(cahaya) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("IP: " + WiFi.localIP().toString());
  
  server.on("/", handleRoot);
  server.on("/api/sensor", handleJson);
  server.begin();
}

void loop() {
  server.handleClient();
}
```

**Yang harus diamati:**
1. Buka browser → ketik IP ESP32 → dashboard tampil
2. Buka IP/api/sensor → data JSON tampil
3. Data auto-refresh setiap 5 detik

**Checkpoint ✅:** Browser menampilkan data sensor real-time dari ESP32.

---

### PERCOBAAN 4: WEB SERVER – KONTROL LED

**Estimasi waktu: 25 menit**

**Rangkaian:** Tambahkan 3 LED ke GPIO2, GPIO15, GPIO13 (masing-masing dengan R220Ω)

**Kode Program:**

```cpp
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);
const int ledPins[] = {2, 15, 13};
bool ledStates[] = {false, false, false};

const char* ssid = "SSID";
const char* password = "PASS";

String generateHTML() {
  String html = "<!DOCTYPE html><html><head><title>LED Control</title>";
  html += "<style>body{font-family:Arial;text-align:center;} ";
  html += ".btn{padding:15px 30px;margin:10px;font-size:18px;border:none;border-radius:8px;cursor:pointer;} ";
  html += ".on{background:#4CAF50;color:white;} .off{background:#f44336;color:white;}</style></head>";
  html += "<body><h1>Kontrol LED Robot</h1>";
  
  for (int i = 0; i < 3; i++) {
    html += "<div><h3>LED " + String(i+1) + " (GPIO" + String(ledPins[i]) + ")</h3>";
    if (ledStates[i]) {
      html += "<a href='/led/" + String(i) + "/off'><button class='btn on'>ON → Matikan</button></a>";
    } else {
      html += "<a href='/led/" + String(i) + "/on'><button class='btn off'>OFF → Nyalakan</button></a>";
    }
    html += "</div>";
  }
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  server.on("/", [](){ server.send(200, "text/html", generateHTML()); });
  
  for (int i = 0; i < 3; i++) {
    String pathOn = "/led/" + String(i) + "/on";
    String pathOff = "/led/" + String(i) + "/off";
    int idx = i;
    server.on(pathOn.c_str(), [idx](){
      ledStates[idx] = true;
      digitalWrite(ledPins[idx], HIGH);
      server.sendHeader("Location", "/");
      server.send(303);
    });
    server.on(pathOff.c_str(), [idx](){
      ledStates[idx] = false;
      digitalWrite(ledPins[idx], LOW);
      server.sendHeader("Location", "/");
      server.send(303);
    });
  }
  
  server.begin();
}

void loop() {
  server.handleClient();
}
```

**Yang harus diamati:**
1. Setiap LED bisa dinyalakan/matikan dari browser
2. Status LED konsisten antara tampilan web dan hardware
3. Responsivitas kontrol (latency)

**Checkpoint ✅:** 3 LED berhasil dikontrol dari browser.

---

### PERCOBAAN 5: MQTT PUBLISHER – KIRIM DATA SENSOR

**Estimasi waktu: 30 menit**

**Prasyarat:** Mosquitto broker berjalan di Ubuntu (`sudo systemctl start mosquitto`)

**Kode Program:**

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

const char* ssid = "SSID";
const char* password = "PASS";
const char* mqtt_server = "IP_UBUNTU_ANDA"; // misal 192.168.1.100

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastMsg = 0;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("ESP32_Pub", NULL, NULL, "robot/status", 1, true, "offline")) {
      Serial.println("connected");
      client.publish("robot/status", "online", true);
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi OK: " + WiFi.localIP().toString());
  
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  
  if (millis() - lastMsg > 2000) {
    lastMsg = millis();
    
    float suhu = dht.readTemperature();
    float kelembaban = dht.readHumidity();
    int cahaya = analogRead(34);
    
    char msg[100];
    snprintf(msg, 100, "{\"suhu\":%.1f,\"kelembaban\":%.1f,\"cahaya\":%d}", suhu, kelembaban, cahaya);
    
    client.publish("robot/sensor/suhu", String(suhu).c_str());
    client.publish("robot/sensor/kelembaban", String(kelembaban).c_str());
    client.publish("robot/sensor/cahaya", String(cahaya).c_str());
    client.publish("robot/sensor/json", msg);
    
    Serial.println("Published: " + String(msg));
  }
}
```

**Verifikasi di Ubuntu:**
```bash
mosquitto_sub -h localhost -t "robot/sensor/#" -v
```

**Yang harus diamati:**
1. Pesan muncul di terminal subscriber Ubuntu
2. Interval pengiriman 2 detik
3. Format JSON benar
4. Last Will (LWT) berfungsi saat ESP32 dicabut

**Checkpoint ✅:** Data sensor terkirim ke MQTT broker dan diterima subscriber.

---

### PERCOBAAN 6: MQTT SUBSCRIBER – KONTROL AKTUATOR

**Estimasi waktu: 25 menit**

**Kode Program (tambahkan callback ke Percobaan 5):**

```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
  
  Serial.println("Received [" + String(topic) + "]: " + message);
  
  if (String(topic) == "robot/aktuator/led1") {
    digitalWrite(2, message == "ON" ? HIGH : LOW);
  }
  if (String(topic) == "robot/aktuator/led2") {
    digitalWrite(15, message == "ON" ? HIGH : LOW);
  }
  if (String(topic) == "robot/aktuator/led3") {
    digitalWrite(13, message == "ON" ? HIGH : LOW);
  }
}

// Di setup(), tambahkan:
// client.setCallback(callback);

// Di reconnect(), setelah connect berhasil:
// client.subscribe("robot/aktuator/#");
```

**Kontrol dari Ubuntu:**
```bash
mosquitto_pub -h localhost -t "robot/aktuator/led1" -m "ON"
mosquitto_pub -h localhost -t "robot/aktuator/led1" -m "OFF"
```

**Yang harus diamati:**
1. LED menyala/mati sesuai perintah MQTT
2. Serial monitor menampilkan pesan yang diterima
3. QoS delivery — apakah pesan selalu sampai?

**Checkpoint ✅:** LED dikontrol via MQTT dari terminal Ubuntu.

---

### PERCOBAAN 7: INTEGRASI WEBSERVER + MQTT + DASHBOARD HTML

**Estimasi waktu: 40 menit**

Gabungkan Web Server dan MQTT client dalam satu ESP32. Buat halaman HTML dashboard yang menampilkan data sensor dan kontrol LED.

**Kode Program (kerangka utama):**

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <DHT.h>

WebServer server(80);
WiFiClient espClient;
PubSubClient mqtt(espClient);
DHT dht(4, DHT22);

float lastSuhu = 0, lastHum = 0;
int lastCahaya = 0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html><head><title>Robot IoT Dashboard</title>
<style>
  body { font-family: Arial; text-align: center; background: #1a1a2e; color: #eee; }
  .card { display: inline-block; margin: 10px; padding: 20px; background: #16213e; 
          border-radius: 10px; min-width: 150px; }
  .value { font-size: 28px; font-weight: bold; color: #0f3460; }
  button { padding: 10px 20px; margin: 5px; border: none; border-radius: 5px; 
           cursor: pointer; font-size: 16px; }
  .on { background: #4CAF50; color: white; }
  .off { background: #f44336; color: white; }
</style>
<script>
  setInterval(() => {
    fetch('/api/sensor').then(r=>r.json()).then(d => {
      document.getElementById('suhu').textContent = d.suhu.toFixed(1);
      document.getElementById('hum').textContent = d.kelembaban.toFixed(1);
      document.getElementById('ldr').textContent = d.cahaya;
    });
  }, 2000);
  function toggleLed(n, state) {
    fetch('/api/led?id=' + n + '&state=' + state);
  }
</script>
</head><body>
  <h1>🤖 Robot IoT Dashboard</h1>
  <div class="card"><h3>Suhu</h3><div class="value"><span id="suhu">--</span> °C</div></div>
  <div class="card"><h3>Kelembaban</h3><div class="value"><span id="hum">--</span> %</div></div>
  <div class="card"><h3>Cahaya</h3><div class="value"><span id="ldr">--</span></div></div>
  <br>
  <h2>Kontrol LED</h2>
  <button class="on" onclick="toggleLed(1,'ON')">LED1 ON</button>
  <button class="off" onclick="toggleLed(1,'OFF')">LED1 OFF</button><br>
  <button class="on" onclick="toggleLed(2,'ON')">LED2 ON</button>
  <button class="off" onclick="toggleLed(2,'OFF')">LED2 OFF</button><br>
  <button class="on" onclick="toggleLed(3,'ON')">LED3 ON</button>
  <button class="off" onclick="toggleLed(3,'OFF')">LED3 OFF</button>
</body></html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(2, OUTPUT); pinMode(15, OUTPUT); pinMode(13, OUTPUT);
  
  WiFi.begin("SSID", "PASS");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  mqtt.setServer("BROKER_IP", 1883);
  
  server.on("/", [](){ server.send_P(200, "text/html", index_html); });
  server.on("/api/sensor", [](){
    lastSuhu = dht.readTemperature();
    lastHum = dht.readHumidity();
    lastCahaya = analogRead(34);
    String json = "{\"suhu\":" + String(lastSuhu) + ",\"kelembaban\":" + String(lastHum) + ",\"cahaya\":" + String(lastCahaya) + "}";
    server.send(200, "application/json", json);
  });
  server.on("/api/led", [](){
    int id = server.arg("id").toInt();
    String state = server.arg("state");
    int pins[] = {2, 15, 13};
    if (id >= 1 && id <= 3) {
      digitalWrite(pins[id-1], state == "ON" ? HIGH : LOW);
      mqtt.publish(("robot/aktuator/led" + String(id)).c_str(), state.c_str());
    }
    server.send(200, "text/plain", "OK");
  });
  
  server.begin();
}

void loop() {
  server.handleClient();
  if (!mqtt.connected()) mqtt.connect("ESP32_Full");
  mqtt.loop();
}
```

**Yang harus diamati:**
1. Dashboard HTML menampilkan data sensor real-time
2. Kontrol LED dari dashboard juga publish ke MQTT
3. Subscriber di Ubuntu menerima data dari kedua sumber (web + MQTT)

**Checkpoint ✅:** Dashboard IoT lengkap berfungsi dengan WebServer + MQTT.

---

## E. ANALISA PERCOBAAN

### Analisa Percobaan 1 – WiFi Station Mode

1. Berapa waktu rata-rata koneksi WiFi? Faktor apa yang mempengaruhinya?
2. Apa arti nilai RSSI? Berapa range RSSI yang baik untuk komunikasi stabil?
3. Apa yang terjadi jika password WiFi salah? Bagaimana ESP32 menanganinya?

### Analisa Percobaan 2 – Access Point Mode

1. Apa perbedaan Station dan AP mode? Kapan masing-masing digunakan?
2. Berapa device maksimal yang bisa terkoneksi ke AP ESP32?
3. Mengapa IP default AP adalah 192.168.4.1? Bisakah diubah?

### Analisa Percobaan 3 – Web Server Sensor

1. Jelaskan perbedaan endpoint HTML (`/`) dan JSON (`/api/sensor`). Kapan masing-masing digunakan?
2. Mengapa digunakan `meta refresh` untuk auto-update? Apa kelemahannya dibanding AJAX/fetch?
3. Apa yang terjadi jika 2 browser mengakses web server ESP32 bersamaan?

### Analisa Percobaan 4 – Web Server LED Control

1. Jelaskan mengapa digunakan HTTP redirect (303) setelah toggle LED.
2. Apa perbedaan metode GET dan POST untuk kontrol aktuator? Mana yang lebih aman?
3. Bagaimana menambahkan PWM slider (bukan hanya ON/OFF)?

### Analisa Percobaan 5 – MQTT Publisher

1. Jelaskan fungsi Last Will and Testament (LWT) pada kode. Kapan pesan LWT dikirim?
2. Apa perbedaan mengirim data per-topic vs satu topic JSON? Kapan masing-masing digunakan?
3. Bagaimana mengatur QoS pada PubSubClient? Apa dampaknya?

### Analisa Percobaan 6 – MQTT Subscriber

1. Jelaskan mekanisme callback pada PubSubClient. Kapan fungsi callback dipanggil?
2. Apa fungsi wildcard `#` pada `robot/aktuator/#`? Apa bedanya dengan wildcard `+`?
3. Apa yang terjadi jika ESP32 disconnect lalu reconnect — apakah pesan yang dikirim saat offline hilang?

### Analisa Percobaan 7 – Integrasi WebServer + MQTT

1. Jelaskan mengapa `PROGMEM` digunakan untuk menyimpan HTML. Apa keuntungannya?
2. Bandingkan latency kontrol LED via Web Server vs via MQTT. Mana yang lebih cepat? Mengapa?
3. Gambarkan diagram arsitektur lengkap sistem IoT yang Anda buat (ESP32, browser, broker, subscriber).

---

## F. KESIMPULAN

Tuliskan minimal 5 poin kesimpulan:

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## G. LAMPIRAN WAJIB

- [ ] Foto rangkaian di breadboard
- [ ] Screenshot Serial Monitor (WiFi connected + MQTT connected)
- [ ] Screenshot browser Web Server (sensor + LED control)
- [ ] Screenshot terminal Ubuntu subscriber
- [ ] Screenshot Dashboard IoT lengkap (Percobaan 7)
- [ ] Screenshot MQTT Explorer (opsional)
- [ ] Kode program lengkap setiap percobaan

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
