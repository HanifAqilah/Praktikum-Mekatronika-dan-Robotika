# PROMPT OPUS 4.6 — MODUL 05: IoT WebServer MQTT ESP32
## 10 Percobaan Lengkap: Web Development → IoT Integration

---

## KONTEKS MODUL

Modul ini adalah praktikum berbasis **full-stack web development** yang diperluas ke **IoT edge computing** menggunakan ESP32. Mahasiswa membangun dari fondasi HTTP REST API hingga sistem IoT produksi dengan MQTT, database, dan edge computing.

**Program Studi:** Teknologi Rekayasa Otomasi — Praktikum Mekatronika dan Robotika  
**Hardware:** ESP32 DevKit + DHT22 + LDR + LED + Push Button + Potensiometer  
**OS Server:** Ubuntu 22.04 LTS  
**Target Audiens:** Mahasiswa teknik semester 3–4 yang sudah kenal pemrograman dasar

---

## INSTRUKSI GLOBAL UNTUK IMPLEMENTASI

Kamu adalah expert full-stack IoT engineer. Implementasikan **10 percobaan lengkap** sesuai spesifikasi berikut. Setiap percobaan harus bisa berjalan mandiri.

### Aturan Implementasi Wajib

1. **Setiap percobaan memiliki `server.sh`** yang bisa dijalankan langsung di terminal Ubuntu
2. **Percobaan 1–5:** Struktur `percobaan N/server.sh`, `percobaan N/Server/Backend/`, `percobaan N/Server/Frontend/`
3. **Percobaan 6–10:** Tambahkan `percobaan N/ESP32/` dan `percobaan N/EdgePython/`
4. **Semua dependency** diinstall otomatis lewat `server.sh` opsi build
5. **Kode adalah produksi-ready**, bukan demo: error handling, graceful shutdown, environment variables
6. **README.md** ada di tiap percobaan — jelaskan konsep, cara jalankan, endpoint API

---

## SPESIFIKASI server.sh

### Untuk Percobaan 1–5

```bash
#!/bin/bash
# server.sh — percobaan N
# Jalankan: bash server.sh  (lalu tekan Enter untuk masuk menu)

clear
echo "============================================"
echo "  PERCOBAAN N — [JUDUL PERCOBAAN]"
echo "  Modul 05: IoT WebServer MQTT ESP32"
echo "============================================"
echo ""
read -p "Tekan ENTER untuk masuk menu..." dummy

while true; do
  clear
  echo "============================================"
  echo "  MENU PERCOBAAN N"
  echo "============================================"
  echo "  1. Start Server"
  echo "  2. Stop Server"
  echo "  3. Status Server"
  echo "  4. Build All & Install Dependencies"
  echo "  0. Keluar"
  echo "============================================"
  read -p "Pilih menu [0-4]: " choice
  case $choice in
    1) # start backend dan frontend processes, simpan PID ke .pid files ;;
    2) # kill proses dari .pid files ;;
    3) # cek apakah proses jalan, tampilkan URL akses ;;
    4) # install semua dependency (npm install, pip install, dsb), build frontend ;;
    0) echo "Keluar."; exit 0 ;;
    *) echo "Pilihan tidak valid"; sleep 1 ;;
  esac
done
```

### Untuk Percobaan 6–10 (tambah opsi 5)

```bash
  echo "  5. Run Edge Python (foreground)"
  # case 4 juga build ESP32 dengan arduino-cli
  # case 5 jalankan EdgePython/main.py di foreground
```

**Implementasi server.sh harus:**
- Menyimpan PID proses ke `.pid` file di folder percobaan
- `status` menampilkan URL akses (localhost:PORT), status running/stopped
- `build` menginstall semua tools yang diperlukan: nodejs, npm, python3, pip, arduino-cli, mosquitto
- Setiap opsi punya feedback yang jelas ke user (warna terminal dengan `tput` atau ANSI codes)
- Timeout graceful kill (SIGTERM dulu, lalu SIGKILL setelah 5 detik)

---

## PERCOBAAN 1: Hello World REST API

**Tema:** Fondasi HTTP, REST API, dan Web Server modern  
**Estimasi waktu:** 45 menit  
**Backend:** Node.js 20 + Express 5  
**Frontend:** Vanilla HTML5 + CSS3 + Vanilla JS (tanpa bundler, tanpa framework)

### Struktur File

```
percobaan 1/
├── server.sh
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── package-lock.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js           ← entry point Express app
│   │       ├── routes/
│   │       │   ├── hello.js       ← GET /api/hello, GET /api/info
│   │       │   └── sensor.js      ← GET /api/sensor (data simulasi)
│   │       └── middleware/
│   │           └── cors.js        ← CORS config
│   └── Frontend/
│       ├── index.html             ← dashboard utama
│       ├── css/
│       │   └── style.css
│       └── js/
│           └── app.js             ← fetch API calls, DOM manipulation
```

### Backend Requirements

- Express 5 dengan ES modules (`"type": "module"`)
- Port dari environment variable `PORT` (default 3001)
- Endpoints:
  - `GET /api/hello` → `{ message: "Hello from ESP32 IoT Server!", timestamp: ISO8601, uptime: detik }`
  - `GET /api/info` → info server (node version, platform, memory usage)
  - `GET /api/sensor` → data sensor **simulasi** (random suhu 20-35°C, kelembaban 40-80%, cahaya 0-4095)
  - `GET /health` → `{ status: "ok" }`
- CORS enabled untuk semua origin (development mode)
- Graceful shutdown dengan SIGTERM/SIGINT handler
- Morgan logging middleware

### Frontend Requirements

- Satu halaman dashboard dengan:
  - Header dengan judul dan status koneksi (hijau/merah)
  - Kartu sensor 3 buah (suhu, kelembaban, cahaya) dengan nilai realtime
  - Kartu info server (uptime, memory, node version)
  - Tombol "Refresh" manual dan auto-refresh setiap 3 detik
  - Tampilkan raw JSON response di `<pre>` tag
- **Tidak boleh** menggunakan CDN/library eksternal — pure vanilla JS
- Responsive dengan CSS Grid/Flexbox
- CSS custom properties (variables) untuk theming

### Konsep yang Didemonstrasikan

- HTTP request-response cycle
- REST API design: URL structure, HTTP methods, status codes
- Asynchronous JS: `fetch()`, `async/await`, `try/catch`
- DOM manipulation tanpa framework
- CORS dan mengapa diperlukan
- Environment variables dengan dotenv

---

## PERCOBAAN 2: CRUD Database — Data Logging Sensor

**Tema:** Persistensi data dengan SQL database, operasi CRUD penuh  
**Estimasi waktu:** 60 menit  
**Backend:** Node.js 20 + Express 5 + SQLite3 (better-sqlite3)  
**Frontend:** HTML5 + Vanilla JS dengan npm build (Parcel bundler)

### Struktur File

