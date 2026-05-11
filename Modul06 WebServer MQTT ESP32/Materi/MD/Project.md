# Project Ideas — Modul 6: WebServer MQTT ESP32
**Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi**  
**Dosen: Rofiq Cahyo Prayogo, S.T., M.T.**

---

## Panduan Memilih Project

Pilih **satu project** dari 10 pilihan berikut sesuai minat dan kemampuan.  
Setiap project memiliki label tingkat kesulitan:

| Level | Simbol | Waktu Pengerjaan | Deskripsi |
|-------|--------|------------------|-----------|
| Pemula | ⭐ | 1-2 hari | Modifikasi kode jobsheet dengan tambahan fitur |
| Menengah | ⭐⭐ | 3-5 hari | Kombinasi sensor baru + arsitektur lebih kompleks |
| Lanjutan | ⭐⭐⭐ | 1-2 minggu | Sistem lengkap dengan cloud, auth, dan UI profesional |

---

## PROJECT 1 — Smart Home Environmental Monitor ⭐

### Deskripsi
Sistem monitoring lingkungan rumah berbasis ESP32 yang menampilkan data suhu, kelembaban, dan kualitas udara secara real-time melalui WebServer lokal dengan update otomatis tanpa refresh.

### Tujuan Pembelajaran
- Implementasi Server-Sent Events (SSE) untuk update real-time
- Penggunaan multiple sensor pada satu ESP32
- Desain dashboard responsif dengan Bootstrap

### Komponen Tambahan
| Komponen | Fungsi |
|----------|--------|
| DHT22 | Suhu + Kelembaban |
| MQ-135 | Kualitas udara (CO2, NH3) |
| BMP280 | Tekanan udara |
| LED RGB | Indikator visual kualitas udara |

### Fitur yang Dibangun
- [ ] Dashboard dengan 4 gauge: suhu, kelembaban, tekanan, AQI
- [ ] Grafik historis 30 menit terakhir
- [ ] Alert visual (LED berubah warna) saat nilai melebihi threshold
- [ ] Export data ke CSV via endpoint `/export`
- [ ] Notifikasi alert ke MQTT topic `home/alert`

### Rumus yang Diimplementasikan

**Air Quality Index (AQI) Sederhana:**
$$AQI = \frac{I_{high} - I_{low}}{C_{high} - C_{low}} \times (C - C_{low}) + I_{low}$$

**Indeks Kenyamanan Ruangan (Heat Index):**
$$HI = -42.379 + 2.04901523T + 10.14333127H - 0.22475541TH$$
$$- 0.00683783T^2 - 0.05481717H^2 + 0.00122874T^2H + 0.00085282TH^2 - 0.00000199T^2H^2$$

Di mana $T$ = suhu (°F), $H$ = kelembaban relatif (%)

### Struktur Kode
```
project1/
├── project1.ino          (main program)
├── data/
│   ├── index.html        (dashboard)
│   ├── app.js            (Chart.js + SSE)
│   └── style.css
└── src/
    ├── sensors.h          (abstraksi sensor)
    └── webhandlers.h      (route handlers)
```

### Snippet Kode Penting

```cpp
// Air Quality Monitoring
int aqiFromPPM(float ppm) {
  // Simplified AQI dari MQ-135 (CO2 equivalent)
  if (ppm < 400)  return map(ppm, 0, 400, 0, 50);    // Good
  if (ppm < 1000) return map(ppm, 400, 1000, 50, 100); // Moderate
  if (ppm < 2000) return map(ppm, 1000, 2000, 100, 150); // Unhealthy
  return 200; // Very Unhealthy
}

// Heat Index Calculation (simplified Rothfusz)
float calculateHeatIndex(float tempC, float humidity) {
  float T = tempC * 9.0 / 5.0 + 32; // Convert to Fahrenheit
  float H = humidity;
  float HI = -42.379 + 2.04901523*T + 10.14333127*H
             - 0.22475541*T*H - 0.00683783*T*T
             - 0.05481717*H*H + 0.00122874*T*T*H
             + 0.00085282*T*H*H - 0.00000199*T*T*H*H;
  return (HI - 32) * 5.0 / 9.0; // Convert back to Celsius
}
```

---

## PROJECT 2 — IoT Weather Station dengan Cloud Upload ⭐⭐

