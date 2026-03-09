# MATERI MODUL 04: ROS 2 DASAR DAN GAZEBO

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – ROS Dasar dan Gazebo  
**Platform:** Ubuntu 22.04 + ROS 2 Humble + Gazebo  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan ROS 2](#1-pendahuluan-ros-2)
2. [Arsitektur ROS 2](#2-arsitektur-ros-2)
3. [Node, Topic, dan Message](#3-node-topic-dan-message)
4. [Publisher dan Subscriber](#4-publisher-dan-subscriber)
5. [Service dan Client](#5-service-dan-client)
6. [Action Server dan Client](#6-action-server-dan-client)
7. [Launch File](#7-launch-file)
8. [URDF – Unified Robot Description Format](#8-urdf--unified-robot-description-format)
9. [Gazebo Simulator](#9-gazebo-simulator)
10. [TF2 – Transform Library](#10-tf2--transform-library)
11. [Simulasi Robot di Gazebo](#11-simulasi-robot-di-gazebo)
12. [ros2_control dan Diff Drive Controller](#12-ros2_control-dan-diff-drive-controller)
13. [SLAM Toolbox](#13-slam-toolbox)
14. [Navigasi Otonom (Nav2)](#14-navigasi-otonom-nav2)
15. [Robot Lengan dan Manipulator](#15-robot-lengan-dan-ros2_control-manipulator)
16. [Multi-Robot dan Namespace](#16-multi-robot-dan-namespace)
17. [Gazebo World Plugin (rclcpp)](#17-gazebo-world-plugin-rclcpp)
18. [Referensi](#18-referensi)

---

## 1. PENDAHULUAN ROS 2

### 1.1 Apa itu ROS 2?

**ROS 2 (Robot Operating System 2)** adalah framework open-source untuk pengembangan software robotika. ROS 2 bukan operating system, melainkan middleware yang menyediakan:

- Komunikasi antar proses (DDS-based)
- Manajemen package dan dependency
- Tools untuk visualisasi, simulasi, debugging
- Ekosistem library untuk navigasi, manipulasi, persepsi

### 1.2 ROS 1 vs ROS 2

| Aspek | ROS 1 | ROS 2 |
|-------|-------|-------|
| Middleware | Custom (TCPROS/UDPROS) | DDS (Data Distribution Service) |
| Master | roscore wajib | Tidak perlu master (distributed) |
| OS support | Linux saja | Linux, Windows, macOS |
| Real-time | Tidak | Dukungan real-time |
| Security | Tidak ada | DDS Security |
| Python | Python 2 | Python 3 |
| Build system | catkin | colcon + ament |

### 1.3 Distribusi ROS 2

| Distribusi | EOL | Ubuntu |
|-----------|-----|--------|
| Foxy | Mei 2023 | 20.04 |
| **Humble** | Mei 2027 | **22.04** |
| Iron | Nov 2024 | 22.04 |
| Jazzy | Mei 2029 | 24.04 |

Praktikum ini menggunakan **ROS 2 Humble** (LTS).

### 1.4 Workspace Structure

```
ros2_ws/
├── build/          # Build artifacts
├── install/        # Installed packages
├── log/            # Build logs
└── src/            # Source packages
    ├── my_robot/
    │   ├── package.xml
    │   ├── setup.py (Python) / CMakeLists.txt (C++)
    │   ├── my_robot/
    │   │   ├── __init__.py
    │   │   ├── publisher.py
    │   │   └── subscriber.py
    │   └── launch/
    │       └── robot_launch.py
    └── my_robot_description/
        ├── urdf/
        ├── meshes/
        └── launch/
```

---

## 2. ARSITEKTUR ROS 2

### 2.1 Computation Graph

```
┌──────────┐  topic: /cmd_vel  ┌──────────┐
│ Teleop   │ ──────────────→   │ Robot    │
│ Node     │  Twist msg        │ Driver   │
└──────────┘                   └──────────┘
                                    │
                              topic: /odom
                                    │
                                    ↓
                              ┌──────────┐
                              │ SLAM     │
                              │ Node     │
                              └──────────┘
```

### 2.2 Konsep Utama

| Konsep | Deskripsi |
|--------|-----------|
| **Node** | Unit proses terkecil, menjalankan satu fungsi |
| **Topic** | Channel komunikasi publish-subscribe (async) |
| **Service** | Request-response (sync) |
| **Action** | Goal-feedback-result (async, preemptable) |
| **Parameter** | Konfigurasi node saat runtime |
| **Launch** | Menjalankan banyak node sekaligus |

### 2.3 CLI Tools

```bash
# Node
ros2 node list
ros2 node info /my_node

# Topic
ros2 topic list
ros2 topic echo /cmd_vel
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.5}, angular: {z: 0.0}}"

# Service
ros2 service list
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 1, b: 2}"

# Parameter
ros2 param list
ros2 param get /my_node my_param
```

---

## 3. NODE, TOPIC, DAN MESSAGE

### 3.1 Node

Node adalah proses yang menjalankan satu fungsi spesifik:
- `camera_node` → mengambil gambar
- `detector_node` → deteksi objek
- `driver_node` → mengontrol motor

### 3.2 Topic

Topic adalah **channel** untuk komunikasi asinkron. Publisher mengirim, subscriber menerima.

| Tipe | QoS Profile |
|------|-------------|
| Sensor data | Best effort, volatile |
| Command | Reliable, volatile |
| Map data | Reliable, transient local |

### 3.3 Message Types Penting

```
geometry_msgs/msg/Twist    # Kecepatan linear + angular
sensor_msgs/msg/LaserScan  # Data LIDAR
sensor_msgs/msg/Image      # Gambar kamera
nav_msgs/msg/Odometry      # Pose + velocity robot
std_msgs/msg/String        # String sederhana
```

---

## 4. PUBLISHER DAN SUBSCRIBER

### 4.1 Publisher (Python)

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

class VelocityPublisher(Node):
    def __init__(self):
        super().__init__('velocity_publisher')
        self.pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.timer = self.create_timer(0.1, self.timer_callback)  # 10 Hz
        self.get_logger().info('Velocity publisher started')
    
    def timer_callback(self):
        msg = Twist()
        msg.linear.x = 0.5   # m/s maju
        msg.angular.z = 0.0   # rad/s
        self.pub.publish(msg)

def main():
    rclpy.init()
    node = VelocityPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

### 4.2 Subscriber (Python)

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry

class OdomSubscriber(Node):
    def __init__(self):
        super().__init__('odom_subscriber')
        self.sub = self.create_subscription(
            Odometry, '/odom', self.odom_callback, 10)
    
    def odom_callback(self, msg):
        x = msg.pose.pose.position.x
        y = msg.pose.pose.position.y
        self.get_logger().info(f'Position: x={x:.2f}, y={y:.2f}')

def main():
    rclpy.init()
    node = OdomSubscriber()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
```

---

## 5. SERVICE DAN CLIENT

### 5.1 Service Definition (.srv)

```
# AddTwoInts.srv
int64 a
int64 b
---
int64 sum
```

### 5.2 Service Server

```python
from example_interfaces.srv import AddTwoInts

class AddServer(Node):
    def __init__(self):
        super().__init__('add_server')
        self.srv = self.create_service(
            AddTwoInts, 'add_two_ints', self.callback)
    
    def callback(self, request, response):
        response.sum = request.a + request.b
        self.get_logger().info(f'{request.a} + {request.b} = {response.sum}')
        return response
```

### 5.3 Service Client

```python
class AddClient(Node):
    def __init__(self):
        super().__init__('add_client')
        self.cli = self.create_client(AddTwoInts, 'add_two_ints')
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Waiting for service...')
    
    def send_request(self, a, b):
        req = AddTwoInts.Request()
        req.a = a
        req.b = b
        future = self.cli.call_async(req)
        return future
```

---

## 6. ACTION SERVER DAN CLIENT

### 6.1 Kapan Gunakan Action?

- Task yang membutuhkan **waktu lama** (navigasi, manipulasi)
- Membutuhkan **feedback** progress
- Bisa di-**cancel** (preemptable)

### 6.2 Contoh: Navigate to Pose

```
# NavigateToPose.action
geometry_msgs/PoseStamped pose  # Goal
---
std_msgs/Empty result            # Result
---
geometry_msgs/PoseStamped current_pose  # Feedback
float32 distance_remaining
```

---

## 7. LAUNCH FILE

### 7.1 Launch File Python

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='my_robot',
            executable='publisher',
            name='velocity_pub',
            output='screen',
            parameters=[{'speed': 0.5}]
        ),
        Node(
            package='my_robot',
            executable='subscriber',
            name='odom_sub',
            output='screen'
        ),
    ])
```

### 7.2 Menjalankan Launch

```bash
ros2 launch my_robot robot_launch.py
```

---

## 8. URDF – UNIFIED ROBOT DESCRIPTION FORMAT

### 8.1 Apa itu URDF?

URDF adalah format XML untuk mendeskripsikan model robot:
- **Links**: bagian rigid body (chassis, roda, sensor)
- **Joints**: koneksi antar link (revolute, prismatic, fixed)

### 8.2 Contoh URDF Sederhana

```xml
<?xml version="1.0"?>
<robot name="my_robot">
  <!-- Base Link -->
  <link name="base_link">
    <visual>
      <geometry>
        <box size="0.3 0.2 0.1"/>
      </geometry>
      <material name="blue">
        <color rgba="0 0 0.8 1"/>
      </material>
    </visual>
    <collision>
      <geometry>
        <box size="0.3 0.2 0.1"/>
      </geometry>
    </collision>
    <inertial>
      <mass value="1.0"/>
      <inertia ixx="0.001" ixy="0" ixz="0" iyy="0.001" iyz="0" izz="0.001"/>
    </inertial>
  </link>
  
  <!-- Left Wheel -->
  <link name="left_wheel">
    <visual>
      <geometry>
        <cylinder radius="0.05" length="0.02"/>
      </geometry>
      <material name="black">
        <color rgba="0 0 0 1"/>
      </material>
    </visual>
    <collision>
      <geometry>
        <cylinder radius="0.05" length="0.02"/>
      </geometry>
    </collision>
    <inertial>
      <mass value="0.1"/>
      <inertia ixx="0.0001" ixy="0" ixz="0" iyy="0.0001" iyz="0" izz="0.0001"/>
    </inertial>
  </link>
  
  <joint name="left_wheel_joint" type="continuous">
    <parent link="base_link"/>
    <child link="left_wheel"/>
    <origin xyz="0 0.11 -0.03" rpy="-1.5708 0 0"/>
    <axis xyz="0 0 1"/>
  </joint>
</robot>
```

### 8.3 Tipe Joint

| Tipe | Deskripsi | Contoh |
|------|-----------|--------|
| `fixed` | Tidak bergerak | Sensor ke chassis |
| `continuous` | Rotasi tanpa batas | Roda |
| `revolute` | Rotasi dengan batas | Joint lengan robot |
| `prismatic` | Translasi linear | Aktuator linear |

### 8.4 XACRO (XML Macros)

```xml
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro" name="my_robot">
  <xacro:property name="wheel_radius" value="0.05"/>
  
  <xacro:macro name="wheel" params="name y_offset">
    <link name="${name}">
      <visual>
        <geometry><cylinder radius="${wheel_radius}" length="0.02"/></geometry>
      </visual>
    </link>
    <joint name="${name}_joint" type="continuous">
      <parent link="base_link"/>
      <child link="${name}"/>
      <origin xyz="0 ${y_offset} -0.03" rpy="-1.5708 0 0"/>
      <axis xyz="0 0 1"/>
    </joint>
  </xacro:macro>
  
  <xacro:wheel name="left_wheel" y_offset="0.11"/>
  <xacro:wheel name="right_wheel" y_offset="-0.11"/>
</robot>
```

---

## 9. GAZEBO SIMULATOR

### 9.1 Apa itu Gazebo?

**Gazebo** adalah simulator 3D untuk robotika yang menyediakan:
- Simulasi fisika (ODE, Bullet, DART)
- Rendering 3D realistis
- Sensor virtual (kamera, LIDAR, IMU, ultrasonik)
- Integrasi penuh dengan ROS 2 via `ros_gz_bridge`

### 9.2 Gazebo Plugins untuk ROS 2

```xml
<!-- Dalam URDF: Differential Drive Plugin -->
<gazebo>
  <plugin filename="libgazebo_ros_diff_drive.so" name="diff_drive">
    <ros>
      <namespace>/</namespace>
    </ros>
    <left_joint>left_wheel_joint</left_joint>
    <right_joint>right_wheel_joint</right_joint>
    <wheel_separation>0.22</wheel_separation>
    <wheel_diameter>0.1</wheel_diameter>
    <max_wheel_torque>10</max_wheel_torque>
    <command_topic>cmd_vel</command_topic>
    <odometry_topic>odom</odometry_topic>
    <publish_odom>true</publish_odom>
    <publish_odom_tf>true</publish_odom_tf>
  </plugin>
</gazebo>
```

### 9.3 Sensor Plugins

```xml
<!-- LIDAR Sensor -->
<gazebo reference="lidar_link">
  <sensor type="ray" name="lidar">
    <pose>0 0 0 0 0 0</pose>
    <visualize>true</visualize>
    <update_rate>10</update_rate>
    <ray>
      <scan>
        <horizontal>
          <samples>360</samples>
          <resolution>1</resolution>
          <min_angle>-3.14159</min_angle>
          <max_angle>3.14159</max_angle>
        </horizontal>
      </scan>
      <range>
        <min>0.12</min>
        <max>10.0</max>
      </range>
    </ray>
    <plugin filename="libgazebo_ros_ray_sensor.so" name="lidar_plugin">
      <ros><remapping>~/out:=scan</remapping></ros>
      <output_type>sensor_msgs/LaserScan</output_type>
    </plugin>
  </sensor>
</gazebo>
```

---

## 10. TF2 – TRANSFORM LIBRARY

### 10.1 Konsep Frame

Robot memiliki banyak coordinate frame:

```
map → odom → base_link → lidar_link
                      → camera_link
                      → left_wheel
                      → right_wheel
```

### 10.2 CLI TF2

```bash
ros2 run tf2_tools view_frames    # Visualisasi tree
ros2 run tf2_ros tf2_echo map base_link  # Lihat transform
```

### 10.3 Publish Static Transform

```bash
ros2 run tf2_ros static_transform_publisher 0.1 0 0.05 0 0 0 base_link lidar_link
```

---

## 11. SIMULASI ROBOT DI GAZEBO

### 11.1 Launch Robot di Gazebo

```python
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    pkg = get_package_share_directory('my_robot_description')
    
    # Robot State Publisher
    robot_state_pub = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': open(
            os.path.join(pkg, 'urdf', 'robot.urdf')).read()}]
    )
    
    # Gazebo
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(get_package_share_directory('gazebo_ros'),
                        'launch', 'gazebo.launch.py')
        ]),
        launch_arguments={'world': os.path.join(pkg, 'worlds', 'empty.world')}.items()
    )
    
    # Spawn robot
    spawn = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-entity', 'my_robot', '-topic', 'robot_description'],
        output='screen'
    )
    
    return LaunchDescription([robot_state_pub, gazebo, spawn])
```

### 11.2 Kontrol Robot via Terminal

```bash
# Jalankan simulasi
ros2 launch my_robot_description gazebo.launch.py

# Di terminal lain, beri perintah gerak
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist \
  "{linear: {x: 0.5, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.3}}"
```

### 11.3 RViz2 untuk Visualisasi

```bash
rviz2
# Tambahkan display:
#   - RobotModel (topic: /robot_description)
#   - TF
#   - LaserScan (topic: /scan)
#   - Odometry (topic: /odom)
```

---

## 12. ROS2_CONTROL DAN DIFF DRIVE CONTROLLER

### 12.1 Arsitektur ros2_control

```
┌──────────────────────────────────────────────────────┐
│                  controller_manager                  │
│  ┌──────────────────┐   ┌──────────────────────┐    │
│  │joint_state_broad.│   │ diff_drive_controller │    │
│  └────────┬─────────┘   └──────────┬───────────┘    │
│           │ /joint_states           │ /odom, /cmd_vel│
└───────────┼─────────────────────────┼───────────────-┘
            │       Hardware Interface│
            └─────────┬───────────────┘
                      │ libgazebo_ros2_control.so
                    Gazebo
```

### 12.2 Konfigurasi YAML

```yaml
# ros2_control_diff_drive.yaml
controller_manager:
  ros__parameters:
    update_rate: 50
    joint_state_broadcaster:
      type: joint_state_broadcaster/JointStateBroadcaster
    diff_drive_controller:
      type: diff_drive_controller/DiffDriveController

diff_drive_controller:
  ros__parameters:
    left_wheel_names: ["roda_kiri"]
    right_wheel_names: ["roda_kanan"]
    wheel_separation: 0.23
    wheel_radius: 0.065
    publish_odom: true
    publish_odom_tf: true
```

### 12.3 Menjalankan Controller

```bash
# Spawn controller setelah controller_manager aktif
ros2 run controller_manager spawner joint_state_broadcaster
ros2 run controller_manager spawner diff_drive_controller

# Cek status
ros2 control list_controllers
```

---

## 13. SLAM TOOLBOX

### 13.1 Apa itu SLAM?

**SLAM** (Simultaneous Localization and Mapping) adalah proses robot:
1. **Memetakan** lingkungan yang tidak diketahui
2. Sambil **melokalisasi** dirinya dalam peta tersebut

### 13.2 slam_toolbox (ROS 2)

`slam_toolbox` adalah implementasi SLAM modern untuk ROS 2:
- **Async mode**: update peta tidak memblokir kontrol robot
- **Lifelong mapping**: peta bisa disimpan dan dilanjutkan
- **Loop closure**: deteksi bahwa robot kembali ke tempat yang sama

### 13.3 Konfigurasi Utama

```yaml
slam_toolbox:
  ros__parameters:
    use_sim_time: true
    scan_topic: /scan
    mode: mapping       # atau localization
    resolution: 0.05    # meter per pixel
    max_laser_range: 8.0
    do_loop_closing: true
```

### 13.4 Perintah Simpan Peta

```bash
ros2 run nav2_map_server map_saver_cli -f ~/maps/peta_saya
# Menghasilkan: peta_saya.pgm + peta_saya.yaml
```

---

## 14. NAVIGASI OTONOM (NAV2)

### 14.1 Stack Nav2

```
Goal Pose (RViz2)
       │
   bt_navigator (Behavior Tree)
       │
   planner_server ────→ global_costmap (peta statik + obstacle)
       │
   controller_server ──→ local_costmap (sensor real-time)
       │
   /cmd_vel → robot
```

### 14.2 Komponen Utama Nav2

| Komponen | Fungsi |
|---------|--------|
| `amcl` | Lokalisasi probabilistik (Adaptive Monte Carlo) |
| `map_server` | Load peta dari file .yaml |
| `planner_server` | Global path planning (Navfn/A*) |
| `controller_server` | Local path following (Regulated Pure Pursuit) |
| `bt_navigator` | Orkestrasi navigasi via Behavior Tree |
| `behavior_server` | Recovery behaviors (spin, back_up) |

### 14.3 Alur Navigasi

```bash
# 1. Launch Nav2 dengan peta
ros2 launch nav2_bringup bringup_launch.py \
  map:=/path/to/map.yaml use_sim_time:=true

# 2. Set initial pose di RViz2 (2D Pose Estimate)
# 3. Berikan goal di RViz2 (2D Goal Pose)
```

---

## 15. ROBOT LENGAN DAN ROS2_CONTROL MANIPULATOR

### 15.1 Deskripsi Hardware Interface (ros2_control)

```xml
<!-- Dalam XACRO: menggantikan <transmission> -->
<ros2_control name="ManipulatorSystem" type="system">
  <hardware>
    <plugin>gazebo_ros2_control/GazeboSystem</plugin>
  </hardware>
  <joint name="joint_1">
    <command_interface name="position"/>
    <state_interface name="position"/>
    <state_interface name="velocity"/>
  </joint>
</ros2_control>
```

### 15.2 Perintah ke Manipulator

```bash
# Gerakkan joint_1 ke 1.57 rad
ros2 topic pub /manipulator/joint_1_position_controller/commands \
  std_msgs/msg/Float64MultiArray "data: [1.57]"
```

---

## 16. MULTI-ROBOT DAN NAMESPACE

### 16.1 Namespace ROS 2

Namespace mengisolasi topik antar robot:

```
/robot1/cmd_vel   → Robot 1
/robot2/cmd_vel   → Robot 2
/robot1/odom      → Odometri Robot 1
```

### 16.2 Implementasi di Launch File

```python
from launch.actions import GroupAction
from launch_ros.actions import PushRosNamespace

GroupAction([
    PushRosNamespace('robot1'),
    Node(package='robot_state_publisher', ...),
    Node(package='gazebo_ros', executable='spawn_entity.py',
         arguments=[..., '-robot_namespace', 'robot1']),
])
```

---

## 17. GAZEBO WORLD PLUGIN (rclcpp)

### 17.1 Anatomi World Plugin

```cpp
#include <gazebo/gazebo.hh>
#include <gazebo_ros/node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>

namespace gazebo {
class MyWorldPlugin : public WorldPlugin {
public:
  void Load(physics::WorldPtr world, sdf::ElementPtr sdf) override {
    // Dapatkan node ROS 2 via gazebo_ros::Node::Get()
    ros_node_ = gazebo_ros::Node::Get(sdf);
    pub_ = ros_node_->create_publisher<std_msgs::msg::Float64>("/topic", 10);
    
    // Daftarkan callback per physics step
    update_conn_ = event::Events::ConnectWorldUpdateBegin(
        std::bind(&MyWorldPlugin::OnUpdate, this));
  }

  void OnUpdate() {
    // Dipanggil setiap step simulasi
    auto msg = std_msgs::msg::Float64();
    msg.data = world_->SimTime().Double();
    pub_->publish(msg);
    rclcpp::spin_some(ros_node_);
  }

private:
  gazebo_ros::Node::SharedPtr ros_node_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_;
  event::ConnectionPtr update_conn_;
};
GZ_REGISTER_WORLD_PLUGIN(MyWorldPlugin)
}
```

### 17.2 CMakeLists.txt untuk Plugin

```cmake
find_package(gazebo_ros REQUIRED)
find_package(gazebo_dev REQUIRED)
find_package(gazebo REQUIRED)

add_library(my_world_plugin SHARED src/plugins/my_world_plugin.cpp)
ament_target_dependencies(my_world_plugin rclcpp std_msgs gazebo_ros gazebo_dev)
target_link_libraries(my_world_plugin ${GAZEBO_LIBRARIES})
```

---

## 18. REFERENSI

1. ROS 2 Humble Documentation: https://docs.ros.org/en/humble/
2. Gazebo Tutorials: https://gazebosim.org/docs
3. slam_toolbox: https://github.com/SteveMacenski/slam_toolbox
4. Nav2 Documentation: https://navigation.ros.org/
5. ros2_control: https://control.ros.org/humble/
6. Pyo, Y. et al. (2017). *ROS Robot Programming*. ROBOTIS.
7. URDF Tutorial: https://docs.ros.org/en/humble/Tutorials/Intermediate/URDF/URDF-Main.html
8. Joseph, L. (2018). *ROS Robotics Projects* (2nd ed). Packt.

---

Dokumen ini menjadi acuan utama materi Modul 04 ROS 2 Dasar dan Gazebo.
