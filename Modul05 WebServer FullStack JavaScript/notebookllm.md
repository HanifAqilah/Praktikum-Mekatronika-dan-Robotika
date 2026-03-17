# NotebookLM Prompts — Modul 05: Web Server & Full-Stack JavaScript

---

## PROMPT 1 — Slide 1-15 (Fondasi Web Dev + Node.js + Express + Percobaan 1-4)

Buat 15 slide presentasi akademik Modul 05: Web Server dan Full-Stack JavaScript menggunakan Node.js, Express.js, React, dan SQLite. Tiap slide informatif dan ringkas (200-250 karakter isi utama), sertakan perintah terminal, kode JavaScript, dan penjelasan konsep.

**Slide 1** — Judul Modul 05: Web Server Full-Stack JavaScript. Stack teknologi: Node.js 18+, Express.js, React 18, Vite, SQLite/PostgreSQL/MongoDB, Socket.IO, Tailwind CSS, CloudFlare Tunnel. Tujuan modul: mahasiswa mampu membangun aplikasi web full-stack dari backend hingga deploy ke internet.

**Slide 2** — Arsitektur Full-Stack Modern: Browser (React/HTML) ←HTTP/WebSocket→ Express.js Server ←Query→ Database. JSON mengalir di semua layer. Satu bahasa JavaScript dari frontend hingga backend. Event loop Node.js: non-blocking I/O, efficient concurrent requests.

**Slide 3** — Node.js Fundamentals: Runtime V8 Chrome untuk server-side JS. Event loop single-thread dengan non-blocking I/O. npm: 2 juta+ packages. Perintah utama: "node --version", "npm init -y", "npm install express", "npm run dev". CommonJS: require/module.exports vs ESM: import/export.

**Slide 4** — Express.js Setup & Middleware: npm install express nodemon. app.use(express.json()) parse body. Middleware chain: request → middleware1 → middleware2 → route handler → response. CORS dengan npm install cors, app.use(cors()). Logger: morgan. Security header: helmet.

**Slide 5** — Express Routing & REST Verbs: GET (read), POST (create 201), PUT (update), DELETE (204). Route param: req.params.id. Query string: req.query.sort. Router modular: express.Router(). app.use('/api/todos', todosRouter). Error middleware: 4 parameter (err, req, res, next).

**Slide 6** — HTTP Status Codes: 200 OK, 201 Created, 204 No Content, 400 Bad Request, 401 Unauthorized, 403 Forbidden, 404 Not Found, 409 Conflict, 500 Server Error. Konsisten gunakan kode tepat agar frontend mudah handle error. res.status(404).json({ error: 'Not found' }).

**Slide 7** — Percobaan 1 – Express Hello World: mkdir p1 && npm init -y && npm install express nodemon. Buat server.js: 3 endpoint (/, /info, /greet?name=). npm run dev. Test di browser: localhost:3000, localhost:3000/info, localhost:3000/greet?name=Budi. Amati auto-restart nodemon saat file disimpan.

**Slide 8** — REST API Design Pattern: Endpoint konsisten /api/resource. Response format: { data, count } atau { success, error }. Input validation dengan express-validator: body('title').notEmpty().withMessage('required'). Async error: const wrap = fn => (req,res,next) => Promise.resolve(fn(req,res,next)).catch(next).

**Slide 9** — Percobaan 2 – CRUD In-Memory: Array JavaScript sebagai temporary store. GET /api/todos, GET /:id, POST (201), PUT, DELETE (204). Filter by query: ?done=true. Test 8 endpoint di Postman. Catat status code tiap request. Data hilang saat server restart (in-memory).

**Slide 10** — SQLite dengan better-sqlite3: npm install better-sqlite3. new Database('app.db'). db.exec(CREATE TABLE). db.prepare(SQL) → .run() untuk write, .get() untuk satu row, .all() untuk banyak. Synchronous API. lastInsertRowid untuk mendapat ID setelah INSERT. File .db tersimpan di disk.

**Slide 11** — Percobaan 3 – SQLite Backend: CREATE TABLE todos (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, done INTEGER DEFAULT 0). stmtAll.all(), stmtOne.get(id), stmtInsert.run(title). Data persisten setelah restart. File todos.db terbentuk otomatis di folder project.

**Slide 12** — React + Vite Setup: npm create vite@latest app --template react. npm install. npm run dev = localhost:5173. JSX: HTML di dalam JavaScript. useState untuk state, useEffect untuk side effect. Functional component return JSX. import/export default.

