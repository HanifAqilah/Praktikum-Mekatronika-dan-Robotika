import React from 'react';
import { useAlertStore } from '../store/alertStore';

const severityColor: Record<string, string> = { critical: '#f87171', warning: '#fbbf24', info: '#60a5fa' };

export default function AlertFeed() {
  const alerts = useAlertStore((s) => s.alerts);

  return (
    <div style={{ background: '#1a1d2e', borderRadius: 12, padding: 16, maxHeight: 280, overflowY: 'auto' }}>
      <div style={{ fontSize: 14, fontWeight: 600, marginBottom: 10 }}>🔔 Alert Feed</div>
      {alerts.length === 0 ? (
        <div style={{ color: '#6b7280', fontSize: 13 }}>No active alerts</div>
      ) : (
        alerts.slice().reverse().map((a) => (
          <div key={a.id} style={{ padding: '8px 10px', marginBottom: 6, borderRadius: 6, background: '#0f1117', borderLeft: `3px solid ${severityColor[a.severity] ?? '#4b5563'}` }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: 12 }}>
              <span style={{ fontWeight: 600, color: severityColor[a.severity] ?? '#9ca3af' }}>
                {a.severity.toUpperCase()} — {a.field}
              </span>
              <span style={{ color: '#6b7280' }}>{new Date(a.created_at).toLocaleTimeString()}</span>
            </div>
            <div style={{ fontSize: 12, color: '#9ca3af', marginTop: 2 }}>
              {a.field} = {a.value} ({a.condition} {a.threshold}) · {a.device_id}
            </div>
          </div>
        ))
      )}
    </div>
  );
}
