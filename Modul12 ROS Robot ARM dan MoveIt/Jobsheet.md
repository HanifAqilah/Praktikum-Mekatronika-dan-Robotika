# JOBSHEET MODUL 12: ROS 2 ROBOT ARM DAN MOVEIT 2

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 12 – ROS Robot ARM dan MoveIt  
**Pertemuan:** 23–24 (2 × 2 SKS)  
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

1. Membuat URDF robot arm sederhana (3-DOF)
2. Menjalankan MoveIt Setup Assistant dan generate config package
3. Melakukan motion planning via RViz2 MoveIt plugin
4. Membuat kode Python untuk FK dan IK menggunakan MoveGroupInterface
5. Mengimplementasikan pick-and-place dasar
6. Menambahkan collision objects ke planning scene
7. Menjalankan robot arm di Gazebo dengan ros2_control

---

## B. ALAT DAN BAHAN

| No | Item | Keterangan |
|----|------|-----------|
| 1 | Laptop/PC Ubuntu 22.04 | RAM ≥ 8 GB |
| 2 | ROS 2 Humble + Gazebo | Dari Modul 08 |
| 3 | MoveIt 2 | `sudo apt install ros-humble-moveit` |
| 4 | MoveIt Setup Assistant | `sudo apt install ros-humble-moveit-setup-assistant` |
| 5 | ros2_control | Dari Modul 09 |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: URDF ROBOT ARM

**Estimasi waktu: 25 menit**

Buat file `my_arm.urdf.xacro`:

```xml
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro" name="my_arm">

  <!-- Base -->
  <link name="base_link">
    <visual>
      <geometry><cylinder length="0.05" radius="0.1"/></geometry>
      <material name="grey"><color rgba="0.5 0.5 0.5 1"/></material>
    </visual>
    <collision><geometry><cylinder length="0.05" radius="0.1"/></geometry></collision>
    <inertial><mass value="1.0"/><inertia ixx="0.001" iyy="0.001" izz="0.001" ixy="0" ixz="0" iyz="0"/></inertial>
  </link>

  <!-- Link 1 (shoulder) -->
  <link name="link_1">
    <visual>
      <geometry><cylinder length="0.3" radius="0.03"/></geometry>
      <origin xyz="0 0 0.15"/>
      <material name="blue"><color rgba="0 0 0.8 1"/></material>
    </visual>
    <collision><geometry><cylinder length="0.3" radius="0.03"/></geometry><origin xyz="0 0 0.15"/></collision>
    <inertial><mass value="0.5"/><inertia ixx="0.001" iyy="0.001" izz="0.001" ixy="0" ixz="0" iyz="0"/></inertial>
  </link>
  
  <joint name="joint_1" type="revolute">
    <parent link="base_link"/><child link="link_1"/>
    <origin xyz="0 0 0.05" rpy="0 0 0"/>
    <axis xyz="0 0 1"/>
    <limit lower="-3.14" upper="3.14" effort="10" velocity="1.0"/>
  </joint>

  <!-- Link 2 (elbow) -->
  <link name="link_2">
    <visual>
      <geometry><cylinder length="0.25" radius="0.025"/></geometry>
      <origin xyz="0 0 0.125"/>
      <material name="green"><color rgba="0 0.8 0 1"/></material>
    </visual>
    <collision><geometry><cylinder length="0.25" radius="0.025"/></geometry><origin xyz="0 0 0.125"/></collision>
    <inertial><mass value="0.3"/><inertia ixx="0.001" iyy="0.001" izz="0.001" ixy="0" ixz="0" iyz="0"/></inertial>
  </link>
  
  <joint name="joint_2" type="revolute">
    <parent link="link_1"/><child link="link_2"/>
    <origin xyz="0 0 0.3" rpy="0 0 0"/>
    <axis xyz="0 1 0"/>
    <limit lower="-1.57" upper="1.57" effort="10" velocity="1.0"/>
  </joint>

  <!-- Link 3 (wrist) -->
  <link name="link_3">
    <visual>
      <geometry><cylinder length="0.15" radius="0.02"/></geometry>
      <origin xyz="0 0 0.075"/>
      <material name="red"><color rgba="0.8 0 0 1"/></material>
    </visual>
    <collision><geometry><cylinder length="0.15" radius="0.02"/></geometry><origin xyz="0 0 0.075"/></collision>
    <inertial><mass value="0.2"/><inertia ixx="0.001" iyy="0.001" izz="0.001" ixy="0" ixz="0" iyz="0"/></inertial>
  </link>
  
  <joint name="joint_3" type="revolute">
    <parent link="link_2"/><child link="link_3"/>
    <origin xyz="0 0 0.25" rpy="0 0 0"/>
    <axis xyz="0 1 0"/>
    <limit lower="-1.57" upper="1.57" effort="10" velocity="1.0"/>
  </joint>

  <!-- End Effector (dummy link) -->
  <link name="end_effector"/>
  <joint name="ee_fixed" type="fixed">
    <parent link="link_3"/><child link="end_effector"/>
    <origin xyz="0 0 0.15"/>
  </joint>

</robot>
```

