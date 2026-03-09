'use strict';

require('dotenv').config();

const express = require('express');
const cors    = require('cors');
const path    = require('path');
const routes  = require('./routes');

const app  = express();
const PORT = process.env.PORT || 3002;

// ── Middleware ───────────────────────────────────────────────
app.use(cors());
app.use(express.json());

// Request logger
app.use((req, _res, next) => {
  console.log(`[${new Date().toISOString()}] ${req.method} ${req.url}`);
  next();
});

// ── API Routes ───────────────────────────────────────────────
app.use('/api', routes);

// ── Serve Parcel-built frontend ──────────────────────────────
const DIST_DIR = path.join(__dirname, '..', '..', 'Frontend', 'dist');
app.use(express.static(DIST_DIR));

// Fallback: serve index.html for any non-API route (SPA support)
app.get(/^(?!\/api).*/, (_req, res) => {
  const indexPath = path.join(DIST_DIR, 'index.html');
  res.sendFile(indexPath, (err) => {
    if (err) {
      res.status(404).send(
        '<h2>Frontend belum di-build.</h2>' +
        '<p>Jalankan <code>npm run build</code> di <code>Server/Frontend</code>.</p>'
      );
    }
  });
});

// ── 404 handler for unknown API routes ───────────────────────
app.use('/api', (_req, res) => {
  res.status(404).json({ error: 'API endpoint tidak ditemukan', code: 'NOT_FOUND' });
});

// ── Global error handler ─────────────────────────────────────
// eslint-disable-next-line no-unused-vars
app.use((err, _req, res, _next) => {
  console.error('[ERROR]', err.stack || err.message);
  res.status(500).json({
    error:  'Internal Server Error',
    detail: err.message,
    code:   'INTERNAL_ERROR',
  });
});

// ── Start ────────────────────────────────────────────────────
app.listen(PORT, () => {
  console.log('╔══════════════════════════════════════════╗');
  console.log('║   ESP32 DataLogger – Backend (P2)        ║');
  console.log('╠══════════════════════════════════════════╣');
  console.log(`║  Server running on http://localhost:${PORT}  ║`);
  console.log('║  Database: SQLite (better-sqlite3)       ║');
  console.log('║  Validation: Zod                         ║');
  console.log('║  Press Ctrl+C to stop                    ║');
  console.log('╚══════════════════════════════════════════╝');
});
