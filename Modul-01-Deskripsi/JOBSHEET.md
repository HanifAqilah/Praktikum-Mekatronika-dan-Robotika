# JOBSHEET PRAKTIKUM
# MODUL 1: ARSITEKTUR SISTEM MEKATRONIKA & ROBOTIKA

---

## 1. TUJUAN PRAKTIKUM

### Tujuan Umum
Mahasiswa dapat merancang arsitektur sistem robotika terintegrasi dengan memahami interaksi antara komponen mechanical, electrical, dan software.

### Sub-tujuan
Setelah menyelesaikan praktikum ini, mahasiswa mampu:
1. **Desain blok diagram sistem** yang mencakup mechanical layer, electrical layer, dan software layer
2. **Mapping signal & pin** dari sensor ke controller ke actuator dengan dokumentasi lengkap
3. **Analisis komunikasi protocol** (MQTT, Modbus, CAN) dan memilih yang sesuai untuk sistem
4. **FMEA basic** untuk mengidentifikasi potential failure dan mitigation strategy

### Kompetensi yang Diharapkan
- Mampu membaca dan membuat block diagram sistem robotika
- Mampu merancang pin mapping dan wiring diagram
- Mampu memilih protocol komunikasi yang sesuai
- Mampu mengidentifikasi dan mengantisipasi system failure

---

## 2. ALAT & BAHAN

### HARDWARE

| No | Item | Spesifikasi | Jumlah | Keterangan |
|----|------|-------------|--------|------------|
| 1 | Mini robot chassis | AGV 2WD atau arm 3-DOF | 1 set | Sesuai pilihan kelompok |
| 2 | Motor DC | 12V, 100-200 RPM, dengan encoder | 2-3 unit | Untuk drive/joint control |
| 3 | Limit Switch | Mechanical, SPDT | 2-4 unit | Safety & homing |
| 4 | IR Sensor | TCRT5000 atau sejenis | 1-2 unit | Line follower/detection |
| 5 | Ultrasonic Sensor | HC-SR04 atau VL53L0X | 1-2 unit | Distance measurement |
| 6 | Microcontroller | Arduino Mega 2560 atau ESP32 | 1 unit | Main controller |
| 7 | Motor Driver | L298N atau BTS7960 | 1-2 unit | H-bridge untuk DC motor |
| 8 | Power Supply | 24V 5A, 12V 3A, 5V 3A | 1 set | Multi-voltage |
| 9 | Kabel & Connector | Jumper wire, terminal block | 1 set | Sesuai kebutuhan |
| 10 | Breadboard | 830 tie-point | 1-2 unit | Prototyping |
| 11 | PCB prototype | Single-layer atau double-layer | 1 unit | Optional, untuk final |
| 12 | Ubuntu PC | Ubuntu 22.04, min 8GB RAM | 1 unit | Untuk ROS2 (optional) |

### SOFTWARE

| No | Software | Version | Purpose | Link Download |
|----|----------|---------|---------|---------------|
| 1 | Fusion 360 | Latest | CAD design, motion study | autodesk.com/products/fusion-360 |
| 2 | EasyEDA | Web-based | Schematic & PCB design | easyeda.com |
| 3 | VS Code | Latest | Code editor | code.visualstudio.com |
| 4 | PlatformIO | Extension | Embedded development | platformio.org |
| 5 | Python | 3.10+ | Data analysis, scripting | python.org |
| 6 | Git | Latest | Version control | git-scm.com |
| 7 | ROS2 Humble | 22.04 LTS | Edge compute (optional) | docs.ros.org |
| 8 | draw.io | Web-based | Block diagram | app.diagrams.net |

### TOOLS

| No | Tool | Purpose | Required? |
|----|------|---------|-----------|
| 1 | Multimeter | Measure voltage, current, resistance | **Wajib** |
| 2 | Oscilloscope | Signal analysis (optional untuk Modul 1) | Optional |
| 3 | USB Cable | Programming microcontroller | **Wajib** |
| 4 | Screwdriver Set | Assembly mechanical | **Wajib** |
| 5 | Wire Stripper | Cable preparation | **Wajib** |
| 6 | Soldering Station | PCB assembly (jika perlu) | Optional |

---

## 3. LANGKAH KERJA

### PERSIAPAN

