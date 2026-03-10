import mqtt from "mqtt";
import "dotenv/config";
import { insertSensorLog } from "../database/postgres.js";
import { broadcast } from "../websocket/broadcast.js";

const BROKER_URL = process.env.MQTT_BROKER || "mqtt://localhost:1883";
const CLIENT_ID = process.env.MQTT_CLIENT_ID || `percobaan8-backend-${Date.now()}`;

const connectOptions = {
  clientId: CLIENT_ID,
  clean: true,
  reconnectPeriod: 5_000,
  connectTimeout: 10_000,
  ...(process.env.MQTT_USERNAME && { username: process.env.MQTT_USERNAME }),
  ...(process.env.MQTT_PASSWORD && { password: process.env.MQTT_PASSWORD }),
};

/** @type {import("mqtt").MqttClient | null} */
let mqttClient = null;

/** Live status exposed to the status endpoint */
export const mqttStatus = { connected: false };

export function getMqttClient() {
  return mqttClient;
}

export function initMqttClient() {
  mqttClient = mqtt.connect(BROKER_URL, connectOptions);

  mqttClient.on("connect", () => {
    mqttStatus.connected = true;
    console.log(`[mqtt] Connected to ${BROKER_URL} as ${CLIENT_ID}`);

    // Subscribe to all sensor topics from ESP32
    mqttClient.subscribe("esp32/sensor/#", { qos: 1 }, (err) => {
      if (err) console.error("[mqtt] Subscribe sensor error:", err.message);
      else console.log("[mqtt] Subscribed to esp32/sensor/#");
    });

    // Subscribe to all ack topics from ESP32
    mqttClient.subscribe("esp32/ack/#", { qos: 0 }, (err) => {
      if (err) console.error("[mqtt] Subscribe ack error:", err.message);
      else console.log("[mqtt] Subscribed to esp32/ack/#");
    });

    broadcast("mqtt_status", { connected: true });
  });

  mqttClient.on("reconnect", () => {
    mqttStatus.connected = false;
    console.log("[mqtt] Reconnecting...");
    broadcast("mqtt_status", { connected: false });
  });

  mqttClient.on("offline", () => {
    mqttStatus.connected = false;
    console.log("[mqtt] Offline");
    broadcast("mqtt_status", { connected: false });
  });

  mqttClient.on("error", (err) => {
    console.error("[mqtt] Error:", err.message);
  });

  mqttClient.on("message", async (topic, messageBuffer) => {
    const raw = messageBuffer.toString();

    try {
      const parsed = JSON.parse(raw);

      if (topic.startsWith("esp32/sensor/")) {
        await handleSensorMessage(topic, parsed, raw);
      } else if (topic.startsWith("esp32/ack/")) {
        handleAckMessage(topic, parsed);
      }
    } catch (err) {
      console.error(`[mqtt] Failed to handle message on ${topic}:`, err.message);
    }
  });

  return mqttClient;
}

async function handleSensorMessage(topic, parsed, rawStr) {
  const device_id = parsed.device_id || topic.split("/").pop() || "esp32-p8";

  const row = await insertSensorLog({
    device_id,
    suhu: parsed.suhu ?? parsed.temperature ?? null,
    kelembaban: parsed.kelembaban ?? parsed.humidity ?? null,
    cahaya: parsed.cahaya ?? parsed.light ?? null,
    tanah: parsed.tanah ?? parsed.soil ?? null,
    raw_payload: parsed,
  });

  broadcast("sensor_data", row);
  console.log(`[mqtt] Sensor data saved id=${row.id} device=${device_id}`);
}

function handleAckMessage(topic, parsed) {
  const actuator = topic.split("/").pop();
  console.log(`[mqtt] ACK from ${actuator}:`, JSON.stringify(parsed));
  broadcast("actuator_ack", { actuator, ...parsed, ts: Date.now() });
}

/**
 * Publish a command to an MQTT topic.
 * @param {string} topic
 * @param {object} payload
 * @returns {Promise<void>}
 */
export function publishCommand(topic, payload) {
  return new Promise((resolve, reject) => {
    if (!mqttClient || !mqttClient.connected) {
      return reject(new Error("MQTT client not connected"));
    }
    mqttClient.publish(topic, JSON.stringify(payload), { qos: 1, retain: false }, (err) => {
      if (err) reject(err);
      else resolve();
    });
  });
}

export async function closeMqttClient() {
  if (!mqttClient) return;
  await new Promise((resolve) => mqttClient.end(false, {}, resolve));
  console.log("[mqtt] Client closed");
}