```
percobaan 2/
├── server.sh
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js
│   │       ├── database/
│   │       │   ├── db.js          ← better-sqlite3 setup, migrasi
│   │       │   └── schema.sql     ← DDL tabel sensor_logs, perangkat
│   │       ├── routes/
│   │       │   ├── sensors.js     ← CRUD endpoint sensor data
│   │       │   └── devices.js     ← CRUD endpoint perangkat
│   │       └── validators/
│   │           └── sensor.js      ← validasi input dengan Zod
│   └── Frontend/
│       ├── package.json           ← Parcel bundler
│       ├── src/
│       │   ├── index.html
│       │   ├── main.js
│       │   └── components/
│       │       ├── SensorTable.js ← render tabel data
│       │       ├── SensorForm.js  ← form tambah/edit data
│       │       └── Pagination.js  ← komponen paginasi
│       └── dist/                  ← hasil build Parcel
```

### Backend Requirements

- Database: SQLite dengan file `data/sensor.db`
- Schema otomatis dibuat saat pertama kali jalan
- Tabel `sensor_logs`: id, device_id, suhu, kelembaban, cahaya, tanah, timestamp
- Tabel `devices`: id, name, location, mac_address, created_at
- API Endpoints (RESTful):
  - `GET /api/sensor-logs` → list dengan paginasi (?page=1&limit=20) dan filter (?device_id=&from=&to=)
  - `POST /api/sensor-logs` → tambah log baru (validasi dengan Zod)
  - `GET /api/sensor-logs/:id` → detail satu log
  - `DELETE /api/sensor-logs/:id` → hapus log
  - `GET /api/sensor-logs/stats` → statistik (avg, min, max per 24 jam)
  - `GET /api/devices` + `POST /api/devices` + `DELETE /api/devices/:id`
- Input validation dengan Zod (bukan manual if/else)
- Error responses dengan format standar `{ error: string, details?: any }`
- Seed data: 100 row dummy sensor data untuk testing

### Frontend Requirements

- Tabel data sensor dengan kolom sortable (klik header kolom)
- Paginasi (prev/next, tampilkan total records)
- Form tambah data sensor baru dengan validasi client-side
- Confirm dialog sebelum hapus
- Filter by tanggal (dari–sampai)
- Auto-refresh tabel setiap 10 detik
- Toast notification untuk sukses/error operasi
- Loading skeleton saat fetch data

### Konsep yang Didemonstrasikan

- SQL Schema Design, normalisasi dasar (1NF, 2NF)
- Prepared statements (mencegah SQL injection)
- Input validation di backend (Zod)
- Pagination and filtering query patterns
- HTTP status codes yang tepat (200, 201, 400, 404, 500)
- Frontend state management tanpa library (vanilla JS modules)

---

## PERCOBAAN 3: Vue 3 + Vite + Python FastAPI

**Tema:** Modern SPA framework, reactive UI, Python async backend  
**Estimasi waktu:** 75 menit  
**Backend:** Python 3.10 + FastAPI + SQLAlchemy 2.0 + SQLite  
**Frontend:** Vue 3 + Vite + TypeScript + Pinia

### Struktur File

```
percobaan 3/
├── server.sh
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── requirements.txt
│   │   ├── .env.example
│   │   ├── main.py                ← FastAPI app entry, CORS, lifespan
│   │   ├── database.py            ← SQLAlchemy engine, session
│   │   ├── models/
│   │   │   ├── sensor.py          ← SQLAlchemy model
│   │   │   └── device.py
│   │   ├── schemas/
│   │   │   ├── sensor.py          ← Pydantic v1 schemas
│   │   │   └── device.py
│   │   └── routers/
│   │       ├── sensors.py
│   │       └── devices.py
│   └── Frontend/
│       ├── package.json
│       ├── vite.config.ts
│       ├── tsconfig.json
│       ├── index.html
│       └── src/
│           ├── main.ts
│           ├── App.vue
│           ├── stores/
│           │   └── sensor.ts      ← Pinia store
│           ├── components/
│           │   ├── SensorCard.vue
│           │   ├── SensorChart.vue ← Chart.js wrapper
│           │   └── DataTable.vue
│           ├── composables/
│           │   └── useApi.ts      ← API call composable
│           ├── types/
│           │   └── sensor.ts      ← TypeScript interfaces
│           └── views/
│               ├── Dashboard.vue
│               └── History.vue
```

### Backend Requirements (FastAPI)

- Python 3.10 FastAPI dengan `async def` endpoints
- SQLAlchemy 2.0 async dengan aiosqlite
- Pydantic v1 (compatible Python 3.10) untuk request/response schemas
- Auto-generated OpenAPI docs di `/docs`
- Endpoints:
  - `GET /api/v1/sensors/latest` → data sensor terbaru (simulasi)
  - `GET /api/v1/sensors/history` → riwayat dengan paginasi
  - `POST /api/v1/sensors` → tambah data
  - `GET /api/v1/sensors/stats` → statistik
  - Server-Sent Events `GET /api/v1/sensors/stream` → streaming data real-time setiap 2 detik
- Background task: generate simulasi sensor data setiap 5 detik ke database
- CORS middleware
- Health check endpoint

### Frontend Requirements (Vue 3 + TypeScript)

- Vue 3 dengan Composition API (`<script setup>`)
- TypeScript strict mode
- Pinia store untuk global state sensor data
- Vue Router untuk navigasi (Dashboard, History, Settings)
- Chart.js integrasi lewat custom composable untuk grafik:
  - Line chart suhu dan kelembaban (60 titik terakhir)
  - Gauge chart untuk nilai terkini
- Server-Sent Events subscription untuk data real-time
- Component `SensorCard` yang reusable dengan props typed
- Vite proxy config untuk development (hindari CORS di dev)
- Build ke `dist/` yang di-serve oleh FastAPI sebagai static files

### Konsep yang Didemonstrasikan

- Vue 3 Composition API vs Options API
- Reactive state dengan `ref`, `computed`, `watch`
- TypeScript generics untuk API response
- Server-Sent Events (SSE) — alternatif real-time tanpa WebSocket
- FastAPI async programming model
- Pydantic v1 schema validation dan serialization (kompatibel Python 3.10)

---

## PERCOBAAN 4: Angular + TypeScript + Python FastAPI + JWT Auth

**Tema:** Enterprise-grade frontend, Python backend dengan autentikasi JWT  
**Estimasi waktu:** 90 menit  
**Backend:** Python 3.10 + FastAPI + python-jose + SQLite  
**Frontend:** Angular 17 + TypeScript + Angular Material

### Struktur File

