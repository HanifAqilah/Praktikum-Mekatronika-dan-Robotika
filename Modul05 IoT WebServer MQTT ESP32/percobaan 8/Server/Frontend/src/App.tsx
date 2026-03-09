import { useWebSocket } from "./hooks/useWebSocket";
import { SystemStatus } from "./components/SystemStatus";
import { ActuatorControl } from "./components/ActuatorControl";
import { SensorHistory } from "./components/SensorHistory";
import { CommandLog } from "./components/CommandLog";

export default function App() {
  useWebSocket();

  return (
    <div className="min-h-screen bg-gray-950 text-white">
      {/* Header */}
      <header className="bg-gray-900 border-b border-gray-800 px-6 py-4 sticky top-0 z-10 shadow-lg">
        <div className="max-w-7xl mx-auto flex items-center justify-between">
          <div>
            <h1 className="text-xl font-bold text-white">
              🤖 Percobaan 8 — IoT Dashboard
            </h1>
            <p className="text-xs text-gray-400 mt-0.5">
              ESP32 · MQTT · WebSocket · PostgreSQL
            </p>
          </div>
          <div className="text-xs text-gray-500 font-mono">
            Praktikum Mekatronika &amp; Robotika
          </div>
        </div>
      </header>

      {/* Main content */}
      <main className="max-w-7xl mx-auto px-4 py-6 space-y-6">
        {/* System status + sensor readings */}
        <SystemStatus />

        {/* Actuator controls */}
        <ActuatorControl />

        {/* History tables side by side on large screens */}
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          <SensorHistory />
          <CommandLog />
        </div>
      </main>

      <footer className="text-center py-4 text-xs text-gray-600 border-t border-gray-800 mt-8">
        Percobaan 8 · IoT WebServer MQTT ESP32 · Modul 05
      </footer>
    </div>
  );
}