### Deskripsi
Stasiun cuaca lengkap yang mengumpulkan data meteorologi dari multiple sensor, menyimpan ke SPIFFS, menampilkan di web lokal, dan secara periodik mengirim ke ThingSpeak atau InfluxDB Cloud.

### Komponen Tambahan
| Komponen | Parameter |
|----------|-----------|
| BME280 | Suhu, Kelembaban, Tekanan |
| BH1750 | Intensitas cahaya (lux) |
| Anemometer DIY | Kecepatan angin (opsional) |
| SD Card Module | Storage data long-term |

### Fitur Utama
- [ ] Data logging ke SD Card setiap 1 menit (format CSV)
- [ ] Sinkronisasi waktu via NTP server
- [ ] Upload ke ThingSpeak setiap 15 menit
- [ ] API endpoint `/api/history?range=24h` untuk ambil data historis
- [ ] Alert cuaca ekstrem via MQTT

### Rumus Meteorologi

**Tekanan ke Altitude:**
$$h = \frac{T_0}{L}\left[\left(\frac{P}{P_0}\right)^{-\frac{RL}{gM}} - 1\right]$$

Di mana:
- $T_0 = 288.15$ K (suhu standar)
- $L = 0.0065$ K/m (lapse rate)
- $P_0 = 101325$ Pa (tekanan standar)
- $R = 8.31446$ J/(mol·K)
- $g = 9.80665$ m/s²
- $M = 0.0289644$ kg/mol

**Dew Point (Titik Embun):**
$$T_{dp} = \frac{b \cdot \gamma(T, RH)}{a - \gamma(T, RH)}$$

$$\gamma(T, RH) = \frac{a \cdot T}{b + T} + \ln\left(\frac{RH}{100}\right)$$

Di mana $a = 17.625$, $b = 243.04$ °C

```cpp
float dewPoint(float tempC, float humidity) {
  float a = 17.625, b = 243.04;
  float gamma = (a * tempC / (b + tempC)) + log(humidity / 100.0);
  return (b * gamma) / (a - gamma);
}
```

### Kode NTP Time Sync
```cpp
#include <time.h>

void setupNTP() {
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("NTP Sync failed");
    return;
  }
  Serial.printf("NTP: %04d-%02d-%02d %02d:%02d:%02d\n",
    timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday,
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

String getTimestamp() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  char buf[20];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  return String(buf);
}
```

---

## PROJECT 3 — Smart LED Strip Controller ⭐

### Deskripsi
Kontroler LED WS2812B (NeoPixel) berbasis ESP32 dengan antarmuka web modern yang mendukung mode warna, animasi, dan kontrol via MQTT untuk integrasi smart home.

### Komponen Tambahan
| Komponen | Jumlah |
|----------|--------|
| LED WS2812B Strip | 30 LED |
| Kapasitor 1000μF 6.3V | 1 |
| Resistor 470Ω | 1 |
| Power Supply 5V 2A | 1 |

### Fitur Utama
- [ ] Color picker di web UI (pilih warna RGB)
- [ ] Mode animasi: Rainbow, Breathing, Police, Strobe
- [ ] Kecerahan adjustable (0-255)
- [ ] Schedule otomatis (nyala jam 18:00, mati jam 23:00)
- [ ] Voice command via MQTT dari aplikasi HP

### Rumus PWM & Warna

**Gamma Correction (untuk persepsi visual lebih baik):**
$$V_{gamma} = 255 \times \left(\frac{V_{input}}{255}\right)^{2.2}$$

**HSV ke RGB Conversion:**
```
H: 0-360 (hue)
S: 0-1 (saturation)
V: 0-1 (value/brightness)

C = V × S
X = C × (1 - |H/60 mod 2 - 1|)
m = V - C
```

```cpp
#include <FastLED.h>

#define LED_PIN   5
#define NUM_LEDS  30

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
}

// Set warna dari hex string "#FF5733"
void setColorFromHex(const String& hex) {
  long color = strtol(hex.c_str() + 1, NULL, 16);
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;
  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
}

// Animasi Rainbow
void rainbowAnimation(uint8_t speed) {
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, hue, 7);
  FastLED.show();
  hue += speed;
  delay(20);
}
```

---

## PROJECT 4 — Multi-Node Sensor Network ⭐⭐

### Deskripsi
Jaringan beberapa ESP32 sebagai sensor node yang publish data ke satu MQTT broker terpusat, dengan gateway ESP32 yang mengagregasi semua data dan menampilkan di dashboard terpadu.

