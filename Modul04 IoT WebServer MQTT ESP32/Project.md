# PROJECT MODUL 04: SISTEM MONITORING DAN KONTROL ROBOT IoT

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – IoT WebServer MQTT ESP32  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. DESKRIPSI PROJECT

### Soal Cerita

> Sebuah perusahaan greenhouse pertanian pintar menugaskan tim Anda untuk membangun **sistem monitoring dan kontrol IoT** berbasis ESP32. Greenhouse memiliki beberapa sensor (suhu, kelembaban, cahaya) dan aktuator (kipas, lampu grow light, pompa irigasi — disimulasikan dengan LED dan buzzer).
>
> Manajer greenhouse ingin bisa **memantau kondisi tanaman dari kantor** melalui browser, sekaligus menerima **notifikasi otomatis via MQTT** ketika suhu terlalu tinggi atau kelembaban terlalu rendah. Selain itu, ia ingin bisa **mengontrol aktuator secara manual** dari dashboard web maupun dari aplikasi MQTT di smartphone.
>
> Sistem harus memiliki **mode otomatis** — aktuator aktif berdasarkan threshold sensor — dan **mode manual** di mana operator bisa override kontrol dari dashboard. Semua data harus di-log dan ditampilkan dalam **grafik real-time** di dashboard HTML.

---

## B. SOAL PROJECT

### Soal 1 — Multi-Sensor Data Acquisition

> Greenhouse menggunakan 3 sensor: DHT22 (suhu & kelembaban), LDR (cahaya), dan potensiometer (simulasi sensor kelembaban tanah). ESP32 harus membaca ketiga sensor dan mengirim data ke:
> 1. MQTT broker dengan topic terpisah per-sensor
> 2. Web Server dengan endpoint JSON `/api/sensor`
>
> Data dikirim setiap 2 detik. Format MQTT topic: `greenhouse/sensor/suhu`, `greenhouse/sensor/kelembaban`, `greenhouse/sensor/cahaya`, `greenhouse/sensor/tanah`.
>
> **Output:** Kode program, screenshot data di MQTT Explorer dan browser.

### Soal 2 — Dashboard Web dengan Grafik Real-Time

> Manajer ingin dashboard yang **tidak hanya angka**, tapi juga **grafik garis real-time** yang menunjukkan tren data sensor 60 detik terakhir. Gunakan library JavaScript ringan (Chart.js yang di-embed atau canvas manual) untuk membuat grafik.
>
> Dashboard harus menampilkan:
> - 4 kartu sensor dengan nilai terkini
> - 1 grafik garis dengan 4 data series (suhu, kelembaban, cahaya, kelembaban tanah)
> - Tombol kontrol untuk 3 aktuator (kipas, lampu, pompa)
> - Indikator mode (otomatis/manual)
>
> **Output:** File HTML dashboard, screenshot dashboard berjalan.

### Soal 3 — Sistem Kontrol Otomatis dan Manual

> Sistem harus memiliki **2 mode**:
> - **Otomatis:** Kipas nyala jika suhu > 30°C, lampu nyala jika cahaya < 500 (ADC), pompa nyala jika kelembaban tanah < 40%.
> - **Manual:** Operator bisa override dari dashboard, semua threshold diabaikan.
>
> Mode dipilih via tombol di dashboard dan juga bisa diubah via MQTT topic `greenhouse/mode` (payload: `auto` atau `manual`).
>
> **Output:** Kode program mode otomatis/manual, screenshot kedua mode berjalan.

### Soal 4 — Sistem Alert dan Notifikasi MQTT

> Ketika sensor melebihi threshold kritis (suhu > 35°C ATAU kelembaban < 30%), ESP32 harus:
> 1. Publish pesan alert ke topic `greenhouse/alert` dengan format JSON
> 2. Nyalakan buzzer selama 3 detik
> 3. Ubah warna kartu sensor di dashboard menjadi merah
>
> Alert hanya dikirim **sekali** per event (bukan setiap 2 detik). Ketika kondisi kembali normal, kirim pesan `greenhouse/alert` dengan status `resolved`.
>
> **Output:** Kode program alert, screenshot alert di MQTT Explorer, screenshot dashboard dengan kartu merah.

### Soal 5 — Integrasi Lengkap dan Stress Test

> Gabungkan semua fitur: multi-sensor, dashboard grafik, kontrol otomatis/manual, dan alert. Lakukan stress test:
> 1. Buka dashboard di 3 browser berbeda secara bersamaan
> 2. Subscribe via MQTT Explorer sekaligus
> 3. Jalankan selama 5 menit tanpa henti
>
> Dokumentasikan: apakah sistem stabil? Apakah ada data loss? Berapa rata-rata latency?
>
> **Output:** Kode program final, video demonstrasi 5 menit, laporan stress test.

---

## C. DELIVERABLES

| No | Deliverable | Format |
|----|------------|--------|
| 1 | Kode program final (semua fitur) | .ino / .cpp |
| 2 | File dashboard HTML | .html |
| 3 | Screenshot dashboard + grafik | PNG |
| 4 | Screenshot MQTT Explorer | PNG |
| 5 | Video demonstrasi 5 menit | MP4 |
| 6 | Laporan stress test | PDF |
| 7 | Diagram arsitektur sistem | PNG/PDF |

---

## D. RUBRIK PENILAIAN PROJECT (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | Multi-sensor data acquisition (MQTT + WebServer) | 20 |
| 2 | Dashboard HTML dengan grafik real-time | 20 |
| 3 | Mode otomatis dan manual berfungsi | 20 |
| 4 | Sistem alert dan notifikasi | 15 |
| 5 | Integrasi lengkap dan stabilitas | 15 |
| 6 | Dokumentasi dan kerapian kode | 10 |
| **TOTAL** | | **100** |

---

## E. REFERENSI

1. OASIS MQTT Specification: http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html
2. PubSubClient: https://pubsubclient.knolleary.net/
3. ESP32 Arduino Core: https://docs.espressif.com/projects/arduino-esp32/en/latest/
4. Chart.js: https://www.chartjs.org/
5. Mosquitto: https://mosquitto.org/
