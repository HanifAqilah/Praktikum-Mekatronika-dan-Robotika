# Percobaan 3 – Real-time Sensor Dashboard (SSE + Vue 3 + FastAPI)

## Konsep

### Server-Sent Events (SSE) vs Polling

| Aspek | SSE | Polling |
|---|---|---|
| Arah data | Server → Client (satu arah) | Client meminta berulang |
| Koneksi | HTTP persisten, tetap terbuka | Buka-tutup tiap request |
| Latensi | Hampir real-time | Tergantung interval polling |
| Overhead | Rendah | Tinggi (banyak request HTTP) |
| Reconnect | Otomatis (browser built-in) | Harus diimplementasikan manual |
| Protokol | HTTP biasa | HTTP biasa |
| Browser support | Semua modern browser | Semua browser |

SSE ideal untuk **dashboard monitoring** di mana server secara aktif mendorong update (sensor readings, log) ke browser tanpa perlu browser meminta setiap kali.

---

## Arsitektur

```
┌─────────────┐    HTTP POST /api/sensor (setiap 3 detik)
│   ESP32-03  │─────────────────────────────────────────►┐
└─────────────┘                                          │
                                                         ▼
                                               ┌──────────────────┐
                                               │  FastAPI Backend  │
                                               │   (Port 8003)     │
                                               │                   │
                                               │  ┌─────────────┐  │
                                               │  │  SQLite DB  │  │
                                               │  │sensor_readings│ │
                                               │  └─────────────┘  │
                                               │  ┌─────────────┐  │
                                               │  │ In-memory   │  │
                                               │  │  latest{}   │  │
                                               │  └─────────────┘  │
                                               └──────┬───────────┘
                                                      │
                              SSE GET /api/stream     │
                                      ◄───────────────┘
                                      │  (setiap 2 detik)
                              ┌───────┴──────────┐
                              │  Vue 3 Frontend   │
                              │   (Port 9003)     │
                              │                   │
                              │  Dashboard View   │
                              │  ┌─────────────┐  │
                              │  │SensorCard x3│  │
                              │  │RealtimeChart│  │
                              │  │ StatsPanel  │  │
                              │  └─────────────┘  │
                              │  Analytics View   │
                              │  ┌─────────────┐  │
                              │  │  BarChart   │  │
                              │  │History Table│  │
                              │  └─────────────┘  │
                              └───────────────────┘
```

---

## Struktur Direktori

```
percobaan 3/
├── Server/
│   ├── Backend/
│   │   ├── main.py            ← FastAPI app utama
│   │   ├── requirements.txt
│   │   ├── .env.example
│   │   └── sensor.db          ← dibuat otomatis saat runtime
│   └── Frontend/
│       ├── index.html
│       ├── package.json
│       ├── vite.config.ts
│       ├── tsconfig.json
│       └── src/
│           ├── main.ts
│           ├── App.vue
│           ├── composables/
│           │   └── useSseData.ts
│           ├── components/
│           │   ├── SensorCard.vue
│           │   ├── RealtimeChart.vue
│           │   └── StatsPanel.vue
│           └── views/
│               ├── Dashboard.vue
│               └── Analytics.vue
├── ESP32/
│   └── percobaan3/
│       ├── percobaan3.ino
│       └── config.h
├── server.sh
└── README.md
```

---

## Cara Menjalankan

### Prasyarat
- Python 3.10+
- Node.js 18+
- Arduino IDE 2 dengan board ESP32

### Opsi 1 – Menggunakan server.sh (Rekomendasi)

```bash
cd "percobaan 3"
chmod +x server.sh
./server.sh
```

Pilih opsi:
- `3` → Install Backend
- `4` → Install Frontend
- `1` → Start Backend
- `2` → Start Frontend (terminal terpisah)

### Opsi 2 – Manual

**Backend:**
```bash
cd Server/Backend
pip install -r requirements.txt
cp .env.example .env
python main.py
```

**Frontend:**
```bash
cd Server/Frontend
npm install
npm run dev
```

Buka browser: [http://localhost:9003](http://localhost:9003)

### ESP32

1. Buka `ESP32/percobaan3/percobaan3.ino` di Arduino IDE
2. Edit `config.h`:
   - `WIFI_SSID` dan `WIFI_PASSWORD`
   - `SERVER_HOST` → IP komputer yang menjalankan backend
3. Install library: `ArduinoJson`, `WiFi`, `HTTPClient`
4. Upload ke ESP32

---

## Endpoint API

| Method | Path | Auth | Deskripsi |
|--------|------|------|-----------|
| `POST` | `/api/sensor` | Tidak | Kirim data sensor (dari ESP32) |
| `GET` | `/api/latest` | Tidak | Baca data terbaru per device |
| `GET` | `/api/history` | Tidak | 100 data terbaru dari DB |
| `GET` | `/api/stats` | Tidak | Statistik 24 jam terakhir |
| `GET` | `/api/stream` | Tidak | SSE stream (update tiap 2 detik) |

### Contoh POST /api/sensor

```json
{
  "device_id": "esp32-03",
  "suhu": 28.5,
  "kelembaban": 65.2,
  "cahaya": 2048
}
```

### Contoh Response GET /api/stats

```json
{
  "avg_suhu": 28.73,
  "avg_kelembaban": 62.14,
  "avg_cahaya": 1856.0,
  "max_suhu": 39.12,
  "min_suhu": 20.34,
  "count": 247
}
```

---

## Troubleshooting

| Masalah | Solusi |
|---------|--------|
| `CORS error` di browser | Pastikan `CORS_ORIGINS` di `.env` sesuai port frontend |
| SSE tidak menerima data | Cek apakah ESP32 sudah terkoneksi dan mengirim data |
| `ModuleNotFoundError` | Jalankan `pip install -r requirements.txt` |
| `npm: command not found` | Install Node.js dari [nodejs.org](https://nodejs.org) |
| ESP32 tidak terkoneksi | Periksa `WIFI_SSID`, `WIFI_PASSWORD`, dan `SERVER_HOST` di `config.h` |
| Port 8003/9003 sudah dipakai | Ubah `PORT` di `.env` dan `server.port` di `vite.config.ts` |
| `sensor.db` bermasalah | Hapus file `sensor.db` dan restart backend (akan dibuat ulang) |
