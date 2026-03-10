# Percobaan 2: ESP32 DataLogger dengan SQLite & Validasi

Data logging sensor ESP32 secara persisten menggunakan SQLite, validasi input dengan Zod, dan dashboard interaktif dibangun dengan Parcel bundler.

---

## Konsep

**Data Persistence dengan SQLite**: Setiap pembacaan sensor disimpan secara permanen ke file database SQLite. Data tidak hilang saat server restart.

**Input Validation dengan Zod**: Sebelum data disimpan ke database, payload dari ESP32 divalidasi menggunakan schema Zod. Data yang tidak valid ditolak dengan HTTP 422 dan pesan error yang deskriptif.

**Module Bundler (Parcel)**: Frontend HTML/CSS/JS diproses oleh Parcel untuk optimasi (minifikasi, code splitting), menghasilkan output statis di folder `dist/`.

---

## Arsitektur

```
┌──────────────────────────────────────────────────────────────────────┐
│                        JARINGAN LOKAL (WiFi)                         │
│                                                                      │
│  ┌────────────┐  POST /api/sensor  ┌──────────────────────────────┐  │
│  │            │ ─────────────────► │    Node.js Express  :3002    │  │
│  │   ESP32    │  JSON payload      │                              │  │
│  │ (Device    │ ◄───────────────── │  ┌──────────┐ ┌──────────┐  │  │
│  │  esp32-02) │  201 / 422         │  │   Zod    │ │  Routes  │  │  │
│  └────────────┘                    │  │Validation│ │  (API)   │  │  │
│                                    │  └────┬─────┘ └────┬─────┘  │  │
│  ┌────────────┐  GET /api/latest   │       │             │        │  │
│  │            │ ─────────────────► │  ┌────▼─────────────▼─────┐ │  │
│  │  Browser   │                    │  │   better-sqlite3        │ │  │
│  │ Dashboard  │ ◄───────────────── │  │   sensor.db (SQLite)    │ │  │
│  │ (Parcel)   │  JSON data         │  └─────────────────────────┘ │  │
│  │            │  GET /api/history  │                              │  │
│  │            │  GET /api/stats    │  Static: Frontend/dist/      │  │
│  └────────────┘                    └──────────────────────────────┘  │
│                                                                      │
└──────────────────────────────────────────────────────────────────────┘
```

---

## Struktur File

```
percobaan 2/
├── server.sh                       # Helper script menu
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js            # Express server entry point
│   │       ├── routes.js           # API route handlers
│   │       ├── database.js         # SQLite (better-sqlite3) setup
│   │       └── validation.js       # Zod schema & validator
│   └── Frontend/
│       ├── package.json            # Parcel bundler
│       └── src/
│           ├── index.html          # HTML entry point
│           ├── main.js             # Dashboard JS
│           └── styles.css          # Dark theme CSS
└── ESP32/
    ├── libraries.txt
    └── percobaan2/
        ├── percobaan2.ino          # Sketch utama
        └── config.h                # Konfigurasi WiFi & server
```

---

## Cara Menjalankan

### Prasyarat
- Node.js >= 20.x
- npm >= 10.x
- Arduino IDE dengan ESP32 board package dan ArduinoJson library
- `better-sqlite3` membutuhkan Python 3 dan `node-gyp` (biasanya sudah tersedia di Linux/Mac)

### Langkah 1 – Setup Backend

```bash
cd "Server/Backend"

# Salin konfigurasi
cp .env.example .env

# Install dependencies (termasuk better-sqlite3 yang perlu dikompilasi)
npm install

# Jalankan server
npm start
# atau: bash server.sh → pilih opsi 2, lalu 1
```

Server berjalan di `http://localhost:3002`. Database `sensor.db` otomatis dibuat.

### Langkah 2 – Build & Serve Frontend

```bash
cd "Server/Frontend"

# Install Parcel dan dependencies
npm install

# Build untuk produksi
npm run build
# Output: Server/Frontend/dist/

# (Opsional) Dev mode dengan hot reload di port 1234
npm run dev
```

Frontend yang sudah di-build akan otomatis disajikan oleh Express di `http://localhost:3002`.