Visualisasi:
```bash
ros2 launch urdf_tutorial display.launch.py model:=my_arm.urdf.xacro
# Joint State Publisher GUI → gerakkan slider
```

**Checkpoint ✅:** Robot arm terlihat di RViz2, joint bisa digerakkan.

---

### PERCOBAAN 2: MOVEIT SETUP ASSISTANT

**Estimasi waktu: 25 menit**

```bash
ros2 launch moveit_setup_assistant setup_assistant.launch.py
```

Langkah-langkah:
1. **Start** → Create New Config → Load URDF `my_arm.urdf.xacro`
2. **Self-Collision** → Generate Collision Matrix
3. **Planning Groups** → Add Group:
   - Name: `arm`
   - Solver: `KDLKinematicsPlugin`
   - Chain: base_link → end_effector
4. **Robot Poses** → Add:
   - `home`: semua joint = 0
   - `ready`: joint_2 = -0.5, joint_3 = 0.5
5. **End Effector** → Skip (opsional)
6. **ros2_control** → Auto-generate controllers
7. **Generate Package** → `my_arm_moveit_config`

**Checkpoint ✅:** Config package ter-generate.

---

### PERCOBAAN 3: MOTION PLANNING VIA RVIZ2

**Estimasi waktu: 20 menit**

```bash
ros2 launch my_arm_moveit_config demo.launch.py
```

Di RViz2:
1. Panel **MotionPlanning** otomatis muncul
2. Tab **Planning** → Drag goal state (orange arm)
3. Klik **Plan** → lihat trajectory preview
4. Klik **Execute** → arm bergerak ke goal
5. Coba dengan named targets: `home`, `ready`

**Dokumentasikan 3 motion plan:**

| No | Start → Goal | Planning Time | Path Length |
|----|-------------|--------------|-------------|
| 1 | home → ready | | |
| 2 | ready → custom | | |
| 3 | custom → home | | |

**Checkpoint ✅:** Motion planning berhasil di RViz2.

---

### PERCOBAAN 4: PYTHON MOVEGROUPINTERFACE

**Estimasi waktu: 30 menit**

Buat `arm_control.py`:

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from moveit.planning import MoveItPy
import numpy as np

def main():
    rclpy.init()
    
    moveit = MoveItPy(node_name="arm_control")
    arm = moveit.get_planning_component("arm")
    
    # Go to named target
    arm.set_start_state_to_current_state()
    arm.set_goal_state(configuration_name="home")
    plan = arm.plan()
    if plan:
        robot = moveit.get_robot_model()
        arm.execute()
        print("Moved to HOME")
    
    # Go to joint values
    arm.set_start_state_to_current_state()
    arm.set_goal_state(
        configuration_name="ready"
    )
    plan = arm.plan()
    if plan:
        arm.execute()
        print("Moved to READY")
    
    # Go to pose target (IK)
    from geometry_msgs.msg import PoseStamped
    target = PoseStamped()
    target.header.frame_id = "base_link"
    target.pose.position.x = 0.2
    target.pose.position.y = 0.0
    target.pose.position.z = 0.4
    target.pose.orientation.w = 1.0
    
    arm.set_start_state_to_current_state()
    arm.set_goal_state(pose_stamped_msg=target, pose_link="end_effector")
    plan = arm.plan()
    if plan:
        arm.execute()
        print("Moved to Cartesian pose (0.2, 0, 0.4)")
    
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

**Checkpoint ✅:** Arm bergerak ke 3 target via Python.

---

### PERCOBAAN 5: COLLISION OBJECTS

**Estimasi waktu: 20 menit**

Buat `add_collision.py`:

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from moveit_msgs.msg import CollisionObject, PlanningScene
from shape_msgs.msg import SolidPrimitive
from geometry_msgs.msg import Pose

class CollisionAdder(Node):
    def __init__(self):
        super().__init__('collision_adder')
        self.pub = self.create_publisher(
            PlanningScene, '/planning_scene', 10)
        self.timer = self.create_timer(1.0, self.add_table)
    
    def add_table(self):
        co = CollisionObject()
        co.header.frame_id = 'base_link'
        co.id = 'table'
        co.operation = CollisionObject.ADD
        
        box = SolidPrimitive()
        box.type = SolidPrimitive.BOX
        box.dimensions = [0.6, 0.6, 0.02]
        
        pose = Pose()
        pose.position.x = 0.3
        pose.position.z = 0.1
        
        co.primitives = [box]
        co.primitive_poses = [pose]
        
        ps = PlanningScene()
        ps.is_diff = True
        ps.world.collision_objects = [co]
        
        self.pub.publish(ps)
        self.get_logger().info('Table added!')
        self.timer.cancel()

def main():
    rclpy.init()
    node = CollisionAdder()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

Jalankan, lalu coba plan ke pose di belakang meja — robot arm harus menghindari meja.

**Checkpoint ✅:** Collision object terlihat di RViz2, path planning menghindarinya.

---

