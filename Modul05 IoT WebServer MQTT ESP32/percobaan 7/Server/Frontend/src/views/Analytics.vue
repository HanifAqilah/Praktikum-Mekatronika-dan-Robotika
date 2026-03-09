<template>
  <div class="analytics">
    <h2 class="page-title">📊 Analytics</h2>

    <!-- Timeframe selector -->
    <div class="toolbar">
      <button
        v-for="tf in timeframes"
        :key="tf.key"
        class="tf-btn"
        :class="{ active: selected === tf.key }"
        @click="switchTimeframe(tf.key)"
      >{{ tf.label }}</button>

      <select v-model="selectedDevice" class="device-select" @change="loadData">
        <option value="">Semua Perangkat</option>
        <option v-for="d in knownDevices" :key="d" :value="d">{{ d }}</option>
      </select>

      <button class="refresh-btn" @click="loadData" :disabled="loading">
        {{ loading ? '⏳' : '🔄' }} Refresh
      </button>
    </div>

    <div v-if="loading" class="loading">Memuat data…</div>
    <div v-else-if="error" class="error">{{ error }}</div>

    <template v-else>
      <!-- Today stats -->
      <StatsCard :stats="todayStats" />

      <!-- Historical charts -->
      <div class="charts-grid">
        <RealtimeChart title="Suhu – Rata-rata per periode"       :datasets="suhuDs"       />
        <RealtimeChart title="Kelembaban – Rata-rata per periode" :datasets="kelDs"        />
        <RealtimeChart title="Cahaya – Rata-rata per periode"     :datasets="cahayaDs"     />
        <RealtimeChart title="Tanah – Rata-rata per periode"      :datasets="tanahDs"      />
      </div>

      <!-- Stats table -->
      <div class="table-wrapper">
        <h3 class="section-title">Data Agregasi</h3>
        <table class="agg-table">
          <thead>
            <tr>
              <th>Bucket</th>
              <th>Suhu Avg</th>
              <th>Suhu Min</th>
              <th>Suhu Max</th>
              <th>Kelembaban Avg</th>
              <th>Cahaya Avg</th>
              <th>Tanah Avg</th>
              <th>Count</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="row in aggregated" :key="row.bucket">
              <td>{{ formatBucket(row.bucket) }}</td>
              <td>{{ row.suhu_avg }}°C</td>
              <td>{{ row.suhu_min }}°C</td>
              <td>{{ row.suhu_max }}°C</td>
              <td>{{ row.kelembaban_avg }}%</td>
              <td>{{ row.cahaya_avg }}</td>
              <td>{{ row.tanah_avg }}</td>
              <td>{{ row.count }}</td>
            </tr>
            <tr v-if="aggregated.length === 0">
              <td colspan="8" class="no-data">Belum ada data</td>
            </tr>
          </tbody>
        </table>
      </div>
    </template>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import RealtimeChart from '@/components/RealtimeChart.vue'
import StatsCard, { type StatItem } from '@/components/StatsCard.vue'

interface AggRow {
  bucket: string
  suhu_avg: number
  suhu_min: number
  suhu_max: number
  kelembaban_avg: number
  cahaya_avg: number
  tanah_avg: number
  count: number
}

type Timeframe = 'minutes' | 'hours'

const timeframes = [
  { key: 'minutes' as Timeframe, label: 'Menit (1 jam terakhir)' },
  { key: 'hours'   as Timeframe, label: 'Jam (24 jam terakhir)' },
]

const selected       = ref<Timeframe>('hours')
const selectedDevice = ref('')
const aggregated     = ref<AggRow[]>([])
const todayRaw       = ref<Record<string, unknown>>({})
const knownDevices   = ref<string[]>([])
const loading        = ref(false)
const error          = ref<string | null>(null)

async function loadData() {
  loading.value = true
  error.value = null
  try {
    const dev = selectedDevice.value ? `&device=${selectedDevice.value}` : ''
    const [aggRes, statsRes] = await Promise.all([
      fetch(`/api/sensors/aggregate/${selected.value}?${dev}`),
      fetch(`/api/sensors/stats/today?${dev}`),
    ])
    aggregated.value = await aggRes.json()
    todayRaw.value   = await statsRes.json()
  } catch (e) {
    error.value = String(e)
  } finally {
    loading.value = false
  }
}

async function loadDevices() {
  try {
    const res = await fetch('/api/sensors/latest?limit=200')
    const rows: { device_id: string }[] = await res.json()
    knownDevices.value = [...new Set(rows.map(r => r.device_id))]
  } catch { /* ignore */ }
}

