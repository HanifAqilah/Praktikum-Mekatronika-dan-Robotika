# JOBSHEET MODUL 09: ROS 2 KINEMATIKA DAN KONTROLER

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 09 – ROS Kinematika dan Kontroler  
**Pertemuan:** 17–18 (2 × 2 SKS)  
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

1. Menghitung dan mengimplementasikan forward kinematics differential drive
2. Menghitung dan mengimplementasikan inverse kinematics differential drive
3. Mempublish odometry dari data kinematik
4. Mengimplementasikan kontroler PID untuk velocity control
5. Membuat Go-to-Goal controller node
6. Mengimplementasikan waypoint following
7. Memverifikasi kinematika dan kontroler di Gazebo

---

## B. ALAT DAN BAHAN

| No | Item | Keterangan |
|----|------|-----------|
| 1 | Laptop/PC Ubuntu 22.04 | RAM ≥ 8 GB |
| 2 | ROS 2 Humble + Gazebo | Dari Modul 08 |
| 3 | Robot URDF | Dari Modul 08 (diff drive) |
| 4 | Python 3 + numpy | Perhitungan kinematik |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: FORWARD KINEMATICS – PERHITUNGAN MANUAL

**Estimasi waktu: 20 menit**

Parameter robot: $r = 0.04$ m, $L = 0.22$ m

Hitung FK untuk setiap kondisi:

| No | ωL (rad/s) | ωR (rad/s) | v (m/s) | ω (rad/s) | Gerakan |
|----|-----------|-----------|---------|-----------|---------|
| 1 | 5.0 | 5.0 | | | |
| 2 | 5.0 | -5.0 | | | |
| 3 | 3.0 | 7.0 | | | |
| 4 | 0.0 | 5.0 | | | |
| 5 | 7.0 | 3.0 | | | |

Rumus:
$$v = \frac{r(\omega_R + \omega_L)}{2}, \quad \omega = \frac{r(\omega_R - \omega_L)}{L}$$

**Checkpoint ✅:** 5 baris perhitungan manual selesai.

---

### PERCOBAAN 2: FORWARD KINEMATICS – IMPLEMENTASI PYTHON + ROS 2

**Estimasi waktu: 30 menit**

Buat node `fk_demo.py`:

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import math

class FKDemo(Node):
    def __init__(self):
        super().__init__('fk_demo')
        self.pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.timer = self.create_timer(0.1, self.control)
        self.r = 0.04
        self.L = 0.22
        self.t = 0.0
    
    def fk(self, wL, wR):
        v = self.r * (wR + wL) / 2.0
        w = self.r * (wR - wL) / self.L
        return v, w
    
    def control(self):
        self.t += 0.1
        if self.t < 3:     wL, wR = 5, 5    # lurus
        elif self.t < 5:   wL, wR = 5, -5   # putar
        elif self.t < 8:   wL, wR = 3, 7    # belok kiri
        else:
            self.pub.publish(Twist()); return
        
        v, w = self.fk(wL, wR)
        msg = Twist()
        msg.linear.x = v
        msg.angular.z = w
        self.pub.publish(msg)
        self.get_logger().info(f't={self.t:.1f} wL={wL} wR={wR} v={v:.3f} w={w:.3f}')

def main(args=None):
    rclpy.init(args=args)
    rclpy.spin(FKDemo())
```

Jalankan di Gazebo:
```bash
ros2 launch my_robot_description gazebo.launch.py  # Terminal 1
ros2 run my_robot_pkg fk_demo                       # Terminal 2
```

Amati gerakan robot dan bandingkan dengan perhitungan manual.

**Checkpoint ✅:** Robot bergerak sesuai prediksi FK.

---

### PERCOBAAN 3: INVERSE KINEMATICS

**Estimasi waktu: 20 menit**

Hitung IK untuk setiap perintah cmd_vel:

| No | v (m/s) | ω (rad/s) | ωL (rad/s) | ωR (rad/s) |
|----|---------|-----------|-----------|-----------|
| 1 | 0.2 | 0.0 | | |
| 2 | 0.0 | 1.0 | | |
| 3 | 0.3 | 0.5 | | |
| 4 | 0.5 | -0.3 | | |
| 5 | 0.1 | 2.0 | | |

Rumus:
$$\omega_L = \frac{v - \omega L/2}{r}, \quad \omega_R = \frac{v + \omega L/2}{r}$$

Buat node `ik_verifier.py` yang subscribe ke `/cmd_vel` dan menampilkan ωL, ωR:

```python
class IKVerifier(Node):
    def __init__(self):
        super().__init__('ik_verifier')
        self.sub = self.create_subscription(Twist, '/cmd_vel', self.cb, 10)
        self.r = 0.04
        self.L = 0.22
    
    def cb(self, msg):
        v, w = msg.linear.x, msg.angular.z
        wL = (v - w * self.L / 2) / self.r
        wR = (v + w * self.L / 2) / self.r
        self.get_logger().info(f'v={v:.2f} w={w:.2f} → wL={wL:.2f} wR={wR:.2f}')
