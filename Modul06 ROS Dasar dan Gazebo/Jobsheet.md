# JOBSHEET MODUL 08: ROS 2 DASAR DAN GAZEBO

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 08 – ROS Dasar dan Gazebo  
**Pertemuan:** 15–16 (2 × 2 SKS)  
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

1. Memahami arsitektur ROS 2 dan konsep node, topic, service, action
2. Membuat package ROS 2 Python
3. Membuat publisher dan subscriber custom
4. Membuat service server dan client
5. Menulis URDF robot differential drive sederhana
6. Men-spawn robot di Gazebo dan mengontrolnya via topic
7. Memvisualisasikan robot dan sensor di RViz2

---

## B. ALAT DAN BAHAN

| No | Item | Keterangan |
|----|------|-----------|
| 1 | Laptop/PC Ubuntu 22.04 | RAM ≥ 8 GB |
| 2 | ROS 2 Humble | Sudah terinstall (Modul 02) |
| 3 | Gazebo | `sudo apt install ros-humble-gazebo-ros-pkgs` |
| 4 | RViz2 | Sudah terinstall dengan ROS 2 |
| 5 | Text editor | VS Code + ROS extension |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: CLI ROS 2 DAN TURTLESIM

**Estimasi waktu: 20 menit**

```bash
# Terminal 1: Jalankan turtlesim
ros2 run turtlesim turtlesim_node

# Terminal 2: Teleop keyboard
ros2 run turtlesim turtle_teleop_key

# Terminal 3: Observasi
ros2 node list
ros2 topic list
ros2 topic echo /turtle1/cmd_vel
ros2 topic info /turtle1/cmd_vel
ros2 service list
```

**Catat:**

| Perintah | Output |
|---------|--------|
| `ros2 node list` | |
| `ros2 topic list` | |
| `ros2 topic info /turtle1/cmd_vel` | |
| `ros2 service list` (3 pertama) | |

**Checkpoint ✅:** Turtlesim berjalan dan dikontrol via keyboard.

---

### PERCOBAAN 2: MEMBUAT PACKAGE DAN PUBLISHER

**Estimasi waktu: 30 menit**

```bash
# Buat workspace
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src

# Buat package Python
ros2 pkg create --build-type ament_python --dependencies rclpy geometry_msgs \
  my_robot_pkg

# Edit publisher
cd my_robot_pkg/my_robot_pkg
```

Buat file `velocity_publisher.py`:

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import math

class VelocityPublisher(Node):
    def __init__(self):
        super().__init__('velocity_publisher')
        self.pub = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)
        self.timer = self.create_timer(0.1, self.callback)
        self.t = 0.0
        self.get_logger().info('Publisher started')
    
    def callback(self):
        msg = Twist()
        msg.linear.x = 1.0
        msg.angular.z = math.sin(self.t) * 2.0  # gerakan zigzag
        self.pub.publish(msg)
        self.t += 0.1

def main(args=None):
    rclpy.init(args=args)
    node = VelocityPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
```

Edit `setup.py` — tambahkan entry point:

```python
entry_points={
    'console_scripts': [
        'vel_pub = my_robot_pkg.velocity_publisher:main',
    ],
},
```

Build dan jalankan:

```bash
cd ~/ros2_ws
colcon build --packages-select my_robot_pkg
source install/setup.bash
ros2 run my_robot_pkg vel_pub
```

**Checkpoint ✅:** Turtle bergerak zigzag otomatis.

---

### PERCOBAAN 3: SUBSCRIBER

**Estimasi waktu: 20 menit**

Buat `pose_subscriber.py`:

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from turtlesim.msg import Pose

class PoseSubscriber(Node):
    def __init__(self):
        super().__init__('pose_subscriber')
        self.sub = self.create_subscription(Pose, '/turtle1/pose', self.callback, 10)
    
    def callback(self, msg):
        self.get_logger().info(
            f'x={msg.x:.2f} y={msg.y:.2f} theta={msg.theta:.2f}')

def main(args=None):
    rclpy.init(args=args)
    node = PoseSubscriber()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
```

Tambahkan di `setup.py`:
```python
'pose_sub = my_robot_pkg.pose_subscriber:main',
```

**Checkpoint ✅:** Node subscriber menerima dan menampilkan pose turtle.

---

### PERCOBAAN 4: SERVICE SERVER DAN CLIENT

**Estimasi waktu: 30 menit**

Buat `distance_service.py` — service yang menghitung jarak antara dua titik:

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts  # pakai sementara
import math

