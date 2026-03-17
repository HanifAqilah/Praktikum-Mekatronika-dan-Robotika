# JOBSHEET MODUL 05: WEB SERVER & FULL-STACK JAVASCRIPT

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – Web Server & Full-Stack JavaScript  
**Pertemuan:** 7–8 (2 × 2 SKS)  
**Tanggal:** ___________________  
**Nama Kelompok:** ___________________  
**Anggota:**

| No | Nama | NIM |
|----|------|-----|
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:

1. Membuat web server menggunakan Node.js dan Express.js
2. Membangun REST API CRUD dengan respons JSON
3. Mengintegrasikan database (SQLite) ke aplikasi backend
4. Membuat frontend interaktif dengan React dan Tailwind CSS
5. Mengimplementasikan real-time update menggunakan Socket.IO
6. Men-deploy aplikasi ke internet menggunakan CloudFlare Tunnel
7. Memahami full-stack architecture dan alur data frontend–backend–database

---

## B. ALAT DAN BAHAN

### Software (wajib ter-install sebelum praktikum)

| No | Software | Versi | Fungsi |
|----|---------|-------|--------|
| 1 | Node.js | 18+ LTS | Runtime JavaScript server |
| 2 | npm | 9+ | Package manager |
| 3 | VS Code | Terbaru | Code editor |
| 4 | Git | Terbaru | Version control |
| 5 | cloudflared | Terbaru | Tunnel deployment |
| 6 | Postman / Thunder Client | Terbaru | API testing |
| 7 | Browser (Chrome/Firefox) | Terbaru | Frontend testing |

### Cek Instalasi

```bash
node --version    # v18.x.x atau lebih baru
npm --version     # 9.x.x atau lebih baru
git --version     # git version 2.x.x
cloudflared --version
```

---

## C. DASAR TEORI SINGKAT

### C.1 Alur Data Full-Stack

```
[Browser / React]
      │  fetch('/api/todos')
      ▼
[Express.js Server :3000]
      │  SELECT * FROM todos
      ▼
[Database – SQLite / PostgreSQL]
      │  rows
      ▼
[Express → JSON Response]
      │  { todos: [...] }
      ▼
[Browser – render list]
```

### C.2 HTTP Methods untuk REST API

| Method | Endpoint | Aksi |
|--------|---------|------|
| GET | /api/todos | Ambil semua |
| GET | /api/todos/:id | Ambil satu |
| POST | /api/todos | Buat baru |
| PUT | /api/todos/:id | Update |
| DELETE | /api/todos/:id | Hapus |

---

## D. LANGKAH KERJA

---

### PERCOBAAN 1: SETUP NODE.JS & EXPRESS HELLO WORLD

**Estimasi waktu: 20 menit**

**Langkah:**

```bash
# 1. Buat folder project
mkdir percobaan1-express && cd percobaan1-express

# 2. Initialize npm project
npm init -y

# 3. Install Express
npm install express

# 4. Install nodemon (auto-restart saat file berubah)
npm install -D nodemon
```

**Edit `package.json` – tambahkan scripts:**

```json
{
  "scripts": {
    "dev": "nodemon server.js",
    "start": "node server.js"
  }
}
```

**Buat file `server.js`:**

```javascript
const express = require('express');
const app = express();
const PORT = 3000;

app.use(express.json());

// Endpoint 1: root
app.get('/', (req, res) => {
  res.json({
    message: 'Hello from Express!',
    timestamp: new Date().toISOString(),
    version: '1.0.0'
  });
});

// Endpoint 2: info server
app.get('/info', (req, res) => {
  res.json({
    node: process.version,
    platform: process.platform,
    uptime: process.uptime().toFixed(1) + 's'
  });
});

// Endpoint 3: query parameter
app.get('/greet', (req, res) => {
  const { name = 'World' } = req.query;
  res.json({ message: `Hello, ${name}!` });
});

app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});
```

**Jalankan:**

```bash
npm run dev
```

**Testing di browser / Postman:**

| Request | Expected Response |
|---------|------------------|
| GET `http://localhost:3000/` | JSON message + timestamp |
| GET `http://localhost:3000/info` | Node version + platform |
| GET `http://localhost:3000/greet?name=Budi` | `"Hello, Budi!"` |

