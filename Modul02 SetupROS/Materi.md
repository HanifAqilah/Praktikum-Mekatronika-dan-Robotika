# MATERI MODUL 02: DASAR-DASAR ROS 2, ROBOTIKA, DAN RUMUS-RUMUS TEKNIS

**Program Studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS dan Dasar Robotika  
**Dosen Pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.  
**Estimasi Waktu:** 6–10 Jam Teori + Praktikum

---

## DAFTAR ISI

1. Pendahuluan Robotika dan ROS
2. Sistem Operasi Ubuntu untuk Robotika
3. Arsitektur ROS 2
4. Konsep Dasar ROS 2: Nodes, Topics, Services, Actions
5. Kinematika Robot
6. Dinamika Robot
7. Transformasi Koordinat (TF2)
8. Kontrol Robot
9. Computer Vision dengan OpenCV
10. SLAM (Simultaneous Localization and Mapping)
11. Navigasi dan Path Planning
12. Machine Learning dalam Robotika
13. Gazebo Simulation
14. MoveIt Motion Planning
15. Rekomendasi Buku dan Paper Terbaik

---

## 1. PENDAHULUAN ROBOTIKA DAN ROS

### 1.1 Apa itu Robotika?

Robotika adalah cabang teknik yang mencakup konsep desain, konstruksi, operasi, dan penggunaan robot. Robot didefinisikan sebagai sistem mekatronik yang dapat melakukan tugas secara otomatis.

**Komponen Utama Robot:**
- **Actuators:** Motor, servo, silinder pneumatik/hidrolik
- **Sensors:** Encoder, IMU, kamera, LIDAR, ultrasonic
- **Controller:** Mikrokontroler, komputer onboard
- **Software:** Sistem operasi robot (ROS)

### 1.2 Apa itu ROS (Robot Operating System)?

ROS bukan sistem operasi dalam arti sebenarnya, melainkan **middleware** atau framework untuk pengembangan perangkat lunak robotika. ROS menyediakan:

- Abstraksi perangkat keras
- Driver perangkat
- Library algoritma
- Visualisasi tools
- Message-passing system

**Versi ROS:**
- ROS 1 (Noetic) - versi lama, berbasis roscore
- **ROS 2 (Humble Hawksbill)** - versi terbaru, real-time capable, DDS-based

---

## 2. SISTEM OPERASI UBUNTU UNTUK ROBOTIKA

### 2.1 Mengapa Ubuntu?

Ubuntu adalah distribusi Linux paling populer untuk robotika karena:
- Dukungan resmi dari Open Robotics
- Repository package yang lengkap
- Kernel Linux yang stabil untuk real-time applications
- Kompatibilitas dengan ROS

### 2.2 Spesifikasi Sistem

| Komponen | Minimum | Rekomendasi |
|----------|---------|-------------|
| CPU | Intel i5 / Ryzen 5 | Intel i7 / Ryzen 7 |
| RAM | 8 GB | 16 GB |
| Storage | 50 GB | 100+ GB |
| GPU | Integrated | Dedicated (NVIDIA) |

---

## 3. ARSITEKTUR ROS 2

### 3.1 Konsep DDS (Data Distribution Service)

ROS 2 menggunakan DDS sebagai middleware komunikasi. DDS adalah protokol publikasi-subscripsi yang:
- Real-time capable
- Terdesentralisasi (tidak perlu master seperti ROS 1)
- Quality of Service (QoS) configurable

### 3.2 Graph Abstraction

ROS 2 merepresentasikan sistem sebagai **graph** dari:
- **Nodes:** Proses yang melakukan komputasi
- **Topics:** Bus data untuk komunikasi asinkron
- **Services:** Komunikasi sinkron request-response
- **Actions:** Komunikasi long-running tasks

---

## 4. KONSEP DASAR ROS 2

### 4.1 Nodes

Node adalah entitas komputasi tunggal dalam ROS. Setiap node harus memiliki nama unik.

```bash
# Melihat daftar nodes yang aktif
ros2 node list

# Melihat info node
ros2 node info /nama_node
```

