# JOBSHEET MODUL 14: OBSERVABILITY & DATA ENGINEERING

## 1. TUJUAN PRAKTIKUM
- Setup InfluxDB dan Grafana
- Insert data time-series
- Buat dashboard monitoring
- Definisikan KPI
- Setup alarm & notifikasi

---

## 2. ALAT & BAHAN
- InfluxDB 2.0
- Grafana
- Python untuk ingest data
- SMTP/email untuk alert

---

## 3. LANGKAH KERJA

### PERCOBAAN 1: DATA INSERT
1. Setup bucket InfluxDB.
2. Insert data sensor (temperature, current).

**Deliverable:** Data muncul saat query.

---

### PERCOBAAN 2: QUERY & AGGREGATION
1. Query 24 jam terakhir.
2. Hitung average per jam.

**Deliverable:** Hasil agregasi.

---

### PERCOBAAN 3: DASHBOARD GRAFANA
1. Tambahkan datasource.
2. Buat panel gauge & graph.

**Deliverable:** Dashboard aktif.

---

### PERCOBAAN 4: KPI
1. Hitung availability dan cycle time.
2. Simpan KPI ke DB.

**Deliverable:** KPI tampil di Grafana.

---

### PERCOBAAN 5: ALARM
1. Buat rule suhu motor > 80°C.
2. Kirim notifikasi email.

**Deliverable:** Alarm trigger sukses.

---

## 4. KESIMPULAN
Tuliskan evaluasi monitoring dan manfaat KPI.
