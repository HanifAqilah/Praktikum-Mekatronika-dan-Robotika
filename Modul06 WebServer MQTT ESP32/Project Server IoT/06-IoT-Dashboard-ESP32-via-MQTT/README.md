# Percobaan 6 – IoT Dashboard ESP32 via MQTT

Implementasi lengkap sistem IoT berbasis ESP32 yang mengirim data sensor melalui MQTT, divisualisasikan di dashboard web real-time, dimonitor oleh Edge Python subscriber, dan dikendalikan melalui REST API.

---

## Arsitektur Sistem

```
┌─────────────┐   MQTT publish    ┌───────────────────┐   HTTP/REST   ┌──────────────┐
│   ESP32     │ ────────────────► │  Mosquitto Broker  │ ◄──────────► │   Browser    │
│  (sensor +  │ ◄──────────────  │  (port 1883)       │              │  Dashboard   │
│   LED GPIO2)│   MQTT subscribe  └────────┬──────────┘              └──────────────┘
└─────────────┘                            │ MQTT subscribe
                                           ▼
                                  ┌─────────────────────────┐
                                  │  Node.js Backend        │
                                  │  Express + MQTT.js      │
                                  │  + SQLite  (port 3006)  │
                                  └─────────────────────────┘
                                           │ MQTT subscribe
                                           ▼
                                  ┌──────────────────────────┐
                                  │  EdgePython              │
                                  │  paho-mqtt subscriber    │
                                  │  + colorama alerts       │
                                  │  + rotating log file     │
                                  └──────────────────────────┘
```

---

## Struktur File

```
percobaan 6/
├── server.sh                    # Menu manajemen (start/install/build)
├── README.md
│
├── Server/
│   ├── Backend/
│   │   ├── server.js            # Express + MQTT.js + SQLite (port 3006)
│   │   ├── package.json
│   │   └── .env.example
│   └── Frontend/
│       └── index.html           # Dashboard vanilla HTML/JS
│
├── ESP32/
│   ├── libraries.txt            # Daftar library Arduino yang dibutuhkan
│   └── percobaan6/
│       ├── percobaan6.ino       # Sketch utama ESP32
│       └── config.h             # Konfigurasi WiFi & MQTT
│
└── EdgePython/
    ├── main.py                  # MQTT subscriber + alert + rotating log
    ├── requirements.txt
    └── .env.example
```

---

## Prasyarat

| Komponen | Versi |
|---|---|
| Node.js | ≥ 20.x |
| Python | ≥ 3.11 |
| Mosquitto MQTT Broker | ≥ 2.x |
| Arduino IDE / arduino-cli | ≥ 2.x |
| ESP32 Board Support | esp32:esp32 |

---

## Cepat Mulai

### 1. Clone / buka folder percobaan 6

```bash
cd "Modul05 IoT WebServer MQTT ESP32/percobaan 6"
chmod +x server.sh
```

### 2. Install semua dependensi

```bash
bash server.sh 3   # pilihan: Install Dependencies
# atau lengkap (termasuk Mosquitto + arduino-cli):
bash server.sh 4   # pilihan: Build All
```

### 3. Konfigurasi Backend

```bash
cp Server/Backend/.env.example Server/Backend/.env
# Edit .env: isi MQTT_BROKER_URL, PORT, dll.
nano Server/Backend/.env
```

### 4. Konfigurasi ESP32

Edit `ESP32/percobaan6/config.h`:
```cpp
#define WIFI_SSID        "nama_wifi_anda"
#define WIFI_PASSWORD    "password_wifi"
#define MQTT_BROKER_HOST "192.168.x.x"   // IP komputer yang menjalankan Mosquitto
```

Flash sketch ke ESP32 via Arduino IDE atau:
```bash
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ESP32/percobaan6/
```

### 5. Jalankan Backend

```bash
bash server.sh 1
# Server berjalan di http://localhost:3006
```

### 6. Jalankan EdgePython (terminal lain)

```bash
cp EdgePython/.env.example EdgePython/.env
bash server.sh 2
```

---

## API Reference

### `GET /api/broker/status`
Status koneksi backend ke MQTT broker.
```json
{
  "connected": true,
  "broker": "mqtt://localhost:1883",
  "clientId": "percobaan6-server-a3f2b1",
  "ts": "2025-01-15T08:30:00.000Z"
}
```

