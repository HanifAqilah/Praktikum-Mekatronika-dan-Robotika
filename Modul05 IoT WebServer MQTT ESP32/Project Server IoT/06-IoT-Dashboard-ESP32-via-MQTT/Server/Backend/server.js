'use strict';

require('dotenv').config();
const express = require('express');
const mqtt = require('mqtt');
const Database = require('better-sqlite3');
const path = require('path');
const fs = require('fs');

// ─── Configuration ────────────────────────────────────────────────────────────
const PORT = parseInt(process.env.PORT || '3006', 10);
const MQTT_BROKER_URL = process.env.MQTT_BROKER_URL || 'mqtt://localhost:1883';
const MQTT_USERNAME = process.env.MQTT_USERNAME || '';
const MQTT_PASSWORD = process.env.MQTT_PASSWORD || '';
const MQTT_CLIENT_ID = `percobaan6-server-${Math.random().toString(16).slice(2, 8)}`;

const SENSOR_TOPICS = [
  'esp32/sensor/suhu',
  'esp32/sensor/kelembaban',
  'esp32/sensor/cahaya',
  'esp32/status',
];
const COMMAND_TOPIC = 'esp32/command/led';
const SUBSCRIBE_WILDCARD = 'esp32/#';
const DB_PATH = path.join(__dirname, 'percobaan6.db');
const FRONTEND_DIR = path.join(__dirname, '..', 'Frontend');

// ─── SQLite Setup ─────────────────────────────────────────────────────────────
const db = new Database(DB_PATH);

db.exec(`
  CREATE TABLE IF NOT EXISTS mqtt_messages (
    id        INTEGER PRIMARY KEY AUTOINCREMENT,
    topic     TEXT    NOT NULL,
    payload   TEXT    NOT NULL,
    received_at TEXT  NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ', 'now'))
  );
  CREATE INDEX IF NOT EXISTS idx_received_at ON mqtt_messages(received_at DESC);
`);

const insertMsg = db.prepare(
  'INSERT INTO mqtt_messages (topic, payload) VALUES (?, ?)'
);

function dbInsert(topic, payload) {
  try {
    insertMsg.run(topic, payload);
    // Prune to keep DB lean – keep last 1000 rows
    db.prepare(
      'DELETE FROM mqtt_messages WHERE id NOT IN (SELECT id FROM mqtt_messages ORDER BY id DESC LIMIT 1000)'
    ).run();
  } catch (err) {
    console.error('[DB] Insert error:', err.message);
  }
}

function dbHistory(limit = 50) {
  return db
    .prepare(
      'SELECT id, topic, payload, received_at FROM mqtt_messages ORDER BY id DESC LIMIT ?'
    )
    .all(limit);
}

// ─── In-memory cache (latest value per topic) ─────────────────────────────────
/** @type {Map<string, {payload: string, ts: string}>} */
const latestCache = new Map();

// ─── MQTT Client ──────────────────────────────────────────────────────────────
let mqttConnected = false;

const mqttOpts = {
  clientId: MQTT_CLIENT_ID,
  clean: true,
  reconnectPeriod: 5000,
  connectTimeout: 10000,
};
if (MQTT_USERNAME) mqttOpts.username = MQTT_USERNAME;
if (MQTT_PASSWORD) mqttOpts.password = MQTT_PASSWORD;

console.log(`[MQTT] Connecting to ${MQTT_BROKER_URL} …`);
const mqttClient = mqtt.connect(MQTT_BROKER_URL, mqttOpts);

mqttClient.on('connect', () => {
  mqttConnected = true;
  console.log('[MQTT] Connected');
  mqttClient.subscribe(SUBSCRIBE_WILDCARD, { qos: 1 }, (err) => {
    if (err) console.error('[MQTT] Subscribe error:', err.message);
    else console.log(`[MQTT] Subscribed to ${SUBSCRIBE_WILDCARD}`);
  });
});

mqttClient.on('reconnect', () => {
  mqttConnected = false;
  console.log('[MQTT] Reconnecting…');
});

mqttClient.on('offline', () => {
  mqttConnected = false;
  console.log('[MQTT] Offline');
});

mqttClient.on('error', (err) => {
  console.error('[MQTT] Error:', err.message);
});

mqttClient.on('message', (topic, buf) => {
  const payload = buf.toString();
  const ts = new Date().toISOString();
  console.log(`[MQTT] ${ts} | ${topic}: ${payload}`);

  latestCache.set(topic, { payload, ts });
  dbInsert(topic, payload);
});

// ─── Express App ──────────────────────────────────────────────────────────────
const app = express();
app.use(express.json());

// Serve frontend static files
if (fs.existsSync(FRONTEND_DIR)) {
  app.use(express.static(FRONTEND_DIR));
} else {
  console.warn('[WEB] Frontend directory not found:', FRONTEND_DIR);
}

// GET /api/broker/status
app.get('/api/broker/status', (_req, res) => {
  res.json({
    connected: mqttConnected,
    broker: MQTT_BROKER_URL,
    clientId: MQTT_CLIENT_ID,
    ts: new Date().toISOString(),
  });
});

// GET /api/latest  – latest value per topic from in-memory cache
app.get('/api/latest', (_req, res) => {
  const result = {};
  for (const [topic, data] of latestCache.entries()) {
    result[topic] = { payload: data.payload, ts: data.ts };
  }
  res.json(result);
});

// GET /api/history – last 50 records from SQLite
app.get('/api/history', (req, res) => {
  const limit = Math.min(parseInt(req.query.limit || '50', 10), 200);
  try {
    const rows = dbHistory(limit);
    res.json(rows);
  } catch (err) {
    console.error('[API] /api/history error:', err.message);
    res.status(500).json({ error: 'Database error' });
  }
});

// POST /api/command  – publish to esp32/command/led
// Body: { "command": "ON" }  or  { "command": "OFF" }
app.post('/api/command', (req, res) => {
  const { command } = req.body || {};
  if (!command || !['ON', 'OFF'].includes(String(command).toUpperCase())) {
    return res.status(400).json({ error: 'Invalid command. Use "ON" or "OFF".' });
  }
  const payload = String(command).toUpperCase();
  if (!mqttConnected) {
    return res.status(503).json({ error: 'MQTT broker not connected' });
  }
  mqttClient.publish(COMMAND_TOPIC, payload, { qos: 1 }, (err) => {
    if (err) {
      console.error('[MQTT] Publish error:', err.message);
      return res.status(500).json({ error: 'Failed to publish command' });
    }
    console.log(`[MQTT] Published to ${COMMAND_TOPIC}: ${payload}`);
    res.json({ success: true, topic: COMMAND_TOPIC, payload });
  });
});

// Catch-all: serve index.html for SPA navigation
app.get('*', (_req, res) => {
  const indexPath = path.join(FRONTEND_DIR, 'index.html');
  if (fs.existsSync(indexPath)) {
    res.sendFile(indexPath);
  } else {
    res.status(404).send('Frontend not found');
  }
});

// ─── Start Server ─────────────────────────────────────────────────────────────
app.listen(PORT, () => {
  console.log(`[HTTP] Server running on http://localhost:${PORT}`);
  console.log(`[HTTP] Dashboard → http://localhost:${PORT}/`);
});

// ─── Graceful shutdown ────────────────────────────────────────────────────────
function shutdown(signal) {
  console.log(`\n[SYS] Received ${signal}, shutting down…`);
  mqttClient.end(false, () => {
    db.close();
    process.exit(0);
  });
}
process.on('SIGINT', () => shutdown('SIGINT'));
process.on('SIGTERM', () => shutdown('SIGTERM'));
