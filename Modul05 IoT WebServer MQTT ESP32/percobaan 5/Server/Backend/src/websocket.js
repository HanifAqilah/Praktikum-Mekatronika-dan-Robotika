const { WebSocketServer, OPEN } = require('ws');
const db = require('./database');

let wss;
const clients = new Set();

function createWsServer(httpServer) {
  wss = new WebSocketServer({ server: httpServer, path: '/ws' });

  wss.on('connection', (ws, req) => {
    ws.isAlive = true;
    clients.add(ws);

    console.log(`[WS] Client connected from ${req.socket.remoteAddress}. Total: ${clients.size}`);

    // Send current latest data immediately so new clients see something right away
    const latest = db.getLatest();
    if (latest) {
      safeSend(ws, JSON.stringify({ type: 'sensor', data: latest }));
    }

    ws.on('pong', () => {
      ws.isAlive = true;
    });

    ws.on('message', (raw) => {
      const msg = raw.toString();
      if (msg === 'ping') {
        safeSend(ws, 'pong');
      }
    });

    ws.on('close', () => {
      clients.delete(ws);
      console.log(`[WS] Client disconnected. Total: ${clients.size}`);
    });

    ws.on('error', (err) => {
      console.error('[WS] Client error:', err.message);
      clients.delete(ws);
    });
  });

  // Heartbeat: ping every 30 s, remove dead connections
  const heartbeat = setInterval(() => {
    for (const ws of clients) {
      if (!ws.isAlive) {
        clients.delete(ws);
        ws.terminate();
        continue;
      }
      ws.isAlive = false;
      ws.ping();
    }
  }, 30_000);

  wss.on('close', () => clearInterval(heartbeat));

  console.log('[WS] WebSocket server attached at path /ws');
  return wss;
}

function safeSend(ws, data) {
  if (ws.readyState === OPEN) {
    ws.send(data);
  }
}

function broadcast(data) {
  const payload = typeof data === 'string' ? data : JSON.stringify(data);
  for (const ws of clients) {
    safeSend(ws, payload);
  }
}

function getClientCount() {
  return clients.size;
}

function closeWsServer() {
  if (wss) {
    wss.close();
  }
}

module.exports = { createWsServer, broadcast, getClientCount, closeWsServer, clients };