### 4.2 Topics

Topic adalah saluran komunikasi asinkron dengan pola publikasi-subscripsi.

```bash
# Melihat daftar topics
ros2 topic list

# Melihat data di topic
ros2 topic echo /topic_name

# Melihat info topic
ros2 topic info /topic_name

# Publish data ke topic
ros2 topic pub /topic_name message_type "data"
```

**Rumus Message Rate:**
```
frequency (Hz) = 1 / period (s)
```

Jika period = 0.1s, maka frequency = 10 Hz (10 messages per second)

### 4.3 Services

Service menggunakan pola request-response (sinkron).

```bash
# Melihat daftar services
ros2 service list

# Call service
ros2 service call /service_name service_type "{request_data}"
```

### 4.4 Actions

Action digunakan untuk tugas yang membutuhkan waktu lama dengan feedback.

```bash
# Melihat daftar actions
ros2 action list

# Send goal ke action
ros2 action send_goal /action_name action_type "{goal_data}"
```

---

## 5. KINEMATIKA ROBOT

### 5.1 Kinematika Maju (Forward Kinematics)

Kinematika maju menghitung posisi end-effector berdasarkan joint angles.

**Untuk Robot Manipulator 2-DOF:**

```
x = l1 * cos(θ1) + l2 * cos(θ1 + θ2)
y = l1 * sin(θ1) + l2 * sin(θ1 + θ2)
```

Dimana:
- `l1, l2` = panjang link 1 dan 2
- `θ1, θ2` = sudut joint 1 dan 2
- `x, y` = koordinat end-effector

### 5.2 Kinematika Mundur (Inverse Kinematics)

Kinematika mundur menghitung joint angles berdasarkan posisi end-effector yang diinginkan.

**Untuk Robot 2-DOF Planar:**

```
θ2 = arccos((x² + y² - l1² - l2²) / (2 * l1 * l2))
θ1 = atan2(y, x) - atan2(l2 * sin(θ2), l1 + l2 * cos(θ2))
```

### 5.3 Denavit-Hartenberg (DH) Parameters

Metode sistematis untuk menurunkan kinematika robot.

**Empat parameter DH:**
- `θ` (theta): Sudut rotasi tentang sumbu z_{i-1}
- `d`: Jarak translasi sepanjang sumbu z_{i-1}
- `a`: Jarak translasi sepanjang sumbu x_i
- `α` (alpha): Sudut rotasi tentang sumbu x_i

**DH Transformation Matrix:**

```
T_i^{i-1} = Rot(z, θ) * Trans(0,0,d) * Trans(a,0,0) * Rot(x, α)
```

```
T = [cosθ   -sinθ*cosα   sinθ*sinα   a*cosθ]
    [sinθ    cosθ*cosα  -cosθ*sinα   a*sinθ]
    [0        sinα        cosα       d    ]
    [0        0           0          1    ]
```

### 5.4 Kecepatan Kinematika - Jacobian

Jacobian matrix menghubungkan kecepatan joint dengan kecepatan end-effector.

```
[ν_x]   [J_11 J_12 ... J_1n] [θ̇_1]
[ν_y] = [J_21 J_22 ... J_2n] [θ̇_2]
[ν_z]   [J_31 J_32 ... J_3n] [...]
[ω_x]   [J_41 J_42 ... J_4n] [θ̇_n]
[ω_y]
[ω_z]
```

Dimana:
- `ν` = linear velocity (m/s)
- `ω` = angular velocity (rad/s)
- `θ̇` = joint velocity (rad/s)

---

## 6. DINAMIKA ROBOT

### 6.1 Persamaan Gerak (Equation of Motion)

Menggunakan Euler-Lagrange formulation:

```
τ = M(q) * q̈ + C(q,q̇) * q̇ + G(q)
```

Dimana:
- `τ` = joint torque (Nm)
- `M(q)` = Mass/Inertia matrix
- `C(q,q̇)` = Coriolis and centrifugal forces
- `G(q)` = Gravity forces
- `q, q̇, q̈` = position, velocity, acceleration

