import { create } from 'zustand';

export interface SensorReading {
  suhu: number | null;
  kelembaban: number | null;
  cahaya: number | null;
  tanah: number | null;
}

interface SensorStore {
  latest: SensorReading;
  mqttConnected: boolean;
  wsConnected: boolean;
  lastUpdate: Date | null;
  setLatest: (data: Partial<SensorReading>) => void;
  setMqttConnected: (v: boolean) => void;
  setWsConnected: (v: boolean) => void;
  setLastUpdate: (d: Date) => void;
}

export const useSensorStore = create<SensorStore>((set) => ({
  latest: { suhu: null, kelembaban: null, cahaya: null, tanah: null },
  mqttConnected: false,
  wsConnected: false,
  lastUpdate: null,
  setLatest: (data) =>
    set((state) => ({ latest: { ...state.latest, ...data } })),
  setMqttConnected: (v) => set({ mqttConnected: v }),
  setWsConnected: (v) => set({ wsConnected: v }),
  setLastUpdate: (d) => set({ lastUpdate: d }),
}));
