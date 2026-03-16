<template>
  <div class="stats-panel">
    <div class="stats-header">
      <h3>📊 Statistik 24 Jam Terakhir</h3>
      <span class="refresh-hint">Auto-refresh 30s</span>
    </div>
    <div v-if="loading" class="stats-loading">Memuat statistik...</div>
    <div v-else-if="stats" class="stats-grid">
      <div class="stat-item">
        <span class="stat-label">Rata-rata Suhu</span>
        <span class="stat-value orange">{{ stats.avg_suhu ?? '--' }} °C</span>
      </div>
      <div class="stat-item">
        <span class="stat-label">Rata-rata Kelembaban</span>
        <span class="stat-value blue">{{ stats.avg_kelembaban ?? '--' }} %</span>
      </div>
      <div class="stat-item">
        <span class="stat-label">Suhu Maks</span>
        <span class="stat-value red">{{ stats.max_suhu ?? '--' }} °C</span>
      </div>
      <div class="stat-item">
        <span class="stat-label">Suhu Min</span>
        <span class="stat-value green">{{ stats.min_suhu ?? '--' }} °C</span>
      </div>
      <div class="stat-item">
        <span class="stat-label">Total Pembacaan</span>
        <span class="stat-value purple">{{ stats.count ?? 0 }}</span>
      </div>
    </div>
    <div v-else class="stats-empty">Belum ada data statistik.</div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import axios from 'axios'

interface Stats {
  avg_suhu: number | null
  avg_kelembaban: number | null
  avg_cahaya: number | null
  max_suhu: number | null
  min_suhu: number | null
  count: number
}

const stats = ref<Stats | null>(null)
const loading = ref(true)
let timer: ReturnType<typeof setInterval> | null = null

async function fetchStats() {
  try {
    const res = await axios.get<Stats>('/api/stats')
    stats.value = res.data
  } catch {
    // silently ignore; keep stale data
  } finally {
    loading.value = false
  }
}

onMounted(() => {
  fetchStats()
  timer = setInterval(fetchStats, 30000)
})

onUnmounted(() => {
  if (timer) clearInterval(timer)
})
</script>

<style scoped>
.stats-panel {
  background: #161b22;
  border: 1px solid #30363d;
  border-radius: 10px;
  padding: 1.25rem 1.5rem;
}

.stats-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 1rem;
}

.stats-header h3 { font-size: 0.95rem; color: #8b949e; }

.refresh-hint { font-size: 0.75rem; color: #484f58; }

.stats-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(150px, 1fr));
  gap: 0.75rem;
}

.stat-item {
  display: flex;
  flex-direction: column;
  gap: 0.25rem;
  background: #0d1117;
  border: 1px solid #21262d;
  border-radius: 8px;
  padding: 0.75rem;
}

.stat-label { font-size: 0.75rem; color: #8b949e; text-transform: uppercase; letter-spacing: 0.04em; }
.stat-value { font-size: 1.25rem; font-weight: 700; }
.stat-value.orange { color: #f97316; }
.stat-value.blue   { color: #38bdf8; }
.stat-value.red    { color: #f87171; }
.stat-value.green  { color: #4ade80; }
.stat-value.purple { color: #a78bfa; }

.stats-loading, .stats-empty { color: #484f58; font-size: 0.9rem; }
</style>
