import { useState, useEffect, useCallback } from 'react';
import axios from 'axios';

export interface SensorReading {
  id: number; device_id: string; suhu: number; kelembaban: number;
  cahaya: number; tanah: number; created_at: string;
}

export function useSensorHistory(limit = 20) {
  const [data, setData] = useState<SensorReading[]>([]);
  const [cursor, setCursor] = useState<number | null>(null);
  const [loading, setLoading] = useState(false);
  const [hasMore, setHasMore] = useState(true);

  const fetch = useCallback(async (reset = false) => {
    setLoading(true);
    try {
      const params: Record<string, string | number> = { limit };
      if (!reset && cursor) params.cursor = cursor;
      const { data: rows } = await axios.get('/api/v1/sensors/history', { params });
      if (reset) { setData(rows); } else { setData((prev) => [...prev, ...rows]); }
      if (rows.length > 0) setCursor(rows[rows.length - 1].id);
      setHasMore(rows.length === limit);
    } catch {} finally { setLoading(false); }
  }, [cursor, limit]);

  useEffect(() => { fetch(true); }, []);

  return { data, loading, hasMore, loadMore: () => fetch(false), refresh: () => fetch(true) };
}
