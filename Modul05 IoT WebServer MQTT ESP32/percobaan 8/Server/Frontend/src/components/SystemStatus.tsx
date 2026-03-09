import { useEffect, useState } from "react";
import { useSystemStore } from "../store/systemStore";

interface ServerStatus {
  service: string;
  mqtt: { connected: boolean };
  uptime: number;
  timestamp: string;
}

function Dot({ on }: { on: boolean }) {
  return (
    <span
      className={`inline-block w-2.5 h-2.5 rounded-full mr-2 ${
        on ? "bg-green-400 shadow-[0_0_6px_2px_rgba(74,222,128,0.5)]" : "bg-red-500"
      }`}
    />
  );
}

function uptimeStr(s: number) {
  const h = Math.floor(s / 3600);
  const m = Math.floor((s % 3600) / 60);
  const sec = Math.floor(s % 60);
  return `${h}h ${m}m ${sec}s`;
}

export function SystemStatus() {
  const { wsConnected, mqttConnected, latestSensor } = useSystemStore();
  const [serverStatus, setServerStatus] = useState<ServerStatus | null>(null);
  const [dbConnected, setDbConnected] = useState(false);

  useEffect(() => {
    async function fetchStatus() {
      try {
        const res = await fetch("/api/status");
        if (!res.ok) return;
        const data: ServerStatus = await res.json();
        setServerStatus(data);
        setDbConnected(true);
      } catch {
        setDbConnected(false);
      }
    }

    fetchStatus();
    const interval = setInterval(fetchStatus, 10_000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div className="bg-gray-900 rounded-2xl p-6 shadow-xl border border-gray-800">
      <h2 className="text-lg font-bold text-white mb-4 flex items-center gap-2">
        <span>🖥️</span> Status Sistem
      </h2>

      <div className="grid grid-cols-2 md:grid-cols-4 gap-4 mb-4">
        <StatusCard label="WebSocket" isOn={wsConnected} detail={wsConnected ? "Terhubung" : "Terputus"} />
        <StatusCard
          label="MQTT Broker"
          isOn={mqttConnected ?? serverStatus?.mqtt.connected ?? false}
          detail={(mqttConnected ?? serverStatus?.mqtt.connected) ? "Online" : "Offline"}
        />
        <StatusCard label="Database" isOn={dbConnected} detail={dbConnected ? "PostgreSQL OK" : "Tidak terhubung"} />
        <StatusCard
          label="Last Reading"
          isOn={!!latestSensor}
          detail={latestSensor ? new Date(latestSensor.created_at).toLocaleTimeString("id-ID") : "—"}
        />
      </div>

      {latestSensor && (
        <div className="mt-4 border-t border-gray-800 pt-4">
          <p className="text-xs font-semibold text-gray-400 mb-3 uppercase tracking-wider">
            Pembacaan Terakhir — {latestSensor.device_id}
          </p>
          <div className="grid grid-cols-2 md:grid-cols-4 gap-3">
            <SensorCard icon="🌡️" label="Suhu" value={latestSensor.suhu} unit="°C" color="orange" />
            <SensorCard icon="💧" label="Kelembaban" value={latestSensor.kelembaban} unit="%" color="cyan" />
            <SensorCard icon="☀️" label="Cahaya" value={latestSensor.cahaya} unit="" color="yellow" />
            <SensorCard icon="🌱" label="Tanah" value={latestSensor.tanah} unit="" color="green" />
          </div>
        </div>
      )}

      {serverStatus && (
        <div className="mt-3 text-xs text-gray-500 flex items-center gap-4">
          <span>Uptime: {uptimeStr(serverStatus.uptime)}</span>
          <span>Server: {serverStatus.timestamp ? new Date(serverStatus.timestamp).toLocaleString("id-ID") : ""}</span>
        </div>
      )}
    </div>
  );
}

function StatusCard({ label, isOn, detail }: { label: string; isOn: boolean; detail: string }) {
  return (
    <div className="bg-gray-800 rounded-xl px-4 py-3 flex flex-col gap-1">
      <div className="flex items-center text-sm font-medium text-gray-300">
        <Dot on={isOn} />
        {label}
      </div>
      <p className="text-xs text-gray-500 ml-4">{detail}</p>
    </div>
  );
}

function SensorCard({
  icon,
  label,
  value,
  unit,
  color,
}: {
  icon: string;
  label: string;
  value: number | null;
  unit: string;
  color: string;
}) {
  const colorMap: Record<string, string> = {
    orange: "text-orange-400",
    cyan: "text-cyan-400",
    yellow: "text-yellow-400",
    green: "text-green-400",
  };
  return (
    <div className="bg-gray-800 rounded-xl px-4 py-3">
      <p className="text-xs text-gray-400 mb-1">
        {icon} {label}
      </p>
      <p className={`text-2xl font-bold font-mono ${colorMap[color] ?? "text-white"}`}>
        {value !== null ? `${value}${unit}` : "—"}
      </p>
    </div>
  );
}
