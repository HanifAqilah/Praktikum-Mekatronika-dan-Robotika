import React, { useState } from 'react';
import axios from 'axios';
import { useSettingsStore } from '../store/settingsStore';

const actuators = [
  { key: 'fan', label: 'Fan', icon: '💨', topic: 'greenhouse/command/fan' },
  { key: 'light', label: 'Light', icon: '💡', topic: 'greenhouse/command/light' },
  { key: 'pump', label: 'Pump', icon: '🚿', topic: 'greenhouse/command/pump' },
];

export default function ActuatorPanel() {
  const { actuators: states, setActuatorState } = useSettingsStore();
  const [loading, setLoading] = useState<string | null>(null);

  const sendCommand = async (actuator: string, state: 'on' | 'off' | 'auto') => {
    setLoading(actuator);
    try {
      await axios.post('/api/v1/actuators/command', { actuator, state });
      setActuatorState(actuator, state);
    } catch {} finally { setLoading(null); }
  };

  return (
    <div style={{ background: '#1a1d2e', borderRadius: 12, padding: 16 }}>
      <div style={{ fontSize: 14, fontWeight: 600, marginBottom: 12 }}>🎛️ Actuator Control</div>
      {states.manual_override && (
        <div style={{ background: '#fbbf2422', border: '1px solid #fbbf24', borderRadius: 6, padding: '6px 10px', fontSize: 12, color: '#fbbf24', marginBottom: 12 }}>
          ⚠️ Manual override active
        </div>
      )}
      <div style={{ display: 'flex', flexDirection: 'column', gap: 10 }}>
        {actuators.map(({ key, label, icon }) => {
          const state = (states as Record<string, string>)[key] as 'on' | 'off' | 'auto';
          const lastChanged = (states as Record<string, string | null>)[`${key}_last_changed`];
          return (
            <div key={key} style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', padding: '10px 12px', background: '#0f1117', borderRadius: 8 }}>
              <div>
                <div style={{ fontWeight: 600 }}>{icon} {label}</div>
                <div style={{ fontSize: 11, color: '#6b7280', marginTop: 2 }}>
                  {lastChanged ? `Changed: ${new Date(lastChanged).toLocaleTimeString()}` : 'Not changed'}
                </div>
              </div>
              <div style={{ display: 'flex', gap: 6 }}>
                {(['auto', 'on', 'off'] as const).map((s) => (
                  <button key={s} disabled={loading === key} onClick={() => sendCommand(key, s)}
                    style={{
                      padding: '5px 10px', borderRadius: 6, border: 'none', cursor: 'pointer', fontSize: 12, fontWeight: 600,
                      background: state === s ? (s === 'on' ? '#4ade80' : s === 'off' ? '#f87171' : '#60a5fa') : '#374151',
                      color: state === s ? '#000' : '#9ca3af',
                    }}>
                    {s.toUpperCase()}
                  </button>
                ))}
              </div>
            </div>
          );
        })}
      </div>
    </div>
  );
}