class DistanceServer(Node):
    def __init__(self):
        super().__init__('distance_server')
        self.srv = self.create_service(
            AddTwoInts, 'calc_distance', self.callback)
        self.get_logger().info('Distance service ready')
    
    def callback(self, request, response):
        # Simulasi: a dan b sebagai x*100+y (encode sederhana)
        response.sum = request.a + request.b  # placeholder
        self.get_logger().info(f'Request: {request.a}, {request.b} → {response.sum}')
        return response

def main(args=None):
    rclpy.init(args=args)
    node = DistanceServer()
    rclpy.spin(node)
```

Test via CLI:
```bash
ros2 service call /calc_distance example_interfaces/srv/AddTwoInts "{a: 3, b: 4}"
```

**Checkpoint ✅:** Service merespons request.

---

### PERCOBAAN 5: URDF ROBOT DIFFERENTIAL DRIVE

**Estimasi waktu: 40 menit**

```bash
cd ~/ros2_ws/src
ros2 pkg create --build-type ament_cmake my_robot_description
mkdir -p my_robot_description/urdf my_robot_description/launch
```

Buat `urdf/robot.urdf`:

```xml
<?xml version="1.0"?>
<robot name="my_robot">
  <!-- Base -->
  <link name="base_footprint"/>
  
  <link name="base_link">
    <visual>
      <geometry><box size="0.3 0.2 0.08"/></geometry>
      <material name="blue"><color rgba="0.1 0.1 0.8 1"/></material>
    </visual>
    <collision><geometry><box size="0.3 0.2 0.08"/></geometry></collision>
    <inertial>
      <mass value="2.0"/>
      <inertia ixx="0.01" ixy="0" ixz="0" iyy="0.01" iyz="0" izz="0.01"/>
    </inertial>
  </link>
  
  <joint name="base_joint" type="fixed">
    <parent link="base_footprint"/>
    <child link="base_link"/>
    <origin xyz="0 0 0.06"/>
  </joint>
  
  <!-- Left Wheel -->
  <link name="left_wheel">
    <visual>
      <geometry><cylinder radius="0.04" length="0.02"/></geometry>
      <material name="black"><color rgba="0 0 0 1"/></material>
    </visual>
    <collision><geometry><cylinder radius="0.04" length="0.02"/></geometry></collision>
    <inertial>
      <mass value="0.2"/>
      <inertia ixx="0.001" ixy="0" ixz="0" iyy="0.001" iyz="0" izz="0.001"/>
    </inertial>
  </link>
  
  <joint name="left_wheel_joint" type="continuous">
    <parent link="base_link"/>
    <child link="left_wheel"/>
    <origin xyz="0 0.11 -0.02" rpy="-1.5708 0 0"/>
    <axis xyz="0 0 1"/>
  </joint>
  
  <!-- Right Wheel -->
  <link name="right_wheel">
    <visual>
      <geometry><cylinder radius="0.04" length="0.02"/></geometry>
      <material name="black"><color rgba="0 0 0 1"/></material>
    </visual>
    <collision><geometry><cylinder radius="0.04" length="0.02"/></geometry></collision>
    <inertial>
      <mass value="0.2"/>
      <inertia ixx="0.001" ixy="0" ixz="0" iyy="0.001" iyz="0" izz="0.001"/>
    </inertial>
  </link>
  
  <joint name="right_wheel_joint" type="continuous">
    <parent link="base_link"/>
    <child link="right_wheel"/>
    <origin xyz="0 -0.11 -0.02" rpy="-1.5708 0 0"/>
    <axis xyz="0 0 1"/>
  </joint>
  
  <!-- Caster -->
  <link name="caster">
    <visual>
      <geometry><sphere radius="0.02"/></geometry>
      <material name="grey"><color rgba="0.5 0.5 0.5 1"/></material>
    </visual>
    <collision>
      <geometry><sphere radius="0.02"/></geometry>
    </collision>
    <inertial>
      <mass value="0.1"/>
      <inertia ixx="0.0001" ixy="0" ixz="0" iyy="0.0001" iyz="0" izz="0.0001"/>
    </inertial>
  </link>
  
  <joint name="caster_joint" type="fixed">
    <parent link="base_link"/>
    <child link="caster"/>
    <origin xyz="-0.12 0 -0.04"/>
  </joint>
  
  <!-- Gazebo diff_drive plugin -->
  <gazebo>
    <plugin filename="libgazebo_ros_diff_drive.so" name="diff_drive">
      <left_joint>left_wheel_joint</left_joint>
      <right_joint>right_wheel_joint</right_joint>
      <wheel_separation>0.22</wheel_separation>
      <wheel_diameter>0.08</wheel_diameter>
      <max_wheel_torque>5</max_wheel_torque>
      <command_topic>cmd_vel</command_topic>
      <odometry_topic>odom</odometry_topic>
      <publish_odom>true</publish_odom>
      <publish_odom_tf>true</publish_odom_tf>
      <odometry_frame>odom</odometry_frame>
      <robot_base_frame>base_footprint</robot_base_frame>
    </plugin>
  </gazebo>
  
  <gazebo reference="caster">
    <mu1>0.0</mu1>
    <mu2>0.0</mu2>
  </gazebo>
