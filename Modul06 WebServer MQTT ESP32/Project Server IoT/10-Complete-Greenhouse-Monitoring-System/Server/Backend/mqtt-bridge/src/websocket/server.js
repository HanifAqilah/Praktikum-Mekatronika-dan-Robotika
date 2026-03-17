'use strict';
const { WebSocketServer, OPEN } = require('ws');

const HEARTBEAT_INTERVAL = 30_000;

function createWsServer(httpServer) {
  const wss = new WebSocketServer({ server: httpServer });
  const clients = new Set();

  wss.on('connection', (ws) => {
    ws.isAlive = true;
    clients.add(ws);
    console.log('[ws] client connected, total:', clients.size);

    ws.on('pong', () => { ws.isAlive = true; });

    ws.on('close', () => {
      clients.delete(ws);
      console.log('[ws] client disconnected, total:', clients.size);
    });

    ws.on('error', err => console.error('[ws] client error', err));
  });

  const heartbeat = setInterval(() => {
    for (const ws of clients) {
      if (!ws.isAlive) {
        ws.terminate();
        clients.delete(ws);
        continue;
      }
      ws.isAlive = false;
      ws.ping();
    }
  }, HEARTBEAT_INTERVAL);

  wss.on('close', () => clearInterval(heartbeat));

  /**
   * Broadcast a message to all connected WebSocket clients.
   * @param {{ type: 'sensor'|'alert'|'status', data: object, ts: string }} message
   */
  function broadcast(message) {
    const payload = JSON.stringify(message);
    for (const ws of clients) {
      if (ws.readyState === OPEN) {
        ws.send(payload, err => { if (err) console.error('[ws] send error', err); });
      }
    }
  }

  return { wss, broadcast };
}

module.exports = { createWsServer };