```

**Checkpoint ✅:** IK terhitung dan terverifikasi.

---

### PERCOBAAN 4: ODOMETRY PUBLISHER

**Estimasi waktu: 30 menit**

Buat `odom_calculator.py` yang menghitung odometry dari cmd_vel:

```python
import math
from nav_msgs.msg import Odometry
from tf2_ros import TransformBroadcaster
from geometry_msgs.msg import TransformStamped

class OdomCalc(Node):
    def __init__(self):
        super().__init__('odom_calc')
        self.sub = self.create_subscription(Twist, '/cmd_vel', self.vel_cb, 10)
        self.odom_pub = self.create_publisher(Odometry, '/odom_calc', 10)
        self.tf_br = TransformBroadcaster(self)
        self.x, self.y, self.theta = 0.0, 0.0, 0.0
        self.v, self.w = 0.0, 0.0
        self.timer = self.create_timer(0.02, self.update)  # 50Hz
        self.last_time = self.get_clock().now()
    
    def vel_cb(self, msg):
        self.v = msg.linear.x
        self.w = msg.angular.z
    
    def update(self):
        now = self.get_clock().now()
        dt = (now - self.last_time).nanoseconds / 1e9
        self.last_time = now
        
        self.x += self.v * math.cos(self.theta) * dt
        self.y += self.v * math.sin(self.theta) * dt
        self.theta += self.w * dt
        
        odom = Odometry()
        odom.header.stamp = now.to_msg()
        odom.header.frame_id = 'odom'
        odom.child_frame_id = 'base_link'
        odom.pose.pose.position.x = self.x
        odom.pose.pose.position.y = self.y
        odom.pose.pose.orientation.z = math.sin(self.theta / 2)
        odom.pose.pose.orientation.w = math.cos(self.theta / 2)
        odom.twist.twist.linear.x = self.v
        odom.twist.twist.angular.z = self.w
        self.odom_pub.publish(odom)
        
        self.get_logger().info(
            f'x={self.x:.3f} y={self.y:.3f} θ={math.degrees(self.theta):.1f}°',
            throttle_duration_sec=1.0)
```

Bandingkan `/odom_calc` dengan `/odom` dari Gazebo di RViz2.

**Checkpoint ✅:** Odometry terhitung dan mendekati data Gazebo.

---

### PERCOBAAN 5: PID VELOCITY CONTROLLER

**Estimasi waktu: 30 menit**

Buat node yang mengontrol kecepatan robot agar tepat sesuai setpoint:

```python
class PIDController:
    def __init__(self, Kp, Ki, Kd, limit=1.0):
        self.Kp, self.Ki, self.Kd = Kp, Ki, Kd
        self.limit = limit
        self.integral = 0.0
        self.prev_error = 0.0
    
    def compute(self, error, dt):
        self.integral += error * dt
        self.integral = max(-self.limit, min(self.limit, self.integral))
        deriv = (error - self.prev_error) / dt if dt > 0 else 0
        out = self.Kp * error + self.Ki * self.integral + self.Kd * deriv
        self.prev_error = error
        return max(-self.limit, min(self.limit, out))

class VelocityController(Node):
    def __init__(self):
        super().__init__('vel_controller')
        self.pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.sub = self.create_subscription(Odometry, '/odom', self.odom_cb, 10)
        self.timer = self.create_timer(0.05, self.control)
        
        self.target_v = 0.3  # m/s
        self.current_v = 0.0
        self.pid = PIDController(2.0, 0.5, 0.1)
        self.last_time = self.get_clock().now()
    
    def odom_cb(self, msg):
        self.current_v = msg.twist.twist.linear.x
    
    def control(self):
        now = self.get_clock().now()
        dt = (now - self.last_time).nanoseconds / 1e9
        self.last_time = now
        
        error = self.target_v - self.current_v
        output = self.pid.compute(error, dt)
        
        msg = Twist()
        msg.linear.x = output
        self.pub.publish(msg)
