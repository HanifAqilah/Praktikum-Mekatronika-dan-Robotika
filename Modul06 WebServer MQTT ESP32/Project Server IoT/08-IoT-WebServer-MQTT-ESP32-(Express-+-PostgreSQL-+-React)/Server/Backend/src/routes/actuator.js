import { Router } from "express";
import { publishCommand, getMqttClient } from "../mqtt/client.js";
import { insertCommandLog } from "../database/postgres.js";
import { pool } from "../database/postgres.js";

const router = Router();

const VALID_STATES = new Set(["on", "off", "toggle"]);

function validateActuatorBody(body) {
  if (!body || typeof body !== "object") return "Request body must be a JSON object";
  if (!VALID_STATES.has(body.state)) return `state must be one of: ${[...VALID_STATES].join(", ")}`;
  return null;
}

/**
 * POST /api/actuator/led
 * Body: { "led": 1|2|3, "state": "on"|"off"|"toggle" }
 * Publishes to esp32/command/led1 | led2 | led3
 */
router.post("/led", async (req, res) => {
  const { led, state } = req.body || {};

  const error = validateActuatorBody(req.body);
  if (error) return res.status(400).json({ error });

  const ledNum = parseInt(led);
  if (![1, 2, 3].includes(ledNum)) {
    return res.status(400).json({ error: "led must be 1, 2, or 3" });
  }

  const topic = `esp32/command/led${ledNum}`;
  const payload = { led: ledNum, state, ts: Date.now() };

  await publishCommand(topic, payload);
  const logRow = await insertCommandLog({
    command_type: "led",
    target: `led${ledNum}`,
    payload,
    status: "sent",
  });

  res.json({ success: true, topic, payload, log_id: logRow.id });
});

/**
 * POST /api/actuator/relay
 * Body: { "state": "on"|"off"|"toggle" }
 * Publishes to esp32/command/relay
 */
router.post("/relay", async (req, res) => {
  const error = validateActuatorBody(req.body);
  if (error) return res.status(400).json({ error });

  const { state } = req.body;
  const topic = "esp32/command/relay";
  const payload = { state, ts: Date.now() };

  await publishCommand(topic, payload);
  const logRow = await insertCommandLog({
    command_type: "relay",
    target: "relay",
    payload,
    status: "sent",
  });

  res.json({ success: true, topic, payload, log_id: logRow.id });
});

/**
 * POST /api/actuator/all
 * Body: { "state": "on"|"off" }
 * Publishes to esp32/command/all (turns all actuators on/off)
 */
router.post("/all", async (req, res) => {
  const error = validateActuatorBody(req.body);
  if (error) return res.status(400).json({ error });

  const { state } = req.body;
  const topic = "esp32/command/all";
  const payload = { state, ts: Date.now() };

  await publishCommand(topic, payload);
  const logRow = await insertCommandLog({
    command_type: "all",
    target: "all",
    payload,
    status: "sent",
  });

  res.json({ success: true, topic, payload, log_id: logRow.id });
});

/**
 * GET /api/commands/log
 * Returns command history with cursor-based pagination
 */
router.get("/log", async (req, res) => {
  const limit = Math.min(Math.max(parseInt(req.query.limit) || 20, 1), 100);
  const cursor = req.query.cursor || null;

  const conditions = [];
  const values = [];

  if (cursor) {
    values.push(cursor);
    conditions.push(`created_at < $${values.length}`);
  }

  const where = conditions.length > 0 ? `WHERE ${conditions.join(" AND ")}` : "";
  values.push(limit + 1);

  const sql = `
    SELECT id, command_type, target, payload, status, created_at
    FROM command_logs
    ${where}
    ORDER BY created_at DESC
    LIMIT $${values.length}
  `;

  const result = await pool.query(sql, values);
  const rows = result.rows;
  const hasMore = rows.length > limit;
  const data = hasMore ? rows.slice(0, limit) : rows;
  const nextCursor = hasMore ? data[data.length - 1].created_at.toISOString() : null;

  res.json({ data, pagination: { limit, hasMore, nextCursor } });
});

export default router;
