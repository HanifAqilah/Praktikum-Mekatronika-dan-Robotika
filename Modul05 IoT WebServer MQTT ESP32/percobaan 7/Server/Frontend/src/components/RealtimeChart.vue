<template>
  <div class="chart-wrapper">
    <h3 class="chart-title">{{ title }}</h3>
    <canvas ref="canvasRef"></canvas>
  </div>
</template>

<script setup lang="ts">
import { ref, watch, onMounted, onUnmounted } from 'vue'
import {
  Chart,
  LineController,
  LineElement,
  PointElement,
  LinearScale,
  TimeScale,
  CategoryScale,
  Title,
  Tooltip,
  Legend,
  Filler,
  type ChartConfiguration,
  type ChartDataset,
} from 'chart.js'

Chart.register(
  LineController, LineElement, PointElement,
  LinearScale, TimeScale, CategoryScale,
  Title, Tooltip, Legend, Filler
)

interface DataPoint {
  x: string  // ISO timestamp or label
  y: number
}

interface Props {
  title: string
  datasets: { label: string; data: DataPoint[]; color: string }[]
  maxPoints?: number
}

const props  = withDefaults(defineProps<Props>(), { maxPoints: 60 })
const canvasRef = ref<HTMLCanvasElement | null>(null)
let chart: Chart | null = null

const COLORS = [
  '#4ade80', '#60a5fa', '#f59e0b', '#f87171',
  '#a78bfa', '#34d399', '#fb923c',
]

function buildConfig(): ChartConfiguration {
  return {
    type: 'line',
    data: {
      datasets: props.datasets.map((ds, i) => ({
        label: ds.label,
        data: ds.data as DataPoint[],
        borderColor: ds.color || COLORS[i % COLORS.length],
        backgroundColor: (ds.color || COLORS[i % COLORS.length]) + '22',
        borderWidth: 2,
        pointRadius: 2,
        tension: 0.3,
        fill: false,
        parsing: { xAxisKey: 'x', yAxisKey: 'y' },
      } as ChartDataset<'line', DataPoint[]>)),
    },
    options: {
      responsive: true,
      maintainAspectRatio: true,
      animation: false,
      plugins: {
        legend: { labels: { color: '#9ca3af', boxWidth: 14 } },
        tooltip: { mode: 'index', intersect: false },
      },
      scales: {
        x: {
          type: 'category',
          ticks: { color: '#6b7280', maxTicksLimit: 8, maxRotation: 0 },
          grid: { color: '#1f2937' },
        },
        y: {
          ticks: { color: '#6b7280' },
          grid: { color: '#1f2937' },
        },
      },
    },
  }
}

function initChart() {
  if (!canvasRef.value) return
  if (chart) { chart.destroy(); chart = null }
  chart = new Chart(canvasRef.value, buildConfig())
}

function updateChart() {
  if (!chart) return
  chart.data.datasets = props.datasets.map((ds, i) => ({
    label: ds.label,
    data: ds.data as DataPoint[],
    borderColor: ds.color || COLORS[i % COLORS.length],
    backgroundColor: (ds.color || COLORS[i % COLORS.length]) + '22',
    borderWidth: 2,
    pointRadius: 2,
    tension: 0.3,
    fill: false,
    parsing: { xAxisKey: 'x', yAxisKey: 'y' },
  } as ChartDataset<'line', DataPoint[]>))
  chart.update('none')
}

watch(() => props.datasets, updateChart, { deep: true })
onMounted(initChart)
onUnmounted(() => chart?.destroy())
</script>

<style scoped>
.chart-wrapper {
  background: #1a1d27;
  border-radius: 12px;
  padding: 1rem;
  border: 1px solid #2a2d3a;
}
.chart-title {
  font-size: 0.9rem;
  font-weight: 600;
  color: #9ca3af;
  margin-bottom: 0.75rem;
  text-transform: uppercase;
  letter-spacing: 0.05em;
}
</style>