</robot>
```

Visualisasi URDF:
```bash
ros2 run robot_state_publisher robot_state_publisher \
  --ros-args -p robot_description:="$(cat ~/ros2_ws/src/my_robot_description/urdf/robot.urdf)"
rviz2  # Add RobotModel, set Fixed Frame = base_link
```

**Checkpoint ✅:** Robot terlihat di RViz2 dengan benar.

---

### PERCOBAAN 6: SPAWN ROBOT DI GAZEBO

**Estimasi waktu: 30 menit**

Buat `launch/gazebo.launch.py`:

```python
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    pkg = get_package_share_directory('my_robot_description')
    urdf_file = os.path.join(pkg, 'urdf', 'robot.urdf')
    
    with open(urdf_file, 'r') as f:
        robot_desc = f.read()
    
    return LaunchDescription([
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_desc}]
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([
                os.path.join(get_package_share_directory('gazebo_ros'),
                            'launch', 'gazebo.launch.py')
            ])
        ),
        Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=['-entity', 'my_robot', '-topic', 'robot_description'],
            output='screen'
        ),
    ])
```

```bash
cd ~/ros2_ws
colcon build
source install/setup.bash
ros2 launch my_robot_description gazebo.launch.py
```

**Test gerak:**
```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist \
  "{linear: {x: 0.3}, angular: {z: 0.5}}" --rate 10
```

**Checkpoint ✅:** Robot muncul di Gazebo dan bergerak via cmd_vel.

---

### PERCOBAAN 7: VISUALISASI DI RVIZ2

**Estimasi waktu: 20 menit**

```bash
# Terminal baru
rviz2
```

Konfigurasi RViz2:
1. Fixed Frame: `odom`
2. Add → RobotModel → Topic: `/robot_description`
3. Add → TF
4. Add → Odometry → Topic: `/odom`
5. Save config sebagai `robot.rviz`

Gerakkan robot dan amati:
- Model robot di RViz2 bergerak sesuai Gazebo
- TF tree menampilkan `odom → base_footprint → base_link → wheels`
- Odometry trail terlihat

**Checkpoint ✅:** RViz2 menampilkan robot, TF, dan odometry.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1 – CLI ROS 2

1. Apa peran turtlesim dalam ekosistem pembelajaran ROS 2?
2. Jelaskan perbedaan topic `/turtle1/cmd_vel` dan `/turtle1/pose`.

### Analisa Percobaan 2 – Publisher

1. Apa yang terjadi jika timer rate diubah dari 10 Hz ke 1 Hz?
2. Mengapa menggunakan `create_timer` dan bukan `while loop`?

### Analisa Percobaan 3 – Subscriber

1. Apa yang terjadi jika subscriber dijalankan sebelum publisher?
2. Jelaskan peran QoS (Quality of Service) pada subscriber.

### Analisa Percobaan 4 – Service

1. Kapan kita menggunakan service (request-response) vs topic (pub-sub)?
2. Apa yang terjadi jika service dipanggil saat server belum aktif?

### Analisa Percobaan 5 – URDF

1. Mengapa setiap link membutuhkan `<inertial>` untuk Gazebo?
2. Jelaskan peran `<collision>` vs `<visual>`.

### Analisa Percobaan 6 – Gazebo

1. Bagaimana plugin `diff_drive` mengubah `cmd_vel` menjadi gerakan roda?
2. Apakah odometry dari simulasi 100% akurat? Mengapa?

### Analisa Percobaan 7 – RViz2

1. Jelaskan perbedaan Gazebo dan RViz2.
2. Mengapa Fixed Frame harus di-set ke `odom` atau `map`?

---

## E. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] Screenshot turtlesim + teleop
- [ ] Screenshot node dan topic list
- [ ] Kode publisher dan subscriber
- [ ] File URDF lengkap
- [ ] Screenshot robot di Gazebo
- [ ] Screenshot robot di RViz2
- [ ] File launch

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
