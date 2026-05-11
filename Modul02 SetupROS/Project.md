# PROJECT MODUL 02: 10 VARIASI PROJECT ROS 2 UNTUK PEMBELAJARAN MENDALAM

**Program Studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS dan Dasar Robotika  
**Dosen Pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.  
**Pengerjaan:** Per Kelompok (maks. 4 orang) atau Individual  
**Platform:** Ubuntu 22.04 + ROS 2 Humble + Gazebo + RViz

---

## PENDAHULUAN

Setelah berhasil melakukan setup lingkungan ROS 2, mahasiswa diharapkan dapat mengimplementasikan pengetahuan dasar ke dalam project nyata. Berikut adalah 10 variasi project yang dirancang untuk memperdalam pemahaman ROS 2, mulai dari tingkat dasar hingga menengah.

Setiap project memiliki tingkat kesulitan yang berbeda dan mencakup berbagai aspek robotika: kinematika, kontrol, computer vision, navigasi, dan SLAM.

---

## PROJECT 1: SIMULASI ROBOT DIFFERENTIAL DRIVE DI GAZEBO

### Deskripsi
Membuat dan mensimulasikan robot differential drive sederhana di Gazebo. Robot dapat bergerak maju, mundur, dan berputar menggunakan perintah ROS 2 topic.

### Komponen yang Dipelajari
- URDF/Xacro robot description
- Gazebo plugins
- ROS 2 topics (cmd_vel)
- Differential drive controller

### Langkah-langkah

1. **Buat package baru:**
```bash
cd ~/ros2_ws/src
ros2 pkg create --build-type ament_cmake my_robot_description
```

2. **Buat URDF robot (my_robot.urdf):**
```xml
<?xml version="1.0"?>
<robot name="my_robot">
  <link name="base_link">
    <visual>
      <geometry><box size="0.5 0.3 0.1"/></geometry>
      <material name="blue"><color rgba="0 0 1 1"/></material>
    </visual>
    <collision>
      <geometry><box size="0.5 0.3 0.1"/></geometry>
    </collision>
    <inertial>
      <mass value="5"/>
      <inertia ixx="0.1" ixy="0" ixz="0" iyy="0.1" iyz="0" izz="0.1"/>
    </inertial>
  </link>
  
  <link name="left_wheel">
    <visual>
      <geometry><cylinder radius="0.1" length="0.05"/></geometry>
    </visual>
    <collision>
      <geometry><cylinder radius="0.1" length="0.05"/></geometry>
    </collision>
    <inertial>
      <mass value="0.5"/>
      <inertia ixx="0.001" ixy="0" ixz="0" iyy="0.001" iyz="0" izz="0.001"/>
    </inertial>
  </link>
  
  <link name="right_wheel">
    <visual>
      <geometry><cylinder radius="0.1" length="0.05"/></geometry>
    </visual>
    <collision>
      <geometry><cylinder radius="0.1" length="0.05"/></geometry>
    </collision>
    <inertial>
      <mass value="0.5"/>
      <inertia ixx="0.001" ixy="0" ixz="0" iyy="0.001" iyz="0" izz="0.001"/>
    </inertial>
  </link>
  
  <joint name="left_wheel_joint" type="continuous">
    <parent link="base_link"/>
    <child link="left_wheel"/>
    <origin xyz="-0.15 0.2 0"/>
    <axis xyz="0 1 0"/>
  </joint>
  
  <joint name="right_wheel_joint" type="continuous">
    <parent link="base_link"/>
    <child link="right_wheel"/>
    <origin xyz="-0.15 -0.2 0"/>
    <axis xyz="0 1 0"/>
  </joint>
</robot>
```

3. **Launch file untuk Gazebo:**
```python
# launch/my_robot.launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def generate_launch_description():
    return LaunchDescription([
        ExecuteProcess(
            cmd=['gazebo', '--verbose', '-s', 'libgazebo_ros_factory.so'],
            output='screen'),
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            arguments=['/path/to/my_robot.urdf']),
    ])
```

4. **Kontrol robot:**
```bash
# Gerakkan robot
ros2 topic pub /cmd_vel geometry_msgs/Twist "{linear: {x: 0.5}, angular: {z: 0.0}}"
```

### Rumus yang Digunakan

**Kinematika Differential Drive:**
```
v_linear = (v_right + v_left) / 2
ω_angular = (v_right - v_left) / L
```

