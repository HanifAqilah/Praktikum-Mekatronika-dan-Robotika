# PROJECT MODUL 05: SISTEM SCADA MINI DENGAN PLC, ESP32, DAN AI

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – IoT PLC AI Modbus  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. DESKRIPSI PROJECT

### Soal Cerita

> Sebuah pabrik minuman botol memiliki **lini produksi sederhana** yang terdiri dari: conveyor belt, sensor proximity untuk menghitung botol, sensor suhu untuk memantau motor conveyor, dan lampu indikator status. Semua dikontrol oleh PLC Schneider TM221.
>
> Manajemen pabrik meminta tim Anda membangun **sistem SCADA mini** yang:
> 1. Menampilkan data produksi (jumlah botol, suhu motor, status conveyor) di **dashboard web** yang bisa diakses dari kantor manajer
> 2. Mengirim data produksi ke **cloud via MQTT** agar bisa diakses dari smartphone
> 3. Memberikan **peringatan otomatis** (alert) jika suhu motor melebihi batas aman
> 4. Memiliki **AI sederhana** yang memprediksi kapan motor perlu maintenance berdasarkan pola data suhu
>
> ESP32 berperan sebagai **gateway** antara PLC (Modbus RTU) dan dunia internet (WiFi/MQTT/WebServer). Python di laptop digunakan untuk AI predictive maintenance.

---

## B. SOAL PROJECT

### Soal 1 — Program PLC Lini Produksi

> Program PLC mengimplementasikan logika berikut:
> - Push Button Start (I0.0) → conveyor ON (Q0.0) dengan self-holding
> - Push Button Stop (I0.1) → conveyor OFF
> - Sensor Proximity (I0.2) → hitung botol ke %MW0 (counter)
> - Potensiometer (AI0) → simulasi suhu motor → simpan ke %MW1
> - Jika counter ≥ target produksi (%MW5, diset via Modbus) → Q0.1 (indikator selesai)
> - Jika suhu > threshold (%MW6) → Q0.2 (alarm) dan conveyor OFF otomatis
>
> **Output:** File program PLC (Ladder + ST), screenshot running.

### Soal 2 — ESP32 Gateway Modbus-ke-MQTT

> ESP32 membaca data PLC via Modbus RTU setiap 2 detik, lalu:
> - Publish ke MQTT: `pabrik/produksi/counter`, `pabrik/produksi/suhu`, `pabrik/produksi/status`
> - Subscribe MQTT: `pabrik/kontrol/target` → tulis ke %MW5, `pabrik/kontrol/threshold` → tulis ke %MW6
> - Menjalankan Web Server dengan dashboard status produksi
>
> **Output:** Kode ESP32, screenshot MQTT dan Web Server.

### Soal 3 — Dashboard SCADA Web

> Buat dashboard HTML di ESP32 yang menampilkan:
> - Jumlah botol terproduksi (angka besar + progress bar ke target)
> - Suhu motor (gauge/thermometer visual)
> - Status conveyor (ON/OFF dengan indikator warna)
> - Tombol Start/Stop conveyor via Modbus
> - Input field untuk set target produksi dan threshold suhu
>
> Dashboard auto-refresh setiap 2 detik via fetch API.
>
> **Output:** File HTML dashboard, screenshot dashboard berjalan.

### Soal 4 — AI Predictive Maintenance

> Kumpulkan data suhu motor dari PLC selama 3 menit operasi normal menggunakan Python. Latih model Isolation Forest. Kemudian:
> - Simulasikan anomali (putar potensiometer ke nilai ekstrem)
> - Model harus mendeteksi anomali
> - Tampilkan grafik data suhu dengan titik anomali ditandai merah
>
> **Output:** Script Python, grafik matplotlib, laporan akurasi.

### Soal 5 — Integrasi dan Demo Lengkap

> Jalankan seluruh sistem secara simultan selama 5 menit:
> - PLC menjalankan lini produksi
> - ESP32 sebagai gateway Modbus-MQTT + Web Server
> - Laptop menampilkan MQTT data + menjalankan AI monitoring
> - Dashboard web diakses dari browser
>
> Dokumentasikan alur data dari sensor → PLC → ESP32 → MQTT → Dashboard/AI.
>
> **Output:** Video demo 5 menit, diagram arsitektur lengkap.

---

## C. DELIVERABLES

| No | Deliverable | Format |
|----|------------|--------|
| 1 | Program PLC (Ladder + ST) | .smbp |
| 2 | Kode ESP32 gateway | .ino |
| 3 | File dashboard HTML | .html |
| 4 | Script Python AI | .py |
| 5 | Diagram arsitektur | PNG/PDF |
| 6 | Video demo 5 menit | MP4 |
| 7 | Laporan predictive maintenance | PDF |

---

## D. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | Program PLC logika produksi lengkap | 20 |
| 2 | Gateway ESP32 Modbus-MQTT berjalan | 20 |
| 3 | Dashboard SCADA web fungsional | 20 |
| 4 | AI anomaly detection berhasil | 20 |
| 5 | Integrasi lengkap + demo stabil | 15 |
| 6 | Dokumentasi dan diagram | 5 |
| **TOTAL** | | **100** |

---

## E. REFERENSI

1. Modbus Specification: http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf
2. Schneider PLC Documentation: https://www.schneider-electric.com/
3. pyModbus: https://github.com/pymodbus-dev/pymodbus
4. scikit-learn Isolation Forest: https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.IsolationForest.html