**Dokumentasikan:**
- [ ] Screenshot terminal menunjukkan server berjalan
- [ ] Screenshot browser/Postman 3 endpoint
- [ ] Catat: apa fungsi `app.use(express.json())`?

---

### PERCOBAAN 2: REST API CRUD (IN-MEMORY)

**Estimasi waktu: 30 menit**

Buat REST API todos menggunakan array javascript (tanpa database dulu).

**Buat folder baru `percobaan2-crud/` dan file `server.js`:**

```javascript
const express = require('express');
const app = express();
app.use(express.json());

// In-memory data store
let todos = [
  { id: 1, title: 'Belajar Node.js',  done: false },
  { id: 2, title: 'Belajar Express',  done: false },
  { id: 3, title: 'Buat REST API',    done: true  }
];
let nextId = 4;

// GET all
app.get('/api/todos', (req, res) => {
  const { done } = req.query;
  const result = done !== undefined
    ? todos.filter(t => t.done === (done === 'true'))
    : todos;
  res.json({ data: result, count: result.length });
});

// GET one
app.get('/api/todos/:id', (req, res) => {
  const todo = todos.find(t => t.id === Number(req.params.id));
  if (!todo) return res.status(404).json({ error: 'Not found' });
  res.json(todo);
});

// POST create
app.post('/api/todos', (req, res) => {
  const { title } = req.body;
  if (!title?.trim()) return res.status(400).json({ error: 'Title required' });
  const todo = { id: nextId++, title: title.trim(), done: false };
  todos.push(todo);
  res.status(201).json(todo);
});

// PUT update
app.put('/api/todos/:id', (req, res) => {
  const idx = todos.findIndex(t => t.id === Number(req.params.id));
  if (idx === -1) return res.status(404).json({ error: 'Not found' });
  todos[idx] = { ...todos[idx], ...req.body, id: todos[idx].id };
  res.json(todos[idx]);
});

// DELETE
app.delete('/api/todos/:id', (req, res) => {
  const idx = todos.findIndex(t => t.id === Number(req.params.id));
  if (idx === -1) return res.status(404).json({ error: 'Not found' });
  todos.splice(idx, 1);
  res.status(204).end();
});

app.listen(3000, () => console.log('API running on :3000'));
```

**Test semua endpoint dengan Postman:**

| No | Method | URL | Body | Expected |
|----|--------|-----|------|---------|
| 1 | GET | /api/todos | - | Array 3 todos |
| 2 | GET | /api/todos?done=true | - | Array 1 todo |
| 3 | GET | /api/todos/1 | - | Todo id=1 |
| 4 | GET | /api/todos/99 | - | 404 error |
| 5 | POST | /api/todos | `{"title":"Test"}` | Status 201, todo baru |
| 6 | POST | /api/todos | `{}` | Status 400, error |
| 7 | PUT | /api/todos/1 | `{"done":true}` | Todo updated |
| 8 | DELETE | /api/todos/1 | - | Status 204 |

**Dokumentasikan:**
- [ ] Screenshot semua 8 test di Postman (dengan request + response terlihat)
- [ ] Catat: perbedaan status 200, 201, 204, 400, 404

---

### PERCOBAAN 3: DATABASE SQLITE

**Estimasi waktu: 25 menit**

Ganti in-memory store dengan SQLite database.

```bash
mkdir percobaan3-sqlite && cd percobaan3-sqlite
npm init -y
npm install express better-sqlite3
npm install -D nodemon
```

**Buat `server.js`:**

```javascript
const express  = require('express');
const Database = require('better-sqlite3');

const app = express();
const db  = new Database('todos.db');
app.use(express.json());

// Initialize table
db.exec(`
  CREATE TABLE IF NOT EXISTS todos (
    id        INTEGER PRIMARY KEY AUTOINCREMENT,
    title     TEXT    NOT NULL,
    done      INTEGER DEFAULT 0,
    created_at TEXT   DEFAULT (datetime('now'))
  )
