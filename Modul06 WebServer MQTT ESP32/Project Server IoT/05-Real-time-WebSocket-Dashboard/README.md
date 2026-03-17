# Percobaan 5: Real-time WebSocket Dashboard

> **Modul IoT WebServer MQTT ESP32 — Mekatronika dan Robotika**

## Konsep

Percobaan 5 memperkenalkan komunikasi **native WebSocket** (full-duplex) antara browser dan server Node.js. Data sensor dari ESP32 dikirim melalui HTTP POST ke backend, lalu langsung di-*broadcast* ke semua klien browser melalui WebSocket tanpa polling. State manajemen React menggunakan **Zustand**, sedangkan visualisasi data menggunakan **Recharts**.

---

## Arsitektur

```
┌─────────────┐        HTTP POST /api/sensor        ┌──────────────────────────┐
│             │ ─────────────────────────────────► │                          │
│   ESP32     │        GET  /api/led (poll)          │   Node.js + Express      │
│  esp32-05   │ ◄───────────────────────────────── │   Port 8005              │
│             │                                     │   better-sqlite3 (WAL)   │
└─────────────┘                                     │                          │
                                                    │   WebSocket Server       │
                                                    │   ws://localhost:8005/ws │
                                                    └───────────┬──────────────┘
                                                                │  WS broadcast
                                                                │  (full-duplex)
                                                    ┌───────────▼──────────────┐
                                                    │   React 18 + Vite        │
                                                    │   Port 9005              │
                                                    │                          │
                                                    │  ┌─────────┐ ┌────────┐  │
                                                    │  │Recharts │ │Zustand │  │
                                                    │  └─────────┘ └────────┘  │
                                                    └──────────────────────────┘
```

---

## Struktur Direktori

```
percobaan 5/
├── server.sh                    # Script manajemen (start, install, build)
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js         # Express + HTTP server
│   │       ├── database.js      # better-sqlite3 helper
│   │       └── websocket.js     # ws WebSocket server
│   └── Frontend/
│       ├── package.json
│       ├── vite.config.ts
│       ├── tsconfig.json
│       ├── tsconfig.node.json
│       └── src/
│           ├── main.tsx
│           ├── App.tsx
│           ├── store/
│           │   └── sensorStore.ts
│           ├── hooks/
│           │   └── useWebSocket.ts
│           └── components/
│               ├── SensorChart.tsx
│               ├── SensorCards.tsx
│               ├── LedControl.tsx
│               ├── HistoryTable.tsx
│               ├── StatsCard.tsx
│               └── StatusBar.tsx
└── ESP32/
    ├── libraries.txt
    └── percobaan5/
        ├── config.h
        └── percobaan5.ino
```

---

## Cara Menjalankan

### Prasyarat

- Node.js >= 20
- npm >= 9
- Arduino IDE 2.x + ESP32 board package

### 1. Konfigurasi environment backend

```bash
cd "Server/Backend"
cp .env.example .env
# Edit .env jika perlu mengubah port atau path database
```

### 2. Install dependensi

```bash
# Gunakan script interaktif:
bash server.sh 3   # install backend
bash server.sh 4   # install frontend

# Atau manual:
cd Server/Backend  && npm install
cd Server/Frontend && npm install
```

### 3. Jalankan Backend

```bash
bash server.sh 1
# Backend tersedia di: http://localhost:8005
# WebSocket di:        ws://localhost:8005/ws
```

### 4. Jalankan Frontend (tab baru)

```bash
bash server.sh 2
# Buka browser: http://localhost:9005
```

### 5. Atau jalankan keduanya sekaligus

```bash
bash server.sh 6
```

### 6. Konfigurasi ESP32

Edit `ESP32/percobaan5/config.h`:

```c
#define WIFI_SSID      "NamaWiFiAnda"
#define WIFI_PASSWORD  "PasswordWiFi"
#define SERVER_IP      "192.168.x.x"   // IP komputer (cek dengan `ip addr` / `ipconfig`)
```

