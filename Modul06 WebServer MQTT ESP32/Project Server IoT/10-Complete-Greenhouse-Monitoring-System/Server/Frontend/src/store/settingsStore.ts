import { create } from 'zustand';
import axios from 'axios';

interface Thresholds {
  suhu_max: number;
  kelembaban_min: number;
  cahaya_min: number;
  tanah_min: number;
}

interface ActuatorState {
  fan: 'on' | 'off' | 'auto';
  light: 'on' | 'off' | 'auto';
  pump: 'on' | 'off' | 'auto';
  fan_last_changed: string | null;
  light_last_changed: string | null;
  pump_last_changed: string | null;
  manual_override: boolean;
}

interface SettingsStore {
  thresholds: Thresholds;
  actuators: ActuatorState;
  setThresholds: (t: Thresholds) => void;
  setActuatorState: (name: string, state: 'on' | 'off' | 'auto') => void;
  saveThresholds: () => Promise<void>;
  fetchThresholds: () => Promise<void>;
}

export const useSettingsStore = create<SettingsStore>((set, get) => ({
  thresholds: { suhu_max: 30, kelembaban_min: 40, cahaya_min: 1500, tanah_min: 1800 },
  actuators: {
    fan: 'auto', light: 'auto', pump: 'auto',
    fan_last_changed: null, light_last_changed: null, pump_last_changed: null,
    manual_override: false,
  },
  setThresholds: (t) => set({ thresholds: t }),
  setActuatorState: (name, state) =>
    set((s) => ({
      actuators: {
        ...s.actuators,
        [name]: state,
        [`${name}_last_changed`]: new Date().toISOString(),
        manual_override: state !== 'auto',
      },
    })),
  saveThresholds: async () => {
    await axios.post('/api/v1/settings/thresholds', get().thresholds);
  },
  fetchThresholds: async () => {
    try {
      const { data } = await axios.get('/api/v1/settings/thresholds');
      set({ thresholds: data });
    } catch {}
  },
}));
