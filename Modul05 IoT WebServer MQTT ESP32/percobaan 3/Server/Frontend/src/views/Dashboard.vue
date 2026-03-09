<template>
  <div class="dashboard">
    <!-- Status bar -->
    <div class="status-bar">
      <span :class="['status-badge', connected ? 'connected' : 'disconnected']">
        {{ connected ? '🟢 SSE Terhubung' : '🔴 SSE Terputus' }}
      </span>
      <span v-if="error" class="status-error">{{ error }}</span>
      <span class="last-update">
        <template v-if="firstDevice">
          Terakhir diperbarui: {{ lastUpdateTime }}
        </template>
        <template v-else>Menunggu data...</template>
      </span>
    </div>

    <!-- Sensor cards -->
    <div class="cards-grid">
      <SensorCard
        title="Suhu"
        :value="firstDevice?.suhu ?? null"
        unit="°C"
        icon="🌡️"
        color="#f97316"
      />
      <SensorCard
        title="Kelembaban"
        :value="firstDevice?.kelembaban ?? null"
        unit="%"
        icon="💧"
        color="#38bdf8"
      />
      <SensorCard
        title="Cahaya"
        :value="firstDevice?.cahaya ?? null"
        unit="ADC"
        icon="☀️"
        color="#facc15"
      />
    </div>

    <!-- Realtime chart -->
    <RealtimeChart :latestData="firstDevice ?? null" />

    <!-- Stats panel -->
    <StatsPanel />
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { useSseData } from '../composables/useSseData'
import SensorCard from '../components/SensorCard.vue'
import RealtimeChart from '../components/RealtimeChart.vue'
import StatsPanel from '../components/StatsPanel.vue'

const { latest, connected, error } = useSseData()

const firstDevice = computed(() => {
  const keys = Object.keys(latest.value)
  return keys.length > 0 ? latest.value[keys[0]] : null
})

const lastUpdateTime = computed(() => {
  if (!firstDevice.value?.created_at) return ''
  return new Date(firstDevice.value.created_at).toLocaleTimeString('id-ID')
})
</script>

<style scoped>
.dashboard { display: flex; flex-direction: column; gap: 1.25rem; }

.status-bar {
  display: flex;
  align-items: center;
  gap: 1rem;
  flex-wrap: wrap;
}

.status-badge {
  padding: 0.3rem 0.75rem;
  border-radius: 999px;
  font-size: 0.82rem;
  font-weight: 600;
}

.status-badge.connected   { background: rgba(74, 222, 128, 0.15); color: #4ade80; border: 1px solid #4ade80; }
.status-badge.disconnected { background: rgba(248, 113, 113, 0.15); color: #f87171; border: 1px solid #f87171; }

.status-error { color: #f87171; font-size: 0.82rem; }

.last-update { margin-left: auto; font-size: 0.82rem; color: #484f58; }

.cards-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
  gap: 1rem;
}
</style>
