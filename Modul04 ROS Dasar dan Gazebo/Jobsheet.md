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
10. Mensimulasikan multi-robot dengan namespace berbeda
11. Mengontrol robot lengan manipulator 3-DOF (visualisasi dan ros2_control)
12. Memahami konsep Forward dan Inverse Kinematics
13. Menggunakan robot standar TurtleBot3 (teleop, SLAM, navigasi)
14. Mensimulasikan robot omnidirectional (omni wheel)
15. Mensimulasikan robot mecanum wheel
16. Membandingkan performa navigasi berbagai tipe robot

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
  ros-humble-rviz2 xacro \
  ros-humble-turtlebot3-gazebo \
  ros-humble-turtlebot3-description \
  ros-humble-turtlebot3-navigation2

# PENTING: Buat symlink tanpa spasi (hanya sekali, satu kali saat pertama kali)
ln -sf "$HOME/Documents/Praktikum-Mekatronika-dan-Robotika/Modul04 ROS Dasar dan Gazebo/ros_ws" ~/ros_praktikum

# Masuk ke workspace via symlink (tanpa spasi - wajib untuk mesh loading di RViz2)
cd ~/ros_praktikum

# Source ROS 2 utama
source /opt/ros/humble/setup.bash

# Build workspace (hanya pertama kali atau setelah mengubah kode)
colcon build --packages-select gazebo_praktikum

# Source workspace via symlink
source ~/ros_praktikum/install/setup.bash
```

> **Catatan:** Menggunakan symlink `~/ros_praktikum` (bukan path langsung) diperlukan karena
> path workspace mengandung spasi ("Modul04 ROS Dasar dan Gazebo") yang menyebabkan
> `resource_retriever` (library RViz2) gagal memuat mesh file STL/DAE via file:// URI.

### Source di setiap terminal baru

> **Penting:** Kedua perintah source berikut **wajib** dijalankan di **setiap terminal baru** sebelum menggunakan `ros2 launch`:

```bash
# 1. Source ROS 2 utama
source /opt/ros/humble/setup.bash

# 2. Source workspace via symlink (wajib menggunakan ~/ros_praktikum bukan path panjang)
source ~/ros_praktikum/install/setup.bash
```

> **Tips:** Tambahkan baris berikut ke `~/.bashrc` agar otomatis tersedia di setiap terminal:
> ```bash
> # Sudah ditambahkan otomatis – cek dengan: tail -10 ~/.bashrc
> # Jika belum ada, jalankan:
> echo 'if [ -f "$HOME/ros_praktikum/install/setup.bash" ]; then source "$HOME/ros_praktikum/install/setup.bash"; fi' >> ~/.bashrc
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

### PERCOBAAN 8 – Multi Robot

**Estimasi waktu:** 25 menit  
**Tujuan:** Menjalankan dua robot sekaligus dengan namespace berbeda

```bash
ros2 launch gazebo_praktikum percobaan8_multi_robot.launch.py
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
| Topik apa saja yang dipublikasikan `robot1`? (`ros2 topic list`) | |
| Apa itu `frame_prefix` pada `robot_state_publisher`? | |
| Apakah dua robot bisa tabrakan di Gazebo? | |

**Checkpoint ✅:** Dua robot berjalan mandiri, dapat dikontrol secara terpisah.

---

### PERCOBAAN 9 – Robot Lengan Manipulator (Visualisasi)

**Estimasi waktu:** 25 menit  
**Tujuan:** Mengenal robot lengan 3-DOF melalui joint_state_publisher_gui

```bash
ros2 launch gazebo_praktikum percobaan9_manipulator.launch.py
```

Di RViz2:
- Fixed Frame: `world`
- Gerakkan slider di `joint_state_publisher_gui` untuk mengontrol setiap joint
- Amati pergerakan TF frame di RViz2

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Berapa DOF (Degrees of Freedom) manipulator ini? | |
| Tipe joint apa yang digunakan untuk setiap joint? | |
| Apa perbedaan joint revolute sumbu Y vs sumbu Z? | |
| Bagaimana TF tree robot arm ini tersusun? (`ros2 run tf2_tools view_frames`) | |

**Checkpoint ✅:** Slider menggerakkan lengan robot di RViz2, TF tree terlihat.

---

### PERCOBAAN 10 – Manipulator dengan ros2_control

**Estimasi waktu:** 30 menit  
**Tujuan:** Mengontrol robot lengan 3-DOF menggunakan framework ros2_control

```bash
ros2 launch gazebo_praktikum percobaan10_manipulator_control.launch.py
```

Script `demo_manipulator.py` **berjalan otomatis** setelah controller aktif (~7 detik).
Demo bergerak halus (interpolasi linear 50 Hz) melalui urutan:
1. Home → Reach depan → Rotasi kiri → Turunkan lengan kiri
2. Rotasi kanan → Turunkan lengan kanan → Kembali Home
3. Gerakan sinusoidal berkelanjutan

Visualisasi gerakan tersedia di **RViz2** (robot arm bergerak sesuai joint path).

**Kontrol manual (opsional, terminal baru):**
```bash
ros2 topic pub /arm_position_controller/commands \
  std_msgs/msg/Float64MultiArray "data: [1.57, 0.5, -1.0]" --once
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Controller apa yang aktif? (`ros2 control list_controllers`) | |
| Tipe hardware interface apa yang digunakan? | |
| Apa perbedaan mock_components vs gazebo_ros2_control? | |
| Rentang nilai (radian) yang valid untuk joint_1? | |

