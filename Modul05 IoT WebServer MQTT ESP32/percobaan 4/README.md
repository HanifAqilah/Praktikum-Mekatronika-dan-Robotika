# Percobaan 4 – Authenticated IoT Dashboard (JWT + WebSocket + Angular)

## Konsep

### JSON Web Token (JWT)

JWT adalah standar terbuka (RFC 7519) untuk mengirimkan informasi secara aman sebagai objek JSON yang di-*sign*. Tiga bagian JWT:

```
header.payload.signature
```

| Bagian | Isi |
|--------|-----|
| Header | Algoritma (HS256) dan tipe token |
| Payload | Claims: `sub` (username), `exp` (expiry) |
| Signature | HMAC dari header + payload menggunakan secret key |

### WebSocket

WebSocket menyediakan **komunikasi dua arah** (full-duplex) melalui satu koneksi TCP persisten:

| Aspek | WebSocket | SSE (Percobaan 3) |
|-------|-----------|-------------------|
| Arah | Dua arah (bidirectional) | Satu arah (server → client) |
| Protokol | WS/WSS | HTTP |
| Overhead | Sangat rendah (frame kecil) | Header HTTP per koneksi |
| Browser | Semua modern | Semua modern |
| Use case | Chat, game, kontrol IoT | Dashboard, notifikasi |

### Angular 17

Framework frontend berbasis TypeScript dengan fitur:
- **Dependency Injection** – service di-inject ke komponen
- **RxJS Observables** – async data streams
- **Angular Material** – komponen UI siap pakai
- **Guards** – proteksi route dengan `canActivate`
- **Interceptors** – middleware HTTP untuk inject token JWT

---

## Arsitektur

```
┌─────────────┐
│   ESP32-04  │──── HTTP POST /api/sensors/data (tanpa auth) ────►┐
└─────────────┘                                                    │
                                                                   ▼
                                                     ┌─────────────────────┐
                                                     │   FastAPI Backend    │
                                                     │     (Port 8004)      │
                                                     │                      │
                                                     │  ┌────────────────┐  │
                                                     │  │   SQLite DB    │  │
                                                     │  │  users         │  │
                                                     │  │  sensor_readings│ │
                                                     │  └────────────────┘  │
                                                     │                      │
                                                     │  POST /api/auth/login│
                                                     │  → return JWT token  │
                                                     │                      │
                                                     │  WS /api/ws/{token}  │
                                                     │  → broadcast sensor  │
                                                     └──────────┬───────────┘
                                                                │
                                         WebSocket (ws://)      │
                                         ◄──────────────────────┘
                                         │
                               ┌─────────┴────────────┐
                               │  Angular 17 Frontend  │
                               │     (Port 9004)       │
                               │                       │
                               │  /login               │
                               │  ┌─────────────────┐  │
                               │  │ LoginComponent  │  │
                               │  │  → POST login   │  │
                               │  │  → store JWT    │  │
                               │  └─────────────────┘  │
                               │                       │
                               │  / (guarded)          │
                               │  ┌─────────────────┐  │
                               │  │DashboardComponent│ │
                               │  │  WS connection  │  │
                               │  │  Sensor cards   │  │
                               │  │  Suhu chart     │  │
                               │  └─────────────────┘  │
                               │                       │
                               │  /data (guarded)      │
                               │  ┌─────────────────┐  │
                               │  │  DataComponent  │  │
                               │  │  History table  │  │
                               │  │  Stats panel    │  │
                               │  └─────────────────┘  │
                               └───────────────────────┘
```

---

## Struktur Direktori

```
percobaan 4/
├── Server/
│   ├── Backend/
│   │   ├── main.py
│   │   ├── requirements.txt
│   │   ├── .env.example
│   │   └── app/
│   │       └── routers/
│   │           ├── auth.py
│   │           └── sensors.py
│   └── Frontend/
│       ├── angular.json
│       ├── package.json
│       ├── proxy.conf.json
│       ├── tsconfig.json
│       └── src/
│           ├── index.html
│           ├── main.ts
│           ├── styles.scss
│           └── app/
│               ├── app.module.ts
│               ├── app-routing.module.ts
│               ├── app.component.ts/html
│               ├── core/
│               │   ├── auth/
│               │   │   ├── auth.service.ts
│               │   │   └── auth.guard.ts
│               │   └── interceptors/
│               │       └── jwt.interceptor.ts
│               └── features/
│                   ├── login/
│                   ├── dashboard/
│                   └── data/
├── ESP32/
│   └── percobaan4/
│       ├── percobaan4.ino
│       └── config.h
├── server.sh
└── README.md
```

