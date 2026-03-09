import { useEffect, useState, useCallback } from "react";
import type { CommandEntry } from "../store/systemStore";

function formatDate(iso: string) {
  return new Date(iso).toLocaleString("id-ID", { timeZone: "Asia/Jakarta" });
}

function StatusBadge({ status }: { status: string }) {
  const colors: Record<string, string> = {
    sent: "bg-blue-900 text-blue-300",
    ok: "bg-green-900 text-green-300",
    success: "bg-green-900 text-green-300",
    failed: "bg-red-900 text-red-300",
    error: "bg-red-900 text-red-300",
  };
  return (
    <span className={`text-xs px-2 py-0.5 rounded-full font-medium ${colors[status] ?? "bg-gray-700 text-gray-300"}`}>
      {status}
    </span>
  );
}

export function CommandLog() {
  const [commands, setCommands] = useState<CommandEntry[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [cursors, setCursors] = useState<string[]>([]);
  const [nextCursor, setNextCursor] = useState<string | null>(null);
  const [hasMore, setHasMore] = useState(false);
  const LIMIT = 10;

  const fetchPage = useCallback(async (cursor: string | null) => {
    setLoading(true);
    setError(null);
    try {
      const params = new URLSearchParams({ limit: String(LIMIT) });
      if (cursor) params.set("cursor", cursor);
      const res = await fetch(`/api/commands/log?${params}`);
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      const json = await res.json();
      setCommands(json.data);
      setHasMore(json.pagination.hasMore);
      setNextCursor(json.pagination.nextCursor);
    } catch (e) {
      setError((e as Error).message);
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    fetchPage(null);
    const interval = setInterval(() => fetchPage(null), 10_000);
    return () => clearInterval(interval);
  }, [fetchPage]);

  function goNext() {
    if (!nextCursor) return;
    setCursors((prev) => [...prev, nextCursor]);
    fetchPage(nextCursor);
  }

  function goPrev() {
    const prev = [...cursors];
    prev.pop();
    const cursor = prev.length > 0 ? prev[prev.length - 1] : null;
    setCursors(prev);
    fetchPage(cursor);
  }

  return (
    <div className="bg-gray-900 rounded-2xl p-6 shadow-xl border border-gray-800">
      <div className="flex items-center justify-between mb-4">
        <h2 className="text-lg font-bold text-white flex items-center gap-2">
          <span>📋</span> Log Perintah
        </h2>
        <button
          onClick={() => { setCursors([]); fetchPage(null); }}
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
              <th className="pb-2 text-left">Tipe</th>
              <th className="pb-2 text-left">Target</th>
              <th className="pb-2 text-left">Payload</th>
              <th className="pb-2 text-left">Status</th>
              <th className="pb-2 text-left">Waktu</th>
            </tr>
          </thead>
          <tbody>
            {loading && commands.length === 0 && (
              <tr>
                <td colSpan={6} className="py-8 text-center text-gray-500">
                  Loading...
                </td>
              </tr>
            )}
            {!loading && commands.length === 0 && (
              <tr>
                <td colSpan={6} className="py-8 text-center text-gray-500">
                  Belum ada perintah dikirim
                </td>
              </tr>
            )}
            {commands.map((cmd) => (
              <tr key={cmd.id} className="border-b border-gray-800/50 hover:bg-gray-800/40 transition-colors">
                <td className="py-2 pr-2 font-mono text-xs text-gray-500">{cmd.id}</td>
                <td className="py-2 pr-2 font-mono text-xs text-purple-400">{cmd.command_type}</td>
                <td className="py-2 pr-2 font-mono text-xs text-brand-500">{cmd.target}</td>
                <td className="py-2 pr-2 font-mono text-xs text-gray-300 max-w-[180px] truncate">
                  {typeof cmd.payload === "object"
                    ? JSON.stringify(cmd.payload)
                    : String(cmd.payload)}
                </td>
                <td className="py-2 pr-2">
                  <StatusBadge status={cmd.status} />
                </td>
                <td className="py-2 text-xs text-gray-400">{formatDate(cmd.created_at)}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>

      <div className="flex items-center justify-between mt-4">
        <button
          onClick={goPrev}
          disabled={cursors.length === 0 || loading}
          className="px-3 py-1 text-xs bg-gray-800 hover:bg-gray-700 rounded-lg disabled:opacity-40 disabled:cursor-not-allowed"
        >
          ← Sebelumnya
        </button>
        <span className="text-xs text-gray-500">Halaman {cursors.length + 1}</span>
        <button
          onClick={goNext}
          disabled={!hasMore || loading}
          className="px-3 py-1 text-xs bg-gray-800 hover:bg-gray-700 rounded-lg disabled:opacity-40 disabled:cursor-not-allowed"
        >
          Berikutnya →
        </button>
      </div>
    </div>
  );
}
