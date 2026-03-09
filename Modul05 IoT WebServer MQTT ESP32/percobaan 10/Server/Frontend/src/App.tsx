import React from 'react';
import { BrowserRouter, Routes, Route, NavLink } from 'react-router-dom';
import { Toaster } from 'react-hot-toast';
import { useWebSocket } from './hooks/useWebSocket';
import Dashboard from './pages/Dashboard';
import Analytics from './pages/Analytics';
import Alerts from './pages/Alerts';
import Devices from './pages/Devices';
import Settings from './pages/Settings';
import { useSensorStore } from './store/sensorStore';

function Layout() {
  useWebSocket();
  const wsConnected = useSensorStore((s) => s.wsConnected);
  const links = [
    { to: '/', label: '🌿 Dashboard' },
    { to: '/analytics', label: '📈 Analytics' },
    { to: '/alerts', label: '🔔 Alerts' },
    { to: '/devices', label: '📡 Devices' },
    { to: '/settings', label: '⚙️ Settings' },
  ];
  return (
    <div style={{ display: 'flex', height: '100vh', fontFamily: 'Inter, sans-serif', background: '#0f1117', color: '#e0e0e0' }}>
      <nav style={{ width: 220, background: '#1a1d2e', padding: '24px 16px', display: 'flex', flexDirection: 'column', gap: 8 }}>
        <div style={{ marginBottom: 24 }}>
          <div style={{ fontSize: 18, fontWeight: 700, color: '#4ade80' }}>🏡 Greenhouse</div>
          <div style={{ fontSize: 12, color: wsConnected ? '#4ade80' : '#f87171', marginTop: 4 }}>
            ● {wsConnected ? 'Live' : 'Disconnected'}
          </div>
        </div>
        {links.map(({ to, label }) => (
          <NavLink key={to} to={to} end={to === '/'}
            style={({ isActive }) => ({
              padding: '10px 14px', borderRadius: 8, textDecoration: 'none',
              color: isActive ? '#4ade80' : '#9ca3af',
              background: isActive ? 'rgba(74,222,128,0.1)' : 'transparent',
              fontWeight: isActive ? 600 : 400, fontSize: 14,
            })}>
            {label}
          </NavLink>
        ))}
      </nav>
      <main style={{ flex: 1, overflow: 'auto', padding: 24 }}>
        <Routes>
          <Route path="/" element={<Dashboard />} />
          <Route path="/analytics" element={<Analytics />} />
          <Route path="/alerts" element={<Alerts />} />
          <Route path="/devices" element={<Devices />} />
          <Route path="/settings" element={<Settings />} />
        </Routes>
      </main>
    </div>
  );
}

export default function App() {
  return (
    <BrowserRouter>
      <Toaster position="top-right" toastOptions={{ style: { background: '#1a1d2e', color: '#e0e0e0' } }} />
      <Layout />
    </BrowserRouter>
  );
}
