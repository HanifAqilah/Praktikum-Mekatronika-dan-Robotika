# PROJECT MODUL 05: FULL-STACK WEB APPLICATION

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – Web Server & Full-Stack JavaScript  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)  
**Durasi:** 2 Minggu

---

## A. DESKRIPSI PROJECT

Setiap kelompok **memilih satu soal** dari 5 pilihan di bawah, lalu membangun aplikasi full-stack lengkap menggunakan:

- **Backend:** Node.js + Express.js
- **Database:** Sesuai rekomendasi per soal
- **Frontend:** React + Tailwind CSS
- **Real-time:** Socket.IO (minimal 1 fitur)
- **Auth:** JWT (register + login)
- **Deploy:** CloudFlare Tunnel

Tidak ada ESP32 atau hardware fisik — semua berbasis software web.

---

## B. SOAL PROJECT (PILIH 1)

---

### SOAL 1 — PERSONAL FINANCE TRACKER *(Level: Easy)*

> Bangun aplikasi pelacak keuangan pribadi. User bisa mencatat transaksi (pemasukan/pengeluaran), melihat saldo, dan menganalisis pengeluaran per kategori lewat grafik.

**Fitur Wajib:**

| Fitur | Keterangan |
|-------|-----------|
| Auth | Register, Login, Logout dengan JWT |
| Transaksi CRUD | Tambah, edit, hapus transaksi (jumlah, kategori, tanggal, catatan) |
| Filter & Search | Filter by kategori, bulan, tipe (income/expense) |
| Dashboard Summary | Saldo total, total income, total expense (bulan ini) |
| Chart | Pie chart pengeluaran per kategori (Chart.js) |
| Persisten | Data tersimpan di database, tidak hilang setelah restart |

**Stack Rekomendasi:**

```
Backend:  Express.js + better-sqlite3
Database: SQLite
Frontend: React + Vite + Tailwind CSS + Chart.js + Axios
Auth:     JWT + bcrypt
Deploy:   CloudFlare Tunnel
```

**Database Schema:**

```sql
-- users
CREATE TABLE users (
  id       INTEGER PRIMARY KEY AUTOINCREMENT,
  name     TEXT    NOT NULL,
  email    TEXT    UNIQUE NOT NULL,
  password TEXT    NOT NULL
);

-- transactions
CREATE TABLE transactions (
  id         INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id    INTEGER REFERENCES users(id),
  type       TEXT    CHECK(type IN ('income','expense')),
  amount     REAL    NOT NULL,
  category   TEXT    NOT NULL,
  note       TEXT,
  date       TEXT    DEFAULT (date('now'))
);
```

**API Endpoints:**

```
POST   /api/auth/register
POST   /api/auth/login

GET    /api/transactions           (protected)
POST   /api/transactions           (protected)
PUT    /api/transactions/:id       (protected, owner only)
DELETE /api/transactions/:id       (protected, owner only)
GET    /api/transactions/summary   (protected, aggregasi)
```

**Deliverable:**
- [ ] Kode backend (`backend/`)
- [ ] Kode frontend (`frontend/`)
- [ ] Screenshot: dashboard, chart, form tambah transaksi
- [ ] Video demo 5 menit
- [ ] URL CloudFlare Tunnel aktif

---

### SOAL 2 — NOTE-TAKING APP DENGAN MARKDOWN *(Level: Medium)*

> Bangun aplikasi catatan yang mendukung format Markdown — teks menjadi heading, bold, list, code block, dll. Catatan diorganisir dengan tag.

**Fitur Wajib:**

| Fitur | Keterangan |
|-------|-----------|
| Auth | Register, Login, JWT |
| CRUD Notes | Judul, konten Markdown, tag (array) |
| Markdown Preview | Split view: editor kiri, preview kanan |
| Tag Filter | Filter catatan berdasarkan tag |
| Search | Full-text search dalam judul dan isi |
| Sort | Sort by: created, updated, title |

**Stack Rekomendasi:**

```
Backend:  Express.js + mysql2
Database: MySQL
Frontend: React + react-markdown + CodeMirror (editor) + Tailwind
Auth:     JWT + bcrypt
Deploy:   CloudFlare Tunnel
```

