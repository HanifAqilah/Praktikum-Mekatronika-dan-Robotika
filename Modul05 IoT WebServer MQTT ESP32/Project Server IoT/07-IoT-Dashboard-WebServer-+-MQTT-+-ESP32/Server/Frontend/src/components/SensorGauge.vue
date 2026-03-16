<template>
  <div class="gauge-card">
    <div class="gauge-label">{{ label }}</div>
    <div class="gauge-ring" :style="ringStyle">
      <div class="gauge-inner">
        <span class="gauge-value">{{ displayValue }}</span>
        <span class="gauge-unit">{{ unit }}</span>
      </div>
    </div>
    <div class="gauge-range">{{ min }} – {{ max }}</div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'

interface Props {
  label: string
  value: number
  unit: string
  min: number
  max: number
  warnThreshold?: number
  critThreshold?: number
  decimals?: number
}

const props = withDefaults(defineProps<Props>(), {
  warnThreshold: undefined,
  critThreshold: undefined,
  decimals: 1,
})

const pct = computed(() => {
  const clamped = Math.max(props.min, Math.min(props.max, props.value))
  return ((clamped - props.min) / (props.max - props.min)) * 100
})

const accentColor = computed(() => {
  if (props.critThreshold !== undefined && props.value >= props.critThreshold) return '#f87171'
  if (props.warnThreshold !== undefined && props.value >= props.warnThreshold) return '#f59e0b'
  return '#4ade80'
})

const ringStyle = computed(() => {
  const p = pct.value
  const color = accentColor.value
  return {
    background: `conic-gradient(${color} ${p}%, #2a2d3a ${p}%)`,
  }
})

const displayValue = computed(() =>
  Number(props.value).toFixed(props.decimals)
)
</script>

<style scoped>
.gauge-card {
  background: #1a1d27;
  border: 1px solid #2a2d3a;
  border-radius: 12px;
  padding: 1rem;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 0.5rem;
  min-width: 140px;
}
.gauge-label {
  font-size: 0.75rem;
  text-transform: uppercase;
  letter-spacing: 0.08em;
  color: #6b7280;
  font-weight: 600;
}
.gauge-ring {
  width: 110px;
  height: 110px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: background 0.4s ease;
}
.gauge-inner {
  width: 80px;
  height: 80px;
  border-radius: 50%;
  background: #0f1117;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}
.gauge-value {
  font-size: 1.4rem;
  font-weight: 700;
  color: #e5e7eb;
  line-height: 1.1;
}
.gauge-unit {
  font-size: 0.65rem;
  color: #6b7280;
}
.gauge-range {
  font-size: 0.7rem;
  color: #4b5563;
}
</style>
