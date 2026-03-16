// ============================================================
//  main.js – Dashboard JS untuk Percobaan 2 (DataLogger)
//  Built by Parcel from Server/Frontend/src/
// ============================================================

'use strict';

const API = '';   // same origin (served by Express)

// ── State ─────────────────────────────────────────────────────
let historyCache = [];

// ── Clock ──────────────────────────────────────────────────────
function updateClock() {
  document.getElementById('clock').textContent =
    new Date().toLocaleTimeString('id-ID', { hour12: false });
}
setInterval(updateClock, 1000);
updateClock();

// ── Online / Offline badge ─────────────────────────────────────
function setOnline(online) {
  const badge = document.getElementById('status-badge');
  const text  = document.getElementById('status-text');
  badge.className = online ? 'online' : 'offline';
  text.textContent = online ? 'Online' : 'Offline';
}

// ── Error box ──────────────────────────────────────────────────
function showError(msg) {
  const box = document.getElementById('error-box');
  box.textContent = '⚠️ ' + msg;
  box.classList.remove('hidden');
}
function clearError() {
  document.getElementById('error-box').classList.add('hidden');
}

// ── Format helpers ─────────────────────────────────────────────
function fmtNum(v, decimals = 1) {
  return v !== null && v !== undefined ? parseFloat(v).toFixed(decimals) : '--';
}

function fmtDate(iso) {
  if (!iso) return '–';
  return new Date(iso).toLocaleString('id-ID', { hour12: false });
}

// ── Render latest reading ──────────────────────────────────────
function renderLatest(row) {
  document.getElementById('val-suhu').innerHTML =
    `${fmtNum(row.suhu)}<span class="card-unit">°C</span>`;

  document.getElementById('val-kelembaban').innerHTML =
    `${fmtNum(row.kelembaban)}<span class="card-unit">%</span>`;

  document.getElementById('val-cahaya').innerHTML =
    `${row.cahaya ?? '--'}<span class="card-unit">ADC</span>`;

  document.getElementById('sub-suhu').textContent =
    row.created_at ? `Update: ${fmtDate(row.created_at)}` : '–';

  document.getElementById('sub-kelembaban').textContent =
    `ID: ${row.id ?? '–'}`;

  document.getElementById('sub-device').textContent =
    `Device: ${row.device_id || '–'}`;

  document.getElementById('last-update').textContent = fmtDate(row.created_at);
}

// ── Render stats ───────────────────────────────────────────────
function renderStats(stats) {
  document.getElementById('stat-avg-suhu').textContent = fmtNum(stats.avg_suhu);
  document.getElementById('stat-max-suhu').textContent = fmtNum(stats.max_suhu);
  document.getElementById('stat-min-suhu').textContent = fmtNum(stats.min_suhu);
  document.getElementById('stat-avg-kel').textContent  = fmtNum(stats.avg_kelembaban);
  document.getElementById('stat-count').textContent    = stats.count ?? '--';
}

// ── Render history table ───────────────────────────────────────
function renderHistory(rows) {
  historyCache = rows;
  const tbody = document.getElementById('history-body');

  if (!rows || rows.length === 0) {
    tbody.innerHTML = '<tr><td colspan="6" class="empty-row">Belum ada data.</td></tr>';
    return;
  }

  tbody.innerHTML = rows.map((r, i) => `
    <tr>
      <td>${r.id}</td>
      <td>${fmtDate(r.created_at)}</td>
      <td>${r.device_id || '–'}</td>
      <td>${fmtNum(r.suhu)}</td>
      <td>${fmtNum(r.kelembaban)}</td>
      <td>${r.cahaya ?? '--'}</td>
    </tr>
  `).join('');
}

// ── Export CSV ─────────────────────────────────────────────────
window.exportCSV = function exportCSV() {
  if (historyCache.length === 0) {
    showError('Tidak ada data untuk diekspor.');
    return;
  }

  const header = ['id', 'created_at', 'device_id', 'suhu', 'kelembaban', 'cahaya'];
  const rows = historyCache.map(r =>
    [r.id, r.created_at, r.device_id, r.suhu, r.kelembaban, r.cahaya].join(',')
  );

  const csv     = [header.join(','), ...rows].join('\n');
  const blob    = new Blob([csv], { type: 'text/csv;charset=utf-8;' });
  const url     = URL.createObjectURL(blob);
  const link    = document.createElement('a');
  const ts      = new Date().toISOString().slice(0, 19).replace(/[:T]/g, '-');

  link.href     = url;
  link.download = `sensor_data_${ts}.csv`;
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
  URL.revokeObjectURL(url);
};

// ── API fetchers ───────────────────────────────────────────────
async function fetchLatest() {
  try {
    const resp = await fetch(`${API}/api/latest`);
    if (resp.status === 404) return; // no data yet
    if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
    const row = await resp.json();
    renderLatest(row);
    clearError();
    setOnline(true);
  } catch (err) {
    setOnline(false);
    showError(`Gagal mengambil data terbaru: ${err.message}`);
  }
}

async function fetchHistory() {
  try {
    const resp = await fetch(`${API}/api/history?limit=20`);
    if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
    const json = await resp.json();
    renderHistory(json.data || []);
  } catch (err) {
    console.error('[fetchHistory]', err.message);
  }
}

async function fetchStats() {
  try {
    const resp = await fetch(`${API}/api/stats`);
    if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
    const stats = await resp.json();
    renderStats(stats);
  } catch (err) {
    console.error('[fetchStats]', err.message);
  }
}

async function refreshAll() {
  await Promise.all([fetchLatest(), fetchHistory(), fetchStats()]);
}

// ── Auto-refresh every 2 seconds ──────────────────────────────
refreshAll();
setInterval(refreshAll, 2000);
