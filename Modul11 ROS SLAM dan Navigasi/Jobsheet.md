# JOBSHEET MODUL 11: ROS 2 SLAM DAN NAVIGASI

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 11 – ROS SLAM dan Navigasi  
**Pertemuan:** 21–22 (2 × 2 SKS)  
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

1. Menjalankan dan memahami SLAM Toolbox untuk pemetaan 2D
2. Membuat peta lingkungan menggunakan LIDAR + teleop
3. Menyimpan dan memuat peta (map_saver / map_server)
4. Menjalankan Nav2 navigation stack
5. Melakukan autonomous navigation ke goal pose via RViz2
6. Melakukan navigasi via kode Python (nav2_simple_commander)
7. Memahami costmap, localization (AMCL), dan path planning

---

## B. ALAT DAN BAHAN

| No | Item | Keterangan |
|----|------|-----------|
| 1 | Laptop/PC Ubuntu 22.04 | RAM ≥ 8 GB |
| 2 | ROS 2 Humble + Gazebo | Dari Modul 08 |
| 3 | Robot URDF + LIDAR | Dari Modul 08 |
| 4 | SLAM Toolbox | `sudo apt install ros-humble-slam-toolbox` |
| 5 | Nav2 | `sudo apt install ros-humble-navigation2 ros-humble-nav2-bringup` |
| 6 | Teleop | `sudo apt install ros-humble-teleop-twist-keyboard` |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: VERIFIKASI LIDAR DI GAZEBO

**Estimasi waktu: 15 menit**

```bash
# Terminal 1: Launch robot di world dengan dinding
ros2 launch my_robot_description gazebo.launch.py

# Terminal 2: Cek topic LIDAR
ros2 topic list | grep scan
ros2 topic echo /scan --once

# Terminal 3: RViz2
rviz2
# Add → LaserScan → Topic: /scan
# Fixed Frame: odom
```

**Catat:**

| Parameter | Nilai |
|----------|-------|
| Topic name | |
| Frame ID | |
| Jumlah ranges | |
| Range min | |
| Range max | |

**Checkpoint ✅:** LIDAR scan terlihat di RViz2.

---

### PERCOBAAN 2: SLAM — BUAT PETA

**Estimasi waktu: 30 menit**

```bash
# Terminal 1: Gazebo (world dengan ruangan/koridor)
ros2 launch my_robot_description gazebo.launch.py

# Terminal 2: SLAM Toolbox
ros2 launch slam_toolbox online_async_launch.py \
  use_sim_time:=true

# Terminal 3: Teleop
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# Terminal 4: RViz2
rviz2
# Add → Map → Topic: /map
# Add → LaserScan → /scan
# Add → TF
# Fixed Frame: map
```

**Proses mapping:**
1. Gerakkan robot perlahan dengan teleop
2. Jelajahi seluruh area (koridor, ruangan)
3. Amati peta terbentuk secara real-time di RViz2
4. Pastikan semua dinding termapping (tidak ada area abu-abu)

**Screenshot peta:**
- Sebelum eksplorasi (awal)
- Tengah eksplorasi (50%)
- Setelah eksplorasi selesai (100%)

**Checkpoint ✅:** Peta 2D terbentuk di RViz2.

---

### PERCOBAAN 3: SIMPAN DAN MUAT PETA

**Estimasi waktu: 15 menit**

```bash
# Simpan peta
mkdir -p ~/maps
ros2 run nav2_map_server map_saver_cli -f ~/maps/lab_map

# Periksa output
ls ~/maps/
# lab_map.pgm  lab_map.yaml
```

Buka `lab_map.yaml`:

```yaml
image: lab_map.pgm
mode: trinary
resolution: 0.05
origin: [-5.0, -5.0, 0.0]
negate: 0
occupied_thresh: 0.65
free_thresh: 0.25
```

**Catat:**

| Parameter | Nilai |
|----------|-------|
| Resolution (m/pixel) | |
| Ukuran gambar (pixels) | |
| Area real (m²) | |

**Test muat peta:**
```bash
ros2 run nav2_map_server map_server --ros-args \
  -p yaml_filename:=$HOME/maps/lab_map.yaml \
  -p use_sim_time:=true

# Di terminal lain
ros2 lifecycle set /map_server activate
```

**Checkpoint ✅:** Peta tersimpan dan bisa dimuat ulang.

---

### PERCOBAAN 4: LOCALIZATION DENGAN AMCL

**Estimasi waktu: 25 menit**

```bash
# Terminal 1: Gazebo
ros2 launch my_robot_description gazebo.launch.py

# Terminal 2: Map server
ros2 run nav2_map_server map_server --ros-args \
  -p yaml_filename:=$HOME/maps/lab_map.yaml -p use_sim_time:=true
ros2 lifecycle set /map_server activate

# Terminal 3: AMCL
ros2 run nav2_amcl amcl --ros-args \
  -p use_sim_time:=true \
  -p set_initial_pose:=true \
  -p initial_pose.x:=0.0 \
  -p initial_pose.y:=0.0
ros2 lifecycle set /amcl activate

# Terminal 4: RViz2 — amati particle cloud
rviz2
# Add → PoseArray → Topic: /particlecloud
# Add → Map → /map
```