#### 3.1 Install Software
1. Download dan install VS Code + PlatformIO extension
2. Install Git dan setup GitHub account
3. Install Python 3.10+ (dengan pip)
4. Install Fusion 360 (student license) dan EasyEDA (web-based)
5. (Optional) Install ROS2 Humble di Ubuntu PC

#### 3.2 Clone Repository
```bash
cd ~/Documents
git clone [REPO_LINK_DARI_INSTRUKTUR]
cd Praktikum_Mekatronika_Robotika
```

#### 3.3 Hardware Check
- Cek semua komponen sesuai daftar
- Test power supply: ukur output 24V, 12V, 5V dengan multimeter
- Test microcontroller: upload blink program, verify LED blink
- Test motor: supply 12V langsung (tanpa controller), motor harus berputar

---

### PERCOBAAN 1: DESAIN BLOK DIAGRAM SISTEM

**Tujuan**: Memahami system architecture dengan membuat block diagram lengkap

#### Langkah:

**a. Definisikan Requirement Sistem**

Tentukan requirement sistem robot yang akan didesain:

- **Input**:
  - User command (via web dashboard atau joystick)
  - Sensor data (encoder, limit switch, proximity sensor)
  - Emergency signal (E-Stop button)

- **Process**:
  - Path planning (jika AGV) atau trajectory planning (jika robot arm)
  - Motion control (PID untuk motor speed/position)
  - Sensor fusion (combine multiple sensor untuk decision)
  - Safety logic (E-Stop handling, limit check, fault detection)

- **Output**:
  - Motor movement (PWM signal to driver)
  - Status feedback (LED indicator, buzzer)
  - Telemetry data (ke web dashboard atau database)
  - Alarm signal (jika fault detected)

**b. Identifikasi Setiap Komponen**

Buat tabel inventory:

| Layer | Komponen | Spesifikasi | Fungsi |
|-------|----------|-------------|--------|
| **Mechanical** | Motor DC | 12V, 150 RPM, 2A stall | Drive wheel |
| | Encoder | 600 PPR, incremental | Speed/position feedback |
| | Gearbox | Ratio 1:20 | Torque amplification |
| | Chassis | Aluminum, 300x200mm | Structure |
| **Electrical** | Power Supply | 24V 5A | Main power source |
| | Voltage Regulator | LM7812 (12V), LM7805 (5V) | Voltage conversion |
| | Motor Driver | L298N, H-bridge | PWM to motor power |
| | Protection | Fuse 5A, TVS diode | Overvoltage/overcurrent |
| **Software** | Embedded Firmware | C++ PlatformIO | Real-time motor control |
| | Edge Compute | ROS2 Python | Path planning, high-level logic |
| | Web Server | Node.js + Express | IoT monitoring |
| | Database | InfluxDB | Time-series data logging |

**c. Buat Block Diagram di Tool**

Gunakan **draw.io** (app.diagrams.net) atau **Visio** atau **Fusion 360** untuk menggambar.

**Contoh Block Diagram:**

```
┌─────────────────────────────────────────────────────────────────────┐
│                        LAYER E: WEB SERVER                          │
│  [Node.js] ←→ [InfluxDB] ←→ [React Dashboard]                      │
└────────────────────────────┬────────────────────────────────────────┘
                             │ HTTP/MQTT (WiFi)
┌────────────────────────────▼────────────────────────────────────────┐
│                      LAYER C: ROS2 UBUNTU                           │
│  [Path Planner] ←→ [Motor Controller Node] ←→ [Sensor Fusion]      │
└────────────────────────────┬────────────────────────────────────────┘
                             │ Serial UART (115200 baud)
┌────────────────────────────▼────────────────────────────────────────┐
│                      LAYER B: ESP32 FIRMWARE                        │
│  [PID Controller] ←→ [Sensor Handler] ←→ [Communication]           │
└─┬───────────────┬────────────┬───────────────────────────────────┬──┘
  │ PWM           │ GPIO       │ ADC                               │ GPIO
┌─▼───────┐  ┌───▼────┐  ┌────▼─────┐                        ┌────▼──────┐
│ Motor   │  │Encoder │  │ Distance │                        │ E-Stop    │
│ Driver  │  │ 600PPR │  │ Sensor   │                        │ Button    │
└─┬───────┘  └────────┘  └──────────┘                        └───────────┘
  │ 24V
┌─▼────────────────────────────────────────────────────────────────────┐
│                      LAYER A: MECHANICAL                             │
│  [DC Motor 12V] ←→ [Gearbox 1:20] ←→ [Wheel 80mm]                   │
└──────────────────────────────────────────────────────────────────────┘
```