### Arsitektur Sistem
```
[Node 1: Ruang A] ──MQTT──┐
[Node 2: Ruang B] ──MQTT──┤──[Broker]──[Gateway ESP32]──[Dashboard]
[Node 3: Gudang]  ──MQTT──┘
```

### Konfigurasi Topic Hierarki
```
factory/
├── node01/
│   ├── sensor      → {"temp":28.5,"hum":65}
│   ├── status      → "online"
│   └── control     → {"led":true}
├── node02/
│   └── sensor      → {...}
└── gateway/
    ├── all_nodes   → {"nodes": [...]}
    └── alert       → {"node":"node01","type":"high_temp"}
```

### Fitur Gateway
- [ ] Subscribe `factory/+/sensor` untuk agregasi semua node
- [ ] Deteksi node offline via LWT
- [ ] Alert otomatis jika suhu > threshold
- [ ] Dashboard menampilkan map lokasi setiap node
- [ ] Data persistence di SPIFFS (rolling buffer 1 jam)

### Kode Gateway — Agregasi Multi-Node

```cpp
// Gateway menggunakan wildcard subscribe
mqttClient.subscribe("factory/+/sensor");  // Subscribe semua node

struct NodeData {
  String nodeId;
  float temp, hum;
  unsigned long lastSeen;
  bool online;
};

std::map<String, NodeData> nodeRegistry;

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  // Ekstrak node ID dari topic
  String topicStr = String(topic);
  // topic format: factory/nodeXX/sensor
  int start = topicStr.indexOf('/') + 1;
  int end = topicStr.indexOf('/', start);
  String nodeId = topicStr.substring(start, end);
  
  // Update registry
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload, len);
  
  nodeRegistry[nodeId] = {
    nodeId,
    doc["temp"].as<float>(),
    doc["hum"].as<float>(),
    millis(),
    true
  };
  
  // Publish agregasi ke gateway topic
  publishAggregation();
}
```

---

## PROJECT 5 — Smart Door Lock dengan Autentikasi ⭐⭐

### Deskripsi
Sistem kunci pintu cerdas menggunakan servo motor yang dikontrol via WebServer (dengan password) dan MQTT, dilengkapi log akses dan notifikasi.

### Komponen
| Komponen | Fungsi |
|----------|--------|
| Servo SG90 | Buka/tutup kunci |
| RFID MFRC522 | Kartu akses |
| LCD I2C 16x2 | Status display |
| Buzzer | Alarm |
| Kamera OV2640 (opsional) | Foto saat akses |

### Fitur Keamanan
- [ ] Login page di WebServer dengan session token (JWT sederhana)
- [ ] Whitelist kartu RFID di SPIFFS
- [ ] Log akses dengan timestamp
- [ ] Alert MQTT saat akses ditolak 3 kali berturut
- [ ] Auto-lock timer (kunci kembali setelah 10 detik)

### Implementasi Autentikasi Sederhana

```cpp
#include <Servo.h>

// Hash password sederhana (untuk demo, produksi gunakan bcrypt)
String hashPassword(const String& password) {
  uint32_t hash = 5381;
  for (size_t i = 0; i < password.length(); i++) {
    hash = ((hash << 5) + hash) + password[i];
  }
  return String(hash, HEX);
}

// Session token
String sessionTokens[5];  // Max 5 sesi aktif
int tokenIndex = 0;

String generateToken() {
  String token = "";
  for (int i = 0; i < 16; i++) {
    token += String(random(0, 16), HEX);
  }
  sessionTokens[tokenIndex++ % 5] = token;
  return token;
}

bool validateToken(const String& token) {
  for (int i = 0; i < 5; i++) {
    if (sessionTokens[i] == token) return true;
  }
  return false;
}

// Route: POST /api/login
// Body: {"password":"secret"}
// Response: {"token":"abc123..."} atau {"error":"Wrong password"}
```

### Rumus Keamanan

**Entropy Token (bit):**
$$H = \log_2(N^L)$$

Di mana:
- $N$ = ukuran alfabet (16 untuk hex)
- $L$ = panjang token (16 karakter)

$$H = \log_2(16^{16}) = 16 \times \log_2(16) = 16 \times 4 = 64 \text{ bit}$$

---

## PROJECT 6 — Industrial Equipment Monitor ⭐⭐⭐