`);

// Prepared statements
const stmtAll    = db.prepare('SELECT * FROM todos ORDER BY id DESC');
const stmtOne    = db.prepare('SELECT * FROM todos WHERE id = ?');
const stmtInsert = db.prepare('INSERT INTO todos (title) VALUES (?)');
const stmtUpdate = db.prepare('UPDATE todos SET title = ?, done = ? WHERE id = ?');
const stmtDelete = db.prepare('DELETE FROM todos WHERE id = ?');

// Routes
app.get('/api/todos', (req, res) =>
  res.json(stmtAll.all()));

app.get('/api/todos/:id', (req, res) => {
  const todo = stmtOne.get(req.params.id);
  todo ? res.json(todo) : res.status(404).json({ error: 'Not found' });
});

app.post('/api/todos', (req, res) => {
  const { title } = req.body;
  if (!title) return res.status(400).json({ error: 'Title required' });
  const result = stmtInsert.run(title);
  res.status(201).json(stmtOne.get(result.lastInsertRowid));
});

app.put('/api/todos/:id', (req, res) => {
  const todo = stmtOne.get(req.params.id);
  if (!todo) return res.status(404).json({ error: 'Not found' });
  const { title = todo.title, done = todo.done } = req.body;
  stmtUpdate.run(title, done ? 1 : 0, req.params.id);
  res.json(stmtOne.get(req.params.id));
});

app.delete('/api/todos/:id', (req, res) => {
  stmtDelete.run(req.params.id);
  res.status(204).end();
});

app.listen(3000, () => console.log('SQLite API on :3000'));
```

**Testing:**
1. Tambah 3 todo via POST
2. Restart server (`Ctrl+C` lalu `npm run dev`)
3. GET semua → **data tetap ada** (persisten di SQLite file)

**Dokumentasikan:**
- [ ] Screenshot API berhasil setelah restart (data persisten)
- [ ] Screenshot file `todos.db` terbentuk di folder project
- [ ] Catat: apa bedanya `stmtAll.all()` vs `stmtOne.get()`?

---

### PERCOBAAN 4: REACT FRONTEND DENGAN VITE

**Estimasi waktu: 30 menit**

Buat frontend React yang mengonsumsi API dari Percobaan 3.

```bash
# Di folder terpisah (bukan di dalam percobaan3)
npm create vite@latest percobaan4-react -- --template react
cd percobaan4-react
npm install
npm install axios
```

**Edit `src/App.jsx`:**

```jsx
import { useState, useEffect } from 'react';
import axios from 'axios';

const API = 'http://localhost:3000/api';

export default function App() {
  const [todos, setTodos]   = useState([]);
  const [input, setInput]   = useState('');
  const [loading, setLoading] = useState(true);

  const fetchTodos = () =>
    axios.get(`${API}/todos`).then(r => {
      setTodos(r.data);
      setLoading(false);
    });

  useEffect(() => { fetchTodos(); }, []);

  const addTodo = async (e) => {
    e.preventDefault();
    if (!input.trim()) return;
    await axios.post(`${API}/todos`, { title: input });
    setInput('');
    fetchTodos();
  };

  const toggleTodo = async (todo) => {
    await axios.put(`${API}/todos/${todo.id}`, { done: !todo.done });
    fetchTodos();
  };

  const deleteTodo = async (id) => {
    await axios.delete(`${API}/todos/${id}`);
    fetchTodos();
  };

  if (loading) return <p>Loading...</p>;

  return (
    <div style={{ maxWidth: 500, margin: '40px auto', fontFamily: 'sans-serif' }}>
      <h1>📝 Todo App</h1>
      <form onSubmit={addTodo} style={{ display: 'flex', gap: 8 }}>
        <input
          value={input} onChange={e => setInput(e.target.value)}
          placeholder="New todo..." style={{ flex: 1, padding: 8 }}
        />
        <button type="submit" style={{ padding: '8px 16px' }}>Add</button>
      </form>
      <ul style={{ listStyle: 'none', padding: 0, marginTop: 20 }}>
        {todos.map(todo => (
          <li key={todo.id} style={{
            display: 'flex', alignItems: 'center', gap: 8,
            padding: 8, borderBottom: '1px solid #eee'
          }}>
            <input type="checkbox" checked={!!todo.done}
              onChange={() => toggleTodo(todo)} />
            <span style={{
              flex: 1,
              textDecoration: todo.done ? 'line-through' : 'none',
              color: todo.done ? '#aaa' : '#000'
            }}>{todo.title}</span>
            <button onClick={() => deleteTodo(todo.id)}
              style={{ color: 'red', border: 'none', background: 'none', cursor: 'pointer' }}>
              ✕
            </button>
          </li>
        ))}
      </ul>
      <p style={{ color: '#888', fontSize: 12 }}>
        {todos.filter(t => !t.done).length} remaining
      </p>
    </div>
  );
}
```

