import { create } from 'zustand';

export interface SensorReading {
  id: number;
  device_id: string;
  suhu: number;
  kelembaban: number;
  cahaya: number;
  created_at: string;
}

export interface Stats {
  count: number;
  avg_suhu: number;
  avg_kelembaban: number;
  avg_cahaya: number;
  max_suhu: number;
  min_suhu: number;
}

interface SensorState {
  readings: SensorReading[];
  latest: SensorReading | null;
  ledState: boolean;
  wsConnected: boolean;
  stats: Stats | null;
  // Actions
  addReading: (reading: SensorReading) => void;
  setReadings: (readings: SensorReading[]) => void;
  setLatest: (reading: SensorReading) => void;
  setLedState: (state: boolean) => void;
  setWsConnected: (connected: boolean) => void;
  setStats: (stats: Stats) => void;
}

const MAX_READINGS = 200;

export const useSensorStore = create<SensorState>((set) => ({
  readings: [],
  latest: null,
  ledState: false,
  wsConnected: false,
  stats: null,

  addReading: (reading) =>
    set((state) => {
      const updated = [...state.readings, reading];
      return {
        readings: updated.length > MAX_READINGS ? updated.slice(-MAX_READINGS) : updated,
        latest: reading,
      };
    }),

  setReadings: (readings) =>
    set({
      readings,
      latest: readings.length > 0 ? readings[readings.length - 1] : null,
    }),

  setLatest: (reading) => set({ latest: reading }),

  setLedState: (state) => set({ ledState: state }),

  setWsConnected: (connected) => set({ wsConnected: connected }),

  setStats: (stats) => set({ stats }),
}));