```
percobaan 4/
├── server.sh
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── requirements.txt
│   │   ├── .env.example
│   │   ├── main.py                ← FastAPI app + CORS + lifespan
│   │   └── app/
│   │       ├── config.py          ← settings dari .env (pydantic-settings)
│   │       ├── database.py        ← SQLAlchemy + SQLite, session factory
│   │       ├── models/
│   │       │   ├── user.py        ← SQLAlchemy User model
│   │       │   ├── sensor.py      ← SQLAlchemy Sensor model
│   │       │   └── device.py
│   │       ├── schemas/
│   │       │   ├── auth.py        ← Pydantic: LoginRequest, TokenResponse
│   │       │   ├── sensor.py
│   │       │   └── device.py
│   │       ├── routers/
│   │       │   ├── auth.py        ← POST /auth/login, POST /auth/register
│   │       │   ├── sensors.py     ← protected CRUD + WebSocket
│   │       │   └── devices.py
│   │       ├── services/
│   │       │   ├── auth_service.py ← hash password, create/verify JWT
│   │       │   └── sensor_service.py
│   │       └── dependencies.py    ← FastAPI Depends: get_current_user
│   └── Frontend/
│       ├── package.json
│       ├── angular.json
│       ├── tsconfig.json
│       └── src/
│           ├── app/
│           │   ├── app.config.ts
│           │   ├── app.routes.ts
│           │   ├── core/
│           │   │   ├── auth/
│           │   │   │   ├── auth.service.ts    ← login, store token localStorage
│           │   │   │   └── auth.guard.ts      ← canActivate cek token
│           │   │   ├── interceptors/
│           │   │   │   └── jwt.interceptor.ts ← tambah Bearer header
│           │   │   └── models/
│           │   │       └── sensor.model.ts
│           │   ├── features/
│           │   │   ├── dashboard/
│           │   │   │   └── dashboard.component.ts ← real-time via WebSocket
│           │   │   ├── sensors/
│           │   │   │   └── sensors.component.ts
│           │   │   └── login/
│           │   │       └── login.component.ts
│           │   └── shared/
│           │       └── components/
│           │           └── sensor-card/
```

### Backend Requirements (Python FastAPI + JWT)

- Python 3.10 FastAPI dengan `async def` endpoints
- `python-jose[cryptography]` untuk JWT (HS256)
- `passlib[bcrypt]` untuk hash password
- SQLAlchemy 2.0 dengan aiosqlite
- Endpoints:
  - `POST /api/v1/auth/login` → return `{ access_token, token_type, expires_in }`
  - `POST /api/v1/auth/register` → register user baru
  - `GET /api/v1/sensors` (protected) → list sensor data
  - `POST /api/v1/sensors` (protected) → tambah data
  - `WS /api/v1/sensors/ws` → WebSocket endpoint broadcast data setiap 2 detik
  - `GET /api/v1/devices` + CRUD (protected)
- Background asyncio task: broadcast simulasi sensor data ke semua WebSocket clients
- Dependency injection `get_current_user` menggunakan FastAPI `Depends`
- CORS middleware, OpenAPI docs di `/docs`
- Graceful shutdown dengan `lifespan` context manager

### Frontend Requirements (Angular 17)

- Angular Standalone Components (tanpa NgModule)
- Angular Material UI (mat-card, mat-table, mat-toolbar, mat-button)
- Angular Signals untuk state management
- HttpClient dengan typed responses
- JWT interceptor: tambah `Authorization: Bearer <token>` ke setiap request
- Auth guard untuk protected routes
- WebSocket service untuk real-time data dari `/api/v1/sensors/ws`
- Reactive forms untuk login dengan Angular Validators
- Chart.js integrasi di Angular component
- `ng build --configuration production` output ke `dist/`

### Konsep yang Didemonstrasikan

- JWT authentication flow (login → store token → include in headers)
- Angular architecture: services, components, guards, interceptors
- FastAPI WebSocket: asyncio background task broadcast ke multiple clients
- FastAPI Depends: dependency injection untuk auth middleware
- bcrypt password hashing: keamanan credentials

---

## PERCOBAAN 5: React + Vite + Node.js WebSocket Server

**Tema:** React ecosystem modern, Node.js real-time WebSocket, full-stack JS  
**Estimasi waktu:** 90 menit  
**Backend:** Node.js 20 + Express 5 + ws (WebSocket) + SQLite  
**Frontend:** React 18 + Vite + TypeScript + TailwindCSS + shadcn/ui

### Struktur File

```
percobaan 5/
├── server.sh
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js               ← Express entry + WebSocket upgrade
│   │       ├── database/
│   │       │   ├── db.js              ← better-sqlite3 setup
│   │       │   └── schema.sql
│   │       ├── routes/
│   │       │   ├── sensors.js         ← REST CRUD sensor
│   │       │   └── devices.js
│   │       ├── websocket/
│   │       │   └── server.js          ← ws library, broadcast manager
│   │       └── jobs/
│   │           └── simulator.js       ← setInterval generate sensor data, broadcast
│   └── Frontend/
│       ├── package.json
│       ├── vite.config.ts
│       ├── tailwind.config.ts
│       ├── tsconfig.json
│       └── src/
│           ├── main.tsx
│           ├── App.tsx
│           ├── hooks/
│           │   ├── useSensorWS.ts     ← custom hook WebSocket lifecycle
│           │   └── useApiQuery.ts     ← fetch wrapper dengan loading/error
│           ├── store/
│           │   └── sensorStore.ts     ← Zustand store
│           ├── components/
│           │   ├── ui/                ← shadcn/ui (Card, Button, Badge, Toast)
│           │   ├── SensorDashboard.tsx
│           │   ├── RealtimeChart.tsx  ← Recharts AreaChart
│           │   └── DataTable.tsx      ← TanStack Table
│           └── types/
│               └── sensor.ts
```

### Backend Requirements (Node.js + WebSocket)

- Node.js 20 + Express 5 dengan ES modules
- `ws` library untuk WebSocket server (bukan Socket.IO — lebih ringan, standar)
- WebSocket di-attach ke Express HTTP server: `new WebSocket.Server({ server })`
- `better-sqlite3` untuk SQLite
- REST API:
  - `GET /api/sensors` → list dengan paginasi dan filter ?limit=&device=
  - `POST /api/sensors` → tambah manual
  - `GET /api/sensors/stats` → statistik (avg/min/max per hari)
  - `GET /api/devices` + CRUD
- WebSocket broadcast:
  - Setiap 2 detik: `setInterval` generate data simulasi → simpan ke SQLite → broadcast JSON ke semua client
  - Format message: `{ type: "sensor_update", data: {...}, ts: ISO8601 }`
  - Heartbeat: server ping setiap 30 detik, client harus pong
- Graceful shutdown: tutup semua WS connection, lalu HTTP server

### Frontend Requirements (React + Vite)

- React 18 dengan hooks (useState, useEffect, useCallback, useMemo)
- TailwindCSS + shadcn/ui untuk components (Card, Button, Table, Badge, Toast)
- Zustand untuk global state management
- Custom hook `useSensorWS`:
  - Connect ke `ws://localhost:PORT/ws`
  - Handle open, message, close, error
  - Auto-reconnect dengan exponential backoff
  - Expose `{ data, status, lastUpdate }`
- Recharts untuk visualisasi:
  - AreaChart suhu dan kelembaban (50 titik rolling window)
  - BarChart statistik harian
- TanStack Table untuk tabel data sortable + filterable
- React Router v6 (Dashboard, Historical Data, Settings)
- Vite proxy config: `/ws` → `ws://localhost:PORT` untuk dev mode

### Konsep yang Didemonstrasikan

