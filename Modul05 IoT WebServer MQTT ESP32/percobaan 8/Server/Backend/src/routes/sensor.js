import { Router } from "express";
import { pool } from "../database/postgres.js";

const router = Router();

/**
 * GET /api/sensor/history
 * Query params:
 *   cursor  - ISO timestamp or ID for cursor-based pagination (optional)
 *   limit   - number of records to return (default 20, max 100)
 *   device  - filter by device_id (optional)
 */
router.get("/history", async (req, res) => {
  const limit = Math.min(Math.max(parseInt(req.query.limit) || 20, 1), 100);
  const cursor = req.query.cursor || null;
  const device = req.query.device || null;

  const conditions = [];
  const values = [];

  if (cursor) {
    values.push(cursor);
    conditions.push(`created_at < $${values.length}`);
  }

  if (device) {
    values.push(device);
    conditions.push(`device_id = $${values.length}`);
  }

  const where = conditions.length > 0 ? `WHERE ${conditions.join(" AND ")}` : "";
  values.push(limit + 1);

  const sql = `
    SELECT id, device_id, suhu, kelembaban, cahaya, tanah, created_at
    FROM sensor_logs
    ${where}
    ORDER BY created_at DESC
    LIMIT $${values.length}
  `;

  const result = await pool.query(sql, values);
  const rows = result.rows;
  const hasMore = rows.length > limit;
  const data = hasMore ? rows.slice(0, limit) : rows;
  const nextCursor = hasMore ? data[data.length - 1].created_at.toISOString() : null;

  res.json({
    data,
    pagination: {
      limit,
      hasMore,
      nextCursor,
    },
  });
});

/**
 * GET /api/sensor/stats
 * Returns aggregated stats for the last N minutes (default 60).
 */
router.get("/stats", async (req, res) => {
  const minutes = Math.min(Math.max(parseInt(req.query.minutes) || 60, 1), 1440);

  const result = await pool.query(
    `SELECT
       COUNT(*)::int                          AS total_readings,
       ROUND(AVG(suhu)::numeric, 2)           AS avg_suhu,
       ROUND(MIN(suhu)::numeric, 2)           AS min_suhu,
       ROUND(MAX(suhu)::numeric, 2)           AS max_suhu,
       ROUND(AVG(kelembaban)::numeric, 2)     AS avg_kelembaban,
       ROUND(AVG(cahaya)::numeric, 0)::int    AS avg_cahaya,
       ROUND(AVG(tanah)::numeric, 0)::int     AS avg_tanah
     FROM sensor_logs
     WHERE created_at >= NOW() - INTERVAL '1 minute' * $1`,
    [minutes]
  );

  res.json({ minutes, stats: result.rows[0] });
});

/**
 * GET /api/sensor/latest
 * Returns the most recent sensor reading.
 */
router.get("/latest", async (_req, res) => {
  const result = await pool.query(
    `SELECT id, device_id, suhu, kelembaban, cahaya, tanah, created_at
     FROM sensor_logs
     ORDER BY created_at DESC
     LIMIT 1`
  );

  if (result.rows.length === 0) {
    return res.status(404).json({ error: "No sensor data available" });
  }

  res.json(result.rows[0]);
});

export default router;