**Jalankan kedua server sekaligus:**

```bash
# Terminal 1 – Backend (Percobaan 3)
cd percobaan3-sqlite && npm run dev

# Terminal 2 – Frontend
cd percobaan4-react && npm run dev
```

**Fix CORS di backend (tambah ke server.js Percobaan 3):**

```bash
npm install cors
```

```javascript
const cors = require('cors');
app.use(cors());
```

**Dokumentasikan:**
- [ ] Screenshot frontend berjalan dengan data dari API
- [ ] Screenshot add todo baru → muncul di list
- [ ] Screenshot checkbox → item ter-coret
- [ ] Screenshot delete item → item hilang

---

### PERCOBAAN 5: REAL-TIME DENGAN SOCKET.IO

**Estimasi waktu: 35 menit**

Buat counter yang terupdate real-time di semua browser yang membuka halaman.

```bash
mkdir percobaan5-realtime && cd percobaan5-realtime
npm init -y
npm install express socket.io
```

**Buat `server.js`:**

```javascript
const express    = require('express');
const http       = require('http');
const { Server } = require('socket.io');
const path       = require('path');

const app    = express();
const server = http.createServer(app);
const io     = new Server(server);

app.use(express.static(path.join(__dirname, 'public')));

let counter = 0;

io.on('connection', (socket) => {
  console.log(`Client connected: ${socket.id}`);
  
  // Kirim nilai awal ke client baru
  socket.emit('counter_update', counter);

  socket.on('increment', () => {
    counter++;
    io.emit('counter_update', counter); // broadcast ke SEMUA
  });

  socket.on('decrement', () => {
    counter = Math.max(0, counter - 1);
    io.emit('counter_update', counter);
  });

  socket.on('reset', () => {
    counter = 0;
    io.emit('counter_update', counter);
  });

  socket.on('disconnect', () => {
    console.log(`Disconnected: ${socket.id}`);
  });
});

server.listen(3000, () => console.log('Socket.IO server on :3000'));
```

**Buat `public/index.html`:**

```html
<!DOCTYPE html>
<html lang="id">
<head>
  <meta charset="UTF-8">
  <title>Real-Time Counter</title>
  <script src="/socket.io/socket.io.js"></script>
  <style>
    body { font-family: sans-serif; text-align: center; padding: 40px; }
    #counter { font-size: 80px; font-weight: bold; color: #333; }
    button { font-size: 24px; padding: 10px 20px; margin: 5px; cursor: pointer; }
  </style>
</head>
<body>
  <h1>Real-Time Counter</h1>
  <div id="counter">0</div>
  <br>
  <button onclick="socket.emit('decrement')">−</button>
  <button onclick="socket.emit('increment')">+</button>
  <button onclick="socket.emit('reset')" style="background:#ffcc00">Reset</button>
  <p id="status" style="color:gray">Connecting...</p>

  <script>
    const socket = io();
    const counterEl = document.getElementById('counter');
    const statusEl  = document.getElementById('status');

    socket.on('connect', () => {
      statusEl.textContent = `✓ Connected (${socket.id})`;
      statusEl.style.color = 'green';
    });

    socket.on('counter_update', (value) => {
      counterEl.textContent = value;
    });

    socket.on('disconnect', () => {
      statusEl.textContent = '✗ Disconnected';
      statusEl.style.color = 'red';
    });
  </script>
</body>
</html>
```

**Testing:**
1. Buka `http://localhost:3000` di 2 browser tab berbeda (atau Chrome + Firefox)
2. Klik tombol + di satu tab → nilai naik di **kedua tab** secara real-time

