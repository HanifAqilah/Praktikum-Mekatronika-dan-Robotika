import { WebSocketServer, WebSocket } from "ws";

/** @type {WebSocketServer | null} */
let wss = null;

/**
 * Attach a WebSocket server to an existing HTTP server.
 * @param {import("http").Server} httpServer
 * @param {string} path
 */
export function attachWebSocketServer(httpServer, path = "/ws") {
  wss = new WebSocketServer({ server: httpServer, path });

  wss.on("connection", (ws, req) => {
    const ip = req.socket.remoteAddress;
    console.log(`[ws] Client connected from ${ip}. Total: ${wss.clients.size}`);

    ws.send(JSON.stringify({ type: "welcome", message: "Percobaan 8 WebSocket connected" }));

    ws.on("message", (raw) => {
      try {
        const msg = JSON.parse(raw.toString());
        if (msg.type === "ping") {
          ws.send(JSON.stringify({ type: "pong", ts: Date.now() }));
        }
      } catch {
        // ignore malformed messages from clients
      }
    });

    ws.on("close", () => {
      console.log(`[ws] Client disconnected. Remaining: ${wss.clients.size}`);
    });

    ws.on("error", (err) => {
      console.error("[ws] Client error:", err.message);
    });
  });

  wss.on("error", (err) => {
    console.error("[ws] Server error:", err.message);
  });

  console.log(`[ws] WebSocket server listening on ${path}`);
  return wss;
}

/**
 * Broadcast a typed message to all connected WebSocket clients.
 * @param {string} type
 * @param {object} payload
 */
export function broadcast(type, payload) {
  if (!wss) return;
  const message = JSON.stringify({ type, payload, ts: Date.now() });
  for (const client of wss.clients) {
    if (client.readyState === WebSocket.OPEN) {
      client.send(message);
    }
  }
}

/**
 * Close all WebSocket connections gracefully.
 */
export async function closeWebSocketServer() {
  if (!wss) return;
  for (const client of wss.clients) {
    client.terminate();
  }
  await new Promise((resolve) => wss.close(resolve));
  console.log("[ws] WebSocket server closed");
}
