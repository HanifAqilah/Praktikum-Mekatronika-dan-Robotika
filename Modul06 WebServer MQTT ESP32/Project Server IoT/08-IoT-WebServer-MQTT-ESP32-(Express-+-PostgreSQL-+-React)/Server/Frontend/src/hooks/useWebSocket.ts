import { useEffect, useRef, useCallback } from "react";
import { useSystemStore } from "../store/systemStore";

const WS_URL =
  typeof window !== "undefined"
    ? `${window.location.protocol === "https:" ? "wss" : "ws"}://${window.location.host}/ws`
    : "ws://localhost:8008/ws";

const RECONNECT_DELAY_MS = 3_000;
const MAX_RECONNECT_DELAY_MS = 30_000;

export function useWebSocket() {
  const wsRef = useRef<WebSocket | null>(null);
  const reconnectDelayRef = useRef(RECONNECT_DELAY_MS);
  const reconnectTimerRef = useRef<ReturnType<typeof setTimeout> | null>(null);
  const unmountedRef = useRef(false);

  const {
    setWsConnected,
    setMqttConnected,
    setLatestSensor,
    handleAck,
  } = useSystemStore();

  const handleMessage = useCallback(
    (event: MessageEvent) => {
      try {
        const msg = JSON.parse(event.data as string);
        switch (msg.type) {
          case "sensor_data":
            setLatestSensor(msg.payload);
            break;
          case "mqtt_status":
            setMqttConnected(msg.payload?.connected ?? false);
            break;
          case "actuator_ack":
            handleAck(msg.payload);
            break;
          case "pong":
            // heartbeat OK
            break;
          default:
            break;
        }
      } catch {
        // ignore parse errors
      }
    },
    [setLatestSensor, setMqttConnected, handleAck]
  );

  const connect = useCallback(() => {
    if (unmountedRef.current) return;
    if (wsRef.current && wsRef.current.readyState === WebSocket.OPEN) return;

    const ws = new WebSocket(WS_URL);
    wsRef.current = ws;

    ws.onopen = () => {
      reconnectDelayRef.current = RECONNECT_DELAY_MS;
      setWsConnected(true);
    };

    ws.onmessage = handleMessage;

    ws.onclose = () => {
      setWsConnected(false);
      if (!unmountedRef.current) {
        reconnectTimerRef.current = setTimeout(() => {
          reconnectDelayRef.current = Math.min(
            reconnectDelayRef.current * 2,
            MAX_RECONNECT_DELAY_MS
          );
          connect();
        }, reconnectDelayRef.current);
      }
    };

    ws.onerror = () => {
      ws.close();
    };
  }, [handleMessage, setWsConnected]);

  // Send heartbeat ping every 30 s
  useEffect(() => {
    const interval = setInterval(() => {
      if (wsRef.current?.readyState === WebSocket.OPEN) {
        wsRef.current.send(JSON.stringify({ type: "ping" }));
      }
    }, 30_000);
    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    unmountedRef.current = false;
    connect();
    return () => {
      unmountedRef.current = true;
      if (reconnectTimerRef.current) clearTimeout(reconnectTimerRef.current);
      wsRef.current?.close();
    };
  }, [connect]);
}
