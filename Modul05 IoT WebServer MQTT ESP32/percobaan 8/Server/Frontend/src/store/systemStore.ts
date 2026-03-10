import { create } from "zustand";

export interface SensorReading {
  id: number;
  device_id: string;
  suhu: number | null;
  kelembaban: number | null;
  cahaya: number | null;
  tanah: number | null;
  created_at: string;
}

export interface CommandEntry {
  id: number;
  command_type: string;
  target: string;
  payload: Record<string, unknown>;
  status: string;
  created_at: string;
}

export interface ActuatorAck {
  actuator: string;
  status: string;
  ts: number;
}

interface SystemStore {
  // Connection
  wsConnected: boolean;
  mqttConnected: boolean;
  dbConnected: boolean;

  // Latest sensor reading (live)
  latestSensor: SensorReading | null;

  // Actuator states
  led1: boolean;
  led2: boolean;
  led3: boolean;
  relay: boolean;

  // Last ack messages per actuator
  acks: Record<string, ActuatorAck>;

  // Actions
  setWsConnected: (v: boolean) => void;
  setMqttConnected: (v: boolean) => void;
  setDbConnected: (v: boolean) => void;
  setLatestSensor: (data: SensorReading) => void;
  setActuator: (name: "led1" | "led2" | "led3" | "relay", state: boolean) => void;
  handleAck: (ack: ActuatorAck) => void;
}

export const useSystemStore = create<SystemStore>((set) => ({
  wsConnected: false,
  mqttConnected: false,
  dbConnected: false,
  latestSensor: null,
  led1: false,
  led2: false,
  led3: false,
  relay: false,
  acks: {},

  setWsConnected: (v) => set({ wsConnected: v }),
  setMqttConnected: (v) => set({ mqttConnected: v }),
  setDbConnected: (v) => set({ dbConnected: v }),
  setLatestSensor: (data) => set({ latestSensor: data }),

  setActuator: (name, state) =>
    set((s) => ({ ...s, [name]: state })),

  handleAck: (ack) =>
    set((s) => ({
      acks: { ...s.acks, [ack.actuator]: ack },
    })),
}));
