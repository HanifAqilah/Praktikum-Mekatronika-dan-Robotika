import { useEffect, useState, useCallback } from "react";
import type { SensorReading } from "../store/systemStore";

interface PaginationState {
  cursor: string | null;
  hasMore: boolean;
  history: SensorReading[];
  cursors: string[];
}

function formatDate(iso: string) {
  return new Date(iso).toLocaleString("id-ID", { timeZone: "Asia/Jakarta" });
}

function Badge({ value, unit, label }: { value: number | null; unit: string; label: string }) {
  return (
    <span className="text-xs text-gray-400">
      {label}:{" "}
      <span className="font-semibold text-white">
        {value !== null ? `${value}${unit}` : "-"}
      </span>
    </span>
  );
}

export function SensorHistory() {
  const [page, setPage] = useState<PaginationState>({
    cursor: null,
    hasMore: false,
    history: [],
    cursors: [],
  });
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const LIMIT = 10;

  const fetchPage = useCallback(async (cursor: string | null) => {
    setLoading(true);
    setError(null);
    try {
      const params = new URLSearchParams({ limit: String(LIMIT) });
      if (cursor) params.set("cursor", cursor);
      const res = await fetch(`/api/sensor/history?${params}`);
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      const json = await res.json();
      setPage((prev) => ({
        cursor: json.pagination.nextCursor,
        hasMore: json.pagination.hasMore,
        history: json.data,
        cursors: cursor === null ? [] : prev.cursors,
      }));
    } catch (e) {
      setError((e as Error).message);
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    fetchPage(null);
    const interval = setInterval(() => fetchPage(null), 15_000);
    return () => clearInterval(interval);
  }, [fetchPage]);

  function nextPage() {
    if (!page.cursor) return;
    setPage((prev) => ({ ...prev, cursors: [...prev.cursors, prev.cursor!] }));
    fetchPage(page.cursor);
  }

  function prevPage() {
    const cursors = [...page.cursors];
    cursors.pop();
    const prevCursor = cursors.length > 0 ? cursors[cursors.length - 1] : null;
    setPage((prev) => ({ ...prev, cursors }));
    fetchPage(prevCursor);
  }

  return (
    <div className="bg-gray-900 rounded-2xl p-6 shadow-xl border border-gray-800">
      <div className="flex items-center justify-between mb-4">
        <h2 className="text-lg font-bold text-white flex items-center gap-2">
          <span>📊</span> Riwayat Sensor
        </h2>
        <button
          onClick={() => fetchPage(null)}
          className="text-xs text-brand-500 hover:underline"
        >
          Refresh
        </button>
      </div>

      {error && (
        <div className="text-sm text-red-400 mb-3 px-3 py-2 bg-red-900/30 rounded-lg">
          Error: {error}
        </div>
      )}

      <div className="overflow-x-auto rounded-lg">
        <table className="w-full text-sm">
          <thead>
            <tr className="text-gray-400 text-xs uppercase border-b border-gray-800">
              <th className="pb-2 text-left">ID</th>
              <th className="pb-2 text-left">Device</th>
              <th className="pb-2 text-right">Suhu (°C)</th>
              <th className="pb-2 text-right">Kelembaban (%)</th>
              <th className="pb-2 text-right">Cahaya</th>
              <th className="pb-2 text-right">Tanah</th>
              <th className="pb-2 text-left">Waktu</th>
            </tr>
          </thead>
          <tbody>
            {loading && page.history.length === 0 && (
              <tr>
                <td colSpan={7} className="py-8 text-center text-gray-500">
                  Loading...
                </td>
              </tr>
            )}
            {!loading && page.history.length === 0 && (
              <tr>
                <td colSpan={7} className="py-8 text-center text-gray-500">
                  Belum ada data sensor
                </td>
              </tr>
            )}
            {page.history.map((row, idx) => (
              <tr
                key={row.id}
                className={`border-b border-gray-800/50 hover:bg-gray-800/40 transition-colors ${
                  idx === 0 ? "bg-gray-800/20" : ""
                }`}
              >
                <td className="py-2 pr-2 font-mono text-xs text-gray-500">{row.id}</td>
                <td className="py-2 pr-2 font-mono text-xs text-brand-500">{row.device_id}</td>
                <td className="py-2 pr-2 text-right text-orange-400 font-mono">
                  {row.suhu !== null ? row.suhu : "-"}
                </td>
                <td className="py-2 pr-2 text-right text-cyan-400 font-mono">
                  {row.kelembaban !== null ? row.kelembaban : "-"}
                </td>
                <td className="py-2 pr-2 text-right text-yellow-400 font-mono">
                  {row.cahaya !== null ? row.cahaya : "-"}
                </td>
                <td className="py-2 pr-2 text-right text-green-400 font-mono">
                  {row.tanah !== null ? row.tanah : "-"}
                </td>
                <td className="py-2 text-xs text-gray-400">{formatDate(row.created_at)}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>

      <div className="flex items-center justify-between mt-4">
        <button
          onClick={prevPage}
          disabled={page.cursors.length === 0 || loading}
          className="px-3 py-1 text-xs bg-gray-800 hover:bg-gray-700 rounded-lg disabled:opacity-40 disabled:cursor-not-allowed"
        >
          ← Sebelumnya
        </button>
        <span className="text-xs text-gray-500">
          Halaman {page.cursors.length + 1}
        </span>
        <button
          onClick={nextPage}
          disabled={!page.hasMore || loading}
          className="px-3 py-1 text-xs bg-gray-800 hover:bg-gray-700 rounded-lg disabled:opacity-40 disabled:cursor-not-allowed"
        >
          Berikutnya →
        </button>
      </div>
    </div>
  );
}