**Fitur Markdown yang Harus Berfungsi:**
- Heading `# H1` `## H2`
- Bold `**text**`, Italic `*text*`
- Code block ` ```js ... ``` `
- Unordered & ordered list
- Blockquote `> text`
- Link `[text](url)`

**API Endpoints:**

```
POST   /api/auth/register
POST   /api/auth/login

GET    /api/notes            (protected, query: ?tag=&search=&sort=)
GET    /api/notes/:id        (protected)
POST   /api/notes            (protected)
PUT    /api/notes/:id        (protected)
DELETE /api/notes/:id        (protected)
GET    /api/notes/tags       (protected, semua tag unik user)
```

**Deliverable:**
- [ ] Kode backend
- [ ] Kode frontend dengan markdown preview berjalan
- [ ] Screenshot: editor + preview side-by-side, search & filter
- [ ] Video demo 5 menit

---

### SOAL 3 — REAL-TIME TASK BOARD *(Level: Medium)*

> Bangun Kanban board — kolom Todo, In Progress, Done. Task bisa dipindah antar kolom. Semua user yang membuka board melihat perubahan secara real-time (tanpa refresh).

**Fitur Wajib:**

| Fitur | Keterangan |
|-------|-----------|
| Auth | Register, Login, JWT |
| Board CRUD | Buat board, undang member |
| Task CRUD | Tambah, edit, hapus task dalam kolom |
| Drag-Drop | Pindah task antar kolom (react-beautiful-dnd atau dnd-kit) |
| Real-Time Sync | Socket.IO: perubahan task langsung sync ke semua user di board |
| Assignee | Assign task ke member board |

**Stack Rekomendasi:**

```
Backend:  Express.js + mongoose + socket.io
Database: MongoDB
Frontend: React + dnd-kit + Tailwind + socket.io-client
Auth:     JWT + bcrypt
Deploy:   CloudFlare Tunnel
```

**Socket.IO Events:**

```javascript
// Server emit ke semua user di room (boardId)
socket.to(boardId).emit('task_moved',   { taskId, fromCol, toCol });
socket.to(boardId).emit('task_created', task);
socket.to(boardId).emit('task_deleted', taskId);

// Client listen
socket.on('task_moved',   data => updateColumn(data));
socket.on('task_created', task => addTask(task));
```

**MongoDB Schema:**

```javascript
// Board
{ title: String, owner: ObjectId, members: [ObjectId], columns: ['Todo','In Progress','Done'] }

// Task
{ title: String, description: String, column: String, board: ObjectId,
  assignee: ObjectId, priority: 'low|medium|high', dueDate: Date }
```

**Deliverable:**
- [ ] Kode backend + frontend
- [ ] Screenshot: 2 browser tersinkronisasi real-time
- [ ] Video demo 5 menit (tunjukkan real-time di 2 tab)

---

### SOAL 4 — BLOG PLATFORM DENGAN AUTH *(Level: Hard)*

> Bangun platform blog multi-user. User bisa register, menulis post, memberi komentar. Admin dapat moderasi. Post menggunakan rich text editor.

**Fitur Wajib:**

| Fitur | Keterangan |
|-------|-----------|
| Auth | Register, Login, Role: admin/author/reader |
| Post CRUD | Judul, konten rich text, cover image upload, tag, published/draft |
| Komentar | Nested comments (reply to), CRUD oleh penulis |
| Search & Filter | Full-text search, filter by tag, sort by date/views |
| Pagination | Server-side pagination (limit + offset) |
| View Counter | Increment views tiap kali post dibuka |

**Stack Rekomendasi:**

```
Backend:  Express.js + pg (PostgreSQL) + multer (upload)
Database: PostgreSQL
Frontend: React + react-quill (rich text) + Tailwind + Axios
Auth:     JWT + bcrypt + role middleware
Deploy:   CloudFlare Tunnel
```

**PostgreSQL Schema:**

