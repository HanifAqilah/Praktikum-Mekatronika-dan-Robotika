import { useEffect, useRef } from 'react';
import toast from 'react-hot-toast';
import { useSensorStore } from '../store/sensorStore';
import { useAlertStore } from '../store/alertStore';

const WS_URL = `ws://${window.location.hostname}:3010/ws`;

export function useWebSocket() {
  const wsRef = useRef<WebSocket | null>(null);
  const reconnectTimer = useRef<ReturnType<typeof setTimeout> | null>(null);
  const delay = useRef(1000);

  const setReading = useSensorStore((s) => s.setReading);
  const setWsConnected = useSensorStore((s) => s.setWsConnected);
  const addAlert = useAlertStore((s) => s.addAlert);

  const connect = () => {
    const ws = new WebSocket(WS_URL);
    wsRef.current = ws;

    ws.onopen = () => {
      setWsConnected(true);
      delay.current = 1000;
    };

    ws.onmessage = (evt) => {
      try {
        const msg = JSON.parse(evt.data);
        if (msg.type === 'sensor') setReading(msg.data);
        if (msg.type === 'alert') {
          addAlert(msg.data);
          toast.error(`🚨 Alert: ${msg.data.field} ${msg.data.condition} ${msg.data.value}`, { duration: 5000 });
        }
      } catch {}
    };

    ws.onclose = () => {
      setWsConnected(false);
      reconnectTimer.current = setTimeout(() => {
        delay.current = Math.min(delay.current * 2, 30000);
        connect();
      }, delay.current);
    };

    ws.onerror = () => ws.close();
  };

  useEffect(() => {
    connect();
    return () => {
      if (reconnectTimer.current) clearTimeout(reconnectTimer.current);
      wsRef.current?.close();
    };
  }, []);
}
