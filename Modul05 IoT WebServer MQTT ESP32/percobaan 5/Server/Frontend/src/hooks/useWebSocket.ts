import { useEffect, useRef, useCallback } from 'react';
import { useSensorStore } from '../store/sensorStore';

const WS_URL =
  typeof window !== 'undefined'
    ? `${window.location.protocol === 'https:' ? 'wss' : 'ws'}://${window.location.host}/ws`
    : 'ws://localhost:8005/ws';

const MIN_RECONNECT_MS = 1_000;
const MAX_RECONNECT_MS = 30_000;

export function useWebSocket() {
  const wsRef = useRef<WebSocket | null>(null);
  const reconnectTimer = useRef<ReturnType<typeof setTimeout> | null>(null);
  const reconnectDelay = useRef(MIN_RECONNECT_MS);
  const unmounted = useRef(false);
  const pingTimer = useRef<ReturnType<typeof setInterval> | null>(null);

  const { addReading, setLedState, setWsConnected } = useSensorStore();

  const clearPing = () => {
    if (pingTimer.current) {
      clearInterval(pingTimer.current);
      pingTimer.current = null;
    }
  };

  const startPing = (ws: WebSocket) => {
    clearPing();
    pingTimer.current = setInterval(() => {
      if (ws.readyState === WebSocket.OPEN) {
        ws.send('ping');
      }
    }, 25_000);
  };

  const connect = useCallback(() => {
    if (unmounted.current) return;

    const ws = new WebSocket(WS_URL);
    wsRef.current = ws;

    ws.onopen = () => {
      if (unmounted.current) { ws.close(); return; }
      console.log('[WS] Connected to', WS_URL);
      reconnectDelay.current = MIN_RECONNECT_MS;
      setWsConnected(true);
      startPing(ws);
    };

    ws.onmessage = (event) => {
      const raw = event.data as string;

      // Server pong response
      if (raw === 'pong') return;

      try {
        const msg = JSON.parse(raw) as { type: string; data?: unknown; state?: boolean };

        if (msg.type === 'sensor' && msg.data) {
          addReading(msg.data as Parameters<typeof addReading>[0]);
        } else if (msg.type === 'led' && typeof msg.state === 'boolean') {
          setLedState(msg.state);
        }
      } catch {
        console.warn('[WS] Unparseable message:', raw);
      }
    };

    ws.onerror = (err) => {
      console.error('[WS] Error', err);
    };

    ws.onclose = () => {
      clearPing();
      setWsConnected(false);
      wsRef.current = null;

      if (unmounted.current) return;

      console.log(`[WS] Disconnected. Reconnecting in ${reconnectDelay.current}ms…`);
      reconnectTimer.current = setTimeout(() => {
        connect();
      }, reconnectDelay.current);

      // Exponential backoff
      reconnectDelay.current = Math.min(reconnectDelay.current * 2, MAX_RECONNECT_MS);
    };
  }, [addReading, setLedState, setWsConnected]);

  useEffect(() => {
    unmounted.current = false;
    connect();

    return () => {
      unmounted.current = true;
      clearPing();
      if (reconnectTimer.current) clearTimeout(reconnectTimer.current);
      if (wsRef.current) {
        wsRef.current.close();
        wsRef.current = null;
      }
    };
  }, [connect]);

  return { wsRef };
}
