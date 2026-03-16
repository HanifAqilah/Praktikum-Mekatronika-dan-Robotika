'use strict';
const mqtt = require('mqtt');
const { createClient } = require('redis');

const BROKER = process.env.MQTT_BROKER || 'mosquitto';
const PORT   = parseInt(process.env.MQTT_PORT || '1883', 10);
const USER   = process.env.MQTT_USERNAME || '';
const PASS   = process.env.MQTT_PASSWORD || '';
const CLIENT_ID = process.env.MQTT_CLIENT_ID || 'mqtt-bridge';
const REDIS_URL = process.env.REDIS_URL || 'redis://redis:6379/0';

const redis = createClient({ url: REDIS_URL });
redis.connect().catch(err => console.error('[redis] connect error', err));

function startMqttClient(broadcast) {
  const options = { clientId: CLIENT_ID };
  if (USER) { options.username = USER; options.password = PASS; }

  const client = mqtt.connect(`mqtt://${BROKER}:${PORT}`, options);

  client.on('connect', () => {
    console.log('[mqtt] connected to', BROKER);
    client.subscribe('greenhouse/#', err => {
      if (err) console.error('[mqtt] subscribe error', err);
      else console.log('[mqtt] subscribed to greenhouse/#');
    });
  });

  client.on('message', async (topic, payload) => {
    let data;
    try { data = JSON.parse(payload.toString()); } catch { return; }

    const parts = topic.split('/');
    if (parts.length < 3) return;

    const deviceId = parts[1];
    const msgType  = parts[2];

    if (msgType === 'sensors') {
      const key = `rt:sensor:${deviceId}`;
      await redis.set(key, JSON.stringify({ ...data, device_id: deviceId }), { EX: 5 });
      broadcast({ type: 'sensor', data: { device_id: deviceId, ...data }, ts: new Date().toISOString() });
    } else if (msgType === 'status') {
      broadcast({ type: 'status', data: { device_id: deviceId, ...data }, ts: new Date().toISOString() });
    } else if (msgType === 'alert') {
      broadcast({ type: 'alert', data: { device_id: deviceId, ...data }, ts: new Date().toISOString() });
    }
  });

  client.on('error', err => console.error('[mqtt] error', err));
  client.on('reconnect', () => console.log('[mqtt] reconnecting...'));
}

module.exports = { startMqttClient };
