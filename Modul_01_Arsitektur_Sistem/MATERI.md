# MODUL 1: ARSITEKTUR SISTEM MEKATRONIKA & ROBOTIKA

## 1. DEFINISI & KONSEP UTAMA

### 1.1 Apa itu Mekatronika?

**Mekatronika** adalah integrasi sinergis dari:
- **Mechanical Engineering**: Desain mekanik, kinematika, dinamika
- **Electronic Engineering**: Sensor, aktuator, driver, power electronics
- **Software Engineering**: Algoritma kontrol, komunikasi, monitoring

Mekatronika bukan sekadar gabungan ketiga bidang, tetapi **integrasi terpadu** di mana setiap elemen saling bergantung dan dioptimasi bersama untuk mencapai fungsi sistem yang tidak bisa dicapai oleh masing-masing bidang secara terpisah.

**Contoh Sistem Mekatronika:**
- Printer 3D: mekanik (gantry system), elektronik (stepper driver), software (G-code parser, motion planner)
- Drone: mekanik (frame, propeller), elektronik (ESC, IMU, GPS), software (flight controller, stabilization algorithm)
- Smart Door Lock: mekanik (latch mechanism), elektronik (motor driver, fingerprint sensor), software (authentication, IoT connectivity)

### 1.2 Sistem Robotika Modern

**Karakteristik Sistem Robotika Modern:**
1. **Autonomous**: Mampu mengambil keputusan berdasarkan sensor input
2. **Adaptive**: Menyesuaikan behavior berdasarkan kondisi lingkungan
3. **Connected**: Terintegrasi dengan sistem lain (IoT, cloud, SCADA)
4. **Safe**: Built-in safety mechanism untuk protect human & equipment
5. **Efficient**: Optimasi energi, waktu, dan resource

**Requirement Sistem Robotika:**
- **Real-time response**: Latency rendah (< 10ms untuk critical control loop)
- **Reliability**: Uptime tinggi (> 99% untuk industrial)
- **Accuracy**: Repeatability ±0.1mm untuk precision task
- **Scalability**: Mudah ditambah sensor/aktuator baru
- **Maintainability**: Mudah troubleshoot, modular design

### 1.3 Real-Time System

**Real-time system** adalah sistem di mana **correctness** tidak hanya bergantung pada **hasil komputasi** tetapi juga **waktu saat hasil tersebut diproduksi**.

**Karakteristik Real-Time System:**
- **Determinism**: Output predictable untuk input yang sama
- **Low Latency**: Response time terjamin (bounded)
- **Low Jitter**: Variasi response time minimal

**Contoh:**
- **Hard Real-Time**: Airbag deployment (harus deploy dalam < 10ms, terlambat = fatal)
- **Soft Real-Time**: Video streaming (frame drop tidak fatal, tapi mengurangi kualitas)

**Dalam Robotika:**
- Motor control loop: 1-10 kHz (hard real-time)
- Sensor fusion: 100-1000 Hz (soft real-time)
- Web monitoring: 1-10 Hz (non real-time)

### 1.4 Distributed System

**Distributed system** adalah sistem di mana **processing** dan **control** tersebar di **multiple processor** yang berkomunikasi via network.

**Advantage:**
- **Scalability**: Mudah tambah node baru
- **Fault Tolerance**: Satu node fail, yang lain masih jalan
- **Specialization**: Setiap node optimized untuk task tertentu

**Challenge:**
- **Synchronization**: Koordinasi antar node kompleks
- **Communication Overhead**: Latency network, bandwidth limitation
- **Debugging**: Sulit trace bug yang melibatkan multiple node

---

## 2. KONSEP INTI

### 2.1 Sensor-Controller-Actuator Loop

Ini adalah **fundamental architecture** dari semua sistem mekatronika dan robotika.

```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│   SENSOR    │─────▶│ CONTROLLER  │─────▶│  ACTUATOR   │
│  (Input)    │      │ (Processing)│      │  (Output)   │
└─────────────┘      └─────────────┘      └─────────────┘
       ▲                                          │
       │                                          │
       └──────────────────────────────────────────┘
                    FEEDBACK LOOP
```

