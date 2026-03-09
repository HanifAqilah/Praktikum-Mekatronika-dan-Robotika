# Percobaan 1: ESP32 WebServer Monitor

Monitoring sensor ESP32 secara real-time menggunakan HTTP Request-Response dengan Node.js Express backend dan tampilan dashboard berbasis browser.

---

## Konsep

**HTTP Request-Response** adalah pola komunikasi di mana:
- **Client** (ESP32 / Browser) mengirim **HTTP Request**
- **Server** (Node.js) memproses permintaan dan mengirim **HTTP Response**
- Tidak ada koneksi permanen – setiap permintaan berdiri sendiri

**Client-Server Architecture**: ESP32 bertindak sebagai HTTP client yang secara periodik mengirimkan data sensor, sementara browser mengambil data terbaru dari server setiap 2 detik.

---

## Arsitektur

```
┌─────────────────────────────────────────────────────────────┐
│                        JARINGAN LOKAL (WiFi)                │
│                                                             │
│   ┌──────────────┐    HTTP POST /api/sensor    ┌─────────┐  │
│   │              │ ──────────────────────────► │         │  │
│   │    ESP32     │                             │ Node.js │  │
│   │  (Sensor +   │ ◄────────────────────────── │ Express │  │
│   │   LED GPIO2) │    200 OK { message }       │  :3001  │  │
│   └──────────────┘                             │         │  │
│                         HTTP GET /api/data     │  In-    │  │
│   ┌──────────────┐ ◄────────────────────────── │ Memory  │  │
│   │              │                             │  Store  │  │
│   │   Browser    │ ──────────────────────────► │         │  │
│   │  Dashboard   │    200 OK { sensor data }   │         │  │
│   │  (Auto 2s)   │                             │         │  │
│   └──────────────┘    POST /api/led (toggle)   └─────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## Struktur File

```
percobaan 1/
├── server.sh                  # Helper script menu
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── server.js          # Express server
│   │   └── .env.example
│   └── Frontend/
│       └── index.html         # Dashboard single-file
└── ESP32/
    ├── libraries.txt
    └── percobaan1/
        ├── percobaan1.ino     # Sketch utama
        └── config.h           # Konfigurasi WiFi & server
```

---

## Cara Menjalankan

### Prasyarat
- Node.js >= 20.x
- Arduino IDE dengan ESP32 board package
- Library ArduinoJson (install via Library Manager)

### Langkah 1 – Setup Backend

```bash
# Masuk ke direktori backend
cd "Server/Backend"

# Salin file konfigurasi
cp .env.example .env

# Install dependencies
npm install

# Jalankan server
npm start
# atau gunakan script helper:
# bash server.sh  → pilih opsi 2 (install), lalu 1 (start)
```

Server akan berjalan di `http://localhost:3001`.

### Langkah 2 – Buka Dashboard

Buka browser dan navigasi ke:
```
http://localhost:3001
```

### Langkah 3 – Flash ESP32

1. Buka `ESP32/percobaan1/percobaan1.ino` di Arduino IDE
2. Edit `config.h`:
   ```cpp
   #define WIFI_SSID     "NamaWiFiAnda"
   #define WIFI_PASSWORD "PasswordWiFiAnda"
   #define SERVER_IP     "192.168.x.x"  // IP laptop Anda
   ```
3. Cek IP laptop:
   - **Windows**: `ipconfig` → lihat IPv4 Address
   - **Linux/Mac**: `ip addr` atau `ifconfig`
4. Pilih Board: **ESP32 Dev Module**, pilih port yang benar
5. Upload sketch
6. Buka Serial Monitor (115200 baud) untuk debug

---

## Endpoint API

| Method | Endpoint        | Deskripsi                          | Request Body                              | Response                          |
|--------|-----------------|------------------------------------|-------------------------------------------|-----------------------------------|
| GET    | `/`             | Serve dashboard HTML               | –                                         | HTML page                         |
| POST   | `/api/sensor`   | Update data sensor dari ESP32      | `{ suhu, kelembaban, cahaya }`            | `{ message, data }`               |
| GET    | `/api/data`     | Ambil data sensor terkini          | –                                         | `{ suhu, kelembaban, cahaya, ledState, lastUpdate }` |
| POST   | `/api/led`      | Toggle LED state                   | –                                         | `{ ledState }`                    |
| GET    | `/api/status`   | Info server (uptime, timestamp)    | –                                         | `{ uptime, timestamp, status }`   |

### Contoh Request

```bash
# Kirim data sensor
curl -X POST http://localhost:3001/api/sensor \
  -H "Content-Type: application/json" \
  -d '{"suhu": 28.5, "kelembaban": 65.2, "cahaya": 1024}'

# Ambil data terkini
curl http://localhost:3001/api/data

# Toggle LED
curl -X POST http://localhost:3001/api/led
```

---

## Troubleshooting

| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| ESP32 tidak bisa konek WiFi | SSID/Password salah | Periksa `config.h`, pastikan ESP32 dan laptop di jaringan yang sama |
| HTTP POST gagal (connection refused) | Server belum jalan atau IP salah | Jalankan `npm start` di backend, cek `SERVER_IP` di `config.h` |
| Dashboard tidak update | Browser tidak bisa akses API | Buka DevTools (F12) → Console, cek error CORS atau fetch |
| Port 3001 sudah dipakai | Proses lain menggunakan port | Ubah `PORT=3002` di `.env`, atau matikan proses dengan `lsof -i :3001` |
| LED tidak berubah di ESP32 | Interval sync terlalu lama | `syncLED()` berjalan setiap 1 detik; tunggu sebentar setelah klik toggle |
| `Cannot find module 'express'` | Dependencies belum diinstall | Jalankan `npm install` di `Server/Backend` |