### Deskripsi
Sistem monitoring peralatan industri real-time yang membaca sensor arus, tegangan, dan vibrasi, menganalisis kondisi, dan mengirim alert prediktif sebelum kerusakan terjadi.

### Komponen
| Komponen | Fungsi |
|----------|--------|
| ACS712 30A | Sensor arus AC/DC |
| ZMPT101B | Sensor tegangan AC |
| ADXL345 | Sensor akselerometer/vibrasi |
| Relay 5V | Kontrol beban |
| Buzzer | Alarm lokal |

### Fitur Monitoring
- [ ] Hitung daya aktif, reaktif, dan power factor
- [ ] FFT vibrasi untuk deteksi anomali frekuensi
- [ ] Threshold alarm multi-level (Warning, Critical)
- [ ] Data logging ke CSV per jam
- [ ] Grafik trend 24 jam terakhir
- [ ] Prediksi maintenance berbasis historical data

### Rumus Elektrik

**Daya Aktif (Watt):**
$$P = V_{rms} \times I_{rms} \times \cos\phi$$

**Daya Reaktif (VAR):**
$$Q = V_{rms} \times I_{rms} \times \sin\phi$$

**Daya Semu (VA):**
$$S = V_{rms} \times I_{rms} = \sqrt{P^2 + Q^2}$$

**Power Factor:**
$$PF = \frac{P}{S} = \cos\phi$$

**Nilai RMS dari Sampling ADC:**
$$V_{rms} = \sqrt{\frac{1}{N}\sum_{i=1}^{N} v_i^2}$$

```cpp
// Hitung V_rms dan I_rms dari ADC samples
float calculateRMS(int pin, int samples, float calibration) {
  long sumSquares = 0;
  for (int i = 0; i < samples; i++) {
    int raw = analogRead(pin);
    int centered = raw - 2048;  // 12-bit ADC, center at 2048
    sumSquares += (long)centered * centered;
  }
  float rms_raw = sqrt((float)sumSquares / samples);
  return rms_raw * calibration;
}

// Power Factor dari phase shift
float powerFactor(float P_watt, float S_va) {
  if (S_va < 0.001) return 1.0;
  float pf = P_watt / S_va;
  return constrain(pf, -1.0, 1.0);
}
```

### Fast Fourier Transform (FFT) untuk Analisis Vibrasi

$$X[k] = \sum_{n=0}^{N-1} x[n] \cdot e^{-j2\pi kn/N}$$

```cpp
#include <arduinoFFT.h>

#define SAMPLES 256
#define SAMPLING_FREQ 1000  // Hz

double vReal[SAMPLES], vImag[SAMPLES];
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

void analyzeVibration() {
  // Ambil samples dari ADXL345
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = readAccelerometer();  // g value
    vImag[i] = 0;
    delayMicroseconds(1000000 / SAMPLING_FREQ);
  }
  
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
  
  double dominantFreq = FFT.MajorPeak();
  Serial.printf("Dominant Vibration Frequency: %.2f Hz\n", dominantFreq);
  
  // Alert jika frekuensi abnormal
  if (dominantFreq > 100 || dominantFreq < 5) {
    mqttClient.publish("factory/machine/alert", 
                       "{\"type\":\"abnormal_vibration\"}");
  }
}
```

---

## PROJECT 7 — Smart Plant Watering System ⭐

### Deskripsi
Sistem penyiraman tanaman otomatis yang memantau kelembaban tanah dan mengaktifkan pompa air secara otomatis via relay, dengan monitoring via WebServer dan MQTT.

### Komponen
| Komponen | Fungsi |
|----------|--------|
| Sensor Kelembaban Tanah | Baca kadar air tanah |
| Relay Module 5V | Kontrol pompa |
| Mini Water Pump | Pompa air |
| Selang silikon | Saluran air |
| Wadah air | Reservoir |

### Fitur Sistem
- [ ] Auto-watering saat kelembaban < threshold (50%)
- [ ] Manual override via WebServer
- [ ] Log waktu penyiraman
- [ ] Alert via MQTT saat reservoir hampir kosong
- [ ] Schedule watering (misal hanya pagi/sore)
- [ ] Grafik kelembaban tanah vs waktu

### Logika Kontrol