**d. Anotasi Interface Antar Blok**

Tambahkan detail interface:

| Interface | Protocol | Signal Type | Frequency/Baud | Purpose |
|-----------|----------|-------------|----------------|---------|
| Web ↔ ROS2 | MQTT | JSON message | 10 Hz | Command & telemetry |
| ROS2 ↔ ESP32 | Serial UART | Text/binary | 115200 baud | Motor command & sensor data |
| ESP32 ↔ Motor Driver | PWM + GPIO | Digital | PWM 20 kHz, GPIO | Speed control, direction |
| ESP32 ↔ Encoder | GPIO interrupt | Digital pulse | Max 10 kHz | Position feedback |
| ESP32 ↔ Distance Sensor | ADC / I2C | Analog / Digital | Sample 100 Hz | Obstacle detection |

**e. Deliverable**

- Block diagram image (save as PNG atau PDF, high resolution)
- Description text (explain setiap block, interface, data flow)
- File naming: `modul01_block_diagram_[KELOMPOK]_20260205.png`

#### Data Pengamatan

| Komponen | Tipe | Fungsi | Interface | Pin/Protocol |
|----------|------|--------|-----------|--------------|
| Motor Left | DC 12V 150RPM | Actuator drive | PWM + DIR | GPIO 25, 26 |
| Motor Right | DC 12V 150RPM | Actuator drive | PWM + DIR | GPIO 27, 28 |
| Encoder Left | Incremental 600PPR | Feedback speed/position | Digital interrupt | GPIO 32, 33 |
| Encoder Right | Incremental 600PPR | Feedback speed/position | Digital interrupt | GPIO 34, 35 |
| Ultrasonic Front | HC-SR04 | Distance measurement | Trigger + Echo | GPIO 18, 19 |
| IR Sensor | TCRT5000 | Line detection | Analog ADC | GPIO 36 |
| E-Stop Button | NO (Normally Open) | Emergency stop | Digital input | GPIO 21 |
| Status LED | Red/Green | Visual indicator | Digital output | GPIO 22, 23 |

**Analisis:**
- Total GPIO used: 16 pin
- ESP32 total GPIO: 34 pin → **Spare = 18 pin** (cukup untuk future expansion)
- Potential conflict: GPIO 36-39 adalah input-only (tidak bisa output) → OK untuk sensor

---

### PERCOBAAN 2: PIN MAPPING & SIGNAL ROUTING

**Tujuan**: Detailed mapping semua signal dari sensor ke controller ke actuator

#### Langkah:

**a. Ambil Hardware & Identifikasi Pin**

1. Ambil ESP32 development board (atau Arduino Mega)
2. Buka datasheet ESP32: https://docs.espressif.com/
3. Identifikasi pin yang **dapat** dan **tidak dapat** digunakan:
   - ESP32: GPIO 6-11 reserved untuk flash (JANGAN GUNAKAN)
   - ESP32: GPIO 34-39 input-only (tidak bisa output atau PWM)
   - ESP32: GPIO 0, 2 adalah strapping pin (avoid jika bisa)

**b. Create Wiring Diagram**

Gunakan **EasyEDA** atau gambar manual dengan label lengkap.

**Template EasyEDA:**
1. Buka EasyEDA → New Project → Schematic
2. Add komponen: ESP32 module, motor driver L298N, motor DC, encoder, sensor
3. Wire semua koneksi dengan label net name
4. Add power symbol: GND, +5V, +12V, +24V
5. Annotate setiap koneksi dengan signal name

**Contoh Wiring (Motor Driver L298N ke ESP32):**

```
ESP32 GPIO 25 (PWM_LEFT)  ──────────▶ L298N ENA (Enable A)
ESP32 GPIO 26 (DIR_LEFT_1) ─────────▶ L298N IN1
ESP32 GPIO 27 (DIR_LEFT_2) ─────────▶ L298N IN2
ESP32 GND ──────────────────────────▶ L298N GND
ESP32 5V ───────────────────────────▶ L298N 5V (logic)

L298N OUT1 ─────────────────────────▶ Motor Left (+)
L298N OUT2 ─────────────────────────▶ Motor Left (-)
L298N 12V ──────────────────────────▶ Power Supply 12V
```