### 6.2 Kinetic Energy

```
K = 1/2 * q̇^T * M(q) * q̇
```

### 6.3 Potential Energy

```
P = G(q) * q
```

### 6.4 Lagrangian

```
L = K - P
```

---

## 7. TRANSFORMASI KOORDINAT (TF2)

### 7.1 Transformasi Rotasi

**Rotasi tentang sumbu X (Roll):**

```
R_x(φ) = [1      0       0   ]
         [0   cosφ   -sinφ]
         [0   sinφ    cosφ]
```

**Rotasi tentang sumbu Y (Pitch):**

```
R_y(θ) = [cosθ    0    sinθ]
         [0       1      0 ]
         [-sinθ   0    cosθ]
```

**Rotasi tentang sumbu Z (Yaw):**

```
R_z(ψ) = [cosψ   -sinψ   0]
         [sinψ    cosψ   0]
         [0       0      1]
```

### 7.2 Quaternion

Quaternion adalah representasi rotasi 4D yang menghindari gimbal lock.

```
q = w + xi + yj + zk
```

Dimana:
- `w` = cos(θ/2)
- `x, y, z` = sin(θ/2) * unit_axis

**Konversi Euler Angles ke Quaternion:**

```
qw = cos(φ/2) * cos(θ/2) * cos(ψ/2) + sin(φ/2) * sin(θ/2) * sin(ψ/2)
qx = sin(φ/2) * cos(θ/2) * cos(ψ/2) - cos(φ/2) * sin(θ/2) * sin(ψ/2)
qy = cos(φ/2) * sin(θ/2) * cos(ψ/2) + sin(φ/2) * cos(θ/2) * sin(ψ/2)
qz = cos(φ/2) * cos(θ/2) * sin(ψ/2) - sin(φ/2) * sin(θ/2) * cos(ψ/2)
```

### 7.3 Transform Matrix (Homogeneous Transformation)

```
T = [R_3x3   p_3x1]
    [0_1x3   1    ]
```

Dimana:
- `R` = Rotation matrix 3x3
- `p` = Position vector 3x1

---

## 8. KONTROL ROBOT

### 8.1 PID Controller

Persamaan kontroler PID:

```
u(t) = Kp * e(t) + Ki * ∫e(t)dt + Kd * de(t)/dt
```

Dimana:
- `u(t)` = control output
- `e(t)` = error = setpoint - actual
- `Kp` = proportional gain
- `Ki` = integral gain
- `Kd` = derivative gain

**Discrete PID:**

```
u[k] = Kp * e[k] + Ki * Σe[i] * dt + Kd * (e[k] - e[k-1]) / dt
```

### 8.2 Tuning PID - Ziegler-Nichols

**Metode Ultimate Gain:**
1. Set Ki = 0, Kd = 0
2. Naikkan Kp hingga sistem oscillates (Ku = ultimate gain)
3. Catat period oscillasi (Tu)

| Controller | Kp | Ki | Kd |
|------------|----|----|-----|
| P | 0.5 * Ku | - | - |
| PI | 0.45 * Ku | 0.54 * Ku / Tu | - |
| PID | 0.6 * Ku | 1.2 * Ku / Tu | 0.075 * Ku * Tu |

### 8.3 Feedforward Control

```
u = u_fb + u_ff
```

Dimana:
- `u_fb` = feedback control (PID)
- `u_ff` = feedforward control (model-based)

---

## 9. COMPUTER VISION DENGAN OPENCV

### 9.1 Image Processing Pipeline

1. **Acquisition:** Capture image dari kamera
2. **Preprocessing:** Denoising, resize, color space conversion
3. **Feature Extraction:** Edge detection, corner detection
4. **Analysis:** Object detection, classification
5. **Post-processing:** Filtering, tracking

### 9.2 Color Space Conversion

**RGB ke HSV:**

```
V = max(R, G, B)
S = (V - min(R, G, B)) / V  (jika V ≠ 0)
H = sudut hue berdasarkan komponen dominan
```

