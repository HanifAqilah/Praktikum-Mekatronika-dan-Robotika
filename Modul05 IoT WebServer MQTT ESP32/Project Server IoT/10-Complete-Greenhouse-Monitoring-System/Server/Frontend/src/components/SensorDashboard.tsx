import React from 'react';
import { useSensorStore } from '../store/sensorStore';

const cards = [
  { key: 'suhu', label: 'Suhu', unit: '°C', icon: '🌡️', warn: 28, crit: 35, color: '#f87171' },
  { key: 'kelembaban', label: 'Kelembaban', unit: '%', icon: '💧', warn: 45, crit: 30, color: '#60a5fa' },
  { key: 'cahaya', label: 'Cahaya ADC', unit: '', icon: '☀️', warn: 2000, crit: 3500, color: '#fbbf24' },
  { key: 'tanah', label: 'Tanah ADC', unit: '', icon: '🌱', warn: 2000, crit: 3000, color: '#a78bfa' },
];

export default function SensorDashboard() {
  const reading = useSensorStore((s) => s.reading);
  const lastUpdate = useSensorStore((s) => s.lastUpdate);

  return (
    <div>
      <div style={{ fontSize: 12, color: '#6b7280', marginBottom: 8 }}>
        Last update: {lastUpdate ? new Date(lastUpdate).toLocaleTimeString() : '—'}
      </div>
      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(2, 1fr)', gap: 12 }}>
        {cards.map(({ key, label, unit, icon, color }) => {
          const val = (reading as Record<string, number | null>)[key];
          return (
            <div key={key} style={{ background: '#1a1d2e', borderRadius: 10, padding: 16, borderLeft: `3px solid ${color}` }}>
              <div style={{ fontSize: 13, color: '#9ca3af' }}>{icon} {label}</div>
              <div style={{ fontSize: 28, fontWeight: 700, color, marginTop: 4 }}>
                {val !== null && val !== undefined ? `${typeof val === 'number' && val % 1 !== 0 ? val.toFixed(1) : val}${unit}` : '—'}
              </div>
            </div>
          );
        })}
      </div>
    </div>
  );
}