Dimana `L` = wheelbase (jarak antar roda)

### Deliverable
- [ ] URDF robot valid
- [ ] Robot muncul di Gazebo
- [ ] Robot dapat bergerak dengan perintah topic
- [ ] Video demonstrasi gerakan robot

---

## PROJECT 2: NODE PENERIMA DAN PENGIRIM CUSTOM (PUB/SUB)

### Deskripsi
Membuat dua node ROS 2: satu node publisher yang mengirimkan data sensor simulasi, dan satu node subscriber yang memproses data tersebut.

### Komponen yang Dipelajari
- Custom message (.msg files)
- rclpy/rclcpp programming
- Publisher-Subscriber pattern

### Langkah-langkah

1. **Buat package dengan dependencies:**
```bash
ros2 pkg create --build-type ament_python sensor_package --dependencies rclpy std_msgs
```

2. **Buat custom message (sensor_data.msg):**
```
float64 temperature
float64 humidity
float64 pressure
string status
```

3. **Buat publisher node (sensor_publisher.py):**
```python
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
import random

class SensorPublisher(Node):
    def __init__(self):
        super().__init__('sensor_publisher')
        self.publisher_ = self.create_publisher(Float64MultiArray, 'sensor_data', 10)
        self.timer_ = self.create_timer(1.0, self.timer_callback)
        self.get_logger().info('Sensor Publisher Started')

    def timer_callback(self):
        msg = Float64MultiArray()
        msg.data = [random.uniform(20, 30), random.uniform(40, 60), random.uniform(1000, 1020)]
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: {msg.data}')

def main(args=None):
    rclpy.init(args=args)
    node = SensorPublisher()
    rclpy.spin(node)
    rclpy.shutdown()
```

4. **Buat subscriber node (sensor_subscriber.py):**
```python
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray

class SensorSubscriber(Node):
    def __init__(self):
        super().__init__('sensor_subscriber')
        self.subscription = self.create_subscription(
            Float64MultiArray,
            'sensor_data',
            self.listener_callback,
            10)
        self.get_logger().info('Sensor Subscriber Started')

    def listener_callback(self, msg):
        self.get_logger().info(f'Received - Temp: {msg.data[0]:.2f}, Hum: {msg.data[1]:.2f}, Press: {msg.data[2]:.2f}')

def main(args=None):
    rclpy.init(args=args)
    node = SensorSubscriber()
    rclpy.spin(node)
    rclpy.shutdown()
```

### Deliverable
- [ ] Custom message terbuat
- [ ] Publisher node berjalan dan publish data
- [ ] Subscriber node menerima dan memproses data
- [ ] Source code terdokumentasi

---

## PROJECT 3: KONTROL PID UNTUK MOTOR DC SIMULASI

### Deskripsi
Mengimplementasikan kontroler PID untuk mengontrol kecepatan motor DC di simulasi Gazebo.

### Komponen yang Dipelajari
- PID control theory
- Feedback control loop
- Gazebo ROS 2 plugins

### Rumus PID

```
u(t) = Kp × e(t) + Ki × ∫e(t)dt + Kd × de(t)/dt
```

Dimana:
- `e(t)` = setpoint - actual_value
- `Kp` = proportional gain
- `Ki` = integral gain
- `Kd` = derivative gain

### Implementasi Python (PID Controller):

```python
class PIDController:
    def __init__(self, Kp, Ki, Kd, dt):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.dt = dt
        self.integral = 0
        self.prev_error = 0

    def compute(self, setpoint, actual):
        error = setpoint - actual
        self.integral += error * self.dt
        derivative = (error - self.prev_error) / self.dt
        output = self.Kp * error + self.Ki * self.integral + self.Kd * derivative
        self.prev_error = error
        return output
```

### Langkah-langkah
1. Setup Gazebo dengan plugin motor DC
2. Buat node PID controller
3. Baca encoder feedback (topic /wheel_odom)
4. Hitung output PID
5. Publish ke cmd_vel dengan feedback term

### Deliverable
- [ ] Implementasi PID berjalan
- [ ] Robot mencapai target kecepatan dengan stabil
- [ ] Grafik response (setpoint vs actual)
- [ ] Analisis nilai Kp, Ki, Kd optimal

---

## PROJECT 4: NAVIGASI 2D DENGAN NAV2 (AMCL DAN PATH PLANNING)