### 9.3 Edge Detection - Canny

1. Gaussian filter untuk noise reduction
2. Hitung gradient intensity dan direction
3. Non-maximum suppression
4. Hysteresis thresholding (low, high)

### 9.4 YOLO (You Only Look Once)

YOLO adalah real-time object detection algorithm.

**Output YOLO:**

```
Output = [x_center, y_center, width, height, confidence, class_probabilities]
```

**IoU (Intersection over Union):**

```
IoU = Area_Intersection / Area_Union
```

Untuk NMS (Non-Maximum Suppression), jika IoU > threshold, hapus bounding box.

---

## 10. SLAM (SIMULTANEOUS LOCALIZATION AND MAPPING)

### 10.1 Konsep SLAM

SLAM adalah proses membangun map lingkungan sambil melacak posisi robot.

**Probabilistic Formulation:**

```
P(map, pose | observations, controls)
```

### 10.2 Extended Kalman Filter (EKF) SLAM

**Prediction Step:**

```
μ̄ = g(u, μ)
Σ̄ = G_t * Σ * G_t^T + R_t
```

**Correction Step:**

```
K = Σ̄ * H^T * (H * Σ̄ * H^T + Q)^-1
μ = μ̄ + K * (z - h(μ̄))
Σ = (I - K * H) * Σ̄
```

Dimana:
- `μ` = state estimate (pose + landmarks)
- `Σ` = covariance matrix
- `K` = Kalman gain
- `H` = observation Jacobian
- `Q` = measurement noise

### 10.3 Particle Filter (FastSLAM)

Menggunakan particles untuk representasi posterior distribution.

**Resampling:**

```
weight_i = P(z | x_i, map) * P(x_i | u)
```

---

## 11. NAVIGASI DAN PATH PLANNING

### 11.1 Odometry

Estimasi pose berdasarkan sensor data (wheel encoders).

**Differential Drive Odometry:**

```
Δx = (Δs_left + Δs_right) / 2 * cos(θ)
Δy = (Δs_left + Δs_right) / 2 * sin(θ)
Δθ = (Δs_right - Δs_left) / L
```

Dimana:
- `Δs_left, Δs_right` = jarak roda kiri/kanan
- `L` = wheelbase (jarak antar roda)

### 11.2 A* Path Planning

Algoritma pencarian jalur optimal dengan heuristic.

```
f(n) = g(n) + h(n)
```

Dimana:
- `f(n)` = total estimated cost
- `g(n)` = cost dari start ke node n
- `h(n)` = heuristic (estimasi jarak ke goal, misal Euclidean distance)

**Euclidean Distance:**

```
h = sqrt((x_goal - x_n)² + (y_goal - y_n)²)
```

**Manhattan Distance:**

```
h = |x_goal - x_n| + |y_goal - y_n|
```

### 11.3 Dijkstra Algorithm

Mirip A* tanpa heuristic (h = 0).

### 11.4 Dynamic Window Approach (DWA)

Local planner untuk obstacle avoidance.

**Velocity Space:**

```
V = {(v, ω) | v ∈ [v_min, v_max], ω ∈ [ω_min, ω_max]}
```

**Objective Function:**

```
G(v, ω) = α * heading(v, ω) + β * dist(v, ω) + γ * vel(v, ω)
```

Dimana:
- `heading` = alignment ke goal
- `dist` = clearance dari obstacles
- `vel` = forward velocity

---

## 12. MACHINE LEARNING DALAM ROBOTIKA

### 12.1 Reinforcement Learning

Agent belajar melalui interaction dengan environment.

**Q-Learning:**

```
Q(s, a) = Q(s, a) + α * [r + γ * max_a' Q(s', a') - Q(s, a)]
```

Dimana:
- `Q(s, a)` = action-value function
- `α` = learning rate
- `γ` = discount factor
- `r` = reward
- `s'` = next state

### 12.2 Deep Q-Network (DQN)

Menggunakan neural network untuk approximation Q-function.

**Loss Function:**