```sql
CREATE TABLE users (id SERIAL PRIMARY KEY, name TEXT, email TEXT UNIQUE, password TEXT, role TEXT DEFAULT 'author');
CREATE TABLE posts  (id SERIAL PRIMARY KEY, title TEXT, content TEXT, author_id INT REFERENCES users(id),
  tags TEXT[], published BOOLEAN DEFAULT false, views INT DEFAULT 0, created_at TIMESTAMP DEFAULT NOW());
CREATE TABLE comments (id SERIAL PRIMARY KEY, post_id INT REFERENCES posts(id),
  author_id INT REFERENCES users(id), content TEXT, parent_id INT REFERENCES comments(id), created_at TIMESTAMP DEFAULT NOW());
CREATE INDEX ON posts USING gin(to_tsvector('english', title || ' ' || content));
```

**API Endpoints:**

```
GET    /api/posts            (public, paginated, ?page=&search=&tag=)
GET    /api/posts/:id        (public, increment views)
POST   /api/posts            (auth – author/admin)
PUT    /api/posts/:id        (auth – owner atau admin)
DELETE /api/posts/:id        (auth – owner atau admin)

POST   /api/posts/:id/comments    (auth)
DELETE /api/comments/:id          (auth – owner atau admin)
```

**Deliverable:**
- [ ] Kode backend + frontend
- [ ] Screenshot: post list, post detail, editor, komentar bersarang
- [ ] Minimal 3 user (admin + 2 author) + 5 post + 10 komentar ter-seed
- [ ] Video demo 7 menit

---

### SOAL 5 — IOT-STYLE REALTIME DASHBOARD *(Level: Advanced)*

> Bangun dashboard yang mensimulasikan data sensor dari server (random generate, bukan hardware nyata). Dashboard menampilkan chart realtime, alert threshold, dan history data tersimpan di database.

**Fitur Wajib:**

| Fitur | Keterangan |
|-------|-----------|
| Auth | JWT login (admin) |
| Sensor Simulation | Server generate data random setiap 2 detik (suhu, kelembaban, tegangan, arus) |
| Realtime Chart | 4 grafik line Chart.js, 60 detik history, Socket.IO |
| Alert System | Threshold konfigurasi per sensor; alert muncul di dashboard + log |
| Database History | Semua readings tersimpan di PostgreSQL, GET history by range |
| Dashboard Summary | Nilai terkini, min/max hari ini, rata-rata per jam |

**Stack Rekomendasi:**

```
Backend:  Express.js + pg + socket.io
Database: PostgreSQL
Frontend: React + Chart.js + Tailwind + socket.io-client + Axios
Auth:     JWT + bcrypt
Deploy:   CloudFlare Tunnel
```

**Sensor Simulation (Server):**

```javascript
// Jalankan di server setiap 2 detik
setInterval(async () => {
  const reading = {
    temperature: +(Math.random() * 20 + 20).toFixed(2),  // 20-40°C
    humidity:    +(Math.random() * 40 + 40).toFixed(2),  // 40-80%
    voltage:     +(Math.random() * 5 + 220).toFixed(2),  // 220-225V
    current:     +(Math.random() * 2 + 1).toFixed(2),    // 1-3A
    timestamp:   new Date()
  };
  // Simpan ke DB
  await pool.query(`INSERT INTO readings (temp,humidity,voltage,current) VALUES ($1,$2,$3,$4)`,
    [reading.temperature, reading.humidity, reading.voltage, reading.current]);
  // Broadcast via Socket.IO
  io.emit('sensor_update', reading);
  // Check alert
  checkAlerts(reading);
}, 2000);
```

**Alert Logic:**

```javascript
const thresholds = { temperature: 35, humidity: 45, voltage: 223 };

function checkAlerts(reading) {
  if (reading.temperature > thresholds.temperature) {
    io.emit('alert', { type: 'temperature', value: reading.temperature, message: 'Suhu melebihi batas!' });
  }
}
```

**PostgreSQL Schema:**

