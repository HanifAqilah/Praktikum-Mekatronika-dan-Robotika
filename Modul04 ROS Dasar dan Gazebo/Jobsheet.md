# JOBSHEET MODUL 04: ROS 2 DASAR DAN GAZEBO

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – ROS Dasar dan Gazebo  
**Pertemuan:** 7–8 (2 × 2 SKS)  
**Tanggal:** ___________________  
**Nama Kelompok:** ___________________  
**Anggota:**

| No | Nama | NIM |
|----|------|-----|
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

1. Memahami konsep ROS 2 Humble (node, topic, service, launch)
2. Mengenal Gazebo Classic sebagai simulator robot
3. Menjalankan dan memahami world SDF — from empty world hingga maze
4. Membuat dan menampilkan model robot URDF di RViz2
5. Men-spawn robot differential drive di Gazebo
6. Mengoperasikan sensor virtual (LIDAR dan kamera)
7. Menggerakkan robot dengan teleop via `ros2_control`
8. Membuat peta lingkungan dengan SLAM Toolbox
9. Menavigasikan robot secara otonom dengan Nav2
10. Mensimulasikan robot lengan 3-DOF dan multi-robot
11. Memahami custom Gazebo world plugin berbasis `rclcpp`

---

## B. ALAT DAN BAHAN

| No | Item | Versi / Keterangan |
|----|------|--------------------|
| 1 | Laptop / PC Ubuntu | 22.04 LTS, RAM ≥ 8 GB |
| 2 | ROS 2 Humble | Sudah terinstall |
| 3 | Gazebo Classic | `ros-humble-gazebo-ros-pkgs` |
| 4 | Package `gazebo_praktikum` | `ros_ws/src/gazebo_praktikum` |
| 5 | VS Code + ROS extension | Opsional |

### Persiapan awal (sekali saja)

```bash
# Install dependensi yang belum ada
sudo apt update && sudo apt install -y \
  ros-humble-gazebo-ros-pkgs \
  ros-humble-gazebo-ros2-control \
  ros-humble-ros2-control \
  ros-humble-ros2-controllers \
  ros-humble-slam-toolbox \
  ros-humble-nav2-bringup \
  ros-humble-teleop-twist-keyboard \
  ros-humble-joint-state-publisher-gui \
  ros-humble-rviz2 xacro

# Masuk ke workspace (gunakan tanda kutip karena path mengandung spasi)
cd "$HOME/Documents/Praktikum-Mekatronika-dan-Robotika/Modul04 ROS Dasar dan Gazebo/ros_ws"

# Source ROS 2 utama
source /opt/ros/humble/setup.bash

# Build workspace (hanya pertama kali atau setelah mengubah kode)
colcon build --packages-select gazebo_praktikum

# Source workspace ros_ws
source install/setup.bash
```

### Source di setiap terminal baru

> **Penting:** Kedua perintah source berikut **wajib** dijalankan di **setiap terminal baru** sebelum menggunakan `ros2 launch`:

```bash
# 1. Source ROS 2 utama
source /opt/ros/humble/setup.bash

# 2. Masuk ke workspace lalu source
cd "$HOME/Documents/Praktikum-Mekatronika-dan-Robotika/Modul04 ROS Dasar dan Gazebo/ros_ws"
source install/setup.bash
```

> **Tips:** Tambahkan kedua baris source di atas ke file `~/.bashrc` agar otomatis tersedia di setiap terminal:
> ```bash
> echo 'source /opt/ros/humble/setup.bash' >> ~/.bashrc
> echo 'source "$HOME/Documents/Praktikum-Mekatronika-dan-Robotika/Modul04 ROS Dasar dan Gazebo/ros_ws/install/setup.bash"' >> ~/.bashrc
> ```

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1 – World Gazebo Kosong

**Estimasi waktu:** 20 menit  
**Tujuan:** Mengenal antarmuka Gazebo

