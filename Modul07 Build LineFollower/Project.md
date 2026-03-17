# PROJECT MODUL 06: ROBOT LINE FOLLOWER KOMPETISI

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 06 – Build Line Follower  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Kompetisi robotika kampus mengadakan **lomba line follower** dengan arena sirkuit sepanjang 5 meter yang memiliki berbagai rintangan: garis lurus panjang, tikungan 90°, tikungan U-turn, persimpangan T (harus lurus), dan zona putus garis (gap 5 cm).
>
> Pemenang ditentukan berdasarkan **waktu tercepat menyelesaikan 3 putaran** tanpa keluar garis. Robot yang keluar garis lebih dari 3 detik didiskualifikasi. Setiap kelompok mendapat **3 kesempatan run** — waktu terbaik yang dihitung.
>
> Sponsor kompetisi memberikan tantangan tambahan: robot yang bisa menampilkan **data telemetri secara real-time** (kecepatan, posisi sensor, PID output) di laptop via WiFi mendapat **poin bonus 20%**.

---

## B. SOAL PROJECT

### Soal 1 — PID Tuning Optimal

> Temukan parameter PID $(K_p, K_i, K_d)$ yang menghasilkan waktu tercepat di arena. Buat **tabel perbandingan** minimal 10 set parameter berbeda, lengkap dengan waktu tempuh dan catatan perilaku robot. Para juri ingin melihat proses **engineering systematic**, bukan asal coba.
>
> **Output:** Tabel 10+ percobaan, analisis grafik error vs waktu, parameter final terbaik.

### Soal 2 — Handling Tikungan dan Gap

> Arena memiliki tikungan 90° dan gap garis 5 cm. Implementasikan:
> - **Predictive turn**: sensor terluar mendeteksi tikungan → kurangi kecepatan SEBELUM sampai
> - **Gap handling**: jika semua sensor putih (garis hilang) → lanjut maju lurus selama maks 300 ms, jika masih hilang → search spiral
> - **T-intersection**: jika semua sensor hitam → jalan lurus (ignore intersection)
>
> **Output:** Kode program, video demo melewati tikungan 90° dan gap.

### Soal 3 — Adaptive Speed Control

> Robot harus berjalan **cepat di garis lurus** dan **pelan di tikungan**. Implementasikan:
> - Deteksi garis lurus: error < threshold selama > 500 ms → naikkan baseSpeed
> - Deteksi tikungan: error > threshold → turunkan baseSpeed
> - Transisi kecepatan harus **gradual** (bukan langsung loncat)
>
> **Output:** Kode program, perbandingan waktu tempuh fixed speed vs adaptive speed.

### Soal 4 — Telemetri WiFi Real-Time (Bonus)

> Kirim data telemetri dari robot ke laptop via WiFi/MQTT setiap 50 ms:
> - Posisi sensor (weighted position)
> - Error dan PID output
> - Kecepatan motor kiri dan kanan
> - Timestamp
>
> Di laptop, buat **dashboard Python** (matplotlib live plot) yang menampilkan grafik real-time.
>
> **Output:** Kode ESP32 + Python dashboard, screenshot grafik telemetri.

### Soal 5 — Race Day

> Lakukan 3 run resmi. Dokumentasikan:
> - Video setiap run (full laps)
> - Waktu per lap dan total
> - Parameter PID yang digunakan
> - Analisis: lap mana yang terbaik? Mengapa? Apa yang bisa ditingkatkan?
>
> **Output:** 3 video run, tabel waktu, analisis performa.

---

## C. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | PID tuning systematic (10+ percobaan) | 20 |
| 2 | Handling tikungan 90° dan gap | 20 |
| 3 | Adaptive speed control | 15 |
| 4 | Telemetri WiFi (bonus) | 15 |
| 5 | Race day performance (3 run) | 20 |
| 6 | Dokumentasi dan analisis | 10 |
| **TOTAL** | | **100** |

---

## D. REFERENSI

1. Ang et al. PID Control System Analysis: https://ieeexplore.ieee.org/abstract/document/4587662
2. Arduino PID Library: https://github.com/br3ttb/Arduino-PID-Library
3. Line Follower Design: https://github.com/topics/line-follower
