'use strict';

require('dotenv').config();

const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3001;
const START_TIME = Date.now();

// ── In-memory sensor data store ─────────────────────────────────────────────
let sensorData = {
  suhu: null,
  kelembaban: null,
  cahaya: null,
  ledState: false,
  lastUpdate: null,
};

// ── Middleware ───────────────────────────────────────────────────────────────
app.use(cors());
app.use(express.json());

// Request logger with timestamp
app.use((req, _res, next) => {
  const ts = new Date().toISOString();
  console.log(`[${ts}] ${req.method} ${req.url}`);
  next();
});

// ── Static frontend ──────────────────────────────────────────────────────────
const FRONTEND_DIR = path.join(__dirname, '..', 'Frontend');
app.use(express.static(FRONTEND_DIR));

app.get('/', (_req, res) => {
  res.sendFile(path.join(FRONTEND_DIR, 'index.html'));
});

// ── API Routes ───────────────────────────────────────────────────────────────

/**
 * POST /api/sensor
 * Body: { suhu: number, kelembaban: number, cahaya: number }
 * Updates the in-memory sensor data.
 */
app.post('/api/sensor', (req, res) => {
  const { suhu, kelembaban, cahaya } = req.body;

  if (suhu === undefined || kelembaban === undefined || cahaya === undefined) {
    return res.status(400).json({
      error: 'Bad Request',
      detail: 'Body must contain suhu, kelembaban, and cahaya fields.',
    });
  }

  sensorData.suhu = parseFloat(suhu);
  sensorData.kelembaban = parseFloat(kelembaban);
  sensorData.cahaya = parseInt(cahaya, 10);
  sensorData.lastUpdate = new Date().toISOString();

  console.log(
    `[SENSOR] suhu=${sensorData.suhu}°C  kelembaban=${sensorData.kelembaban}%  cahaya=${sensorData.cahaya}`
  );

  return res.status(200).json({ message: 'Sensor data updated', data: sensorData });
});

/**
 * GET /api/data
 * Returns the current sensor state.
 */
app.get('/api/data', (_req, res) => {
  return res.json(sensorData);
});

/**
 * POST /api/led
 * Toggles the LED state.
 * Returns { ledState: boolean }
 */
app.post('/api/led', (_req, res) => {
  sensorData.ledState = !sensorData.ledState;
  console.log(`[LED] State toggled → ${sensorData.ledState ? 'ON' : 'OFF'}`);
  return res.json({ ledState: sensorData.ledState });
});

/**
 * GET /api/status
 * Returns server uptime and current timestamp.
 */
app.get('/api/status', (_req, res) => {
  return res.json({
    uptime: Math.floor((Date.now() - START_TIME) / 1000),
    timestamp: new Date().toISOString(),
    status: 'OK',
  });
});

// ── 404 handler ──────────────────────────────────────────────────────────────
app.use((_req, res) => {
  res.status(404).json({ error: 'Not Found' });
});

// ── Global error handler ─────────────────────────────────────────────────────
// eslint-disable-next-line no-unused-vars
app.use((err, _req, res, _next) => {
  console.error('[ERROR]', err.message);
  res.status(500).json({ error: 'Internal Server Error', detail: err.message });
});

// ── Start ────────────────────────────────────────────────────────────────────
app.listen(PORT, () => {
  console.log('╔══════════════════════════════════════════╗');
  console.log('║   ESP32 WebServer Monitor – Backend      ║');
  console.log('╠══════════════════════════════════════════╣');
  console.log(`║  Server running on http://localhost:${PORT}  ║`);
  console.log('║  Press Ctrl+C to stop                    ║');
  console.log('╚══════════════════════════════════════════╝');
});
