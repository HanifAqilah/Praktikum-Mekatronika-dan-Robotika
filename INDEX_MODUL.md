# Index Modul — Praktikum Mekatronika dan Robotika

**14 Modul + Capstone Project**

---

## 📌 Modul 1: Arsitektur Sistem Mekatronika & Robotika

**Tujuan:** Memahami arsitektur sistem robotika secara keseluruhan, dari komponen hardware hingga layer software.

**Konsep Utama:**
- Sensor → Controller → Actuator → Feedback loop
- Centralized vs Distributed Control
- Real-time vs non-real-time system
- Safety layer (E-Stop, interlock, fault handling)

**Praktikum:**
- Blok diagram sistem robot
- Mapping sinyal (analog, digital, PWM, bus)
- Failure analysis

**Output:** Desain blok diagram, mapping pin, identification diagram

---

## 🔐 Modul 2: Safety & Commissioning Sistem

**Tujuan:** Implementasi safety system yang reliable dan commissioning prosedur.

**Konsep Utama:**
- E-Stop hardware & software implementation
- Limit switch dan homing procedure
- Interlock logic, fault state management
- Recovery procedure & SOP fault handling

**Praktikum:**
- Implementasi safety system lengkap
- Commissioning checklist
- Testing fault scenarios

**Output:** Safety diagram, code, commissioning report

---

## ⚡ Modul 3: Power & EMC Praktis untuk Robot/IIoT

**Tujuan:** Desain power distribution dan electromagnetic compatibility untuk sistem robotika.

**Konsep Utama:**
- Power tree design (24V/12V/5V/3.3V)
- Grounding & shielding, EMC dasar
- Noise motor ke sensor, filtering
- Layout kabel (power vs signal separation)

**Praktikum:**
- Analisis noise sebelum/sesudah filtering
- Stabilitas kontrol dengan noise
- PCB design considerations

**Output:** Power tree diagram, schematic, noise analysis plot, PCB layout

---

## 📊 Modul 4: Sensor Fusion & Conditioning (ADVANCED)

**Tujuan:** Implementasi sensor fusion untuk estimasi state yang akurat dan robust.

**Konsep Utama:**
- Encoder, IMU (accelerometer + gyroscope), limit switch, proximity sensor
- Filtering: moving average, low-pass filter, complementary filter, Kalman filter
- Sensor redundancy & voting
- Noise vs delay trade-off

**Praktikum:**
- Gabung encoder + IMU untuk estimasi posisi
- Implementasi Kalman filter
- Analisis noise vs delay

**Output:** Filter design, sensor fusion implementation, validation data

---

## 🔧 Modul 5: Motor & Drive System (INDUSTRIAL-STYLE)

**Tujuan:** Memilih dan mengoperasikan motor serta driver untuk berbagai aplikasi.

**Konsep Utama:**
- DC motor, BLDC, Stepper, Servo motor (perbandingan)
- Driver: H-bridge, ESC, stepper driver, servo drive
- PWM, direction control, current limiting
- Closed-loop vs open-loop control

**Praktikum:**
- Kontrol kecepatan & posisi motor
- Pengaruh beban mekanik
- Karakterisasi motor (torque-speed curve)

**Output:** Motor selection chart, driver implementation, characterization report

---

## 🎯 Modul 6: Motion Control di ESP32 (CLOSED-LOOP)

**Tujuan:** Desain dan implementasi PID controller untuk kontrol gerak yang presisi.

**Konsep Utama:**
- PID controller implementation (proportional, integral, derivative)
- Anti-windup, derivative filtering
- Feedforward control (velocity + acceleration)
- Tuning methods (Ziegler-Nichols, manual, auto-tuning)

**Praktikum:**
- Tuning PID nyata di hardware
- Kurva step response
- Analisis settling time & overshoot
- Comparison berbagai tuning method

**Output:** PID code, tuning report, step response plot, performance analysis

---

## ⚙️ Modul 7: Mekanisme Robot & Transmisi

**Tujuan:** Desain mekanisme robot dengan analisis kinematika dasar dan presisi gerak.

**Konsep Utama:**
- Gear (spur, planetary), belt & pulley, lead screw, rack & pinion
- Backlash, compliance, friction
- Workspace, DOF (degree of freedom)
- Desain di Fusion 360: assembly, motion study

**Praktikum:**
- Desain mekanisme (Fusion 360)
- Analisis rasio gear
- Uji presisi gerak, backlash compensation
- Tolerance stack-up analysis

**Output:** CAD design, assembly drawing, tolerance analysis, test report

---

## 🤖 Modul 8: Kinematika Robot (APPLIED)

