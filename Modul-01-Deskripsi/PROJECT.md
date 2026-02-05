# PROJECT MODUL 1: DESIGN & ANALYZE END-TO-END ROBOTICS SYSTEM

## LATAR BELAKANG

Industri manufaktur modern membutuhkan sistem robotika yang terintegrasi sempurna, dari mekanik, elektronik, kontrol, hingga monitoring. Desain yang tidak tepat akan menyebabkan system failure, safety issue, atau performance suboptimal. Dalam project ini, mahasiswa akan merancang sistem robotika lengkap dengan mempertimbangkan aspek mekanik, elektronik, dan software secara terintegrasi.

## TUJUAN PROJECT

Setelah menyelesaikan project ini, mahasiswa diharapkan dapat:
- Merancang arsitektur sistem robotika lengkap (mechanical, electrical, software)
- Membuat detailed blok diagram, wiring diagram, dan signal flow
- Melakukan FMEA (Failure Mode & Effects Analysis) untuk identifikasi dan mitigasi risiko kegagalan
- Membuat implementation plan yang komprehensif (BOM, timeline, responsibility matrix)
- Mengintegrasikan multiple subsystem menjadi satu sistem yang bekerja secara kohesif

## SPESIFIKASI SISTEM

**PILIH SALAH SATU SISTEM BERIKUT:**

### A. AGV Mini untuk Warehouse Automation

**Requirement:**
- Navigasi antar station dengan akurasi ±5 cm
- Obstacle avoidance dengan response time < 500ms
- Autonomous docking di charging station
- Payload capacity: 2-5 kg
- Battery life: minimum 2 jam continuous operation

**Sensor:**
- LiDAR atau Ultrasonic sensor array (untuk navigation & obstacle detection)
- Rotary encoder pada setiap motor (untuk odometry)
- Bumper switch atau contact sensor (untuk safety)
- Current sensor (untuk battery monitoring)
- IMU (Inertial Measurement Unit) untuk orientation

**Actuator:**
- 2 motor DC dengan gearbox (differential drive)
- Optional: Linear actuator untuk gripper atau lift mechanism

**Control:**
- Real-time navigation menggunakan ROS2 (path planning, obstacle avoidance)
- Local motion control di ESP32 (motor PID control, odometry)
- State machine untuk autonomous operation

**Monitoring:**
- Web dashboard showing:
  - Robot position & trajectory (real-time map)
  - Battery status & remaining runtime
  - Sensor readings (distance, speed, current)
  - System status (idle/moving/charging/error)

---

### B. Robot Arm 3-DOF untuk Pick & Place

**Requirement:**
- Workspace: radius 30-50 cm, height 20-40 cm
- Positioning accuracy: ±1 cm
- Payload: 100-500 gram
- Cycle time: < 10 detik per pick-place operation
- Repeatability: ±2 mm

**Actuator:**
- 3 stepper motor atau servo motor untuk joint control
- 1 servo gripper (atau pneumatic gripper)

**Sensor:**
- Rotary encoder pada setiap joint (untuk position feedback)
- Force sensor pada gripper (optional, untuk grip detection)
- Limit switch pada setiap joint (untuk safety & homing)
- Vision sensor atau camera (optional, untuk object detection)

**Control:**
- Forward Kinematics (FK) solver untuk position calculation
- Inverse Kinematics (IK) solver untuk target reaching
- Trajectory planning (point-to-point atau smooth path)
- PID control untuk setiap joint

**Monitoring:**
- Web dashboard showing:
  - Joint angles (actual vs target)
  - End-effector position (X, Y, Z coordinates)
  - Gripper status (open/close, force)
  - Error metrics (position error, velocity)

---

### C. Conveyor + Sorting System

**Requirement:**
- Conveyor speed: 0.1 - 0.5 m/s (variable)
- Sorting rate: minimum 10 parts per minute
- Sorting accuracy: > 95%
- Part detection response time: < 100ms
- Support 2-3 kategori sorting (by color, size, atau material)

**Actuator:**
- DC motor untuk conveyor drive dengan speed control
- Pneumatic solenoid atau servo actuator untuk diverter mechanism (2-3 jalur)