### Langkah 3 – Flash ESP32

1. Buka `ESP32/percobaan2/percobaan2.ino` di Arduino IDE
2. Edit `config.h`:
   ```cpp
   #define WIFI_SSID     "NamaWiFiAnda"
   #define WIFI_PASSWORD "PasswordWiFiAnda"
   #define SERVER_IP     "192.168.x.x"   // IP laptop Anda
   #define DEVICE_ID     "esp32-02"
   ```
3. Cari IP laptop:
   - **Windows**: `ipconfig` → IPv4 Address
   - **Linux/Mac**: `ip addr` atau `hostname -I`
4. Upload sketch ke ESP32
5. Buka Serial Monitor (115200 baud)

---

## Endpoint API

| Method   | Endpoint           | Deskripsi                             | Request Body / Query                    | Response                                    |
|----------|--------------------|---------------------------------------|-----------------------------------------|---------------------------------------------|
| POST     | `/api/sensor`      | Simpan pembacaan sensor (+ validasi)  | `{ suhu, kelembaban, cahaya, device_id? }` | `{ id, message, data }` atau `{ error, detail, code }` |
| GET      | `/api/latest`      | Pembacaan sensor terbaru              | –                                       | `{ id, suhu, kelembaban, cahaya, device_id, created_at }` |
| GET      | `/api/history`     | Riwayat pembacaan                     | `?limit=50` (default 50, max 500)       | `{ count, data: [...] }`                    |
| GET      | `/api/stats`       | Statistik 24 jam terakhir             | –                                       | `{ avg_suhu, avg_kelembaban, max_suhu, min_suhu, count, ... }` |
| DELETE   | `/api/data`        | Hapus data lama                       | `?days=7` (default 7 hari)              | `{ message, deleted }`                      |

### Validasi Zod (POST /api/sensor)

| Field        | Tipe    | Constraint                     |
|--------------|---------|--------------------------------|
| `suhu`       | number  | min -10, max 60                |
| `kelembaban` | number  | min 0, max 100                 |
| `cahaya`     | integer | min 0, max 4095 (12-bit ADC)   |
| `device_id`  | string  | opsional, max 64 karakter      |

### Format Error Response

```json
{
  "error":  "Validation Error",
  "detail": "suhu: suhu maksimum adalah 60°C",
  "code":   "VALIDATION_FAILED"
}
```

### Contoh Request

```bash
# Kirim data sensor
curl -X POST http://localhost:3002/api/sensor \
  -H "Content-Type: application/json" \
  -d '{"suhu": 27.3, "kelembaban": 61.5, "cahaya": 2048, "device_id": "esp32-02"}'

# Ambil statistik
curl http://localhost:3002/api/stats

# Ambil riwayat 10 terakhir
curl "http://localhost:3002/api/history?limit=10"

# Hapus data > 3 hari
curl -X DELETE "http://localhost:3002/api/data?days=3"
```

---

## Troubleshooting

| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| `node-gyp` error saat `npm install` | Python atau build tools belum terinstall | Install Python 3 + `npm install -g node-gyp` atau `apt install build-essential python3` |
| `ENOENT: sensor.db` | DB_FILE path tidak valid | Periksa `.env`, atau biarkan default (dibuat di `Server/Backend/`) |
| HTTP 422 di Serial Monitor | Data di luar range validasi | Periksa nilai sensor; suhu harus -10 s/d 60°C, kelembaban 0-100%, cahaya 0-4095 |
| Dashboard kosong setelah `npm run build` | Parcel belum di-build atau path salah | Jalankan `npm run build` di `Server/Frontend`, pastikan `dist/` ada |
| Port 3002 sudah dipakai | Proses lain | Ubah `PORT` di `.env`, atau `lsof -i :3002` untuk cari prosesnya |
| ESP32 tidak POST data | WiFi/IP salah | Cek `config.h`, pastikan ESP32 dan laptop di subnet yang sama |
| `Cannot find module 'better-sqlite3'` | Dependencies belum diinstall | Jalankan `npm install` di `Server/Backend` |