**Tujuan:** Implementasi forward & inverse kinematics untuk robot arm.

**Konsep Utama:**
- Forward Kinematics (DH parameters, transformation matrix)
- Inverse Kinematics (analytical & numerical solution)
- Joint space vs Cartesian space
- Jacobian matrix (velocity kinematics)
- Singularity analysis

**Praktikum:**
- Hitung FK & IK robot 2-DOF / 3-DOF
- Implementasi di mikrokontroler
- Validasi gerak nyata
- Workspace visualization

**Output:** Kinematics code, IK validation report, workspace plot, demo video

---

## 📈 Modul 9: Trajectory Planning & Motion Control

**Tujuan:** Implementasi trajectory planner untuk gerak robot yang smooth dan efisien.

**Konsep Utama:**
- Point-to-point vs continuous path
- Trapezoidal velocity profile, S-curve profile
- Soft start / soft stop, jerk limitation
- Path interpolation (linear, circular, spline)

**Praktikum:**
- Implementasi trajectory planner
- Gerak robot halus tanpa getaran
- Perbandingan profil (trapezoid vs S-curve)
- Parameter optimization

**Output:** Trajectory planner code, motion profile plot, smooth motion demo

---

## 🐧 Modul 10: ROS2 Fundamental untuk Robot Nyata (UBUNTU)

**Tujuan:** Memahami ROS2 framework dan implementasi untuk sistem robotika nyata.

**Konsep Utama:**
- Node, topic, service, action
- Publisher/Subscriber pattern
- TF (transformation), URDF (robot model)
- Launch files, parameters, logging
- Package structure & best practices

**Praktikum:**
- Buat node ROS2
- URDF robot design
- Visualisasi di RViz2
- Teleop dengan keyboard/joystick
- Multi-robot coordination (jika ada)

**Output:** ROS2 nodes, URDF, launch files, RViz visualization, demo video

---

## 🌉 Modul 11: ESP32 ↔ ROS2 Bridge (KOMUNIKASI IoT-ROBOT)

**Tujuan:** Integrasi embedded control dengan ROS2 untuk sistem terdistribusi.

**Konsep Utama:**
- Opsi A: micro-ROS (ESP32 sebagai node ROS2 native)
- Opsi B: MQTT bridge (ESP32 publish → ROS2 subscribe)
- Latency measurement, QoS (Quality of Service)
- Retry strategy, connection handling
- Real-time constraint

**Praktikum:**
- Setup bridge (micro-ROS atau MQTT)
- Test latency dan jitter
- Stress test dengan high-frequency data
- Fault tolerance testing

**Output:** Bridge code, latency report, stress test result, integration test

---

## 🎛️ Modul 12: State Machine & Sequencing (ROS2)

**Tujuan:** Implementasi state machine untuk kontrol high-level dari sistem robotika kompleks.

**Konsep Utama:**
- State machine design (idle/ready/run/fault/manual/auto)
- Event-driven programming
- Timeout handling, debouncing
- Recovery & fault tolerance
- SMCL (State Machine Control Language) atau SMCL libraries

**Praktikum:**
- Implementasi state machine di ROS2
- Test scenario (normal, fault, recovery)
- Mode switching (manual → auto)
- Fault injection & recovery

**Output:** State machine diagram, code, test scenario, demo video

---

## 🌐 Modul 13: Webserver IoT di UBUNTU (REMOTE MONITORING)

**Tujuan:** Membuat sistem monitoring dan control jarak jauh via web interface.

**Konsep Utama:**
- Backend: Node.js + Express atau Flask (Python)
- API REST untuk telemetry, command, configuration
- WebSocket untuk realtime data streaming
- Frontend: HTML/CSS/JavaScript (React/Vue optional)
- Authentication & authorization sederhana
- Database: InfluxDB atau PostgreSQL untuk time-series data

**Praktikum:**
- Buat dashboard monitoring
- Chart realtime
- Send command ke robot via web
- User login & permission
- Data export (CSV, JSON)

**Output:** Web dashboard, API documentation, backend code, frontend code

---

## 📈 Modul 14: Observability & Data Engineering (LEVEL INDUSTRI)

**Tujuan:** Implementasi monitoring stack untuk visibility & KPI calculation.

**Konsep Utama:**
- Time-series database (InfluxDB, Timescale)
- Data retention policy, aggregation
- Event log vs telemetry data
- KPI: uptime, MTBF, cycle time, fault frequency
- Visualization: Grafana dashboard
- Alert & notification system

**Praktikum:**
- Setup monitoring stack (InfluxDB + Grafana)
- Data collection dari multiple sources
- KPI calculation & visualization
- Create alarm system
- Report generation

