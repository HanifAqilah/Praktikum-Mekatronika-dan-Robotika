# Praktikum Gazebo – Mekatronika dan Robotika
### ROS 2 Humble Hawksbill · Gazebo Classic 11

Paket ini berisi 10 percobaan bertahap untuk mempelajari simulasi robot menggunakan **Gazebo Classic** dan **ROS 2 Humble**, mulai dari tampilan world kosong hingga navigasi otonom dan plugin kustom.

---

## Prasyarat

| Komponen | Versi |
|---|---|
| Ubuntu | 22.04 LTS |
| ROS 2 | Humble Hawksbill |
| Gazebo | Classic 11 |

### Instalasi dependensi

```bash
sudo apt update
sudo apt install -y \
  ros-humble-gazebo-ros-pkgs \
  ros-humble-gazebo-ros2-control \
  ros-humble-ros2-control \
  ros-humble-ros2-controllers \
  ros-humble-slam-toolbox \
  ros-humble-nav2-bringup \
  ros-humble-nav2-map-server \
  ros-humble-teleop-twist-keyboard \
  ros-humble-joint-state-publisher-gui \
  ros-humble-robot-state-publisher \
  ros-humble-rviz2 \
  ros-humble-xacro
```

---

## Build Paket

```bash
cd ~/ros_ws   # atau sesuaikan dengan lokasi workspace Anda
source /opt/ros/humble/setup.bash
colcon build --packages-select gazebo_praktikum
source install/setup.bash
```

---

## Struktur Direktori

```
gazebo_praktikum/
├── CMakeLists.txt
├── package.xml
├── README.md
├── config/
│   ├── nav2_params.yaml          # Parameter Nav2 lengkap
│   ├── slam_params.yaml          # Parameter slam_toolbox
│   ├── ros2_control_diff_drive.yaml
│   ├── diff_drive_controller.yaml
│   └── manipulator_controllers.yaml
├── launch/
│   ├── percobaan1_empty_world.launch.py
│   ├── percobaan2_shapes.launch.py
│   ├── percobaan3_urdf_rviz.launch.py
│   ├── percobaan4_spawn_robot.launch.py
│   ├── percobaan5_sensor.launch.py
│   ├── percobaan6_teleop.launch.py
│   ├── percobaan7a_slam.launch.py
│   ├── percobaan7b_navigasi.launch.py
│   ├── percobaan8_manipulator.launch.py
│   ├── percobaan9_multi_robot.launch.py
│   └── percobaan10_advanced.launch.py
├── maps/                         # Simpan peta hasil SLAM di sini
├── rviz/                         # Konfigurasi RViz2
├── scripts/
│   ├── demo_manipulator.py       # Demo gerakan lengan 3-DOF (rclpy)
│   └── advanced_demo.py          # Demo spawn objek dinamis (rclpy)
├── src/plugins/
│   └── my_world_plugin.cpp       # World plugin kustom (rclcpp)
├── urdf/
│   ├── robot_sederhana.urdf      # Robot differential drive tanpa sensor
│   ├── robot_lengkap.urdf.xacro  # Robot dengan kamera + LIDAR + diff drive
│   └── robot_manipulator.urdf.xacro  # Lengan robot 3-DOF
└── worlds/
    ├── percobaan1_empty_world.world
    ├── percobaan2_shapes_world.world
    ├── percobaan4_robot_world.world
    ├── percobaan5_sensor_world.world
    ├── percobaan7_navigasi_world.world
    ├── percobaan9_multi_robot.world
    └── percobaan10_advanced_world.world
```

---

## Percobaan

### Percobaan 1 – World Kosong

**Tujuan:** Mengenal antarmuka Gazebo, memahami konsep dunia simulasi.

```bash
ros2 launch gazebo_praktikum percobaan1_empty_world.launch.py
```

**Yang dipelajari:**
- Tampilan Gazebo Classic
- Panel Insert, World, Model
- Navigasi kamera 3D (scroll zoom, middle-click rotate)
- Pengaturan gravitasi dan fisika di Edit → World Properties

---

### Percobaan 2 – Objek Primitif

**Tujuan:** Mengenal model SDF, menambah objek primitif ke world.

```bash
ros2 launch gazebo_praktikum percobaan2_shapes.launch.py
```

**Yang dipelajari:**
- Format SDF (Simulation Description Format)
- Elemen `<collision>`, `<visual>`, `<inertial>`
- Material dan warna Gazebo
- Properti fisika objek (massa, inersia)

