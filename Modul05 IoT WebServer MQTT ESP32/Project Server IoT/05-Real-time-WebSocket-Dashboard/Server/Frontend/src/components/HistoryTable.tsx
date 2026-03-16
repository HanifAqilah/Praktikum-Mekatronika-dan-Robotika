import { useSensorStore } from '../store/sensorStore';

function formatTime(dateStr: string): string {
  return new Date(dateStr).toLocaleString('id-ID', {
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
  });
}

const th: React.CSSProperties = {
  padding: '10px 12px',
  textAlign: 'left',
  color: '#6b7280',
  fontWeight: 600,
  fontSize: 12,
  textTransform: 'uppercase',
  borderBottom: '1px solid #374151',
  whiteSpace: 'nowrap',
};

const td: React.CSSProperties = {
  padding: '9px 12px',
  color: '#d1d5db',
  fontSize: 13,
  borderBottom: '1px solid #1f2937',
  whiteSpace: 'nowrap',
};

export default function HistoryTable() {
  const readings = useSensorStore((s) => s.readings);
  const last20 = [...readings].reverse().slice(0, 20);

  return (
    <div
      style={{
        background: '#1a1d2e',
        border: '1px solid #2d3148',
        borderRadius: 12,
        overflow: 'hidden',
      }}
    >
      <p style={{ color: '#e5e7eb', fontWeight: 600, margin: 0, padding: '14px 16px', fontSize: 14 }}>
        🗂️ Riwayat Pembacaan (20 terbaru)
      </p>
      <div style={{ overflowX: 'auto' }}>
        <table style={{ width: '100%', borderCollapse: 'collapse' }}>
          <thead>
            <tr>
              <th style={th}>Waktu</th>
              <th style={th}>Device</th>
              <th style={{ ...th, color: '#ef4444' }}>Suhu (°C)</th>
              <th style={{ ...th, color: '#3b82f6' }}>Kelembaban (%)</th>
              <th style={{ ...th, color: '#f59e0b' }}>Cahaya (ADC)</th>
            </tr>
          </thead>
          <tbody>
            {last20.length === 0 ? (
              <tr>
                <td colSpan={5} style={{ ...td, textAlign: 'center', color: '#6b7280', padding: '24px' }}>
                  Belum ada data. Tunggu pembacaan dari ESP32…
                </td>
              </tr>
            ) : (
              last20.map((r) => (
                <tr key={r.id} style={{ transition: 'background 0.1s' }}>
                  <td style={td}>{formatTime(r.created_at)}</td>
                  <td style={{ ...td, color: '#818cf8' }}>{r.device_id}</td>
                  <td style={{ ...td, color: '#fca5a5' }}>{r.suhu.toFixed(1)}</td>
                  <td style={{ ...td, color: '#93c5fd' }}>{r.kelembaban.toFixed(1)}</td>
                  <td style={{ ...td, color: '#fcd34d' }}>{r.cahaya}</td>
                </tr>
              ))
            )}
          </tbody>
        </table>
      </div>
    </div>
  );
}
