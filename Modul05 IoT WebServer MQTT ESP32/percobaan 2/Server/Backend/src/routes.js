'use strict';

const { Router } = require('express');
const db         = require('./database');
const { validateSensorData } = require('./validation');

const router = Router();

// ── POST /api/sensor ──────────────────────────────────────────
// Validates payload with Zod, then persists to SQLite.
router.post('/sensor', (req, res) => {
  const validation = validateSensorData(req.body);

  if (!validation.success) {
    return res.status(422).json({
      error:  'Validation Error',
      detail: validation.error,
      code:   'VALIDATION_FAILED',
    });
  }

  try {
    const { id } = db.insertReading(validation.data);
    return res.status(201).json({
      id,
      message: 'Data sensor berhasil disimpan',
      data:    validation.data,
    });
  } catch (err) {
    return res.status(500).json({
      error:  'Database Error',
      detail: err.message,
      code:   'DB_INSERT_FAILED',
    });
  }
});

// ── GET /api/latest ───────────────────────────────────────────
// Returns the most recent sensor reading.
router.get('/latest', (_req, res) => {
  try {
    const row = db.getLatest();
    if (!row) {
      return res.status(404).json({
        error:  'Not Found',
        detail: 'Belum ada data sensor yang tersimpan.',
        code:   'NO_DATA',
      });
    }
    return res.json(row);
  } catch (err) {
    return res.status(500).json({
      error:  'Database Error',
      detail: err.message,
      code:   'DB_READ_FAILED',
    });
  }
});

// ── GET /api/history?limit=50 ─────────────────────────────────
// Returns an array of recent readings.
router.get('/history', (req, res) => {
  const limit = parseInt(req.query.limit, 10) || 50;
  if (isNaN(limit) || limit < 1) {
    return res.status(400).json({
      error:  'Bad Request',
      detail: 'Parameter limit harus berupa angka positif.',
      code:   'INVALID_LIMIT',
    });
  }

  try {
    const rows = db.getHistory(limit);
    return res.json({ count: rows.length, data: rows });
  } catch (err) {
    return res.status(500).json({
      error:  'Database Error',
      detail: err.message,
      code:   'DB_READ_FAILED',
    });
  }
});

// ── GET /api/stats ────────────────────────────────────────────
// Returns aggregate statistics for the last 24 hours.
router.get('/stats', (_req, res) => {
  try {
    const stats = db.getStats();
    return res.json({
      period: 'last_24_hours',
      ...stats,
    });
  } catch (err) {
    return res.status(500).json({
      error:  'Database Error',
      detail: err.message,
      code:   'DB_READ_FAILED',
    });
  }
});

// ── DELETE /api/data ──────────────────────────────────────────
// Deletes records older than 7 days.
router.delete('/data', (req, res) => {
  const days = parseInt(req.query.days, 10) || 7;
  try {
    const result = db.deleteOld(days);
    return res.json({
      message: `Data lebih dari ${days} hari berhasil dihapus`,
      deleted: result.deleted,
    });
  } catch (err) {
    return res.status(500).json({
      error:  'Database Error',
      detail: err.message,
      code:   'DB_DELETE_FAILED',
    });
  }
});

module.exports = router;
