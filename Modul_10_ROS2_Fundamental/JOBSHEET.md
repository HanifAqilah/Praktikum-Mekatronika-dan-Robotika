# JOBSHEET MODUL 10: ROS2 FUNDAMENTAL

## 1. TUJUAN PRAKTIKUM
- Setup ROS2 workspace
- Buat publisher & subscriber
- Buat service server
- Desain URDF robot
- Publish TF
- Visualisasi di RViz
- Launch multi-node
- Teleop control

---

## 2. ALAT & BAHAN
- Ubuntu 22.04 + ROS2 Humble
- Python 3.10+
- Robot arm (Batch 1–3)

---

## 3. LANGKAH KERJA

### PERCOBAAN 1: WORKSPACE & NODE
1. Buat workspace ROS2.
2. Buat package `ament_python`.
3. Buat publisher node IMU.

**Deliverable:** Node publish sukses.

---

### PERCOBAAN 2: SUBSCRIBER & PROCESSING
1. Buat subscriber node.
2. Integrasi data (contoh: IMU → estimasi velocity).

**Deliverable:** Topic output visible.

---

### PERCOBAAN 3: SERVICE
1. Definisikan custom service (MotorCommand).
2. Implementasikan service server.

**Deliverable:** Service call sukses.

---

### PERCOBAAN 4: URDF
1. Buat URDF robot 2-link.
2. Validasi syntax.

**Deliverable:** URDF valid + tree image.

---

### PERCOBAAN 5: TF BROADCAST
1. Publish transform base → link1 → link2.
2. Verifikasi TF tree.

**Deliverable:** TF tree dan RViz.

---

### PERCOBAAN 6: RVIZ
1. Load URDF.
2. Visualisasi TF dan sensor data.

**Deliverable:** Screenshot RViz.

---

### PERCOBAAN 7: LAUNCH FILE
1. Buat launch file untuk multi-node.
2. Jalankan semua node.

**Deliverable:** Launch sukses.

---

### PERCOBAAN 8: TELEOP
1. Buat teleop node (keyboard).
2. Kirim command motor.

**Deliverable:** Robot bergerak via keyboard.

---

## 4. DATA PENGAMATAN
- Daftar topic & service
- Screenshot RViz
- Log node

---

## 5. KESIMPULAN
Tuliskan hasil integrasi ROS2 dengan robot nyata.
