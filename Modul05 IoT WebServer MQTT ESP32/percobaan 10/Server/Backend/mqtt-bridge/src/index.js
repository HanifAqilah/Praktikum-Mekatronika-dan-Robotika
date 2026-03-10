'use strict';
require('dotenv').config();

const express = require('express');
const { startMqttClient } = require('./mqtt/client');
const { createWsServer } = require('./websocket/server');

const app = express();
const PORT = parseInt(process.env.WS_PORT || '3010', 10);

app.use(express.json());

app.get('/health', (_req, res) => {
  res.json({ status: 'ok', ts: new Date().toISOString() });
});

const server = app.listen(PORT, () => {
  console.log(`[bridge] HTTP/WS server listening on port ${PORT}`);
});

const { broadcast } = createWsServer(server);
startMqttClient(broadcast);