- WebSocket native (ws library): upgrade HTTP → WebSocket
- Broadcast pattern: satu server kirim ke semua clients terhubung
- React custom hooks: encapsulate WebSocket lifecycle dengan cleanup
- Zustand: minimal state management vs Redux
- TanStack Table: headless UI pattern
- Recharts: deklaratif chart di React
- shadcn/ui: copy-paste component pattern

---

## PERCOBAAN 6: ESP32 MQTT Dasar + Node.js Broker Bridge

**Tema:** First contact IoT — ESP32 ke MQTT ke Dashboard  
**Estimasi waktu:** 60 menit  
**Backend:** Node.js + Express + MQTT.js + SQLite  
**Frontend:** Vanilla HTML + CSS + JS (no bundler)  
**ESP32:** WiFi Station + MQTT Publisher + basic Subscriber  
**EdgePython:** Python + paho-mqtt subscriber sederhana

### Struktur File

```
percobaan 6/
├── server.sh                      ← menu 1-5 + build ESP32
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js           ← Express + MQTT client
│   │       ├── mqtt/
│   │       │   └── client.js      ← MQTT.js subscriber, data store in-memory
│   │       ├── routes/
│   │       │   └── data.js        ← REST API for frontend
│   │       └── database/
│   │           └── db.js          ← SQLite log MQTT messages
│   └── Frontend/
│       ├── index.html
│       ├── css/style.css
│       └── js/
│           ├── app.js             ← polling REST API setiap 2 detik
│           └── mqtt-status.js     ← tampilkan status MQTT broker
├── ESP32/
│   ├── percobaan6/
│   │   ├── percobaan6.ino         ← main Arduino sketch
│   │   └── config.h               ← WiFi credentials, MQTT server IP
│   └── libraries.txt              ← daftar library yang dibutuhkan
└── EdgePython/
    ├── requirements.txt
    ├── .env.example
    └── main.py                    ← paho-mqtt subscriber, print ke terminal
```

### Backend Requirements (Node.js + MQTT)

- Node.js sebagai **MQTT Bridge**: subscribe ke Mosquitto, expose data via REST API ke frontend
- Topic yang disubscribe: `esp32/sensor/+`, `esp32/status`, `esp32/aktuator/#`
- SQLite: log setiap MQTT message yang masuk (topic, payload, timestamp, qos)
- REST API untuk frontend:
  - `GET /api/latest` → data terbaru per topic (dari in-memory cache)
  - `GET /api/history` → riwayat dari SQLite (50 terakhir)
  - `GET /api/broker/status` → status koneksi ke Mosquitto
  - `POST /api/command` → publish pesan ke `esp32/command/led` (kontrol LED)
- Environment: `MQTT_BROKER_URL`, `MQTT_USERNAME`, `MQTT_PASSWORD`, `PORT`

### Frontend Requirements

- Dashboard sederhana:
  - Status badge: MQTT Connected / Disconnected (polling `/api/broker/status`)
  - 3 kartu sensor: suhu, kelembaban, cahaya (dari `/api/latest`)
  - Tombol ON/OFF untuk LED via `POST /api/command`
  - Tabel riwayat 20 pesan MQTT terakhir (dari `/api/history`)
- Auto-refresh setiap 2 detik
- Indikator "LIVE" yang berkedip saat data baru masuk

### ESP32 Requirements (Arduino/C++)

- File: `percobaan6.ino` dan `config.h`
- WiFi Station mode (connect ke router)
- MQTT via PubSubClient library
- Publish setiap 3 detik ke:
  - `esp32/sensor/suhu` → float suhu (simulasi atau dari DHT22)
  - `esp32/sensor/kelembaban` → float kelembaban
  - `esp32/sensor/cahaya` → int ADC LDR (0-4095)
  - `esp32/status` → `{ "ip": "...", "rssi": -70, "uptime": 123 }` (JSON)
- Subscribe ke:
  - `esp32/command/led` → payload "on"/"off" untuk kontrol LED GPIO2
- LWT (Last Will): topic `esp32/status`, payload `"offline"`, QoS 1, retain true
- Reconnect otomatis ke WiFi dan MQTT jika koneksi putus

### EdgePython Requirements

- paho-mqtt subscriber
- Subscribe ke `esp32/#` (semua topik ESP32)
- Print setiap pesan dengan format: `[TIMESTAMP] TOPIC: PAYLOAD`
- Filter alert: jika suhu > 35°C, print warning dengan warna merah (colorama)
- Tulis log ke file `edge_log.txt` dengan rotation (max 1MB, 3 backup)
- Config via `.env` file (MQTT broker URL)

### server.sh Opsi 4 — Build All

```bash
# Install mosquitto jika belum ada
sudo apt-get install -y mosquitto mosquitto-clients
# Install arduino-cli jika belum ada
# Install library ESP32: PubSubClient, ArduinoJson
arduino-cli lib install "PubSubClient" "ArduinoJson" "DHT sensor library"
# Compile ESP32 sketch
arduino-cli compile --fqbn esp32:esp32:esp32 ESP32/percobaan6/
# npm install backend
cd Server/Backend && npm install
# pip install EdgePython  
cd EdgePython && pip3 install -r requirements.txt
```

### Konsep yang Didemonstrasikan

- MQTT Publish/Subscribe pattern
- Topic hierarchy dan naming convention
- LWT (Last Will and Testament) untuk deteksi disconnect
- MQTT QoS 0 vs 1 tradeoff
- Bridge pattern: ESP32 → MQTT Broker → Node.js → REST → Browser
- arduino-cli untuk build ESP32 dari command line

---

## PERCOBAAN 7: Multi-Sensor Real-time Dashboard + Time Series

**Tema:** IoT sensor fusion, time series data, real-time grafik  
**Estimasi waktu:** 75 menit  
**Backend:** Python 3.10 + FastAPI + aiomqtt + SQLite (time series style)  
**Frontend:** Vue 3 + Vite + TypeScript + Chart.js  
**ESP32:** WiFi + MQTT + DHT22 + LDR + Potensiometer  
**EdgePython:** Data aggregator + alert engine

### Struktur File

```
percobaan 7/
├── server.sh
├── README.md
├── Server/
│   ├── Backend/
│   │   ├── requirements.txt
│   │   ├── .env.example
│   │   ├── main.py
│   │   └── app/
│   │       ├── mqtt_handler.py    ← aiomqtt subscriber task
│   │       ├── database.py        ← SQLite dengan tabel time series
│   │       ├── models.py          ← SQLAlchemy models
│   │       ├── schemas.py         ← Pydantic schemas
│   │       ├── routers/
│   │       │   ├── sensors.py     ← REST + SSE endpoint
│   │       │   └── alerts.py      ← alert management
│   │       └── services/
│   │           └── aggregation.py ← statistik per menit/jam/hari
│   └── Frontend/
│       ├── package.json
│       ├── vite.config.ts
│       └── src/
│           ├── components/
│           │   ├── RealtimeChart.vue     ← Chart.js multi-dataset
│           │   ├── SensorGauge.vue       ← radial gauge custom
│           │   ├── AlertBanner.vue       ← notifikasi alert
│           │   └── StatsCard.vue
│           ├── composables/
│           │   └── useMqttData.ts        ← SSE consumer
│           └── views/
│               ├── Dashboard.vue         ← real-time view
│               └── Analytics.vue         ← grafik historis
├── ESP32/
│   ├── percobaan7/
│   │   ├── percobaan7.ino
│   │   └── config.h
│   └── libraries.txt
└── EdgePython/
    ├── requirements.txt
    ├── .env.example
    └── main.py                    ← aggregator + alert rules engine
```