### Deskripsi
Menggunakan Nav2 stack untuk melakukan navigasi otonom di lingkungan simulasi Gazebo dengan localization (AMCL) dan path planning.

### Komponen yang Dipelajari
- Navigation2 stack
- AMCL (Adaptive Monte Carlo Localization)
- Global planner (A*, Dijkstra)
- Local planner (DWB, TEB)

### Langkah-langkah

1. **Install Nav2:**
```bash
sudo apt install -y ros-humble-navigation2 ros-humble-nav2-bringup
```

2. **Buat map menggunakan SLAM Toolbox:**
```bash
ros2 launch slam_toolbox online_async_launch.py slam_params_file:=/path/to/mapper_params.yaml
# Jalanin robot dan buat map
# Save map: ros2 run nav2_map_server map_saver_cli -f my_map
```

3. **Launch Nav2:**
```bash
ros2 launch nav2_bringup bringup_launch.py map:=/path/to/my_map.yaml
```

4. **Kirim goal navigation:**
```bash
ros2 action send_goal /navigate_to_pose nav2_msgs/action/NavigateToPose "{pose: {header: {frame_id: 'map'}, pose: {position: {x: 2.0, y: 1.0, z: 0.0}, orientation: {w: 1.0}}}"
```

### Rumus Lokalisasi (AMCL)

**Particle Filter Update:**
```
weight_i = P(z | x_i, map) × P(x_i | u)
bel(x_t) = {<x_i, weight_i>} for i = 1...N
```

### Deliverable
- [ ] Map berhasil dibuat
- [ ] Nav2 berjalan dan melakukan lokalisasi
- [ ] Robot dapat mencapai goal secara otonom
- [ ] Video navigasi robot

---

## PROJECT 5: COMPUTER VISION DENGAN OPENCV DAN YOLO

### Deskripsi
Menggunakan kamera simulasi di Gazebo, mendeteksi objek menggunakan OpenCV dan YOLO, kemudian mengirimkan hasil deteksi sebagai ROS 2 message.

### Komponen yang Dipelajari
- OpenCV image processing
- YOLO object detection
- ROS 2 image transport
- cv_bridge

### Langkah-langkah

1. **Buat package:**
```bash
ros2 pkg create --build-type ament_python vision_package --dependencies rclpy sensor_msgs cv_bridge opencv2
```

2. **Node deteksi objek (object_detector.py):**
```python
import rclpy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
from ultralytics import YOLO

class ObjectDetector(Node):
    def __init__(self):
        super().__init__('object_detector')
        self.bridge = CvBridge()
        self.model = YOLO('yolov8n.pt')
        self.subscription = self.create_subscription(
            Image,
            '/camera/image_raw',
            self.listener_callback,
            10)
        self.publisher_ = self.create_publisher(String, 'detected_objects', 10)
        self.get_logger().info('Object Detector Started')

    def listener_callback(self, msg):
        cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        results = self.model(cv_image)
        detections = results[0].boxes
        for box in detections:
            cls = int(box.cls[0])
            conf = float(box.conf[0])
            if conf > 0.5:
                self.get_logger().info(f'Detected: {self.model.names[cls]} with confidence {conf:.2f}')
                self.publisher_.publish(String(data=f'{self.model.names[cls]}: {conf:.2f}'))

def main(args=None):
    rclpy.init(args=args)
    node = ObjectDetector()
    rclpy.spin(node)
    rclpy.shutdown()
```

### Rumus IoU (Intersection over Union)

```
IoU = Area_Intersection / Area_Union
IoU = (Area_Box1 ∩ Area_Box2) / (Area_Box1 ∪ Area_Box2)
```

### Deliverable
- [ ] Kamera Gazebo memberikan feed
- [ ] YOLO mendeteksi objek dengan confidence > 0.5
- [ ] Hasil deteksi dipublish ke ROS topic
- [ ] Screenshot deteksi objek

---

## PROJECT 6: SLAM DENGAN LIDAR DI GAZEBO

### Deskripsi
Melakukan SLAM (Simultaneous Localization and Mapping) menggunakan LIDAR simulasi di Gazebo untuk membuat peta lingkungan.

### Komponen yang Dipelajari
- SLAM Toolbox
- LIDAR sensor
- Map building
- Localization

### Langkah-langkah

