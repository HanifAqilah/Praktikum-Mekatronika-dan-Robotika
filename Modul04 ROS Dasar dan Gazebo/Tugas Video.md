# TUGAS VIDEO MODUL 04: ROS 2 DASAR DAN GAZEBO

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – ROS Dasar dan Gazebo  
**Sifat Tugas:** Individual  
**Durasi Video:** 18–30 menit  
**Format:** MP4 (minimal 720p)

---

## A. KETENTUAN UMUM

1. Video **per orang**, bukan per kelompok.
2. **Nama dan NIM** wajib ditampilkan di awal video.
3. **Screen recording** wajib mencakup terminal, Gazebo, RViz2.
4. **Webcam** terlihat di sudut layar saat menjelaskan konsep.
5. Seluruh demo menggunakan package `gazebo_praktikum` yang telah di-build.

---

## B. URUTAN WAJIB ISI VIDEO

### BAGIAN 1 — PENJELASAN KONSEP (Maks. 4 menit)

- [ ] Jelaskan arsitektur ROS 2: node, topic, service, TF
- [ ] Jelaskan URDF / XACRO dan fungsinya dalam simulasi
- [ ] Jelaskan arsitektur `ros2_control` (controller_manager, controller, hardware interface)
- [ ] Jelaskan SLAM Toolbox dan Nav2 secara singkat

### BAGIAN 2 — DEMO 10 PERCOBAAN (Maks. 14 menit)

Jalankan setiap percobaan menggunakan `ros2 launch gazebo_praktikum <launch_file>`:

- [ ] **Percobaan 1:** World Gazebo kosong — tampilkan panel dan kamera Gazebo
- [ ] **Percobaan 2:** Objek SDF primitif — identifikasi setiap objek
- [ ] **Percobaan 3:** URDF di RViz2 — gerakkan slider joint
- [ ] **Percobaan 4:** Spawn robot di Gazebo + RViz2
- [ ] **Percobaan 5:** Sensor LIDAR dan kamera — tampilkan di RViz2 (LaserScan + Image)
- [ ] **Percobaan 6:** Teleop robot dengan keyboard — tampilkan odometri
- [ ] **Percobaan 7A:** SLAM — gerakkan robot + tampilkan peta yang terbentuk
- [ ] **Percobaan 7B:** Navigasi Nav2 — set goal dan robot bergerak otonom
- [ ] **Percobaan 8:** Manipulator 3-DOF — demo_manipulator.py berjalan
- [ ] **Percobaan 9:** Multi-robot — kontrol robot1 dan robot2 secara terpisah
- [ ] **Percobaan 10:** Advanced plugin — tampilkan `/plugin/sim_time` dan spawn kotak

### BAGIAN 3 — DEMO PROJECT (Maks. 8 menit)

- [ ] World Gazebo custom yang dibuat kelompok
- [ ] URDF robot custom dengan LIDAR + kamera
- [ ] Launch file terintegrasi berjalan satu perintah
- [ ] Proses SLAM — peta terbentuk dari awal hingga lengkap
- [ ] Navigasi Nav2 ke minimal 2 goal pose berbeda

### BAGIAN 4 — PENJELASAN KODE (Maks. 3 menit)

- [ ] Jelaskan isi file `urdf/robot_lengkap.urdf.xacro` (fokus pada plugin Gazebo)
- [ ] Jelaskan isi file `config/nav2_params.yaml` (minimal: AMCL + planner)
- [ ] Jelaskan bagaimana `my_world_plugin.cpp` mempublikasikan topik ke ROS 2

### BAGIAN 5 — PENUTUP (Maks. 2 menit)

- [ ] Ringkasan: apa yang dipelajari dari modul ini
- [ ] Kendala yang ditemui dan cara mengatasinya
- [ ] Saran pengembangan simulasi selanjutnya

---

## C. RUBRIK PENILAIAN VIDEO (100 POIN)

| No | Komponen | Bobot | Indikator |
|----|---------|-------|-----------|
| 1 | Penjelasan konsep ROS 2 + ros2_control + Nav2 | 20 | Akurat, tidak membaca script |
| 2 | Demo 10 percobaan (paket gazebo_praktikum) | 35 | Semua percobaan jalan, dijelaskan |
| 3 | Demo project kelompok | 25 | Custom world + robot + SLAM + Nav2 |
| 4 | Penjelasan kode (URDF, nav2_params, plugin) | 10 | Pemahaman kode |
| 5 | Kualitas produksi (audio, screen, webcam, durasi) | 10 | Jelas, dalam durasi |
| **TOTAL** | | **100** | |

---

## D. CHECKLIST SEBELUM SUBMIT

- [ ] Nama dan NIM terlihat di awal video
- [ ] Screen recording aktif sepanjang video
- [ ] Webcam terlihat di sudut layar
- [ ] Seluruh 10 percobaan didemonstrasikan
- [ ] Demo project kelompok (world custom + SLAM + Nav2)
- [ ] Penjelasan kode minimal 3 file
- [ ] Durasi 18–30 menit, resolusi ≥ 720p

**Penamaan file:** `Video_Modul04_[NIM]_[NamaLengkap].mp4`

---

## E. TIPS TEKNIS REKAMAN

```bash
# Source workspace sebelum merekam
source /opt/ros/humble/setup.bash
source ~/ros_ws/install/setup.bash

# Cek semua launch file tersedia
ros2 pkg prefix gazebo_praktikum

# Buka 3–4 terminal dengan split (Terminator / Tilix)
# Terminal 1: launch command
# Terminal 2: teleop / monitoring
# Terminal 3: ros2 topic list / ros2 control list_controllers
```

Mulai rekam dengan `obs-studio` atau `SimpleScreenRecorder`:
```bash
sudo apt install simplescreenrecorder
simplescreenrecorder
```

