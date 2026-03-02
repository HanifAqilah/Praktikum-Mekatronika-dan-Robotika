# PROJECT MODUL 08: CUSTOM ROBOT SIMULATION

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 08 – ROS Dasar dan Gazebo  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Sebuah perusahaan startup robotika mendapat proyek membuat **robot delivery indoor** untuk rumah sakit. Sebelum membangun prototipe fisik, klien meminta **simulasi lengkap** robot di Gazebo dengan environment koridor rumah sakit. Robot harus bisa dikendalikan dari terminal dan menunjukkan data sensor virtual.
>
> Tim kalian ditugaskan membuat **URDF robot custom** (bukan TurtleBot) yang dilengkapi sensor LIDAR dan kamera, lalu disimulasikan di Gazebo dalam custom world berupa koridor dengan rintangan. Robot harus bisa dikendalikan via teleop keyboard dan menampilkan data sensor di RViz2.

---

## B. SOAL PROJECT

### Soal 1 — URDF Robot Custom

> Buat URDF robot dengan spesifikasi:
> - Chassis bentuk custom (bukan box standar — gunakan mesh dari Fusion360 atau bentuk gabungan primitif)
> - 2 roda + 1 caster (differential drive)
> - 1 LIDAR sensor (RPLiDAR style, 360°)
> - 1 kamera RGB (resolusi 640×480)
> - Warna dan visual yang menarik
>
> **Output:** File URDF/XACRO, screenshot robot di RViz2 dari 4 sudut.

### Soal 2 — Gazebo World: Koridor Rumah Sakit

> Buat world Gazebo yang merepresentasikan koridor rumah sakit:
> - Minimal 3 koridor yang saling berhubungan
> - Minimal 5 rintangan statis (meja, kursi, tempat sampah)
> - Tekstur lantai dan dinding
>
> **Output:** File .world, screenshot Gazebo.

### Soal 3 — Sensor Integration

> Konfigurasi sensor Gazebo:
> - LIDAR harus publish ke topic `/scan` (sensor_msgs/LaserScan)
> - Kamera harus publish ke topic `/camera/image_raw` (sensor_msgs/Image)
> - Tampilkan keduanya di RViz2
>
> **Output:** Screenshot RViz2 dengan LaserScan overlay dan Image display.

### Soal 4 — Teleop dan Autonomi Dasar

> - Kontrol robot via `teleop_twist_keyboard`
> - Buat node Python yang membaca `/scan` dan jika ada rintangan di depan < 0.5 m, otomatis berhenti + mundur + belok
>
> **Output:** Video robot bergerak dan menghindari rintangan secara otonom.

### Soal 5 — Launch File Lengkap

> Buat satu launch file yang menjalankan semua komponen:
> - robot_state_publisher
> - Gazebo + world
> - Spawn entity
> - RViz2 dengan config
> - Node autonomi (opsional)
>
> **Output:** File launch, demo satu perintah meluncurkan segalanya.

---

## C. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | URDF robot custom + sensor | 25 |
| 2 | Gazebo world koridor | 15 |
| 3 | Sensor LIDAR + kamera berfungsi | 20 |
| 4 | Teleop + autonomi dasar | 20 |
| 5 | Launch file terintegrasi | 10 |
| 6 | Dokumentasi dan analisis | 10 |
| **TOTAL** | | **100** |

---

## D. REFERENSI

1. ROS 2 Humble Docs: https://docs.ros.org/en/humble/
2. Gazebo Sim Docs: https://gazebosim.org/docs
3. URDF Tutorial: https://docs.ros.org/en/humble/Tutorials/Intermediate/URDF/URDF-Main.html
4. Joseph, L. (2018). *ROS Robotics Projects* (2nd ed). Packt.