**c. Create Pin Mapping Table**

| Signal Name | Component | Microcontroller Pin | Type | Direction | Purpose |
|-------------|-----------|---------------------|------|-----------|---------|
| PWM_MOTOR_LEFT | Motor Driver L298N ENA | GPIO 25 | PWM Output | OUT | Speed control left motor |
| DIR_MOTOR_LEFT_1 | Motor Driver L298N IN1 | GPIO 26 | Digital Output | OUT | Direction control IN1 |
| DIR_MOTOR_LEFT_2 | Motor Driver L298N IN2 | GPIO 27 | Digital Output | OUT | Direction control IN2 |
| PWM_MOTOR_RIGHT | Motor Driver L298N ENB | GPIO 14 | PWM Output | OUT | Speed control right motor |
| DIR_MOTOR_RIGHT_1 | Motor Driver L298N IN3 | GPIO 12 | Digital Output | OUT | Direction control IN3 |
| DIR_MOTOR_RIGHT_2 | Motor Driver L298N IN4 | GPIO 13 | Digital Output | OUT | Direction control IN4 |
| ENC_LEFT_A | Encoder Left Channel A | GPIO 32 | Digital Input | IN | Speed feedback phase A |
| ENC_LEFT_B | Encoder Left Channel B | GPIO 33 | Digital Input | IN | Speed feedback phase B |
| ENC_RIGHT_A | Encoder Right Channel A | GPIO 34 | Digital Input | IN | Speed feedback phase A |
| ENC_RIGHT_B | Encoder Right Channel B | GPIO 35 | Digital Input | IN | Speed feedback phase B |
| ULTRASONIC_TRIG | HC-SR04 Trigger | GPIO 18 | Digital Output | OUT | Trigger ultrasonic pulse |
| ULTRASONIC_ECHO | HC-SR04 Echo | GPIO 19 | Digital Input | IN | Receive echo pulse |
| IR_SENSOR | TCRT5000 | GPIO 36 (ADC0) | Analog Input | IN | Line detection |
| LIMIT_SWITCH_1 | Limit Switch Front | GPIO 21 | Digital Input | IN | Safety front limit |
| LIMIT_SWITCH_2 | Limit Switch Rear | GPIO 22 | Digital Input | IN | Safety rear limit |
| E_STOP | Emergency Stop Button | GPIO 23 | Digital Input | IN | Emergency stop |
| LED_STATUS_RED | LED Indicator | GPIO 2 | Digital Output | OUT | Error/fault indicator |
| LED_STATUS_GREEN | LED Indicator | GPIO 4 | Digital Output | OUT | Normal operation |

**d. Deliverable**

- Wiring diagram (EasyEDA file + exported PNG/PDF)
- Pin mapping table (Excel atau Markdown table)
- File naming: `modul01_wiring_diagram_[KELOMPOK].png`, `modul01_pin_mapping_[KELOMPOK].xlsx`

#### Data Pengamatan

Setelah selesai mapping, isi data:

- **Total digital pin used**: _____ pin
- **Total analog pin used**: _____ pin
- **Total PWM pin used**: _____ pin
- **Spare pin available**: _____ pin
- **Potential conflict**: (e.g., I2C pada pin yang sama dengan SPI, atau pin yang reserved)
  - Contoh conflict: _______________
  - Solution: _______________

**Verifikasi:**
- [ ] Semua pin assignment tidak konflik
- [ ] Pin input-only tidak diassign sebagai output
- [ ] PWM pin assignment sesuai dengan hardware PWM channel
- [ ] Reserved pin (0, 2, 6-11 untuk ESP32) dihindari
- [ ] Ground connection semua subsystem terhubung (common ground)

---

### PERCOBAAN 3: COMMUNICATION PROTOCOL ANALYSIS

**Tujuan**: Memahami dan memilih protocol komunikasi yang sesuai untuk sistem

#### Langkah:

**a. Research 3 Protocol: MQTT, Modbus RTU, CAN Bus**

