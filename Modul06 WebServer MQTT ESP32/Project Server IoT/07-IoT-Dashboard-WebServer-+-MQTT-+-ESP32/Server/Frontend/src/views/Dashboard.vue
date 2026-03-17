<template>
  <div class="dashboard">
    <!-- Connection status -->
    <div class="status-bar">
      <span class="dot" :class="connected ? 'online' : 'offline'"></span>
      <span>{{ connected ? 'Live – data setiap 2 detik' : 'Menghubungkan…' }}</span>
      <span v-if="lastUpdate" class="last-update">
        Update: {{ lastUpdate.toLocaleTimeString('id-ID') }}
      </span>
    </div>

    <!-- Alerts -->
    <AlertBanner :alerts="openAlerts" @resolve="resolveAlert" />

    <!-- Stats cards (last reading) -->
    <StatsCard :stats="statItems" />

    <!-- Gauges row -->
    <div v-if="latestDevice" class="gauges-row">
      <SensorGauge label="Suhu"       :value="latestDevice.suhu"       unit="°C" :min="0"    :max="50"   :warn-threshold="30" :crit-threshold="35" />
      <SensorGauge label="Kelembaban" :value="latestDevice.kelembaban" unit="%"  :min="0"    :max="100"  :warn-threshold="80" :crit-threshold="90" />
      <SensorGauge label="Cahaya"     :value="latestDevice.cahaya"     unit="ADC" :min="0"   :max="4095" :warn-threshold="3000" :crit-threshold="4000" :decimals="0" />
      <SensorGauge label="Tanah"      :value="latestDevice.tanah"      unit="ADC" :min="0"   :max="4095" :warn-threshold="2500" :crit-threshold="3000" :decimals="0" />
    </div>

    <!-- Realtime charts -->
    <div class="charts-grid">
      <RealtimeChart title="Suhu & Kelembaban" :datasets="tempHumDatasets" />
      <RealtimeChart title="Cahaya & Tanah"    :datasets="lightSoilDatasets" />
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted } from 'vue'
import { useMqttData, type SensorReading } from '@/composables/useMqttData'
import RealtimeChart from '@/components/RealtimeChart.vue'
import SensorGauge   from '@/components/SensorGauge.vue'
import AlertBanner, { type AlertItem } from '@/components/AlertBanner.vue'
import StatsCard,   { type StatItem }   from '@/components/StatsCard.vue'

const { devices, connected, lastUpdate } = useMqttData()

// Rolling history for charts
interface Point { x: string; y: number }
const MAX_PTS = 60
const histSuhu:       Point[] = []
const histKelembaban: Point[] = []
const histCahaya:     Point[] = []
const histTanah:      Point[] = []

// Reactive wrappers for chart datasets (need to be re-assigned to trigger reactivity)
const tempHumDatasets  = ref(buildTempHumDs())
const lightSoilDatasets = ref(buildLightSoilDs())

function buildTempHumDs() {
  return [
    { label: 'Suhu (°C)',       data: [...histSuhu],       color: '#f87171' },
    { label: 'Kelembaban (%)',  data: [...histKelembaban], color: '#60a5fa' },
  ]
}
function buildLightSoilDs() {
  return [
    { label: 'Cahaya (ADC)', data: [...histCahaya], color: '#f59e0b' },
    { label: 'Tanah (ADC)',  data: [...histTanah],  color: '#34d399' },
  ]
}

function pushToHistory(arr: Point[], x: string, y: number) {
  arr.push({ x, y })
  if (arr.length > MAX_PTS) arr.shift()
}

// Poll devices and update history
let pollTimer: ReturnType<typeof setInterval>

const latestDevice = computed<SensorReading | null>(() =>
  devices.value[0] ?? null
)

function updateHistory() {
  if (!latestDevice.value) return
  const d = latestDevice.value
  const ts = new Date().toLocaleTimeString('id-ID')
  pushToHistory(histSuhu,       ts, d.suhu)
  pushToHistory(histKelembaban, ts, d.kelembaban)
  pushToHistory(histCahaya,     ts, d.cahaya)
  pushToHistory(histTanah,      ts, d.tanah)
  tempHumDatasets.value  = buildTempHumDs()
  lightSoilDatasets.value = buildLightSoilDs()
}

onMounted(() => { pollTimer = setInterval(updateHistory, 2000) })
onUnmounted(() => clearInterval(pollTimer))

// Alerts
const openAlerts = ref<AlertItem[]>([])
let alertTimer: ReturnType<typeof setInterval>

async function fetchAlerts() {
  try {
    const res  = await fetch('/api/alerts?resolved=false&limit=10')
    openAlerts.value = await res.json()
  } catch { /* silently ignore */ }
}

async function resolveAlert(id: number) {
  await fetch(`/api/alerts/${id}/resolve`, { method: 'PATCH' })
  await fetchAlerts()
}

onMounted(() => {
  fetchAlerts()
  alertTimer = setInterval(fetchAlerts, 5000)
})
onUnmounted(() => clearInterval(alertTimer))

// Stats cards
const statItems = computed<StatItem[]>(() => {
  const d = latestDevice.value
  if (!d) return []
  return [
    { label: 'Suhu',       value: `${d.suhu.toFixed(1)} °C`,   icon: '🌡️',  status: d.suhu > 35 ? 'crit' : d.suhu > 30 ? 'warn' : 'normal' },
    { label: 'Kelembaban', value: `${d.kelembaban.toFixed(1)} %`, icon: '💧', status: d.kelembaban > 90 ? 'crit' : d.kelembaban > 80 ? 'warn' : 'normal' },
    { label: 'Cahaya',     value: d.cahaya,                      icon: '☀️',  status: d.cahaya > 4000 ? 'crit' : d.cahaya > 3000 ? 'warn' : 'normal' },
    { label: 'Tanah',      value: d.tanah,                       icon: '🌱',  status: d.tanah > 3000 ? 'crit' : d.tanah > 2500 ? 'warn' : 'normal' },
    { label: 'Perangkat',  value: d.device_id,                   icon: '📡',  sub: connected.value ? 'Online' : 'Offline' },
  ]
})
</script>

<style scoped>
.dashboard { display: flex; flex-direction: column; gap: 1rem; }
.status-bar {
  display: flex; align-items: center; gap: 0.6rem;
  font-size: 0.82rem; color: #6b7280;
}
.dot {
  width: 9px; height: 9px; border-radius: 50%;
  display: inline-block; flex-shrink: 0;
}
.dot.online  { background: #4ade80; box-shadow: 0 0 6px #4ade80; }
.dot.offline { background: #f87171; }
.last-update { margin-left: auto; }

.gauges-row {
  display: flex; gap: 0.75rem; flex-wrap: wrap;
}

.charts-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 1rem;
}
@media (max-width: 700px) {
  .charts-grid { grid-template-columns: 1fr; }
}
</style>