**Sensor:**
- Photoelectric sensor atau proximity sensor (untuk part detection)
- Color sensor atau vision camera (untuk classification)
- Encoder pada conveyor motor (untuk speed & position)
- Sensor konfirmasi di setiap jalur sorting (untuk counting)

**Control:**
- Sequencing logic (PLC-style programming)
- Part tracking algorithm (predict position based on speed)
- Timing control untuk actuate diverter
- Counting & statistics

**Monitoring:**
- Web dashboard showing:
  - Production count per kategori
  - Sorting accuracy (correct vs incorrect)
  - System status & alarm
  - Real-time conveyor speed & throughput

---

## HARDWARE YANG DIGUNAKAN

### Mechanical:
- Mini robot chassis (commercial atau custom design di Fusion 360)
- Motor dengan gearbox (sesuai pilihan sistem)
- Structure frame (aluminum extrusion, 3D printed parts, atau acrylic)
- Mounting bracket & hardware (bolt, nut, bearing jika perlu)

### Electronic:
- **Mikrokontroler:** Arduino Mega 2560 atau ESP32 DevKit
- **Motor Driver:** L298N (DC motor), A4988/DRV8825 (stepper), atau servo driver
- **Sensor:** Sesuai pilihan sistem (ultrasonic, encoder, color sensor, dll)
- **Power Supply:** 24V/12V untuk motor, 5V untuk logic
- **Protection:** Fuse, diode flyback, voltage regulator
- **Wiring:** Kabel, connector, terminal block
- **PCB/Breadboard:** Untuk prototyping

### Computer:
- **Ubuntu PC** untuk edge compute (optional, untuk full integration dengan ROS2)
- Minimum spec: 4GB RAM, dual-core processor

---

## SOFTWARE YANG DIGUNAKAN

### Design Tools:
- **Fusion 360:** Mechanical design, assembly, simulation
- **EasyEDA:** Electrical schematic & PCB design
- **Draw.io / Visio:** Block diagram, flowchart

### Embedded Development:
- **PlatformIO (VS Code extension):** Embedded C/C++ development
- **Arduino IDE (alternative):** Jika tidak familiar dengan PlatformIO

### High-Level Control (Optional):
- **ROS2 Humble:** Robot Operating System untuk high-level control
- **Python 3.10+:** Data analysis, automation script
- **Node.js / Flask:** Web server untuk monitoring

### Version Control & Documentation:
- **Git / GitHub:** Version control, collaboration
- **Markdown:** Documentation

---

## INSTRUKSI IMPLEMENTASI

### FASE 1: REQUIREMENT & ANALYSIS (Minggu 1)

**Target Output:**
- System Requirement Specification (SRS) document

**Langkah Kerja:**

1. **Define Detailed Requirement**
   - Functional requirement: Apa yang harus bisa dilakukan sistem?
     - Input: user command, sensor data
     - Process: control algorithm, decision logic
     - Output: motor movement, display data
   - Non-functional requirement:
     - Performance: speed, accuracy, response time
     - Reliability: uptime, error rate
     - Safety: emergency stop, fail-safe behavior
     - Usability: interface design, ease of operation

2. **Identify Constraint & Assumption**
   - Budget constraint: berapa maksimal cost?
   - Technical constraint: tools available, skill limitation
   - Time constraint: berapa lama development?
   - Assumption: e.g., "assume power supply always stable", "assume flat surface operation"

3. **Define Success Criteria (Measurable)**
   - Contoh: "Robot dapat navigasi dari Point A ke Point B dengan error < 5cm dalam 90% percobaan"
   - Contoh: "Robot arm dapat pick & place object dengan success rate > 85%"
   - Contoh: "Sorting system dapat classify dengan akurasi > 95%"

4. **Create Use Case Scenario**
   - Scenario 1: Normal operation
   - Scenario 2: Error handling (sensor failure, motor stuck)
   - Scenario 3: Emergency stop
   - Scenario 4: Recovery after fault

**Deliverable:**
- SRS document (3-5 halaman) dalam format Markdown atau PDF
- Use case diagram

---

### FASE 2: DESIGN (Minggu 2)

