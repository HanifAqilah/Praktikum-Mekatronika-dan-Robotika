# MODUL 13: WEBSERVER IoT (REST API, DASHBOARD, DATABASE)

## 1. DEFINISI

- **Web Server**: aplikasi yang menerima HTTP request dan mengembalikan response.
- **REST API**: gaya arsitektur web service yang stateless dan berbasis resource.
- **Dashboard**: UI web untuk monitoring dan kontrol robot.
- **WebSocket**: koneksi dua arah untuk data real-time.

---

## 2. KONSEP INTI

### 2.1 HTTP Method & Status Code
- **GET**: read
- **POST**: create
- **PUT**: update
- **DELETE**: remove

Status code: 200, 201, 400, 404, 500.

### 2.2 REST Endpoint
Contoh endpoint:
- `/api/robot/status`
- `/api/motor/1/position`
- `/api/motor/1/command`
- `/api/sensor/imu`

### 2.3 JSON Request/Response
Request body (POST):
```json
{"target_angle": 90, "speed": 1.0}
```

Response success:
```json
{"status": "success", "target": 90}
```

### 2.4 Authentication & CORS
- API key atau JWT
- CORS diperlukan untuk akses lintas origin dari browser.

### 2.5 Database & ORM
- Database menyimpan log sensor dan command.
- ORM (SQLAlchemy) menyederhanakan query.

### 2.6 WebSocket Real-Time
- Push data sensor tanpa polling.
- Cocok untuk dashboard live.

---

## 3. ARSITEKTUR RINGKAS

Client (Dashboard) → REST API → Database
Client (Dashboard) ↔ WebSocket ↔ Streaming data sensor

---

## 4. RINGKASAN
- REST API untuk kontrol dan data statis.
- WebSocket untuk data real-time.
- Database penting untuk history dan audit.
