import React from 'react';
import {
  ComposedChart, Line, XAxis, YAxis, CartesianGrid, Tooltip,
  Legend, ResponsiveContainer,
} from 'recharts';

interface DataPoint {
  time: string; suhu?: number; kelembaban?: number; cahaya?: number; tanah?: number;
}

interface Props { data: DataPoint[]; }

export default function AnalyticsChart({ data }: Props) {
  return (
    <div style={{ background: '#1a1d2e', borderRadius: 12, padding: 16 }}>
      <div style={{ fontSize: 14, fontWeight: 600, marginBottom: 12 }}>📊 Sensor Trends</div>
      <ResponsiveContainer width="100%" height={320}>
        <ComposedChart data={data} margin={{ top: 5, right: 20, left: 0, bottom: 5 }}>
          <CartesianGrid strokeDasharray="3 3" stroke="#1f2937" />
          <XAxis dataKey="time" stroke="#4b5563" tick={{ fill: '#6b7280', fontSize: 11 }} />
          <YAxis yAxisId="left" stroke="#4b5563" tick={{ fill: '#6b7280', fontSize: 11 }} />
          <YAxis yAxisId="right" orientation="right" stroke="#4b5563" tick={{ fill: '#6b7280', fontSize: 11 }} />
          <Tooltip contentStyle={{ background: '#1a1d2e', border: '1px solid #374151', color: '#e0e0e0', fontSize: 12 }} />
          <Legend wrapperStyle={{ fontSize: 12 }} />
          <Line yAxisId="left" type="monotone" dataKey="suhu" stroke="#f87171" dot={false} name="Suhu (°C)" />
          <Line yAxisId="right" type="monotone" dataKey="kelembaban" stroke="#60a5fa" dot={false} name="Kelembaban (%)" />
          <Line yAxisId="left" type="monotone" dataKey="cahaya" stroke="#fbbf24" dot={false} name="Cahaya ADC" strokeDasharray="4 4" />
          <Line yAxisId="left" type="monotone" dataKey="tanah" stroke="#a78bfa" dot={false} name="Tanah ADC" strokeDasharray="4 4" />
        </ComposedChart>
      </ResponsiveContainer>
    </div>
  );
}
