# MATERI MODUL 11: ROS 2 SLAM DAN NAVIGASI

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 11 – ROS SLAM dan Navigasi  
**Platform:** Ubuntu 22.04 + ROS 2 Humble + Nav2 + SLAM Toolbox  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan SLAM](#1-pendahuluan-slam)
2. [Sensor untuk SLAM: LIDAR](#2-sensor-untuk-slam-lidar)
3. [Occupancy Grid Map](#3-occupancy-grid-map)
4. [SLAM Toolbox](#4-slam-toolbox)
5. [Cartographer (Alternatif)](#5-cartographer-alternatif)
6. [Nav2 — Navigation Stack](#6-nav2--navigation-stack)
7. [Costmap](#7-costmap)
8. [Path Planning](#8-path-planning)
9. [Behavior Tree](#9-behavior-tree)
10. [Localization: AMCL](#10-localization-amcl)
11. [Full Navigation Pipeline](#11-full-navigation-pipeline)
12. [Referensi](#12-referensi)

---

## 1. PENDAHULUAN SLAM

### 1.1 Definisi

**SLAM (Simultaneous Localization and Mapping)** adalah proses dimana robot secara simultan:
- **Mapping**: membangun peta lingkungan
- **Localization**: menentukan posisi dirinya di dalam peta tersebut

### 1.2 The Chicken-and-Egg Problem

- Untuk membuat peta, robot perlu tahu posisinya
- Untuk tahu posisi, robot perlu peta
- SLAM menyelesaikan keduanya **secara bersamaan**

### 1.3 Jenis SLAM

| Jenis | Sensor | Output |
|-------|--------|--------|
| **2D SLAM** | 2D LIDAR | Occupancy grid (gambar 2D) |
| **3D SLAM** | 3D LIDAR / Depth camera | Point cloud 3D |
| **Visual SLAM** | Kamera mono/stereo | Sparse/dense 3D map |
| **RGBD SLAM** | Kamera RGB-D | Dense 3D map |

### 1.4 Alur SLAM

```
[LIDAR data] + [Odometry] → [SLAM Algorithm] → [Map] + [Robot Pose]
     /scan        /odom         slam_toolbox       /map    /tf (map→odom)
```

---

## 2. SENSOR UNTUK SLAM: LIDAR

### 2.1 LIDAR 2D

LIDAR (Light Detection and Ranging) memancarkan laser dan mengukur waktu pantulan.

| Parameter | Typical Value |
|----------|--------------|
| Range | 0.15–12 m |
| Angular resolution | 0.36°–1° |
| Scan rate | 5–15 Hz |
| Field of view | 360° |

### 2.2 ROS 2 LaserScan Message

```
sensor_msgs/LaserScan:
  header:
    stamp: {sec, nanosec}
    frame_id: "lidar_link"
  angle_min: -3.14159    # -π
  angle_max: 3.14159     # π
  angle_increment: 0.01745  # ~1°
  range_min: 0.15
  range_max: 12.0
  ranges: [1.2, 1.3, 1.1, ..., 999]  # 360 values
```

### 2.3 Visualisasi di RViz2

```bash
rviz2
# Add → LaserScan → Topic: /scan
# Set Size: 0.05, Color: Flat Color → Red
```

---

## 3. OCCUPANCY GRID MAP

### 3.1 Konsep

Occupancy Grid membagi lingkungan menjadi **grid sel** (cell), setiap sel memiliki probabilitas terisi:

| Nilai | Warna | Arti |
|-------|-------|------|
| 0 | Putih | Free space (kosong) |
| 100 | Hitam | Occupied (dinding/rintangan) |
| -1 | Abu-abu | Unknown (belum dieksplore) |

### 3.2 ROS 2 OccupancyGrid Message

```
nav_msgs/OccupancyGrid:
  header: {frame_id: "map"}
  info:
    resolution: 0.05    # 5 cm per cell
    width: 200           # 200 cells = 10 m
    height: 200
    origin: {x: -5.0, y: -5.0}
  data: [0, 0, -1, 100, 0, ...]  # width × height values
```

### 3.3 Menyimpan dan Memuat Peta

```bash
# Simpan peta
ros2 run nav2_map_server map_saver_cli -f my_map

# Output: my_map.pgm (gambar) + my_map.yaml (metadata)

# Muat peta
ros2 run nav2_map_server map_server --ros-args -p yaml_filename:=my_map.yaml
```

---

## 4. SLAM TOOLBOX

### 4.1 Instalasi

```bash
sudo apt install ros-humble-slam-toolbox
```

### 4.2 Mode Operasi

| Mode | Deskripsi |
|------|-----------|
| **Online Async** | Real-time SLAM saat robot bergerak |
| **Online Sync** | Sync dengan scan — lebih akurat, lebih lambat |
| **Offline** | Proses bag file untuk buat peta |
| **Localization** | Localize di peta yang sudah ada (no mapping) |

### 4.3 Menjalankan SLAM Toolbox

```bash
ros2 launch slam_toolbox online_async_launch.py
```

### 4.4 Parameter Penting

```yaml
# slam_toolbox_params.yaml
slam_toolbox:
  ros__parameters:
    odom_frame: odom
    map_frame: map
    base_frame: base_footprint
    scan_topic: /scan
    mode: mapping   # mapping / localization
    
    resolution: 0.05
    max_laser_range: 12.0
    
    # Loop closure
    do_loop_closing: true
    loop_search_maximum_distance: 3.0
```

---

## 5. CARTOGRAPHER (ALTERNATIF)

### 5.1 Instalasi

```bash
sudo apt install ros-humble-cartographer ros-humble-cartographer-ros
```

### 5.2 Konfigurasi

```lua
-- cartographer.lua
include "map_builder.lua"
include "trajectory_builder.lua"

options = {
  map_builder = MAP_BUILDER,
  trajectory_builder = TRAJECTORY_BUILDER,
  map_frame = "map",
  tracking_frame = "base_link",
  published_frame = "odom",
  odom_frame = "odom",
  provide_odom_frame = false,
  use_odometry = true,
  use_nav_sat = false,
  num_laser_scans = 1,
}

MAP_BUILDER.use_trajectory_builder_2d = true
TRAJECTORY_BUILDER_2D.min_range = 0.15
TRAJECTORY_BUILDER_2D.max_range = 12.0
```

---

## 6. NAV2 — NAVIGATION STACK

### 6.1 Arsitektur Nav2

```
                    ┌──────────────────────────┐
                    │     Behavior Tree         │
                    │   (navigate_to_pose)      │
                    └─────────┬────────────────┘
                              │
              ┌───────────────┼───────────────┐
              │               │               │
    ┌─────────▼──────┐ ┌─────▼─────┐ ┌──────▼──────┐
    │   Planner      │ │Controller │ │ Recovery    │
    │ (NavFn/Theta*) │ │(DWB/MPPI) │ │ (Spin/Back) │
    └────────┬───────┘ └─────┬─────┘ └─────────────┘
             │               │
    ┌────────▼───────────────▼──────┐
    │        Costmap 2D             │
    │  (global_costmap / local)     │
    └───────────────────────────────┘
```

### 6.2 Instalasi Nav2

```bash
sudo apt install ros-humble-navigation2 ros-humble-nav2-bringup
```

### 6.3 Menjalankan Nav2

```bash
# Dengan peta
ros2 launch nav2_bringup bringup_launch.py \
  map:=/path/to/my_map.yaml \
  use_sim_time:=true

# Atau navigation launch
ros2 launch nav2_bringup navigation_launch.py use_sim_time:=true
```

---

## 7. COSTMAP

### 7.1 Jenis Costmap

| Costmap | Tujuan | Ukuran |
|---------|--------|--------|
| **Global Costmap** | Path planning jarak jauh | Seluruh peta |
| **Local Costmap** | Obstacle avoidance lokal | Area sekitar robot (3×3 m) |

### 7.2 Layers

```yaml
global_costmap:
  ros__parameters:
    update_frequency: 1.0
    publish_frequency: 1.0
    global_frame: map
    robot_base_frame: base_link
    resolution: 0.05
    plugins: ["static_layer", "obstacle_layer", "inflation_layer"]
    
    static_layer:
      plugin: "nav2_costmap_2d::StaticLayer"
      map_subscribe_transient_local: true
    
    obstacle_layer:
      plugin: "nav2_costmap_2d::ObstacleLayer"
      observation_sources: scan
      scan:
        topic: /scan
        data_type: "LaserScan"
    
    inflation_layer:
      plugin: "nav2_costmap_2d::InflationLayer"
      cost_scaling_factor: 3.0
      inflation_radius: 0.55
```

### 7.3 Inflation

Inflation menambahkan "zona bahaya" di sekitar rintangan:
- Titik rintangan: cost = 254 (lethal)
- Semakin jauh: cost menurun eksponensial
- Di luar `inflation_radius`: cost = 0

---

## 8. PATH PLANNING

### 8.1 Global Planner

| Algoritma | Deskripsi |
|----------|-----------|
| **NavFn** | Dijkstra / A* pada costmap |
| **Theta*** | Any-angle path planning |
| **Smac Planner** | State lattice planner |

### 8.2 Local Controller

| Controller | Deskripsi |
|-----------|-----------|
| **DWB** | Dynamic Window Approach |
| **TEB** | Timed Elastic Band |
| **MPPI** | Model Predictive Path Integral |
| **Regulated Pure Pursuit** | Smooth path following |

### 8.3 A* Algorithm (Singkat)

$$f(n) = g(n) + h(n)$$

- $g(n)$: cost dari start ke node $n$
- $h(n)$: heuristic (estimasi cost ke goal)
- $f(n)$: total estimated cost

---

## 9. BEHAVIOR TREE

### 9.1 Konsep

Nav2 menggunakan **Behavior Tree (BT)** untuk mengorganisir navigasi:

```
Root
├── Sequence
│   ├── ComputePathToPose (Planner)
│   ├── FollowPath (Controller)
│   └── Success
├── Recovery
│   ├── Spin (putar 360°)
│   ├── BackUp (mundur)
│   └── Wait
```

### 9.2 Konfigurasi BT

```yaml
bt_navigator:
  ros__parameters:
    default_bt_xml_filename: "navigate_to_pose_w_replanning_and_recovery.xml"
    plugin_lib_names:
      - nav2_compute_path_to_pose_action_bt_node
      - nav2_follow_path_action_bt_node
      - nav2_spin_action_bt_node
      - nav2_back_up_action_bt_node
```

---

## 10. LOCALIZATION: AMCL

### 10.1 AMCL (Adaptive Monte Carlo Localization)

AMCL menggunakan **particle filter** untuk melokalisasi robot di peta yang sudah ada.

```
Particles (banyak titik hipotesis posisi)
     ↓ motion model update (odometry)
     ↓ sensor model update (LIDAR scan vs map)
     ↓ resample (hapus partikel buruk, duplikasi partikel baik)
     ↓ Estimated pose = weighted average particles
```

### 10.2 Parameter AMCL

```yaml
amcl:
  ros__parameters:
    max_particles: 2000
    min_particles: 500
    robot_model_type: "nav2_amcl::DifferentialMotionModel"
    scan_topic: /scan
    tf_broadcast: true
    set_initial_pose: true
    initial_pose:
      x: 0.0
      y: 0.0
      yaw: 0.0
```

---

## 11. FULL NAVIGATION PIPELINE

### 11.1 Pipeline SLAM → Save Map → Navigation

```bash
# Step 1: SLAM (buat peta sambil teleop)
ros2 launch my_robot_description gazebo.launch.py
ros2 launch slam_toolbox online_async_launch.py
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# Step 2: Simpan peta
ros2 run nav2_map_server map_saver_cli -f ~/maps/my_map

# Step 3: Navigation (gunakan peta)
ros2 launch nav2_bringup bringup_launch.py \
  map:=$HOME/maps/my_map.yaml use_sim_time:=true

# Step 4: Set initial pose di RViz2 (2D Pose Estimate)
# Step 5: Set goal di RViz2 (2D Goal Pose) → robot navigasi otonom!
```

### 11.2 Navigasi via Code

```python
from geometry_msgs.msg import PoseStamped
from nav2_simple_commander.robot_navigator import BasicNavigator

nav = BasicNavigator()

# Set initial pose
init_pose = PoseStamped()
init_pose.header.frame_id = 'map'
init_pose.pose.position.x = 0.0
init_pose.pose.position.y = 0.0
init_pose.pose.orientation.w = 1.0
nav.setInitialPose(init_pose)
nav.waitUntilNav2Active()

# Navigate to goal
goal = PoseStamped()
goal.header.frame_id = 'map'
goal.pose.position.x = 3.0
goal.pose.position.y = 2.0
goal.pose.orientation.w = 1.0

nav.goToPose(goal)

while not nav.isTaskComplete():
    feedback = nav.getFeedback()
    print(f'Distance remaining: {feedback.distance_remaining:.2f}')

result = nav.getResult()
print(f'Navigation result: {result}')
```

---

## 12. REFERENSI

1. Thrun, S. et al. (2005). *Probabilistic Robotics*. MIT Press.
2. Nav2 Documentation: https://docs.nav2.org
3. SLAM Toolbox: https://github.com/SteveMacenski/slam_toolbox
4. Cartographer ROS: https://google-cartographer-ros.readthedocs.io
5. ROS 2 Navigation: https://docs.ros.org/en/humble/
6. Macenski, S. et al. (2020). *Marathon 2: A Navigation System*. IEEE/RSJ IROS.

---

Dokumen ini menjadi acuan utama materi Modul 11 ROS 2 SLAM dan Navigasi.
