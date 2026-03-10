import "dotenv/config";
import express from "express";
import cors from "cors";
import morgan from "morgan";
import http from "http";
import { runSchema, closePgPool } from "./database/postgres.js";
import { initMqttClient, closeMqttClient, mqttStatus } from "./mqtt/client.js";
import { attachWebSocketServer, closeWebSocketServer } from "./websocket/broadcast.js";
import sensorRoutes from "./routes/sensor.js";
import actuatorRoutes from "./routes/actuator.js";

const PORT = Number(process.env.PORT) || 8008;

const app = express();

app.use(cors({ origin: "*" }));
app.use(morgan("dev"));
app.use(express.json());

// Health / status
app.get("/api/status", (_req, res) => {
  res.json({
    service: "percobaan8-backend",
    version: "1.0.0",
    mqtt: mqttStatus,
    uptime: process.uptime(),
    timestamp: new Date().toISOString(),
  });
});

// Routes
app.use("/api/sensor", sensorRoutes);
app.use("/api/actuator", actuatorRoutes);
// Commands log lives under actuator router but also mapped here for convenience
app.use("/api/commands", actuatorRoutes);

// 404 handler
app.use((_req, res) => {
  res.status(404).json({ error: "Not found" });
});

// Global error handler (Express 5 supports async errors natively)
app.use((err, _req, res, _next) => {
  console.error("[express] Unhandled error:", err.message);
  res.status(500).json({ error: err.message || "Internal server error" });
});

const server = http.createServer(app);

// Attach WebSocket server
attachWebSocketServer(server, process.env.WS_PATH || "/ws");

async function start() {
  try {
    // Run schema migrations first
    await runSchema();

    // Connect MQTT
    initMqttClient();

    // Start HTTP + WS server
    server.listen(PORT, "0.0.0.0", () => {
      console.log(`[server] HTTP listening on http://0.0.0.0:${PORT}`);
      console.log(`[server] WebSocket on ws://0.0.0.0:${PORT}/ws`);
    });
  } catch (err) {
    console.error("[server] Startup failed:", err.message);
    process.exit(1);
  }
}

// Graceful shutdown
async function shutdown(signal) {
  console.log(`\n[server] Received ${signal}, shutting down...`);
  server.close(async () => {
    await Promise.allSettled([
      closeMqttClient(),
      closeWebSocketServer(),
      closePgPool(),
    ]);
    console.log("[server] Graceful shutdown complete");
    process.exit(0);
  });

  // Force exit after 10 s
  setTimeout(() => {
    console.error("[server] Forced shutdown");
    process.exit(1);
  }, 10_000).unref();
}

process.on("SIGTERM", () => shutdown("SIGTERM"));
process.on("SIGINT", () => shutdown("SIGINT"));

start();
