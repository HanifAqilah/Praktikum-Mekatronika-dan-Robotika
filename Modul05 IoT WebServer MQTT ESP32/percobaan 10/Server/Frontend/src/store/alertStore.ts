import { create } from 'zustand';

export type AlertSeverity = 'critical' | 'warning' | 'info';

export interface Alert {
  id: string;
  severity: AlertSeverity;
  message: string;
  sensor: string;
  value: number;
  threshold: number;
  timestamp: string;
  acknowledged: boolean;
  resolved: boolean;
}

interface AlertStore {
  alerts: Alert[];
  newAlert: Alert | null;
  setAlerts: (alerts: Alert[]) => void;
  addAlert: (alert: Alert) => void;
  acknowledgeAlert: (id: string) => void;
  resolveAlert: (id: string) => void;
  clearNewAlert: () => void;
}

export const useAlertStore = create<AlertStore>((set) => ({
  alerts: [],
  newAlert: null,
  setAlerts: (alerts) => set({ alerts }),
  addAlert: (alert) =>
    set((state) => ({
      alerts: [alert, ...state.alerts].slice(0, 200),
      newAlert: alert,
    })),
  acknowledgeAlert: (id) =>
    set((state) => ({
      alerts: state.alerts.map((a) =>
        a.id === id ? { ...a, acknowledged: true } : a
      ),
    })),
  resolveAlert: (id) =>
    set((state) => ({
      alerts: state.alerts.map((a) =>
        a.id === id ? { ...a, resolved: true, acknowledged: true } : a
      ),
    })),
  clearNewAlert: () => set({ newAlert: null }),
}));
