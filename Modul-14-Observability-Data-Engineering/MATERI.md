# MODUL 14: OBSERVABILITY & DATA ENGINEERING

## 1. DEFINISI

- **Observability**: kemampuan memahami kondisi sistem melalui metric, log, dan trace.
- **Time-series data**: data berurutan berbasis timestamp.
- **KPI**: metrik kinerja utama (availability, efficiency, energy).

---

## 2. KONSEP INTI

### 2.1 Time-Series Database
InfluxDB/TimescaleDB dioptimasi untuk ingest dan query data bertimestamp.

### 2.2 Grafana
Dashboard untuk visualisasi metric (gauge, graph, heatmap).

### 2.3 KPI & Threshold
- **Availability**: $\frac{\text{uptime}}{\text{total time}}\times 100\%$
- **Cycle time**, **Energy per cycle**
- Threshold untuk alarm otomatis.

### 2.4 Alert & Alarm
Alarm akan aktif saat metric melewati batas (misal: suhu motor > 80°C).

### 2.5 Retention & Downsampling
- **Retention policy**: hapus data lama.
- **Downsampling**: simpan rata-rata per interval untuk data jangka panjang.

---

## 3. RINGKASAN
- Observability penting untuk monitoring dan maintenance.
- Time-series DB + Grafana = standar industri.
- KPI & alarm mempercepat deteksi masalah.