Untuk setiap protocol, baca dokumentasi dan isi comparison table.

**b. Comparison Table**

| Parameter | MQTT | Modbus RTU | CAN Bus |
|-----------|------|------------|---------|
| **Topology** | Publish-Subscribe (broker-based) | Master-Slave (polling) | Multi-master (peer-to-peer) |
| **Physical Layer** | TCP/IP (Ethernet, WiFi) | RS485 (serial differential) | CAN transceiver (differential) |
| **Latency** | 50-200 ms | 10-100 ms | 1-10 ms |
| **Max Distance** | Unlimited (internet) | 1200 m @ 9600 baud | 40 m @ 1 Mbps, 1000 m @ 50 kbps |
| **Max Devices** | Unlimited (broker limit) | 32 devices (RS485) | 110 devices (teoritis) |
| **Reliability** | QoS 0/1/2 (adjustable) | CRC checksum | CRC + ACK + retransmission |
| **Bandwidth** | Efficient (small message) | Low (9600-115200 baud) | High (125 kbps - 1 Mbps) |
| **Complexity** | Medium (broker setup) | Low (simple master-slave) | High (arbitration, error handling) |
| **Determinism** | No (internet delay unpredictable) | Yes (polling interval fixed) | Yes (priority arbitration) |
| **Real-time** | Not suitable | Suitable (with fast polling) | Very suitable (hard real-time) |
| **Cost** | Low (WiFi/Ethernet module) | Low (UART + RS485 chip) | Medium (CAN transceiver) |
| **Typical Use** | IoT monitoring, cloud connectivity | PLC, industrial sensor/actuator | Automotive ECU, real-time robot |

**c. Analisis Requirement Sistem**

Berdasarkan sistem yang didesain, tentukan:

| Requirement | Value | Justification |
|-------------|-------|---------------|
| **Latency requirement** | _____ ms | Berdasarkan control loop frequency (e.g., 100Hz = 10ms) |
| **Real-time critical?** | Yes / No | Jika real-time (e.g., motor control), pilih CAN atau Modbus. Jika monitoring only, MQTT OK. |
| **Number of devices** | _____ | Jumlah sensor + actuator + controller |
| **Bandwidth needed** | _____ kbps | Estimate: message size × frequency × number of device |
| **Distance** | _____ m | Jarak maksimum antara node |
| **Reliability** | High / Medium / Low | Industrial = high, prototype = medium |

**Contoh Calculation Bandwidth:**
- Message size: 50 bytes (motor command: speed, direction, status)
- Frequency: 100 Hz (10ms cycle time)
- Number of motor: 2

$$
Bandwidth = 50 \text{ bytes} \times 100 \text{ Hz} \times 2 = 10000 \text{ bytes/s} = 80 \text{ kbps}
$$

→ CAN @ 125 kbps **sufficient**. Modbus @ 115200 baud = 11.5 kBps = 92 kbps → **marginal, bisa bottleneck**.

**d. Recommendation**

**Pilih protocol yang paling sesuai dan justifikasi.**

**Contoh Jawaban:**

> **Recommendation: CAN Bus** untuk komunikasi ESP32 ↔ Motor Driver, **MQTT** untuk komunikasi ESP32 ↔ Web Server.
>
> **Justification:**
> - Motor control adalah **real-time critical** (latency < 10ms) → CAN Bus ideal dengan latency 1-10ms dan deterministic.
> - Web monitoring adalah **non-critical** (latency 100-500ms acceptable) → MQTT efficient untuk IoT, QoS level adjustable.
> - **Hybrid approach**: CAN untuk local real-time, MQTT untuk remote monitoring → best of both.
>
> **Alternative**: Jika budget limited, gunakan **Serial UART** (ESP32 ↔ motor driver lokal, simple & low-cost), **MQTT** untuk ESP32 ↔ Web.

**e. Deliverable**

- Protocol comparison table (Excel atau Markdown)
- Bandwidth calculation spreadsheet
- Recommendation document dengan justification
- File naming: `modul01_protocol_analysis_[KELOMPOK].xlsx`

---

### PERCOBAAN 4: FMEA BASIC (FAILURE MODE & EFFECTS ANALYSIS)

**Tujuan**: Mengidentifikasi potential failure dan mitigation strategy