1. **Tambahkan LIDAR ke URDF robot:**
```xml
<link name="lidar_link">
  <visual>
    <geometry><cylinder radius="0.05" length="0.1"/></geometry>
  </visual>
</link>
<joint name="lidar_joint" type="fixed">
  <parent link="base_link"/>
  <child link="lidar_link"/>
  <origin xyz="0.2 0 0.1"/>
</joint>
```

2. **Gazebo ROS LIDAR plugin:**
```xml
<gazebo reference="lidar_link">
  <sensor type="ray" name="lidar">
    <pose>0 0 0 0 0 0</pose>
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
        <min>0.1</min>
        <max>30.0</max>
      </range>
    </ray>
    <plugin name="gazebo_ros_lidar" filename="libgazebo_ros_ray_sensor.so">
      <ros>
        <topic_name>/scan</topic_name>
      </ros>
    </plugin>
  </sensor>
</gazebo>
```

3. **Jalankan SLAM Toolbox:**
```bash
ros2 launch slam_toolbox online_async_launch.py
```

4. **Visualisasi di RViz:**
```bash
rviz2
# Add: LaserScan (/scan), Map (/map)
```

### Deliverable
- [ ] LIDAR memberikan data scan
- [ ] SLAM membuat peta secara real-time
- [ ] Peta tersimpan (map.pgm dan map.yaml)
- [ ] Video proses mapping

---

## PROJECT 7: KINEMATIKA MANIPULATOR 2-DOF (FORWARD & INVERSE)

### Deskripsi
Mensimulasikan robot arm sederhana 2-DOF, menghitung forward kinematics dan inverse kinematics, serta mengontrol end-effector.

### Komponen yang Dipelajari
- DH Parameters
- Forward Kinematics
- Inverse Kinematics
- Joint control

### Rumus Kinematika

**Forward Kinematics (2-DOF Planar):**
```
x = l1 × cos(θ1) + l2 × cos(θ1 + θ2)
y = l1 × sin(θ1) + l2 × sin(θ1 + θ2)
```

**Inverse Kinematics:**
```
θ2 = arccos((x² + y² - l1² - l2²) / (2 × l1 × l2))
θ1 = atan2(y, x) - atan2(l2 × sin(θ2), l1 + l2 × cos(θ2))
```

### Implementasi Python (FK & IK):

```python
import math

def forward_kinematics(theta1, theta2, l1, l2):
    x = l1 * math.cos(theta1) + l2 * math.cos(theta1 + theta2)
    y = l1 * math.sin(theta1) + l2 * math.sin(theta1 + theta2)
    return x, y

def inverse_kinematics(x, y, l1, l2):
    d = (x**2 + y**2 - l1**2 - l2**2) / (2 * l1 * l2)
    theta2 = math.acos(d)
    theta1 = math.atan2(y, x) - math.atan2(l2 * math.sin(theta2), l1 + l2 * math.cos(theta2))
    return theta1, theta2
```

### Deliverable
- [ ] URDF robot arm 2-DOF
- [ ] FK: Input sudut → Output posisi benar
- [ ] IK: Input posisi → Output sudut benar
- [ ] Visualisasi di RViz

---

## PROJECT 8: MOVEIT MOTION PLANNING UNTUK ROBOT ARM

### Deskripsi
Menggunakan MoveIt untuk merencanakan gerakan robot arm ke target pose tertentu dengan obstacle avoidance.

### Komponent yang Dipelajari
- MoveIt Setup Assistant
- Motion planning algorithms (RRT, PRM)
- Trajectory execution
- Collision checking

### Langkah-langkah

1. **Jalankan MoveIt Setup Assistant:**
```bash
ros2 launch moveit_setup_assistant setup_assistant.launch.py
# Load URDF robot arm
# Generate MoveIt config
```

2. **Launch MoveIt:**
```bash
ros2 launch my_robot_moveit_config demo.launch.py
```

3. **Kirim goal pose menggunakan RViz:**
- Klik "Planning" tab
- Drag end-effector ke target pose
- Klik "Plan & Execute"

### Deliverable
- [ ] MoveIt config berhasil dibuat
- [ ] Robot arm dapat merencanakan jalur ke target
- [ ] Video motion planning dengan obstacle
- [ ] Analisis algoritma planning (RRT vs PRM)

---

## PROJECT 9: TELEOPERASI ROBOT DENGAN KEYBOARD/JOYSTICK