**Dokumentasikan:**
- [ ] Screenshot 2 tab browser menampilkan nilai yang sama
- [ ] Video singkat (screen record 15 detik) klik tombol → update simultaneous

---

### PERCOBAAN 6: CLOUDFLARE TUNNEL DEPLOYMENT

**Estimasi waktu: 25 menit**

Deploy aplikasi dari Percobaan 4+5 ke internet publik via CloudFlare Tunnel.

**Pastikan backend dari Percobaan 3 berjalan di port 3000.**

```bash
# Buka terminal baru, jalankan tunnel
cloudflared tunnel --url http://localhost:3000
```

**Output yang akan muncul:**

```
...
2024-03-17T10:30:00Z INF +--------------------------------------------------------------------------------------------+
2024-03-17T10:30:00Z INF |  Your quick Tunnel has been created! Visit it at (it may take some time to be reachable): |
2024-03-17T10:30:00Z INF |  https://random-words-here.trycloudflare.com                                             |
2024-03-17T10:30:00Z INF +--------------------------------------------------------------------------------------------+
```

**Test dari smartphone atau PC lain:**
1. Buka URL tunnel di browser smartphone (pastikan gunakan data seluler, bukan WiFi yang sama)
2. Test endpoint: `https://random-words.trycloudflare.com/api/todos`
3. Buka app React, update `API` constant dengan URL tunnel, rebuild dan test

**Dokumentasikan:**
- [ ] Screenshot terminal menampilkan tunnel URL
- [ ] Screenshot API diakses via URL tunnel di smartphone
- [ ] Screenshot URL tunnel terlihat di address bar (bukan localhost)

---

### PERCOBAAN 7: INTEGRASI FULL-STACK (PROYEK MINI)

**Estimasi waktu: 40 menit**

Gabungkan Express + SQLite + React + Socket.IO dalam satu project.

```bash
mkdir percobaan7-fullstack && cd percobaan7-fullstack
npm init -y
npm install express better-sqlite3 socket.io cors
npm install -D nodemon

# Frontend
npm create vite@latest client -- --template react
cd client && npm install axios socket.io-client && cd ..
```

**Struktur folder:**

```
percobaan7-fullstack/
├── server.js        ← Express + Socket.IO + SQLite
├── todos.db         ← auto-created
└── client/          ← React app
    └── src/
        └── App.jsx
```

**`server.js`:**

```javascript
const express    = require('express');
const http       = require('http');
const { Server } = require('socket.io');
const Database   = require('better-sqlite3');
const cors       = require('cors');

const app  = express();
const srv  = http.createServer(app);
const io   = new Server(srv, { cors: { origin: '*' } });
const db   = new Database('todos.db');

app.use(cors(), express.json());

db.exec(`CREATE TABLE IF NOT EXISTS todos (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  title TEXT NOT NULL, done INTEGER DEFAULT 0
)`);

const q = {
  all:    db.prepare('SELECT * FROM todos ORDER BY id DESC'),
  one:    db.prepare('SELECT * FROM todos WHERE id=?'),
  insert: db.prepare('INSERT INTO todos (title) VALUES (?)'),
  update: db.prepare('UPDATE todos SET done=? WHERE id=?'),
  delete: db.prepare('DELETE FROM todos WHERE id=?')
};

app.get('/api/todos',      (req, res) => res.json(q.all.all()));
app.post('/api/todos',     (req, res) => {
  if (!req.body.title) return res.status(400).json({ error: 'Title required' });
  const r = q.insert.run(req.body.title);
  const t = q.one.get(r.lastInsertRowid);
  io.emit('todo_added', t);
  res.status(201).json(t);
});
app.put('/api/todos/:id',  (req, res) => {
  q.update.run(req.body.done ? 1 : 0, req.params.id);
  const t = q.one.get(req.params.id);
  io.emit('todo_updated', t);
  res.json(t);
});
app.delete('/api/todos/:id', (req, res) => {
  q.delete.run(req.params.id);
  io.emit('todo_deleted', Number(req.params.id));
  res.status(204).end();
});

srv.listen(3000, () => console.log('Full-stack server on :3000'));
```

**`client/src/App.jsx`:**

