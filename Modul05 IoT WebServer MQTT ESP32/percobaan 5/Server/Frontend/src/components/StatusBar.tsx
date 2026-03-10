import { useEffect, useState } from 'react';
import axios from 'axios';
import { useSensorStore } from '../store/sensorStore';

interface StatusResponse {
  uptime: number;
  clients: number;
  db_ok: boolean;
}

function formatUptime(seconds: number): string {
  const h = Math.floor(seconds / 3600);
  const m = Math.floor((seconds % 3600) / 60);
  const s = Math.floor(seconds % 60);
  return `${h}h ${m}m ${s}s`;
}

export default function StatusBar() {
  const wsConnected = useSensorStore((s) => s.wsConnected);
  const [serverStatus, setServerStatus] = useState<StatusResponse | null>(null);

  useEffect(() => {
    const fetch = async () => {
      try {
        const res = await axios.get<StatusResponse>('/api/status');
        setServerStatus(res.data);
      } catch {
        setServerStatus(null);
      }
    };
    fetch();
    const interval = setInterval(fetch, 5_000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div
      style={{
        background: '#0f1120',
        border: '1px solid #1f2937',
        borderRadius: 10,
        padding: '10px 16px',
        display: 'flex',
        alignItems: 'center',
        flexWrap: 'wrap',
        gap: 16,
        fontSize: 13,
      }}
    >
      {/* WS connection badge */}
      <div style={{ display: 'flex', alignItems: 'center', gap: 6 }}>
        {wsConnected ? (
          <>
            <span
              style={{
                display: 'inline-block',
                width: 8,
                height: 8,
                borderRadius: '50%',
                background: '#22c55e',
                animation: 'blink 1.2s infinite',
              }}
            />
            <span style={{ color: '#22c55e', fontWeight: 600 }}>LIVE</span>
            <span style={{ color: '#6b7280' }}>WebSocket Terhubung</span>
          </>
        ) : (
          <>
            <span
              style={{
                display: 'inline-block',
                width: 8,
                height: 8,
                borderRadius: '50%',
                background: '#ef4444',
              }}
            />
            <span style={{ color: '#ef4444', fontWeight: 600 }}>OFFLINE</span>
            <span style={{ color: '#6b7280' }}>Mencoba menghubungkan…</span>
          </>
        )}
      </div>

      {/* Divider */}
      <div style={{ width: 1, height: 16, background: '#374151' }} />

      {/* Server status */}
      {serverStatus ? (
        <>
          <span style={{ color: '#9ca3af' }}>
            🖥️ Klien:&nbsp;
            <span style={{ color: '#e5e7eb', fontWeight: 600 }}>{serverStatus.clients}</span>
          </span>
          <span style={{ color: '#9ca3af' }}>
            ⏱ Uptime:&nbsp;
            <span style={{ color: '#e5e7eb' }}>{formatUptime(serverStatus.uptime)}</span>
          </span>
          <span style={{ color: serverStatus.db_ok ? '#22c55e' : '#ef4444' }}>
            💾 DB: {serverStatus.db_ok ? 'OK' : 'ERROR'}
          </span>
        </>
      ) : (
        <span style={{ color: '#6b7280' }}>Server tidak tersedia</span>
      )}

      {/* Global blink keyframe */}
      <style>{`@keyframes blink { 0%,100%{opacity:1} 50%{opacity:0.3} }`}</style>
    </div>
  );
}
