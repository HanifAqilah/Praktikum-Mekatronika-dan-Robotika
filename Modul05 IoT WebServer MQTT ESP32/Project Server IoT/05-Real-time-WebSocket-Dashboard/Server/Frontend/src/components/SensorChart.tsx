import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
  ResponsiveContainer,
} from 'recharts';
import { useSensorStore } from '../store/sensorStore';

const COLORS = {
  suhu: '#ef4444',
  kelembaban: '#3b82f6',
  grid: '#374151',
  text: '#9ca3af',
  bg: '#1a1d2e',
};

function formatTime(dateStr: string): string {
  const d = new Date(dateStr);
  return d.toLocaleTimeString('id-ID', { hour: '2-digit', minute: '2-digit', second: '2-digit' });
}

export default function SensorChart() {
  const readings = useSensorStore((s) => s.readings);
  const data = readings.slice(-30).map((r) => ({
    time: formatTime(r.created_at),
    suhu: r.suhu,
    kelembaban: r.kelembaban,
  }));

  return (
    <div
      style={{
        background: COLORS.bg,
        borderRadius: 12,
        padding: '16px 8px 8px',
        border: '1px solid #2d3148',
      }}
    >
      <p style={{ color: COLORS.text, margin: '0 0 12px 16px', fontWeight: 600, fontSize: 14 }}>
        📈 Grafik Sensor Real-time (30 data terakhir)
      </p>
      <ResponsiveContainer width="100%" height={280}>
        <LineChart data={data} margin={{ top: 5, right: 30, left: 0, bottom: 5 }}>
          <CartesianGrid strokeDasharray="3 3" stroke={COLORS.grid} />
          <XAxis
            dataKey="time"
            tick={{ fill: COLORS.text, fontSize: 11 }}
            tickLine={{ stroke: COLORS.grid }}
            axisLine={{ stroke: COLORS.grid }}
            interval="preserveStartEnd"
          />
          <YAxis
            yAxisId="left"
            domain={['auto', 'auto']}
            tick={{ fill: COLORS.text, fontSize: 11 }}
            tickLine={{ stroke: COLORS.grid }}
            axisLine={{ stroke: COLORS.grid }}
            label={{ value: '°C', fill: COLORS.text, position: 'insideTopLeft', offset: -4, fontSize: 11 }}
          />
          <YAxis
            yAxisId="right"
            orientation="right"
            domain={['auto', 'auto']}
            tick={{ fill: COLORS.text, fontSize: 11 }}
            tickLine={{ stroke: COLORS.grid }}
            axisLine={{ stroke: COLORS.grid }}
            label={{ value: '%', fill: COLORS.text, position: 'insideTopRight', offset: -4, fontSize: 11 }}
          />
          <Tooltip
            contentStyle={{ background: '#0f1120', border: '1px solid #374151', borderRadius: 8 }}
            labelStyle={{ color: COLORS.text }}
            itemStyle={{ color: COLORS.text }}
            formatter={(value: number, name: string) =>
              name === 'suhu' ? [`${value} °C`, 'Suhu'] : [`${value} %`, 'Kelembaban']
            }
          />
          <Legend
            wrapperStyle={{ color: COLORS.text, fontSize: 12, paddingTop: 4 }}
          />
          <Line
            yAxisId="left"
            type="monotone"
            dataKey="suhu"
            stroke={COLORS.suhu}
            strokeWidth={2}
            dot={false}
            activeDot={{ r: 4 }}
            name="suhu"
          />
          <Line
            yAxisId="right"
            type="monotone"
            dataKey="kelembaban"
            stroke={COLORS.kelembaban}
            strokeWidth={2}
            dot={false}
            activeDot={{ r: 4 }}
            name="kelembaban"
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}