```cpp
#define SOIL_SENSOR_PIN  34
#define PUMP_PIN         26
#define THRESHOLD_DRY    40  // % kelembaban
#define THRESHOLD_WET    70  // % kelembaban
#define MAX_PUMP_TIME    30  // detik maksimum pompa aktif

bool pumpState = false;
unsigned long pumpStartTime = 0;

int readSoilMoisture() {
  // Sensor analog: nilai rendah = basah, nilai tinggi = kering
  // Kalibrasi: dry=3200, wet=1500 (ESP32 ADC 12-bit)
  int rawVal = analogRead(SOIL_SENSOR_PIN);
  int moistPct = map(rawVal, 3200, 1500, 0, 100);
  return constrain(moistPct, 0, 100);
}

void autoWatering() {
  int moisture = readSoilMoisture();
  
  if (!pumpState && moisture < THRESHOLD_DRY) {
    // Mulai penyiraman
    pumpState = true;
    pumpStartTime = millis();
    digitalWrite(PUMP_PIN, HIGH);
    mqttClient.publish("garden/pump", "{\"state\":true,\"trigger\":\"auto\"}");
    Serial.printf("→ Pump ON (soil: %d%%)\n", moisture);
  }
  
  if (pumpState && (moisture >= THRESHOLD_WET || 
      millis() - pumpStartTime > MAX_PUMP_TIME * 1000)) {
    // Hentikan penyiraman
    pumpState = false;
    digitalWrite(PUMP_PIN, LOW);
    mqttClient.publish("garden/pump", "{\"state\":false}");
    Serial.printf("→ Pump OFF (soil: %d%%)\n", moisture);
  }
}
```

---

## PROJECT 8 — Energy Monitor (Smart Meter) ⭐⭐

### Deskripsi
Smart meter sederhana menggunakan sensor arus dan tegangan untuk mengukur konsumsi energi rumah tangga secara real-time, dengan perhitungan biaya listrik dan dashboard monitoring.

### Komponen
| Komponen | Fungsi |
|----------|--------|
| PZEM-004T v3 | Modul monitoring daya AC (V, I, P, E, PF, Hz) |
| Optocoupler | Isolasi galvanik |
| LCD I2C 20x4 | Display lokal |

### Fitur
- [ ] Tampilkan: V, A, W, kWh, PF, Hz secara real-time
- [ ] Hitung estimasi biaya listrik (tarif PLN)
- [ ] Alert saat konsumsi > batas yang ditentukan
- [ ] Grafik konsumsi per jam/hari/bulan
- [ ] Reset counter energi via WebServer

### Rumus Energi

**Energi (kWh):**
$$E = P \times t$$

Di mana $P$ dalam kW dan $t$ dalam jam.

**Biaya Listrik:**
$$C = E \times R_{tarif}$$

**Tarif PLN (2024 — golongan R-1 900VA):**
- Tarif: Rp 1.352/kWh

```cpp
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(Serial2, 16, 17);  // RX2=16, TX2=17

struct PowerData {
  float voltage, current, power;
  float energy, frequency, pf;
};

PowerData readPZEM() {
  return {
    pzem.voltage(),    // Volt
    pzem.current(),    // Ampere
    pzem.power(),      // Watt
    pzem.energy(),     // kWh
    pzem.frequency(),  // Hz
    pzem.pf()          // Power Factor
  };
}

float estimateCost(float energyKWh) {
  const float TARIF_PLN = 1352.0;  // Rp/kWh
  return energyKWh * TARIF_PLN;
}
```

### Dashboard Visualisasi Konsumsi

**Perhitungan Efisiensi Daya:**
$$\eta_{power} = \frac{P_{aktif}}{S_{semu}} \times 100\% = PF \times 100\%$$

**Prediksi Tagihan Bulanan:**
$$C_{monthly} = \overline{P}_{harian} \times 30 \times R_{tarif}$$

Di mana $\overline{P}_{harian}$ = rata-rata konsumsi per hari (kWh)

---

## PROJECT 9 — GPS Asset Tracker dengan MQTT ⭐⭐⭐

### Deskripsi
Sistem pelacak aset berbasis GPS yang mengirimkan posisi real-time ke broker MQTT dan menampilkan di peta Leaflet.js, cocok untuk pelacakan kendaraan atau aset berharga.

### Komponen
| Komponen | Fungsi |
|----------|--------|
| Neo-6M GPS Module | Data posisi (lat, lon, alt, speed) |
| SIM800L / WiFi | Konektivitas (WiFi untuk praktikum) |
| Buzzer | Geofencing alert |
| Baterai LiPo + TP4056 | Power management |

