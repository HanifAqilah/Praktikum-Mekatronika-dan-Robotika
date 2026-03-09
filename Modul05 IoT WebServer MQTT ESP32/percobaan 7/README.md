# Percobaan 7 – IoT Dashboard: WebServer + MQTT + ESP32

Sistem monitoring IoT lengkap dengan backend FastAPI, frontend Vue 3, firmware ESP32, dan EdgePython sebagai rules engine.

## Arsitektur

```
ESP32 ──MQTT──► EdgePython (rules engine + aggregator)
                     │
                     ├──alert──► MQTT broker ◄──► Backend FastAPI
                     └──agg───►                        │
                                                   SQLite DB
                                                        │
                                              Vue 3 Dashboard ◄── SSE
```

## Komponen

| Komponen     | Teknologi                      | Port  |
|--------------|-------------------------------|-------|
| Backend      | Python 3.10 + FastAPI + aiomqtt | 8007 |
| Frontend     | Vue 3 + Vite + TypeScript      | 9007  |
| EdgePython   | Python 3.10 + aiomqtt + structlog | –   |
| Database     | SQLite (aiosqlite)             | –     |
| MQTT Broker  | Mosquitto (install separately) | 1883  |

## Prasyarat

- Python 3.10+
- Node.js 18+
- MQTT Broker (Mosquitto): `sudo apt install mosquitto mosquitto-clients`
- Arduino IDE / PlatformIO dengan board ESP32

## Instalasi & Menjalankan

```bash
# Clone / buka direktori
cd "Modul05 IoT WebServer MQTT ESP32/percobaan 7"

# Instal semua dependensi + buat file .env
bash server.sh install

# Edit konfigurasi MQTT
nano Server/Backend/.env
nano EdgePython/.env

# Terminal 1 – Backend
bash server.sh backend

# Terminal 2 – Frontend
bash server.sh frontend

# Terminal 3 – EdgePython
bash server.sh edge
```

Buka browser: **http://localhost:9007**

## Konfigurasi ESP32

1. Buka `ESP32/percobaan7/config.h`
2. Isi `WIFI_SSID`, `WIFI_PASSWORD`, `MQTT_SERVER`
3. Flash ke ESP32 via Arduino IDE (pilih board: ESP32 Dev Module)
4. Install library dari `ESP32/libraries.txt`

## Pin ESP32

| Sensor          | GPIO | Keterangan           |
|-----------------|------|----------------------|
| DHT22 (data)    | 4    | Suhu & kelembaban    |
| LDR (ADC)       | 34   | Intensitas cahaya    |
| Soil moisture   | 35   | Kelembaban tanah     |
| Status LED      | 2    | Built-in blue LED    |

## MQTT Topics

| Topic         | Arah          | Format                                                        |
|---------------|---------------|---------------------------------------------------------------|
| `sensor/data` | ESP32 → Broker | `{"suhu":28.5,"kelembaban":65.2,"cahaya":1234,"tanah":2100,"device":"esp32-01","ts":1234}` |
| `sensor/alert`| Edge → Broker  | `{"field":"suhu","message":"Suhu kritis!","actual":36,...}`   |
| `sensor/agg`  | Edge → Broker  | `{"device":"esp32-01","averages":{"suhu":27.3,...},...}`      |

## REST API

```
GET  /api/health                    # Health check
GET  /api/sensors/latest            # Data terbaru
GET  /api/sensors/aggregate/minutes # Agregasi per menit (1 jam)
GET  /api/sensors/aggregate/hours   # Agregasi per jam (24 jam)
GET  /api/sensors/stats/today       # Statistik hari ini
GET  /api/stream                    # SSE stream (setiap 2 detik)
GET  /api/alerts                    # Daftar alert
POST /api/alerts                    # Buat alert manual
PATCH /api/alerts/{id}/resolve      # Tandai resolved
DELETE /api/alerts/{id}             # Hapus alert
```

Dokumentasi API interaktif: **http://localhost:8007/docs**

## Konfigurasi Threshold Alert (.env)

```env
SUHU_MAX=35.0
SUHU_MIN=10.0
KELEMBABAN_MAX=90.0
KELEMBABAN_MIN=20.0
CAHAYA_MAX=4000
TANAH_MAX=3000
TANAH_MIN=500
```

## EdgePython Rules Engine

Edit `EdgePython/rules.json` (dibuat otomatis saat pertama jalan):

```json
[
  {"field": "suhu", "op": ">", "value": 35, "action": "alert", "message": "Suhu kritis!"},
  {"field": "tanah", "op": "<", "value": 500, "action": "alert", "message": "Tanah terlalu kering!"}
]
```

Operator yang didukung: `>`, `<`, `>=`, `<=`, `==`, `!=`

## Troubleshooting

| Masalah | Solusi |
|---------|--------|
| ESP32 tidak terkoneksi MQTT | Periksa `MQTT_SERVER` di `config.h`, pastikan broker berjalan |
| Backend tidak menerima data | Cek `MQTT_HOST` di `Server/Backend/.env` |
| Chart tidak update | Pastikan SSE `/api/stream` aktif (lihat Network tab browser) |
| DHT22 read failed | Periksa koneksi + resistor pull-up 10kΩ |
