const Database = require('better-sqlite3');
const path = require('path');
require('dotenv').config();

const DB_FILE = process.env.DB_FILE || 'sensor.db';
const dbPath = path.resolve(process.cwd(), DB_FILE);

let db;

function getDb() {
  if (!db) {
    db = new Database(dbPath);
    db.pragma('journal_mode = WAL');
    db.pragma('foreign_keys = ON');
    initSchema();
  }
  return db;
}

function initSchema() {
  db.exec(`
    CREATE TABLE IF NOT EXISTS sensor_readings (
      id          INTEGER PRIMARY KEY AUTOINCREMENT,
      device_id   TEXT    NOT NULL,
      suhu        REAL,
      kelembaban  REAL,
      cahaya      INTEGER,
      created_at  DATETIME DEFAULT CURRENT_TIMESTAMP
    );
    CREATE INDEX IF NOT EXISTS idx_sensor_created_at ON sensor_readings(created_at);
    CREATE INDEX IF NOT EXISTS idx_sensor_device_id  ON sensor_readings(device_id);
  `);
}

function insert(data) {
  const { device_id, suhu, kelembaban, cahaya } = data;
  const stmt = getDb().prepare(
    'INSERT INTO sensor_readings (device_id, suhu, kelembaban, cahaya) VALUES (?, ?, ?, ?)'
  );
  const result = stmt.run(device_id, suhu, kelembaban, cahaya);
  return result.lastInsertRowid;
}

function getLatest() {
  return getDb()
    .prepare('SELECT * FROM sensor_readings ORDER BY id DESC LIMIT 1')
    .get() || null;
}

function getHistory(limit = 100) {
  const safe = Math.min(Math.max(parseInt(limit) || 100, 1), 500);
  return getDb()
    .prepare('SELECT * FROM sensor_readings ORDER BY id DESC LIMIT ?')
    .all(safe)
    .reverse();
}

function getStats() {
  return getDb()
    .prepare(`
      SELECT
        COUNT(*)           AS count,
        ROUND(AVG(suhu), 2)        AS avg_suhu,
        ROUND(AVG(kelembaban), 2)  AS avg_kelembaban,
        ROUND(AVG(cahaya), 2)      AS avg_cahaya,
        ROUND(MAX(suhu), 2)        AS max_suhu,
        ROUND(MIN(suhu), 2)        AS min_suhu
      FROM sensor_readings
    `)
    .get();
}

function pruneOld(days = 30) {
  const stmt = getDb().prepare(
    "DELETE FROM sensor_readings WHERE created_at < datetime('now', ?)"
  );
  const result = stmt.run(`-${days} days`);
  return result.changes;
}

function closeDb() {
  if (db) {
    db.close();
    db = null;
  }
}

module.exports = { insert, getLatest, getHistory, getStats, pruneOld, closeDb };