### `GET /api/latest`
Nilai terbaru setiap topic dari in-memory cache.
```json
{
  "esp32/sensor/suhu":       { "payload": "{\"suhu\":28.5,\"unit\":\"C\"}",    "ts": "2025-01-15T08:30:01.123Z" },
  "esp32/sensor/kelembaban": { "payload": "{\"kelembaban\":62.1,\"unit\":\"%RH\"}", "ts": "2025-01-15T08:30:01.124Z" },
  "esp32/sensor/cahaya":     { "payload": "{\"cahaya\":512,\"unit\":\"lux\"}",  "ts": "2025-01-15T08:30:01.125Z" },
  "esp32/status":            { "payload": "{\"state\":\"online\",\"ip\":\"192.168.1.50\",\"rssi\":-62,\"uptime\":120,\"led\":\"OFF\"}", "ts": "..." }
}
```

### `GET /api/history?limit=50`
50 pesan terakhir dari SQLite (urutan terbaru dulu).
```json
[
  { "id": 42, "topic": "esp32/sensor/suhu", "payload": "{\"suhu\":29.0,\"unit\":\"C\"}", "received_at": "2025-01-15T08:30:01.123Z" }
]
```

### `POST /api/command`
Kirim perintah LED ke ESP32.
```json
// Request body:
{ "command": "ON" }   // atau "OFF"

// Response (200 OK):
{ "success": true, "topic": "esp32/command/led", "payload": "ON" }
```

---

## MQTT Topics

| Topic | Arah | Format Payload | Deskripsi |
|---|---|---|---|
| `esp32/sensor/suhu` | ESP32 → Broker | `{"suhu": 28.5, "unit": "C"}` | Suhu setiap 3 detik |
| `esp32/sensor/kelembaban` | ESP32 → Broker | `{"kelembaban": 62.1, "unit": "%RH"}` | Kelembaban setiap 3 detik |
| `esp32/sensor/cahaya` | ESP32 → Broker | `{"cahaya": 512, "unit": "lux"}` | Cahaya (ADC GPIO34) |
| `esp32/status` | ESP32 → Broker | `{"state":"online","ip":"...","rssi":-62,"uptime":120,"led":"OFF"}` | Status + LWT |
| `esp32/command/led` | Broker → ESP32 | `ON` / `OFF` | Kendalikan LED GPIO2 |

LWT (Last Will Testament): topic `esp32/status`, payload `"offline"`, QoS 1, retain true.

---

## Dashboard Web

Buka browser: **http://localhost:3006**

Fitur:
- 🟢 **Status badge** MQTT Connected/Disconnected (polling setiap 2 detik)
- 🌡️ **Kartu sensor** suhu, kelembaban, cahaya – update otomatis 2 detik
- 💡 **Tombol ON/OFF** untuk LED via POST `/api/command`
- 📋 **Tabel riwayat** 20 pesan MQTT terakhir dari SQLite
- ● **Indikator LIVE** berkedip saat data baru tiba

---

## EdgePython – Alert System

- Subscribe ke `esp32/#` (semua topic ESP32)
- Setiap pesan dicetak: `[TIMESTAMP]  TOPIC: PAYLOAD`
- Jika suhu > 35 °C → teks merah terang di terminal
- Log disimpan ke `edge_log.txt` dengan rotasi otomatis (max 1 MB, 3 backup)
- Konfigurasi via `EdgePython/.env`

Contoh output:
```
[2025-01-15 08:31:05.123]  esp32/sensor/suhu: {"suhu":36.2,"unit":"C"}
⚠  PERINGATAN SUHU TINGGI!  Suhu = 36.2 °C  (ambang: 35.0 °C)
```

---

## Troubleshooting

| Masalah | Solusi |
|---|---|
| `MQTT broker not connected` | Pastikan Mosquitto berjalan: `mosquitto -v` |
| ESP32 tidak publish | Cek IP broker di `config.h`, pastikan ESP32 di jaringan yang sama |
| Dashboard tidak update | Buka DevTools → Console, cek error CORS/network |
| `better-sqlite3` gagal build | Jalankan `npm install --build-from-source` atau install `build-essential` |
| Python `ModuleNotFoundError` | Aktifkan venv: `source EdgePython/.venv/bin/activate` |

---

## Lisensi

MIT – Bebas digunakan untuk keperluan praktikum dan pembelajaran.