```

**Checkpoint ✅:** Kecepatan robot konvergen ke setpoint.

---

### PERCOBAAN 6: GO-TO-GOAL CONTROLLER

**Estimasi waktu: 35 menit**

```python
class GoToGoal(Node):
    def __init__(self):
        super().__init__('go_to_goal')
        self.pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.sub = self.create_subscription(Odometry, '/odom', self.odom_cb, 10)
        self.timer = self.create_timer(0.1, self.control)
        
        self.declare_parameter('goal_x', 2.0)
        self.declare_parameter('goal_y', 1.0)
        self.gx = self.get_parameter('goal_x').value
        self.gy = self.get_parameter('goal_y').value
        
        self.x, self.y, self.theta = 0.0, 0.0, 0.0
        self.Kp_lin, self.Kp_ang = 0.5, 2.0
    
    def odom_cb(self, msg):
        self.x = msg.pose.pose.position.x
        self.y = msg.pose.pose.position.y
        q = msg.pose.pose.orientation
        self.theta = math.atan2(2*(q.w*q.z), 1-2*q.z**2)
    
    def control(self):
        dx, dy = self.gx - self.x, self.gy - self.y
        dist = math.sqrt(dx**2 + dy**2)
        angle = math.atan2(dy, dx)
        err = math.atan2(math.sin(angle-self.theta), math.cos(angle-self.theta))
        
        msg = Twist()
        if dist < 0.05:
            self.get_logger().info('Goal reached!', once=True)
        elif abs(err) > 0.3:
            msg.angular.z = self.Kp_ang * err
        else:
            msg.linear.x = min(self.Kp_lin * dist, 0.5)
            msg.angular.z = self.Kp_ang * err
        self.pub.publish(msg)
```

Test:
```bash
ros2 run my_robot_pkg go_to_goal --ros-args -p goal_x:=3.0 -p goal_y:=2.0
```

**Checkpoint ✅:** Robot bergerak ke goal (3, 2) secara otonom.

---

### PERCOBAAN 7: WAYPOINT FOLLOWING

**Estimasi waktu: 30 menit**

```python
class WaypointFollower(Node):
    def __init__(self):
        super().__init__('waypoint_follower')
        self.pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.sub = self.create_subscription(Odometry, '/odom', self.odom_cb, 10)
        self.timer = self.create_timer(0.1, self.control)
        
        self.waypoints = [
            (1.0, 0.0), (2.0, 0.0), (2.0, 1.0),
            (1.0, 1.0), (0.0, 0.0)
        ]
        self.wp_idx = 0
        self.x, self.y, self.theta = 0.0, 0.0, 0.0
    
    def odom_cb(self, msg):
        self.x = msg.pose.pose.position.x
        self.y = msg.pose.pose.position.y
        q = msg.pose.pose.orientation
        self.theta = math.atan2(2*(q.w*q.z), 1-2*q.z**2)
    
    def control(self):
        if self.wp_idx >= len(self.waypoints):
            self.pub.publish(Twist())
            self.get_logger().info('All waypoints done!', once=True)
            return
        
        gx, gy = self.waypoints[self.wp_idx]
        dx, dy = gx - self.x, gy - self.y
        dist = math.sqrt(dx**2 + dy**2)
        
        if dist < 0.1:
            self.get_logger().info(f'Waypoint {self.wp_idx} reached')
            self.wp_idx += 1
            return
        
        angle = math.atan2(dy, dx)
        err = math.atan2(math.sin(angle-self.theta), math.cos(angle-self.theta))
        
        msg = Twist()
        if abs(err) > 0.3:
            msg.angular.z = 2.0 * err
        else:
            msg.linear.x = min(0.5 * dist, 0.4)
            msg.angular.z = 2.0 * err
        self.pub.publish(msg)
```

**Checkpoint ✅:** Robot mengunjungi semua 5 waypoint secara berurutan.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1-2 – Forward Kinematics
1. Apakah hasil FK manual sesuai dengan gerakan robot di Gazebo?
2. Apa yang terjadi jika ωL = ωR? Jika ωL = -ωR?

### Analisa Percobaan 3 – Inverse Kinematics
1. Pada v dan ω berapa, salah satu roda harus berputar terbalik?
2. Apa batas kecepatan linear yang bisa dicapai jika ω ≠ 0?

### Analisa Percobaan 4 – Odometry
1. Setelah robot jalan 10 detik, seberapa besar selisih odometry kalkulasi vs Gazebo?
2. Mengapa odometry drift terjadi seiring waktu?

### Analisa Percobaan 5 – PID Velocity
1. Berapa lama settling time PID untuk mencapai v = 0.3 m/s?
2. Apakah ada overshoot? Bagaimana mengatasinya?

### Analisa Percobaan 6-7 – Navigation
1. Apakah robot selalu mengambil jalur terpendek ke goal?
2. Apa yang terjadi jika ada rintangan antara robot dan goal?

---

## E. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] Tabel perhitungan FK dan IK manual
- [ ] Screenshot odometry di RViz2
- [ ] Video robot go-to-goal
- [ ] Video waypoint following
- [ ] Kode program semua node

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