#### Langkah:

**a. Identifikasi Potential Failure Point**

Brainstorm semua kemungkinan failure di setiap layer:

**Mechanical Failure:**
- Bearing failure (friction meningkat → motor stall)
- Belt slip (tension kendur → power tidak transmitted)
- Gear tooth break (overload → sudden stop)
- Structure crack (vibration → alignment shift)

**Electrical Failure:**
- Power loss (fuse blown, connector loose)
- Wire break (vibration, poor soldering)
- Short circuit (insulation fail, metal debris)
- Driver overheat (thermal shutdown)
- Sensor damage (overvoltage, ESD)

**Software Failure:**
- Firmware crash (memory leak, stack overflow)
- Communication loss (timeout, packet drop)
- Sensor timeout (cable disconnect, I2C collision)
- Control divergence (PID tuning salah → unstable)

**b. FMEA Table**

Untuk setiap failure mode, isi tabel berikut:

| ID | Failure Mode | Component | Effect | Cause | Detection | Mitigation | Severity (1-9) |
|----|--------------|-----------|--------|-------|-----------|------------|----------------|
| 1 | Motor not moving | Motor + Driver | Robot stuck, mission failed | Power loss, driver thermal shutdown, firmware crash | Current sensor = 0A, encoder tidak berubah > 1s | Watchdog timer (auto reset), E-Stop (safe stop), alarm operator | **High (8)** |
| 2 | Sensor loss (encoder) | Encoder | Wrong position estimate, collision risk | Cable disconnect, sensor damaged, EMI noise | Timeout > 100ms, inconsistent reading | Use last known position (short-term), reduce speed to safe mode, LED alarm | **Medium (6)** |
| 3 | Communication timeout | ESP32 ↔ ROS2 | Loss of high-level command, manual control only | WiFi disconnect, firmware crash, network congestion | Heartbeat timeout > 500ms | Auto reconnect (retry 3x), buffer command locally, fallback to autonomous mode | **Medium (5)** |
| 4 | Power supply failure | 24V PSU | Complete system shutdown | Fuse blown, PSU overload, AC input loss | Voltage monitor < 20V | UPS (battery backup), alarm (buzzer + LED), graceful shutdown (save state) | **High (9)** |
| 5 | E-Stop not working | E-Stop button/circuit | Cannot stop in emergency, safety risk | Button stuck, relay fail, wiring short | Self-test @ startup (trigger E-Stop, verify motor stop) | Redundant E-Stop (2 button series), hardwired circuit (independent dari CPU) | **Critical (9)** |
| 6 | Firmware hang | ESP32 firmware | No response, motor tidak controlled | Memory leak, infinite loop, interrupt conflict | Watchdog timer expire (no heartbeat > 2s) | Watchdog auto-reset, LED blink pattern (indicate hang), debug via JTAG | **High (7)** |
| 7 | Limit switch fail | Limit switch | Over-travel, mechanical damage | Switch mechanical wear, cable break | Position exceed software limit, force sensor spike | Software limit (backup), current monitoring (motor stall detection), auto reverse | **High (8)** |
| 8 | Motor stall | Motor | Cannot complete task, overheating | Mechanical jam, overload, low voltage | Current spike (> 3A nominal), encoder tidak berubah meskipun PWM max | Current limit (cut power @ 5A), timeout (stop after 2s stuck), thermal sensor | **Medium (6)** |
| 9 | Sensor noise (ADC) | IR sensor / Distance sensor | False detection, erratic behavior | EMI dari motor switching, poor grounding | Reading variance > 20% in 100ms | Software filter (moving average, Kalman), hardware filter (RC low-pass), shielded cable | **Low (4)** |
| 10 | Battery low (jika mobile) | Battery | Unpredictable shutdown, data loss | Discharge over time, high current draw | Voltage monitor < 10V | Low battery alarm (80% → warning, 90% → RTH), auto return-to-home, save state to EEPROM | **Medium (5)** |
| 11 | Overheating | Motor driver / Regulator | Thermal shutdown, system stop | Insufficient cooling, high ambient temp, overload | Temperature sensor > 70°C | Heatsink + fan (active cooling), reduce PWM (derating), thermal shutdown (safe stop) | **Medium (6)** |
| 12 | Memory overflow | ESP32 firmware | Crash, data corruption | Large buffer, no garbage collection | Free heap < 10%, stack monitor | Reduce buffer size, dynamic allocation check, periodic restart (every 24h) | **Medium (5)** |