---

### Percobaan 3 – Visualisasi URDF di RViz2

**Tujuan:** Memahami format URDF, memvisualisasikan robot tanpa Gazebo.

```bash
ros2 launch gazebo_praktikum percobaan3_urdf_rviz.launch.py
```

**Yang dipelajari:**
- Format URDF (link, joint, material)
- `robot_state_publisher` dan topik `/robot_description`
- `joint_state_publisher_gui` untuk menggerakkan joint
- Tampilan TF dan model di RViz2

**Topik penting:**
| Topik | Tipe | Keterangan |
|---|---|---|
| `/robot_description` | `std_msgs/String` | Deskripsi URDF robot |
| `/joint_states` | `sensor_msgs/JointState` | Status joint |
| `/tf` | `tf2_msgs/TFMessage` | Transform frames |

---

### Percobaan 4 – Spawn Robot di Gazebo

**Tujuan:** Meletakkan robot (URDF) ke dalam dunia Gazebo.

```bash
ros2 launch gazebo_praktikum percobaan4_spawn_robot.launch.py
```

**Yang dipelajari:**
- Service `/spawn_entity` untuk memasukkan robot
- Integrasi `robot_state_publisher` dengan Gazebo
- Melihat TF dan model robot di RViz2

---

### Percobaan 5 – Robot dengan Sensor

**Tujuan:** Menambahkan kamera dan LIDAR ke robot, membaca data sensor.

```bash
ros2 launch gazebo_praktikum percobaan5_sensor.launch.py
```

**Yang dipelajari:**
- Plugin Gazebo ROS 2 untuk kamera (`libgazebo_ros_camera.so`)
- Plugin LIDAR (`libgazebo_ros_ray_sensor.so`)
- Visualisasi point cloud dan laser scan di RViz2

**Topik sensor:**
| Topik | Tipe | Keterangan |
|---|---|---|
| `/camera/image_raw` | `sensor_msgs/Image` | Gambar dari kamera |
| `/camera/camera_info` | `sensor_msgs/CameraInfo` | Parameter kamera |
| `/scan` | `sensor_msgs/LaserScan` | Data laser LIDAR |

---

### Percobaan 6 – Teleop dan ros2_control

**Tujuan:** Menggerakkan robot secara manual menggunakan keyboard.

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan6_teleop.launch.py

# Terminal 2 (setelah controller siap)
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

**Yang dipelajari:**
- Arsitektur `ros2_control`: `controller_manager`, `joint_state_broadcaster`, `diff_drive_controller`
- Topik `/cmd_vel` untuk perintah kecepatan
- Odometri dari plugin diff drive

**Topik penting:**
| Topik | Tipe | Keterangan |
|---|---|---|
| `/cmd_vel` | `geometry_msgs/Twist` | Perintah kecepatan |
| `/odom` | `nav_msgs/Odometry` | Odometri robot |
| `/joint_states` | `sensor_msgs/JointState` | Status joint roda |

---

### Percobaan 7A – SLAM (Pemetaan)

**Tujuan:** Membuat peta lingkungan secara otomatis menggunakan SLAM.

```bash
# Terminal 1
ros2 launch gazebo_praktikum percobaan7a_slam.launch.py

# Terminal 2 – gerakkan robot untuk memetakan
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# Terminal 3 – simpan peta saat selesai
ros2 run nav2_map_server map_saver_cli -f ~/ros_ws/src/gazebo_praktikum/maps/peta_labirin
```

**Yang dipelajari:**
- `slam_toolbox` async mapping
- Topik `/map` (OccupancyGrid)
- Format file peta: `.pgm` + `.yaml`

---

### Percobaan 7B – Navigasi Otonom (Nav2)

**Tujuan:** Robot bernavigasi secara otonom menggunakan peta yang telah dibuat.

```bash
ros2 launch gazebo_praktikum percobaan7b_navigasi.launch.py \
  map:=/home/<user>/ros_ws/src/gazebo_praktikum/maps/peta_labirin.yaml
```

Setelah launch, buka RViz2 dan:
1. Klik **"2D Pose Estimate"** → klik di posisi awal robot di peta
2. Klik **"2D Goal Pose"** → klik tujuan navigasi

**Yang dipelajari:**
- Stack navigasi Nav2: planner, controller, AMCL
- Costmap (global dan local)
- Behavior Tree navigasi

---

### Percobaan 8 – Robot Lengan Manipulator