**Target Output:**
- Complete design documentation (CAD, schematic, architecture diagram)

**Langkah Kerja:**

1. **Design Blok Diagram (Functional Architecture)**
   - Identifikasi semua subsystem:
     - Sensor subsystem
     - Controller subsystem
     - Actuator subsystem
     - Communication subsystem
     - Power subsystem
   - Define interface antar subsystem:
     - Protocol: I2C, SPI, UART, PWM, GPIO
     - Data rate: berapa Hz update rate?
     - Signal type: digital, analog, PWM
   - Tools: Draw.io, Visio, atau hand-drawn dengan annotation

2. **Design Electrical Architecture**
   - Power tree diagram:
     - Input power: 24V dari adaptor
     - Regulated output: 12V untuk motor, 5V untuk logic, 3.3V untuk sensor
   - Signal routing:
     - Sensor signal → Mikrokontroler input pin
     - Mikrokontroler output → Motor driver input
     - Motor driver output → Motor
   - Protection design:
     - Fuse di input power
     - Flyback diode di motor coil
     - TVS diode untuk spike protection (optional)
   - Tools: EasyEDA atau schematic paper

3. **Design Mechanical Layout**
   - CAD design di Fusion 360:
     - 3D model semua komponen (motor, sensor, chassis)
     - Assembly dengan constraint (mate, revolute, slider)
     - Motion study untuk verify workspace & collision
   - Engineering drawing:
     - Dimensioning critical parts
     - Tolerance specification (jika machining custom part)
   - Bill of Materials (BOM) dari Fusion 360

4. **Create Wiring Diagram**
   - Detail connection:
     - Pin 1 mikrokontroler → Component X pin Y
     - Wire color coding (merah=power, hitam=ground, biru=signal, dll)
     - Connector type & part number
   - Pin mapping table (sudah dibuat di Jobsheet, bisa refined)

**Deliverable:**
- Block diagram (PNG/PDF dengan annotation)
- Electrical schematic (EasyEDA file + exported PDF)
- Mechanical CAD (Fusion 360 file + exported STEP/STL)
- Engineering drawing (PDF dengan dimensi)
- Wiring diagram (PNG/PDF)
- BOM (Excel/CSV dengan part number, qty, estimated price)

---

### FASE 3: FAILURE ANALYSIS (Minggu 2)

**Target Output:**
- FMEA report dengan mitigation strategy

**Langkah Kerja:**

1. **Identifikasi Semua Potential Failure Point**
   - Brainstorm dengan team: apa yang bisa salah?
   - Kategorikan failure:
     - **Mechanical failure:** bearing fail, belt slip, gear tooth break, structure crack
     - **Electrical failure:** power loss, wire break, short circuit, driver overheat
     - **Software failure:** firmware crash, communication timeout, sensor glitch, wrong algorithm

2. **Create FMEA Table**
   
   Untuk setiap failure mode, isi:
   
   | Failure Mode | Component | Effect | Cause | Detection Method | Mitigation Strategy | Severity (1-10) | Occurrence (1-10) | Detection (1-10) | RPN |
   |--------------|-----------|--------|-------|------------------|---------------------|-----------------|-------------------|------------------|-----|
   | Motor not rotating | Motor + Driver | Robot stuck, cannot move | Power loss, driver fault, motor burn | Current sensor, watchdog timer | Add fuse, current limiting, watchdog reset | 8 | 3 | 2 | 48 |
   | Encoder signal loss | Encoder | Wrong position estimate | Cable disconnect, sensor damage | Timeout detection, signal validation | Use last known position, reduce speed, alarm | 6 | 4 | 3 | 72 |
   | ... | ... | ... | ... | ... | ... | ... | ... | ... | ... |
   
   **Risk Priority Number (RPN) = Severity × Occurrence × Detection**
   - RPN > 100: Critical, need immediate mitigation
   - RPN 50-100: High, need mitigation plan
   - RPN < 50: Medium-Low, monitor

3. **Severity Assessment**
   - **1-3 (Low):** Minor inconvenience, easy recovery
   - **4-6 (Medium):** Performance degradation, need manual intervention
   - **7-9 (High):** System stop, potential safety issue
   - **10 (Critical):** Safety hazard, injury possible