**Output:** Grafana dashboard, monitoring code, KPI report, alert rules

---

## 🚀 Capstone Project (2-4 MINGGU)

Pilihan tema:

### Tema A: AGV Mini
- **Spesifikasi:**
  - Navigasi antar station dengan path planning
  - Automatic docking di charging station
  - Web monitoring + SCADA (optional)
  - Multi-load support
  
- **Technology Stack:**
  - ROS2 untuk navigation stack (SLAM, path planning)
  - ESP32 untuk motor control & sensor
  - Web dashboard untuk monitoring
  - Optional: PLC untuk sequencing
  
- **Deliverable:**
  - Mechanical design (chassis, wheel, bumper)
  - Electronics (motor, sensor, PCB)
  - ROS2 navigation & control
  - Web interface
  - Test report & video demo

### Tema B: Pick & Place 3-DOF
- **Spesifikasi:**
  - Robot arm 3-DOF dengan end-effector
  - Conveyor system untuk part transport
  - Color-based sorting (vision)
  - Automatic sequencing
  
- **Technology Stack:**
  - Fusion 360 untuk arm design
  - Kinematics (FK/IK) untuk control
  - Trajectory planning untuk smooth motion
  - ROS2 untuk coordination
  - OpenCV untuk vision
  
- **Deliverable:**
  - Mechanical design & fabrication
  - Electronics & motor drivers
  - Kinematics implementation
  - Vision-based control
  - ROS2 sequencing
  - Web monitoring
  - Test report & video

### Tema C: Mini Production Cell
- **Spesifikasi:**
  - Inspection station (vision-based)
  - Automated reject gate
  - Data historian & KPI
  - Fault recovery & diagnostics
  
- **Technology Stack:**
  - Vision system (OpenCV, TensorFlow optional)
  - State machine for sequencing
  - PLC/ESP32 for real-time control
  - Database for historian
  - Grafana for visualization
  
- **Deliverable:**
  - Complete mechanical system
  - Vision algorithm
  - Control logic
  - Data logging & historian
  - Grafana dashboard
  - Test report & video
  - Operations manual

### Capstone Requirements (Mandatory)
✅ Ada desain mekanik (Fusion 360) + PCB (EasyEDA)  
✅ Ada embedded control (ESP32/STM32) dengan PID/fuzzy  
✅ Ada edge computing (ROS2 di Ubuntu)  
✅ Ada web server IoT untuk monitoring & control  
✅ Ada kinematika (untuk robot arm/AGV)  
✅ Ada safety system & fault handling  
✅ Ada data logging & visualization  

### Deliverable Capstone
1. **Desain & Fabrication Report** — CAD, tolerance analysis, BOM
2. **Source Code** — Embedded, ROS2, Web, organized di GitHub
3. **Documentation** — README, API docs, deployment guide
4. **Test Report** — Performance metrics, test result, lessons learned
5. **Video Demo** — 5-10 menit, show hardware + control demo
6. **Presentation** — Slide + live demo (15-20 menit)

---

## 📚 Roadmap Pembelajaran

```
Modul 1-2        → Fundamentals (Architecture, Safety)
    ↓
Modul 3-4        → Power & Sensors (Electrical foundation)
    ↓
Modul 5-6        → Motor & Control (Basic motion control)
    ↓
Modul 7-9        → Mechanics & Kinematics (Advanced motion)
    ↓
Modul 10-12      → ROS2 & Software (High-level control)
    ↓
Modul 13-14      → IoT & Monitoring (Observability)
    ↓
Capstone         → Integration & Real-world application
```

---

## 🎓 Learning Outcomes

Setelah menyelesaikan program ini, peserta dapat:

1. **Desain:** Merancang sistem mekatronika dari requirement hingga implementasi
2. **Analisis:** Menganalisis performa sistem, identify bottleneck, optimize
3. **Implementasi:** Implement hardware, firmware, dan software secara terintegrasi
4. **Integration:** Mengintegrasikan multiple subsystem (mechanical, electrical, software)
5. **Troubleshooting:** Debug issue di hardware maupun software
6. **Documentation:** Dokumentasi teknis yang jelas dan profesional
7. **Communication:** Presentasi dan menjelaskan sistem ke stakeholder

---

## 📖 Topik Lanjutan (Optional)

- Advanced trajectory planning (optimal control, MPC)
- Machine learning untuk predictive maintenance
- Advanced vision (deep learning, 3D vision)
- Underwater robotics / Aerial robotics
- Human-robot interaction
- Swarm robotics
- Digital twin simulation

---

**Last Updated:** February 2026
