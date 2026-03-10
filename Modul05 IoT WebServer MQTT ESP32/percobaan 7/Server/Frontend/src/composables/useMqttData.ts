/**
 * useMqttData – SSE consumer composable
 * Connects to /api/stream (Server-Sent Events) and keeps
 * reactive state for all connected devices.
 */

import { ref, onMounted, onUnmounted } from 'vue'

export interface SensorReading {
  id: number
  device_id: string
  suhu: number
  kelembaban: number
  cahaya: number
  tanah: number
  created_at: string
}

export interface StreamPayload {
  devices: SensorReading[]
  count: number
}

export function useMqttData() {
  const devices   = ref<SensorReading[]>([])
  const connected = ref(false)
  const lastUpdate = ref<Date | null>(null)
  const error     = ref<string | null>(null)

  let es: EventSource | null = null

  function connect() {
    if (es) es.close()

    es = new EventSource('/api/stream')

    es.onopen = () => {
      connected.value = true
      error.value = null
    }

    es.onmessage = (evt) => {
      try {
        const payload: StreamPayload = JSON.parse(evt.data)
        devices.value  = payload.devices
        lastUpdate.value = new Date()
        error.value    = null
      } catch {
        error.value = 'Invalid data from stream'
      }
    }

    es.onerror = () => {
      connected.value = false
      error.value = 'SSE connection lost – reconnecting…'
      // EventSource reconnects automatically; we just update state
    }
  }

  function disconnect() {
    es?.close()
    connected.value = false
  }

  onMounted(connect)
  onUnmounted(disconnect)

  return { devices, connected, lastUpdate, error }
}