function switchTimeframe(tf: Timeframe) {
  selected.value = tf
  loadData()
}

function formatBucket(b: string): string {
  if (!b) return '–'
  return b.replace('T', ' ').substring(0, 16)
}

// Build chart datasets from aggregated data
function makeDs(field: keyof AggRow, label: string, color: string) {
  return [{
    label,
    color,
    data: aggregated.value.map(r => ({
      x: formatBucket(r.bucket),
      y: Number(r[field]),
    })),
  }]
}

const suhuDs    = computed(() => makeDs('suhu_avg',       'Suhu Avg (°C)',        '#f87171'))
const kelDs     = computed(() => makeDs('kelembaban_avg', 'Kelembaban Avg (%)',   '#60a5fa'))
const cahayaDs  = computed(() => makeDs('cahaya_avg',     'Cahaya Avg (ADC)',     '#f59e0b'))
const tanahDs   = computed(() => makeDs('tanah_avg',      'Tanah Avg (ADC)',      '#34d399'))

const todayStats = computed<StatItem[]>(() => {
  const s = todayRaw.value as {
    suhu?: { avg: number; min: number; max: number }
    kelembaban?: { avg: number; min: number; max: number }
    cahaya?: { avg: number }
    tanah?: { avg: number }
    total_readings?: number
  }
  if (!s.suhu) return []
  return [
    { label: 'Suhu Hari Ini',       value: `${s.suhu.avg}°C`,           icon: '🌡️', sub: `Min ${s.suhu.min}°C · Max ${s.suhu.max}°C` },
    { label: 'Kelembaban Hari Ini', value: `${s.kelembaban?.avg ?? 0}%`, icon: '💧', sub: `Min ${s.kelembaban?.min}% · Max ${s.kelembaban?.max}%` },
    { label: 'Cahaya Avg',          value: String(s.cahaya?.avg ?? 0),   icon: '☀️' },
    { label: 'Tanah Avg',           value: String(s.tanah?.avg ?? 0),    icon: '🌱' },
    { label: 'Total Readings',      value: String(s.total_readings ?? 0),icon: '📋' },
  ]
})

onMounted(() => {
  loadDevices()
  loadData()
})
</script>

<style scoped>
.analytics { display: flex; flex-direction: column; gap: 1rem; }
.page-title { font-size: 1.2rem; font-weight: 700; color: #e5e7eb; margin-bottom: 0.25rem; }
.toolbar { display: flex; align-items: center; gap: 0.5rem; flex-wrap: wrap; }

.tf-btn {
  padding: 0.35rem 0.9rem; background: #1a1d27; border: 1px solid #2a2d3a;
  border-radius: 6px; color: #9ca3af; cursor: pointer; font-size: 0.82rem;
  transition: all 0.2s;
}
.tf-btn.active, .tf-btn:hover { border-color: #4ade80; color: #4ade80; }

.device-select {
  padding: 0.35rem 0.6rem; background: #1a1d27; border: 1px solid #2a2d3a;
  border-radius: 6px; color: #9ca3af; font-size: 0.82rem;
}

.refresh-btn {
  padding: 0.35rem 0.9rem; background: #1a1d27; border: 1px solid #4ade8044;
  border-radius: 6px; color: #4ade80; cursor: pointer; font-size: 0.82rem;
}
.refresh-btn:disabled { opacity: 0.5; cursor: not-allowed; }

.charts-grid {
  display: grid; grid-template-columns: 1fr 1fr; gap: 1rem;
}
@media (max-width: 700px) { .charts-grid { grid-template-columns: 1fr; } }

.loading, .error { padding: 2rem; text-align: center; color: #6b7280; }
.error { color: #f87171; }

.section-title { font-size: 0.9rem; font-weight: 600; color: #6b7280; margin-bottom: 0.5rem; text-transform: uppercase; }
.table-wrapper { overflow-x: auto; }
.agg-table { width: 100%; border-collapse: collapse; font-size: 0.8rem; }
.agg-table th, .agg-table td {
  padding: 0.5rem 0.75rem; text-align: left;
  border-bottom: 1px solid #1f2937;
}
.agg-table th { color: #6b7280; background: #1a1d27; }
.agg-table td { color: #d1d5db; }
.agg-table tr:hover td { background: #1f2937; }
.no-data { text-align: center; color: #4b5563; padding: 1.5rem; }
</style>