4. **Mitigation Strategy untuk Failure Critical**
   - Hardware mitigation: redundancy, protection device
   - Software mitigation: timeout detection, fallback algorithm, watchdog
   - Procedural mitigation: SOP, training, regular maintenance

5. **Create Contingency Plan**
   - Jika failure X terjadi, langkah recovery:
     1. Detect failure (sensor/alarm)
     2. Safe stop (e-stop atau controlled stop)
     3. Alert user (LED, buzzer, web notification)
     4. Attempt auto-recovery (jika possible)
     5. If recovery fail → manual intervention required

**Deliverable:**
- FMEA table (Excel/CSV) dengan minimum 15 failure modes
- Risk matrix graph (Severity vs Occurrence)
- Mitigation plan document (1-2 halaman)
- Contingency flowchart

---

### FASE 4: IMPLEMENTATION PLAN (Minggu 3)

**Target Output:**
- Detailed implementation plan (timeline, BOM, responsibility)

**Langkah Kerja:**

1. **Create Detailed Bill of Materials (BOM)**
   
   | No | Part Name | Part Number / Spec | Qty | Unit Price | Total Price | Supplier | Lead Time | Notes |
   |----|-----------|-------------------|-----|------------|-------------|----------|-----------|-------|
   | 1 | DC Motor with Encoder | JGA25-370 12V 178RPM | 2 | Rp 150.000 | Rp 300.000 | Tokopedia | 3 days | Confirm voltage & torque |
   | 2 | Motor Driver | L298N Dual H-Bridge | 1 | Rp 25.000 | Rp 25.000 | Local store | 1 day | Max 2A per channel |
   | ... | ... | ... | ... | ... | ... | ... | ... | ... |
   | **TOTAL** | | | | | **Rp X.XXX.XXX** | | | |

2. **Create Project Timeline (Gantt Chart)**
   
   Contoh timeline:
   - **Week 1:** Requirement gathering, design review
   - **Week 2:** Procurement, CAD finalization
   - **Week 3:** Mechanical assembly, PCB soldering
   - **Week 4:** Firmware development, testing individual subsystem
   - **Week 5:** Integration, system testing, debugging
   - **Week 6:** Performance tuning, documentation, video recording
   
   Tools: Excel, Microsoft Project, atau online Gantt chart tool

3. **Define Responsibility Matrix (RACI)**
   
   | Task | Person A | Person B | Person C | Person D |
   |------|----------|----------|----------|----------|
   | Mechanical design | R | A | C | I |
   | Electrical schematic | A | R | C | I |
   | Firmware coding | C | C | R | A |
   | Testing | I | A | R | R |
   | Documentation | A | C | I | R |
   
   Legend:
   - **R = Responsible** (yang mengerjakan)
   - **A = Accountable** (yang bertanggung jawab final)
   - **C = Consulted** (yang dimintai input)
   - **I = Informed** (yang perlu tahu progress)

4. **Risk Assessment**
   
   | Risk | Impact (H/M/L) | Probability (H/M/L) | Mitigation Plan |
   |------|----------------|---------------------|------------------|
   | Component out of stock | H | M | Prepare alternative supplier, order early |
   | Technical skill gap | M | M | Study tutorial, ask mentor, prototype simple version first |
   | Timeline delay | M | H | Add buffer time, parallel work where possible |

**Deliverable:**
- BOM with total cost estimate
- Gantt chart (timeline with milestones)
- RACI matrix
- Risk assessment table

---

### FASE 5: PROTOTYPE & TEST (Minggu 3-4)

**Target Output:**
- Working prototype dengan test results

**Langkah Kerja:**

1. **Build Prototype**
   - Mechanical assembly:
     - Assemble chassis, motor mounting
     - Verify alignment, check for wobble atau binding
     - Cable routing & management
   - Electrical wiring:
     - Follow wiring diagram strictly
     - Double-check polarity (power, motor, sensor)
     - Use multimeter untuk verify voltage di setiap node
   - Software development:
     - Code structure: modular (sensor.h, motor.h, control.h)
     - Config file untuk easy parameter tuning
     - Debug output via serial monitor

