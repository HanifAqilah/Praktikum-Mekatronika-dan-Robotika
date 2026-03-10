import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

export default defineConfig({
  plugins: [react()],
  server: {
    port: 9008,
    host: true,
    proxy: {
      "/api": {
        target: "http://localhost:8008",
        changeOrigin: true,
      },
      "/ws": {
        target: "ws://localhost:8008",
        ws: true,
        changeOrigin: true,
      },
    },
  },
  build: {
    outDir: "dist",
    sourcemap: true,
  },
});
