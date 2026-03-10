import { useEffect, useState, useCallback } from 'react';
import axios from 'axios';
import { useSensorStore, type Stats } from '../store/sensorStore';

interface StatItem {
  label: string;
  key: keyof Stats;
  color: string;
  suffix: string;
}

const STAT_ITEMS: StatItem[] = [
  { label: 'Total Data',      key: 'count',           color: '#818cf8', suffix: '' },
  { label: 'Rata-rata Suhu',  key: 'avg_suhu',        color: '#ef4444', suffix: ' °C' },
  { label: 'Rata-rata Lembab',key: 'avg_kelembaban',  color: '#3b82f6', suffix: ' %' },
  { label: 'Suhu Maks',       key: 'max_suhu',        color: '#f97316', suffix: ' °C' },
  { label: 'Suhu Min',        key: 'min_suhu',        color: '#06b6d4', suffix: ' °C' },
];

export default function StatsCard() {
  const { stats, setStats } = useSensorStore();
  const [error, setError] = useState<string | null>(null);

  const fetchStats = useCallback(async () => {
    try {
      const res = await axios.get<Stats>('/api/stats');
      setStats(res.data);
      setError(null);
    } catch {
      setError('Gagal memuat statistik');
    }
  }, [setStats]);

  useEffect(() => {
    fetchStats();
    const interval = setInterval(fetchStats, 60_000);
    return () => clearInterval(interval);
  }, [fetchStats]);

  return (
    <div
      style={{
        background: '#1a1d2e',
        border: '1px solid #2d3148',
        borderRadius: 12,
        padding: '16px',
      }}
    >
      <p style={{ color: '#e5e7eb', fontWeight: 600, margin: '0 0 14px', fontSize: 14 }}>
        📊 Statistik Keseluruhan
      </p>

      {error && (
        <p style={{ color: '#f87171', fontSize: 13, margin: 0 }}>{error}</p>
      )}

      {!error && (
        <div style={{ display: 'grid', gridTemplateColumns: 'repeat(5, 1fr)', gap: 12 }}>
          {STAT_ITEMS.map(({ label, key, color, suffix }) => {
            const raw = stats ? stats[key] : null;
            const display =
              raw === null || raw === undefined
                ? '—'
                : key === 'count'
                ? String(raw)
                : (raw as number).toFixed(1) + suffix;
            return (
              <div
                key={key}
                style={{
                  background: '#0f1120',
                  borderRadius: 8,
                  padding: '12px 10px',
                  textAlign: 'center',
                }}
              >
                <div style={{ color, fontSize: 20, fontWeight: 700 }}>{display}</div>
                <div style={{ color: '#6b7280', fontSize: 11, marginTop: 4 }}>{label}</div>
              </div>
            );
          })}
        </div>
      )}
    </div>
  );
}