### Fitur
- [ ] Publish posisi GPS setiap 10 detik via MQTT
- [ ] Dashboard peta dengan Leaflet.js
- [ ] Geofencing: alert saat keluar area tertentu
- [ ] Track history (rekam jalur pergerakan)
- [ ] Deep sleep saat tidak bergerak (power saving)

### Kode GPS + MQTT

```cpp
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

HardwareSerial gpsSerial(1);  // UART1
TinyGPSPlus gps;

void publishGPSData() {
  if (gps.location.isValid() && gps.location.isUpdated()) {
    StaticJsonDocument<256> doc;
    doc["device_id"]  = "tracker_01";
    doc["lat"]        = gps.location.lat();
    doc["lon"]        = gps.location.lng();
    doc["alt"]        = gps.altitude.meters();
    doc["speed_kmh"]  = gps.speed.kmph();
    doc["hdop"]       = gps.hdop.hdop();  // GPS accuracy
    doc["satellites"] = gps.satellites.value();
    doc["timestamp"]  = getTimestamp();
    
    char buf[256];
    serializeJson(doc, buf);
    mqttClient.publish("tracker/esp32/gps", buf);
    Serial.printf("GPS: %.6f, %.6f, %.1f km/h\n",
                  gps.location.lat(), gps.location.lng(),
                  gps.speed.kmph());
  }
}
```

### Algoritma Geofencing (Haversine Distance)

**Rumus Haversine (jarak dua titik di permukaan bumi):**

$$a = \sin^2\left(\frac{\Delta\phi}{2}\right) + \cos\phi_1 \cdot \cos\phi_2 \cdot \sin^2\left(\frac{\Delta\lambda}{2}\right)$$

$$c = 2 \cdot \text{atan2}\left(\sqrt{a}, \sqrt{1-a}\right)$$

$$d = R \cdot c$$

Di mana:
- $\phi$ = latitude (radians)
- $\lambda$ = longitude (radians)
- $R = 6371$ km (radius bumi)

```cpp
float haversineDistance(float lat1, float lon1, float lat2, float lon2) {
  const float R = 6371000.0;  // meter
  float dLat = (lat2 - lat1) * PI / 180.0;
  float dLon = (lon2 - lon1) * PI / 180.0;
  float a = sin(dLat/2) * sin(dLat/2) +
            cos(lat1*PI/180) * cos(lat2*PI/180) *
            sin(dLon/2) * sin(dLon/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  return R * c;  // meter
}

bool isInsideGeofence(float lat, float lon, float centerLat, float centerLon, float radiusM) {
  return haversineDistance(lat, lon, centerLat, centerLon) <= radiusM;
}
```

### Leaflet.js Map Dashboard

```html
<!-- Di dashboard HTML -->
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"/>
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<div id="map" style="height:400px;"></div>

<script>
  const map = L.map('map').setView([-7.983, 112.621], 15);
  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(map);
  
  let marker = null;
  const trackPolyline = L.polyline([], {color: 'blue'}).addTo(map);
  
  // Update dari SSE/WebSocket
  evtSource.addEventListener('gps', (e) => {
    const d = JSON.parse(e.data);
    const pos = [d.lat, d.lon];
    
    if (!marker) marker = L.marker(pos).addTo(map);
    else marker.setLatLng(pos);
    
    trackPolyline.addLatLng(pos);
    map.setView(pos);
  });
</script>
```

---

## PROJECT 10 — Multi-Protocol IoT Gateway ⭐⭐⭐

### Deskripsi
ESP32 sebagai gateway IoT yang menjembatani protokol MQTT dengan HTTP REST API, mengagregasi data dari multiple sensor nodes, dan mengirim ke cloud (Adafruit IO atau ThingSpeak). Sistem termasuk autentikasi API key dan rate limiting.

### Arsitektur Sistem Lengkap

```
[Sensor Nodes] ──MQTT──▶ [ESP32 Gateway] ──HTTP──▶ [Adafruit IO / Cloud]
                                │
                          [WebServer API]
                                │
                          [Browser Client]
                                │
                          [Node-RED Flow]
                                │
                          [Grafana Dashboard]
```

### Fitur Gateway
- [ ] MQTT Subscriber untuk semua node (`sensors/#`)
- [ ] Data aggregation dan normalisasi
- [ ] HTTP POST ke cloud setiap 1 menit (rate limit)
- [ ] REST API dengan API key authentication
- [ ] Rate limiting (max 100 req/menit per client)
- [ ] In-memory cache dengan TTL
- [ ] Webhook alert ke Telegram Bot