2. **Test Individual Subsystem**
   
   **Test Sensor:**
   - Verify sensor reading accuracy (compare dengan manual measurement)
   - Test response time (berapa ms delay?)
   - Test di berbagai kondisi (normal, extreme, noisy environment)
   
   **Test Actuator:**
   - Verify motor direction (CW/CCW correct?)
   - Verify speed control (PWM → RPM linear?)
   - Verify torque (dapat menggerakkan beban target?)
   
   **Test Communication:**
   - Verify data transmission (ESP32 ↔ Ubuntu, ESP32 ↔ Web)
   - Test latency (berapa ms round-trip?)
   - Test reliability (packet loss rate?)

3. **Integration Test**
   - Connect semua subsystem together
   - Test end-to-end operation:
     - User command → Controller → Actuator → Feedback → Display
   - Test scenario:
     - Normal operation (happy path)
     - Error scenario (sensor disconnect, motor stuck)
     - Edge case (max load, min/max speed)

4. **Performance Measurement**
   
   Bandingkan actual vs specification:
   
   | Parameter | Specification | Actual Result | Pass/Fail | Notes |
   |-----------|---------------|---------------|-----------|-------|
   | Positioning accuracy | ±5 cm | ±3.2 cm (average) | **PASS** | Tested 20 runs |
   | Response time | < 500 ms | 320 ms (average) | **PASS** | E-stop to full stop |
   | Success rate | > 85% | 92% | **PASS** | 23 success / 25 trials |
   | Battery life | > 2 hours | 2.5 hours | **PASS** | Continuous operation |

**Deliverable:**
- Assembled prototype (with photos from multiple angles)
- Test procedure document (step-by-step test scenario)
- Test results (data table, graph, screenshot)
- Performance comparison table (spec vs actual)
- Video demo (5-10 menit showing system operation)

---

### FASE 6: DOCUMENTATION & PRESENTATION (Minggu 4)

**Target Output:**
- Final technical report, presentation, video

**Langkah Kerja:**

1. **Create Final Technical Report**
   
   Structure:
   - **Executive Summary (1 halaman)**
     - Project overview
     - Key achievement
     - Conclusion
   - **Introduction (1-2 halaman)**
     - Background
     - Objective
     - Scope
   - **System Design (5-10 halaman)**
     - Requirement analysis
     - Architecture design (block diagram, schematic, CAD)
     - Component selection & justification
   - **Implementation (3-5 halaman)**
     - Build process
     - Challenges & solutions
   - **Testing & Results (3-5 halaman)**
     - Test procedure
     - Performance measurement
     - Analysis
   - **Failure Analysis (2-3 halaman)**
     - FMEA summary
     - Mitigation implementation
   - **Conclusion & Recommendation (1-2 halaman)**
     - Lesson learned
     - Future improvement
   - **Appendix**
     - Code listing
     - Detailed BOM
     - Test data

2. **Organize Source Code di GitHub**
   
   Repository structure:
   ```
   project-name/
   ├── README.md (overview, how to run, dependencies)
   ├── docs/
   │   ├── design/
   │   │   ├── block_diagram.png
   │   │   ├── schematic.pdf
   │   │   └── CAD_model.step
   │   ├── test_results/
   │   └── fmea_report.xlsx
   ├── firmware/
   │   ├── platformio.ini
   │   ├── src/
   │   │   ├── main.cpp
   │   │   ├── config.h
   │   │   ├── sensor.h/cpp
   │   │   ├── motor.h/cpp
   │   │   └── control.h/cpp
   │   └── lib/
   ├── ros2_ws/ (optional)
   │   └── src/
   │       └── robot_control/
   ├── web_dashboard/ (optional)
   │   ├── app.js
   │   ├── index.html
   │   └── package.json
   └── data/
       ├── test_results.csv
       └── performance_log.json
   ```