**Slide 13** — React Hooks Penting: useState([]) untuk list, setList untuk update. useEffect(() => { fetch... }, []) run sekali saat mount. useRef untuk DOM access langsung. Custom hook: function useFetch(url) { useState + useEffect + return { data, loading, error } }. Dependency array mengontrol kapan effect jalan.

**Slide 14** — Axios untuk HTTP Client: npm install axios. axios.get/post/put/delete('/api/todos'). response.data berisi payload. Axios sudah parse JSON otomatis. Error handling: try-catch atau .catch(). Konfigurasi base URL: axios.create({ baseURL: 'http://localhost:3000/api' }). Lebih nyaman dari fetch native.

**Slide 15** — Percobaan 4 – React Todo Frontend: npm create vite + npm install axios. App.jsx: useState(todos), useEffect fetch awal. Form onSubmit → POST → fetchTodos(). Checkbox onChange → PUT. Button delete → DELETE. Backend (P3) harus running. npm install cors di backend. Screenshot 2 terminal + browser.

---

## PROMPT 2 — Slide 16-30 (Socket.IO + Auth + Database Lanjut + CloudFlare + Projects)

Lanjutkan presentasi Modul 05: Web Server Full-Stack JavaScript, slide 16-30. Fokus pada Socket.IO real-time, autentikasi JWT, database lanjut (MySQL, PostgreSQL, MongoDB), deployment CloudFlare Tunnel, dan overview 20 npm projects.

**Slide 16** — WebSocket vs HTTP Polling: HTTP polling: fetch tiap 2 detik, boros bandwidth, latensi tinggi. WebSocket: koneksi persistent bidirectional, latensi <50ms, efisien. Socket.IO: abstraksi WebSocket + fallback polling + rooms + namespaces. npm install socket.io (server) + socket.io-client (browser/React).

**Slide 17** — Socket.IO Server Setup: const http = require('http'); const { Server } = require('socket.io'). io.on('connection', socket => { socket.emit(), socket.on(), io.emit() broadcast semua, socket.to(room).emit() }). http.createServer(app) tidak express.listen agar Socket.IO dan Express share port yang sama.

**Slide 18** — Socket.IO Client (React): import { io } from 'socket.io-client'. const socket = io('http://localhost:3000'). useEffect: socket.on('event', handler); return () => socket.off('event'). Cleanup on unmount penting untuk mencegah memory leak. socket.emit('event', data) kirim ke server.

**Slide 19** — Percobaan 5 – Real-Time Counter: server.js: let counter = 0. io.on('connection'): socket.emit('counter_update', counter). socket.on('increment'): counter++; io.emit('counter_update', counter). public/index.html dengan socket.io.js CDN. Test: 2 tab browser → increment di satu tab → update keduanya.

**Slide 20** — JWT Authentication Flow: POST /auth/register → bcrypt.hash(password, 12) → simpan ke DB. POST /auth/login → bcrypt.compare() → jwt.sign({ id, email }, SECRET, { expiresIn:'7d' }) → kirim token. Client simpan token di localStorage. Setiap request: Authorization: Bearer <token>. Server: jwt.verify().

**Slide 21** — JWT Middleware Implementation: function authenticate(req,res,next) { const token = req.headers.authorization?.slice(7). try { req.user = jwt.verify(token, SECRET); next(); } catch { 401 } }. Protect route: app.get('/profile', authenticate, handler). npm install jsonwebtoken bcrypt. Jangan simpan password plain text!

**Slide 22** — MySQL dengan mysql2: npm install mysql2. mysql.createPool({ host, user, password, database, connectionLimit:10 }). await pool.query('SELECT * FROM notes'). Parameterized: pool.execute('INSERT INTO notes VALUES (?,?,?)', [title, content, tags]). createPool lebih efisien dari createConnection karena reuse connection.

**Slide 23** — PostgreSQL dengan pg: npm install pg. new Pool({ connectionString: process.env.DATABASE_URL }). pool.query('SELECT...', [param]). Parameter $1, $2 (bukan ? seperti MySQL). JSON support native. JSONB column type. Advanced: aggregation, window functions, full-text search. Cocok production. Banyak dipakai Proyek 7,8,11,14,15.

**Slide 24** — MongoDB dengan Mongoose: npm install mongoose. mongoose.connect(MONGO_URI). Schema → Model → CRUD: Post.find(), Post.findById(id), Post.create(data), Post.findByIdAndUpdate(), Post.findByIdAndDelete(). populate() untuk join. $text search. timestamps: true otomatis createdAt + updatedAt. Banyak dipakai Proyek 9,10,13.

