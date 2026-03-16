import { useState, useEffect, useCallback } from 'react';
import axios from 'axios';

export interface Alert {
  id: number; device_id: string; field: string; condition: string;
  value: number; threshold: number; severity: string; status: string;
  created_at: string; acknowledged_at?: string; resolved_at?: string;
}

export function useAlerts(severity?: string) {
  const [alerts, setAlerts] = useState<Alert[]>([]);
  const [loading, setLoading] = useState(false);
  const [page, setPage] = useState(0);

  const fetch = useCallback(async () => {
    setLoading(true);
    try {
      const params: Record<string, string | number> = { skip: page * 20, limit: 20 };
      if (severity) params.severity = severity;
      const { data } = await axios.get('/api/v1/alerts', { params });
      setAlerts(data);
    } catch {} finally { setLoading(false); }
  }, [page, severity]);

  useEffect(() => { fetch(); }, [fetch]);

  const acknowledge = async (id: number) => {
    await axios.patch(`/api/v1/alerts/${id}/acknowledge`);
    fetch();
  };

  const resolve = async (id: number) => {
    await axios.patch(`/api/v1/alerts/${id}/resolve`);
    fetch();
  };

  const bulkAcknowledge = async (ids: number[]) => {
    await Promise.all(ids.map((id) => axios.patch(`/api/v1/alerts/${id}/acknowledge`)));
    fetch();
  };

  return { alerts, loading, page, setPage, fetch, acknowledge, resolve, bulkAcknowledge };
}