**Komponen:**
1. **Sensor**: Mengukur kondisi fisik (posisi, kecepatan, temperatur, tekanan)
2. **Controller**: Membandingkan sensor reading dengan target, menghitung control action
3. **Actuator**: Mengubah electrical signal menjadi physical action (motor, solenoid, valve)
4. **Feedback**: Sensor mengukur hasil action actuator, kembali ke controller

**Contoh Konkret - Temperature Control:**
- **Sensor**: Thermocouple mengukur suhu = 23°C
- **Controller**: Target = 25°C, error = +2°C → PID calculate → output = 30% heater power
- **Actuator**: Heater ON dengan 30% duty cycle
- **Feedback**: Suhu naik ke 24°C → error = +1°C → heater power reduced

### 2.2 Information Flow vs Control Flow

**Information Flow:**
- Data mengalir dari sensor → controller → monitoring/logging
- Purpose: **observability** (know what's happening)
- Latency requirement: relaxed (100ms - 1s acceptable)
- Examples: telemetry data, sensor log, video stream

**Control Flow:**
- Command mengalir dari controller → actuator
- Feedback mengalir dari sensor → controller
- Purpose: **controllability** (make things happen)
- Latency requirement: strict (< 10ms untuk real-time)
- Examples: motor command, PID output, E-Stop signal

### 2.3 Synchronous vs Asynchronous Communication

**Synchronous Communication:**
- Sender **wait** untuk response dari receiver sebelum continue
- Example: Request-Response (HTTP, Modbus master-slave)
- **Advantage**: Simple logic, guaranteed order
- **Disadvantage**: Blocking, latency propagation

**Asynchronous Communication:**
- Sender **tidak wait**, continue setelah send message
- Example: Publish-Subscribe (MQTT), event-driven
- **Advantage**: Non-blocking, parallel processing
- **Disadvantage**: Complex synchronization, message ordering

### 2.4 Centralized vs Distributed Control

**Centralized Control:**
```
                  ┌─────────────────┐
  Sensor 1 ─────▶│                 │
  Sensor 2 ─────▶│   SINGLE CPU    │────▶ Actuator 1
  Sensor 3 ─────▶│   (All Logic)   │────▶ Actuator 2
                  │                 │────▶ Actuator 3
                  └─────────────────┘
```

**Distributed Control:**
```
  Sensor 1 ─────▶┌─────┐          ┌─────┐────▶ Actuator 1
                  │CPU 1│◀────────▶│CPU 2│────▶ Actuator 2
  Sensor 2 ─────▶└─────┘          └─────┘
                      │               │
                      └───────┬───────┘
                          ┌───▼───┐
                          │CPU 3  │────▶ Actuator 3
                          │(Coord)│
                          └───────┘
```

**Comparison Table:**

| Aspek | Centralized | Distributed |
|-------|-------------|-------------|
| **Complexity** | Low (single code base) | High (coordination logic) |
| **Reliability** | Single point of failure | Fault tolerant |
| **Scalability** | Limited (CPU bottleneck) | Easy to expand |
| **Real-time** | Easier to guarantee | Harder (network latency) |
| **Cost** | Lower (1 powerful CPU) | Higher (multiple CPU) |
| **Debugging** | Easier (single system) | Complex (multi-system) |
| **Typical Use** | Small robot, prototype | Large robot, industrial |

**Trade-off Analysis:**
- **Complexity vs Reliability**: Distributed lebih reliable tapi lebih complex
- **Cost vs Performance**: Centralized lebih murah untuk sistem kecil, distributed lebih cost-effective untuk sistem besar
- **Recommendation**: Gunakan centralized untuk prototype & sistem kecil, distributed untuk production & sistem kompleks

---

## 3. DIAGRAM & ILUSTRASI

### 3.1 Block Diagram Sistem Robot - 5 Layer Architecture

```
┌──────────────────────────────────────────────────────────────────────┐
│  LAYER E: WEBSERVER IoT (Monitoring Jarak Jauh)                     │
│  - Node.js / Express / React Dashboard                              │
│  - Database: InfluxDB / MongoDB                                     │
│  - Protocol: HTTP / WebSocket                                       │
└──────────────────────────┬───────────────────────────────────────────┘
                           │ WiFi / Ethernet
                           │ (MQTT / REST API)
┌──────────────────────────▼───────────────────────────────────────────┐
│  LAYER D: OTOMASI INDUSTRI (Optional)                               │
│  - PLC: Modbus TCP / OPC UA                                         │
│  - SCADA: HMI, alarm management                                     │
└──────────────────────────┬───────────────────────────────────────────┘
                           │ Ethernet
                           │ (Modbus, OPC UA)
┌──────────────────────────▼───────────────────────────────────────────┐
│  LAYER C: EDGE COMPUTE (ROS2 Ubuntu)                                │
│  - High-level planning: path planning, task scheduling              │
│  - Sensor fusion: LiDAR, camera, IMU                                │
│  - Kinematics: FK/IK solver, trajectory generator                   │
│  - Protocol: ROS2 DDS                                               │
└──────────────────────────┬───────────────────────────────────────────┘
                           │ Serial / WiFi / Ethernet
                           │ (UART / MQTT / ROS2 bridge)
┌──────────────────────────▼───────────────────────────────────────────┐
│  LAYER B: REAL-TIME CONTROL (ESP32 / STM32)                         │
│  - Motor control: PID loop @ 1-10 kHz                               │
│  - Sensor reading: Encoder, limit switch, current sensor            │
│  - Safety: E-Stop monitoring, watchdog, timeout detection           │
│  - Communication: Serial, I2C, SPI, CAN                             │
└──────────────────────────┬───────────────────────────────────────────┘
                           │ PWM, Digital I/O, Analog
                           │
┌──────────────────────────▼───────────────────────────────────────────┐
│  LAYER A: PLANT / ROBOT (Mekanik + Elektronik)                      │
│  - Mechanical: Motor, gearbox, linkage, gripper                     │
│  - Sensor: Encoder, limit switch, proximity, force sensor           │
│  - Driver: Motor driver (H-bridge, stepper driver, servo driver)    │
│  - Power: 24V, 12V, 5V distribution                                 │
└──────────────────────────────────────────────────────────────────────┘
```

### 3.2 Signal Flow Diagram (Sensor → Processing → Actuator)

**Example: Motor Speed Control dengan PID**

```
 Encoder                ESP32                    Motor Driver          DC Motor
┌───────┐            ┌─────────────┐            ┌──────────┐         ┌────────┐
│       │   Pulse    │             │   PWM      │          │  Power  │        │
│ 600   │───────────▶│ Read Speed  │───────────▶│ H-Bridge │────────▶│ 1200   │
│ PPR   │  (GPIO)    │             │  (GPIO)    │          │ (24V)   │  RPM   │
└───────┘            │      │      │            └──────────┘         └────────┘
                     │      ▼      │
                     │   Calculate │
                     │   Error     │
                     │      │      │
                     │      ▼      │
                     │   PID       │
                     │   Compute   │
                     │      │      │
                     │      ▼      │
                     │   Output    │
                     └─────────────┘

Timeline (1 control cycle = 1ms):
t=0.0ms: Read encoder = 580 RPM
t=0.1ms: Target = 600 RPM, error = +20 RPM
t=0.2ms: PID compute: PWM = 75%
t=0.3ms: Output PWM to driver
t=1.0ms: Next cycle
```

### 3.3 Architecture Comparison: Centralized vs Distributed

**Centralized Example - Small Robot Arm:**
```
┌─────────────────────────────────────────┐
│         ESP32 (Main Controller)         │
│  ┌───────────────────────────────────┐  │
│  │ Motor Control (3 joints)          │  │
│  │ Sensor Reading (3 encoders)       │  │
│  │ Kinematics (FK/IK)                │  │
│  │ Communication (Serial to PC)      │  │
│  │ Safety (E-Stop, limit switch)     │  │
│  └───────────────────────────────────┘  │
└─────┬─────┬─────┬──────────┬─────┬──────┘
      │     │     │          │     │
   Motor1 Motor2 Motor3   Sensor  E-Stop
```

**Distributed Example - AGV Fleet:**
```
       ┌──────────────────────┐
       │  Cloud Server (AWS)  │ ← Fleet management
       └──────────┬───────────┘
                  │ MQTT
       ┌──────────▼───────────┐
       │  Edge Gateway (RPi)  │ ← Local coordination
       └──────────┬───────────┘
                  │ WiFi
       ┌──────────┴───────────┬───────────────┬──────────────┐
       ▼                      ▼               ▼              ▼
  ┌────────┐            ┌────────┐      ┌────────┐    ┌────────┐
  │ AGV 1  │            │ AGV 2  │      │ AGV 3  │    │ AGV 4  │
  │ (ESP32)│            │ (ESP32)│      │ (ESP32)│    │ (ESP32)│
  └────────┘            └────────┘      └────────┘    └────────┘
  ↓ Motor              ↓ Motor         ↓ Motor       ↓ Motor
  ↓ Sensor             ↓ Sensor        ↓ Sensor      ↓ Sensor
```

### 3.4 Communication Protocol Overview

#### MQTT (Message Queuing Telemetry Transport)

**Topology**: Publish-Subscribe (broker-based)

**Timing Diagram:**
```
 Publisher         Broker          Subscriber
    │                │                │
    │─Publish(topic)─▶│                │
    │                │◀─Subscribe(topic)─│
    │                │                │
    │─Message────────▶│                │
    │                │─Message────────▶│
    │                │                │
    t=0ms         t=50ms           t=100ms
```

**Karakteristik:**
- Latency: 50-200 ms (tergantung network)
- QoS Level: 0 (fire-forget), 1 (at least once), 2 (exactly once)
- Typical use: IoT monitoring, telemetry, non-real-time command

#### Modbus RTU (Serial Communication)

**Topology**: Master-Slave (polling-based)

**Timing Diagram:**
```
  Master                  Slave
    │                       │
    │─Request Read Reg 1───▶│
    │                       │ (Process)
    │◀───Response Data──────│
    │                       │
    │─Request Read Reg 2───▶│
    │                       │
    │◀───Response Data──────│
    │                       │
  t=0ms    t=10ms    t=20ms   t=30ms
```

**Karakteristik:**
- Latency: 10-100 ms (tergantung baud rate, polling rate)
- Checksum: CRC untuk error detection
- Typical use: PLC communication, industrial sensor/actuator

#### CAN Bus (Controller Area Network)

**Topology**: Multi-master (event-driven)

**Timing Diagram:**
```
 Node 1    Node 2    Node 3
   │         │         │
   │─Message(ID=0x100)─▶│ (All node receive)
   │         │         │
   │         │─Message(ID=0x200)─▶│
   │         │         │
   │◀────────┴─────────┘ (Arbitration)
   │                    
  t=0    t=1ms    t=2ms
```

**Karakteristik:**
- Latency: 1-10 ms (very fast, deterministic)
- Reliability: CRC, ACK, error detection & retransmission
- Typical use: Automotive ECU, real-time robot control

### 3.5 Failure Mode Examples

**Example 1: Motor Tidak Bergerak**
- **Symptom**: Motor command sent, tapi motor tidak bergerak
- **Possible Causes**:
  - Power supply disconnected (24V tidak sampai motor)
  - Driver rusak (short circuit, thermal shutdown)
  - Firmware crash (controller hang, tidak kirim PWM)
  - Kabel putus (signal wire break)
- **Detection**: Current sensor (no current draw), encoder tidak berubah, timeout
- **Mitigation**: Check power (LED indicator), watchdog (auto reset firmware), fallback to manual

**Example 2: Sensor Loss**
- **Symptom**: Encoder tidak report data
- **Possible Causes**:
  - Kabel disconnect (mechanical vibration)
  - Sensor rusak (overheating, overvoltage)
  - Communication error (I2C collision, UART framing error)
- **Detection**: Timeout (> 100ms tidak ada data), checksum fail
- **Mitigation**: Use last known position (short-term), reduce speed (safe mode), alarm operator

**Example 3: Communication Timeout**
- **Symptom**: ROS2 node tidak receive data dari ESP32
- **Possible Causes**:
  - WiFi disconnect (weak signal, interference)
  - ESP32 firmware crash (buffer overflow, memory leak)
  - Network congestion (bandwidth saturated)
- **Detection**: Heartbeat timeout (> 500ms no message)
- **Mitigation**: Reconnect auto (retry logic), buffer data locally, trigger E-Stop jika critical

---

## 4. CONTOH KASUS INDUSTRI

### 4.1 AGV untuk Warehouse Automation

**System Architecture:**
- **Sensor**:
  - LiDAR: SLAM (Simultaneous Localization and Mapping) untuk navigation
  - Encoder: Odometry (wheel rotation → distance traveled)
  - Bumper: Emergency collision detection
  - Camera: QR code reading untuk station identification
- **Controller**:
  - ESP32: Low-level motor control (PID speed control)
  - Ubuntu + ROS2: Path planning (A* algorithm), obstacle avoidance
- **Actuator**:
  - 2 DC Motor + Gearbox: Differential drive
  - Linear actuator: Lift platform untuk pallet loading
- **Monitoring**:
  - Web Dashboard: Real-time location, battery status, task queue
  - Database: Log semua trip (timestamp, path, duration)

**Latency Requirement:**
- Motor control loop: 10 ms (PID @ 100Hz)
- Obstacle detection → E-Stop: < 50 ms (safety critical)
- Path replanning: 100-500 ms (soft real-time)
- Web update: 1 s (non-critical)

**Communication Architecture:**
- ESP32 ↔ Ubuntu: Serial (115200 baud) atau WiFi (MQTT)
- Ubuntu ↔ Web Server: WiFi (HTTP REST API)
- AGV ↔ Fleet Manager: 4G/5G (MQTT to cloud broker)

### 4.2 Robot Arm untuk Manufacturing

**System Architecture:**
- **Mechanical**: 3-DOF (3 revolute joints), reach = 500mm, payload = 2kg
- **Actuator**:
  - 3 Stepper Motor (NEMA 23): Joint control dengan high torque
  - 1 Servo Motor (MG996R): Gripper control
- **Sensor**:
  - Encoder setiap joint: Position feedback (resolution 0.1°)
  - Force sensor di gripper: Detect object grasp
- **Controller**:
  - STM32: Real-time joint control (PID, trajectory interpolation)
  - ROS2 Ubuntu: Kinematics solver (FK/IK), motion planning (MoveIt)

**Control Flow:**
1. User input target position (x, y, z) via web
2. ROS2 compute IK: (x, y, z) → (θ1, θ2, θ3)
3. ROS2 generate trajectory: smooth path dari current → target
4. ROS2 send joint command via serial → STM32
5. STM32 run PID loop @ 1kHz untuk track trajectory
6. Encoder feedback → STM32 → ROS2 → Web (telemetry)

**Safety Features:**
- Software limit: Prevent joint over-rotation
- Workspace boundary: Cannot move outside allowed zone
- Collision detection: Force sensor spike → immediate stop
- E-Stop: Hardwired circuit cut power to all motor

### 4.3 Smart Building - HVAC Control

**System Architecture:**
- **Sensor**:
  - Temperature sensor (10 unit, 1 per room)
  - Motion sensor (occupancy detection)
  - CO₂ sensor (air quality)
- **Controller**:
  - PLC (Allen-Bradley MicroLogix): Centralized control logic
  - SCADA (FactoryTalk View): HMI untuk operator
- **Actuator**:
  - HVAC damper (pneumatic actuator)
  - Chiller pump (VFD variable frequency drive)
  - Fan (on/off relay)

**Control Logic:**
- IF (room occupied AND temp > 24°C) → AC ON
- IF (CO₂ > 1000 ppm) → increase fresh air intake
- IF (night mode) → reduce AC to 26°C (energy saving)

**Communication:**
- Sensor → PLC: Modbus RTU (RS485 bus, up to 32 device)
- PLC → SCADA: Ethernet/IP (real-time data)
- SCADA → Cloud: OPC UA (for analytics)

### 4.4 Automotive - ECU (Engine Control Unit)

**Distributed Control Example:**
- **ECU 1 (Engine)**: Fuel injection timing, ignition timing
- **ECU 2 (Transmission)**: Gear shift control
- **ECU 3 (ABS)**: Brake pressure modulation
- **ECU 4 (Airbag)**: Crash detection, deployment
- **ECU 5 (Infotainment)**: Display, audio, navigation

**Communication**: CAN Bus (125 kbps - 1 Mbps)
- Priority arbitration: Airbag message (ID=0x001, highest priority)
- Timing: Message broadcast every 10ms (deterministic)

**Why Distributed?**
- **Safety**: Airbag ECU independent, jika engine ECU fail, airbag tetap kerja
- **Specialization**: Setiap ECU optimized untuk real-time task
- **Scalability**: Tambah feature = tambah ECU, tidak overload main CPU

---

## 5. RINGKASAN

**Poin-poin Kunci:**

1. **Mekatronika adalah integrasi** mechanical, electronic, dan software — bukan sekadar gabungan, tetapi **synergy** yang menciptakan fungsi baru.

2. **Sensor-Controller-Actuator loop** adalah fundamental architecture — feedback loop critical untuk accurate control.

3. **Real-time system** membutuhkan **deterministic response** — latency harus bounded dan predictable.

4. **Centralized vs Distributed** adalah **trade-off** antara simplicity vs reliability/scalability — pilih sesuai requirement.

5. **Communication protocol** harus dipilih berdasarkan **latency requirement** dan **topology** — MQTT untuk IoT, Modbus untuk PLC, CAN untuk real-time.

6. **Failure mode analysis (FMEA)** adalah **mandatory** untuk safety-critical system — identify risk sejak design phase.

7. **Layered architecture** memisahkan **concern**: real-time control (Layer B), high-level planning (Layer C), monitoring (Layer E) — memudahkan debugging & maintenance.

**Take-away Message:**

> Sistem robotika modern adalah **integrasi kompleks** dari mekanik, elektronik, dan software. Desain yang baik membutuhkan **pemahaman end-to-end** — dari sensor physical characteristic, communication protocol, sampai control algorithm. Failure mode harus dianalisis sejak awal (**design for safety**), bukan di-add setelah sistem jadi.

**Hubungan dengan Modul Berikutnya:**

- **Modul 2 (Safety & Commissioning)**: Fokus pada implementasi **safety system** yang reliable — E-Stop circuit, limit switch, interlock logic, watchdog timer, fault diagnostics. Akan dijelaskan bagaimana **prevent sistem jadi unsafe** dan bagaimana **commissioning procedure** yang benar.

- **Modul 3 (Power & EMC)**: Fokus pada **power distribution** yang stable dan **EMC (Electromagnetic Compatibility)** — noise filtering, grounding, PCB layout best practice. Power integrity adalah **foundation** untuk reliable control system.

---

## 6. DESKRIPSI TUGAS VIDEO

Mahasiswa **WAJIB** membuat video tutorial dengan durasi **15-20 menit** yang menjelaskan konsep dan praktikum Modul 1.

### Point-point yang HARUS Dijelaskan:

#### A. PEMBUKAAN (1-2 menit)
- Perkenalan diri & kelompok (nama, NIM, kelas)
- Judul: **"Arsitektur Sistem Mekatronika dan Robotika"**
- Tujuan pembelajaran: "Memahami bagaimana sensor, controller, dan actuator terintegrasi dalam satu sistem robotika"
- Outline video: "Akan dijelaskan 5-layer architecture, block diagram sistem, contoh aplikasi industri, failure analysis, dan demo praktikum"

#### B. PENJELASAN TEORI (4-5 menit)
- **Sensor-Controller-Actuator loop**: Jelaskan dengan whiteboard atau slide, tunjukkan feedback loop
- **5-layer architecture**: Explain Layer A (Plant), B (Real-time), C (Edge), D (Industrial), E (IoT)
- **Centralized vs Distributed**: Kapan pakai yang mana? Berikan contoh concrete
- **Communication protocol**: MQTT vs Modbus vs CAN — comparison table, latency, typical use case
- **Real-time system**: Kenapa penting? Berikan contoh hard vs soft real-time
- **Contoh nyata**: AGV warehouse (sensor fusion, path planning), robot arm (kinematics), smart building (PLC logic)
- **Formula/metrik**: Latency (ms), throughput (msg/sec), reliability (%)

#### C. SETUP & HARDWARE (2-3 menit)
- Tunjukkan komponen sistem robot: motor, sensor, PCB, microcontroller (ESP32/Arduino), Ubuntu PC
- Tunjukkan koneksi fisik: Motor ↔ Driver ↔ ESP32 ↔ (WiFi/Serial) ↔ Ubuntu ↔ Web
- Jelaskan software stack:
  - **PlatformIO** (embedded firmware untuk ESP32)
  - **ROS2 Humble** (high-level control di Ubuntu)
  - **Node.js** (web server untuk monitoring)
- Show block diagram di slide/whiteboard (5-layer integration)

#### D. DEMO PRAKTIKUM (5-7 menit)

**Show & Explain:**
1. **Fusion 360**: Tunjukkan 3D model robot arm atau AGV (assembly, motion study)
2. **EasyEDA**: Tunjukkan schematic koneksi motor-driver-ESP32-sensor (explain pin connection)
3. **Code structure**: Show modular code (sensor.h, motor.h, control.h, main.cpp)
4. **Demo command flow**:
   - User kirim command via web (e.g., "move forward 1 meter")
   - Web server kirim MQTT message → ROS2 node
   - ROS2 kirim serial command → ESP32
   - ESP32 run motor dengan PID control
   - Encoder feedback → ESP32 → ROS2 → Web dashboard
5. **Signal routing**: Jelaskan digital signal (GPIO), PWM (motor speed), analog (sensor ADC), serial (UART/I2C/SPI)
6. **Latency analysis**: "Dari user click button sampai motor bergerak, total latency berapa? Ukur dengan timestamp log"
   - Example: Web → ROS2 (50ms) + ROS2 → ESP32 (20ms) + ESP32 → Motor (5ms) = **75ms total**

#### E. FAILURE ANALYSIS (2-3 menit)
- Tunjukkan **FMEA table** (Failure Mode & Effects Analysis) dengan min 5-10 failure mode
- **Contoh konkret**:
  - **Failure**: Motor tidak bergerak
  - **Penyebab**: Kabel putus, driver rusak, firmware crash
  - **Effect**: Robot stuck, mission gagal
  - **Detection**: Encoder tidak berubah > 1s, current sensor = 0A
  - **Mitigation**: Watchdog timer (auto restart firmware), timeout detection (alarm operator), fallback mode (manual control)
- **Demonstrasikan mitigation**: Unplug kabel motor → observe timeout alarm → show safe stop
- **Safety perspective**: "Jika motor gagal, system harus **safe stop**, bukan continue blindly"

#### F. TROUBLESHOOTING & INSIGHT (2-3 menit)
- Ceritakan masalah yang dihadapi saat praktikum (real experience)
  - Example: "Awalnya motor tidak jalan, ternyata driver masuk thermal shutdown karena heatsink kurang"
- **Debugging step-by-step**:
  1. Check power: Ukur voltage 24V dengan multimeter
  2. Check kabel: Visual inspection, continuity test
  3. Check driver: Swap dengan driver lain
  4. Check firmware: Upload blink test, cek serial monitor
- **Lesson learned**: "Penting verifikasi setiap layer **independent** dulu sebelum integrate — jangan langsung gabung semua, susah troubleshoot"
- **Tips praktis**:
  - Dokumentasikan pin mapping di comment code atau Excel
  - Jangan hardcode pin number, pakai #define atau config.h
  - Gunakan version control (Git) dari awal

#### G. PENUTUP (1 menit)
- **Kesimpulan**: "Sistem robotika modern adalah integrasi multiple layer — mechanical precise, electrical reliable, software robust. Desain arsitektur yang baik memudahkan development, testing, dan maintenance."
- **Modul berikutnya**: "Modul 2 akan fokus ke **Safety & Commissioning** — bagaimana prevent sistem jadi unsafe, bagaimana commissioning procedure yang benar, implementasi E-Stop, limit switch, watchdog."
- **Closing statement**: "Terima kasih telah menonton. Untuk pertanyaan, silakan hubungi via email atau forum praktikum."

### Catatan Penting:
- **WAJIB show hardware** (robot nyata atau prototype) — tidak boleh hanya slide
- **WAJIB screen recording** saat coding, compile, upload, serial monitor output
- **Audio jelas**: Gunakan microphone eksternal, background noise minimal
- **Resolusi video**: Minimum 720p (1280x720), recommended 1080p
- **Editing profesional**: Transisi smooth, text overlay untuk poin penting, zoom saat show detail

---

## LAMPIRAN: FORMULA & PERHITUNGAN

### Latency Calculation

**Total latency** dari user command sampai motor response:

$$
T_{total} = T_{web} + T_{network} + T_{ros2} + T_{serial} + T_{embedded} + T_{motor}
$$

**Example:**
- $T_{web}$ = 10 ms (JavaScript processing)
- $T_{network}$ = 50 ms (WiFi latency)
- $T_{ros2}$ = 20 ms (ROS2 node processing)
- $T_{serial}$ = 5 ms (UART transmission @ 115200 baud)
- $T_{embedded}$ = 2 ms (ESP32 processing + PID compute)
- $T_{motor}$ = 10 ms (motor mechanical response)

$$
T_{total} = 10 + 50 + 20 + 5 + 2 + 10 = 97 \text{ ms}
$$

**Acceptable?** Tergantung requirement — untuk joystick control, 100ms masih OK. Untuk high-speed pick-and-place, target < 20ms.

### Throughput Calculation

**Message throughput** di MQTT broker:

$$
Throughput = \frac{Messages}{Time} = \frac{1000 \text{ msg}}{1 \text{ s}} = 1000 \text{ msg/s}
$$

Dengan message size = 100 bytes:

$$
Bandwidth = 1000 \times 100 \times 8 = 800 \text{ kbps}
$$

**Cukup?** WiFi typical = 10-50 Mbps, jadi 800 kbps adalah **1-8%** dari bandwidth → OK.

### Reliability Calculation

**System uptime** (industrial requirement = 99.9%):

$$
Uptime = \frac{Total Time - Downtime}{Total Time} \times 100\%
$$

Example:
- Total time = 365 days = 8760 hours
- Downtime = 8.76 hours (maintenance + fault)

$$
Uptime = \frac{8760 - 8.76}{8760} \times 100\% = 99.9\%
$$

**Target**: 99.9% (three nines) untuk industrial. Medical robot atau automotive safety system target 99.99% atau lebih.

---

## REFERENSI

Lihat file [Referensi.md](Referensi.md) untuk daftar lengkap buku, paper, dan online resource.

---

**Prepared by:** Tim Penyusun Praktikum Mekatronika & Robotika  
**Last Updated:** 5 Februari 2026  
**Version:** 1.0
