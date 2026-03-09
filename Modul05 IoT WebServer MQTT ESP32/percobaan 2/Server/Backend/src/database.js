'use strict';

const Database = require('better-sqlite3');
const path     = require('path');

const DB_FILE = process.env.DB_FILE
  ? path.resolve(process.env.DB_FILE)
  : path.join(__dirname, '..', 'sensor.db');

const db = new Database(DB_FILE);

// Enable WAL mode for better concurrent read performance
db.pragma('journal_mode = WAL');
db.pragma('foreign_keys = ON');

// ── Schema ────────────────────────────────────────────────────
db.exec(`
  CREATE TABLE IF NOT EXISTS sensor_logs (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    suhu       REAL    NOT NULL,
    kelembaban REAL    NOT NULL,
    cahaya     INTEGER NOT NULL,
    device_id  TEXT    NOT NULL DEFAULT 'esp32-01',
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
  );

  CREATE INDEX IF NOT EXISTS idx_sensor_logs_created_at
    ON sensor_logs (created_at DESC);

  CREATE INDEX IF NOT EXISTS idx_sensor_logs_device_id
    ON sensor_logs (device_id);
`);

// ── Prepared statements ───────────────────────────────────────
const stmtInsert = db.prepare(`
  INSERT INTO sensor_logs (suhu, kelembaban, cahaya, device_id)
  VALUES (@suhu, @kelembaban, @cahaya, @device_id)
`);

const stmtLatest = db.prepare(`
  SELECT * FROM sensor_logs
  ORDER BY created_at DESC
  LIMIT 1
`);

const stmtHistory = db.prepare(`
  SELECT * FROM sensor_logs
  ORDER BY created_at DESC
  LIMIT ?
`);

const stmtStats = db.prepare(`
  SELECT
    ROUND(AVG(suhu), 2)       AS avg_suhu,
    ROUND(AVG(kelembaban), 2) AS avg_kelembaban,
    ROUND(MAX(suhu), 2)       AS max_suhu,
    ROUND(MIN(suhu), 2)       AS min_suhu,
    ROUND(MAX(kelembaban), 2) AS max_kelembaban,
    ROUND(MIN(kelembaban), 2) AS min_kelembaban,
    CAST(MAX(cahaya) AS INTEGER) AS max_cahaya,
    CAST(MIN(cahaya) AS INTEGER) AS min_cahaya,
    COUNT(*)                  AS count
  FROM sensor_logs
  WHERE created_at >= datetime('now', '-24 hours')
`);

const stmtDeleteOld = db.prepare(`
  DELETE FROM sensor_logs
  WHERE created_at < datetime('now', ? || ' days')
`);

// ── Exported functions ────────────────────────────────────────

/**
 * Insert a new sensor reading.
 * @param {{ suhu: number, kelembaban: number, cahaya: number, device_id?: string }} data
 * @returns {{ id: number }}
 */
function insertReading(data) {
  const info = stmtInsert.run({
    suhu:       data.suhu,
    kelembaban: data.kelembaban,
    cahaya:     data.cahaya,
    device_id:  data.device_id || 'esp32-01',
  });
  return { id: info.lastInsertRowid };
}

/**
 * Get the most recent sensor reading.
 * @returns {object|undefined}
 */
function getLatest() {
  return stmtLatest.get();
}

/**
 * Get sensor history (newest first).
 * @param {number} limit
 * @returns {object[]}
 */
function getHistory(limit = 50) {
  return stmtHistory.all(Math.max(1, Math.min(500, limit)));
}

/**
 * Get aggregate statistics for the last 24 hours.
 * @returns {object}
 */
function getStats() {
  return stmtStats.get();
}

/**
 * Delete sensor readings older than `days` days.
 * @param {number} days
 * @returns {{ deleted: number }}
 */
function deleteOld(days = 7) {
  const info = stmtDeleteOld.run(`-${days}`);
  return { deleted: info.changes };
}

module.exports = { insertReading, getLatest, getHistory, getStats, deleteOld };