```bash
ros2 launch gazebo_praktikum percobaan1_empty_world.launch.py
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Tombol apa yang digunakan untuk zoom kamera Gazebo? | |
| Di panel World, apa saja properti yang bisa diubah? | |
| Berapa nilai gravitasi default (m/s²)? | |
| Node apa yang aktif? (`ros2 node list`) | |

**Checkpoint ✅:** Gazebo terbuka dengan world kosong, ada ground plane dan matahari.

---

### PERCOBAAN 2 – Objek Primitif SDF

**Estimasi waktu:** 20 menit  
**Tujuan:** Mengenal format SDF dan objek statis

```bash
ros2 launch gazebo_praktikum percobaan2_shapes.launch.py
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Berapa objek yang muncul? Sebutkan warnanya | |
| Buka file `worlds/percobaan2_shapes_world.world` — apa tag SDF untuk mengatur warna objek? | |
| Apa perbedaan `<collision>` dan `<visual>` dalam SDF? | |

**Eksperimen tambahan:** Buka file world, ubah ukuran salah satu objek, jalankan ulang. Catat perbedaannya.

**Checkpoint ✅:** World menampilkan kotak merah, silinder biru, bola hijau, dan dinding.

---

### PERCOBAAN 3 – Visualisasi URDF di RViz2

**Estimasi waktu:** 25 menit  
**Tujuan:** Memahami URDF dan melihatnya di RViz2

```bash
ros2 launch gazebo_praktikum percobaan3_urdf_rviz.launch.py
```

Di RViz2, pastikan:
- Fixed Frame: `base_link`
- Tambahkan display: **RobotModel** (topic `/robot_description`)
- Tambahkan display: **TF**
- Gerakkan slider di `joint_state_publisher_gui`

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Berapa link yang dimiliki `robot_sederhana.urdf`? | |
| Joint tipe apa yang digunakan untuk roda? | |
| Apa fungsi `base_footprint`? | |
| Topik apa yang dipublikasikan `robot_state_publisher`? | |

**Checkpoint ✅:** Robot tervisualisasi di RViz2, slider bisa menggerakkan joint roda.

---

### PERCOBAAN 4 – Spawn Robot di Gazebo

**Estimasi waktu:** 25 menit  
**Tujuan:** Meletakkan robot URDF ke dalam simulasi Gazebo

```bash
ros2 launch gazebo_praktikum percobaan4_spawn_robot.launch.py
```

Buka RViz2 dan Gazebo, amati robot di kedua tampilan.

**Monitor topik:**
```bash
ros2 topic list
ros2 topic echo /joint_states
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Service apa yang digunakan untuk memasukkan robot ke Gazebo? | |
| Topik apa yang dishare antara Gazebo dan RViz2? | |
| Apa yang terjadi jika `robot_state_publisher` tidak dijalankan? | |

**Checkpoint ✅:** Robot muncul di Gazebo dan RViz2 secara bersamaan.

---

### PERCOBAAN 5 – Robot dengan Sensor

**Estimasi waktu:** 30 menit  
**Tujuan:** Menggunakan LIDAR dan kamera virtual di Gazebo

```bash
ros2 launch gazebo_praktikum percobaan5_sensor.launch.py
```

Di RViz2:
- Tambahkan **LaserScan** → topic `/scan`
- Tambahkan **Image** → topic `/camera/image_raw`
- Set Fixed Frame ke `odom`

**Monitor sensor:**
```bash
ros2 topic echo /scan --once
ros2 topic hz /camera/image_raw
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Berapa Hz data `/scan` dipublikasikan? | |
| Berapa Hz data kamera? | |
| Plugin Gazebo apa yang digunakan untuk LIDAR? | |
| Field range apa yang ada di `sensor_msgs/LaserScan`? | |

**Checkpoint ✅:** LaserScan terlihat di RViz2, kamera menampilkan gambar dunia Gazebo.

---

### PERCOBAAN 6 – Teleop dengan ros2_control

**Estimasi waktu:** 30 menit  
**Tujuan:** Menggerakkan robot dengan keyboard via `ros2_control`

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan6_teleop.launch.py

# Terminal 2 (setelah controller aktif ~5 detik)
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

