<template>
  <div id="root">
    <nav class="navbar">
      <span class="brand">🌿 IoT Dashboard – Percobaan 7</span>
      <div class="nav-links">
        <RouterLink to="/">Dashboard</RouterLink>
        <RouterLink to="/analytics">Analytics</RouterLink>
      </div>
    </nav>
    <main>
      <RouterView />
    </main>
  </div>
</template>

<script setup lang="ts">
import { RouterLink, RouterView } from 'vue-router'
</script>

<style>
* { box-sizing: border-box; margin: 0; padding: 0; }
body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: #0f1117; color: #e0e0e0; }
#root { min-height: 100vh; display: flex; flex-direction: column; }

.navbar {
  display: flex; align-items: center; justify-content: space-between;
  padding: 0.75rem 1.5rem; background: #1a1d27;
  border-bottom: 1px solid #2a2d3a; position: sticky; top: 0; z-index: 100;
}
.brand { font-size: 1.1rem; font-weight: 700; color: #4ade80; }
.nav-links { display: flex; gap: 1.5rem; }
.nav-links a { color: #9ca3af; text-decoration: none; font-size: 0.9rem; }
.nav-links a.router-link-active { color: #4ade80; font-weight: 600; }
main { flex: 1; padding: 1.5rem; }
</style>
