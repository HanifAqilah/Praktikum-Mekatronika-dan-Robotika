import { useEffect } from 'react';
import axios from 'axios';
import { useWebSocket } from './hooks/useWebSocket';
import { useSensorStore, type SensorReading } from './store/sensorStore';
import StatusBar from './components/StatusBar';
import SensorCards from './components/SensorCards';
import SensorChart from './components/SensorChart';
import LedControl from './components/LedControl';
import HistoryTable from './components/HistoryTable';
import StatsCard from './components/StatsCard';

const appStyle: React.CSSProperties = {
  minHeight: '100vh',
  background: '#0f1120',
  color: '#e5e7eb',
  fontFamily: "'Segoe UI', system-ui, sans-serif",
  padding: '0 0 40px',
};

const headerStyle: React.CSSProperties = {
  background: '#12152a',
  borderBottom: '1px solid #1f2937',
  padding: '16px 24px',
  display: 'flex',
  alignItems: 'center',
  gap: 12,
};

const contentStyle: React.CSSProperties = {
  maxWidth: 1280,
  margin: '0 auto',
  padding: '24px 20px',
  display: 'flex',
  flexDirection: 'column',
  gap: 20,
};

export default function App() {
  useWebSocket();
  const setReadings = useSensorStore((s) => s.setReadings);

  useEffect(() => {
    axios
      .get<SensorReading[]>('/api/history?limit=100')
      .then((res) => setReadings(res.data))
      .catch((err) => console.warn('[App] Failed to fetch history:', err));
  }, [setReadings]);

  return (
    <div style={appStyle}>
      {/* Header */}
      <header style={headerStyle}>
        <span style={{ fontSize: 24 }}>🤖</span>
        <div>
          <h1 style={{ margin: 0, fontSize: 18, fontWeight: 700, color: '#f3f4f6' }}>
            Percobaan 5 — Real-time Sensor Dashboard
          </h1>
          <p style={{ margin: 0, fontSize: 12, color: '#6b7280' }}>
            Native WebSocket · React 18 · Recharts · SQLite
          </p>
        </div>
      </header>

      {/* Main content */}
      <main style={contentStyle}>
        <StatusBar />
        <SensorCards />
        <SensorChart />
        <div style={{ display: 'grid', gridTemplateColumns: '1fr 280px', gap: 20, alignItems: 'start' }}>
          <StatsCard />
          <LedControl />
        </div>
        <HistoryTable />
      </main>
    </div>
  );
}