### PERCOBAAN 6: PICK-AND-PLACE

**Estimasi waktu: 30 menit**

Buat `pick_place.py`:

```python
#!/usr/bin/env python3
"""
Simplified pick-and-place sequence using MoveGroupInterface.
"""
import rclpy
from rclpy.node import Node
from moveit.planning import MoveItPy
from geometry_msgs.msg import PoseStamped
import time

def create_pose(frame, x, y, z, w=1.0):
    p = PoseStamped()
    p.header.frame_id = frame
    p.pose.position.x = x
    p.pose.position.y = y
    p.pose.position.z = z
    p.pose.orientation.w = w
    return p

def main():
    rclpy.init()
    moveit = MoveItPy(node_name="pick_place")
    arm = moveit.get_planning_component("arm")
    
    # Step 1: Move to pre-grasp
    print("Step 1: Pre-grasp")
    arm.set_start_state_to_current_state()
    pre_grasp = create_pose("base_link", 0.25, 0.0, 0.35)
    arm.set_goal_state(pose_stamped_msg=pre_grasp, pose_link="end_effector")
    plan = arm.plan()
    if plan:
        arm.execute()
    time.sleep(1)
    
    # Step 2: Move down to grasp
    print("Step 2: Grasp")
    arm.set_start_state_to_current_state()
    grasp = create_pose("base_link", 0.25, 0.0, 0.2)
    arm.set_goal_state(pose_stamped_msg=grasp, pose_link="end_effector")
    plan = arm.plan()
    if plan:
        arm.execute()
    time.sleep(1)
    print("  → Close gripper (simulated)")
    
    # Step 3: Lift
    print("Step 3: Lift")
    arm.set_start_state_to_current_state()
    lift = create_pose("base_link", 0.25, 0.0, 0.4)
    arm.set_goal_state(pose_stamped_msg=lift, pose_link="end_effector")
    plan = arm.plan()
    if plan:
        arm.execute()
    time.sleep(1)
    
    # Step 4: Move to place
    print("Step 4: Place")
    arm.set_start_state_to_current_state()
    place = create_pose("base_link", -0.25, 0.0, 0.2)
    arm.set_goal_state(pose_stamped_msg=place, pose_link="end_effector")
    plan = arm.plan()
    if plan:
        arm.execute()
    print("  → Open gripper (simulated)")
    
    # Step 5: Return home
    print("Step 5: Home")
    arm.set_start_state_to_current_state()
    arm.set_goal_state(configuration_name="home")
    plan = arm.plan()
    if plan:
        arm.execute()
    
    print("Pick-and-Place complete!")
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

**Checkpoint ✅:** Arm melakukan pick-and-place sequence (5 step).

---

### PERCOBAAN 7: ARM DI GAZEBO

**Estimasi waktu: 30 menit**

Buat launch file untuk spawn arm di Gazebo dengan ros2_control:

```bash
# Launch Gazebo + arm + controllers
ros2 launch my_arm_moveit_config gazebo.launch.py

# Di terminal lain, launch MoveIt
ros2 launch my_arm_moveit_config moveit.launch.py
```

Lakukan motion planning dari RViz2 → amati arm bergerak di Gazebo.

**Catat:**

| Aspek | Observasi |
|-------|-----------|
| Trajectory smooth? | |
| Waktu eksekusi vs plan | |
| Collision avoidance bekerja? | |

**Checkpoint ✅:** Arm bergerak di Gazebo sesuai MoveIt plan.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1 – URDF
1. Berapa DOF yang dimiliki arm? Apa workspace-nya?
2. Apa fungsi `<limit>` di setiap joint?

### Analisa Percobaan 2 – MoveIt Setup Assistant
1. Apa fungsi self-collision matrix?
2. Mengapa perlu mendefinisikan planning group?

### Analisa Percobaan 3 – Motion Planning RViz2
1. Apakah setiap plan menghasilkan trajectory yang sama?
2. Apa yang mempengaruhi planning time?

### Analisa Percobaan 4 – Python API
1. Apa perbedaan joint target vs pose target (FK vs IK)?
2. Bagaimana jika IK tidak punya solusi?

### Analisa Percobaan 5 – Collision Objects
1. Bagaimana path berubah setelah collision object ditambahkan?
2. Apa terjadi jika goal berada inside collision object?

### Analisa Percobaan 6 – Pick-and-Place
1. Apa tantangan utama dalam pick-and-place sequence?
2. Bagaimana cara memastikan gripper grip objek dengan benar?

### Analisa Percobaan 7 – Gazebo
1. Apakah ada perbedaan behavior antara RViz2-only dan Gazebo?
2. Mengapa ros2_control diperlukan untuk Gazebo?

---

## E. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] File URDF arm (my_arm.urdf.xacro)
- [ ] Screenshot RViz2 motion planning (3 plan)
- [ ] Screenshot collision object di planning scene
- [ ] Kode Python (arm_control.py, add_collision.py, pick_place.py)
- [ ] Video pick-and-place sequence
- [ ] Screenshot arm di Gazebo

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