**Amati:**
- Awalnya partikel tersebar
- Gerakkan robot → partikel konvergen
- Semakin lama → partikel mengelompok di posisi benar

**Checkpoint ✅:** AMCL melokalisasi robot di peta.

---

### PERCOBAAN 5: NAV2 — NAVIGASI VIA RVIZ2

**Estimasi waktu: 30 menit**

```bash
# Launch Nav2 lengkap
ros2 launch nav2_bringup bringup_launch.py \
  map:=$HOME/maps/lab_map.yaml \
  use_sim_time:=true

# RViz2 (biasanya otomatis launch)
# 1. Set initial pose: klik "2D Pose Estimate" → klik+drag di posisi awal
# 2. Set goal: klik "2D Goal Pose" → klik+drag di posisi tujuan
# 3. Amati: path planning → robot bergerak otonom
```

**Dokumentasikan 3 navigasi:**

| No | Goal (x, y) | Jarak | Waktu Tempuh | Berhasil? |
|----|-------------|-------|-------------|-----------|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |

**Checkpoint ✅:** Robot bernavigasi otonom ke 3 goal.

---

### PERCOBAAN 6: NAVIGASI VIA PYTHON CODE

**Estimasi waktu: 30 menit**

Buat `nav_commander.py`:

```python
#!/usr/bin/env python3
import rclpy
from geometry_msgs.msg import PoseStamped
from nav2_simple_commander.robot_navigator import BasicNavigator
import math

def main():
    rclpy.init()
    nav = BasicNavigator()
    
    # Set initial pose
    init = PoseStamped()
    init.header.frame_id = 'map'
    init.header.stamp = nav.get_clock().now().to_msg()
    init.pose.position.x = 0.0
    init.pose.position.y = 0.0
    init.pose.orientation.w = 1.0
    nav.setInitialPose(init)
    nav.waitUntilNav2Active()
    
    # Waypoints
    waypoints = [
        (2.0, 0.0, 0.0),
        (2.0, 2.0, 1.57),
        (0.0, 2.0, 3.14),
        (0.0, 0.0, -1.57),
    ]
    
    for i, (x, y, yaw) in enumerate(waypoints):
        goal = PoseStamped()
        goal.header.frame_id = 'map'
        goal.header.stamp = nav.get_clock().now().to_msg()
        goal.pose.position.x = x
        goal.pose.position.y = y
        goal.pose.orientation.z = math.sin(yaw / 2)
        goal.pose.orientation.w = math.cos(yaw / 2)
        
        print(f'Navigating to waypoint {i+1}: ({x}, {y})')
        nav.goToPose(goal)
        
        while not nav.isTaskComplete():
            fb = nav.getFeedback()
            if fb:
                dist = fb.distance_remaining
                print(f'  Distance remaining: {dist:.2f} m')
        
        result = nav.getResult()
        print(f'  Result: {result}')
    
    print('All waypoints completed!')
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

**Checkpoint ✅:** Robot mengunjungi 4 waypoint via Python code.

---

### PERCOBAAN 7: COSTMAP DAN PATH ANALYSIS

**Estimasi waktu: 20 menit**

Di RViz2, amati:

1. **Global Costmap** → Add → Map → Topic: `/global_costmap/costmap`
2. **Local Costmap** → Add → Map → Topic: `/local_costmap/costmap`
3. **Global Path** → Add → Path → `/plan`
4. **Local Path** → Add → Path → `/local_plan`

**Dokumentasikan:**

| Aspek | Observasi |
|-------|-----------|
| Warna costmap (free/occupied/inflation) | |
| Global path planning algorithm | |
| Local controller behavior | |
| Recovery behavior saat stuck | |

**Test khusus:** Letakkan rintangan dinamis di path → amati replanning.

**Checkpoint ✅:** Costmap dan path planning tervisualisasi dan dipahami.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1 – LIDAR
1. Berapa sample per scan? Apa resolusi angular-nya?
2. Apakah semua dinding terdeteksi? Apa yang bisa menyebabkan blind spots?

### Analisa Percobaan 2 – SLAM
1. Berapa lama waktu yang dibutuhkan untuk memetakan seluruh area?
2. Apakah ada area yang sulit dimapping? Mengapa?

### Analisa Percobaan 3 – Map Save/Load
1. Apa hubungan resolution dan ukuran file peta?
2. Bagaimana peta berubah jika lingkungan berubah setelah mapping?

### Analisa Percobaan 4 – AMCL
1. Berapa lama partikel konvergen ke posisi benar?
2. Apa yang terjadi jika initial pose salah jauh?

### Analisa Percobaan 5-6 – Nav2
1. Apakah robot selalu ambil jalur terpendek? Atau jalur aman?
2. Apa yang terjadi jika goal inside rintangan di peta?

### Analisa Percobaan 7 – Costmap
1. Apa peran inflation layer dalam menghindari tabrakan?
2. Bagaimana local costmap membantu menghindari rintangan dinamis?

---

## E. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] Screenshot peta (3 tahap: awal, tengah, selesai)
- [ ] File peta (lab_map.pgm + lab_map.yaml)
- [ ] Screenshot RViz2 navigasi (path + costmap)
- [ ] Video navigasi otonom (minimal 2 goal)
- [ ] Kode nav_commander.py
- [ ] Screenshot AMCL particle convergence

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