**Severity Rating Guide:**
- **1-3 (Low)**: Minor inconvenience, tidak affect mission, easy recovery
- **4-6 (Medium)**: Mission disruption, manual intervention needed, potential minor damage
- **7-9 (High)**: Safety risk, major damage, cannot recover without repair
- **10 (Critical)**: Catastrophic (injury, death, total system loss) — **MUST mitigate**

**c. Deliverable**

- FMEA table dengan minimal **10 failure modes**
- Mitigation strategy document
- File naming: `modul01_fmea_[KELOMPOK].xlsx`

#### Data Pengamatan

- Total failure modes identified: _____
- Severity distribution:
  - High (7-9): _____ items
  - Medium (4-6): _____ items
  - Low (1-3): _____ items
- Failure dengan mitigation **belum implemented**: _____ (list ID)
- Failure dengan mitigation **sudah implemented**: _____ (list ID)

**Prioritization:**
1. Implementasikan mitigation untuk **High severity** dulu
2. Medium severity: implementasi jika waktu cukup
3. Low severity: acceptable risk, document only

---

## 4. DATA PENGAMATAN (SUMMARY)

### 4.1 Block Diagram
- [ ] Block diagram image tersedia (PNG/PDF)
- [ ] Annotation lengkap (protocol, frequency, signal type)
- [ ] Description text menjelaskan data flow

### 4.2 Pin Mapping
- [ ] Wiring diagram tersedia (EasyEDA + exported image)
- [ ] Pin mapping table lengkap (semua signal documented)
- [ ] Pin conflict analysis (spare pin, potential issue)

### 4.3 Protocol Comparison
- [ ] Comparison table (MQTT, Modbus, CAN)
- [ ] Bandwidth calculation
- [ ] Recommendation + justification

### 4.4 FMEA
- [ ] FMEA table minimal 10 failure modes
- [ ] Severity rating logical
- [ ] Mitigation strategy realistic & implementable

---

## 5. ANALISIS

### PERTANYAAN ANALISIS (WAJIB DIJAWAB):

#### Pertanyaan 1: Centralized vs Distributed Control
**Q**: Apa advantage & disadvantage dari centralized control vs distributed control untuk sistem robot yang Anda desain? Pilih mana yang lebih sesuai?

**A (Template)**: 
> Sistem yang kami desain adalah [AGV/Robot Arm/Conveyor]. Kami memilih **[Centralized/Distributed]** control karena:
> - Advantage: _____
> - Disadvantage: _____
> - Trade-off yang dipertimbangkan: _____
> - Justification: _____

#### Pertanyaan 2: Protocol Selection
**Q**: Protocol komunikasi mana yang Anda pilih untuk sistem ini? Mengapa?

**A (Template)**:
> Kami memilih **[Protocol Name]** untuk komunikasi antara **[Node A]** dan **[Node B]** karena:
> - Latency requirement: _____ ms → Protocol latency = _____ ms → **[Memenuhi/Tidak memenuhi]**
> - Bandwidth requirement: _____ kbps → Protocol bandwidth = _____ kbps → **[Sufficient/Insufficient]**
> - Reliability: **[High/Medium/Low]** sesuai dengan aplikasi **[Real-time/Non-real-time]**
> - Justification: _____

#### Pertanyaan 3: Critical Failure Mode
**Q**: Apa failure mode paling critical dalam sistem Anda? Bagaimana mitigasi yang Anda implementasikan?

**A (Template)**:
> Failure mode paling critical adalah **[Failure Mode Name]** dengan severity **[Score]** karena:
> - Effect: _____
> - Potential consequence: _____
> - Mitigation yang kami implementasikan:
>   1. _____
>   2. _____
>   3. _____
> - Residual risk setelah mitigation: **[Low/Medium/High]**

#### Pertanyaan 4: Design Validation
**Q**: Apakah current design memenuhi requirement sistem? Jika tidak, apa yang perlu diubah?