```jsx
import { useState, useEffect } from 'react';
import axios from 'axios';
import { io } from 'socket.io-client';

const API    = 'http://localhost:3000/api';
const socket = io('http://localhost:3000');

export default function App() {
  const [todos, setTodos] = useState([]);
  const [input, setInput] = useState('');

  // Load initial data
  useEffect(() => {
    axios.get(`${API}/todos`).then(r => setTodos(r.data));
  }, []);

  // Real-time sync via Socket.IO
  useEffect(() => {
    socket.on('todo_added',   (t)  => setTodos(p => [t, ...p]));
    socket.on('todo_updated', (t)  => setTodos(p => p.map(x => x.id===t.id ? t : x)));
    socket.on('todo_deleted', (id) => setTodos(p => p.filter(x => x.id!==id)));
    return () => socket.removeAllListeners();
  }, []);

  const add    = async (e) => {
    e.preventDefault();
    if (!input.trim()) return;
    await axios.post(`${API}/todos`, { title: input });
    setInput('');
  };
  const toggle = (t) => axios.put(`${API}/todos/${t.id}`, { done: !t.done });
  const del    = (id) => axios.delete(`${API}/todos/${id}`);

  return (
    <div style={{ maxWidth:480, margin:'40px auto', fontFamily:'sans-serif' }}>
      <h1>📝 Real-Time Todo</h1>
      <form onSubmit={add} style={{ display:'flex', gap:8 }}>
        <input value={input} onChange={e=>setInput(e.target.value)}
          placeholder="Tambah todo..." style={{ flex:1, padding:8 }} />
        <button type="submit">Add</button>
      </form>
      <ul style={{ listStyle:'none', padding:0 }}>
        {todos.map(t=>(
          <li key={t.id} style={{ display:'flex', alignItems:'center', gap:8, padding:8, borderBottom:'1px solid #eee' }}>
            <input type="checkbox" checked={!!t.done} onChange={()=>toggle(t)}/>
            <span style={{ flex:1, textDecoration:t.done?'line-through':'none' }}>{t.title}</span>
            <button onClick={()=>del(t.id)} style={{ border:'none', background:'none', color:'red', cursor:'pointer' }}>✕</button>
          </li>
        ))}
      </ul>
    </div>
  );
}
```

```bash
# Terminal 1: backend
npm run dev

# Terminal 2: frontend
cd client && npm run dev
```

**Test real-time:**
- Buka 2 tab `http://localhost:5173`
- Tambah todo di Tab 1 → **muncul langsung di Tab 2 (tanpa refresh!)**

**Dokumentasikan:**
- [ ] Screenshot 2 tab tersinkronisasi secara real-time
- [ ] Screenshot semua 7 percobaan selesai (endpoint, data, UI)

---

## E. KESIMPULAN

Isi setelah menyelesaikan semua percobaan:

1. **Percobaan 1:** Hal baru yang dipelajari tentang Express.js:
   > _______________________________________________

2. **Percobaan 3:** Perbedaan penyimpanan data in-memory vs SQLite:
   > _______________________________________________

3. **Percobaan 5:** Jelaskan dengan kata sendiri bagaimana Socket.IO bekerja:
   > _______________________________________________

4. **Percobaan 6:** Apa kegunaan CloudFlare Tunnel dalam proyek nyata?
   > _______________________________________________

5. **Percobaan 7:** Tantangan terbesar membuat full-stack app dan solusinya:
   > _______________________________________________

---

## F. RUBRIK PENILAIAN JOBSHEET (100 POIN)

| No | Percobaan | Bobot | Kriteria |
|----|----------|-------|---------|
| 1 | Express Hello World | 10 | 3 endpoint berjalan, screenshot |
| 2 | REST API CRUD | 15 | Semua 8 test Postman, status code tepat |
| 3 | SQLite Integration | 15 | Data persisten setelah restart |
| 4 | React Frontend | 20 | CRUD via UI, connection ke API |
| 5 | Socket.IO Real-Time | 15 | 2 tab tersinkronisasi |
| 6 | CloudFlare Tunnel | 10 | URL publik berjalan, akses dari HP |
| 7 | Full-Stack Integration | 15 | Semua fitur tergabung |
| - | Kesimpulan | **0** | *(wajib diisi)* |

**Total: 100 Poin**