### Implementasi Rate Limiting

```cpp
// Rate limiter per IP address
std::map<String, RateLimit> rateLimitMap;

struct RateLimit {
  int count;
  unsigned long windowStart;
};

bool checkRateLimit(const String& ip, int maxReq = 100, int windowMs = 60000) {
  unsigned long now = millis();
  auto& rl = rateLimitMap[ip];
  
  if (now - rl.windowStart > windowMs) {
    rl.count = 0;
    rl.windowStart = now;
  }
  
  if (rl.count >= maxReq) return false;  // Rate limited
  rl.count++;
  return true;  // Allowed
}

// Di route handler:
server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *req) {
  String clientIP = req->client()->remoteIP().toString();
  if (!checkRateLimit(clientIP)) {
    req->send(429, "application/json", 
              "{\"error\":\"Too Many Requests\",\"retry_after\":60}");
    return;
  }
  // ... handle request
});
```

### Upload ke Adafruit IO

```cpp
#include <HTTPClient.h>

void uploadToAdafruitIO(float temp, float hum) {
  const String AIO_KEY   = "your_aio_key";
  const String AIO_USER  = "your_username";
  const String FEED_TEMP = "temperature";
  
  HTTPClient http;
  http.begin("https://io.adafruit.com/api/v2/" + AIO_USER + 
             "/feeds/" + FEED_TEMP + "/data");
  http.addHeader("X-AIO-Key", AIO_KEY);
  http.addHeader("Content-Type", "application/json");
  
  String payload = "{\"value\":" + String(temp, 2) + "}";
  int code = http.POST(payload);
  
  Serial.printf("Adafruit IO Response: %d\n", code);
  http.end();
}
```

### Telegram Bot Notification

```cpp
void sendTelegramAlert(const String& message) {
  const String BOT_TOKEN = "your_bot_token";
  const String CHAT_ID   = "your_chat_id";
  
  HTTPClient http;
  http.begin("https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage");
  http.addHeader("Content-Type", "application/json");
  
  StaticJsonDocument<256> doc;
  doc["chat_id"] = CHAT_ID;
  doc["text"]    = "🚨 IoT Alert: " + message;
  doc["parse_mode"] = "HTML";
  
  String payload;
  serializeJson(doc, payload);
  http.POST(payload);
  http.end();
}
```

---

## Panduan Pengerjaan Project

### Timeline Pengerjaan

| Minggu | Kegiatan |
|--------|----------|
| Hari 1 | Pilih project, rancang arsitektur sistem, buat diagram blok |
| Hari 2 | Siapkan hardware, uji komponen satu per satu |
| Hari 3 | Coding program utama, uji fungsi dasar |
| Hari 4 | Integrasi semua komponen, debugging |
| Hari 5+ | Tambah fitur lanjutan, buat dokumentasi, rekam video |

### Struktur Dokumentasi Project

Buat README.md di folder project dengan struktur:

```markdown
# Nama Project
**Nama:** [Nama Mahasiswa]
**NIM:** [NIM]
**Tanggal:** [Tanggal]

## Deskripsi
[Jelaskan apa yang dibuat]

## Arsitektur Sistem
[Diagram blok + flowchart]

## Komponen
[List komponen dengan harga]

## Instalasi & Setup
[Langkah-langkah menjalankan project]

## Demo
[Link video demo di YouTube]

## Rumus yang Diimplementasikan
[Rumus-rumus dari materi yang digunakan]

## Screenshot
[Screenshot dashboard, terminal, dll]

## Referensi
[Sumber-sumber yang digunakan]
```

### Kriteria Penilaian Project

| Aspek | Bobot | Indikator |
|-------|-------|-----------|
| Fungsionalitas | 40% | Semua fitur berjalan sesuai spesifikasi |
| Kualitas Kode | 20% | Terstruktur, terdokumentasi, tidak ada bug |
| Inovasi | 20% | Tambahan fitur di luar requirement dasar |
| Dokumentasi | 10% | README lengkap, screenshot, diagram |
| Presentasi Video | 10% | Penjelasan jelas dan demo berhasil |

---

*Pilih project yang paling sesuai dengan minat dan kemampuan Anda. Jangan ragu untuk berkreasi dan menambahkan fitur-fitur inovatif!*