3. **Create Video Demo**
   
   Script outline (15-20 menit):
   - **[00:00-02:00] Intro**
     - Team introduction
     - Project title & objective
   - **[02:00-07:00] System Overview**
     - Show block diagram
     - Explain architecture (sensor → controller → actuator)
     - Show hardware components
   - **[07:00-12:00] Demo**
     - Show system operation (live demo)
     - Explain what's happening (code flow, signal routing)
     - Show monitoring dashboard
   - **[12:00-15:00] Analysis**
     - Performance result
     - Comparison spec vs actual
     - Challenges & solutions
   - **[15:00-17:00] Conclusion**
     - Lesson learned
     - Future work
   - **[17:00-18:00] Q&A / Closing**

4. **Create Presentation Slide**
   
   Slide outline (20-25 slides):
   1. Title slide
   2. Team & agenda
   3. Background & motivation
   4. Objective & scope
   5-7. System requirement
   8-12. Design (diagram, CAD, schematic)
   13-15. Implementation (build process, challenges)
   16-18. Testing & results
   19. FMEA summary
   20. Conclusion
   21-22. Demo video (embedded atau link)
   23. Lesson learned
   24. Q&A
   25. Thank you

**Deliverable:**
- Final technical report (PDF, 15-30 halaman)
- GitHub repository (organized, dengan README lengkap)
- Video demo (MP4, 15-20 menit, min 720p)
- Presentation slide (PowerPoint/PDF)
- Poster (optional, A3 size, summary project)

---

## DELIVERABLE SUMMARY

### Mandatory Deliverables:

1. **System Requirement Specification (SRS)** — PDF/Markdown
2. **Technical Design Document** — CAD files, schematic, architecture diagram
3. **FMEA Report** — Excel/CSV dengan mitigation strategy
4. **Implementation Plan** — BOM, Gantt chart, RACI matrix
5. **Source Code** — GitHub repository (organized, documented)
6. **Test Report** — Performance measurement, comparison table
7. **Video Demo** — 15-20 menit, show hardware & software
8. **Presentation Slide** — PowerPoint/PDF
9. **Final Technical Report** — Comprehensive documentation (15-30 halaman)
10. **Lessons Learned Document** — Reflection & recommendation

### Optional (Bonus):

11. **Prototype PCB** — Custom PCB design (jika ada waktu & budget)
12. **Advanced Analysis** — Cost-benefit analysis, monte carlo simulation untuk reliability
13. **Full Integration** — ROS2 + Web dashboard fully functional
14. **Innovation** — Fitur tambahan beyond basic requirement

---

## KRITERIA PENILAIAN

### Total: 100%

| Aspek | Bobot | Deskripsi |
|-------|-------|-----------|
| **1. Desain Mekanik** | 15% | Assembly lengkap dengan constraint, motion study accurate, engineering drawing, foto assembly profesional |
| **2. Desain Elektronik** | 15% | Schematic lengkap & jelas, PCB layout optimal, protection device, BOM & foto implementasi |
| **3. Software Embedded** | 20% | Code modular & readable, algoritma correct, safety system robust, compile clean |
| **4. Software PC (ROS2, Web)** | 15% | Architecture well-designed, web dashboard functional, data logging comprehensive |
| **5. Integrasi Sistem** | 15% | Semua layer terintegrasi seamless, komunikasi reliable, latency acceptable, fault recovery |
| **6. Performa Sistem** | 10% | Memenuhi spesifikasi (akurasi, speed, repeatability), data kuantitatif lengkap |
| **7. Laporan & Dokumentasi** | 10% | Terstruktur, technical content jelas, code documentation lengkap, GitHub organized |

### Bonus Points (Maximum +10%):

- Innovative design atau fitur tambahan: **+5%**
- Advanced analysis (cost-benefit, simulation): **+3%**
- Full integration dengan ROS2 & Web dashboard: **+5%**
- Prototype working demo dengan performance exceptional: **+5%**

### Penilaian Grade:

| Range | Grade |
|-------|-------|
| 91-100+ | A |
| 81-90 | AB |
| 71-80 | B |
| 61-70 | BC |
| 51-60 | C |
| < 50 | D/E |

---

## TIMELINE REKOMENDASI

| Minggu | Aktivitas Utama | Deliverable |
|--------|-----------------|-------------|
| **1** | Requirement gathering, analysis, team meeting | SRS document |
| **2** | Design (CAD, schematic, architecture) | Design document, FMEA |
| **3** | Procurement, assembly, firmware development | Implementation plan, prototype v1 |
| **4** | Integration, testing, debugging | Test report, working prototype |
| **5** | Performance tuning, optimization | Performance data |
| **6** | Documentation, video recording, presentation prep | Final report, video, slide |