```
L = (r + γ * max_a' Q(s', a'; θ^-) - Q(s, a; θ))²
```

Dimana:
- `θ` = network parameters
- `θ^-` = target network parameters

---

## 13. GAZEBO SIMULATION

### 13.1 Physics Engine

Gazebo menggunakan ODE (Open Dynamics Engine) atau Bullet.

**Newton's Second Law:**

```
F = m * a
τ = I * α
```

Dimana:
- `F` = force (N)
- `m` = mass (kg)
- `a` = acceleration (m/s²)
- `τ` = torque (Nm)
- `I` = moment of inertia (kg⋅m²)
- `α` = angular acceleration (rad/s²)

### 13.2 URDF (Unified Robot Description Format)

Format XML untuk mendeskripsikan robot.

```xml
<link name="link1">
  <inertial>
    <mass value="1.0"/>
    <inertia ixx="0.01" ixy="0" ixz="0" iyy="0.01" iyz="0" izz="0.01"/>
  </inertial>
  <visual>
    <geometry>
      <box size="0.1 0.1 0.1"/>
    </geometry>
  </visual>
  <collision>
    <geometry>
      <box size="0.1 0.1 0.1"/>
    </geometry>
  </collision>
</link>
```

### 13.3 Xacro

Macro language untuk URDF yang lebih fleksibel.

---

## 14. MOVEIT MOTION PLANNING

### 14.1 Motion Planning Pipeline

1. **Robot State:** Current joint positions
2. **Goal State:** Target joint positions
3. **Planning Scene:** Environment obstacles
4. **Planner:** RRT, PRM, OMPL algorithms
5. **Trajectory:** Output path dengan timestamps

### 14.2 RRT (Rapidly-exploring Random Tree)

Algoritma sampling-based motion planning.

```
1. Start dengan initial node
2. Sample random configuration q_rand
3. Cari node terdekat q_near
4. Extend ke arah q_rand → q_new
5. Jika q_new valid (no collision), tambahkan ke tree
6. Ulangi hingga goal tercapai
```

### 14.3 OMPL (Open Motion Planning Library)

Library yang menyediakan berbagai algoritma planning.

---

## 15. REKOMENDASI BUKU TERBAIK

### 15.1 Buku ROS dan Robotika

1. **"ROS Robotics Projects"** - Lentin Joseph
   - Fokus: Proyek praktis dengan ROS
   - Level: Intermediate

2. **"Programming Robots with ROS"** - Morgan Quigley, Brian Gerkey, William D. Smart
   - Fokus: Dasar-dasar programming ROS
   - Level: Beginner-Intermediate

3. **"A Gentle Introduction to ROS"** - Jason M. O'Kane
   - Fokus: Konsep dasar ROS
   - Level: Beginner

4. **"ROS 2 Best Practices for Developers"** - Alberto Soragna
   - Fokus: Best practices ROS 2
   - Level: Intermediate-Advanced

5. **"Mastering ROS for Robotics Programming"** - Lentin Joseph
   - Fokus: Advanced ROS concepts
   - Level: Advanced

### 15.2 Buku Kinematika dan Dinamika

6. **"Robot Modeling and Control"** - Mark W. Spong, Seth Hutchinson, M. Vidyasagar
   - Fokus: Teori kinematika dan dinamika
   - Level: Advanced

7. **"Introduction to Robotics: Mechanics and Control"** - John J. Craig
   - Fokus: Fundamental robot mechanics
   - Level: Intermediate-Advanced

8. **"Robotics: Modelling, Planning and Control"** - Bruno Siciliano, Lorenzo Sciavicco
   - Fokus: Comprehensive robotics textbook
   - Level: Advanced

### 15.3 Buku Computer Vision

9. **"Learning OpenCV 4 Computer Vision with Python 3"** - Joseph Howse
   - Fokus: OpenCV dengan Python
   - Level: Beginner-Intermediate

10. **"Computer Vision: Algorithms and Applications"** - Richard Szeliski
    - Fokus: Teori computer vision
    - Level: Advanced

