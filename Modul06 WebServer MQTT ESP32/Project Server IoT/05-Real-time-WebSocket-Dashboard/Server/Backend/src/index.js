require('dotenv').config();

const http = require('http');
const express = require('express');
const cors = require('cors');
const db = require('./database');
const { createWsServer, broadcast, getClientCount, closeWsServer } = require('./websocket');

const PORT = parseInt(process.env.PORT || '8005', 10);
const CORS_ORIGINS = (process.env.CORS_ORIGINS || 'http://localhost:9005')
  .split(',')
  .map((s) => s.trim());

const app = express();
const server = http.createServer(app);

// ── WebSocket (same port, path /ws) ────────────────────────────────────────
createWsServer(server);

// ── In-memory LED state ─────────────────────────────────────────────────────
let ledState = false;

// ── Middleware ──────────────────────────────────────────────────────────────
app.use(cors({ origin: CORS_ORIGINS, methods: ['GET', 'POST', 'OPTIONS'] }));
app.use(express.json());

// ── Routes ──────────────────────────────────────────────────────────────────

// POST /api/sensor — receive reading from ESP32
app.post('/api/sensor', (req, res) => {
  const { device_id, suhu, kelembaban, cahaya } = req.body;

  if (suhu === undefined || kelembaban === undefined || cahaya === undefined) {
    return res.status(400).json({
      error: 'Missing required fields: suhu, kelembaban, cahaya',
    });
  }
  if (typeof suhu !== 'number' || typeof kelembaban !== 'number' || typeof cahaya !== 'number') {
    return res.status(400).json({ error: 'Fields suhu, kelembaban, cahaya must be numbers' });
  }

  const id = db.insert({
    device_id: device_id || 'esp32-unknown',
    suhu,
    kelembaban,
    cahaya,
  });

  const record = db.getLatest();
  broadcast({ type: 'sensor', data: record });

  return res.status(201).json({ id, message: 'Sensor data saved successfully' });
});

// GET /api/history?limit=100
app.get('/api/history', (req, res) => {
  const limit = parseInt(req.query.limit) || 100;
  const rows = db.getHistory(limit);
  res.json(rows);
});

// GET /api/latest
app.get('/api/latest', (req, res) => {
  const row = db.getLatest();
  if (!row) return res.status(404).json({ error: 'No data yet' });
  res.json(row);
});

// GET /api/stats
app.get('/api/stats', (req, res) => {
  res.json(db.getStats());
});

// POST /api/led — toggle LED state
app.post('/api/led', (req, res) => {
  // Accept explicit state from body, otherwise toggle
  if (typeof req.body.state === 'boolean') {
    ledState = req.body.state;
  } else {
    ledState = !ledState;
  }
  broadcast({ type: 'led', state: ledState });
  res.json({ state: ledState });
});

// GET /api/led — read current LED state
app.get('/api/led', (req, res) => {
  res.json({ state: ledState });
});

// GET /api/status
app.get('/api/status', (req, res) => {
  res.json({
    uptime: process.uptime(),
    clients: getClientCount(),
    db_ok: true,
    led_state: ledState,
  });
});

// ── 404 catch-all ───────────────────────────────────────────────────────────
app.use((req, res) => res.status(404).json({ error: 'Not found' }));

// ── Start ───────────────────────────────────────────────────────────────────
server.listen(PORT, () => {
  console.log(`[HTTP] Express server running on http://localhost:${PORT}`);
  console.log(`[WS]   WebSocket endpoint: ws://localhost:${PORT}/ws`);
  console.log(`[INFO] CORS allowed origins: ${CORS_ORIGINS.join(', ')}`);
});

// ── Graceful shutdown ───────────────────────────────────────────────────────
function shutdown(signal) {
  console.log(`\n[INFO] Received ${signal}. Shutting down...`);
  closeWsServer();
  db.closeDb();
  server.close(() => {
    console.log('[INFO] HTTP server closed.');
    process.exit(0);
  });
  setTimeout(() => process.exit(1), 5000);
}

process.on('SIGINT', () => shutdown('SIGINT'));
process.on('SIGTERM', () => shutdown('SIGTERM'));