**Slide 25** — Prisma ORM (Multi-DB): npm install prisma @prisma/client. npx prisma init. schema.prisma: model Todo { id Int @id @default(autoincrement()) title String done Boolean @default(false) }. npx prisma migrate dev. await prisma.todo.findMany() type-safe. Support SQLite, MySQL, PostgreSQL, MongoDB.

**Slide 26** — CloudFlare Tunnel – Konsep: Tunnel agent (cloudflared) membuat outbound connection ke CloudFlare edge. Traffic publik diselesaikan di edge, diteruskan ke localhost. Tidak perlu IP publik, port forwarding, atau firewall rules. Quick tunnel (URL random) vs Named tunnel (domain tetap). Gratis untuk development.

**Slide 27** — CloudFlare Tunnel – Instalasi dan Penggunaan: Windows: winget install Cloudflare.cloudflared. macOS: brew install cloudflared. Quick tunnel: cloudflared tunnel --url http://localhost:3000. Output URL https://random.trycloudflare.com. Test dari HP (data seluler). Named tunnel: cloudflared tunnel login → create → route dns → run.

**Slide 28** — Percobaan 6 – Deploy via Tunnel: Backend percobaan3 running di :3000. Terminal baru: cloudflared tunnel --url http://localhost:3000. Copy URL tunnel. Buka di HP (data seluler bukan WiFi). Test GET /api/todos dari URL tunnel. Screenshot URL tunnel di address bar smartphone. Akses dari luar jaringan lokal berhasil.

**Slide 29** — Environment Variables & .env: npm install dotenv. require('dotenv').config() di baris pertama. File .env: PORT=3000, DATABASE_URL=postgresql://..., JWT_SECRET=..., OPENAI_API_KEY=... File .gitignore wajib include .env. process.env.PORT || 3000. Jangan hardcode credential di kode!

**Slide 30** — Percobaan 7 – Full-Stack Integration: Gabung Express + SQLite + React + Socket.IO. server.js: db + io. Saat POST todo: io.emit('todo_added', t). Client: socket.on('todo_added', t => setTodos(p=>[t,...p])). Test 2 tab: tambah todo di satu tab → muncul real-time di tab lain. CloudFlare tunnel expose ke internet.

---

## PROMPT 3 — Slide 31-45 (20 Projects + Security + Project + Tugas Video)

Lanjutkan presentasi Modul 05: Web Server Full-Stack JavaScript, slide 31-45. Slide 31-37 overview 20 npm projects dengan jalur belajar progresif, slide 38-42 project akhir full-stack todo/dashboard, slide 43-45 panduan tugas video demonstrasi.

**Slide 31** — 20 npm Projects Overview: Level Easy (1-4): Frontend only – Portfolio, Todo localStorage, Weather API, Movie Search. Level Sedang (5-12): Backend+DB – Expense SQLite, Notes MySQL, IoT Dashboard PostgreSQL, Chat Socket.IO, Blog MongoDB, Kanban, Social Feed GraphQL, Quiz. Level Advanced (13-20): AI Chatbot, ML, E-Commerce, Smart Home, Microservices, Analytics, Collaborative Editor, MLOps.

**Slide 32** — Projects Level Easy (1-4): P1 Portfolio: React + Vite + Tailwind, deploy static. P2 Todo: React + useState + localStorage, no backend needed. P3 Weather: fetch OpenWeather API, async/await, error handling. P4 Movie: TMDB API, search + filter + pagination. Fokus: React hooks, fetch, CSS, browser API.

**Slide 33** — Projects Level Sedang (5-8): P5 Expense Tracker: Express + SQLite, CRUD category, chart pie. P6 Markdown Notes: Express + MySQL, markdown-it, code highlighting. P7 IoT Dashboard: Express + Socket.IO + PostgreSQL, Chart.js realtime, MQTT.js. P8 Chat App: Socket.IO rooms, PostgreSQL + Redis, online users.

**Slide 34** — Projects Level Sedang (9-12): P9 Blog CMS: Express + MongoDB + Mongoose, rich text editor, tag search. P10 Kanban Board: MongoDB, drag-drop columns, user assignment. P11 Social Feed: PostgreSQL + GraphQL Apollo, timeline algorithm, likes/comments. P12 Quiz Platform: MySQL, timer, leaderboard, session management.