**Checkpoint ✅:** Lengan robot bergerak sesuai perintah, tervisualisasi di RViz2.

---

### PERCOBAAN 11 – Kinematika Robot Manipulator (FK/IK)

**Estimasi waktu:** 30 menit  
**Tujuan:** Memahami Forward Kinematics dan Inverse Kinematics pada robot 3-DOF

```bash
ros2 launch gazebo_praktikum percobaan11_kinematics.launch.py
```

Script `kinematics_demo.py` berjalan otomatis dalam **dua fase**:

**FASE 1 – Forward Kinematics (FK):**
- Arm bergerak ke 6 konfigurasi sudut joint yang berbeda
- Setiap konfigurasi: FK dihitung dan posisi end-effector (x, y, z) ditampilkan di terminal
- Marker hijau muncul di RViz2 menandai posisi end-effector

**FASE 2 – Inverse Kinematics (IK) Smooth:**
- Diberikan 5 target posisi Cartesian
- IK analitik menghitung sudut joint yang diperlukan
- Arm bergerak **halus** (interpolasi 50 Hz, 3 detik per target) ke setiap posisi
- Marker merah (target) dan hijau (posisi EE aktual) ditampilkan di RViz2

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apa itu Forward Kinematics? | |
| Apa itu Inverse Kinematics? | |
| Berapa panjang link 1, 2, dan 3? (Lihat `scripts/kinematics_demo.py`) | |
| Berapa SHOULDER_Z (tinggi bahu dari lantai)? | |
| Apa yang terjadi jika target di luar jangkauan (unreachable)? | |
| Mengapa gerakan IK lebih halus dibanding FK yang diskret? | |

**Checkpoint ✅:** Marker FK (hijau) terlihat di RViz2, arm bergerak halus ke setiap target IK, tidak ada error out-of-range.

---

### PERCOBAAN 12 – TurtleBot3 di Gazebo

**Estimasi waktu:** 25 menit  
**Tujuan:** Menjalankan robot standar TurtleBot3 Waffle di Gazebo

```bash
ros2 launch gazebo_praktikum percobaan12_turtlebot3.launch.py
```

**Di terminal lain (teleop):**
```bash
export TURTLEBOT3_MODEL=waffle
ros2 run teleop_twist_keyboard teleop_twist_keyboard \
  --ros-args --remap /cmd_vel:=/cmd_vel
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Sensor apa saja yang dimiliki TurtleBot3 Waffle? | |
| Apa perbedaan TurtleBot3 Burger vs Waffle? | |
| Topik apa yang dipublikasikan? (`ros2 topic list`) | |
| Berapakah frame rate kamera TurtleBot3? (`ros2 topic hz /camera/image_raw`) | |

**Checkpoint ✅:** TurtleBot3 Waffle tampil di Gazebo, bisa dikontrol teleop.

---

### PERCOBAAN 13 – TurtleBot3 SLAM

**Estimasi waktu:** 30 menit  
**Tujuan:** Membuat peta dengan TurtleBot3 menggunakan SLAM Toolbox

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan13_turtlebot3_slam.launch.py

# Terminal 2 (teleop)
export TURTLEBOT3_MODEL=waffle
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

Keliling untuk memetakan, lalu simpan:
```bash
ros2 run nav2_map_server map_saver_cli -f ~/map_turtlebot3
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apa perbedaan SLAM dengan TurtleBot3 vs robot custom? | |
| Seberapa cepat peta terbentuk? | |
| Apakah kualitas peta TurtleBot3 lebih baik dari robot custom? Mengapa? | |

