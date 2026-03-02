# MATERI MODUL 12: ROS 2 ROBOT ARM DAN MOVEIT 2

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 12 – ROS Robot ARM dan MoveIt  
**Platform:** Ubuntu 22.04 + ROS 2 Humble + MoveIt 2 + Gazebo  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan Robot Arm](#1-pendahuluan-robot-arm)
2. [Kinematika: Forward Kinematics (FK)](#2-kinematika-forward-kinematics-fk)
3. [Kinematika: Inverse Kinematics (IK)](#3-kinematika-inverse-kinematics-ik)
4. [DH Parameters](#4-dh-parameters)
5. [URDF Robot Arm](#5-urdf-robot-arm)
6. [MoveIt 2 Framework](#6-moveit-2-framework)
7. [MoveIt Setup Assistant](#7-moveit-setup-assistant)
8. [Motion Planning — OMPL](#8-motion-planning--ompl)
9. [Trajectory Execution](#9-trajectory-execution)
10. [Gripper dan Pick-and-Place](#10-gripper-dan-pick-and-place)
11. [Collision Detection & Planning Scene](#11-collision-detection--planning-scene)
12. [Referensi](#12-referensi)

---

## 1. PENDAHULUAN ROBOT ARM

### 1.1 Definisi

Robot arm (manipulator) adalah rantai **serial link** yang dihubungkan oleh **joint** untuk memanipulasi objek di ruang 3D.

### 1.2 Jenis Joint

| Joint Type | DOF | Gerak |
|-----------|-----|-------|
| **Revolute** | 1 | Rotasi (θ) |
| **Prismatic** | 1 | Translasi (d) |
| **Fixed** | 0 | Tidak bergerak |

### 1.3 Workspace

- **Reachable Workspace**: Semua titik yang bisa dicapai end-effector
- **Dexterous Workspace**: Titik yang bisa dicapai dengan orientasi apa saja

### 1.4 Contoh Robot Arm

| Robot | DOF | Aplikasi |
|-------|-----|---------|
| 3-DOF Planar | 3 | Edukasi, sorting |
| 6-DOF General | 6 | Industri, welding |
| 7-DOF Redundant | 7 | Flexible manipulation |
| SCARA | 4 | Assembly, pick-and-place |

---

## 2. KINEMATIKA: FORWARD KINEMATICS (FK)

### 2.1 Definisi

FK menghitung **posisi dan orientasi end-effector** dari **nilai joint** yang diketahui.

$$FK: \mathbf{q} = [\theta_1, \theta_2, ..., \theta_n] \rightarrow \mathbf{T} = \begin{bmatrix} R & p \\ 0 & 1 \end{bmatrix}$$

### 2.2 Transformation Matrix

Setiap joint menghasilkan transformasi:

$$T_0^n = T_0^1 \cdot T_1^2 \cdot T_2^3 \cdot ... \cdot T_{n-1}^n$$

### 2.3 Contoh 2-Link Planar

```
     θ₁          θ₂
  O────────O────────●  (end-effector)
  ←   L₁   → ←  L₂  →
```

$$x = L_1 \cos\theta_1 + L_2 \cos(\theta_1 + \theta_2)$$
$$y = L_1 \sin\theta_1 + L_2 \sin(\theta_1 + \theta_2)$$

---

## 3. KINEMATIKA: INVERSE KINEMATICS (IK)

### 3.1 Definisi

IK menghitung **nilai joint** dari **posisi target** end-effector.

$$IK: \mathbf{T}_{target} \rightarrow \mathbf{q} = [\theta_1, \theta_2, ..., \theta_n]$$

### 3.2 Tantangan IK

| Isu | Penjelasan |
|-----|-----------|
| Multiple solutions | Satu pose bisa dicapai dengan konfigurasi berbeda |
| No solution | Target di luar workspace |
| Singularity | Jacobian rank deficient, robot kehilangan DOF |

### 3.3 Metode IK

| Metode | Kecepatan | Akurasi |
|--------|----------|---------|
| **Analytical** | Sangat cepat | Exact (jika ada) |
| **Numerical (Newton-Raphson)** | Cepat | Approx |
| **KDL** | Medium | Good |
| **TRAC-IK** | Cepat | Better than KDL |

### 3.4 Jacobian

Jacobian menghubungkan kecepatan joint dengan kecepatan end-effector:

$$\dot{\mathbf{x}} = J(\mathbf{q}) \cdot \dot{\mathbf{q}}$$

---

## 4. DH PARAMETERS

### 4.1 Denavit-Hartenberg Convention

Setiap joint dideskripsikan oleh 4 parameter:

| Parameter | Simbol | Deskripsi |
|----------|--------|-----------|
| Link length | $a_i$ | Jarak antara $z_{i-1}$ dan $z_i$ sepanjang $x_i$ |
| Link twist | $\alpha_i$ | Sudut antara $z_{i-1}$ dan $z_i$ di sekitar $x_i$ |
| Link offset | $d_i$ | Jarak sepanjang $z_{i-1}$ |
| Joint angle | $\theta_i$ | Sudut di sekitar $z_{i-1}$ |

### 4.2 Contoh DH Table (3-DOF ARM)

| Joint | $\theta_i$ | $d_i$ | $a_i$ | $\alpha_i$ |
|-------|-----------|-------|-------|------------|
| 1 | $\theta_1$ | $d_1$ | 0 | $\pi/2$ |
| 2 | $\theta_2$ | 0 | $a_2$ | 0 |
| 3 | $\theta_3$ | 0 | $a_3$ | 0 |

---

## 5. URDF ROBOT ARM

### 5.1 Struktur URDF

```xml
<robot name="my_arm">
  <!-- Base Link -->
  <link name="base_link">
    <visual>
      <geometry><cylinder length="0.05" radius="0.1"/></geometry>
    </visual>
  </link>

  <!-- Link 1 -->
  <link name="link_1">
    <visual>
      <geometry><cylinder length="0.3" radius="0.03"/></geometry>
      <origin xyz="0 0 0.15"/>
    </visual>
  </link>
  
  <!-- Joint 1: Revolute -->
  <joint name="joint_1" type="revolute">
    <parent link="base_link"/>
    <child link="link_1"/>
    <origin xyz="0 0 0.05"/>
    <axis xyz="0 0 1"/>
    <limit lower="-3.14" upper="3.14" effort="10" velocity="1.0"/>
  </joint>

  <!-- Link 2 -->
  <link name="link_2">
    <visual>
      <geometry><cylinder length="0.25" radius="0.025"/></geometry>
      <origin xyz="0 0 0.125"/>
    </visual>
  </link>
  
  <!-- Joint 2: Revolute -->
  <joint name="joint_2" type="revolute">
    <parent link="link_1"/>
    <child link="link_2"/>
    <origin xyz="0 0 0.3"/>
    <axis xyz="0 1 0"/>
    <limit lower="-1.57" upper="1.57" effort="10" velocity="1.0"/>
  </joint>

  <!-- Gripper Link -->
  <link name="gripper_link">
    <visual>
      <geometry><box size="0.05 0.1 0.02"/></geometry>
    </visual>
  </link>
  
  <joint name="gripper_joint" type="revolute">
    <parent link="link_2"/>
    <child link="gripper_link"/>
    <origin xyz="0 0 0.25"/>
    <axis xyz="0 1 0"/>
    <limit lower="0" upper="0.04" effort="5" velocity="0.5"/>
  </joint>
</robot>
```

---

## 6. MOVEIT 2 FRAMEWORK

### 6.1 Arsitektur MoveIt 2

```
┌──────────────────────────────────┐
│         MoveIt 2                 │
│  ┌──────────┐  ┌──────────────┐  │
│  │ Move     │  │ Planning     │  │
│  │ Group    │  │ Scene        │  │
│  │ Interface│  │ (collision)  │  │
│  └────┬─────┘  └──────┬───────┘  │
│       │               │          │
│  ┌────▼───────────────▼───────┐  │
│  │    Motion Planning         │  │
│  │    (OMPL / Pilz / Custom)  │  │
│  └────────────┬───────────────┘  │
│               │                  │
│  ┌────────────▼───────────────┐  │
│  │  Trajectory Execution      │  │
│  │  (ros2_control + hw)       │  │
│  └────────────────────────────┘  │
└──────────────────────────────────┘
```

### 6.2 Instalasi MoveIt 2

```bash
sudo apt install ros-humble-moveit
sudo apt install ros-humble-moveit-setup-assistant
```

### 6.3 Komponen Utama

| Komponen | Fungsi |
|---------|--------|
| **MoveGroupInterface** | API utama untuk motion planning |
| **PlanningScene** | Representasi dunia + collision objects |
| **OMPL** | Library motion planning (RRT, PRM, etc.) |
| **IK Solver** | KDL / TRAC-IK |
| **Trajectory Execution** | Kirim trajectory ke controller |

---

## 7. MOVEIT SETUP ASSISTANT

### 7.1 Menjalankan

```bash
ros2 launch moveit_setup_assistant setup_assistant.launch.py
```

### 7.2 Langkah-langkah di GUI

1. **Load URDF** — pilih file .urdf.xacro
2. **Self-Collision** — generate collision matrix
3. **Planning Groups** — definisikan arm group (joint chain)
4. **Robot Poses** — simpan pose default (home, ready)
5. **End Effector** — definisikan gripper group
6. **Controllers** — konfigurasi ros2_control
7. **Generate Package** — output MoveIt config package

---

## 8. MOTION PLANNING — OMPL

### 8.1 OMPL (Open Motion Planning Library)

| Algoritma | Tipe | Deskripsi |
|----------|------|-----------|
| **RRTConnect** | Sampling-based | Bidirectional RRT, cepat |
| **RRT*** | Optimal | RRT + path optimization |
| **PRM** | Multi-query | Buat roadmap, query cepat |
| **EST** | Sampling | Expansive Space Trees |
| **LBKPIECE** | Sampling | Gridded planning |

### 8.2 Planning Request

```python
from moveit_msgs.msg import MotionPlanRequest

# Planning dilakukan oleh MoveGroupInterface:
move_group.set_planning_time(5.0)         # max 5 detik
move_group.set_num_planning_attempts(10)  # coba 10 kali
move_group.set_planner_id("RRTConnectkConfigDefault")
```

---

## 9. TRAJECTORY EXECUTION

### 9.1 Joint Trajectory

```
trajectory_msgs/JointTrajectory:
  joint_names: [joint_1, joint_2, joint_3]
  points:
    - positions: [0.0, 0.0, 0.0]        # start
      time_from_start: {sec: 0}
    - positions: [0.5, -0.3, 0.2]       # mid
      time_from_start: {sec: 1}
    - positions: [1.0, -0.6, 0.5]       # goal
      time_from_start: {sec: 2}
```

### 9.2 ros2_control Integration

```yaml
controller_manager:
  ros__parameters:
    joint_state_broadcaster:
      type: joint_state_broadcaster/JointStateBroadcaster
    arm_controller:
      type: joint_trajectory_controller/JointTrajectoryController

arm_controller:
  ros__parameters:
    joints: [joint_1, joint_2, joint_3]
    command_interfaces: [position]
    state_interfaces: [position, velocity]
```

---

## 10. GRIPPER DAN PICK-AND-PLACE

### 10.1 Gripper Control

```python
# Open gripper
gripper_group.set_named_target("open")
gripper_group.go(wait=True)

# Close gripper
gripper_group.set_named_target("close")
gripper_group.go(wait=True)
```

### 10.2 Pick-and-Place Sequence

```
1. Move to pre-grasp pose  (above object)
2. Open gripper
3. Move down to grasp pose (at object)
4. Close gripper            (attach object)
5. Move up (lift)
6. Move to place pose
7. Open gripper             (detach object)
8. Retreat
```

---

## 11. COLLISION DETECTION & PLANNING SCENE

### 11.1 Menambahkan Collision Object

```python
from moveit_msgs.msg import CollisionObject
from shape_msgs.msg import SolidPrimitive

co = CollisionObject()
co.header.frame_id = "base_link"
co.id = "table"
co.operation = CollisionObject.ADD

box = SolidPrimitive()
box.type = SolidPrimitive.BOX
box.dimensions = [0.5, 0.5, 0.02]  # x, y, z

co.primitives = [box]
co.primitive_poses = [table_pose]

planning_scene_interface.apply_collision_object(co)
```

### 11.2 Attach / Detach Object

```python
# Saat gripper pick objek
move_group.attach_object("block", "gripper_link")

# Saat gripper release
move_group.detach_object("block")
```

---

## 12. REFERENSI

1. Siciliano, B. et al. (2009). *Robotics: Modelling, Planning and Control*. Springer.
2. MoveIt 2 Documentation: https://moveit.picknik.ai/humble
3. OMPL Library: https://ompl.kavrakilab.org
4. ROS 2 Control: https://control.ros.org/humble
5. Craig, J. J. (2005). *Introduction to Robotics*. Pearson.
6. Lynch, K. M. & Park, F. C. (2017). *Modern Robotics*. Cambridge University Press.

---

Dokumen ini menjadi acuan utama materi Modul 12 ROS 2 Robot ARM dan MoveIt 2.