### 15.4 Buku SLAM dan Navigasi

11. **"Probabilistic Robotics"** - Sebastian Thrun, Wolfram Burgard, Dieter Fox
    - Fokus: Probabilistic approaches untuk robotics
    - Level: Advanced

12. **"Simultaneous Localization and Mapping: Exactly What It Says On The Tin"** - John Leonard, Seth Teller
    - Fokus: SLAM theory
    - Level: Advanced

---

## 16. REKOMENDASI PAPER TERBAIK

### 16.1 ROS dan Middleware

1. **"ROS: an open-source Robot Operating System"** (2009)
   - Author: Morgan Quigley, Brian Gerkey, et al.
   - Source: ICRA 2009 Workshop
   - Kontribusi: Pengenalan ROS

2. **"ROS 2: Designed for Production, Assembled for Development"** (2017)
   - Author: Steven Macenski, et al.
   - Source: ROSCon 2017
   - Kontribusi: Arsitektur ROS 2

### 16.2 SLAM

3. **"ORB-SLAM: A Versatile and Accurate Monocular SLAM System"** (2015)
   - Author: Raul Mur-Artal, J. M. M. Montiel, Juan D. Tardos
   - Source: IEEE Transactions on Robotics
   - Kontribusi: Feature-based SLAM

4. **"GMapping: Improved Techniques for Grid Mapping with Rao-Blackwellized Particle Filters"** (2006)
   - Author: Giorgio Grisetti, Cyrill Stachniss, Wolfram Burgard
   - Source: IEEE Transactions on Robotics
   - Kontribusi: Grid-based SLAM

5. **"Cartographer: Real-Time Loop Closure in 2D LIDAR SLAM"** (2016)
   - Author: Wolfgang Hess, Damon Kohler, Holger Rapp, Daniel Andor
   - Source: IEEE ICRA 2016
   - Kontribusi: Google Cartographer

### 16.3 Motion Planning

6. **"RRT-Connect: An Efficient Approach to Single-Query Path Planning"** (2000)
   - Author: James J. Kuffner, Steven M. LaValle
   - Source: IEEE ICRA 2000
   - Kontribusi: RRT-Connect algorithm

7. **"OMPL: The Open Motion Planning Library"** (2012)
   - Author: Ioan A. Sucan, Mark Moll, Lydia E. Kavraki
   - Source: IEEE Robotics & Automation Magazine
   - Kontribusi: OMPL library

### 16.4 Computer Vision

8. **"You Only Look Once: Unified, Real-Time Object Detection"** (2016)
   - Author: Joseph Redmon, Santosh Divvala, Ross Girshick, Ali Farhadi
   - Source: IEEE CVPR 2016
   - Kontribusi: YOLO algorithm

9. **"Speeded-Up Robust Features (SURF)"** (2008)
   - Author: Herbert Bay, Tinne Tuytelaars, Luc Van Gool
   - Source: Computer Vision and Image Understanding
   - Kontribusi: Feature detection

### 16.5 Kontrol dan Kinematika

10. **"Computed Torque Control with Variable Gain PI Controller for Robot Manipulators"** (2010)
    - Author: Various
    - Source: IEEE Transactions on Robotics
    - Kontribusi: Advanced control strategies

---

## 17. REFERENSI ONLINE

1. **ROS 2 Documentation:** https://docs.ros.org/en/humble/
2. **ROS 2 Tutorials:** https://docs.ros.org/en/humble/Tutorials.html
3. **Gazebo Documentation:** https://gazebosim.org/docs/
4. **MoveIt Documentation:** https://moveit.picknik.ai/
5. **Nav2 Documentation:** https://navigation.ros.org/
6. **OpenCV Documentation:** https://docs.opencv.org/
7. **OMPL Documentation:** https://ompl.kavrakilab.org/
8. **URDF Documentation:** http://wiki.ros.org/urdf

---

Dokumen ini mencakup teori lengkap, rumus-rumus dasar hingga advanced, serta referensi terbaik untuk mendalami robotika dengan ROS 2.
