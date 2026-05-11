# Materi Modul 6: WebServer MQTT ESP32
**Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi**  
**Dosen: Rofiq Cahyo Prayogo, S.T., M.T.**  
**Praktikum Mekatronika — 2 SKS, 5 Jam 40 Menit**

---

## DAFTAR ISI

1. [Pendahuluan IoT & Arsitektur Sistem](#bab-1)
2. [Protokol HTTP & WebServer](#bab-2)
3. [Protokol MQTT Dasar](#bab-3)
4. [MQTT Lanjutan: QoS, Retain, LWT, Security](#bab-4)
5. [Arsitektur & Spesifikasi ESP32](#bab-5)
6. [WiFi pada ESP32 & Rumus Link Budget](#bab-6)
7. [Jaringan TCP/IP & WebSocket](#bab-7)
8. [JSON & Serialisasi Data](#bab-8)
9. [Keamanan IoT & Enkripsi](#bab-9)
10. [Cloud IoT & Node-RED](#bab-10)
11. [FreeRTOS & Real-Time Scheduling](#bab-11)
12. [Sampling Rate & Power Management](#bab-12)
13. [Dashboard & Visualisasi Data](#bab-13)
14. [Integrasi Sistem WebServer + MQTT](#bab-14)
15. [Studi Kasus & Best Practices](#bab-15)
16. [Rekomendasi Buku & Paper Ilmiah](#referensi)

---

## BAB 1: Pendahuluan IoT & Arsitektur Sistem {#bab-1}

### 1.1 Pengertian Internet of Things (IoT)

Internet of Things (IoT) adalah paradigma komputasi di mana objek fisik ("things") dilengkapi sensor, aktuator, dan konektivitas jaringan sehingga dapat mengumpulkan, bertukar, dan merespons data secara mandiri tanpa intervensi manusia langsung.

**Definisi Formal:**  
IoT adalah infrastruktur jaringan global yang menghubungkan objek fisik dan virtual melalui eksploitasi data capture dan komunikasi.

### 1.2 Arsitektur Tiga Layer IoT

```
┌─────────────────────────────────────────────────────────┐
│  APPLICATION LAYER  │  Dashboard, Analytics, Cloud App  │
├─────────────────────────────────────────────────────────┤
│  NETWORK LAYER      │  WiFi, MQTT, HTTP, TCP/IP, 4G     │
├─────────────────────────────────────────────────────────┤
│  PERCEPTION LAYER   │  Sensor, Aktuator, MCU (ESP32)    │
└─────────────────────────────────────────────────────────┘
```

### 1.3 Arsitektur Sistem IoT pada Praktikum Ini

```
[DHT22/Sensor] ──> [ESP32] ──WiFi──> [Router] ──> [Mosquitto Broker]
                     │                                     │
                     │                                     ▼
                  [WebServer]                         [Node-RED]
                     │                                     │
                     ▼                                     ▼
               [Browser/Client]                    [Dashboard UI]
```

### 1.4 Protokol Komunikasi IoT – Perbandingan

| Protokol | Layer    | Transport | Port        | QoS | Overhead | Use Case        |
|----------|----------|-----------|-------------|-----|----------|-----------------|
| HTTP     | App      | TCP       | 80/443      | -   | Tinggi   | REST API, Web   |
| MQTT     | App      | TCP       | 1883/8883   | 0-2 | Sangat Rendah | Sensor data |
| CoAP     | App      | UDP       | 5683        | -   | Rendah   | Constrained dev |
| WebSocket| App      | TCP       | 80/443      | -   | Rendah   | Real-time web   |
| AMQP     | App      | TCP       | 5672        | -   | Tinggi   | Enterprise msg  |

### 1.5 Rumus Dasar Komunikasi IoT

**Throughput Aktual:**

$$T_{actual} = \frac{D_{payload}}{t_{transfer} + t_{overhead}}$$

Di mana:
- $D_{payload}$ = ukuran data payload (bytes)
- $t_{transfer}$ = waktu transfer data
- $t_{overhead}$ = waktu overhead protokol (header, handshake)

**Efisiensi Protokol:**

$$\eta = \frac{D_{payload}}{D_{total}} \times 100\%$$

Di mana $D_{total} = D_{payload} + D_{header} + D_{overhead}$

**End-to-End Latency:**

$$L_{e2e} = L_{propagation} + L_{transmission} + L_{processing} + L_{queuing}$$

$$L_{propagation} = \frac{d}{v}$$

Di mana:
- $d$ = jarak transmisi (meter)
- $v$ = kecepatan propagasi medium (≈ $2 \times 10^8$ m/s untuk kabel, $3 \times 10^8$ m/s untuk udara)

---

## BAB 2: Protokol HTTP & WebServer {#bab-2}

### 2.1 HTTP (HyperText Transfer Protocol)

HTTP adalah protokol application layer stateless yang mendefinisikan format dan urutan pesan antara web client dan server.

**Versi HTTP:**
- HTTP/1.0: Koneksi per-request (1996)
- HTTP/1.1: Persistent connection, pipelining (1997)
- HTTP/2: Multiplexing, header compression (2015)
- HTTP/3: QUIC transport (2022)

### 2.2 Format HTTP Request

```
METHOD /path HTTP/1.1\r\n
Host: 192.168.1.100\r\n
Content-Type: application/json\r\n
Content-Length: 42\r\n
Connection: keep-alive\r\n
\r\n
{"sensor":"DHT22","temperature":28.5}
```

**HTTP Methods (Verbs):**

| Method | Idempoten | Body | Fungsi                        |
|--------|-----------|------|-------------------------------|
| GET    | Ya        | -    | Ambil data dari server        |
| POST   | Tidak     | Ya   | Kirim/buat data di server     |
| PUT    | Ya        | Ya   | Perbarui keseluruhan resource |
| PATCH  | Tidak     | Ya   | Perbarui sebagian resource    |
| DELETE | Ya        | -    | Hapus resource di server      |

### 2.3 HTTP Status Codes

| Kode | Kategori     | Deskripsi              |
|------|--------------|------------------------|
| 200  | OK           | Request berhasil       |
| 201  | Created      | Resource dibuat        |
| 204  | No Content   | Berhasil, tanpa body   |
| 301  | Redirect     | Pindah permanen        |
| 400  | Bad Request  | Syntax request salah   |
| 401  | Unauthorized | Perlu autentikasi      |
| 403  | Forbidden    | Akses ditolak          |
| 404  | Not Found    | Resource tidak ada     |
| 500  | Server Error | Error internal server  |

### 2.4 REST API Design

REST (Representational State Transfer) adalah gaya arsitektur untuk API berbasis HTTP.

**Prinsip REST:**
1. Stateless — setiap request berdiri sendiri
2. Client-Server — pemisahan tanggung jawab
3. Cacheable — response dapat di-cache
4. Uniform Interface — konsisten endpoint
5. Layered System — middleware transparan

**Contoh Endpoint REST untuk IoT:**

```
GET  /api/sensors          → Ambil semua data sensor
GET  /api/sensors/{id}     → Ambil sensor tertentu
POST /api/sensors          → Tambah data sensor baru
GET  /api/actuators/{id}   → Cek status aktuator
POST /api/actuators/{id}/on  → Aktifkan aktuator
POST /api/actuators/{id}/off → Nonaktifkan aktuator
```

### 2.5 WebServer pada ESP32 — Implementasi

**Library yang Digunakan:**
- `ESPAsyncWebServer` — Asinkron (rekomendasi)
- `WebServer` — Bawaan Arduino ESP32 (sinkron)

**Kode Dasar AsyncWebServer:**

```cpp
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebServer server(80);

void setup() {
  WiFi.begin("SSID", "password");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  // Route GET /
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlPage);
  });
  
  // Route GET /api/sensor
  server.on("/api/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<200> doc;
    doc["temperature"] = readDHT();
    doc["humidity"] = readHumidity();
    doc["timestamp"] = millis();
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
  
  // Route POST /api/led
  server.on("/api/led", HTTP_POST, [](AsyncWebServerRequest *request){},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      StaticJsonDocument<100> doc;
      deserializeJson(doc, data, len);
      bool state = doc["state"];
      digitalWrite(LED_PIN, state ? HIGH : LOW);
      request->send(200, "application/json", "{\"status\":\"ok\"}");
  });
  
  server.begin();
}
```

### 2.6 Rumus Kinerja HTTP

**Request Rate Maksimum:**

$$R_{max} = \frac{1}{RTT + t_{server}}$$

Di mana:
- $RTT$ = Round Trip Time (detik)
- $t_{server}$ = waktu pemrosesan server

**HTTP Pipelining Throughput:**

$$T_{pipeline} = \frac{N \cdot D}{N \cdot t_{req} - (N-1) \cdot t_{overlap}}$$

Di mana $N$ adalah jumlah request yang di-pipeline.

---

## BAB 3: Protokol MQTT Dasar {#bab-3}

### 3.1 Sejarah & Latar Belakang MQTT

MQTT (Message Queuing Telemetry Transport) dikembangkan oleh Andy Stanford-Clark (IBM) dan Arlen Nipper pada tahun 1999 untuk monitoring pipeline minyak via satelit. Menjadi standar OASIS pada 2014 (MQTT 3.1.1) dan diperbarui menjadi MQTT 5.0 pada 2019.

**Filosofi MQTT:**
- **Ringan** — header minimal 2 byte
- **Publish-Subscribe** — decoupled sender & receiver
- **Store & Forward** — broker simpan & teruskan pesan
- **Reliable** — QoS 0, 1, 2

### 3.2 Arsitektur Publish-Subscribe MQTT

```
                    ┌─────────────────────┐
 [ESP32 Publisher] ─┤                     ├─ [Node-RED Subscriber]
                    │   MQTT BROKER       │
 [Sensor Node 2]  ─┤   (Mosquitto)       ├─ [Python Subscriber]
                    │                     │
 [Python Publisher]─┤  TOPIC: home/#      ├─ [ESP32 Subscriber]
                    └─────────────────────┘
```

**Perbedaan Pub-Sub vs Request-Response:**

| Aspek          | Request-Response (HTTP) | Publish-Subscribe (MQTT) |
|----------------|------------------------|--------------------------|
| Coupling       | Tight (langsung)       | Loose (via broker)       |
| Skalabilitas   | Rendah                 | Tinggi                   |
| Real-time      | Polling                | Push (event-driven)      |
| Fan-out        | Tidak efisien          | Efisien (1 to N)         |

### 3.3 Struktur Topik MQTT

Topik MQTT adalah hierarki string dipisahkan `/`:

```
home/floor1/livingroom/temperature
home/floor1/livingroom/humidity
factory/line1/machine3/vibration
factory/line1/machine3/temperature
```

**Wildcard Topik:**

| Wildcard | Level      | Contoh Subscription        | Cocok Dengan                         |
|----------|------------|----------------------------|--------------------------------------|
| `+`      | Single     | `home/+/temperature`       | `home/livingroom/temperature` ✓      |
|          |            |                            | `home/bedroom/temperature` ✓         |
|          |            |                            | `home/floor1/room/temperature` ✗     |
| `#`      | Multi      | `home/#`                   | `home/livingroom` ✓                  |
|          |            |                            | `home/floor1/bedroom/temp` ✓         |
|          |            |                            | `office/room` ✗                      |

**Aturan Topik:**
- Tidak boleh mulai/akhir dengan `/` (best practice)
- Case-sensitive: `home/Temp` ≠ `home/temp`
- Topik yang diawali `$` dicadangkan untuk sistem (contoh: `$SYS/broker/version`)

### 3.4 Format Paket MQTT

**Fixed Header (wajib, minimal 2 byte):**

```
Byte 1:  [7..4] = Packet Type | [3..0] = Flags
Byte 2+: Remaining Length (Variable-Length Encoding)
```

**Tipe Paket MQTT 3.1.1:**

| Nilai | Nama       | Arah             | Fungsi                    |
|-------|------------|------------------|---------------------------|
| 1     | CONNECT    | Client→Broker    | Inisiasi koneksi          |
| 2     | CONNACK    | Broker→Client    | Konfirmasi koneksi        |
| 3     | PUBLISH    | Dua arah         | Kirim pesan               |
| 4     | PUBACK     | Dua arah         | Ack QoS 1                 |
| 5     | PUBREC     | Dua arah         | Ack QoS 2 step 1          |
| 6     | PUBREL     | Dua arah         | Ack QoS 2 step 2          |
| 7     | PUBCOMP    | Dua arah         | Ack QoS 2 step 3          |
| 8     | SUBSCRIBE  | Client→Broker    | Subscribe topik           |
| 9     | SUBACK     | Broker→Client    | Konfirmasi subscribe      |
| 10    | UNSUBSCRIBE| Client→Broker    | Unsubscribe               |
| 11    | UNSUBACK   | Broker→Client    | Konfirmasi unsubscribe    |
| 12    | PINGREQ    | Client→Broker    | Keep-alive ping           |
| 13    | PINGRESP   | Broker→Client    | Respons ping              |
| 14    | DISCONNECT | Client→Broker    | Putus koneksi             |

### 3.5 Variable-Length Encoding (Remaining Length)

MQTT menggunakan skema encoding efisien untuk Remaining Length:

| Nilai          | Encoding     | Bytes |
|----------------|--------------|-------|
| 0 – 127        | 1 byte       | 1     |
| 128 – 16,383   | 2 bytes      | 2     |
| 16,384 – 2,097,151 | 3 bytes  | 3     |
| 2,097,152 – 268,435,455 | 4 bytes | 4 |

**Formula Encoding:**

Setiap byte menggunakan 7 bit untuk data dan 1 bit (MSB) sebagai continuation flag:

$$byte_k = (length \mod 128) + \begin{cases} 128 & \text{jika masih ada byte berikutnya} \\ 0 & \text{jika ini byte terakhir} \end{cases}$$

$$length = length \div 128$$

---

## BAB 4: MQTT Lanjutan — QoS, Retain, LWT, Security {#bab-4}

### 4.1 Quality of Service (QoS) MQTT

QoS mendefinisikan jaminan pengiriman pesan antara publisher dan subscriber (melalui broker).

**QoS 0 — At Most Once (Fire and Forget):**

```
Publisher ──[PUBLISH]──▶ Broker ──[PUBLISH]──▶ Subscriber
```

- Tidak ada acknowledgment
- Pesan **mungkin hilang** jika jaringan gangguan
- Overhead paling rendah
- Cocok untuk: data sensor frekuensi tinggi, status update

**QoS 1 — At Least Once:**

```
Publisher ──[PUBLISH, DUP=0]──▶ Broker ──[PUBACK]──▶ Publisher
           (disimpan di local store)
           ◀──[PUBACK]── (hapus dari store)

Broker ──[PUBLISH, DUP=0]──▶ Subscriber ──[PUBACK]──▶ Broker
```

- Pesan dijamin terkirim **minimal sekali**
- Mungkin **duplikat** jika PUBACK hilang
- Cocok untuk: perintah kontrol, data penting

**QoS 2 — Exactly Once (Four-Way Handshake):**

```
Publisher → [PUBLISH] → Broker
Publisher ← [PUBREC]  ← Broker
Publisher → [PUBREL]  → Broker
Publisher ← [PUBCOMP] ← Broker

Broker → [PUBLISH] → Subscriber
Broker ← [PUBREC]  ← Subscriber
Broker → [PUBREL]  → Subscriber
Broker ← [PUBCOMP] ← Subscriber
```

- Pesan dijamin terkirim **tepat sekali**
- Overhead tertinggi (4 round-trips)
- Cocok untuk: transaksi keuangan, perintah kritis

### 4.2 Rumus Latency per QoS Level

**Latency QoS 0:**
$$L_{QoS0} = L_{pub \to broker} = \frac{RTT_{pub-broker}}{2}$$

**Latency QoS 1:**
$$L_{QoS1} = L_{pub \to broker} + RTT_{pub-broker} + L_{broker \to sub} + RTT_{broker-sub}$$

$$L_{QoS1} = RTT_{pub-broker} + RTT_{broker-sub} + 2 \cdot L_{broker}$$

**Latency QoS 2:**
$$L_{QoS2} = 2 \cdot RTT_{pub-broker} + 2 \cdot RTT_{broker-sub} + 4 \cdot L_{broker}$$

**Perbandingan Latency (contoh RTT = 20 ms, L_broker = 1 ms):**

| QoS | Latency Publisher→Subscriber |
|-----|------------------------------|
| 0   | 10 ms                        |
| 1   | 42 ms                        |
| 2   | 84 ms                        |

### 4.3 Efisiensi Protokol MQTT

**Overhead Ratio:**

$$\eta_{MQTT} = \frac{D_{payload}}{D_{total}} \times 100\%$$

$$D_{total} = D_{fixed\_header} + D_{variable\_header} + D_{payload}$$

Untuk topik `home/temperature` (16 karakter) dengan payload 10 byte:
- Fixed header: 2 bytes
- Variable header: 2 (topik length) + 16 (topik) + 2 (packet ID) = 20 bytes
- Payload: 10 bytes
- Total: 32 bytes

$$\eta_{MQTT} = \frac{10}{32} \times 100\% = 31.25\%$$

Bandingkan dengan HTTP yang headernya bisa 200-800 bytes untuk payload sama:

$$\eta_{HTTP} = \frac{10}{210} \times 100\% = 4.76\%$$

### 4.4 Retained Messages

Ketika flag `retain = true`, broker menyimpan pesan terakhir pada topik tersebut dan langsung mengirimkannya ke subscriber baru yang subscribe topik itu.

**Kode Publish dengan Retain:**
```cpp
client.publish("home/temperature", "28.5", true);  // retain = true
```

**Use Case:**
- Status device terakhir (online/offline)
- Konfigurasi terakhir
- Nilai sensor terakhir

### 4.5 Last Will and Testament (LWT)

LWT adalah pesan yang dikirim broker secara otomatis ketika client terputus secara tidak normal (misalnya koneksi terputus tanpa DISCONNECT).

**Konfigurasi LWT dalam CONNECT:**
```cpp
// Di PubSubClient
client.setServer(mqttBroker, 1883);
// LWT dikonfigurasi saat connect:
client.connect(
  "ESP32_Client_01",      // Client ID
  "user",                  // Username
  "password",              // Password  
  "home/ESP32/status",     // LWT Topic
  1,                       // LWT QoS
  true,                    // LWT Retain
  "offline"                // LWT Message
);
// Kirim pesan "online" saat connect berhasil
client.publish("home/ESP32/status", "online", true);
```

### 4.6 Keep-Alive Mechanism

Untuk mempertahankan koneksi pada jaringan yang tidak aktif:

$$t_{keepalive} \leq 1.5 \times T_{keepalive\_interval}$$

Di mana $T_{keepalive\_interval}$ adalah interval PINGREQ (default 60 detik).

Jika broker tidak menerima paket dari client dalam $1.5 \times T_{keepalive\_interval}$, broker menganggap client sudah tidak aktif dan menjalankan LWT.

---

## BAB 5: Arsitektur & Spesifikasi ESP32 {#bab-5}

### 5.1 ESP32 — Gambaran Umum

ESP32 adalah SoC (System on Chip) dari Espressif Systems yang mengintegrasikan WiFi, Bluetooth, dan mikrokontroler dual-core dalam satu chip.

**Keunggulan ESP32:**
- WiFi + Bluetooth terintegrasi
- Dual-core 240 MHz
- Konsumsi daya rendah (mode deep sleep ~10 μA)
- Banyak peripheral (ADC, DAC, SPI, I2C, UART, PWM, dll)
- Ekosistem library luas (Arduino, ESP-IDF, MicroPython)

### 5.2 Spesifikasi Teknis ESP32

| Parameter          | Spesifikasi                        |
|--------------------|------------------------------------|
| CPU                | Xtensa LX6 Dual-Core, 32-bit       |
| Clock Frequency    | 80 / 160 / 240 MHz (adjustable)    |
| SRAM               | 520 KB                             |
| ROM                | 448 KB                             |
| Flash (external)   | 4 MB (up to 16 MB)                 |
| RTC Memory         | 16 KB (survives deep sleep)        |
| WiFi               | 802.11 b/g/n, 2.4 GHz             |
| Bluetooth          | BT Classic + BLE 4.2               |
| GPIO               | 34 programmable pins               |
| ADC                | 12-bit SAR, 18 channels            |
| DAC                | 8-bit, 2 channels (GPIO25, GPIO26) |
| UART               | 3 interfaces                       |
| SPI                | 4 interfaces (2 usable)            |
| I2C                | 2 interfaces                       |
| I2S                | 2 interfaces                       |
| PWM                | 16 channels (LEDC controller)      |
| Touch Sensor       | 10 capacitive inputs               |
| Hall Sensor        | Built-in                           |
| Temperature Sensor | Built-in (rough estimate)          |
| Tegangan Operasi   | 2.2 – 3.6 V                        |
| Arus Operasi       | Avg 80 mA (WiFi active)            |

### 5.3 Blok Diagram Internal ESP32

```
┌─────────────────────────────────────────────────────────┐
│                       ESP32 SoC                         │
│  ┌──────────┐  ┌──────────┐  ┌────────────────────────┐│
│  │  CPU 0   │  │  CPU 1   │  │     RF Module          ││
│  │(Protocol)│  │ (App)    │  │  WiFi 802.11 b/g/n     ││
│  └────┬─────┘  └────┬─────┘  │  Bluetooth 4.2 / BLE  ││
│       └──────┬───────┘        └────────────────────────┘│
│         ┌────▼─────┐                                    │
│         │  L1/L2   │                                    │
│         │  Cache   │                                    │
│         └────┬─────┘                                    │
│  ┌───────────▼──────────────────────────────────────┐  │
│  │                System Bus (AHB/APB)              │  │
│  └──┬──────┬──────┬──────┬────┬──────┬─────┬──────┘  │
│     │      │      │      │    │      │     │           │
│  ┌──▼──┐ ┌─▼──┐ ┌▼─┐ ┌──▼┐ ┌▼─┐ ┌──▼──┐ ┌▼────┐    │
│  │UART │ │SPI │ │I2C│ │ADC│ │DAC│ │GPIO │ │Flash│    │
│  └─────┘ └────┘ └───┘ └───┘ └───┘ └─────┘ └─────┘    │
└─────────────────────────────────────────────────────────┘
```

### 5.4 Memory Architecture ESP32

**Jenis Memori:**
1. **IRAM** (Instruction RAM): 192 KB — untuk kode yang butuh eksekusi cepat
2. **DRAM** (Data RAM): 328 KB — untuk data program
3. **RTC Memory**: 16 KB — bertahan saat deep sleep
4. **Flash (SPI)**: 4 MB+ — untuk kode & SPIFFS
5. **PSRAM** (opsional): 4-8 MB eksternal

**Memory Map (disederhanakan):**
```
0x3FF80000  RTC Slow Memory (8KB)
0x3FF90000  RTC Fast Memory (8KB)
0x3FFAE000  DRAM (Internal)
0x40000000  ROM 0 (Internal ROM)
0x40070000  IRAM (192KB)
0x400D0000  Flash Cache
0x50000000  RTC Peripherals
```

---

## BAB 6: WiFi pada ESP32 & Rumus Link Budget {#bab-6}

### 6.1 Mode WiFi ESP32

**Station Mode (STA):**
- ESP32 terhubung ke Access Point (router)
- Mendapat IP dari DHCP router
- Paling umum digunakan

**Access Point Mode (AP):**
- ESP32 bertindak sebagai Access Point
- Device lain connect ke ESP32 langsung
- Maksimal 4 koneksi simultaneous (default)
- Tidak ada akses internet

**Station + Access Point Mode (STA+AP):**
- ESP32 terhubung ke router DAN jadi AP
- Berguna untuk provisioning WiFi

```cpp
// Station Mode
WiFi.mode(WIFI_STA);
WiFi.begin("SSID", "password");

// AP Mode
WiFi.mode(WIFI_AP);
WiFi.softAP("ESP32_AP", "password123", 1, 0, 4);

// STA+AP Mode
WiFi.mode(WIFI_AP_STA);
WiFi.begin("Router_SSID", "router_pass");
WiFi.softAP("ESP32_Config", "config_pass");
```

### 6.2 Rumus Transmisi WiFi

**Free Space Path Loss (FSPL):**

$$L_{FSPL}(dB) = 20\log_{10}(d) + 20\log_{10}(f) + 20\log_{10}\left(\frac{4\pi}{c}\right)$$

$$L_{FSPL}(dB) = 20\log_{10}(d) + 20\log_{10}(f) - 147.55$$

Untuk WiFi 2.4 GHz ($f = 2.4 \times 10^9$ Hz):

$$L_{FSPL}(dB) = 20\log_{10}(d) + 100.0$$

Di mana $d$ dalam meter.

**Contoh:** Pada jarak 10 meter:
$$L_{FSPL} = 20\log_{10}(10) + 100 = 20 + 100 = 120 \text{ dB}$$

### 6.3 Link Budget WiFi ESP32

$$P_{rx}(dBm) = P_{tx}(dBm) + G_{tx}(dBi) + G_{rx}(dBi) - L_{FSPL}(dB) - L_{misc}(dB)$$

**Nilai Tipikal ESP32:**
- $P_{tx}$ = +20 dBm (100 mW, max)
- $G_{tx}$ = 2 dBi (antena PCB)
- $G_{rx}$ = 2 dBi (antena router)
- $L_{misc}$ = 3-5 dB (refleksi, dinding, dll)

**Contoh pada 15 meter indoor (L_misc = 10 dB):**
$$P_{rx} = 20 + 2 + 2 - (20\log_{10}(15) + 100) - 10$$
$$P_{rx} = 20 + 2 + 2 - 123.5 - 10 = -109.5 \text{ dBm}$$

*Sangat lemah — indoor perlu penambahan L_misc lebih realistis (~30 dB)*

### 6.4 RSSI (Received Signal Strength Indicator)

| RSSI Range (dBm) | Kualitas | Kecepatan Efektif |
|-------------------|----------|-------------------|
| > -50             | Excellent | Penuh             |
| -50 s/d -60       | Good      | Baik              |
| -60 s/d -70       | Fair      | Cukup             |
| -70 s/d -80       | Weak      | Lambat            |
| < -80             | Poor      | Sangat lambat/putus |

```cpp
// Baca RSSI
int rssi = WiFi.RSSI();
Serial.printf("RSSI: %d dBm\n", rssi);
```

### 6.5 Throughput WiFi 802.11n

**Throughput Teoritis:**
$$T_{teoritis} = MCS_{index} \times N_{spatial\_streams}$$

Untuk IEEE 802.11n 2.4 GHz, single stream:
| MCS | Modulation | Coding Rate | Throughput (20 MHz) |
|-----|-----------|-------------|---------------------|
| 0   | BPSK      | 1/2         | 6.5 Mbps            |
| 7   | 64-QAM    | 5/6         | 65 Mbps             |

**Throughput Efektif (dengan overhead protokol):**
$$T_{eff} \approx 0.6 \times T_{teoritis}$$

### 6.6 WiFi Manager — Konfigurasi Dinamis

```cpp
#include <WiFiManager.h>

WiFiManager wm;
bool res = wm.autoConnect("AutoConnectAP", "password");
if(!res) {
  Serial.println("Failed to connect");
  ESP.restart();
}
Serial.println("WiFi connected. IP: " + WiFi.localIP().toString());
```

---

## BAB 7: Jaringan TCP/IP & WebSocket {#bab-7}

### 7.1 Model TCP/IP

```
┌─────────────────────────────────────────────────────────────┐
│  APPLICATION   │  HTTP, MQTT, DNS, SMTP, FTP, WebSocket    │
├─────────────────────────────────────────────────────────────┤
│  TRANSPORT     │  TCP (reliable), UDP (unreliable)         │
├─────────────────────────────────────────────────────────────┤
│  INTERNET      │  IP (IPv4/IPv6), ICMP, ARP                │
├─────────────────────────────────────────────────────────────┤
│  NETWORK ACCESS│  WiFi 802.11, Ethernet, Bluetooth         │
└─────────────────────────────────────────────────────────────┘
```

### 7.2 TCP vs UDP

| Parameter       | TCP                     | UDP                    |
|----------------|--------------------------|------------------------|
| Koneksi        | Connection-oriented      | Connectionless         |
| Keandalan      | Guaranteed delivery      | Best-effort            |
| Ordering       | Ordered                  | No ordering guarantee  |
| Error checking | Checksum + ACK           | Checksum only          |
| Flow control   | Ya (sliding window)      | Tidak                  |
| Congestion ctrl| Ya (slow start, AIMD)    | Tidak                  |
| Header size    | 20-60 bytes              | 8 bytes                |
| Overhead       | Tinggi                   | Rendah                 |
| Latency        | Lebih tinggi             | Lebih rendah           |
| Use case       | HTTP, MQTT, FTP          | DNS, Video, VoIP       |

### 7.3 Rumus Throughput TCP

**Throughput Ideal:**
$$T_{TCP} = \frac{W_{window}}{RTT}$$

Di mana $W_{window}$ adalah ukuran window TCP (bytes).

**Throughput dengan Packet Loss (Model Mathis):**
$$T_{TCP} = \frac{C \cdot MSS}{RTT \cdot \sqrt{p}}$$

Di mana:
- $C \approx 1.22$ (konstanta TCP Reno)
- $MSS$ = Maximum Segment Size (bytes)
- $RTT$ = Round Trip Time (detik)
- $p$ = probabilitas packet loss

**Contoh:** MSS = 1460 bytes, RTT = 50 ms, p = 0.01%

$$T_{TCP} = \frac{1.22 \times 1460}{0.05 \times \sqrt{0.0001}} = \frac{1781}{0.005} = 356,200 \text{ bytes/s} \approx 2.85 \text{ Mbps}$$

### 7.4 TCP Three-Way Handshake

```
Client               Server
  |──────[SYN, seq=x]──────▶|
  |◀──[SYN-ACK, seq=y,ack=x+1]─|
  |──────[ACK, ack=y+1]─────▶|
  |                           |
  |  (koneksi terbentuk)      |
```

**Waktu Overhead Handshake TCP:**
$$t_{handshake} = 1.5 \times RTT$$

### 7.5 WebSocket Protocol

WebSocket menyediakan komunikasi **full-duplex** melalui satu koneksi TCP persisten, ideal untuk update data real-time.

**WebSocket Handshake (Upgrade dari HTTP):**

```http
→ Request:
GET /ws HTTP/1.1
Host: 192.168.1.100
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Sec-WebSocket-Version: 13

← Response:
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
```

**Sec-WebSocket-Accept Computation:**
$$Accept = Base64\left(SHA1\left(Key + \text{"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"}\right)\right)$$

**WebSocket Frame Format:**
```
 0               1               2               3
 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)    |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - -+-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
|    Masking-key (continued)    |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
```

**WebSocket pada ESP32:**
```cpp
#include <AsyncWebSocket.h>

AsyncWebSocket ws("/ws");

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if(type == WS_EVT_DATA) {
    String msg = String((char*)data, len);
    // Process incoming message
    ws.textAll("Server received: " + msg);
  }
}

// Kirim data sensor ke semua client
void broadcastSensorData() {
  StaticJsonDocument<200> doc;
  doc["temp"] = readTemp();
  doc["humidity"] = readHumidity();
  String json;
  serializeJson(doc, json);
  ws.textAll(json);
}
```

### 7.6 Server-Sent Events (SSE)

SSE adalah alternatif WebSocket untuk aliran data satu arah (server → client):

```cpp
// Server (ESP32)
server.on("/events", HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncEventSourceResponse *response = new AsyncEventSourceResponse("/events");
  request->send(response);
});

// Broadcast event
events.send(sensorJsonString, "update", millis());
```

```javascript
// Client (Browser JavaScript)
const evtSource = new EventSource("/events");
evtSource.addEventListener("update", (event) => {
  const data = JSON.parse(event.data);
  updateChart(data);
});
```

---

## BAB 8: JSON & Serialisasi Data {#bab-8}

### 8.1 Format JSON untuk IoT

JSON (JavaScript Object Notation) adalah format pertukaran data ringan berbasis teks, mudah dibaca manusia dan mudah diproses mesin.

**Tipe Data JSON:**
| Tipe     | Contoh              |
|----------|---------------------|
| String   | `"hello"`           |
| Number   | `42`, `3.14`        |
| Boolean  | `true`, `false`     |
| Null     | `null`              |
| Object   | `{"key": "value"}`  |
| Array    | `[1, 2, 3]`         |

**Contoh JSON Sensor IoT:**
```json
{
  "device_id": "ESP32_Lab01",
  "firmware": "v1.2.0",
  "timestamp": 1715000000,
  "uptime_ms": 86400000,
  "sensors": {
    "temperature": 28.5,
    "humidity": 65.2,
    "pressure": 1013.25
  },
  "actuators": {
    "led": true,
    "pump": false,
    "fan_speed": 75
  },
  "network": {
    "rssi": -58,
    "ip": "192.168.1.105"
  }
}
```

### 8.2 ArduinoJson Library

ArduinoJson adalah library C++ untuk parsing dan serialisasi JSON pada mikrokontroler.

**Menentukan Ukuran Buffer:**
```cpp
// Untuk dokumen di atas (estimasi)
// Gunakan ArduinoJson Assistant: https://arduinojson.org/v6/assistant/
const size_t CAPACITY = JSON_OBJECT_SIZE(10) + 256;
StaticJsonDocument<CAPACITY> doc;
```

**Rumus Estimasi Memori ArduinoJson:**
$$size_{doc} \approx 1.5 \times size_{JSON\_string} + N_{nodes} \times 16 + N_{strings} \times len_{strings}$$

**Serialisasi (C++ → JSON string):**
```cpp
#include <ArduinoJson.h>

StaticJsonDocument<256> doc;
doc["device_id"] = "ESP32_Lab01";
doc["temperature"] = 28.5;
doc["humidity"] = 65.2;

JsonObject sensors = doc.createNestedObject("sensors");
sensors["co2"] = 420;
sensors["lux"] = 1200;

JsonArray readings = doc.createNestedArray("history");
readings.add(28.1);
readings.add(28.3);
readings.add(28.5);

String output;
serializeJson(doc, output);
// Output: {"device_id":"ESP32_Lab01","temperature":28.5,...}

// Untuk MQTT (publish sebagai char array)
char buffer[256];
serializeJson(doc, buffer, sizeof(buffer));
mqttClient.publish("topic/data", buffer);
```

**Deserialisasi (JSON string → C++):**
```cpp
const char* jsonString = "{\"temp\":28.5,\"hum\":65.2,\"led\":true}";

StaticJsonDocument<256> doc;
DeserializationError error = deserializeJson(doc, jsonString);

if (error) {
  Serial.print("Deserialization failed: ");
  Serial.println(error.c_str());
  return;
}

float temp = doc["temp"];
float hum = doc["hum"];
bool led = doc["led"];
```

### 8.3 MessagePack — Alternatif JSON

MessagePack adalah format biner yang lebih kompak dari JSON, didukung ArduinoJson:

```cpp
// Serialize ke MessagePack (lebih kecil 20-50% dari JSON)
uint8_t buffer[256];
size_t n = serializeMsgPack(doc, buffer, sizeof(buffer));
mqttClient.publish("topic/data", buffer, n);
```

**Perbandingan Ukuran:**
| Format      | Ukuran untuk data sensor tipikal |
|-------------|----------------------------------|
| JSON        | 120 bytes                        |
| MessagePack | 85 bytes (-29%)                  |
| CBOR        | 82 bytes (-32%)                  |

---

## BAB 9: Keamanan IoT & Enkripsi {#bab-9}

### 9.1 OWASP IoT Top 10

Berdasarkan OWASP IoT Attack Surface Areas:

1. **Weak/Guessable/Hardcoded Passwords** — Jangan hardcode kredensial
2. **Insecure Network Services** — Nonaktifkan port yang tidak digunakan
3. **Insecure Ecosystem Interfaces** — Amankan API dan dashboard
4. **Lack of Secure Update Mechanism** — Implementasi OTA yang aman
5. **Use of Insecure/Outdated Components** — Update library secara berkala
6. **Insufficient Privacy Protection** — Enkripsi data sensitif
7. **Insecure Data Transfer and Storage** — Gunakan TLS/SSL
8. **Lack of Device Management** — Implementasi device lifecycle
9. **Insecure Default Settings** — Ubah default credentials
10. **Lack of Physical Hardening** — Proteksi akses fisik

### 9.2 MQTT dengan TLS/SSL

**Port MQTT:**
- 1883 — Plaintext (tidak aman, hanya untuk development)
- 8883 — TLS/SSL (produksi)

**Proses TLS Handshake:**
```
Client                    Server
  |──[ClientHello]──────▶|  (cipher suites, random)
  |◀──[ServerHello]───────|  (chosen cipher, server random)
  |◀──[Certificate]───────|  (server certificate)
  |◀──[ServerHelloDone]───|
  |──[ClientKeyExchange]▶|  (pre-master secret)
  |──[ChangeCipherSpec]──▶|
  |──[Finished]──────────▶|
  |◀──[ChangeCipherSpec]───|
  |◀──[Finished]───────────|
  |                        |
  |  (encrypted data)      |
```

**Implementasi MQTT TLS pada ESP32:**
```cpp
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// Sertifikat CA (dari broker penyedia, misal HiveMQ Cloud)
const char* ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
...
-----END CERTIFICATE-----
)EOF";

void setupMQTT() {
  espClient.setCACert(ca_cert);
  // Untuk mutual TLS (client certificate):
  // espClient.setCertificate(client_cert);
  // espClient.setPrivateKey(private_key);
  
  mqttClient.setServer("broker.hivemq.com", 8883);
  mqttClient.connect("ESP32_Client", "username", "password");
}
```

### 9.3 Enkripsi AES

**AES (Advanced Encryption Standard):**

$$C = AES\_Encrypt(K, P)$$
$$P = AES\_Decrypt(K, C)$$

Di mana:
- $C$ = ciphertext
- $K$ = kunci simetris (128, 192, atau 256 bit)
- $P$ = plaintext

**Contoh Enkripsi AES-128 pada ESP32:**
```cpp
#include <mbedtls/aes.h>

void encryptAES(const uint8_t *plaintext, uint8_t *ciphertext, 
                const uint8_t *key) {
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, key, 128);
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, plaintext, ciphertext);
  mbedtls_aes_free(&aes);
}
```

### 9.4 Hashing dengan SHA-256

$$H = SHA256(data)$$

```cpp
#include <mbedtls/sha256.h>

void computeSHA256(const uint8_t *data, size_t len, uint8_t *output) {
  mbedtls_sha256_context ctx;
  mbedtls_sha256_init(&ctx);
  mbedtls_sha256_starts(&ctx, 0);  // 0 = SHA-256
  mbedtls_sha256_update(&ctx, data, len);
  mbedtls_sha256_finish(&ctx, output);
  mbedtls_sha256_free(&ctx);
}
```

### 9.5 Best Practices Keamanan IoT

1. **Jangan hardcode credentials** — Gunakan SPIFFS/NVS untuk menyimpan
2. **Gunakan TLS** — Selalu enkripsi komunikasi di produksi
3. **Validasi input** — Cek panjang dan format data sebelum proses
4. **Rate limiting** — Batasi request per detik di server
5. **Firmware signing** — Tanda tangani firmware OTA
6. **Principle of least privilege** — Beri permission minimal
7. **Log & monitor** — Catat aktivitas abnormal

---

## BAB 10: Cloud IoT & Node-RED {#bab-10}

### 10.1 Platform IoT Cloud

| Platform       | Protokol      | Free Tier        | Keunggulan               |
|---------------|---------------|------------------|--------------------------|
| HiveMQ Cloud  | MQTT          | 100 devices      | MQTT dedicated           |
| Adafruit IO   | MQTT/HTTP     | 30 msg/min       | Dashboard mudah          |
| ThingSpeak    | HTTP/MQTT     | 8 field/channel  | Built-in MATLAB analytics|
| CloudMQTT     | MQTT          | 5 koneksi        | Simple setup             |
| AWS IoT Core  | MQTT/HTTP     | 250K msg/bulan   | Skalabel enterprise      |
| Google IoT Core| MQTT/HTTP    | Berbayar         | Integrasi BigQuery       |
| Azure IoT Hub | MQTT/HTTP     | 8000 msg/hari    | Integrasi Azure services |

### 10.2 Node-RED

Node-RED adalah platform low-code berbasis Node.js untuk pemrograman IoT secara visual (flow-based programming).

**Konsep Dasar Node-RED:**
- **Node** = blok fungsi
- **Wire** = koneksi data antar node
- **Flow** = kumpulan node yang terhubung
- **Message (msg)** = objek data yang mengalir

**Node Utama Node-RED:**
| Node             | Fungsi                          |
|------------------|---------------------------------|
| `mqtt in`        | Subscribe topik MQTT            |
| `mqtt out`       | Publish ke topik MQTT           |
| `json`           | Parse/stringify JSON            |
| `function`       | JavaScript processing           |
| `switch`         | Routing berdasarkan kondisi     |
| `debug`          | Tampilkan data di sidebar       |
| `inject`         | Trigger manual atau terjadwal   |
| `http in`        | Buat endpoint HTTP              |
| `http response`  | Kirim HTTP response             |
| `ui_gauge`       | Gauge dashboard                 |
| `ui_chart`       | Chart dashboard                 |

**Instalasi Node-RED:**
```bash
# Install Node.js LTS terlebih dahulu
npm install -g --unsafe-perm node-red
node-red

# Install dashboard UI
npm install node-red-dashboard
```

**Contoh Flow Node-RED:**
```json
[
  {
    "id": "mqtt-in-1",
    "type": "mqtt in",
    "topic": "home/+/temperature",
    "broker": "local-broker"
  },
  {
    "id": "json-1",
    "type": "json"
  },
  {
    "id": "gauge-1",
    "type": "ui_gauge",
    "label": "Temperature",
    "min": 0, "max": 50
  }
]
```

### 10.3 InfluxDB & Grafana

Untuk penyimpanan dan visualisasi data time-series IoT:

```
ESP32 → MQTT → Node-RED → InfluxDB → Grafana Dashboard
```

**InfluxDB Line Protocol:**
```
measurement,tag_key=tag_value field_key=field_value timestamp_ns
temperature,sensor=ESP32_01,location=lab value=28.5 1715000000000000000
```

**Contoh Node-RED Function Node untuk InfluxDB:**
```javascript
// Konversi MQTT JSON ke InfluxDB line protocol
const data = msg.payload;
msg.payload = [
  {
    measurement: "temperature",
    tags: { sensor: data.device_id, location: "lab" },
    fields: { value: data.temperature },
    timestamp: new Date()
  }
];
return msg;
```

---

## BAB 11: FreeRTOS & Real-Time Scheduling {#bab-11}

### 11.1 FreeRTOS pada ESP32

ESP32 Arduino framework berjalan di atas FreeRTOS secara transparan. FreeRTOS (Free Real-Time Operating System) menyediakan:
- Task management (multitasking preemptive)
- Queue untuk komunikasi antar task
- Semaphore/Mutex untuk sinkronisasi
- Timer software
- Memory management

### 11.2 Task Management

**Membuat Task:**
```cpp
TaskHandle_t sensorTaskHandle;
TaskHandle_t mqttTaskHandle;

void sensorTask(void *parameter) {
  for(;;) {  // infinite loop
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedTemp = temp;
    sharedHum = hum;
    xSemaphoreGive(dataMutex);
    vTaskDelay(pdMS_TO_TICKS(5000));  // delay 5 detik
  }
}

void mqttPublishTask(void *parameter) {
  for(;;) {
    if(mqttClient.connected()) {
      xSemaphoreTake(dataMutex, portMAX_DELAY);
      float t = sharedTemp;
      float h = sharedHum;
      xSemaphoreGive(dataMutex);
      
      // Publish ke MQTT
      publishJSON(t, h);
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup() {
  dataMutex = xSemaphoreCreateMutex();
  
  // Core 0 = WiFi/Protocol, Core 1 = App
  xTaskCreatePinnedToCore(sensorTask, "SensorTask", 4096, NULL, 1, 
                          &sensorTaskHandle, 1);
  xTaskCreatePinnedToCore(mqttPublishTask, "MQTTTask", 8192, NULL, 2,
                          &mqttTaskHandle, 1);
}
```

**Parameter xTaskCreate:**
```
xTaskCreate(
  pvTaskCode,     // Fungsi task
  pcName,         // Nama (untuk debug)
  usStackDepth,   // Stack size dalam words (1 word = 4 bytes di ESP32)
  pvParameters,   // Parameter yang dikirim ke task
  uxPriority,     // Prioritas (0=terendah, configMAX_PRIORITIES-1=tertinggi)
  pxCreatedTask   // Handle output
);
```

**Prioritas Task ESP32:**
- 0: Idle task
- 1: Background tasks
- 2-5: User tasks (tipikal)
- 24: WiFi/Protocol stack (core 0)

### 11.3 Rumus Schedulability

**Rate Monotonic Scheduling (RMS):**

Kondisi sufficient (cukup, bukan perlu) untuk schedulability:

$$U = \sum_{i=1}^{n} \frac{C_i}{T_i} \leq n\left(2^{1/n} - 1\right)$$

Di mana:
- $U$ = total CPU utilization
- $C_i$ = worst-case execution time task ke-$i$
- $T_i$ = periode task ke-$i$
- $n$ = jumlah task

**Batas utilitas untuk $n \to \infty$:**
$$U_{bound} = \lim_{n \to \infty} n\left(2^{1/n} - 1\right) = \ln(2) \approx 0.693$$

**Tabel Batas Utilitas RMS:**
| n tasks | U_bound |
|---------|---------|
| 1       | 1.000   |
| 2       | 0.828   |
| 3       | 0.779   |
| 4       | 0.756   |
| 5       | 0.743   |
| ∞       | 0.693   |

**Contoh Analisis:**

Task 1 (Sensor): $C_1 = 2$ ms, $T_1 = 100$ ms → $\frac{C_1}{T_1} = 0.02$  
Task 2 (MQTT): $C_2 = 5$ ms, $T_2 = 500$ ms → $\frac{C_2}{T_2} = 0.01$  
Task 3 (WebServer): $C_3 = 10$ ms, $T_3 = 200$ ms → $\frac{C_3}{T_3} = 0.05$

$$U = 0.02 + 0.01 + 0.05 = 0.08 \leq 0.779 \checkmark$$

Sistem dapat dijadwalkan!

### 11.4 Komunikasi Antar Task

**Queue (FIFO Message Buffer):**
```cpp
QueueHandle_t dataQueue = xQueueCreate(10, sizeof(SensorData));

// Producer task
SensorData data = {28.5, 65.2};
xQueueSend(dataQueue, &data, portMAX_DELAY);

// Consumer task
SensorData received;
xQueueReceive(dataQueue, &received, portMAX_DELAY);
```

**Semaphore (Binary):**
```cpp
SemaphoreHandle_t sem = xSemaphoreCreateBinary();
xSemaphoreGive(sem);    // Release
xSemaphoreTake(sem, portMAX_DELAY);  // Acquire (blocking)
```

**Mutex (Mutual Exclusion):**
```cpp
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
xSemaphoreTake(mutex, portMAX_DELAY);
// critical section
xSemaphoreGive(mutex);
```

---

## BAB 12: Sampling Rate & Power Management {#bab-12}

### 12.1 Teorema Sampling Nyquist-Shannon

Untuk merekonstruksi sinyal analog dari sampel digital tanpa aliasing:

$$f_s \geq 2 \times f_{max}$$

Di mana:
- $f_s$ = frekuensi sampling (Hz)
- $f_{max}$ = frekuensi komponen tertinggi dalam sinyal (Hz)

**Anti-Aliasing:**

Frekuensi Nyquist: $f_N = \frac{f_s}{2}$

Jika $f > f_N$, terjadi aliasing yang menghasilkan frekuensi palsu:
$$f_{alias} = |f - k \cdot f_s|, \quad k \in \mathbb{Z}$$

**Contoh Aplikasi:**
| Sinyal Sensor   | f_max    | f_s Min  | Interval Max |
|-----------------|----------|----------|--------------|
| Suhu udara      | 0.1 Hz   | 0.2 Hz   | 5 detik      |
| Kelembaban      | 0.05 Hz  | 0.1 Hz   | 10 detik     |
| Vibrasi mesin   | 1000 Hz  | 2000 Hz  | 0.5 ms       |
| Sinyal audio    | 20 kHz   | 44.1 kHz | 22.7 μs      |
| Detak jantung   | 5 Hz     | 10 Hz    | 100 ms       |

### 12.2 Resolusi ADC ESP32

**Resolusi Tegangan ADC:**
$$V_{resolution} = \frac{V_{ref}}{2^n - 1}$$

Di mana:
- $V_{ref}$ = tegangan referensi (3.3V untuk ESP32)
- $n$ = bit ADC (12-bit untuk ESP32)

$$V_{resolution} = \frac{3.3V}{4095} \approx 0.806 \text{ mV/LSB}$$

**Konversi ADC ke Tegangan:**
$$V = ADC_{value} \times \frac{V_{ref}}{2^n - 1}$$

**Koreksi Atenuasi ADC ESP32:**
```cpp
// Atenuasi untuk rentang input berbeda
analogSetAttenuation(ADC_11db);   // 0 - 3.3V (paling umum)
analogSetAttenuation(ADC_6db);    // 0 - 2.2V
analogSetAttenuation(ADC_2_5db);  // 0 - 1.5V
analogSetAttenuation(ADC_0db);    // 0 - 1.1V

int rawValue = analogRead(GPIO34);
float voltage = rawValue * (3.3 / 4095.0);  // Untuk ADC_11db
```

### 12.3 Power Management ESP32

| Mode          | CPU         | WiFi       | Arus Tipikal |
|--------------|-------------|------------|--------------|
| Active        | Running     | TX aktif   | 160-260 mA   |
| Modem Sleep   | Running     | Off        | 20-30 mA     |
| Light Sleep   | Paused      | Off        | 0.8 mA       |
| Deep Sleep    | Off         | Off        | 10-150 μA    |
| Hibernation   | Off         | Off        | 5 μA         |

**Rumus Estimasi Battery Life:**

$$t_{battery} = \frac{C_{battery}}{I_{avg}}$$

$$I_{avg} = I_{active} \times D_{active} + I_{sleep} \times (1 - D_{active})$$

Di mana:
- $C_{battery}$ = kapasitas baterai (mAh)
- $I_{active}$ = arus saat aktif (mA)
- $I_{sleep}$ = arus saat sleep (mA)
- $D_{active}$ = duty cycle (0 hingga 1)

**Contoh:**  
Baterai 18650 = 2600 mAh  
ESP32 aktif (WiFi+MQTT): $I_{active}$ = 200 mA, 10 detik per menit  
Deep sleep: $I_{sleep}$ = 0.1 mA, 50 detik per menit  
$D_{active}$ = 10/60 = 0.167

$$I_{avg} = 200 \times 0.167 + 0.1 \times 0.833 = 33.4 + 0.083 \approx 33.5 \text{ mA}$$

$$t_{battery} = \frac{2600}{33.5} \approx 77.6 \text{ jam} \approx 3.2 \text{ hari}$$

**Kode Deep Sleep:**
```cpp
#define uS_TO_S_FACTOR 1000000ULL
#define SLEEP_DURATION 60  // 60 detik

void setup() {
  // Lakukan pekerjaan (baca sensor, kirim MQTT)
  collectAndSendData();
  
  // Set wake-up source
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * uS_TO_S_FACTOR);
  
  // Masuk deep sleep
  esp_deep_sleep_start();
}
```

---

## BAB 13: Dashboard & Visualisasi Data {#bab-13}

### 13.1 Dashboard Real-Time dengan Chart.js

**HTML/JavaScript Dashboard ESP32:**

```html
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 IoT Dashboard</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body { font-family: Arial; background: #1a1a2e; color: #eee; }
    .card { background: #16213e; border-radius: 10px; padding: 20px; margin: 10px; }
    .value { font-size: 2em; color: #00d4ff; }
  </style>
</head>
<body>
  <div class="card">
    <h3>Temperature</h3>
    <div class="value" id="tempValue">--</div>
  </div>
  <canvas id="tempChart" width="400" height="200"></canvas>
  
  <script>
    const maxPoints = 20;
    const labels = [];
    const tempData = [];
    
    const ctx = document.getElementById('tempChart').getContext('2d');
    const chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [{
          label: 'Temperature (°C)',
          data: tempData,
          borderColor: '#00d4ff',
          backgroundColor: 'rgba(0,212,255,0.1)',
          fill: true, tension: 0.4
        }]
      },
      options: {
        animation: false,
        scales: { y: { min: 20, max: 40 } }
      }
    });
    
    // WebSocket atau SSE untuk update real-time
    const evtSource = new EventSource('/events');
    evtSource.addEventListener('sensor', (event) => {
      const data = JSON.parse(event.data);
      document.getElementById('tempValue').textContent = data.temp.toFixed(1) + ' °C';
      
      labels.push(new Date().toLocaleTimeString());
      tempData.push(data.temp);
      if(labels.length > maxPoints) { labels.shift(); tempData.shift(); }
      chart.update();
    });
  </script>
</body>
</html>
```

### 13.2 Bootstrap Responsive Dashboard

Untuk tampilan profesional di berbagai ukuran layar:

```html
<!-- Tambahkan di <head> -->
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css">

<!-- Card sensor dengan Bootstrap Grid -->
<div class="container-fluid">
  <div class="row g-3">
    <div class="col-md-3">
      <div class="card text-center bg-primary text-white">
        <div class="card-body">
          <h5>🌡️ Temperature</h5>
          <h2 id="temp">--°C</h2>
        </div>
      </div>
    </div>
    <div class="col-md-3">
      <div class="card text-center bg-info text-white">
        <div class="card-body">
          <h5>💧 Humidity</h5>
          <h2 id="hum">--%</h2>
        </div>
      </div>
    </div>
  </div>
</div>
```

### 13.3 SPIFFS — File System pada ESP32

Untuk menyimpan file HTML, CSS, JS di flash ESP32:

```cpp
#include <SPIFFS.h>

void setup() {
  if(!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  
  // Serve file dari SPIFFS
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.serveStatic("/js", SPIFFS, "/js/");
  server.serveStatic("/css", SPIFFS, "/css/");
}
```

**Upload file ke SPIFFS:**
- Arduino IDE: Install plugin "ESP32 Sketch Data Upload"
- PlatformIO: `pio run --target uploadfs`

---

## BAB 14: Integrasi Sistem WebServer + MQTT {#bab-14}

### 14.1 Arsitektur Integrasi Lengkap

```
                           INTERNET
                              │
            ┌─────────────────┼─────────────────┐
            │                 │                 │
      [HiveMQ Cloud]   [ThingSpeak]      [Node-RED]
            │                 │                 │
            └─────────────────┼─────────────────┘
                              │
                        [WiFi Router]
                              │
                          [ESP32]
                         ┌────┴────┐
                  [WebServer]    [MQTT Client]
                         │            │
                   [Browser]    [Sensor/LED]
```

### 14.2 Kode Integrasi Lengkap

```cpp
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// Konfigurasi
const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASS     = "YOUR_PASS";
const char* MQTT_BROKER   = "192.168.1.100";
const int   MQTT_PORT     = 1883;
const char* MQTT_TOPIC_PUB = "esp32/sensors";
const char* MQTT_TOPIC_SUB = "esp32/control";

// Objek
DHT dht(4, DHT22);
AsyncWebServer server(80);
AsyncEventSource events("/events");
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Shared data (protected by mutex)
SemaphoreHandle_t dataMutex;
float gTemp = 0, gHum = 0;
bool gLedState = false;

// MQTT Callback
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<100> doc;
  deserializeJson(doc, payload, length);
  
  if (String(topic) == MQTT_TOPIC_SUB) {
    gLedState = doc["led"];
    digitalWrite(2, gLedState ? HIGH : LOW);
    events.send("{\"led\":" + String(gLedState) + "}", "control", millis());
  }
}

// Task: Baca Sensor & Publish MQTT
void sensorMQTTTask(void *pvParameters) {
  for(;;) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    if(!isnan(t) && !isnan(h)) {
      xSemaphoreTake(dataMutex, portMAX_DELAY);
      gTemp = t; gHum = h;
      xSemaphoreGive(dataMutex);
      
      // MQTT Publish
      StaticJsonDocument<200> doc;
      doc["device"] = "ESP32_Lab";
      doc["temp"]   = t;
      doc["hum"]    = h;
      doc["led"]    = gLedState;
      doc["uptime"] = millis() / 1000;
      
      char buffer[200];
      serializeJson(doc, buffer);
      mqttClient.publish(MQTT_TOPIC_PUB, buffer);
      
      // SSE ke browser
      events.send(buffer, "sensor", millis());
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

// Task: MQTT Loop
void mqttLoopTask(void *pvParameters) {
  for(;;) {
    if(!mqttClient.connected()) {
      mqttClient.connect("ESP32_01");
      mqttClient.subscribe(MQTT_TOPIC_SUB);
    }
    mqttClient.loop();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(2, OUTPUT);
  
  dataMutex = xSemaphoreCreateMutex();
  
  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("IP: " + WiFi.localIP().toString());
  
  // WebServer Routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send(SPIFFS, "/index.html", "text/html");
  });
  
  server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest *req){
    StaticJsonDocument<100> doc;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    doc["temp"] = gTemp; doc["hum"] = gHum; doc["led"] = gLedState;
    xSemaphoreGive(dataMutex);
    String json; serializeJson(doc, json);
    req->send(200, "application/json", json);
  });
  
  events.onConnect([](AsyncEventSourceClient *client){
    client->send("connected", NULL, millis(), 1000);
  });
  server.addHandler(&events);
  server.begin();
  
  // MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  
  // Tasks (Core 1)
  xTaskCreatePinnedToCore(sensorMQTTTask, "SensorMQTT", 8192, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(mqttLoopTask, "MQTTLoop", 4096, NULL, 3, NULL, 1);
}

void loop() { /* intentionally empty */ }
```

### 14.3 Rumus dan Parameter Sistem Integrasi

**Memori Stack Task:**
$$Stack_{min} = Stack_{local\_vars} + Stack_{function\_calls} \times Stack_{per\_call} + Stack_{safety}$$

Rekomendasi:
- Task ringan (GPIO): 2048 bytes
- Task sensor: 4096 bytes
- Task MQTT + JSON: 8192 bytes
- Task WebServer: 8192 bytes

**Interval Publish Optimal:**
$$T_{publish} = \max\left(T_{nyquist}, \frac{1}{R_{MQTT_{max}}}\right)$$

Di mana $R_{MQTT_{max}}$ adalah rate maksimum broker (pesan/detik).

---

## BAB 15: Studi Kasus & Best Practices {#bab-15}

### 15.1 Desain Sistem IoT yang Baik

**Prinsip SOLID untuk Embedded:**
- **Single Responsibility**: Satu fungsi, satu tugas
- **Open/Closed**: Mudah extend, tidak perlu modify core
- **Dependency Inversion**: Depend on interface, bukan implementation

**Anti-Pattern yang Harus Dihindari:**
1. ❌ Blocking `delay()` di loop utama
2. ❌ Hardcode WiFi credentials
3. ❌ Tidak handle disconnect reconnect
4. ❌ `String` concatenation berlebihan (fragmentasi heap)
5. ❌ Stack allocation berlebihan (stack overflow)
6. ❌ Tidak cek return value fungsi
7. ❌ Tidak beri timeout pada operasi network

### 15.2 Error Handling Pattern

```cpp
// Pattern untuk operasi yang bisa gagal
bool connectWithRetry(const char* ssid, const char* pass, int maxRetry = 10) {
  WiFi.begin(ssid, pass);
  int count = 0;
  while(WiFi.status() != WL_CONNECTED && count < maxRetry) {
    delay(500);
    count++;
    Serial.print(".");
  }
  return WiFi.status() == WL_CONNECTED;
}

// Non-blocking reconnect MQTT
unsigned long lastReconnectAttempt = 0;
bool reconnectMQTT() {
  if(millis() - lastReconnectAttempt < 5000) return false;
  lastReconnectAttempt = millis();
  
  if(mqttClient.connect("ESP32", MQTT_USER, MQTT_PASS,
                         LWT_TOPIC, 1, true, "offline")) {
    mqttClient.publish(LWT_TOPIC, "online", true);
    mqttClient.subscribe(CTRL_TOPIC);
    return true;
  }
  return false;
}
```

### 15.3 OTA (Over-The-Air) Firmware Update

```cpp
#include <ArduinoOTA.h>

void setupOTA() {
  ArduinoOTA.setHostname("ESP32-Lab01");
  ArduinoOTA.setPassword("ota_password");
  
  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start: " + String(ArduinoOTA.getCommand() == U_FLASH ? "Sketch" : "SPIFFS"));
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
  });
  
  ArduinoOTA.begin();
}

// Di loop atau task:
// ArduinoOTA.handle();
```

---

## REFERENSI & REKOMENDASI {#referensi}

### Buku Terbaik

#### Untuk Pemula

1. **"Programming ESP32 with Arduino IDE"** — Neil Cameron (2023)  
   *Buku praktis step-by-step untuk ESP32, mencakup GPIO, WiFi, Bluetooth, WebServer, dan MQTT. Sangat direkomendasikan sebagai buku pertama.*

2. **"Getting Started with ESP32"** — Dogan Ibrahim (2022)  
   *Pengenalan sistematis untuk pemula, dari instalasi hingga proyek IoT sederhana.*

3. **"Internet of Things with ESP8266/ESP32"** — Marco Schwartz  
   *Konsep IoT dengan keluarga ESP, banyak proyek langsung praktek.*

#### Tingkat Menengah

4. **"MQTT Essentials — A Lightweight IoT Protocol"** — HiveMQ Team (Free E-book)  
   *Panduan paling komprehensif tentang MQTT, gratis di hivemq.com/mqtt-essentials. WAJIB baca.*

5. **"Programming the Internet of Things"** — Andy King (O'Reilly, 2021)  
   *Arsitektur IoT modern, protokol komunikasi, cloud integration.*

6. **"Building the Web of Things"** — Dominique Guinard & Vlad Trifa (Manning, 2016)  
   *Fondasi teoritis Web of Things, REST API untuk IoT, WebSocket.*

7. **"Node-RED: A Visual Tool for Wiring the Internet of Things"** — Node-RED Community  
   *Dokumentasi resmi Node-RED, tersedia gratis di nodered.org/docs.*

#### Tingkat Lanjut

8. **"Designing the Internet of Things"** — Adrian McEwen & Hakim Cassimally (Wiley, 2013)  
   *Desain sistem IoT end-to-end, dari hardware ke cloud.*

9. **"IoT Security Issues"** — Bharat Bhargava & Leszek Lilien (De Gruyter, 2018)  
   *Keamanan IoT komprehensif, OWASP IoT, enkripsi, autentikasi.*

10. **"Real-Time Embedded Systems"** — Jiacun Wang (Wiley-IEEE Press, 2017)  
    *FreeRTOS, scheduling theory, task analysis untuk embedded.*

11. **"FreeRTOS Reference Manual"** — Richard Barry (free PDF di freertos.org)  
    *Referensi API FreeRTOS lengkap.*

12. **"Computer Networks"** — Andrew S. Tanenbaum & David Wetherall (5th Ed)  
    *Fondasi jaringan komputer: TCP/IP, HTTP, protokol komunikasi.*

---

### Paper Ilmiah Terbaik (Terindeks IEEE, Scopus, Q1-Q2)

#### MQTT & IoT Protokol

1. **Hunkeler, U., Truong, H.L., & Stanford-Clark, A. (2008).** "MQTT-S — A Publish/Subscribe Protocol for Wireless Sensor Networks." *3rd International Conference on Communication System Software and Middleware (COMSWARE).* IEEE.  
   → *Paper asli MQTT-S, fondasi protokol MQTT modern.*

2. **Naik, N. (2017).** "Choice of Effective Messaging Protocols for IoT Systems: MQTT, CoAP, AMQP and HTTP." *IEEE Systems, Man, and Cybernetics (SMC).* DOI: 10.1109/SMC.2017.8122697  
   → *Perbandingan komprehensif protokol IoT dengan benchmark nyata.*

3. **Dizdarević, J., et al. (2019).** "A Survey on Communication Protocols for Internet of Things and Related Challenges of Fog and Cloud Computing Integration." *ACM Computing Surveys, 51(6).* DOI: 10.1145/3292674  
   → *Survey luas tentang protokol IoT dan integrasi fog/cloud.*

4. **Yokotani, T., & Sasaki, Y. (2016).** "Comparison with HTTP and MQTT on Required Network Resources for IoT." *2016 International Conference on Control, Electronics, Renewable Energy and Communications (ICCEREC).* IEEE.  
   → *Evaluasi kuantitatif performa MQTT vs HTTP.*

5. **Al-Fuqaha, A., et al. (2015).** "Internet of Things: A Survey on Enabling Technologies, Protocols, and Applications." *IEEE Communications Surveys & Tutorials, 17(4).* DOI: 10.1109/COMST.2015.2444095  
   → *Survey IoT paling banyak dikutip (>10.000 sitasi), wajib baca.*

#### WebServer & ESP32

6. **Gomez, C., et al. (2019).** "TCP in the Internet of Things: From Ostracism to Prominence." *IEEE Internet of Things Journal.* DOI: 10.1109/JIOT.2018.2852468  
   → *Peran TCP dalam IoT modern.*

7. **Minerva, R., Biru, A., & Rotondi, D. (2015).** "Towards a Definition of the Internet of Things." *IEEE Internet Initiative.*  
   → *Definisi formal dan taksonomi IoT dari IEEE.*

#### Security IoT

8. **Frustaci, M., et al. (2018).** "Evaluating Critical Security Issues of the IoT World: Present and Future Challenges." *IEEE Internet of Things Journal, 5(4).* DOI: 10.1109/JIOT.2017.2767291  
   → *Analisis keamanan IoT komprehensif.*

9. **Mishra, A.K., et al. (2020).** "Security Challenges in MQTT-based IoT Deployments and Countermeasures." *IEEE Access.* DOI: 10.1109/ACCESS.2020.2996813  
   → *Serangan dan mitigasi keamanan MQTT.*

#### Real-Time & FreeRTOS

10. **Liu, C.L., & Layland, J.W. (1973).** "Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment." *Journal of the ACM, 20(1).* DOI: 10.1145/321738.321743  
    → *Paper klasik yang memperkenalkan Rate Monotonic Scheduling (RMS). Fundamental.*

11. **Laplante, P.A. (2004).** *Real-Time Systems Design and Analysis.* IEEE Press / Wiley.  
    → *Buku teks standar untuk real-time systems.*

#### Dashboard & Visualisasi

12. **Perera, C., et al. (2014).** "Context Aware Computing for the Internet of Things: A Survey." *IEEE Communications Surveys & Tutorials, 16(1).* DOI: 10.1109/SURV.2013.042313.00197  
    → *Context-aware IoT, sensor fusion, dashboard intelligence.*

---

### Sumber Belajar Online Terpercaya

| Sumber | URL | Konten |
|--------|-----|--------|
| ESP32 Docs | docs.espressif.com | Dokumentasi resmi ESP-IDF |
| Arduino Ref | arduino.cc/reference | Referensi Arduino API |
| MQTT.org | mqtt.org | Spesifikasi MQTT resmi |
| HiveMQ Learn | hivemq.com/mqtt-essentials | Tutorial MQTT lengkap |
| Node-RED Docs | nodered.org/docs | Dokumentasi resmi Node-RED |
| RandomNerd | randomnerdtutorials.com | Tutorial ESP32 praktis |
| Last Mile | lastmilesolutions.com | IoT case studies |
| ArduinoJson | arduinojson.org | Dokumentasi ArduinoJson |

---

*Materi ini disusun sebagai panduan komprehensif Praktikum Mekatronika Modul 6. Pelajari secara bertahap dan lengkapi dengan praktik langsung di lab.*