### Backend Requirements (FastAPI + aiomqtt)

- `aiomqtt` sebagai async MQTT client (jalankan di asyncio background task)
- Python 3.10 — gunakan `asyncio.create_task()` di lifespan FastAPI
- Terima dari ESP32 JSON payload: `{ "suhu": 28.5, "kelembaban": 65.2, "cahaya": 1234, "tanah": 2100, "device": "esp32-01", "ts": 1710000000 }`
- SQLite time series: tabel `sensor_readings(id, device_id, suhu, kelembaban, cahaya, tanah, created_at)`
- Aggregation queries untuk:
  - rata-rata per menit (last 60 menit)
  - rata-rata per jam (last 24 jam)
  - min/max/avg per hari
- SSE endpoint `/api/stream` yang push data terbaru setiap 2 detik
- Alert system:
  - `GET /api/alerts` → list alert aktif
  - Alert otomatis dibuat saat threshold dilanggar (konfigurasi via env)
  - Alert resolved otomatis saat kondisi normal kembali

### ESP32 Requirements

- Baca 4 sensor: DHT22 (GPIO4), LDR+ADC (GPIO34), Potensiometer+ADC (GPIO35)
- Kirim JSON ke topik `sensor/data` setiap 2 detik
- LED status: berkedip cepat saat publish, solid saat error MQTT
- Watchdog timer: reset otomatis jika loop hang > 30 detik

### EdgePython Requirements

- Subscribe `sensor/data` dan `sensor/alert`
- Rules engine: list aturan dari file `rules.json`
  ```json
  [{"field": "suhu", "op": ">", "value": 35, "action": "alert", "message": "Suhu kritis!"}]
  ```
- Publish alert ke `sensor/alert` jika aturan terpenuhi
- Hitung rolling average 5 menit, publish ke `sensor/agg`
- Log ke file dengan structlog (JSON format)

### Konsep yang Didemonstrasikan

- Time series data: insert-only pattern, agregasi temporal
- SSE vs polling: kapan pakai mana
- Async Python: asyncio, aiomqtt, FastAPI concurrent request handling
- MQTT JSON payload: format standar IoT payload
- Alert rules engine: konfigurable business logic
- Sensor fusion: kombinasi multiple sensor dalam satu payload

---

## PERCOBAAN 8: Bidirectional MQTT — Kontrol Aktuator + Database Logging

**Tema:** Dua arah IoT, kontrol relay/LED, persistent logging dengan PostgreSQL  
**Estimasi waktu:** 90 menit  
**Backend:** Node.js 20 + Express 5 + MQTT.js + PostgreSQL (Docker)  
**Frontend:** React 18 + Vite + TypeScript + TailwindCSS  
**ESP32:** Publisher sensor + Subscriber perintah aktuator  
**EdgePython:** Edge bridge + command processor

### Struktur File

```
percobaan 8/
├── server.sh                      ← Docker PostgreSQL start juga di opsi 1
├── README.md
├── docker-compose.yml             ← PostgreSQL service
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js               ← Express entry + WebSocket server
│   │       ├── mqtt/
│   │       │   └── client.js          ← MQTT.js: subscribe sensor, publish command
│   │       ├── database/
│   │       │   ├── postgres.js        ← pg pool setup
│   │       │   └── schema.sql         ← sensor_logs, command_logs tables
│   │       ├── routes/
│   │       │   ├── sensor.js          ← GET history, stats (cursor pagination)
│   │       │   └── actuator.js        ← POST command → publish ke MQTT
│   │       └── websocket/
│   │           └── broadcast.js       ← ws broadcast manager
│   └── Frontend/
│       ├── package.json
│       ├── vite.config.ts
│       └── src/
│           ├── App.tsx
│           ├── hooks/
│           │   └── useWebSocket.ts    ← WebSocket hook dengan reconnect
│           ├── components/
│           │   ├── ActuatorControl.tsx ← kontrol LED/relay (toggle)
│           │   ├── SensorHistory.tsx   ← tabel historis dengan pagination
│           │   ├── CommandLog.tsx      ← riwayat command
│           │   └── SystemStatus.tsx    ← status MQTT + DB
│           └── store/
│               └── systemStore.ts     ← Zustand
├── ESP32/
│   ├── percobaan8/
│   │   ├── percobaan8.ino
│   │   └── config.h
│   └── libraries.txt
└── EdgePython/
    ├── requirements.txt
    ├── .env.example
    └── main.py                    ← command validator + rate limiter
```

### Backend Requirements (Node.js + MQTT.js + PostgreSQL)

- `mqtt` package (MQTT.js) untuk connection ke Mosquitto broker
- Subscribe: `esp32/sensor/#` → parse JSON → insert ke PostgreSQL
- `ws` library WebSocket server: broadcast setiap data baru ke browser clients
- `pg` library (node-postgres) untuk PostgreSQL connection pool
- REST API:
  - `POST /api/actuator/led` → body `{ "led": 1, "state": "on" }` → publish ke `esp32/command/led1`
  - `POST /api/actuator/relay` → publish ke `esp32/command/relay`
  - `GET /api/sensor/history` → cursor-based pagination (?cursor=&limit=20)
  - `GET /api/commands/log` → riwayat command dari `command_logs` table
  - `WS /ws` → WebSocket endpoint real-time push ke frontend
- PostgreSQL schema (via `schema.sql`, dijalankan saat startup):
  - `sensor_logs(id SERIAL, device_id, suhu, kelembaban, cahaya, tanah, created_at)`
  - `command_logs(id SERIAL, command_type, payload, status, created_at)`
- Setiap command POST juga INSERT ke `command_logs` untuk audit trail
- Docker Compose untuk PostgreSQL (port 5432)
- Graceful shutdown: tutup MQTT, WebSocket clients, PostgreSQL pool

### ESP32 Requirements

- 3 LED di GPIO2, GPIO4, GPIO5 (representasi aktuator)
- 1 relay simulasi (GPIO15) — LED merah terang
- Publish sensor data ke `esp32/sensor/data` (JSON) setiap 3 detik
- Subscribe `esp32/command/#`:
  - `esp32/command/led1`, `led2`, `led3` → kontrol setiap LED
  - `esp32/command/relay` → kontrol relay
  - `esp32/command/all` → matikan/nyalakan semua
- Feedback: setelah terima command, publish acknowledgement ke `esp32/ack/` dengan status sukses/gagal

### EdgePython Requirements

