# MODUL 10: ROS2 FUNDAMENTAL UNTUK ROBOT NYATA

## 1. DEFINISI

- **ROS2**: middleware framework untuk robotika (pub/sub, service, action).
- **Node**: proses independen yang menjalankan fungsi tertentu.
- **Topic**: channel komunikasi asynchronous.
- **Service**: request-response synchronous.
- **Action**: task jangka panjang dengan feedback.

---

## 2. KONSEP INTI

### 2.1 Node, Topic, Service, Action
- **Publisher** mengirim message ke topic.
- **Subscriber** menerima message dari topic.
- **Service** cocok untuk command yang perlu respons instan.
- **Action** untuk proses lama (motion planning, navigation).

### 2.2 Message Type
Contoh: `sensor_msgs/Imu`, `geometry_msgs/Twist`, `std_msgs/String`.

### 2.3 QoS (Quality of Service)
- **Best Effort**: latency rendah, bisa loss.
- **Reliable**: delivery dijamin.

### 2.4 URDF & TF
- **URDF**: deskripsi robot (link, joint, inertia).
- **TF**: transformasi antar frame (base → link1 → link2).

### 2.5 Launch & Parameter
- **Launch file**: menjalankan multi-node.
- **Parameter**: konfigurasi runtime.

### 2.6 Tooling
- `ros2 topic list/echo`
- `rviz2`
- `rqt`
- `rosbag`

---

## 3. ALUR IMPLEMENTASI
1. Buat workspace dan package.
2. Buat publisher/subscriber.
3. Buat service untuk command motor.
4. Buat URDF robot.
5. Publish TF.
6. Visualisasi di RViz.

---

## 4. RINGKASAN
- ROS2 menyediakan arsitektur modular untuk robot real.
- Pub/sub cocok untuk streaming data sensor.
- Service/action untuk command dan task kompleks.
- URDF + TF wajib untuk visualisasi dan perencanaan.
