<template>
  <div class="sensor-card" :style="{ '--card-color': color }">
    <div class="card-icon">{{ icon }}</div>
    <div class="card-body">
      <div class="card-title">{{ title }}</div>
      <div class="card-value">
        <span class="value-number">{{ displayValue }}</span>
        <span class="value-unit">{{ unit }}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'

const props = defineProps<{
  title: string
  value: number | null
  unit: string
  icon: string
  color: string
}>()

const displayValue = computed(() =>
  props.value !== null && props.value !== undefined
    ? Number(props.value).toFixed(1)
    : '--'
)
</script>

<style scoped>
.sensor-card {
  background: #161b22;
  border: 1px solid #30363d;
  border-top: 3px solid var(--card-color, #58a6ff);
  border-radius: 10px;
  padding: 1.25rem 1.5rem;
  display: flex;
  align-items: center;
  gap: 1rem;
  transition: transform 0.15s, box-shadow 0.15s;
}

.sensor-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
}

.card-icon {
  font-size: 2rem;
  flex-shrink: 0;
}

.card-body {
  flex: 1;
}

.card-title {
  font-size: 0.8rem;
  text-transform: uppercase;
  letter-spacing: 0.05em;
  color: #8b949e;
  margin-bottom: 0.25rem;
}

.card-value {
  display: flex;
  align-items: baseline;
  gap: 0.25rem;
}

.value-number {
  font-size: 2rem;
  font-weight: 700;
  color: var(--card-color, #e6edf3);
  transition: color 0.3s;
}

.value-unit {
  font-size: 0.9rem;
  color: #8b949e;
}
</style>