- **Command Validator**: subscribe `esp32/command/#` → validasi payload (JSON schema check) → republish jika valid, drop jika tidak
- Rate limiter: max 10 command per menit per jenis aktuator (pakai `collections.deque`)
- Log semua command ke file `command_audit.log` dengan timestamp
- Subscribe `esp32/ack/#` → tampilkan acknowledgement di terminal dengan warna (colorama)

### Konsep yang Didemonstrasikan

- MQTT bidirectional: command flow dan acknowledgement pattern
- PostgreSQL dengan Docker Compose: setup database production-grade
- Cursor-based pagination: lebih efisien dari offset untuk data besar
- Node.js WebSocket broadcast: event-driven pattern
- Command logging untuk audit trail IoT
- Rate limiting di edge layer mencegah aktuator overload

---

## PERCOBAAN 9: Full IoT Dashboard + Autentikasi + Alerting

**Tema:** Sistem IoT siap produksi dengan auth, dashboard advanced, notifikasi  
**Estimasi waktu:** 105 menit  
**Backend:** Python 3.10 + FastAPI + aiomqtt + python-jose + PostgreSQL (Docker)  
**Frontend:** Angular 17 + TypeScript + Angular Material + WebSocket  
**ESP32:** Multi-sensor + mode auto/manual + status reporting  
**EdgePython:** ML anomaly detection + alert publisher

### Struktur File

```
percobaan 9/
├── server.sh
├── README.md
├── docker-compose.yml             ← PostgreSQL + Mosquitto
├── Server/
│   ├── Backend/
│   │   ├── requirements.txt
│   │   ├── .env.example
│   │   ├── main.py                ← FastAPI app + lifespan + CORS
│   │   └── app/
│   │       ├── config.py          ← pydantic-settings dari .env
│   │       ├── database.py        ← SQLAlchemy async + PostgreSQL (asyncpg)
│   │       ├── mqtt_handler.py    ← aiomqtt background task
│   │       ├── models/
│   │       │   ├── sensor.py
│   │       │   ├── alert.py
│   │       │   ├── user.py
│   │       │   └── device.py
│   │       ├── schemas/
│   │       │   ├── auth.py        ← LoginRequest, TokenResponse
│   │       │   ├── sensor.py
│   │       │   └── alert.py
│   │       ├── routers/
│   │       │   ├── auth.py        ← POST login, register, refresh
│   │       │   ├── sensors.py     ← CRUD + export CSV + WebSocket
│   │       │   ├── alerts.py      ← list, acknowledge, resolve, delete
│   │       │   └── devices.py
│   │       ├── services/
│   │       │   ├── auth_service.py   ← JWT dengan python-jose
│   │       │   ├── alert_service.py  ← create/resolve alert + WebSocket notify
│   │       │   └── mqtt_service.py   ← publish command ke ESP32
│   │       ├── dependencies.py    ← get_current_user, get_db
│   │       └── websocket_manager.py ← broadcast ke semua WS clients
│   └── Frontend/
│       └── src/app/
│           ├── core/
│           │   ├── auth/
│           │   │   ├── auth.service.ts
│           │   │   └── auth.guard.ts
│           │   ├── websocket/
│           │   │   └── ws.service.ts       ← WebSocket native Angular service
│           │   └── interceptors/
│           │       └── jwt.interceptor.ts
│           └── features/
│               ├── dashboard/              ← real-time sensor overview
│               ├── analytics/              ← Chart.js grafik historis
│               ├── alerts/                 ← manajemen alert
│               ├── devices/                ← device management
│               └── settings/               ← konfigurasi threshold
├── ESP32/
│   ├── percobaan9/
│   │   ├── percobaan9.ino
│   │   └── config.h
│   └── libraries.txt
└── EdgePython/
    ├── requirements.txt
    ├── models/
    │   └── anomaly_model.pkl      ← pre-trained IsolationForest
    ├── train_model.py             ← fetch data dari REST API, fit, save pkl
    └── main.py                    ← inference + publish anomaly alerts
```

### Backend Requirements (Python FastAPI + PostgreSQL)

- `aiomqtt` untuk subscribe MQTT dalam asyncio background task
- `asyncpg` + SQLAlchemy 2.0 async untuk PostgreSQL
- `python-jose[cryptography]` + `passlib[bcrypt]` untuk JWT auth
- FastAPI WebSocket: `websocket_manager.py` manage connection pool, broadcast ke subscriber
- Alert system:
  - Saat threshold dilanggar → buat alert di PostgreSQL → broadcast via WebSocket ke frontend
  - `alert_service` cek kondisi setiap kali data MQTT masuk
  - Alert auto-resolve saat kondisi kembali normal
- REST API:
  - `/api/auth/` (login, register, refresh token)
  - `/api/sensors/` (CRUD, history, stats, export CSV)
  - `/api/alerts/` (list, acknowledge, resolve, delete)
  - `/api/devices/` (CRUD, status, last seen)
  - `/api/dashboard/summary` → satu endpoint agregasi semua data
  - `WS /api/ws` → WebSocket realtime (requires JWT token sebagai query param)
- `functools.lru_cache` untuk cache query berat (stats, aggregation)

### ESP32 Requirements

- Mode **Auto** dan **Manual** yang bisa diswitch via MQTT atau tombol fisik
- Mode Auto: kipas nyala jika suhu > 30°C, pompa nyala jika kelembaban < 40%
- Mode Manual: semua kontrol dari MQTT command
- Publish status lengkap ke `esp32/status` setiap 5 detik (mode, aktuator state, sensor values, heap free)
- Subscribe `esp32/config` untuk update threshold tanpa reflash
- Persistent mode (simpan di EEPROM / Preferences library) agar tidak reset saat power off

### EdgePython Requirements

- scikit-learn IsolationForest untuk anomaly detection
- `train_model.py`: fetch 1000 data terakhir dari REST API → fit model → save pkl
- `main.py`: subscribe MQTT, buat feature vector [suhu, kelembaban, cahaya, tanah] → predict → jika anomaly: publish ke `esp32/anomaly` dan POST ke REST API alerts
- Running sebagai background process yang bisa distart dari server.sh opsi 5

### Konsep yang Didemonstrasikan

- FastAPI JWT: python-jose, token expiry, refresh token pattern
- FastAPI WebSocket + connection manager: pattern broadcast ke multiple clients
- aiomqtt: integrasi MQTT di dalam event loop FastAPI
- asyncpg: koneksi PostgreSQL fully async
- Alert lifecycle: created → acknowledged → resolved
- ML di Edge: IsolationForest unsupervised anomaly detection

---

## PERCOBAAN 10: Complete Greenhouse Monitoring System

**Tema:** Sistem IoT produksi penuh — greenhouse monitoring dan kontrol otomatis  
**Estimasi waktu:** 120 menit  
**Backend:** Microservices — Python 3.10 FastAPI (API) + Node.js 20 (MQTT Bridge) + PostgreSQL + Redis  
**Frontend:** React 18 + Vite + TypeScript + advanced dashboard  
**ESP32:** Greenhouse sensor suite + multi-actuator + auto mode lengkap  
**EdgePython:** Complete edge computing — MQTT bridge + data aggregation + ML rule engine