**Slide 35** — Projects Level Advanced (13-16): P13 AI Chatbot: LangChain.js + OpenAI API + MongoDB + Redis, streaming responses, conversation memory. P14 Image Recognition: TensorFlow.js + PostgreSQL, upload + inference. P15 E-Commerce: PostgreSQL + Redis + Stripe, cart, inventory, payment. P16 Smart Home: MongoDB + Cassandra + Redis, device state, rules engine.

**Slide 36** — Projects Level Advanced (17-20): P17 Microservices: NestJS + RabbitMQ + Docker, user/product/order/payment/notification services. P18 Analytics: PostgreSQL + InfluxDB, realtime metrics, time-series aggregation. P19 Collaborative Editor: WebRTC + PostgreSQL, CRDT, multi-user sync. P20 ML Platform: PostgreSQL + MLflow, experiment tracking, model versioning.

**Slide 37** — Jalur Belajar dan Database Recommendation: Rekomendasi: P2→P5→P7→P9→P13→P17. SQLite: P5 (zero config, local). MySQL: P6,P12 (tradisional, shared hosting). PostgreSQL: P7,P8,P11,P14,P15,P18 (production, JSON, advanced). MongoDB: P9,P10,P13 (flexible schema). Redis: P8,P13,P15 (cache, session pub-sub).

**Slide 38** — Project Brief: Full-Stack Dashboard App: Bangun aplikasi full-stack web dengan minimal: REST API CRUD 2+ resource, database persistent, frontend React dengan Tailwind, real-time update Socket.IO untuk 1 fitur, autentikasi JWT (register+login), dan accessible via CloudFlare Tunnel. Pilih salah satu tema dari 5 option Soal Project.

**Slide 39** — Soal 1 & 2 Project: Soal 1 (Easy): Personal Finance Tracker – CRUD transaksi, filter kategori, chart pie Chart.js, SQLite, React. Soal 2 (Medium): Note-Taking dengan Markdown – CRUD notes, markdown preview, tag filter, MySQL, React + react-markdown. Deliverable: kode, screenshot UI, video demo 3 menit.

**Slide 40** — Soal 3 & 4 Project: Soal 3 (Medium): Real-Time Task Board – Kanban columns (Todo/In Progress/Done), drag-drop, Socket.IO sync antar user, MongoDB. Soal 4 (Hard): Blog Platform dengan Auth – JWT login/register, post CRUD, comment, tag search full-text, PostgreSQL, pagination, React. Deliverable: kode + dokumentasi API + video.

**Slide 41** — Soal 5 Project + Bonus: Soal 5 (Advanced): IoT-Style Dashboard – Socket.IO mock sensor data (random generate server-side), Chart.js 4 grafik realtime, PostgreSQL store readings, alert threshold, REST API data history, CloudFlare deploy. Bonus: AI fitur (OpenAI API, summarize, suggest), OAuth Google login, Docker compose.

**Slide 42** — Rubrik Penilaian Project (100 poin): Fungsionalitas backend API 25%, Frontend React UI 20%, Database integration 20%, Real-time Socket.IO 15%, Autentikasi JWT 10%, Deploy CloudFlare 5%, Code quality + dokumentasi 5%. Bonus: AI fitur +5, OAuth +3, Docker +2.

**Slide 43** — Panduan Tugas Video: Durasi 10-15 menit. Bagian wajib: (1) Penjelasan arsitektur full-stack 2 menit – gambar diagram, jelaskan alur data. (2) Demo percobaan 1-7 total 5 menit – screen recording tiap percobaan, terminal + browser terlihat. (3) Demo project soal terpilih 5 menit – semua fitur berjalan, testing edge case.

**Slide 44** — Rubrik Video & Teknis: Resolusi minimal 1080p 30fps. Format MP4. Narasi jelas Bahasa Indonesia. Diagram arsitektur harus ada. Kode terminal + browser terlihat. Test API di Postman direkam. CloudFlare tunnel URL diakses live. Webcam opsional. Skor: Materi 25%, Demo percobaan 35%, Demo project 30%, Kualitas produksi 10%.

**Slide 45** — Checklist Final Submission: Kode semua percobaan (P1-P7) dalam folder terpisah. Kode project final (backend/ + client/). Screenshot semua percobaan (min 1 per percobaan). Video demo MP4 10-15 menit. README.md instruksi cara menjalankan project. File .env.example (tanpa credential asli). Push ke GitHub, submit link ke LMS.
