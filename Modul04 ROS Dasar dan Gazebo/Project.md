# PROJECT MODUL 04: SIMULASI ROBOT LENGKAP DENGAN ROS 2 DAN GAZEBO

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – ROS Dasar dan Gazebo  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Sebuah perusahaan startup robotika mendapat proyek membuat **robot delivery indoor** untuk rumah sakit. Sebelum membangun prototipe fisik, klien meminta **simulasi lengkap** di Gazebo mencakup:
> - Robot navigasi otonom dengan sensor LIDAR dan kamera
> - Robot lengan untuk membantu pengambilan barang (opsional/bonus)
> - Environment berupa koridor dengan rintangan
>
> Tim kalian bertugas mengembangkan simulasi ROS 2 Humble menggunakan package `gazebo_praktikum` sebagai referensi, lalu membuat **versi custom** sendiri dengan world dan robot yang berbeda.

---

## B. SOAL PROJECT

### Soal 1 — World Gazebo Custom

> Buat world Gazebo yang merepresentasikan satu lantai rumah sakit (bukan labirin default):
> - Minimal 3 ruangan/koridor yang saling terhubung
> - Minimal 6 objek SDF statis (meja, kursi, tempat sampah, tempat tidur)
> - Pencahayaan realistis (minimal 1 directional light)
>
> **Referensi:** `worlds/percobaan7_navigasi_world.world` (struktur SDF)  
> **Output:** File `.world`, screenshot Gazebo dari sudut isometrik.

### Soal 2 — URDF Robot Custom (Differential Drive + Sensor)

> Buat robot URDF/XACRO dengan:
> - Chassis yang bukan kotak polos (gunakan kombinasi paling tidak 2 primitif)
> - 2 roda + 1 caster
> - **LIDAR** 360° → topik `/scan`
> - **Kamera RGB** → topik `/camera/image_raw`
> - Semua plugin dalam format **ROS 2** (`libgazebo_ros_ray_sensor.so`, `libgazebo_ros_camera.so`, `libgazebo_ros_diff_drive.so`)
>
> **Referensi:** `urdf/robot_lengkap.urdf.xacro`  
> **Output:** File URDF/XACRO, screenshot robot di RViz2 (4 sudut pandang).

### Soal 3 — Launch File Terintegrasi

> Buat satu launch file `delivery_robot.launch.py` yang menjalankan:
> - Gazebo dengan world custom
> - `robot_state_publisher`
> - Spawn robot di posisi awal
> - `ros2_control` dengan diff drive controller
> - RViz2 (dengan konfigurasi LaserScan + Camera + TF)
>
> **Referensi:** `launch/percobaan6_teleop.launch.py`  
> **Output:** File launch, demo `ros2 launch ... delivery_robot.launch.py` langsung jalan.

### Soal 4 — SLAM dan Peta

> - Jalankan robot di world custom, gerakkan dengan teleop untuk memetakan seluruh area
> - Simpan peta ke `maps/peta_rumahsakit.pgm` + `maps/peta_rumahsakit.yaml`
> - Verifikasi peta mencakup semua area (bandingkan dengan layout world)
>
> **Referensi:** `percobaan7a_slam.launch.py` + `config/slam_params.yaml`  
> **Output:** File peta + screenshot perbandingan peta vs world.

### Soal 5 — Navigasi Otonom Nav2

> - Gunakan peta dari Soal 4 untuk navigasi otonom
> - Buat script Python yang mengirimkan 3 goal pose berbeda secara berurutan menggunakan Nav2 action server
> - Robot harus berhasil mencapai semua goal tanpa tabrakan
>
> **Referensi:** `config/nav2_params.yaml`, `percobaan7b_navigasi.launch.py`  
> **Output:** Script Python, video/screenshot navigasi berhasil.

### Soal 6 (Bonus) — Obstacle Avoidance Node

> Buat node Python yang membaca `/scan` dan secara reaktif:
> - Jika jarak depan < 0.4 m: berhenti + mundur 0.5 detik + putar 90°
> - Terus maju jika jalur bebas
>
> **Output:** File Python, video demonstrasi.

---

## C. RUBRIK PENILAIAN (100 + 15 POIN BONUS)

| No | Kriteria | Bobot | Indikator |
|----|---------|-------|-----------|
| 1 | World Gazebo custom | 20 | Min. 3 ruangan, 6 objek, pencahayaan |
| 2 | URDF robot custom + sensor | 25 | Plugin ROS 2, LIDAR + kamera berfungsi |
| 3 | Launch file terintegrasi | 15 | Satu perintah, semua komponen nyala |
| 4 | SLAM + peta tersimpan | 20 | Peta akurat, file .pgm + .yaml ada |
| 5 | Navigasi 3 waypoint | 20 | Semua waypoint tercapai tanpa collision |
| 6 | Obstacle avoidance (bonus) | 15 | Menghindar secara reaktif |
| **TOTAL** | | **100 + 15** | |

---

## D. FORMAT PENGUMPULAN

```
NamaKelompok_Modul04/
├── worlds/
│   └── world_custom.world
├── urdf/
│   └── robot_custom.urdf.xacro
├── launch/
│   └── delivery_robot.launch.py
├── maps/
│   ├── peta_rumahsakit.pgm
│   └── peta_rumahsakit.yaml
├── scripts/
│   ├── navigate_waypoints.py
│   └── obstacle_avoidance.py   (bonus)
└── README.md                   (cara menjalankan)
```

---

## E. REFERENSI

1. Package `gazebo_praktikum` (di workspace ini) – seluruh percobaan 1–20
2. ROS 2 Humble Docs: https://docs.ros.org/en/humble/
3. Nav2 Docs: https://navigation.ros.org/
4. URDF Tutorial: https://docs.ros.org/en/humble/Tutorials/Intermediate/URDF/URDF-Main.html
5. slam_toolbox: https://github.com/SteveMacenski/slam_toolbox
6. TurtleBot3 e-Manual: https://emanual.robotis.com/docs/en/platform/turtlebot3/overview/
