# PROJECT MODUL 09: AUTONOMOUS DELIVERY ROBOT NAVIGATION

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 09 – ROS Kinematika dan Kontroler  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Startup delivery robot mendapat order: robot harus mengantarkan paket ke **5 meja berbeda** di kantor (waypoint). Setelah sampai di setiap meja, robot berhenti 3 detik (simulasi drop-off), lalu lanjut ke meja berikutnya. Setelah semua meja terkirim, robot kembali ke posisi awal (home).
>
> Klien menginginkan robot bergerak **halus** (tidak zigzag), **cepat** (waktu total minimal), dan menampilkan **progress delivery** di terminal (meja ke-n dari 5). Arena: Gazebo world koridor dari Modul 08 + 5 marker meja.

---

## B. SOAL PROJECT

### Soal 1 — Multi-Waypoint Delivery

> Buat node `delivery_robot.py` yang:
> - Menerima list 5 waypoint (koordinat x, y) via parameter
> - Navigasi ke setiap waypoint secara berurutan
> - Berhenti 3 detik di setiap waypoint
> - Kembali ke home setelah semua selesai
> - Tampilkan progress: "Delivering to desk 2/5..."
>
> **Output:** Kode, video robot mengunjungi 5 waypoint.

### Soal 2 — Smooth Trajectory

> Implementasikan **Pure Pursuit** atau **PID tuning ketat** agar robot bergerak halus tanpa zigzag. Bandingkan dua metode: Go-to-Goal biasa vs Pure Pursuit.
>
> **Output:** Video perbandingan, analisis kualitas trajectory (osilasi, overshoot).

### Soal 3 — Speed Optimization

> Implementasikan adaptive speed:
> - Jarak ke waypoint > 1 m → kecepatan tinggi
> - Jarak < 0.5 m → perlambat mendekati waypoint
> - Hitung total waktu delivery dan optimasi parameter
>
> **Output:** Tabel perbandingan waktu (fixed speed vs adaptive), video.

### Soal 4 — Odometry Accuracy

> Jalankan robot menempuh lintasan persegi 2×2 meter (kembali ke titik awal). Bandingkan posisi akhir dari odometry vs posisi awal. Hitung error drift.
>
> **Output:** Data posisi awal vs akhir, screenshot RViz2 trajectory, analisis akurasi odometry.

### Soal 5 — Dynamic Goal (Bonus)

> Buat node yang menerima goal baru via topic `/new_goal` (geometry_msgs/Pose2D). Robot langsung pindah tujuan ke goal baru (preempt saat ini). Demonstrasikan dengan mengirim 3 goal secara berurutan via CLI.
>
> **Output:** Kode, video robot bereaksi terhadap goal baru.

---

## C. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | Multi-waypoint delivery berfungsi | 25 |
| 2 | Smooth trajectory | 20 |
| 3 | Speed optimization + timing | 15 |
| 4 | Odometry accuracy analysis | 20 |
| 5 | Dynamic goal (bonus) | 10 |
| 6 | Dokumentasi dan analisis | 10 |
| **TOTAL** | | **100** |

---

## D. REFERENSI

1. Siegwart, R. et al. *Introduction to Autonomous Mobile Robots*. MIT Press.
2. Siciliano, B. et al. *Robotics: Modelling, Planning and Control*. Springer.
3. ROS 2 Navigation: https://docs.ros.org/en/humble/
