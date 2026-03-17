<template>
  <div class="analytics">
    <div class="analytics-header">
      <h2>📂 Analitik Data Historis</h2>
      <button class="refresh-btn" @click="fetchData" :disabled="loading">
        {{ loading ? 'Memuat...' : '🔄 Refresh' }}
      </button>
    </div>

    <!-- Bar chart: suhu last 50 -->
    <div class="chart-box">
      <h3>Suhu – 50 Pembacaan Terakhir</h3>
      <Bar :data="barChartData" :options="barChartOptions" />
    </div>

    <!-- History table -->
    <div class="table-box">
      <h3>Riwayat 20 Pembacaan Terakhir</h3>
      <div class="table-wrap">
        <table>
          <thead>
            <tr>
              <th>#</th>
              <th>Device</th>
              <th>Suhu (°C)</th>
              <th>Kelembaban (%)</th>
              <th>Cahaya</th>
              <th>Waktu</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="row in tableRows" :key="row.id">
              <td>{{ row.id }}</td>
              <td>{{ row.device_id }}</td>
              <td>{{ row.suhu.toFixed(1) }}</td>
              <td>{{ row.kelembaban.toFixed(1) }}</td>
              <td>{{ row.cahaya }}</td>
              <td>{{ formatDate(row.created_at) }}</td>
            </tr>
            <tr v-if="tableRows.length === 0">
              <td colspan="6" style="text-align:center; color:#484f58;">Belum ada data.</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import { Bar } from 'vue-chartjs'
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  BarElement,
  Title,
  Tooltip,
  Legend,
} from 'chart.js'
import axios from 'axios'
import type { SensorData } from '../composables/useSseData'

ChartJS.register(CategoryScale, LinearScale, BarElement, Title, Tooltip, Legend)

const history = ref<SensorData[]>([])
const loading = ref(false)

async function fetchData() {
  loading.value = true
  try {
    const res = await axios.get<SensorData[]>('/api/history')
    history.value = res.data
  } finally {
    loading.value = false
  }
}

onMounted(fetchData)

const chartRows = computed(() => [...history.value].reverse().slice(-50))
const tableRows = computed(() => history.value.slice(0, 20))

const barChartData = computed(() => ({
  labels: chartRows.value.map((r) => formatDate(r.created_at)),
  datasets: [
    {
      label: 'Suhu (°C)',
      data: chartRows.value.map((r) => r.suhu),
      backgroundColor: 'rgba(249, 115, 22, 0.7)',
      borderColor: '#f97316',
      borderWidth: 1,
    },
  ],
}))

const barChartOptions = {
  responsive: true,
  maintainAspectRatio: false,
  plugins: {
    legend: { labels: { color: '#8b949e' } },
  },
  scales: {
    x: {
      ticks: { color: '#8b949e', maxRotation: 45, maxTicksLimit: 15 },
      grid: { color: 'rgba(48, 54, 61, 0.8)' },
    },
    y: {
      ticks: { color: '#8b949e' },
      grid: { color: 'rgba(48, 54, 61, 0.8)' },
    },
  },
}

function formatDate(dt: string) {
  return new Date(dt).toLocaleString('id-ID', {
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
  })
}
</script>

<style scoped>
.analytics { display: flex; flex-direction: column; gap: 1.5rem; }

.analytics-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.analytics-header h2 { font-size: 1.1rem; }

.refresh-btn {
  background: #21262d;
  border: 1px solid #30363d;
  color: #e6edf3;
  padding: 0.4rem 1rem;
  border-radius: 6px;
  cursor: pointer;
  font-size: 0.9rem;
  transition: background 0.2s;
}

.refresh-btn:hover:not(:disabled) { background: #30363d; }
.refresh-btn:disabled { opacity: 0.5; cursor: default; }

.chart-box {
  background: #161b22;
  border: 1px solid #30363d;
  border-radius: 10px;
  padding: 1.25rem 1.5rem;
  height: 300px;
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

.chart-box h3 { font-size: 0.9rem; color: #8b949e; }

.table-box {
  background: #161b22;
  border: 1px solid #30363d;
  border-radius: 10px;
  padding: 1.25rem 1.5rem;
}

.table-box h3 { font-size: 0.9rem; color: #8b949e; margin-bottom: 0.75rem; }

.table-wrap { overflow-x: auto; }

table { width: 100%; border-collapse: collapse; font-size: 0.88rem; }

th {
  text-align: left;
  color: #8b949e;
  font-weight: 600;
  padding: 0.5rem 0.75rem;
  border-bottom: 1px solid #21262d;
  white-space: nowrap;
}

td {
  padding: 0.5rem 0.75rem;
  border-bottom: 1px solid #21262d;
  color: #c9d1d9;
}

tr:last-child td { border-bottom: none; }
tr:hover td { background: rgba(255, 255, 255, 0.03); }
</style>
