<template>
  <div class="chart-wrapper">
    <h3 class="chart-title">📈 Realtime Suhu &amp; Kelembaban</h3>
    <Line :data="chartData" :options="chartOptions" />
  </div>
</template>

<script setup lang="ts">
import { computed, watch, ref } from 'vue'
import { Line } from 'vue-chartjs'
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler,
} from 'chart.js'
import type { SensorData } from '../composables/useSseData'

ChartJS.register(CategoryScale, LinearScale, PointElement, LineElement, Title, Tooltip, Legend, Filler)

const props = defineProps<{
  latestData: SensorData | null
}>()

const MAX_POINTS = 20

interface DataPoint {
  label: string
  suhu: number
  kelembaban: number
}

const history = ref<DataPoint[]>([])

watch(
  () => props.latestData,
  (val) => {
    if (!val) return
    history.value.push({
      label: new Date(val.created_at).toLocaleTimeString('id-ID'),
      suhu: val.suhu,
      kelembaban: val.kelembaban,
    })
    if (history.value.length > MAX_POINTS) {
      history.value.shift()
    }
  }
)

const chartData = computed(() => ({
  labels: history.value.map((d) => d.label),
  datasets: [
    {
      label: 'Suhu (°C)',
      data: history.value.map((d) => d.suhu),
      borderColor: '#f97316',
      backgroundColor: 'rgba(249, 115, 22, 0.15)',
      borderWidth: 2,
      pointRadius: 3,
      fill: true,
      tension: 0.4,
    },
    {
      label: 'Kelembaban (%)',
      data: history.value.map((d) => d.kelembaban),
      borderColor: '#38bdf8',
      backgroundColor: 'rgba(56, 189, 248, 0.15)',
      borderWidth: 2,
      pointRadius: 3,
      fill: true,
      tension: 0.4,
    },
  ],
}))

const chartOptions = {
  responsive: true,
  maintainAspectRatio: false,
  animation: { duration: 300 },
  plugins: {
    legend: {
      labels: { color: '#8b949e' },
    },
  },
  scales: {
    x: {
      ticks: { color: '#8b949e', maxRotation: 0 },
      grid: { color: 'rgba(48, 54, 61, 0.8)' },
    },
    y: {
      ticks: { color: '#8b949e' },
      grid: { color: 'rgba(48, 54, 61, 0.8)' },
    },
  },
}
</script>

<style scoped>
.chart-wrapper {
  background: #161b22;
  border: 1px solid #30363d;
  border-radius: 10px;
  padding: 1.25rem 1.5rem;
  height: 280px;
  display: flex;
  flex-direction: column;
  gap: 0.75rem;
}

.chart-title {
  font-size: 0.95rem;
  color: #8b949e;
  font-weight: 600;
}
</style>
