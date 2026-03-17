# MATERI MODUL 05: WEB SERVER & FULL-STACK JAVASCRIPT

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – IoT WebServer MQTT ESP32  
**Estimasi Waktu Belajar:** 8–10 Jam  

---

## DAFTAR ISI

1. [Pendahuluan Web Development](#1-pendahuluan-web-development)
2. [Node.js dan npm](#2-nodejs-dan-npm)
3. [Express.js – Backend Framework](#3-expressjs--backend-framework)
4. [React – Frontend Library](#4-react--frontend-library)
5. [REST API Design](#5-rest-api-design)
6. [Database Integration](#6-database-integration)
7. [Real-Time dengan Socket.IO](#7-real-time-dengan-socketio)
8. [Autentikasi JWT](#8-autentikasi-jwt)
9. [CloudFlare Tunnel Deployment](#9-cloudflare-tunnel-deployment)
10. [Overview 20 npm Projects](#10-overview-20-npm-projects)

---

## 1. PENDAHULUAN WEB DEVELOPMENT

### 1.1 Arsitektur Full-Stack Modern

```
┌─────────────────────────────────────────────┐
│               CLIENT (Browser)               │
│  React / HTML / CSS / JavaScript             │
│  Chart.js / Tailwind CSS / Axios             │
└───────────────────┬─────────────────────────┘
                    │ HTTP / WebSocket
┌───────────────────▼─────────────────────────┐
│             BACKEND (Server)                 │
│  Node.js + Express.js                        │
│  REST API / Socket.IO / JWT Auth             │
└───────────────────┬─────────────────────────┘
                    │ Query / ORM
┌───────────────────▼─────────────────────────┐
│              DATABASE LAYER                  │
│  SQLite │ MySQL │ PostgreSQL │ MongoDB        │
└─────────────────────────────────────────────┘
```

### 1.2 Kenapa JavaScript Full-Stack?

| Keuntungan | Keterangan |
|-----------|-----------|
| Satu bahasa | Frontend dan Backend sama-sama JavaScript |
| Ekosistem besar | npm: 2 juta+ package tersedia |
| Non-blocking I/O | Node.js event loop sangat efisien |
| JSON native | Data transfer tanpa konversi format |
| Komunitas aktif | Update cepat, dokumentasi lengkap |

### 1.3 Tech Stack Modul Ini

| Layer | Teknologi |
|-------|----------|
| Frontend | React 18, Tailwind CSS, Chart.js, Vite |
| Backend | Node.js 18+, Express.js |
| Database | SQLite / MySQL / PostgreSQL / MongoDB |
| Real-time | Socket.IO |
| Auth | JWT (jsonwebtoken), bcrypt |
| Deploy | CloudFlare Tunnel |
| Dev Tools | Nodemon, ESLint, Vitest |

---

## 2. NODE.JS DAN NPM

### 2.1 Apa itu Node.js?

**Node.js** adalah runtime JavaScript di server side, menggunakan V8 engine dari Google Chrome. Node dapat menjalankan JS di luar browser dengan akses ke file system, network, dan OS.

### 2.2 Event Loop Node.js

```
┌─────────────────────────────┐
│        Event Queue           │
│  callback1 → callback2 → ... │
└──────────────┬──────────────┘
               │
   ┌───────────▼────────────┐
   │   Event Loop           │
   │   (Single Thread)      │
   └───────────┬────────────┘
               │
  ┌────────────▼────────────┐
  │ Non-blocking I/O        │
  │ (OS handles: file, net) │
  └─────────────────────────┘
```

**Kunci Node.js:** I/O non-blocking – saat menunggu database/file, thread tidak idle tetapi melayani request lain.

### 2.3 npm – Package Manager

```bash
# Initialize project
npm init -y

# Install package
npm install express
npm install -D nodemon  # dev dependency

# Run scripts
npm run dev
npm run build
npm test

# package.json scripts
{
  "scripts": {
    "dev": "nodemon server.js",
    "start": "node server.js",
    "build": "vite build",
    "test": "vitest run"
  }
}
```

### 2.4 Modul System (CommonJS vs ESM)

```javascript
// CommonJS (require) – default Node.js
const express = require('express');
const { Router } = require('express');
module.exports = router;

// ES Modules (import) – modern, perlu "type": "module" di package.json
import express from 'express';
import { Router } from 'express';
export default router;
```

### 2.5 Built-in Node.js Modules

```javascript
const fs   = require('fs');        // file system
const path = require('path');      // path manipulation
const http = require('http');      // HTTP server
const os   = require('os');        // operating system info
const crypto = require('crypto');  // hashing & encryption

// Async/await dengan fs
const { readFile, writeFile } = require('fs/promises');

async function readData() {
  const data = await readFile('data.json', 'utf8');
  return JSON.parse(data);
}
```

---

## 3. EXPRESS.JS – BACKEND FRAMEWORK

### 3.1 Hello World

```javascript
const express = require('express');
const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());  // parse JSON body

app.get('/', (req, res) => {
  res.json({ message: 'Hello World!', time: new Date() });
});

app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
```

### 3.2 Middleware

```
Request → [Middleware 1] → [Middleware 2] → [Route Handler] → Response
```

```javascript
// Built-in middleware
app.use(express.json());                        // parse JSON
app.use(express.urlencoded({ extended: true })); // parse form data
app.use(express.static('public'));              // serve static files

// Third-party middleware
const cors    = require('cors');
const morgan  = require('morgan');
const helmet  = require('helmet');

app.use(cors());           // enable Cross-Origin requests
app.use(morgan('dev'));    // HTTP request logger
app.use(helmet());         // security HTTP headers

// Custom middleware
app.use((req, res, next) => {
  req.timestamp = Date.now();
  console.log(`[${req.method}] ${req.path}`);
  next();
});
```

### 3.3 Routing

```javascript
const router = express.Router();

// CRUD endpoints
router.get('/',        getAll);
router.get('/:id',     getOne);
router.post('/',       create);
router.put('/:id',     update);
router.delete('/:id',  remove);

// Route parameter & query
app.get('/users/:id', (req, res) => {
  const { id } = req.params;  // /users/42 → id = "42"
  const { sort } = req.query; // /users/42?sort=name → sort = "name"
  res.json({ id, sort });
});

// Route grouping
app.use('/api/todos',  require('./routes/todos'));
app.use('/api/users',  require('./routes/users'));
app.use('/api/auth',   require('./routes/auth'));
```

### 3.4 Error Handling

```javascript
// Try-catch async helper
const asyncHandler = fn => (req, res, next) =>
  Promise.resolve(fn(req, res, next)).catch(next);

router.get('/:id', asyncHandler(async (req, res) => {
  const item = await db.find(req.params.id);
  if (!item) return res.status(404).json({ error: 'Not found' });
  res.json(item);
}));

// Global error middleware (4 params!)
app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(err.status || 500).json({
    error: err.message || 'Internal Server Error'
  });
});
```

### 3.5 Project Structure (MVC)

```
backend/
├── server.js          # Entry point, init Express + DB
├── routes/
│   ├── todos.js       # Route definitions
│   └── users.js
├── controllers/
│   ├── todoController.js  # Business logic
│   └── userController.js
├── models/
│   ├── Todo.js        # DB schema / model
│   └── User.js
├── middleware/
│   ├── auth.js        # JWT verification
│   └── validate.js    # Input validation
└── config/
    └── db.js          # Database connection
```

---

## 4. REACT – FRONTEND LIBRARY

### 4.1 Setup dengan Vite

```bash
npm create vite@latest my-app -- --template react
cd my-app
npm install
npm run dev    # http://localhost:5173
```

### 4.2 Component dan JSX

```jsx
// Functional Component
function TodoItem({ todo, onToggle, onDelete }) {
  return (
    <div className={`todo ${todo.done ? 'done' : ''}`}>
      <input
        type="checkbox"
        checked={todo.done}
        onChange={() => onToggle(todo.id)}
      />
      <span>{todo.text}</span>
      <button onClick={() => onDelete(todo.id)}>✕</button>
    </div>
  );
}

export default TodoItem;
```

### 4.3 Hooks Penting

```jsx
import { useState, useEffect, useContext, useRef } from 'react';

function App() {
  // State
  const [todos, setTodos] = useState([]);
  const [loading, setLoading] = useState(true);

  // Side effect – fetch data on mount
  useEffect(() => {
    fetch('/api/todos')
      .then(r => r.json())
      .then(data => {
        setTodos(data);
        setLoading(false);
      });
  }, []); // empty deps = run once on mount

  // Ref – DOM access without re-render
  const inputRef = useRef(null);

  if (loading) return <p>Loading...</p>;

  return (
    <div>
      {todos.map(todo => (
        <TodoItem key={todo.id} todo={todo} />
      ))}
    </div>
  );
}
```

### 4.4 Custom Hook

```jsx
// useFetch.js – reusable fetch logic
function useFetch(url) {
  const [data, setData]     = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError]   = useState(null);

  useEffect(() => {
    setLoading(true);
    fetch(url)
      .then(r => { if (!r.ok) throw new Error(r.statusText); return r.json(); })
      .then(setData)
      .catch(setError)
      .finally(() => setLoading(false));
  }, [url]);

  return { data, loading, error };
}

// Usage
const { data: todos, loading } = useFetch('/api/todos');
```

### 4.5 State Management

```jsx
// Context API (medium-scale apps)
const TodoContext = createContext();

function TodoProvider({ children }) {
  const [todos, setTodos] = useState([]);

  const addTodo = (text) =>
    setTodos(prev => [...prev, { id: Date.now(), text, done: false }]);

  const toggleTodo = (id) =>
    setTodos(prev => prev.map(t => t.id === id ? {...t, done: !t.done} : t));

  return (
    <TodoContext.Provider value={{ todos, addTodo, toggleTodo }}>
      {children}
    </TodoContext.Provider>
  );
}

const useTodos = () => useContext(TodoContext);
```

---

## 5. REST API DESIGN

### 5.1 HTTP Methods & Semantics

| Method | Endpoint | Aksi | Body |
|--------|---------|------|------|
| GET | /todos | Ambil semua | - |
| GET | /todos/:id | Ambil satu | - |
| POST | /todos | Buat baru | JSON |
| PUT | /todos/:id | Update penuh | JSON |
| PATCH | /todos/:id | Update sebagian | JSON |
| DELETE | /todos/:id | Hapus | - |

### 5.2 HTTP Status Codes

| Code | Arti | Kapan Digunakan |
|------|------|----------------|
| 200 | OK | GET/PUT/PATCH berhasil |
| 201 | Created | POST berhasil |
| 204 | No Content | DELETE berhasil |
| 400 | Bad Request | Validasi gagal |
| 401 | Unauthorized | Belum login |
| 403 | Forbidden | Tidak punya akses |
| 404 | Not Found | Data tidak ada |
| 409 | Conflict | Duplikat data |
| 500 | Server Error | Error backend |

### 5.3 Response Format Konsisten

```javascript
// Success
res.status(200).json({
  success: true,
  data: todos,
  count: todos.length
});

// Error
res.status(400).json({
  success: false,
  error: 'Title is required',
  code: 'VALIDATION_ERROR'
});

// Paginated
res.json({
  data: todos,
  pagination: { page: 1, limit: 10, total: 100, pages: 10 }
});
```

### 5.4 Input Validation

```javascript
// express-validator
const { body, validationResult } = require('express-validator');

const validateTodo = [
  body('title')
    .trim()
    .notEmpty().withMessage('Title required')
    .isLength({ max: 200 }).withMessage('Max 200 chars'),
  body('priority')
    .optional()
    .isIn(['low', 'medium', 'high']),

  (req, res, next) => {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      return res.status(400).json({ errors: errors.array() });
    }
    next();
  }
];

router.post('/', validateTodo, createTodo);
```

---

## 6. DATABASE INTEGRATION

### 6.1 SQLite (Project 5 – Expense Tracker)

Ideal untuk: prototype, small app, embedded, zero config.

```javascript
const Database = require('better-sqlite3');
const db = new Database('app.db');

// Create table
db.exec(`
  CREATE TABLE IF NOT EXISTS expenses (
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    amount  REAL    NOT NULL,
    category TEXT   NOT NULL,
    note    TEXT,
    date    TEXT    DEFAULT (datetime('now'))
  )
`);

// CRUD (synchronous API)
const insert = db.prepare(
  'INSERT INTO expenses (amount, category, note) VALUES (?, ?, ?)'
);
const all    = db.prepare('SELECT * FROM expenses ORDER BY date DESC');
const byId   = db.prepare('SELECT * FROM expenses WHERE id = ?');
const del    = db.prepare('DELETE FROM expenses WHERE id = ?');

// Usage in route
app.post('/expenses', (req, res) => {
  const { amount, category, note } = req.body;
  const result = insert.run(amount, category, note);
  res.status(201).json({ id: result.lastInsertRowid });
});
```

### 6.2 MySQL (Project 6 – Markdown Notes)

Ideal untuk: traditional web app, familiarity, shared hosting.

```javascript
const mysql = require('mysql2/promise');

const pool = mysql.createPool({
  host: 'localhost', port: 3306,
  user: 'root', password: 'password',
  database: 'notes_db',
  waitForConnections: true, connectionLimit: 10
});

// Query
async function getNotes() {
  const [rows] = await pool.query(
    'SELECT * FROM notes ORDER BY updated_at DESC'
  );
  return rows;
}

async function createNote({ title, content, tags }) {
  const [result] = await pool.execute(
    'INSERT INTO notes (title, content, tags) VALUES (?, ?, ?)',
    [title, content, JSON.stringify(tags)]
  );
  return result.insertId;
}
```

### 6.3 PostgreSQL (Project 7 – IoT Dashboard)

Ideal untuk: production, advanced queries, JSON support.

```javascript
const { Pool } = require('pg');

const pool = new Pool({
  connectionString: process.env.DATABASE_URL ||
    'postgresql://user:password@localhost:5432/iot_db'
});

// With parameterized query (SQL injection safe)
async function getSensorReadings(sensorId, limit = 60) {
  const { rows } = await pool.query(
    `SELECT temperature, humidity, timestamp
     FROM readings
     WHERE sensor_id = $1
     ORDER BY timestamp DESC LIMIT $2`,
    [sensorId, limit]
  );
  return rows;
}

// Aggregation
async function getDailyAverage(sensorId) {
  const { rows } = await pool.query(`
    SELECT
      DATE(timestamp) as day,
      AVG(temperature)::numeric(5,2) as avg_temp,
      AVG(humidity)::numeric(5,2)    as avg_humidity
    FROM readings
    WHERE sensor_id = $1
    GROUP BY DATE(timestamp)
    ORDER BY day DESC LIMIT 30
  `, [sensorId]);
  return rows;
}
```

### 6.4 MongoDB (Project 9 – Blog CMS)

Ideal untuk: flexible schema, document-centric, rapid iteration.

```javascript
const mongoose = require('mongoose');
mongoose.connect(process.env.MONGO_URI || 'mongodb://localhost:27017/blog');

// Schema + Model
const postSchema = new mongoose.Schema({
  title:     { type: String, required: true, trim: true },
  content:   { type: String, required: true },
  author:    { type: mongoose.Schema.Types.ObjectId, ref: 'User' },
  tags:      [String],
  published: { type: Boolean, default: false },
  views:     { type: Number, default: 0 }
}, { timestamps: true });

postSchema.index({ title: 'text', content: 'text' }); // full-text search
const Post = mongoose.model('Post', postSchema);

// CRUD
const posts     = await Post.find({ published: true }).populate('author', 'name');
const post      = await Post.findById(id);
const created   = await Post.create({ title, content, author: userId });
await Post.findByIdAndUpdate(id, { $set: { title } });
await Post.findByIdAndDelete(id);

// Text search
const results = await Post.find({ $text: { $search: 'node.js express' } });
```

### 6.5 Prisma ORM (Multi-database)

```javascript
// schema.prisma
model Todo {
  id        Int      @id @default(autoincrement())
  title     String
  done      Boolean  @default(false)
  createdAt DateTime @default(now())
}

// Usage (type-safe!)
const { PrismaClient } = require('@prisma/client');
const prisma = new PrismaClient();

const todos  = await prisma.todo.findMany({ orderBy: { createdAt: 'desc' } });
const todo   = await prisma.todo.create({ data: { title: 'Buy milk' } });
await prisma.todo.update({ where: { id: 1 }, data: { done: true } });
await prisma.todo.delete({ where: { id: 1 } });
```

---

## 7. REAL-TIME DENGAN SOCKET.IO

### 7.1 Konsep WebSocket vs HTTP Polling

| Aspek | HTTP Polling | WebSocket (Socket.IO) |
|-------|-------------|----------------------|
| Koneksi | Request tiap N detik | Persistent connection |
| Latensi | Tinggi (delay polling) | Sangat rendah (<50ms) |
| Bandwidth | Boros (header HTTP ulang) | Efisien (frame kecil) |
| Use case | Data tidak terlalu realtime | Chat, live dashboard, game |

### 7.2 Setup Server + Client

```javascript
// server.js
const http    = require('http');
const express = require('express');
const { Server } = require('socket.io');

const app    = express();
const server = http.createServer(app);
const io     = new Server(server, {
  cors: { origin: '*' }
});

io.on('connection', (socket) => {
  console.log('Client connected:', socket.id);

  socket.on('send_message', (data) => {
    io.emit('receive_message', data);  // broadcast to ALL
  });

  socket.on('join_room', (room) => {
    socket.join(room);
    socket.to(room).emit('user_joined', socket.id);
  });

  socket.on('disconnect', () => {
    console.log('Disconnected:', socket.id);
  });
});

server.listen(3000);
```

```jsx
// React client
import { io } from 'socket.io-client';

const socket = io('http://localhost:3000');

function Chat() {
  const [messages, setMessages] = useState([]);

  useEffect(() => {
    socket.on('receive_message', (msg) => {
      setMessages(prev => [...prev, msg]);
    });
    return () => socket.off('receive_message');
  }, []);

  const send = (text) => socket.emit('send_message', { text, time: Date.now() });

  return ( /* ... */ );
}
```

### 7.3 Real-Time Dashboard Pattern

```javascript
// Server: push data to clients every 2 seconds
setInterval(() => {
  const reading = generateSensorReading();
  io.emit('sensor_update', reading);
}, 2000);

// Client: update chart on receive
socket.on('sensor_update', (data) => {
  chart.data.labels.push(new Date().toLocaleTimeString());
  chart.data.datasets[0].data.push(data.temperature);
  if (chart.data.labels.length > 30) {
    chart.data.labels.shift();
    chart.data.datasets[0].data.shift();
  }
  chart.update('none'); // no animation = smooth
});
```

---

## 8. AUTENTIKASI JWT

### 8.1 Alur JWT

```
1. User POST /auth/login { email, password }
2. Server verifikasi → buat JWT token
3. Server kirim token ke client
4. Client simpan token (localStorage / cookie)
5. Client sertakan token di setiap request: Authorization: Bearer <token>
6. Server verifikasi token sebelum proses request
```

### 8.2 Implementasi

```javascript
const jwt    = require('jsonwebtoken');
const bcrypt = require('bcrypt');

const SECRET = process.env.JWT_SECRET || 'change_this_in_production';

// Register
app.post('/auth/register', async (req, res) => {
  const { name, email, password } = req.body;
  const hashed = await bcrypt.hash(password, 12);
  const user = await User.create({ name, email, password: hashed });
  res.status(201).json({ message: 'Registered' });
});

// Login
app.post('/auth/login', async (req, res) => {
  const { email, password } = req.body;
  const user = await User.findOne({ email });
  if (!user || !await bcrypt.compare(password, user.password)) {
    return res.status(401).json({ error: 'Invalid credentials' });
  }
  const token = jwt.sign({ id: user.id, email }, SECRET, { expiresIn: '7d' });
  res.json({ token });
});

// Middleware: protect routes
function authenticate(req, res, next) {
  const auth = req.headers.authorization;
  if (!auth?.startsWith('Bearer ')) return res.status(401).end();
  try {
    req.user = jwt.verify(auth.slice(7), SECRET);
    next();
  } catch {
    res.status(401).json({ error: 'Invalid token' });
  }
}

// Protected route
app.get('/profile', authenticate, (req, res) => {
  res.json({ user: req.user });
});
```

---

## 9. CLOUDFLARE TUNNEL DEPLOYMENT

### 9.1 Apa itu CloudFlare Tunnel?

CloudFlare Tunnel (sebelumnya Argo Tunnel) memungkinkan mengekspos server **localhost ke internet publik** tanpa:
- Public IP address
- Port forwarding di router
- Firewall rule changes

```
localhost:3000  ──cloudflared──>  CloudFlare Edge  ──>  myapp.example.com
```

### 9.2 Install cloudflared

```bash
# Windows (PowerShell – jalankan sebagai Admin)
winget install Cloudflare.cloudflared

# macOS
brew install cloudflared

# Linux (Ubuntu/Debian)
wget https://github.com/cloudflare/cloudflared/releases/latest/download/cloudflared-linux-amd64.deb
sudo dpkg -i cloudflared-linux-amd64.deb
```

### 9.3 Quick Tunnel (tanpa domain)

```bash
# Expose port 3000 ke internet (URL temporary)
cloudflared tunnel --url http://localhost:3000

# Output:
# +--------------------------------------------------------------------------------------------+
# |  Your quick Tunnel has been created! Visit it at (it may take some time to be reachable): |
# |  https://random-string.trycloudflare.com                                                  |
# +--------------------------------------------------------------------------------------------+
```

### 9.4 Permanent Tunnel (dengan domain)

```bash
# 1. Login ke CloudFlare
cloudflared tunnel login

# 2. Buat tunnel
cloudflared tunnel create my-project

# 3. Buat config file: ~/.cloudflared/config.yml
tunnel: <TUNNEL_ID>
credentials-file: /home/user/.cloudflared/<TUNNEL_ID>.json

ingress:
  - hostname: myproject.example.com
    service: http://localhost:3000
  - service: http_status:404

# 4. Route DNS
cloudflared tunnel route dns my-project myproject.example.com

# 5. Jalankan tunnel
cloudflared tunnel run my-project
```

### 9.5 Environment Variables di Production

```bash
# .env file (JANGAN di-commit ke git!)
PORT=3000
NODE_ENV=production
DATABASE_URL=postgresql://user:pass@localhost:5432/mydb
JWT_SECRET=super_secret_key_change_this
OPENAI_API_KEY=sk-...

# Load di Node.js
require('dotenv').config();
const PORT = process.env.PORT || 3000;

# .gitignore
.env
node_modules/
dist/
```

---

## 10. OVERVIEW 20 NPM PROJECTS

### Level Easy – Frontend Only (Proyek 1-4)

| No | Proyek | Stack | Fokus Pembelajaran |
|----|--------|-------|-------------------|
| 1 | Portfolio Website | React + Vite + Tailwind | Component, props, routing |
| 2 | Todo List App | React + localStorage | State management, hooks |
| 3 | Weather App | React + OpenWeather API | API fetch, async/await |
| 4 | Movie Database | React + TMDB API | Search, filter, pagination |

### Level Sedang – Menambah Backend & Database (Proyek 5-12)

| No | Proyek | Backend | Database | Fokus |
|----|--------|---------|----------|-------|
| 5 | Expense Tracker | Express | SQLite | CRUD, REST API dasar |
| 6 | Markdown Notes | Express | MySQL | File editor, rich text |
| 7 | IoT Dashboard | Express + Socket.IO | PostgreSQL | Real-time chart, time-series |
| 8 | Real-Time Chat | Express + Socket.IO | PostgreSQL + Redis | WebSocket, room management |
| 9 | Blog CMS | Express | MongoDB | Full CRUD, pagination |
| 10 | Kanban Board | Express | MongoDB | Drag-drop, board state |
| 11 | Social Media Feed | Express + GraphQL | PostgreSQL | GraphQL API, feed algorithm |
| 12 | Quiz Platform | Express | MySQL | Session, scoring, timer |

### Level Advanced – Arsitektur Kompleks (Proyek 13-20)

| No | Proyek | Stack | Fokus |
|----|--------|-------|-------|
| 13 | AI Chatbot | Express + LangChain | LLM API, streaming, context memory |
| 14 | Image Recognition | Express + TensorFlow.js | ML inference di server |
| 15 | E-Commerce | Express + Stripe | Payment, cart, inventory |
| 16 | Smart Home System | Express + Socket.IO | Multi-device state, rules engine |
| 17 | Microservices | NestJS + RabbitMQ | Service separation, message queue |
| 18 | Analytics Dashboard | Express + InfluxDB | Time-series, aggregation |
| 19 | Collaborative Editor | Express + WebRTC | CRDT, real-time sync |
| 20 | ML Training Platform | Express + MLflow | Model lifecycle, experiment tracking |

### Jalur Belajar Rekomendasi

```
Mulai →  Proyek 2 (Todo)
       → Proyek 5 (Expense + Backend)
       → Proyek 7 (IoT Dashboard + Socket.IO)
       → Proyek 9 (Blog + MongoDB)
       → Proyek 13 (AI Chatbot)
       → Proyek 17 (Microservices)  ← Level Expert
```

### Stack Database per Proyek

```
SQLite  → Proyek 5, 12  (simple, no server)
MySQL   → Proyek 6, 12  (traditional relational)
PostgreSQL → Proyek 7, 8, 11, 14, 15, 18, 19, 20  (production-grade)
MongoDB → Proyek 9, 10, 13  (document store, flexible)
Redis   → Proyek 8, 13, 15  (cache, session, pub-sub)
InfluxDB → Proyek 18  (time-series)
```

### Tools & Libraries Penting

```bash
# HTTP Client
axios                 # promise-based HTTP client
node-fetch            # browser-like fetch for Node

# Validation
joi                   # schema validation
express-validator     # Express specific validator
zod                   # TypeScript-first schema

# Auth
jsonwebtoken          # JWT sign & verify
bcrypt / bcryptjs     # password hashing
passport              # auth middleware umbrella

# Database ORM / Query Builder
prisma                # modern ORM (multi-db)
sequelize             # classic ORM (SQL)
mongoose              # MongoDB ODM
knex                  # SQL query builder

# Real-time
socket.io             # WebSocket library
ws                    # minimal WebSocket

# File handling
multer                # multipart/file upload
sharp                 # image processing

# Testing
jest / vitest         # unit testing
supertest             # HTTP integration tests
```

---

## REFERENSI

| Topik | Link |
|-------|------|
| Node.js Docs | https://nodejs.org/docs |
| Express Guide | https://expressjs.com/guide |
| React Docs | https://react.dev |
| Socket.IO Docs | https://socket.io/docs |
| Prisma Docs | https://www.prisma.io/docs |
| CloudFlare Tunnel | https://developers.cloudflare.com/cloudflare-one/connections/connect-networks |
| JWT | https://jwt.io |
| MongoDB Mongoose | https://mongoosejs.com |
| Chart.js | https://www.chartjs.org/docs |
