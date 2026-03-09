import { ref, onMounted, onUnmounted } from 'vue'

export interface SensorData {
  id: number
  device_id: string
  suhu: number
  kelembaban: number
  cahaya: number
  created_at: string
}

export interface LatestReadings {
  [device_id: string]: SensorData
}

export function useSseData() {
  const latest = ref<LatestReadings>({})
  const connected = ref(false)
  const error = ref<string | null>(null)

  let es: EventSource | null = null
  let reconnectTimer: ReturnType<typeof setTimeout> | null = null

  function connect() {
    if (es) {
      es.close()
    }

    es = new EventSource('/api/stream')

    es.onopen = () => {
      connected.value = true
      error.value = null
    }

    es.onmessage = (event: MessageEvent) => {
      try {
        latest.value = JSON.parse(event.data) as LatestReadings
      } catch {
        // ignore malformed frames
      }
    }

    es.onerror = () => {
      connected.value = false
      error.value = 'Koneksi SSE terputus. Mencoba ulang...'
      es?.close()
      es = null
      reconnectTimer = setTimeout(() => {
        connect()
      }, 3000)
    }
  }

  onMounted(() => {
    connect()
  })

  onUnmounted(() => {
    if (reconnectTimer) clearTimeout(reconnectTimer)
    es?.close()
  })

  return { latest, connected, error }
}