**A (Template)**:
> **Requirement Check:**
> - Latency: Target = _____ ms, Actual estimate = _____ ms → **[Pass/Fail]**
> - Reliability: Target = _____ % uptime, Design capability = _____ % → **[Pass/Fail]**
> - Scalability: Future expansion planned? **[Yes/No]** → Design support? **[Yes/No]**
> 
> **Gap Analysis:**
> - Requirement yang belum dipenuhi: _____
> - Proposed solution: _____

#### Pertanyaan 5: Design Trade-off
**Q**: Apakah ada trade-off dalam design Anda? (e.g., complexity vs reliability, cost vs performance)

**A (Template)**:
> Trade-off yang kami identify:
> 1. **[Trade-off 1]**: Memilih **[Option A]** dibanding **[Option B]** karena _____
>    - Advantage: _____
>    - Disadvantage: _____
>    - Acceptable? **[Yes/No]** → Alasan: _____
> 2. **[Trade-off 2]**: _____

**Example Jawaban:**
> Trade-off: **Centralized control (ESP32 only)** vs **Distributed control (ESP32 + ROS2 Ubuntu)**.
> - Kami pilih **Centralized** untuk prototype karena:
>   - Advantage: Simple, low cost, easier debugging
>   - Disadvantage: CPU limited, tidak scalable untuk sensor complex (LiDAR)
> - Acceptable untuk **prototype & learning**, tapi untuk **production**, kami recommend upgrade ke distributed dengan ROS2 untuk path planning yang lebih sophisticated.

---

## 6. KESIMPULAN

### Template Kesimpulan:

1. **Rangkum Desain Arsitektur**
   > Sistem yang kami desain adalah [AGV/Robot Arm/Conveyor] dengan arsitektur [Centralized/Distributed]. Komponen utama meliputi [list komponen]. Communication protocol yang dipilih adalah [Protocol] karena [alasan]. FMEA mengidentifikasi [jumlah] failure modes dengan [jumlah] yang critical.

2. **Apakah Tujuan Praktikum Tercapai?**
   > - [✓] Desain blok diagram sistem → Tercapai, block diagram lengkap dengan 5-layer architecture
   > - [✓] Pin mapping & signal routing → Tercapai, dokumentasi lengkap dengan wiring diagram
   > - [✓] Protocol analysis → Tercapai, comparison table dan recommendation
   > - [✓] FMEA → Tercapai, 12 failure modes dengan mitigation strategy

3. **Lesson Learned**
   > - _____
   > - _____
   > - _____

4. **Suggestion untuk Improvement**
   > - Design: _____
   > - Implementation: _____
   > - Testing: _____

---

## 7. TROUBLESHOOTING UMUM

### Issue 1: Block Diagram Terlalu Kompleks
**Symptom**: Diagram tidak jelas, terlalu banyak detail  
**Solution**: Buat 2 level diagram — high-level (overview) dan detailed (per subsystem)

### Issue 2: Pin Conflict di ESP32
**Symptom**: Code compile OK tapi pin tidak kerja  
**Solution**: Cek datasheet ESP32, avoid pin 6-11 (flash), pin 34-39 (input-only)

### Issue 3: Tidak Tahu Pilih Protocol
**Symptom**: Bingung antara MQTT, Modbus, CAN  
**Solution**: Tentukan latency requirement dulu — jika < 50ms → CAN/Modbus, jika > 100ms → MQTT OK

### Issue 4: FMEA Terlalu Banyak
**Symptom**: Ratusan failure mode, overwhelming  
**Solution**: Focus di **critical failure** (severity 7-9) dulu, ignore low severity (1-3) untuk prototype

### Issue 5: Tidak Punya Hardware Lengkap
**Symptom**: Beberapa sensor tidak tersedia  
**Solution**: Simulasi dengan dummy data atau simplified model — yang penting **konsep desain** benar

---

## 8. REFERENSI

Lihat file [Referensi.md](Referensi.md) untuk daftar lengkap buku, paper, dan resource.

**Quick Links:**
- ESP32 Datasheet: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
- MQTT Protocol: https://mqtt.org/
- Modbus Protocol: https://modbus.org/
- CAN Bus Tutorial: https://www.ti.com/lit/an/sloa101b/sloa101b.pdf

---

**Prepared by:** Tim Penyusun Praktikum Mekatronika & Robotika  
**Last Updated:** 5 Februari 2026  
**Version:** 1.0