### Konteks Proyek

> **Greenhouse Smart Monitoring** — sistem monitoring pertanian cerdas yang memantau suhu, kelembaban udara, cahaya, dan kelembaban tanah secara real-time. Sistem mengontrol kipas, lampu grow light, dan pompa irigasi secara otomatis berdasarkan kondisi sensor atau override manual dari dashboard.

### Struktur File

```
percobaan 10/
├── server.sh                      ← orchestrate semua services
├── README.md
├── ARCHITECTURE.md                ← diagram arsitektur sistem
├── docker-compose.yml             ← PostgreSQL + Redis + Mosquitto
├── Server/
│   ├── Backend/
│   │   ├── api/                   ← FastAPI service (port 8000)
│   │   │   ├── requirements.txt
│   │   │   ├── main.py
│   │   │   └── app/
│   │   │       ├── routers/
│   │   │       │   ├── sensors.py
│   │   │       │   ├── actuators.py
│   │   │       │   ├── alerts.py
│   │   │       │   ├── auth.py
│   │   │       │   └── analytics.py
│   │   │       ├── services/
│   │   │       │   ├── sensor_service.py
│   │   │       │   ├── alert_service.py
│   │   │       │   └── cache_service.py  ← Redis cache
│   │   │       ├── models.py             ← SQLAlchemy
│   │   │       ├── schemas.py            ← Pydantic v1
│   │   │       └── database.py
│   │   └── mqtt-bridge/           ← Node.js MQTT bridge (port 3001)
│   │       ├── package.json
│   │       └── src/
│   │           ├── index.js
│   │           ├── mqtt/
│   │           │   └── client.js       ← MQTT.js, subscribe all topics
│   │           └── websocket/
│   │               └── server.js       ← ws library, broadcast ke browser
│   └── Frontend/
│       ├── package.json
│       ├── vite.config.ts
│       └── src/
│           ├── App.tsx
│           ├── pages/
│           │   ├── Dashboard.tsx        ← overview + aktuator control
│           │   ├── Analytics.tsx        ← grafik historis + export
│           │   ├── Alerts.tsx           ← manajemen alert
│           │   ├── Devices.tsx          ← registrasi device
│           │   └── Settings.tsx         ← konfigurasi threshold + rules
│           ├── components/
│           │   ├── GreenhouseMap.tsx    ← visual layout greenhouse (SVG)
│           │   ├── SensorDashboard.tsx
│           │   ├── ActuatorPanel.tsx
│           │   ├── AlertFeed.tsx        ← real-time alert stream
│           │   ├── AnalyticsChart.tsx   ← Recharts advanced
│           │   └── ExportModal.tsx      ← CSV/JSON export
│           ├── hooks/
│           │   ├── useWebSocket.ts      ← WebSocket ke mqtt-bridge
│           │   ├── useAlerts.ts
│           │   └── useSensorHistory.ts
│           └── store/
│               ├── sensorStore.ts       ← Zustand
│               ├── alertStore.ts
│               └── settingsStore.ts
├── ESP32/
│   ├── percobaan10/
│   │   ├── percobaan10.ino
│   │   ├── config.h
│   │   ├── sensors.h              ← sensor reading functions
│   │   ├── actuators.h            ← aktuator control functions
│   │   └── mqtt_handler.h         ← MQTT pub/sub
│   └── libraries.txt
└── EdgePython/
    ├── requirements.txt
    ├── config/
    │   └── rules.yaml             ← alert rules + thresholds
    ├── models/
    │   └── greenhouse_model.pkl
    ├── utils/
    │   ├── logger.py              ← structlog JSON logging
    │   └── metrics.py             ← Prometheus metrics export
    └── main.py                    ← orchestrator: MQTT + aggregation + ML + alerting
```

### Backend Requirements (Microservices Architecture)

**FastAPI Service (port 8000) — Python 3.10:**
- Redis untuk caching sensor data terbaru (TTL 5 detik)
- PostgreSQL untuk persistensi historis
- Endpoints:
  - `GET /api/v1/sensors/realtime` → dari Redis cache
  - `GET /api/v1/sensors/history` → dari PostgreSQL (cursor pagination)
  - `GET /api/v1/analytics/hourly` → agregasi per jam 24 jam terakhir
  - `GET /api/v1/analytics/daily` → agregasi per hari 30 hari
  - `GET /api/v1/alerts` + POST + PATCH (acknowledge) + DELETE
  - `POST /api/v1/actuators/command` → publish ke MQTT via aiomqtt
  - `GET /api/v1/export/sensors` → download CSV data historis
  - `POST /api/v1/settings/thresholds` → update threshold global
- Background task: cleanup data lama > 30 hari setiap tengah malam

**Node.js MQTT Bridge (port 3001):**
- subscribe semua topik ESP32 → update Redis → broadcast WebSocket ke connected browsers
- WebSocket server: `ws://localhost:3001/ws` 
- Payload WebSocket: `{ type: "sensor"|"alert"|"status", data: {...}, ts: ISO8601 }`
- Heartbeat mechanism: ping/pong setiap 30 detik

### Frontend Requirements (React Advanced)

- **Greenhouse Map**: representasi visual greenhouse dalam SVG, sensor nilai overlay di posisi yang tepat, warna berubah sesuai kondisi (hijau = normal, kuning = warning, merah = kritis)
- **Real-time Dashboard**: WebSocket ke MQTT bridge, data latensi < 500ms
- **Analytics Page**:
  - Multi-axis chart (suhu kiri, kelembaban kanan)
  - Date range picker (react-day-picker)
  - Export ke CSV dan JSON
  - Statistics summary cards
- **Actuator Panel**: toggle untuk 3 aktuator (kipas, lampu, pompa) dengan:
  - State indicator (on/off/auto)
  - Last changed timestamp
  - Override indicator ("manual override active")
- **Alerts Page**: filter by severity, paginasi, bulk acknowledge
- **Settings Page**: edit threshold per sensor, save ke backend
- Alert notification: toast pop-up saat alert baru masuk via WebSocket

### ESP32 Requirements (Production Grade)

- **Multi-file**: pisahkan `sensors.h`, `actuators.h`, `mqtt_handler.h`
- Baca 4 sensor setiap 2 detik: DHT22, LDR, Potensiometer (soil moisture sim), push button (manual override trigger)
- Kontrol 3 aktuator: GPIO2 (kipas/FAN), GPIO4 (lampu/LIGHT), GPIO5 (pompa/PUMP)
- Mode Auto: 
  - FAN ON jika suhu > THRESH_SUHU (default 30°C)
  - LIGHT ON jika cahaya < THRESH_CAHAYA (default 1500 ADC)
  - PUMP ON jika tanah < THRESH_TANAH (default 1800 ADC)
- Mode Manual: all control dari MQTT commands
- MQTT Topics publish:
  - `greenhouse/sensor` → JSON semua sensor setiap 2 detik
  - `greenhouse/status` → mode, aktuator state, uptime setiap 10 detik
  - `greenhouse/alert` → jika kondisi kritis (suhu > 38°C)