**Cek status controller:**
```bash
ros2 control list_controllers
ros2 control list_hardware_interfaces
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Controller apa saja yang aktif? | |
| Topik publish ke mana untuk menggerakkan robot? | |
| Topik `/odom` berisi informasi apa? | |
| Apa kecepatan maksimum yang bisa dicapai? | |

**Eksperimen:** Gerakkan robot melingkar (tahan `i` + `j`), rekam nilai `/odom`.

**Checkpoint ✅:** Robot bergerak mengikuti perintah keyboard, odometri terupdate di RViz2.

---

### PERCOBAAN 7A – SLAM (Pemetaan Otomatis)

**Estimasi waktu:** 35 menit  
**Tujuan:** Membuat peta labirin menggunakan SLAM Toolbox

```bash
# Terminal 1 – launch simulasi + SLAM
ros2 launch gazebo_praktikum percobaan7a_slam.launch.py

# Terminal 2 – gerakkan robot untuk memetakan
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

Di RViz2 tambahkan **Map** → topic `/map`.

Petakan seluruh labirin, kemudian simpan:
```bash
ros2 run nav2_map_server map_saver_cli \
  -f ~/ros_ws/src/gazebo_praktikum/maps/peta_labirin
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Node apa yang menjalankan SLAM? | |
| Format file peta yang disimpan (.pgm + .yaml) — apa isi file .yaml? | |
| Warna piksel apa yang mewakili: dinding / ruang bebas / unknown? | |
| Seberapa akurat peta dibanding world Gazebo? | |

**Checkpoint ✅:** Peta labirin tersimpan di folder `maps/`.

---

### PERCOBAAN 7B – Navigasi Otonom Nav2

**Estimasi waktu:** 30 menit  
**Tujuan:** Robot bernavigasi otonom menggunakan peta yang dibuat

> Pastikan Percobaan 7A sudah selesai dan peta tersimpan.

```bash
ros2 launch gazebo_praktikum percobaan7b_navigasi.launch.py \
  map:=$(ros2 pkg prefix gazebo_praktikum)/share/gazebo_praktikum/../../../src/gazebo_praktikum/maps/peta_labirin.yaml
```

Di RViz2:
1. Klik **"2D Pose Estimate"** → klik posisi awal robot di peta
2. Tunggu AMCL konvergen (partikel menyatu)
3. Klik **"2D Goal Pose"** → klik tujuan navigasi

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apa itu AMCL dan fungsinya dalam navigasi? | |
| Apa perbedaan global costmap dan local costmap? | |
| Node apa yang merencanakan jalur? | |
| Apa yang terjadi jika tujuan berada di dalam dinding? | |

**Checkpoint ✅:** Robot bergerak otonom menuju tujuan yang ditentukan.

---

### PERCOBAAN 8 – Robot Lengan Manipulator

**Estimasi waktu:** 30 menit  
**Tujuan:** Mengontrol robot lengan 3-DOF via ros2_control

```bash
ros2 launch gazebo_praktikum percobaan8_manipulator.launch.py
```

Script `demo_manipulator.py` otomatis berjalan dan mendemonstrasikan gerakan.

**Kontrol manual:**
```bash
ros2 topic pub /manipulator/joint_1_position_controller/commands \
  std_msgs/msg/Float64MultiArray "data: [1.57]"
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Berapa DOF (Degrees of Freedom) manipulator ini? | |
| Tipe ros2_control hardware interface apa yang digunakan? | |
| Apa gerakan yang terjadi saat nilai joint_2 bertambah positif? | |
| Rentang nilai (radian) yang valid untuk joint_1? | |

**Checkpoint ✅:** Lengan robot bergerak sesuai perintah, tervisualisasi di RViz2.

---

### PERCOBAAN 9 – Multi Robot

**Estimasi waktu:** 25 menit  
**Tujuan:** Menjalankan dua robot sekaligus dengan namespace berbeda

```bash
ros2 launch gazebo_praktikum percobaan9_multi_robot.launch.py
```