**Checkpoint ✅:** Peta lingkungan TurtleBot3 World tersimpan.

---

### PERCOBAAN 14 – TurtleBot3 Navigasi Otonom

**Estimasi waktu:** 30 menit  
**Tujuan:** Navigasi otonom TurtleBot3 menggunakan Nav2

> Prasyarat: Peta dari Percobaan 13 sudah tersimpan.

```bash
ros2 launch gazebo_praktikum percobaan14_turtlebot3_nav.launch.py \
  map:=/path/to/map_turtlebot3.yaml
```

Di RViz2:
1. Gunakan **"2D Pose Estimate"** untuk inisialisasi posisi robot
2. Gunakan **"2D Nav Goal"** untuk mengirim tujuan

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apakah TurtleBot3 berhasil menghindari semua rintangan? | |
| Apa perbedaan performa navigasi TurtleBot3 vs robot custom? | |
| Berapa waktu rata-rata TurtleBot3 mencapai target? | |

**Checkpoint ✅:** TurtleBot3 bergerak otonom ke tujuan, menghindari rintangan.

---

### PERCOBAAN 15 – Robot Omni Wheel (Teleop)

**Estimasi waktu:** 25 menit  
**Tujuan:** Mengenal robot omnidirectional 4-roda dan gerakan lateralnya

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan15_omni_teleop.launch.py

# Terminal 2
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

> **Catatan:** Robot omni dapat bergerak ke segala arah — linear X (maju/mundur), linear Y (kiri/kanan), dan angular Z (rotasi).

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apa perbedaan robot diff-drive vs omni-directional? | |
| Tombol apa di teleop untuk gerak lateral (kiri/kanan)? | |
| Plugin Gazebo apa yang digunakan untuk robot omni? | |
| Berapa roda yang dimiliki robot omni ini? | |

**Checkpoint ✅:** Robot omni bergerak di Gazebo termasuk gerak lateral.

---

### PERCOBAAN 16 – Robot Omni SLAM

**Estimasi waktu:** 30 menit  
**Tujuan:** Membuat peta menggunakan robot omnidirectional

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan16_omni_slam.launch.py

# Terminal 2 (teleop)
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

Simpan peta:
```bash
ros2 run nav2_map_server map_saver_cli -f ~/map_omni
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apakah proses SLAM lebih cepat dengan robot omni? | |
| Apa keuntungan gerak lateral saat memetakan? | |
| Bandingkan kualitas peta omni vs diff-drive | |

**Checkpoint ✅:** Peta tersimpan menggunakan robot omni.

---

### PERCOBAAN 17 – Robot Omni Navigasi Otonom

**Estimasi waktu:** 30 menit  
**Tujuan:** Navigasi otonom menggunakan robot omnidirectional

> Prasyarat: Peta dari Percobaan 16 sudah tersimpan.

```bash
ros2 launch gazebo_praktikum percobaan17_omni_nav.launch.py \
  map:=/path/to/map_omni.yaml
```

Di RViz2: Gunakan **"2D Pose Estimate"** lalu **"2D Nav Goal"**.

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apakah robot omni lebih manuver dalam ruang sempit? | |
| Apa keuntungan kinematik omni untuk navigasi? | |
| Bandingkan waktu tempuh omni vs diff-drive ke tujuan yang sama | |

**Checkpoint ✅:** Robot omni bernavigasi otonom ke tujuan.

---

### PERCOBAAN 18 – Robot Mecanum Wheel (Teleop)

**Estimasi waktu:** 25 menit  
**Tujuan:** Mengenal robot mecanum 4-roda dan gerakan holonomic

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan18_mecanum_teleop.launch.py

# Terminal 2
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Apa perbedaan roda mecanum vs roda omni? | |
| Bagaimana desain roller pada roda mecanum (sudut 45°)? | |
| Apakah gerak lateral mecanum sehalus omni? | |
| Plugin Gazebo apa yang digunakan? | |

**Checkpoint ✅:** Robot mecanum bergerak di Gazebo, termasuk gerak lateral.

---

### PERCOBAAN 19 – Robot Mecanum SLAM

**Estimasi waktu:** 30 menit  
**Tujuan:** Membuat peta menggunakan robot mecanum

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan19_mecanum_slam.launch.py

# Terminal 2 (teleop)
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

Simpan peta:
```bash
ros2 run nav2_map_server map_saver_cli -f ~/map_mecanum
```

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Bandingkan proses SLAM: diff-drive vs omni vs mecanum | |
| Mana yang menghasilkan peta paling akurat? Mengapa? | |

**Checkpoint ✅:** Peta tersimpan menggunakan robot mecanum.

---

### PERCOBAAN 20 – Robot Mecanum Navigasi Otonom

**Estimasi waktu:** 30 menit  
**Tujuan:** Navigasi otonom menggunakan robot mecanum

> Prasyarat: Peta dari Percobaan 19 sudah tersimpan.

```bash
ros2 launch gazebo_praktikum percobaan20_mecanum_nav.launch.py \
  map:=/path/to/map_mecanum.yaml
