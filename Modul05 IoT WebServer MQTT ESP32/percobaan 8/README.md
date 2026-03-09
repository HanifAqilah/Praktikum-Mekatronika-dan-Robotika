# Percobaan 8 — IoT WebServer MQTT ESP32

Sistem IoT lengkap menggunakan ESP32, Node.js backend (Express 5 + MQTT.js + PostgreSQL), React frontend, dan EdgePython bridge.

## Arsitektur

```
ESP32 ──► MQTT Broker ──► Backend (Node.js 20 + Express 5)
                │               │
                │        ┌──────┴───────┐
                │        │  PostgreSQL  │
                │        └──────────────┘
                │               │ WebSocket /ws
                │               ▼
          EdgePython      Browser (React 18 + Vite)
          (Validator)
```

## Komponen

| Komponen    | Teknologi                    | Port  |
|-------------|------------------------------|-------|
| Backend     | Node.js 20, Express 5, ws    | 8008  |
| Frontend    | React 18, Vite, TailwindCSS  | 9008  |
| Database    | PostgreSQL 16 (Docker)       | 5432  |
| MQTT Broker | Mosquitto / broker eksternal | 1883  |
| EdgePython  | Python 3.11+, paho-mqtt      | —     |

## Hardware ESP32

| Komponen  | GPIO | Keterangan             |
|-----------|------|------------------------|
| LED 1     | 2    | Aktuator biru          |
| LED 2     | 4    | Aktuator hijau         |
| LED 3     | 5    | Aktuator kuning        |
| Relay     | 15   | Simulasi relay (merah) |

## Prasyarat

- Node.js 20+
- Python 3.11+
- Docker & Docker Compose
- MQTT Broker (misal: Mosquitto)
- Arduino IDE dengan ESP32 core

## Cara Cepat

```bash
# 1. Jalankan menu interaktif
chmod +x server.sh
./server.sh

# Atau langsung jalankan komponen tertentu:
./server.sh install    # Install semua dependensi
./server.sh up         # Jalankan PostgreSQL (Docker)
./server.sh backend    # Jalankan backend
./server.sh frontend   # Jalankan frontend
./server.sh edge       # Jalankan EdgePython
./server.sh all        # Jalankan semua sekaligus
./server.sh status     # Cek status semua service
./server.sh stop       # Stop semua service
```

## Konfigurasi

### Backend
```bash
cd Server/Backend
cp .env.example .env
# Edit .env: POSTGRES_*, MQTT_BROKER, PORT
```

### EdgePython
```bash
cd EdgePython
cp .env.example .env
# Edit .env: MQTT_BROKER, RATE_LIMIT_MAX, dll
```

### ESP32
Edit `ESP32/percobaan8/config.h`:
```c
#define WIFI_SSID    "NamaWiFiAnda"
#define WIFI_PASSWORD "PasswordWiFiAnda"
#define MQTT_BROKER  "192.168.x.x"  // IP laptop/server
```

## API Endpoints

### Sensor
| Method | Path                    | Keterangan                        |
|--------|-------------------------|-----------------------------------|
| GET    | /api/sensor/history     | Riwayat sensor (cursor pagination)|
| GET    | /api/sensor/stats       | Statistik agregat                 |
| GET    | /api/sensor/latest      | Pembacaan terakhir                |

### Aktuator
| Method | Path                    | Body                              |
|--------|-------------------------|-----------------------------------|
| POST   | /api/actuator/led       | `{"led":1,"state":"on"}`          |
| POST   | /api/actuator/relay     | `{"state":"off"}`                 |
| POST   | /api/actuator/all       | `{"state":"on"}`                  |

### Command Log
| Method | Path                    | Keterangan                        |
|--------|-------------------------|-----------------------------------|
| GET    | /api/commands/log       | Log perintah (cursor pagination)  |

### System
| Method | Path                    | Keterangan                        |
|--------|-------------------------|-----------------------------------|
| GET    | /api/status             | Status MQTT, uptime               |
| WS     | /ws                     | WebSocket live data               |

## MQTT Topics

| Topic                  | Arah             | Payload contoh                          |
|------------------------|------------------|-----------------------------------------|
| `esp32/sensor/data`    | ESP32 → Broker   | `{"suhu":27.5,"kelembaban":65,...}`     |
| `esp32/command/led1`   | Backend → ESP32  | `{"led":1,"state":"on"}`               |
| `esp32/command/led2`   | Backend → ESP32  | `{"led":2,"state":"toggle"}`           |
| `esp32/command/led3`   | Backend → ESP32  | `{"led":3,"state":"off"}`              |
| `esp32/command/relay`  | Backend → ESP32  | `{"state":"on"}`                       |
| `esp32/command/all`    | Backend → ESP32  | `{"state":"off"}`                      |
| `esp32/ack/led1`       | ESP32 → Broker   | `{"status":"ok","state":"on"}`         |
| `esp32/ack/relay`      | ESP32 → Broker   | `{"status":"ok","state":"off"}`        |

## WebSocket Events

```json
{ "type": "sensor_data",  "payload": { "id": 1, "suhu": 27.5, ... } }
{ "type": "mqtt_status",  "payload": { "connected": true } }
{ "type": "actuator_ack", "payload": { "actuator": "led1", "status": "ok" } }
{ "type": "welcome",      "message": "Percobaan 8 WebSocket connected" }
```

## EdgePython Rate Limiter

- Maksimum **10 perintah per menit** per tipe aktuator
- Perintah yang melebihi batas di-drop dan dicatat di `command_audit.log`
- Validasi JSON schema dilakukan sebelum rate limiting

## Struktur File

```
percobaan 8/
├── server.sh               ← Menu manajemen
├── README.md
├── docker-compose.yml      ← PostgreSQL service
├── Server/
│   ├── Backend/
│   │   ├── package.json
│   │   ├── .env.example
│   │   └── src/
│   │       ├── index.js
│   │       ├── mqtt/client.js
│   │       ├── database/
│   │       │   ├── postgres.js
│   │       │   └── schema.sql
│   │       ├── routes/
│   │       │   ├── sensor.js
│   │       │   └── actuator.js
│   │       └── websocket/broadcast.js
│   └── Frontend/
│       ├── package.json
│       ├── vite.config.ts
│       └── src/
│           ├── App.tsx
│           ├── hooks/useWebSocket.ts
│           ├── components/
│           │   ├── ActuatorControl.tsx
│           │   ├── SensorHistory.tsx
│           │   ├── CommandLog.tsx
│           │   └── SystemStatus.tsx
│           └── store/systemStore.ts
├── ESP32/
│   ├── percobaan8/
│   │   ├── percobaan8.ino
│   │   └── config.h
│   └── libraries.txt
└── EdgePython/
    ├── requirements.txt
    ├── .env.example
    └── main.py
```
