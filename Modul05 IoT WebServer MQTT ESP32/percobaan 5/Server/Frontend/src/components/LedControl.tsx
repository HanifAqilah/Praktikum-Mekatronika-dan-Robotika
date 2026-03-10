import { useState } from 'react';
import axios from 'axios';
import { useSensorStore } from '../store/sensorStore';

export default function LedControl() {
  const ledState = useSensorStore((s) => s.ledState);
  const setLedState = useSensorStore((s) => s.setLedState);
  const [loading, setLoading] = useState(false);

  const toggle = async () => {
    setLoading(true);
    try {
      const res = await axios.post<{ state: boolean }>('/api/led');
      setLedState(res.data.state);
    } catch (err) {
      console.error('[LED] Toggle failed:', err);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div
      style={{
        background: '#1a1d2e',
        border: '1px solid #2d3148',
        borderRadius: 12,
        padding: '20px 24px',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'space-between',
        gap: 16,
      }}
    >
      <div>
        <p style={{ color: '#e5e7eb', fontWeight: 600, margin: 0, fontSize: 15 }}>💡 Kontrol LED</p>
        <p style={{ color: '#6b7280', margin: '4px 0 0', fontSize: 13 }}>
          Status:{' '}
          <span style={{ color: ledState ? '#22c55e' : '#9ca3af', fontWeight: 600 }}>
            {ledState ? 'ON' : 'OFF'}
          </span>
        </p>
      </div>

      <div style={{ display: 'flex', alignItems: 'center', gap: 12 }}>
        {/* Indicator */}
        <div
          style={{
            width: 20,
            height: 20,
            borderRadius: '50%',
            background: ledState ? '#22c55e' : '#374151',
            boxShadow: ledState ? '0 0 12px #22c55e' : 'none',
            transition: 'all 0.3s',
          }}
        />

        <button
          onClick={toggle}
          disabled={loading}
          style={{
            padding: '10px 28px',
            borderRadius: 8,
            border: 'none',
            cursor: loading ? 'not-allowed' : 'pointer',
            fontWeight: 700,
            fontSize: 14,
            transition: 'all 0.2s',
            background: ledState ? '#22c55e' : '#4b5563',
            color: '#fff',
            opacity: loading ? 0.6 : 1,
            minWidth: 80,
          }}
        >
          {loading ? '...' : ledState ? 'OFF' : 'ON'}
        </button>
      </div>
    </div>
  );
}