**Kontrol individual:**
```bash
# Robot pertama
ros2 topic pub /robot1/cmd_vel geometry_msgs/msg/Twist \
  "{linear: {x: 0.3}, angular: {z: 0.5}}"

# Robot kedua
ros2 topic pub /robot2/cmd_vel geometry_msgs/msg/Twist \
  "{linear: {x: 0.3}, angular: {z: -0.5}}"
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Bagaimana namespace memisahkan topik kedua robot? | |
| Topik apa saja yang dipublikasikan `robot1`? | |
| Apa itu `frame_prefix` pada `robot_state_publisher`? | |
| Apakah dua robot bisa tabrakan di Gazebo? | |

**Checkpoint ✅:** Dua robot berjalan mandiri, dapat dikontrol secara terpisah.

---

### PERCOBAAN 10 – Advanced: World Plugin Kustom

**Estimasi waktu:** 30 menit  
**Tujuan:** Memahami Gazebo world plugin berbasis rclcpp dan spawn dinamis

```bash
ros2 launch gazebo_praktikum percobaan10_advanced.launch.py
```

Monitor output plugin:
```bash
# Terminal lain
ros2 topic echo /plugin/sim_time
ros2 topic echo /plugin/status
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apa yang dipublikasikan topik `/plugin/sim_time`? | |
| Berapa objek kotak yang di-spawn oleh `advanced_demo.py`? | |
| Bagaimana cara `gazebo_ros::Node::Get()` bekerja? | |
| Di mana lokasi file `.so` plugin setelah di-build? | |

**Checkpoint ✅:** Plugin aktif, status tercetak di terminal, kotak dinamis terlihat di Gazebo.

---

## D. TABEL RINGKASAN PERCOBAAN

| No | Percobaan | Launch Command | Output Utama |
|----|-----------|---------------|--------------|
| 1 | World Kosong | `percobaan1_empty_world.launch.py` | Gazebo terbuka |
| 2 | Objek Primitif | `percobaan2_shapes.launch.py` | Box, cylinder, sphere |
| 3 | URDF di RViz2 | `percobaan3_urdf_rviz.launch.py` | Model + TF |
| 4 | Spawn Robot | `percobaan4_spawn_robot.launch.py` | Robot di Gazebo |
| 5 | Sensor | `percobaan5_sensor.launch.py` | `/scan`, `/camera/image_raw` |
| 6 | Teleop | `percobaan6_teleop.launch.py` | Robot gerak keyboard |
| 7A | SLAM | `percobaan7a_slam.launch.py` | Peta `.pgm` + `.yaml` |
| 7B | Nav2 | `percobaan7b_navigasi.launch.py` | Navigasi otonom |
| 8 | Manipulator | `percobaan8_manipulator.launch.py` | Lengan 3-DOF gerak |
| 9 | Multi Robot | `percobaan9_multi_robot.launch.py` | 2 robot terpisah |
| 10 | Advanced | `percobaan10_advanced.launch.py` | Plugin + spawn dinamis |

---

## E. ANALISA

1. Jelaskan alur data dari `teleop_twist_keyboard` hingga roda robot bergerak di Gazebo (Percobaan 6).
2. Apa perbedaan mendasar antara URDF sederhana (Percobaan 3) dan URDF dengan Gazebo plugin (Percobaan 5)?
3. Mengapa SLAM Toolbox membutuhkan robot bergerak untuk membuat peta yang lengkap?
4. Apa peran `controller_manager` dalam ros2_control?
5. Jelaskan mengapa namespace penting saat menjalankan multi-robot (Percobaan 9).

---

## F. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## G. LAMPIRAN WAJIB

- [ ] Screenshot setiap percobaan (10 foto)
- [ ] Peta hasil SLAM (file `.pgm` dan `.yaml`)
- [ ] Rekaman navigasi otonom (foto atau video singkat)
- [ ] Output `ros2 control list_controllers` (Percobaan 6 dan 8)
- [ ] Kode `my_world_plugin.cpp` yang sudah dipahami (annotasi)

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