- MQTT Subscribe:
  - `greenhouse/command/#` → kontrol aktuator, ganti mode, update threshold
- Config update via `greenhouse/config` → simpan ke EEPROM (Preferences library)
- Hardware watchdog timer (30 detik timeout)

### EdgePython Requirements (Complete Edge Stack)

- **Multi-subscriber**: subscribe `greenhouse/#` semua topik
- **Aggregator**: hitung rolling 5-menit average, publish ke `greenhouse/agg`
- **Rules Engine**: baca `config/rules.yaml`, evaluasi kondisi per pesan, publish alert ke `greenhouse/alert` dan ke REST API
- **ML Anomaly Detection**: 
  - Load `models/greenhouse_model.pkl` (IsolationForest pretrained)
  - Feature vector: [suhu, kelembaban, cahaya, tanah], normalisasi dengan StandardScaler
  - Jika anomaly score < -0.2: publish ke `greenhouse/anomaly`
- **Prometheus Metrics**: export `/metrics` endpoint (port 8001) untuk monitoring
  - `greenhouse_temperature_celsius` gauge
  - `greenhouse_alerts_total` counter
  - `esp32_messages_received_total` counter
- **Structured Logging**: JSON log dengan structlog, rotasi file
- Jalankan sebagai main loop yang tidak keluar kecuali interrupt

### server.sh Opsi 4 dan 5

```bash
# Opsi 4: Build All
# - docker pull postgres:16-alpine redis:7-alpine eclipse-mosquitto
# - docker-compose up -d (postgres + redis + mosquitto)
# - pip install -r Server/Backend/api/requirements.txt
# - pip install -r EdgePython/requirements.txt
# - npm install di Server/Backend/mqtt-bridge/
# - npm install && npm run build di Server/Frontend/
# - arduino-cli compile ESP32/percobaan10/ --fqbn esp32:esp32:esp32
# - python EdgePython/train_model.py (generate dummy model)
# Tampilkan URL akses semua service

# Opsi 5: Run Edge Python
# - Jalankan python3 EdgePython/main.py di foreground (Ctrl+C untuk stop)
# - Tampilkan MQTT broker yang disubscribe
# - Print startup banner dengan semua konfigurasi aktif
```

### Konsep yang Didemonstrasikan

- Microservices: pisahkan concern antara API service dan MQTT bridge service
- Redis: cache layer untuk reduce database queries pada high-frequency sensor data
- WebSocket: low-latency push dari server ke browser (gantikan polling)
- SVG interaktif: representasi visual sistem fisik di browser
- Export data: CSV generation server-side
- Prometheus metrics: observability di edge
- YAML configuration: external config tanpa hardcode

---

## PANDUAN TAMBAHAN UNTUK IMPLEMENTASI

### Urutan Teknologi Per Percobaan

| Percobaan | Frontend | Backend | Database | Extra |
|-----------|----------|---------|----------|-------|
| 1 | Vanilla HTML/JS | Node.js 20 + Express | — | — |
| 2 | HTML/JS + Parcel | Node.js 20 + Express | SQLite | Zod validation |
| 3 | Vue 3 + Vite + TS | Python 3.10 + FastAPI | SQLite | SSE, Chart.js |
| 4 | Angular 17 + TS | Python 3.10 + FastAPI + JWT | SQLite | JWT auth, WebSocket |
| 5 | React + Vite + TS | Node.js 20 + Express + ws | SQLite | WebSocket native, Recharts |
| 6 | Vanilla HTML/JS | Node.js 20 + MQTT.js | SQLite | ESP32 + EdgePython |
| 7 | Vue 3 + Vite + TS | Python 3.10 + FastAPI + aiomqtt | SQLite | ESP32 + EdgePython ML rules |
| 8 | React + Vite + TS | Node.js 20 + MQTT.js | PostgreSQL | ESP32 + EdgePython validator |
| 9 | Angular 17 + TS | Python 3.10 + FastAPI + JWT | PostgreSQL | ESP32 + EdgePython IsolationForest |
| 10 | React + Vite + TS | Python 3.10 FastAPI + Node.js bridge | PostgreSQL + Redis | ESP32 + EdgePython full stack |

### Port Assignments

| Service | Port |
|---------|------|
| Percobaan 1 Backend | 3001 |
| Percobaan 2 Backend | 3002 |
| Percobaan 3 Backend | 8003 |
| Percobaan 4 Backend | 8004 |
| Percobaan 5 Backend | 8005 |
| Percobaan 6 Backend | 3006 |
| Percobaan 7 Backend | 8007 |
| Percobaan 8 Backend | 8008 |
| Percobaan 9 Backend | 8009 |
| Percobaan 10 API | 8010 |
| Percobaan 10 MQTT Bridge | 3010 |
| Frontend (semua percobaan dev) | Backend PORT + 1000 |
| Mosquitto MQTT | 1883 |
| Mosquitto WS | 9001 |
| PostgreSQL | 5432 |
| Redis | 6379 |
| EdgePython Metrics (P10) | 8001 |

### Standar Kode Wajib

**Struktur README.md setiap percobaan:**
```markdown
# Percobaan N: [Judul]
## Konsep
## Arsitektur (ASCII diagram)
## Cara Menjalankan
## Endpoint API
## Topik MQTT (untuk P6-10)
## Troubleshooting
```

**Error Response Format (semua backend):**
```json
{ "error": "Pesan error singkat", "detail": "Penjelasan teknis", "code": "ERROR_CODE" }
```

**MQTT Topic Convention:**
```
{prefix}/{device_id}/{sensor|command|status|alert}/{name}
esp32/sensor/suhu, esp32/command/led, greenhouse/sensor, greenhouse/command/fan
```

**Environment Variables (wajib, bukan hardcode):**
```
PORT, MQTT_BROKER_URL, MQTT_USERNAME, MQTT_PASSWORD, DB_URL, JWT_SECRET, REDIS_URL
```

---

## URUTAN OUTPUT YANG DIHARAPKAN

Untuk setiap percobaan yang diimplementasikan, hasilkan dalam urutan:

1. **`README.md`** — dokumentasi lengkap dengan diagram, cara jalankan, endpoint
2. **`server.sh`** — script lengkap dan fungsional dengan semua menu
3. **`Server/Backend/`** — semua file backend dengan struktur folder lengkap
4. **`Server/Frontend/`** — semua file frontend dengan konfigurasi build
5. **`ESP32/`** (P6-10) — sketch Arduino lengkap, termasuk `config.h`
6. **`EdgePython/`** (P6-10) — Python script dengan requirements.txt

Pastikan setiap file adalah **implementasi lengkap**, bukan skeleton atau TODO comment. Kode harus bisa langsung dijalankan setelah `npm install` atau `pip install -r requirements.txt`.

---

*Prompt ini dibuat untuk Modul 05: IoT WebServer MQTT ESP32 — Praktikum Mekatronika dan Robotika*  
*Teknologi Rekayasa Otomasi | March 2026*