---

## TIPS & BEST PRACTICES

### Design Phase:
✓ **Start simple, iterate:** Build minimum viable product (MVP) first, add features gradually  
✓ **Document decisions:** Why choose component X over Y? Document reasoning  
✓ **Review early, review often:** Get feedback dari instruktur/mentor sebelum terlalu jauh  

### Implementation Phase:
✓ **Test as you build:** Jangan tunggu semua selesai baru test, test setiap subsystem  
✓ **Version control:** Commit to Git frequently, meaningful commit message  
✓ **Backup everything:** Code, data, design files — backup di cloud  

### Testing Phase:
✓ **Systematic testing:** Buat test plan, checklist setiap test scenario  
✓ **Record everything:** Video, screenshot, data log — semua bisa jadi evidence  
✓ **Analyze failure:** Jangan cuma note "gagal", analyze why & how to fix  

### Documentation Phase:
✓ **Write as you go:** Jangan tunggu akhir, document progress setiap minggu  
✓ **Use version control for docs too:** Track changes di report & slide  
✓ **Professional presentation:** Format consistent, grammar check, visual clear  

---

## CONTOH PROJECT SUKSES (REFERENCE)

### AGV Navigation Example:
- **Navigation:** Dead reckoning (encoder odometry) + LiDAR correction
- **Control:** PID untuk motor speed, pure pursuit untuk path following
- **Safety:** E-stop, bumper sensor, software boundary
- **Monitor:** Web showing map, robot position, battery level

### Robot Arm Example:
- **Kinematics:** DH parameter untuk FK, analytical/numerical IK solver
- **Control:** PID per joint, trajectory interpolation (linear/cubic spline)
- **Safety:** Joint limit (software & hardware), force limiting (current sense)
- **Monitor:** Joint angle graph, end-effector position plot

### Conveyor Sorting Example:
- **Detection:** Color sensor + timing algorithm untuk predict position
- **Control:** State machine (wait → detect → track → sort → count)
- **Safety:** E-stop, mechanical guard, overcurrent protection
- **Monitor:** Production count, sorting accuracy, reject rate

---

## SUMBER REFERENSI

### Books:
- W. Bolton — *Mechatronics: Electronic Control Systems* (7th ed.)
- David G. Alciatore — *Introduction to Mechatronics and Measurement Systems* (5th ed.)

### Online Resources:
- ROS2 Documentation: https://docs.ros.org/en/humble/
- Fusion 360 Tutorials: https://www.autodesk.com/products/fusion-360/learn
- EasyEDA Learning Center: https://docs.easyeda.com/
- PlatformIO Documentation: https://docs.platformio.org/

### Community:
- ROS Discourse: https://discourse.ros.org/
- Arduino Forum: https://forum.arduino.cc/
- Stack Overflow (for coding questions)

---

## CONTACT & SUPPORT

**Instruktur Modul 1:**
- Email: [instruktur email]
- Office hour: [waktu]
- Discord/Slack: [channel link]

**GitHub Classroom:**
- Repository: [repo link]
- Issues & Discussion: [link]

**Lab Access:**
- Schedule: [waktu lab buka]
- Equipment borrowing: [prosedur]

---

## CATATAN PENTING

⚠️ **Plagiarism Policy:**
- Code, design, documentation harus karya sendiri
- Boleh reference tutorial/library (dengan proper citation)
- Copy-paste tanpa understanding akan ketahuan saat presentation

⚠️ **Safety:**
- Gunakan safety glasses saat soldering atau machining
- Power off sebelum wiring/modifikasi hardware
- Verify polarity sebelum power on

⚠️ **Deadline:**
- Final submission: [tanggal]
- Late submission: penalty -5% per hari
- No submission = 0 (tidak ada toleransi)

---

**GOOD LUCK & HAVE FUN BUILDING! 🚀🤖**

*Remember: The goal is not just to finish the project, but to learn the process of designing & building a real robotics system from scratch.*
