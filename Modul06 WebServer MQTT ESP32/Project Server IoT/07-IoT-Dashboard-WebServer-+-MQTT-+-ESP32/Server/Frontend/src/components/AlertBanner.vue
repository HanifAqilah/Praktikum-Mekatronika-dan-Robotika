<template>
  <div v-if="alerts.length > 0" class="alert-container">
    <div
      v-for="alert in alerts"
      :key="alert.id"
      class="alert-banner"
      :class="{ resolved: alert.resolved }"
    >
      <span class="alert-icon">{{ alert.resolved ? '✅' : '⚠️' }}</span>
      <div class="alert-body">
        <span class="alert-message">{{ alert.message }}</span>
        <span class="alert-meta">
          {{ alert.device_id }} · {{ formatTime(alert.created_at) }}
        </span>
      </div>
      <button
        v-if="!alert.resolved"
        class="resolve-btn"
        @click="$emit('resolve', alert.id)"
      >Resolve</button>
    </div>
  </div>
</template>

<script setup lang="ts">
export interface AlertItem {
  id: number
  device_id: string
  field: string
  message: string
  value: number
  threshold: number
  resolved: boolean
  created_at: string
}

defineProps<{ alerts: AlertItem[] }>()
defineEmits<{ (e: 'resolve', id: number): void }>()

function formatTime(iso: string): string {
  return new Date(iso).toLocaleTimeString('id-ID')
}
</script>

<style scoped>
.alert-container {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  margin-bottom: 1rem;
}
.alert-banner {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  padding: 0.65rem 1rem;
  background: #3b1f1f;
  border: 1px solid #7f1d1d;
  border-radius: 8px;
  transition: opacity 0.3s;
}
.alert-banner.resolved {
  background: #1a2d1f;
  border-color: #166534;
  opacity: 0.7;
}
.alert-icon { font-size: 1.1rem; flex-shrink: 0; }
.alert-body { flex: 1; display: flex; flex-direction: column; }
.alert-message { font-size: 0.88rem; color: #fca5a5; font-weight: 500; }
.alert-banner.resolved .alert-message { color: #86efac; }
.alert-meta { font-size: 0.72rem; color: #6b7280; margin-top: 0.1rem; }
.resolve-btn {
  padding: 0.25rem 0.75rem;
  background: transparent;
  border: 1px solid #7f1d1d;
  border-radius: 6px;
  color: #fca5a5;
  cursor: pointer;
  font-size: 0.78rem;
  white-space: nowrap;
  transition: background 0.2s;
}
.resolve-btn:hover { background: #7f1d1d44; }
</style>