Upload sketch ke ESP32 via Arduino IDE.

---

## Endpoint API

| Method | Path            | Body / Query          | Deskripsi                                      | Response                          |
|--------|-----------------|-----------------------|------------------------------------------------|-----------------------------------|
| `POST` | `/api/sensor`   | `{device_id, suhu, kelembaban, cahaya}` | Simpan data sensor, broadcast WS | `{id, message}` HTTP 201 |
| `GET`  | `/api/history`  | `?limit=100`          | Ambil riwayat (max 500)                        | `SensorReading[]`                 |
| `GET`  | `/api/latest`   | —                     | Baca pembacaan terakhir                        | `SensorReading`                   |
| `GET`  | `/api/stats`    | —                     | Statistik agregat (avg, max, min, count)       | `Stats`                           |
| `POST` | `/api/led`      | `{state?: boolean}`   | Toggle/set LED, broadcast WS                   | `{state: boolean}`                |
| `GET`  | `/api/led`      | —                     | Baca status LED saat ini                       | `{state: boolean}`                |
| `GET`  | `/api/status`   | —                     | Health check: uptime, WS clients, DB status    | `{uptime, clients, db_ok}`        |

---

## WebSocket Events

Semua pesan adalah JSON kecuali ping/pong.

| Arah            | Type        | Payload                                                    | Keterangan                               |
|-----------------|-------------|-------------------------------------------------------------|------------------------------------------|
| Server → Client | `sensor`    | `{type:"sensor", data: SensorReading}`                     | Dikirim saat ada POST /api/sensor baru   |
| Server → Client | `led`       | `{type:"led", state: boolean}`                             | Dikirim saat status LED berubah          |
| Client → Server | `ping`      | string `"ping"`                                            | Keep-alive dari client                   |
| Server → Client | `pong`      | string `"pong"`                                            | Response keep-alive                      |
| Server → Client | —           | Data sensor terakhir (sensor event) saat koneksi baru masuk| Inisialisasi state awal                  |

---

## Tipe Data

```typescript
interface SensorReading {
  id: number;
  device_id: string;
  suhu: number;        // °C
  kelembaban: number;  // %
  cahaya: number;      // ADC 0–4095
  created_at: string;  // ISO datetime
}

interface Stats {
  count: number;
  avg_suhu: number;
  avg_kelembaban: number;
  avg_cahaya: number;
  max_suhu: number;
  min_suhu: number;
}
```

---

## Troubleshooting

| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| `Cannot find module 'better-sqlite3'` | Dependensi belum diinstall | Jalankan `npm install` di `Server/Backend` |
| Frontend tidak bisa connect ke WS | Vite proxy belum aktif / backend mati | Pastikan backend berjalan, restart `npm run dev` |
| ESP32 HTTP POST gagal (HTTP -1) | IP server salah atau firewall | Cek `SERVER_IP` di `config.h`, pastikan port 8005 terbuka |
| `CORS error` di browser | Origin tidak terdaftar | Tambahkan origin ke `CORS_ORIGINS` di `.env` |
| Database terkunci (`SQLITE_BUSY`) | WAL mode | Sudah dihandle dengan `PRAGMA journal_mode=WAL` |
| WebSocket disconnect berulang | Jaringan tidak stabil | Hook auto-reconnect dengan exponential backoff (1s→30s) aktif |
| Grafik tidak muncul | Belum ada data di store | Tunggu pembacaan pertama dari ESP32 atau kirim data manual |

### Tes Manual API (tanpa ESP32)

```bash
# Kirim data sensor
curl -s -X POST http://localhost:8005/api/sensor \
  -H "Content-Type: application/json" \
  -d '{"device_id":"test","suhu":28.5,"kelembaban":65.2,"cahaya":1024}' | python3 -m json.tool

# Cek status server
curl -s http://localhost:8005/api/status | python3 -m json.tool

# Toggle LED
curl -s -X POST http://localhost:8005/api/led | python3 -m json.tool
```

---

*Praktikum Mekatronika dan Robotika — Percobaan 5*
