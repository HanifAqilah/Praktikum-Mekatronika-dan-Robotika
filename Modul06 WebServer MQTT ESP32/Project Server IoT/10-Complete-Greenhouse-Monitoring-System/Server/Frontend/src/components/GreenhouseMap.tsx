import React from 'react';
import { useSensorStore } from '../store/sensorStore';

function getColor(field: string, value: number) {
  if (field === 'suhu' && value > 35) return '#f87171';
  if (field === 'suhu' && value > 28) return '#fbbf24';
  if (field === 'kelembaban' && value < 30) return '#f87171';
  if (field === 'kelembaban' && value < 45) return '#fbbf24';
  return '#4ade80';
}

export default function GreenhouseMap() {
  const { suhu, kelembaban, cahaya, tanah } = useSensorStore((s) => s.reading);

  const suhuColor = getColor('suhu', suhu ?? 0);
  const kelColor = getColor('kelembaban', kelembaban ?? 0);

  return (
    <div style={{ background: '#1a1d2e', borderRadius: 12, padding: 16 }}>
      <div style={{ fontSize: 13, color: '#9ca3af', marginBottom: 8 }}>🗺️ Greenhouse Map</div>
      <svg viewBox="0 0 400 260" style={{ width: '100%', height: 220, border: '1px solid #2d3148', borderRadius: 8, background: '#0f1117' }}>
        {/* Greenhouse structure */}
        <rect x="20" y="40" width="360" height="200" rx="4" fill="none" stroke="#374151" strokeWidth="2" />
        <polyline points="20,40 200,10 380,40" fill="none" stroke="#374151" strokeWidth="2" />
        {/* Zones */}
        <rect x="30" y="50" width="160" height="180" rx="4" fill="rgba(74,222,128,0.05)" stroke="#1f2937" strokeWidth="1" />
        <rect x="210" y="50" width="160" height="180" rx="4" fill="rgba(74,222,128,0.05)" stroke="#1f2937" strokeWidth="1" />
        {/* Suhu sensor */}
        <circle cx="80" cy="120" r="18" fill={suhuColor + '33'} stroke={suhuColor} strokeWidth="2" />
        <text x="80" y="116" textAnchor="middle" fill={suhuColor} fontSize="9" fontWeight="600">🌡️</text>
        <text x="80" y="128" textAnchor="middle" fill={suhuColor} fontSize="8">{suhu?.toFixed(1)}°C</text>
        {/* Kelembaban sensor */}
        <circle cx="160" cy="180" r="18" fill={kelColor + '33'} stroke={kelColor} strokeWidth="2" />
        <text x="160" y="176" textAnchor="middle" fill={kelColor} fontSize="9">💧</text>
        <text x="160" y="188" textAnchor="middle" fill={kelColor} fontSize="8">{kelembaban?.toFixed(1)}%</text>
        {/* Cahaya sensor */}
        <circle cx="280" cy="100" r="18" fill="#fbbf2433" stroke="#fbbf24" strokeWidth="2" />
        <text x="280" y="96" textAnchor="middle" fill="#fbbf24" fontSize="9">☀️</text>
        <text x="280" y="108" textAnchor="middle" fill="#fbbf24" fontSize="8">{cahaya}</text>
        {/* Tanah sensor */}
        <circle cx="350" cy="190" r="18" fill="#a78bfa33" stroke="#a78bfa" strokeWidth="2" />
        <text x="350" y="186" textAnchor="middle" fill="#a78bfa" fontSize="9">🌱</text>
        <text x="350" y="198" textAnchor="middle" fill="#a78bfa" fontSize="8">{tanah}</text>
        {/* Labels */}
        <text x="110" y="65" fill="#4b5563" fontSize="10">Zone A</text>
        <text x="290" y="65" fill="#4b5563" fontSize="10">Zone B</text>
      </svg>
    </div>
  );
}
