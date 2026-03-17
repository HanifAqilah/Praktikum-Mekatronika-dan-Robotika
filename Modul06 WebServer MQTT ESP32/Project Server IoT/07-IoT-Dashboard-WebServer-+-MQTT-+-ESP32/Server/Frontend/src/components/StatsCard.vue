<template>
  <div class="stats-grid">
    <div
      v-for="stat in stats"
      :key="stat.label"
      class="stat-card"
      :class="stat.status"
    >
      <div class="stat-icon">{{ stat.icon }}</div>
      <div class="stat-info">
        <div class="stat-value">{{ stat.value }}</div>
        <div class="stat-label">{{ stat.label }}</div>
      </div>
      <div v-if="stat.sub" class="stat-sub">{{ stat.sub }}</div>
    </div>
  </div>
</template>

<script setup lang="ts">
export interface StatItem {
  label: string
  value: string | number
  icon: string
  sub?: string
  status?: 'normal' | 'warn' | 'crit'
}
defineProps<{ stats: StatItem[] }>()
</script>

<style scoped>
.stats-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(160px, 1fr));
  gap: 0.75rem;
  margin-bottom: 1rem;
}
.stat-card {
  background: #1a1d27;
  border: 1px solid #2a2d3a;
  border-radius: 10px;
  padding: 0.85rem 1rem;
  display: flex;
  flex-direction: column;
  gap: 0.3rem;
  transition: border-color 0.3s;
}
.stat-card.warn  { border-color: #d97706; }
.stat-card.crit  { border-color: #dc2626; }
.stat-icon { font-size: 1.4rem; }
.stat-value { font-size: 1.5rem; font-weight: 700; color: #e5e7eb; line-height: 1.1; }
.stat-label { font-size: 0.72rem; color: #6b7280; text-transform: uppercase; letter-spacing: 0.06em; }
.stat-sub { font-size: 0.7rem; color: #4b5563; margin-top: 0.1rem; }
</style>