```

Di RViz2: Gunakan **"2D Pose Estimate"** lalu **"2D Nav Goal"**.

**Tugas observasi:**

| Pertanyaan | Jawaban |
|-----------|---------|
| Bandingkan navigasi: diff-drive vs omni vs mecanum | |
| Robot mana yang paling cepat mencapai tujuan? | |
| Robot mana yang paling stabil saat bermanuver? | |
| Dalam skenario apa mecanum lebih unggul dari diff-drive? | |

**Checkpoint ✅:** Robot mecanum bernavigasi otonom ke tujuan.

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
| 8 | Multi Robot | `percobaan8_multi_robot.launch.py` | 2 robot terpisah |
| 9 | Manipulator Visual | `percobaan9_manipulator.launch.py` | Lengan 3-DOF + slider |
| 10 | Manipulator Control | `percobaan10_manipulator_control.launch.py` | ros2_control arm |
| 11 | Kinematika FK/IK | `percobaan11_kinematics.launch.py` | Demo FK + IK marker |
| 12 | TurtleBot3 | `percobaan12_turtlebot3.launch.py` | TB3 Waffle di Gazebo |
| 13 | TurtleBot3 SLAM | `percobaan13_turtlebot3_slam.launch.py` | Peta TB3 World |
| 14 | TurtleBot3 Nav | `percobaan14_turtlebot3_nav.launch.py` | Nav2 + TB3 |
| 15 | Omni Teleop | `percobaan15_omni_teleop.launch.py` | Robot omni bergerak |
| 16 | Omni SLAM | `percobaan16_omni_slam.launch.py` | Peta omni |
| 17 | Omni Nav | `percobaan17_omni_nav.launch.py` | Nav2 + omni |
| 18 | Mecanum Teleop | `percobaan18_mecanum_teleop.launch.py` | Robot mecanum bergerak |
| 19 | Mecanum SLAM | `percobaan19_mecanum_slam.launch.py` | Peta mecanum |
| 20 | Mecanum Nav | `percobaan20_mecanum_nav.launch.py` | Nav2 + mecanum |

---

## E. ANALISA

1. Jelaskan alur data dari `teleop_twist_keyboard` hingga roda robot bergerak di Gazebo (Percobaan 6).
2. Apa perbedaan mendasar antara URDF sederhana (Percobaan 3) dan URDF dengan Gazebo plugin (Percobaan 5)?
3. Mengapa SLAM Toolbox membutuhkan robot bergerak untuk membuat peta yang lengkap?
4. Apa peran `controller_manager` dalam ros2_control? (Percobaan 10)
5. Jelaskan mengapa namespace penting saat menjalankan multi-robot (Percobaan 8).
6. Jelaskan perbedaan Forward Kinematics dan Inverse Kinematics (Percobaan 11).
7. Bandingkan karakteristik gerakan robot differential drive, omnidirectional, dan mecanum.
8. Dalam skenario warehouse/gudang, tipe robot mana yang paling cocok dan mengapa?

---

## F. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## G. LAMPIRAN WAJIB

- [ ] Screenshot setiap percobaan (20 foto minimum)
- [ ] Peta hasil SLAM untuk setiap jenis robot (diff-drive, TB3, omni, mecanum)
- [ ] Rekaman navigasi otonom (foto atau video singkat)
- [ ] Output `ros2 control list_controllers` (Percobaan 6 dan 10)
- [ ] Tabel perbandingan performa navigasi: diff-drive vs omni vs mecanum
- [ ] Diagram TF tree robot manipulator (`ros2 run tf2_tools view_frames`)

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
