import pg from "pg";
import { readFileSync } from "fs";
import { fileURLToPath } from "url";
import { dirname, join } from "path";
import "dotenv/config";

const { Pool } = pg;

const __dirname = dirname(fileURLToPath(import.meta.url));

export const pool = new Pool({
  host: process.env.POSTGRES_HOST || "localhost",
  port: Number(process.env.POSTGRES_PORT) || 5432,
  user: process.env.POSTGRES_USER || "p8user",
  password: process.env.POSTGRES_PASSWORD || "p8pass",
  database: process.env.POSTGRES_DB || "percobaan8",
  max: 10,
  idleTimeoutMillis: 30_000,
  connectionTimeoutMillis: 5_000,
});

pool.on("error", (err) => {
  console.error("[postgres] Unexpected pool error:", err.message);
});

export async function runSchema() {
  const schemaPath = join(__dirname, "schema.sql");
  const sql = readFileSync(schemaPath, "utf8");
  const client = await pool.connect();
  try {
    await client.query(sql);
    console.log("[postgres] Schema applied successfully");
  } finally {
    client.release();
  }
}

export async function insertSensorLog(data) {
  const { device_id = "esp32-p8", suhu, kelembaban, cahaya, tanah, raw_payload } = data;
  const result = await pool.query(
    `INSERT INTO sensor_logs (device_id, suhu, kelembaban, cahaya, tanah, raw_payload)
     VALUES ($1, $2, $3, $4, $5, $6)
     RETURNING *`,
    [device_id, suhu ?? null, kelembaban ?? null, cahaya ?? null, tanah ?? null, raw_payload ?? null]
  );
  return result.rows[0];
}

export async function insertCommandLog(data) {
  const { command_type, target, payload, status = "sent" } = data;
  const result = await pool.query(
    `INSERT INTO command_logs (command_type, target, payload, status)
     VALUES ($1, $2, $3, $4)
     RETURNING *`,
    [command_type, target, JSON.stringify(payload), status]
  );
  return result.rows[0];
}

export async function closePgPool() {
  await pool.end();
  console.log("[postgres] Pool closed");
}
