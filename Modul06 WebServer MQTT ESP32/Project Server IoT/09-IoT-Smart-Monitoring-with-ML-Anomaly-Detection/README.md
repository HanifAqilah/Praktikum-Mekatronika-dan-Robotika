# Percobaan 9 – IoT Smart Monitoring with ML Anomaly Detection

## Overview

Full-stack IoT monitoring platform integrating:
- **ESP32** sensors (suhu, kelembaban, cahaya, tanah) with auto/manual actuator control
- **MQTT** (Mosquitto) for real-time device communication
- **FastAPI** backend with PostgreSQL, WebSocket, and JWT authentication
- **Angular 17** frontend with real-time dashboard and charts
- **EdgePython** ML anomaly detection using scikit-learn IsolationForest

---

## Architecture

```
ESP32 ──MQTT──▶ Mosquitto ──aiomqtt──▶ FastAPI Backend ──▶ PostgreSQL
                    ▲                       │ WebSocket
                    │                       ▼
            EdgePython ML          Angular 17 Frontend
            (IsolationForest)        (port 9009)
```

---

## Quick Start

### 1. Prerequisites

```bash
# Required
docker, docker-compose
python3 (>= 3.10) + pip
node.js (>= 18) + npm
```

### 2. Start Infrastructure

```bash
./server.sh docker-up
# or via menu: ./server.sh → option 1
```

### 3. Install and Start Backend

```bash
cd Server/Backend
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
cp .env.example .env       # edit if needed
uvicorn main:app --port 8009 --reload
```

### 4. Install and Start Frontend

```bash
cd Server/Frontend
npm install
npx ng serve --port 9009
```
Open: http://localhost:9009

### 5. Flash ESP32

1. Install Arduino IDE + ESP32 board support
2. Install libraries listed in `ESP32/libraries.txt`
3. Edit `ESP32/percobaan9/config.h` (WiFi SSID/password, MQTT broker IP)
4. Open `ESP32/percobaan9/percobaan9.ino` and upload

### 6. EdgePython (ML Anomaly Detection)

```bash
cd EdgePython
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt

# After collecting data (200+ readings), train the model:
python3 train_model.py --api http://localhost:8009 --token YOUR_JWT_TOKEN

# Start anomaly detection:
python3 main.py
```

---

## API Reference

| Method | Endpoint                         | Description              |
|--------|----------------------------------|--------------------------|
| POST   | /api/auth/register               | Register user            |
| POST   | /api/auth/login                  | Login (returns JWT)      |
| POST   | /api/auth/refresh                | Refresh access token     |
| GET    | /api/auth/me                     | Current user info        |
| GET    | /api/sensors/                    | List sensor readings     |
| GET    | /api/sensors/history             | Historical data          |
| GET    | /api/sensors/stats               | Statistics               |
| GET    | /api/sensors/export/csv          | Export CSV               |
| GET    | /api/alerts/                     | List alerts              |
| POST   | /api/alerts/{id}/acknowledge     | Acknowledge alert        |
| POST   | /api/alerts/{id}/resolve         | Resolve alert            |
| DELETE | /api/alerts/{id}                 | Delete alert             |
| GET    | /api/devices/                    | List devices             |
| POST   | /api/devices/                    | Register device          |
| POST   | /api/devices/{id}/thresholds     | Update thresholds        |
| POST   | /api/devices/{id}/mode           | Toggle auto/manual       |
| POST   | /api/devices/{id}/actuator       | Control fan/pump         |
| GET    | /api/dashboard/summary           | Dashboard summary        |
| WS     | /api/ws?token=JWT                | WebSocket feed           |

API Docs: http://localhost:8009/docs

---

## MQTT Topics

| Topic                      | Direction      | Description                        |
|----------------------------|----------------|------------------------------------|
| `esp32/status`             | ESP32 → Server | Sensor + actuator status (5s)     |
| `esp32/config`             | Server → ESP32 | Threshold updates                  |
| `esp32/command/<device_id>`| Server → ESP32 | Mode/actuator commands             |
| `esp32/anomaly`            | Edge → Server  | ML anomaly detections              |

### ESP32 Status Payload

```json
{
  "device_id": "esp32-01",
  "firmware": "1.0.0",
  "suhu": 27.5,
  "kelembaban": 65.0,
  "cahaya": 850,
  "tanah": 55.0,
  "fan": false,
  "pump": false,
  "auto_mode": true,
  "rssi": -65
}
```

### Config Payload

```json
{
  "device_id": "esp32-01",
  "suhu_max": 30.0,
  "kelembaban_min": 40.0,
  "cahaya_min": 500.0,
  "tanah_min": 30.0
}
```

---

## Wiring Diagram

```
ESP32           Component
GPIO4   ──────► DHT22 (Data)
GPIO34  ──────► LDR (Analog)
GPIO35  ──────► Soil Moisture Sensor (Analog)
GPIO26  ──────► Relay IN1 (Fan)
GPIO27  ──────► Relay IN2 (Pump)
GPIO0   ──────► Mode Button (to GND, active LOW)
3.3V    ──────► DHT22 VCC, LDR VCC
GND     ──────► All GND
5V      ──────► Relay VCC
```

---

## Configuration

### Backend `.env`

```ini
DATABASE_URL=postgresql+asyncpg://iot_user:iot_password@localhost:5432/iot_db
MQTT_HOST=localhost
MQTT_PORT=1883
SECRET_KEY=change-this-to-a-random-secret
BACKEND_PORT=8009
SUHU_THRESHOLD=30.0
KELEMBABAN_THRESHOLD=40.0
```

### EdgePython `.env`

```ini
MQTT_HOST=localhost
MQTT_PORT=1883
API_URL=http://localhost:8009
API_TOKEN=your_jwt_token_here
PREDICTION_INTERVAL_S=60
```

---

## Directory Structure

```
percobaan 9/
├── server.sh                    # Management script (menu)
├── README.md
├── docker-compose.yml           # PostgreSQL + Mosquitto
├── mosquitto.conf
├── Server/
│   ├── Backend/                 # FastAPI Python backend
│   │   ├── main.py
│   │   ├── requirements.txt
│   │   ├── .env.example
│   │   └── app/
│   │       ├── config.py
│   │       ├── database.py
│   │       ├── mqtt_handler.py
│   │       ├── websocket_manager.py
│   │       ├── dependencies.py
│   │       ├── models/
│   │       ├── schemas/
│   │       ├── routers/
│   │       └── services/
│   └── Frontend/                # Angular 17 frontend
│       └── src/app/
│           ├── core/            # Auth, WebSocket, JWT interceptor
│           └── features/        # Dashboard, Analytics, Alerts, etc.
├── ESP32/
│   ├── percobaan9/
│   │   ├── percobaan9.ino
│   │   └── config.h
│   └── libraries.txt
└── EdgePython/
    ├── train_model.py
    ├── main.py
    ├── requirements.txt
    └── models/
        └── anomaly_model.pkl    (generated by train_model.py)
```
