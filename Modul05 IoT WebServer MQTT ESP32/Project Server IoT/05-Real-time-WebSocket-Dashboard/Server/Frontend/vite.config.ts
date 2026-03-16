import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

export default defineConfig({
  plugins: [react()],
  server: {
    port: 9005,
    proxy: {
      '/api': {
        target: 'http://localhost:8005',
        changeOrigin: true,
      },
      '/ws': {
        target: 'ws://localhost:8005',
        ws: true,
        changeOrigin: true,
      },
    },
  },
});
