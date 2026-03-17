# MATERI MODUL 09: ROS 2 KINEMATIKA DAN KONTROLER

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 09 – ROS Kinematika dan Kontroler  
**Platform:** Ubuntu 22.04 + ROS 2 Humble + Gazebo  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan Kinematika Robot](#1-pendahuluan-kinematika-robot)
2. [Kinematika Differential Drive](#2-kinematika-differential-drive)
3. [Forward Kinematics](#3-forward-kinematics)
4. [Inverse Kinematics](#4-inverse-kinematics)
5. [Odometry dari Encoder](#5-odometry-dari-encoder)
6. [Denavit-Hartenberg (DH) Parameters](#6-denavit-hartenberg-dh-parameters)
7. [Kontroler PID di ROS 2](#7-kontroler-pid-di-ros-2)
8. [ros2_control Framework](#8-ros2_control-framework)
9. [Go-to-Goal Controller](#9-go-to-goal-controller)
10. [Trajectory Following](#10-trajectory-following)
11. [Referensi](#11-referensi)

---

## 1. PENDAHULUAN KINEMATIKA ROBOT

### 1.1 Kinematika vs Dinamika

| Aspek | Kinematika | Dinamika |
|-------|-----------|---------|
| Fokus | Gerak tanpa membahas gaya | Gerak + gaya |
| Input | Kecepatan roda/joint | Torsi/gaya |
| Output | Posisi, orientasi, kecepatan | Percepatan, gaya reaksi |
| Kompleksitas | Lebih sederhana | Perlu massa, inersia |

### 1.2 Jenis Kinematika

- **Forward Kinematics (FK)**: Diketahui kecepatan/posisi joint → hitung posisi/orientasi end-effector
- **Inverse Kinematics (IK)**: Diketahui posisi target → hitung kecepatan/posisi joint yang diperlukan

---

## 2. KINEMATIKA DIFFERENTIAL DRIVE

### 2.1 Konfigurasi

```
        ↑ x (depan)
        │
   ┌────┼────┐
   │    │    │
[Roda L]│[Roda R]
   │    │    │
   └────┼────┘
        │
        O (caster belakang)

- L = jarak antar roda (wheel separation)
- r = radius roda
- ωL = kecepatan angular roda kiri (rad/s)
- ωR = kecepatan angular roda kanan (rad/s)
```

### 2.2 Parameter

| Simbol | Deskripsi | Satuan |
|--------|-----------|--------|
| $r$ | Radius roda | meter |
| $L$ | Jarak antar roda (track width) | meter |
| $v$ | Kecepatan linear robot | m/s |
| $\omega$ | Kecepatan angular robot | rad/s |
| $\omega_L$ | Kecepatan angular roda kiri | rad/s |
| $\omega_R$ | Kecepatan angular roda kanan | rad/s |

---

## 3. FORWARD KINEMATICS

### 3.1 Dari Roda ke Robot

$$v = \frac{r(\omega_R + \omega_L)}{2}$$

$$\omega = \frac{r(\omega_R - \omega_L)}{L}$$

### 3.2 Dari Robot ke World (Pose Update)

$$\dot{x} = v \cos(\theta)$$
$$\dot{y} = v \sin(\theta)$$
$$\dot{\theta} = \omega$$

### 3.3 Diskretisasi (Euler)

$$x_{k+1} = x_k + v_k \cos(\theta_k) \cdot \Delta t$$
$$y_{k+1} = y_k + v_k \sin(\theta_k) \cdot \Delta t$$
$$\theta_{k+1} = \theta_k + \omega_k \cdot \Delta t$$

### 3.4 Implementasi Python

```python
import math

class DiffDriveFK:
    def __init__(self, wheel_radius=0.04, wheel_separation=0.22):
        self.r = wheel_radius
        self.L = wheel_separation
        self.x = 0.0
        self.y = 0.0
        self.theta = 0.0
    
    def update(self, omega_left, omega_right, dt):
        v = self.r * (omega_right + omega_left) / 2.0
        w = self.r * (omega_right - omega_left) / self.L
        
        self.x += v * math.cos(self.theta) * dt
        self.y += v * math.sin(self.theta) * dt
        self.theta += w * dt
        self.theta = math.atan2(math.sin(self.theta), math.cos(self.theta))
        
        return v, w
```

---

## 4. INVERSE KINEMATICS

### 4.1 Dari Robot ke Roda

Diberikan kecepatan linear $v$ dan angular $\omega$ yang diinginkan:

$$\omega_L = \frac{v - \frac{\omega L}{2}}{r}$$

$$\omega_R = \frac{v + \frac{\omega L}{2}}{r}$$

### 4.2 Implementasi Python

```python
class DiffDriveIK:
    def __init__(self, wheel_radius=0.04, wheel_separation=0.22):
        self.r = wheel_radius
        self.L = wheel_separation
    
    def compute(self, v, omega):
        omega_left  = (v - omega * self.L / 2.0) / self.r
        omega_right = (v + omega * self.L / 2.0) / self.r
        return omega_left, omega_right
```

### 4.3 Contoh Perhitungan

| Gerakan | v (m/s) | ω (rad/s) | ωL (rad/s) | ωR (rad/s) |
|---------|---------|-----------|-----------|-----------|
| Maju lurus | 0.3 | 0.0 | 7.5 | 7.5 |
| Putar di tempat CW | 0.0 | 1.0 | -2.75 | 2.75 |
| Belok kiri | 0.3 | 0.5 | 6.13 | 8.88 |

---

## 5. ODOMETRY DARI ENCODER

### 5.1 Wheel Encoder

Encoder mengukur rotasi roda → dihitung jarak tempuh per roda → update posisi robot.

$$\Delta s_L = \frac{2\pi r \cdot \Delta \text{ticks}_L}{\text{CPR}}$$
$$\Delta s_R = \frac{2\pi r \cdot \Delta \text{ticks}_R}{\text{CPR}}$$

### 5.2 Pose Update dari Encoder

$$\Delta s = \frac{\Delta s_R + \Delta s_L}{2}$$
$$\Delta \theta = \frac{\Delta s_R - \Delta s_L}{L}$$
$$x_{k+1} = x_k + \Delta s \cos(\theta_k + \frac{\Delta\theta}{2})$$
$$y_{k+1} = y_k + \Delta s \sin(\theta_k + \frac{\Delta\theta}{2})$$
$$\theta_{k+1} = \theta_k + \Delta\theta$$

### 5.3 Implementasi ROS 2 Odometry Publisher

```python
from nav_msgs.msg import Odometry
from geometry_msgs.msg import TransformStamped
from tf2_ros import TransformBroadcaster

class OdomPublisher(Node):
    def __init__(self):
        super().__init__('odom_publisher')
        self.odom_pub = self.create_publisher(Odometry, '/odom', 10)
        self.tf_broadcaster = TransformBroadcaster(self)
        self.x, self.y, self.theta = 0.0, 0.0, 0.0
    
    def publish_odom(self, v, omega, dt):
        self.x += v * math.cos(self.theta) * dt
        self.y += v * math.sin(self.theta) * dt
        self.theta += omega * dt
        
        odom = Odometry()
        odom.header.stamp = self.get_clock().now().to_msg()
        odom.header.frame_id = 'odom'
        odom.child_frame_id = 'base_link'
        odom.pose.pose.position.x = self.x
        odom.pose.pose.position.y = self.y
        # Quaternion from theta
        odom.pose.pose.orientation.z = math.sin(self.theta / 2.0)
        odom.pose.pose.orientation.w = math.cos(self.theta / 2.0)
        odom.twist.twist.linear.x = v
        odom.twist.twist.angular.z = omega
        self.odom_pub.publish(odom)
```

---

## 6. DENAVIT-HARTENBERG (DH) PARAMETERS

### 6.1 Konsep DH

DH Convention digunakan untuk mendeskripsikan kinematika serial chain (robot lengan).

| Parameter | Simbol | Deskripsi |
|-----------|--------|-----------|
| Link length | $a_i$ | Jarak antar sumbu Z (sepanjang X) |
| Link twist | $\alpha_i$ | Sudut antara sumbu Z (rotasi X) |
| Link offset | $d_i$ | Jarak sepanjang sumbu Z |
| Joint angle | $\theta_i$ | Rotasi di sekitar sumbu Z |

### 6.2 Transformation Matrix

$$T_i = \begin{bmatrix} 
\cos\theta_i & -\sin\theta_i\cos\alpha_i & \sin\theta_i\sin\alpha_i & a_i\cos\theta_i \\
\sin\theta_i & \cos\theta_i\cos\alpha_i & -\cos\theta_i\sin\alpha_i & a_i\sin\theta_i \\
0 & \sin\alpha_i & \cos\alpha_i & d_i \\
0 & 0 & 0 & 1
\end{bmatrix}$$

### 6.3 Contoh: Robot 2-DOF Planar

| Joint | $\theta$ | $d$ | $a$ | $\alpha$ |
|-------|---------|-----|-----|----------|
| 1 | $\theta_1$ | 0 | $L_1$ | 0 |
| 2 | $\theta_2$ | 0 | $L_2$ | 0 |

$$x = L_1\cos\theta_1 + L_2\cos(\theta_1+\theta_2)$$
$$y = L_1\sin\theta_1 + L_2\sin(\theta_1+\theta_2)$$

---

## 7. KONTROLER PID DI ROS 2

### 7.1 PID untuk Velocity Control

```python
class PIDController:
    def __init__(self, Kp, Ki, Kd, output_limit=1.0):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.limit = output_limit
        self.integral = 0.0
        self.prev_error = 0.0
    
    def compute(self, error, dt):
        self.integral += error * dt
        self.integral = max(-self.limit, min(self.limit, self.integral))
        derivative = (error - self.prev_error) / dt if dt > 0 else 0.0
        output = self.Kp * error + self.Ki * self.integral + self.Kd * derivative
        self.prev_error = error
        return max(-self.limit, min(self.limit, output))
```

---

## 8. ROS2_CONTROL FRAMEWORK

### 8.1 Arsitektur

```
┌─────────────────────┐
│  Controller Manager │
│  ┌───────────────┐  │
│  │ Diff Drive    │  │
│  │ Controller    │  │  cmd_vel → wheel velocities
│  └───────────────┘  │
│          ↕           │
│  ┌───────────────┐  │
│  │ Hardware      │  │  wheel velocities ↔ Gazebo / Real HW
│  │ Interface     │  │
│  └───────────────┘  │
└─────────────────────┘
```

### 8.2 Konfigurasi Controller

```yaml
# config/controllers.yaml
controller_manager:
  ros__parameters:
    update_rate: 50
    
    diff_drive_controller:
      type: diff_drive_controller/DiffDriveController

    joint_state_broadcaster:
      type: joint_state_broadcaster/JointStateBroadcaster

diff_drive_controller:
  ros__parameters:
    left_wheel_names: ["left_wheel_joint"]
    right_wheel_names: ["right_wheel_joint"]
    wheel_separation: 0.22
    wheel_radius: 0.04
    publish_rate: 50.0
    odom_frame_id: odom
    base_frame_id: base_link
```

---

## 9. GO-TO-GOAL CONTROLLER

### 9.1 Algoritma

```
1. Hitung jarak ke goal: d = sqrt((gx-rx)² + (gy-ry)²)
2. Hitung sudut ke goal: α = atan2(gy-ry, gx-rx)
3. Hitung heading error: e = α - θ_robot
4. Jika |e| > threshold → putar dulu (v=0, ω=Kp_ang × e)
5. Jika |e| < threshold → maju (v=Kp_lin × d, ω=Kp_ang × e)
6. Jika d < goal_tolerance → STOP
```

### 9.2 Implementasi ROS 2

```python
class GoToGoal(Node):
    def __init__(self):
        super().__init__('go_to_goal')
        self.pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.sub = self.create_subscription(Odometry, '/odom', self.odom_cb, 10)
        self.timer = self.create_timer(0.1, self.control_loop)
        
        self.goal_x = 2.0
        self.goal_y = 1.0
        self.x, self.y, self.theta = 0.0, 0.0, 0.0
        self.Kp_lin = 0.5
        self.Kp_ang = 2.0
    
    def odom_cb(self, msg):
        self.x = msg.pose.pose.position.x
        self.y = msg.pose.pose.position.y
        q = msg.pose.pose.orientation
        self.theta = math.atan2(2*(q.w*q.z + q.x*q.y), 
                                1-2*(q.y**2+q.z**2))
    
    def control_loop(self):
        dx = self.goal_x - self.x
        dy = self.goal_y - self.y
        dist = math.sqrt(dx**2 + dy**2)
        angle = math.atan2(dy, dx)
        error_ang = math.atan2(math.sin(angle-self.theta), 
                               math.cos(angle-self.theta))
        
        msg = Twist()
        if dist < 0.05:
            msg.linear.x = 0.0
            msg.angular.z = 0.0
            self.get_logger().info('Goal reached!')
        elif abs(error_ang) > 0.3:
            msg.linear.x = 0.0
            msg.angular.z = self.Kp_ang * error_ang
        else:
            msg.linear.x = min(self.Kp_lin * dist, 0.5)
            msg.angular.z = self.Kp_ang * error_ang
        
        self.pub.publish(msg)
```

---

## 10. TRAJECTORY FOLLOWING

### 10.1 Waypoint Following

```python
waypoints = [(1.0, 0.0), (2.0, 1.0), (1.0, 2.0), (0.0, 1.0)]
current_wp = 0

def control_loop(self):
    if current_wp >= len(waypoints):
        self.stop(); return
    
    gx, gy = waypoints[current_wp]
    dist = math.sqrt((gx-self.x)**2 + (gy-self.y)**2)
    
    if dist < 0.1:
        current_wp += 1  # next waypoint
        return
    
    # Go-to-goal untuk waypoint saat ini
    self.go_to(gx, gy)
```

### 10.2 Pure Pursuit

Pure Pursuit adalah algoritma path following yang memilih **lookahead point** di depan robot pada path, lalu steering ke titik tersebut.

$$\kappa = \frac{2 \sin(\alpha)}{L_d}$$

Dimana $\alpha$ = sudut antara heading robot dan lookahead point, $L_d$ = lookahead distance.

---

## 11. REFERENSI

1. Siegwart, R. et al. (2011). *Introduction to Autonomous Mobile Robots* (2nd ed). MIT Press.
2. Siciliano, B. et al. (2009). *Robotics: Modelling, Planning and Control*. Springer.
3. Corke, P. (2023). *Robotics, Vision and Control* (3rd ed). Springer.
4. ROS 2 Humble Docs: https://docs.ros.org/en/humble/
5. ros2_control: https://control.ros.org/humble/
6. Ang et al. PID Control: https://ieeexplore.ieee.org/abstract/document/4587662

---

Dokumen ini menjadi acuan utama materi Modul 09 ROS 2 Kinematika dan Kontroler.
