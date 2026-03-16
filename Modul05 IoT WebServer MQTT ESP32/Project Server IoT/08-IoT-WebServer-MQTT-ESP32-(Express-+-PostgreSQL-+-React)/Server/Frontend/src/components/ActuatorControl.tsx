import { useState } from "react";
import { useSystemStore } from "../store/systemStore";

interface ActuatorButtonProps {
  label: string;
  isOn: boolean;
  loading: boolean;
  onToggle: () => void;
  color?: string;
}

function ActuatorButton({ label, isOn, loading, onToggle, color = "blue" }: ActuatorButtonProps) {
  const colorMap: Record<string, string> = {
    blue: isOn ? "bg-blue-600 hover:bg-blue-700" : "bg-gray-700 hover:bg-gray-600",
    red: isOn ? "bg-red-600 hover:bg-red-700" : "bg-gray-700 hover:bg-gray-600",
    green: isOn ? "bg-green-600 hover:bg-green-700" : "bg-gray-700 hover:bg-gray-600",
    yellow: isOn ? "bg-yellow-500 hover:bg-yellow-600" : "bg-gray-700 hover:bg-gray-600",
  };

  return (
    <button
      onClick={onToggle}
      disabled={loading}
      className={`flex flex-col items-center justify-center gap-2 px-6 py-4 rounded-xl font-semibold transition-all duration-200 shadow-lg min-w-[120px] ${colorMap[color]} disabled:opacity-50 disabled:cursor-not-allowed`}
    >
      <span className={`w-5 h-5 rounded-full border-2 ${isOn ? "bg-white border-white shadow-glow" : "bg-transparent border-gray-400"}`} />
      <span className="text-sm">{label}</span>
      <span className="text-xs opacity-75">{loading ? "..." : isOn ? "ON" : "OFF"}</span>
    </button>
  );
}

export function ActuatorControl() {
  const { led1, led2, led3, relay, setActuator, acks } = useSystemStore();
  const [loading, setLoading] = useState<Record<string, boolean>>({});

  async function sendCommand(type: "led" | "relay" | "all", target: string, nextState: boolean) {
    const key = target;
    setLoading((prev) => ({ ...prev, [key]: true }));

    try {
      const body: Record<string, unknown> = { state: nextState ? "on" : "off" };
      if (type === "led") body.led = parseInt(target.replace("led", ""));

      const endpoint = type === "led" ? "/api/actuator/led" : `/api/actuator/${target}`;
      const res = await fetch(endpoint, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });

      if (!res.ok) throw new Error("Command failed");

      setActuator(target as "led1" | "led2" | "led3" | "relay", nextState);
    } catch (err) {
      console.error(`Command error for ${target}:`, err);
      alert(`Failed to send command to ${target}`);
    } finally {
      setLoading((prev) => ({ ...prev, [key]: false }));
    }
  }

  async function sendAll(state: boolean) {
    setLoading({ led1: true, led2: true, led3: true, relay: true });
    try {
      const res = await fetch("/api/actuator/all", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ state: state ? "on" : "off" }),
      });
      if (!res.ok) throw new Error("All command failed");
      setActuator("led1", state);
      setActuator("led2", state);
      setActuator("led3", state);
      setActuator("relay", state);
    } catch (err) {
      alert("Failed to send ALL command");
    } finally {
      setLoading({});
    }
  }

  return (
    <div className="bg-gray-900 rounded-2xl p-6 shadow-xl border border-gray-800">
      <h2 className="text-lg font-bold mb-4 text-white flex items-center gap-2">
        <span>⚡</span> Actuator Control
      </h2>

      <div className="flex flex-wrap gap-4 mb-6">
        <ActuatorButton
          label="LED 1"
          isOn={led1}
          loading={loading.led1 ?? false}
          onToggle={() => sendCommand("led", "led1", !led1)}
          color="blue"
        />
        <ActuatorButton
          label="LED 2"
          isOn={led2}
          loading={loading.led2 ?? false}
          onToggle={() => sendCommand("led", "led2", !led2)}
          color="green"
        />
        <ActuatorButton
          label="LED 3"
          isOn={led3}
          loading={loading.led3 ?? false}
          onToggle={() => sendCommand("led", "led3", !led3)}
          color="yellow"
        />
        <ActuatorButton
          label="Relay"
          isOn={relay}
          loading={loading.relay ?? false}
          onToggle={() => sendCommand("relay", "relay", !relay)}
          color="red"
        />
      </div>

      <div className="flex gap-3">
        <button
          onClick={() => sendAll(true)}
          className="px-4 py-2 bg-green-700 hover:bg-green-600 rounded-lg text-sm font-medium transition-colors"
        >
          All ON
        </button>
        <button
          onClick={() => sendAll(false)}
          className="px-4 py-2 bg-red-800 hover:bg-red-700 rounded-lg text-sm font-medium transition-colors"
        >
          All OFF
        </button>
      </div>

      {Object.keys(acks).length > 0 && (
        <div className="mt-4 border-t border-gray-700 pt-4">
          <p className="text-xs font-semibold text-gray-400 mb-2">Last ACKs</p>
          <div className="flex flex-wrap gap-2">
            {Object.entries(acks).map(([actuator, ack]) => (
              <span
                key={actuator}
                className={`text-xs px-2 py-1 rounded-full font-mono ${
                  ack.status === "ok" || ack.status === "success"
                    ? "bg-green-900 text-green-300"
                    : "bg-red-900 text-red-300"
                }`}
              >
                {actuator}: {ack.status}
              </span>
            ))}
          </div>
        </div>
      )}
    </div>
  );
}