```sql
CREATE TABLE readings (
  id          SERIAL PRIMARY KEY,
  temperature FLOAT, humidity FLOAT, voltage FLOAT, current FLOAT,
  recorded_at TIMESTAMP DEFAULT NOW()
);
CREATE TABLE alerts (
  id         SERIAL PRIMARY KEY,
  sensor     TEXT, value FLOAT, threshold FLOAT,
  message    TEXT,
  created_at TIMESTAMP DEFAULT NOW()
);
CREATE INDEX idx_readings_time ON readings(recorded_at DESC);
```

**API Endpoints:**

```
GET /api/readings              history (query: ?from=&to=&limit=)
GET /api/readings/summary      min/max/avg per sensor hari ini
GET /api/alerts                riwayat alert (paginated)
POST /api/thresholds           update threshold values
```

**Deliverable:**
- [ ] Kode backend + frontend
- [ ] Screenshot: 4 chart realtime berjalan, alert muncul di dashboard
- [ ] Screenshot: API history diakses via Postman
- [ ] Video demo 7 menit (tunjukkan chart, alert, history endpoint)
- [ ] URL CloudFlare Tunnel aktif

---

## C. DELIVERABLES (SEMUA SOAL)

| No | Item | Format |
|----|------|--------|
| 1 | Kode backend lengkap | Folder `backend/` di GitHub |
| 2 | Kode frontend lengkap | Folder `frontend/` di GitHub |
| 3 | README.md instruksi setup | Markdown di root folder |
| 4 | .env.example | File (tanpa credential asli) |
| 5 | Screenshot semua fitur utama | PNG, ≥5 screenshot |
| 6 | Video demo | MP4, sesuai durasi per soal |
| 7 | URL CloudFlare Tunnel | Aktif saat pengumpulan |

### Struktur Folder Submission

```
Kelompok_XX_Soal_Y/
├── backend/
│   ├── server.js
│   ├── routes/
│   ├── controllers/
│   ├── models/ (atau db.js)
│   ├── middleware/
│   ├── package.json
│   └── .env.example
├── frontend/
│   ├── src/
│   ├── package.json
│   └── vite.config.js
├── screenshots/
│   └── *.png
├── README.md
└── video_demo.mp4
```

---

## D. RUBRIK PENILAIAN PROJECT (100 POIN)

| No | Kriteria | Bobot | Deskripsi |
|----|---------|-------|----------|
| 1 | **Backend API** | 25 | Semua endpoint berfungsi, status code tepat, error handling |
| 2 | **Frontend UI** | 20 | Layout rapih, Tailwind, responsif, UX baik |
| 3 | **Database** | 20 | Schema tepat, query efisien, data persisten |
| 4 | **Real-Time** | 15 | Socket.IO berfungsi, update tanpa refresh |
| 5 | **Auth JWT** | 10 | Register/login, protected routes, role |
| 6 | **Deploy** | 5 | CloudFlare Tunnel accessible |
| 7 | **Kode & Dokumentasi** | 5 | README lengkap, kode terstruktur, .env.example |

**Bonus (+10 Poin Maks):**
- AI fitur (OpenAI API, summarize, suggest): +5
- OAuth Google login: +3
- Docker Compose setup: +2

---

## E. TIPS PENGERJAAN

### Setup Awal (Hari 1)

```bash
# Inisialisasi project
mkdir kelompok-xx-project && cd kelompok-xx-project
git init

# Backend
mkdir backend && cd backend
npm init -y
npm install express cors dotenv

# Frontend
cd ..
npm create vite@latest frontend -- --template react
cd frontend && npm install
```

### Git Workflow

```bash
git add .
git commit -m "feat: add user authentication"
git push origin main

# Branch per fitur
git checkout -b feature/socket-realtime
```

### Testing API Cepat

```bash
# Postman Collection atau curl
curl -X POST http://localhost:3000/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"test@test.com","password":"123456"}'
```

### Security Checklist

- [ ] Password di-hash dengan bcrypt
- [ ] JWT secret dari environment variable
- [ ] Input validation semua endpoint
- [ ] Parameterized query (bukan string concatenation)
- [ ] `.env` di `.gitignore`
- [ ] CORS dikonfigurasi (bukan `*` di production)

---

**Deadline: Minggu ke-4, Jumat 23:59 WIB**

Submit: GitHub repository link + URL CloudFlare Tunnel ke LMS