**Tujuan:** Mensimulasikan robot lengan 3-DOF dengan `ros2_control`.

```bash
ros2 launch gazebo_praktikum percobaan8_manipulator.launch.py
```

Script `demo_manipulator.py` akan otomatis berjalan dan menggerakkan lengan dalam urutan yang telah ditentukan, dilanjutkan gerakan sinusoidal.

**Menggerakkan manual:**
```bash
# Gerakkan joint secara manual
ros2 topic pub /manipulator/joint_1_position_controller/commands \
  std_msgs/msg/Float64MultiArray "data: [1.57]"
```

**Topik controller:**
| Topik | Tipe | Keterangan |
|---|---|---|
| `.../joint_1_position_controller/commands` | `Float64MultiArray` | Perintah joint 1 (yaw, ±π rad) |
| `.../joint_2_position_controller/commands` | `Float64MultiArray` | Perintah joint 2 (pitch bahu) |
| `.../joint_3_position_controller/commands` | `Float64MultiArray` | Perintah joint 3 (pitch siku) |

---

### Percobaan 9 – Multi Robot

**Tujuan:** Menjalankan lebih dari satu robot sekaligus dalam namespace berbeda.

```bash
ros2 launch gazebo_praktikum percobaan9_multi_robot.launch.py
```

**Yang dipelajari:**
- Namespace ROS 2 untuk mengisolasi robot
- Parameter `frame_prefix` pada `robot_state_publisher`
- Perintah kecepatan per robot:

```bash
# Gerakkan robot1
ros2 topic pub /robot1/cmd_vel geometry_msgs/msg/Twist \
  "{linear: {x: 0.3}, angular: {z: 0.0}}"

# Gerakkan robot2
ros2 topic pub /robot2/cmd_vel geometry_msgs/msg/Twist \
  "{linear: {x: 0.3}, angular: {z: 0.5}}"
```

---

### Percobaan 10 – Advanced (World Plugin)

**Tujuan:** Membuat dan menggunakan plugin Gazebo kustom berbasis `rclcpp`.

```bash
ros2 launch gazebo_praktikum percobaan10_advanced.launch.py
```

Plugin `libmy_world_plugin.so` yang dibuild dari `my_world_plugin.cpp` akan:
- Mempublikasikan waktu simulasi ke `/plugin/sim_time`
- Mempublikasikan status ke `/plugin/status`

Script `advanced_demo.py` akan di-spawn oleh launch file dan secara dinamis menambahkan kotak berwarna ke dunia simulasi setiap 5 detik.

**Memantau plugin:**
```bash
ros2 topic echo /plugin/sim_time
ros2 topic echo /plugin/status
```

---

## Perintah Berguna

```bash
# Lihat semua topik yang aktif
ros2 topic list

# Monitor topik tertentu
ros2 topic echo /scan

# Informasi topik
ros2 topic info /cmd_vel

# Lihat semua node
ros2 node list

# Lihat TF tree
ros2 run tf2_tools view_frames

# Simpan peta
ros2 run nav2_map_server map_saver_cli -f ~/maps/peta_saya

# Cek controller aktif
ros2 control list_controllers

# Cek hardware interface
ros2 control list_hardware_interfaces
```

---

## Troubleshooting

| Masalah | Solusi |
|---|---|
| Gazebo tidak terbuka | Cek `export GAZEBO_MODEL_PATH` dan install `ros-humble-gazebo-ros-pkgs` |
| Robot tidak muncul di Gazebo | Tunggu ~5 detik untuk TimerAction, atau perbesar `period` |
| Controller tidak aktif | `ros2 control list_controllers` – pastikan status `active` |
| SLAM tidak membuat peta | Pastikan topik `/scan` tersedia: `ros2 topic echo /scan` |
| Nav2 tidak menerima tujuan | Set initial pose di RViz2 terlebih dahulu |
| Plugin tidak ditemukan | `echo $GAZEBO_PLUGIN_PATH` dan pastikan `libmy_world_plugin.so` sudah di-build |
| `colcon build` error | `source /opt/ros/humble/setup.bash` sebelum build |

---

## Referensi

- [ROS 2 Humble Docs](https://docs.ros.org/en/humble/)
- [Gazebo Classic Docs](http://classic.gazebosim.org/tutorials)
- [Nav2 Docs](https://navigation.ros.org/)
- [slam_toolbox](https://github.com/SteveMacenski/slam_toolbox)
- [ros2_control](https://control.ros.org/humble/)
