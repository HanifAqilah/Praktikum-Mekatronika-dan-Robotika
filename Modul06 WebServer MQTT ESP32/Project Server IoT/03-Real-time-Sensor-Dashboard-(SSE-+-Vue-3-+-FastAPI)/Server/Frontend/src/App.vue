<template>
  <div class="app-shell">
    <header class="app-header">
      <div class="header-inner">
        <span class="logo">🌐 IoT Sensor Monitor</span>
        <nav class="nav-tabs">
          <button
            :class="['tab-btn', activeView === 'dashboard' ? 'active' : '']"
            @click="activeView = 'dashboard'"
          >Dashboard</button>
          <button
            :class="['tab-btn', activeView === 'analytics' ? 'active' : '']"
            @click="activeView = 'analytics'"
          >Analytics</button>
        </nav>
      </div>
    </header>

    <main class="main-content">
      <Dashboard v-if="activeView === 'dashboard'" />
      <Analytics v-if="activeView === 'analytics'" />
    </main>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import Dashboard from './views/Dashboard.vue'
import Analytics from './views/Analytics.vue'

const activeView = ref<'dashboard' | 'analytics'>('dashboard')
</script>

<style>
*, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

body {
  background: #0d1117;
  color: #e6edf3;
  font-family: 'Segoe UI', system-ui, sans-serif;
  min-height: 100vh;
}

.app-shell {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
}

.app-header {
  background: #161b22;
  border-bottom: 1px solid #30363d;
  position: sticky;
  top: 0;
  z-index: 100;
}

.header-inner {
  max-width: 1280px;
  margin: 0 auto;
  padding: 0.75rem 1.5rem;
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.logo {
  font-size: 1.1rem;
  font-weight: 700;
  color: #58a6ff;
}

.nav-tabs { display: flex; gap: 0.5rem; }

.tab-btn {
  background: transparent;
  border: 1px solid transparent;
  color: #8b949e;
  padding: 0.4rem 1rem;
  border-radius: 6px;
  cursor: pointer;
  font-size: 0.9rem;
  transition: all 0.2s;
}

.tab-btn:hover { color: #e6edf3; border-color: #30363d; }

.tab-btn.active {
  color: #58a6ff;
  border-color: #58a6ff;
  background: rgba(88, 166, 255, 0.1);
}

.main-content {
  flex: 1;
  max-width: 1280px;
  margin: 0 auto;
  width: 100%;
  padding: 1.5rem;
}
</style>