### Deskripsi
Membuat node teleoperasi untuk mengontrol robot secara manual menggunakan keyboard atau joystick gamepad.

### Komponen yang Dipelajari
- Input handling
- Twist messages
- Joy node (untuk gamepad)

### Implementasi Keyboard Teleop (keyboard_teleop.py):

```python
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import sys, tty, termios

class KeyboardTeleop(Node):
    def __init__(self):
        super().__init__('keyboard_teleop')
        self.publisher_ = self.create_publisher(Twist, '/cmd_vel', 10)
        self.get_logger().info('Keyboard Teleop Started - WASD untuk gerak, Q/E untuk rotasi')
        self.run()

    def get_key(self):
        fd = sys.stdin.fileno()
        old = termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            key = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)
        return key

    def run(self):
        while True:
            key = self.get_key()
            msg = Twist()
            if key == 'w':
                msg.linear.x = 0.5
            elif key == 's':
                msg.linear.x = -0.5
            elif key == 'a':
                msg.angular.z = 0.5
            elif key == 'd':
                msg.angular.z = -0.5
            elif key == 'q':
                msg.angular.z = 0.5
            elif key == 'e':
                msg.angular.z = -0.5
            self.publisher_.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = KeyboardTeleop()
    rclpy.shutdown()
```

### Deliverable
- [ ] Node teleop berjalan
- [ ] Robot merespons input keyboard
- [ ] (Opsional) Joystick teleop dengan joy node
- [ ] Video demonstrasi kontrol

---

## PROJECT 10: INTEGRASI SENSOR FUSION (IMU + ENCODER + LIDAR)

### Deskripsi
Melakukan sensor fusion menggunakan Extended Kalman Filter (EKF) untuk meningkatkan akurasi odometry dengan menggabungkan data IMU, encoder roda, dan LIDAR.

### Komponen yang Dipelajari
- Sensor fusion
- Extended Kalman Filter
- robot_localization package
- State estimation

### Rumus EKF (Simplified)

**Prediction:**
```
μ̄ = g(u, μ)
Σ̄ = G_t × Σ × G_t^T + R_t
```

**Correction:**
```
K = Σ̄ × H^T × (H × Σ̄ × H^T + Q)^-1
μ = μ̄ + K × (z - h(μ̄))
Σ = (I - K × H) × Σ̄
```

### Konfigurasi robot_localization (ekf.yaml):

```yaml
ekf_filter_node:
  ros__parameters:
    frequency: 30.0
    sensor_timeout: 0.1
    two_d_mode: true
    
    odom_frame: odom
    base_link_frame: base_link
    world_frame: odom
    
    odom0: /odom
    odom0_config: [true, true, false, false, false, true, true, true, false, false, false, true, false, false, false]
    
    imu0: /imu/data
    imu0_config: [false, false, false, true, true, true, false, false, false, true, true, true, false, false, false]
```

### Langkah-langkah
1. Install robot_localization:
```bash
sudo apt install -y ros-humble-robot-localization
```

2. Launch EKF:
```bash
ros2 launch robot_localization ekf_node.launch.py config:=/path/to/ekf.yaml
```

### Deliverable
- [ ] EKF berjalan dan menerima data multi-sensor
- [ ] Odometry lebih stabil dibanding single sensor
- [ ] Grafik perbandingan odometry (dengan dan tanpa EKF)
- [ ] Analisis peningkatan akurasi

---

## PENUTUP

### Kesimpulan
Sepuluh project di atas mencakup spektrum luas dari robotika dengan ROS 2:
- Dasar ROS 2 (Project 2)
- Mobile robot (Project 1, 3, 4, 9)
- Computer vision (Project 5)
- Mapping & SLAM (Project 6)
- Manipulator (Project 7, 8)
- Sensor fusion (Project 10)

Mahasiswa disarankan mengerjakan project secara berurutan sesuai kemampuan masing-masing.

### Referensi
1. ROS 2 Documentation: https://docs.ros.org/en/humble/
2. Nav2 Documentation: https://navigation.ros.org/
3. MoveIt Tutorials: https://moveit.picknik.ai/
4. Gazebo Tutorials: https://gazebosim.org/docs/
5. OpenCV Documentation: https://docs.opencv.org/

---

**Dosen Pengampu:**  
Rofiq Cahyo Prayogo, S.T., M.T.
