import { createApp } from 'vue'
import { createRouter, createWebHistory } from 'vue-router'
import App from './App.vue'
import Dashboard from './views/Dashboard.vue'
import Analytics from './views/Analytics.vue'

const router = createRouter({
  history: createWebHistory(),
  routes: [
    { path: '/',          component: Dashboard, name: 'dashboard' },
    { path: '/analytics', component: Analytics, name: 'analytics' },
  ],
})

createApp(App).use(router).mount('#app')