---

## Cara Menjalankan

### Prasyarat
- Python 3.10+
- Node.js 18+ dengan Angular CLI: `npm install -g @angular/cli`
- Arduino IDE 2 dengan board ESP32

### Opsi 1 – Menggunakan server.sh (Rekomendasi)

```bash
cd "percobaan 4"
chmod +x server.sh
./server.sh
```

Pilih opsi `6` untuk Full Setup (install semua + start kedua server).

### Opsi 2 – Manual

**Backend:**
```bash
cd Server/Backend
pip install -r requirements.txt
cp .env.example .env
# Edit .env: ubah JWT_SECRET menjadi string acak yang aman!
python main.py
```

**Frontend:**
```bash
cd Server/Frontend
npm install
npm start
```

Buka browser: [http://localhost:9004](http://localhost:9004)

### Login

| Field | Nilai |
|-------|-------|
| Username | `admin` |
| Password | `admin123` |

### ESP32

1. Buka `ESP32/percobaan4/percobaan4.ino` di Arduino IDE
2. Edit `config.h`:
   - `WIFI_SSID` dan `WIFI_PASSWORD`
   - `SERVER_HOST` → IP komputer yang menjalankan backend
3. Install library: `ArduinoJson`, `WiFi`, `HTTPClient`
4. Upload ke ESP32

---

## Endpoint API

| Method | Path | Auth | Deskripsi |
|--------|------|------|-----------|
| `POST` | `/api/auth/login` | Tidak | Login → return JWT token |
| `POST` | `/api/sensors/data` | Tidak | Kirim data sensor (untuk ESP32) |
| `GET`  | `/api/sensors/latest` | JWT Bearer | Baca data terbaru per device |
| `GET`  | `/api/sensors/history` | JWT Bearer | Riwayat data (default limit=50) |
| `GET`  | `/api/sensors/stats` | JWT Bearer | Statistik 24 jam terakhir |
| `WS`   | `/api/ws/{token}` | JWT (URL param) | WebSocket real-time stream |

### Contoh Login

```bash
curl -X POST http://localhost:8004/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"username":"admin","password":"admin123"}'
```

Response:
```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "bearer"
}
```

### Contoh Authenticated Request

```bash
curl http://localhost:8004/api/sensors/history \
  -H "Authorization: Bearer <token>"
```

---

## Keamanan

| Praktik | Implementasi |
|---------|--------------|
| Password hashing | `passlib[bcrypt]` – tidak disimpan plaintext |
| JWT signing | `python-jose` dengan HS256 |
| Token expiry | Default 60 menit (dapat diubah di `.env`) |
| Route guard | Angular `AuthGuard` – redirect ke /login |
| HTTP Interceptor | JWT otomatis di-inject ke setiap request |
| WS auth | Token divalidasi saat koneksi WS dibuka |

**⚠️ Penting:** Ubah `JWT_SECRET` di `.env` dengan nilai yang kuat (min. 32 karakter acak) sebelum deployment.

---

## Troubleshooting

| Masalah | Solusi |
|---------|--------|
| `401 Unauthorized` di API | Pastikan token valid dan belum kadaluarsa |
| WebSocket tidak terhubung | Pastikan token dikirim di URL (`/api/ws/{token}`) |
| `CORS error` | Periksa `allow_origins` di `main.py` |
| Angular build error | Jalankan `npm install` lagi |
| `ModuleNotFoundError` | Jalankan `pip install -r requirements.txt` |
| ESP32 tidak kirim data | Periksa `SERVER_HOST` dan `SERVER_PORT` di `config.h` |
| Port 8004/9004 sudah dipakai | Ubah `PORT` di `.env` dan `"port": 9004` di `angular.json` |
