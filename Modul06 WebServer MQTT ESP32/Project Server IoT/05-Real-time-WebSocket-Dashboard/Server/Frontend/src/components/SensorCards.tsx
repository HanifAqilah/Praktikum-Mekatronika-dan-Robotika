import { useSensorStore } from '../store/sensorStore';

interface CardConfig {
  key: 'suhu' | 'kelembaban' | 'cahaya';
  label: string;
  icon: string;
  unit: string;
  color: string;
  accent: string;
}

const CARDS: CardConfig[] = [
  { key: 'suhu',       label: 'Suhu',       icon: '🌡️', unit: '°C',  color: '#ef4444', accent: '#7f1d1d' },
  { key: 'kelembaban', label: 'Kelembaban', icon: '💧', unit: '%',   color: '#3b82f6', accent: '#1e3a5f' },
  { key: 'cahaya',     label: 'Cahaya',     icon: '☀️', unit: 'ADC', color: '#f59e0b', accent: '#78350f' },
];

export default function SensorCards() {
  const latest = useSensorStore((s) => s.latest);

  return (
    <div style={{ display: 'grid', gridTemplateColumns: 'repeat(3, 1fr)', gap: 16 }}>
      {CARDS.map(({ key, label, icon, unit, color, accent }) => {
        const value = latest ? latest[key] : null;
        return (
          <div
            key={key}
            style={{
              background: '#1a1d2e',
              border: `1px solid ${accent}`,
              borderRadius: 12,
              padding: '20px 16px',
              display: 'flex',
              flexDirection: 'column',
              gap: 8,
            }}
          >
            <span style={{ fontSize: 28 }}>{icon}</span>
            <span style={{ color: '#9ca3af', fontSize: 13, fontWeight: 500 }}>{label}</span>
            <span style={{ color, fontSize: 36, fontWeight: 700, lineHeight: 1 }}>
              {value !== null ? (key === 'cahaya' ? value : value.toFixed(1)) : '—'}
            </span>
            <span style={{ color: '#6b7280', fontSize: 12 }}>{unit}</span>
          </div>
        );
      })}
    </div>
  );
}
