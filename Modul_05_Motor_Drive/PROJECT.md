# PROJECT MODULE 5
## Motor Drive System Design & Optimization

**Durasi:** 2 minggu  
**Bobot:** 30% dari nilai Modul 5

---

## DESKRIPSI PROJECT

Mahasiswa merancang dan mengimplementasikan sistem motor drive untuk aplikasi nyata dengan fokus pada efisiensi energi, reliability, dan performance optimization. Project ini mengintegrasikan semua konsep dari Jobsheet (PWM control, current sensing, characterization, closed-loop control) ke dalam satu sistem fungsional.

---

## PILIHAN APLIKASI

Pilih **salah satu** aplikasi berikut:

### Opsi A: Conveyor Speed Synchronization System
**Skenario:** Sistem conveyor dual-motor untuk material handling dengan synchronization presisi.

**Requirements:**
- 2 motor DC dengan encoder
- Speed synchronization error < 2%
- Load balancing otomatis
- Total throughput: 1 meter/detik
- Variable speed control via user interface

**Deliverables:**
- Speed sync algorithm (master-slave atau distributed)
- Load detection via current sensing
- Automatic calibration procedure
- Real-time monitoring dashboard

---

### Opsi B: AGV (Automated Guided Vehicle) Drive System
**Skenario:** Differential drive system untuk mobile robot dengan trajectory tracking.

**Requirements:**
- 2 motor DC dengan encoder (left & right wheel)
- Straight-line deviation < 5 cm over 2 meters
- 90° turn accuracy < 3°
- Maximum speed: 0.5 m/s
- Obstacle stop reaction time < 500 ms

**Deliverables:**
- Differential drive kinematics implementation
- Odometry calculation
- Velocity profiling (trapezoidal)
- Emergency stop system

---

### Opsi C: Precision Positioning System (Linear Actuator)
**Skenario:** Motor-driven linear actuator untuk pick-and-place application.

**Requirements:**
- Motor DC dengan encoder + ballscrew/leadscrew
- Positioning accuracy: ±0.5 mm
- Repeatability: ±0.2 mm
- Homing procedure dengan limit switch
- Maximum travel: 200 mm

**Deliverables:**
- Point-to-point motion control
- Homing algorithm
- Soft-start & soft-stop (jerk limiting)
- Position error compensation

---

## MILESTONE & TIMELINE

### Week 1: Design & Characterization
**Tasks:**
1. **System Design (2 hari)**
   - Pilih aplikasi
   - Buat block diagram sistem
   - Tentukan spesifikasi motor & driver
   - Design rangkaian lengkap (schematic)

2. **Motor Characterization (2 hari)**
   - Ukur torque-speed curve untuk motor yang dipilih
   - Determine motor constants (Kt, Ke, Ra, La)
   - Efficiency mapping
   - Thermal profiling (temperature vs load vs time)

3. **Driver Selection & Testing (1 hari)**
   - Evaluasi driver options (L298N vs DRV8833 vs TB6612)
   - Efficiency comparison
   - Thermal performance test
   - EMI/noise analysis

**Deliverable Week 1:**
- Design document (PDF, 5-10 halaman):
  - Requirements specification
  - Hardware selection justification
  - Schematic diagram
  - Motor characterization report (grafik + tabel)
- Code repository (initial commit):
  - Motor testing code
  - Characterization data (CSV)

---

### Week 2: Implementation & Optimization
**Tasks:**
4. **Firmware Development (3 hari)**
   - Core control loop implementation
   - Sensor integration (encoder, current, limit switch)
   - Safety & fault handling
   - User interface (serial command / web dashboard)

5. **Tuning & Optimization (2 hari)**
   - Controller tuning (P-controller atau preview PID)
   - Slew rate optimization
   - Current limiting calibration
   - Energy efficiency optimization

6. **Testing & Validation (2 hari)**
   - Performance testing vs requirements
   - Stress testing (continuous operation, overload, thermal)
   - Failure mode testing (encoder disconnect, overvoltage, stall)
   - Field test di aplikasi target

**Deliverable Week 2:**
- Firmware source code (complete & documented)
- Test report (PDF):
  - Performance metrics vs requirements
  - Test scenarios & results
  - Failure mode analysis
- Demo video (3-5 menit)
- Presentation slides (15 menit presentasi)

---

## REQUIREMENTS DETAIL

### 1. Hardware Requirements

#### Minimum Components:
- [x] ESP32 DevKit / Arduino Mega
- [x] 1-2 motor DC dengan encoder (sesuai aplikasi)
- [x] Motor driver (minimal dual H-bridge untuk 2 motor)
- [x] Current sensor (INA219 atau shunt resistor + op-amp)
- [x] Power supply 12V ≥3A
- [x] Limit switch / photoelectric sensor (untuk homing, optional)
- [x] LED indicators (status, fault, power)
- [x] Emergency stop button

#### Recommended Additions:
- [ ] LCD display untuk monitoring lokal
- [ ] Buzzer untuk alarm
- [ ] Voltage regulator 5V untuk logic
- [ ] Fuse / circuit breaker untuk proteksi

---

### 2. Firmware Requirements

#### Core Features (Mandatory):
```cpp
// 1. Motor control dengan direction & speed
void setMotorSpeed(int motor_id, int speed);  // speed: -255 to +255

// 2. Encoder reading dengan interrupt
long getEncoderPosition(int motor_id);
float getMotorRPM(int motor_id);

// 3. Current monitoring
float getMotorCurrent(int motor_id);

// 4. Safety features
void emergencyStop();
void overcurrentProtection();
void stallDetection();

// 5. Closed-loop control (minimal P-controller)
void speedController(float setpoint_rpm, int motor_id);
```

#### Advanced Features (Bonus):
- [ ] PID controller dengan anti-windup (preview Modul 6)
- [ ] Trajectory planning (trapezoidal / S-curve)
- [ ] Fault diagnostics (error codes)
- [ ] Data logging ke SD card
- [ ] Real-time plotting via Serial Plotter / web dashboard

---

### 3. Performance Metrics

Setiap aplikasi harus memenuhi target berikut:

| Metric                     | Opsi A (Conveyor) | Opsi B (AGV)   | Opsi C (Actuator) |
|----------------------------|-------------------|----------------|-------------------|
| **Accuracy**               | Sync error <2%    | Deviation <5cm | Position ±0.5mm   |
| **Response Time**          | Load change <1s   | Stop <500ms    | Settle <2s        |
| **Efficiency**             | >75%              | >70%           | >80%              |
| **Reliability (MTBF)**     | >8 hours          | >4 hours       | >1000 cycles      |
| **Thermal Stability**      | ΔT <20°C/hour     | ΔT <15°C/hour  | ΔT <10°C/hour     |

**Measurement Procedure:**
- Jalankan sistem continuous selama minimal 30 menit
- Log semua metrics setiap 1 detik
- Hitung mean, std dev, min, max untuk setiap metric
- Bandingkan dengan target

---

## RUBRIK PENILAIAN

### A. Design & Planning (20%)

| Kriteria                          | Excellent (18-20) | Good (15-17) | Adequate (12-14) | Poor (<12) |
|-----------------------------------|-------------------|--------------|------------------|------------|
| **Requirements Analysis**         | Komprehensif, spesifik, measurable | Jelas, sebagian besar measurable | Basic, kurang detail | Tidak jelas |
| **Hardware Selection Justification** | Data-driven, comparison matrix | Reasonable rationale | Minimal justification | Arbitrary |
| **Schematic Quality**             | Professional, complete, no errors | Clear, minor errors | Basic, beberapa missing | Incomplete |

---

### B. Motor Characterization (15%)

| Kriteria                   | Excellent (14-15) | Good (12-13) | Adequate (10-11) | Poor (<10) |
|----------------------------|-------------------|--------------|------------------|------------|
| **Data Quality**           | Akurat, dense sampling, multiple trials | Good accuracy | Basic dataset | Sparse/inaccurate |
| **Curve Fitting**          | Linear regression + R², confidence interval | Linear fit | Manual plot | No fitting |
| **Analysis Depth**         | Motor constants derived, efficiency map | Basic analysis | Minimal interpretation | No analysis |

---

### C. Firmware Implementation (30%)

| Kriteria                     | Excellent (27-30) | Good (24-26) | Adequate (20-23) | Poor (<20) |
|------------------------------|-------------------|--------------|------------------|------------|
| **Code Quality**             | Modular, well-commented, consistent style | Readable, some comments | Basic structure | Messy |
| **Functionality**            | All features work flawlessly | Minor bugs | Limited functionality | Major bugs |
| **Safety Features**          | Comprehensive protection, tested | Basic protection | Minimal safety | No safety |
| **Error Handling**           | Robust, informative messages | Basic handling | Minimal handling | No handling |

---

### D. Performance & Testing (20%)

| Kriteria                    | Excellent (18-20) | Good (15-17) | Adequate (12-14) | Poor (<12) |
|-----------------------------|-------------------|--------------|------------------|------------|
| **Meets Specifications**    | Exceed all targets | Meet all targets | Meet most targets | Miss critical targets |
| **Test Coverage**           | Comprehensive, edge cases | Good coverage | Basic tests | Minimal testing |
| **Stress Testing**          | Thermal, overload, failure modes | Some stress tests | Nominal only | No stress test |

---

### E. Documentation & Presentation (15%)

| Kriteria                   | Excellent (14-15) | Good (12-13) | Adequate (10-11) | Poor (<10) |
|----------------------------|-------------------|--------------|------------------|------------|
| **Technical Report**       | Publication-quality, reproducible | Clear, complete | Basic report | Incomplete |
| **Video Demo**             | Professional, informative, <5min | Clear demonstration | Basic recording | Unclear |
| **Oral Presentation**      | Engaging, Q&A excellent | Clear delivery | Adequate | Poor communication |

---

### Bonus Points (+15% maksimum)

- **+5%:** Implementasi PID controller dengan tuning sistematis (Ziegler-Nichols)
- **+5%:** Real-time web dashboard (WebSocket / MQTT) dengan plotting
- **+3%:** Energy efficiency optimization (regenerative braking, sleep mode)
- **+2%:** Advanced trajectory planning (S-curve acceleration)

---

## SUBMISSION FORMAT

### 1. Git Repository Structure
```
Project_Motor_Drive_<NamaAnda>/
├── README.md                 # Panduan quick start
├── docs/
│   ├── design_document.pdf   # Week 1 deliverable
│   ├── test_report.pdf       # Week 2 deliverable
│   ├── presentation.pptx
│   └── schematic.png
├── firmware/
│   ├── platformio.ini
│   ├── src/
│   │   ├── main.cpp
│   │   ├── motor_control.h
│   │   ├── encoder.h
│   │   ├── current_sensor.h
│   │   └── safety.h
│   └── lib/
├── characterization/
│   ├── torque_speed_data.csv
│   ├── efficiency_data.csv
│   └── plot_curves.py
├── test_data/
│   ├── performance_log.csv
│   ├── stress_test_log.csv
│   └── thermal_profile.csv
└── video/
    └── demo.mp4
```

### 2. README.md Template
```markdown
# Motor Drive System - [Aplikasi yang Dipilih]

## Quick Start
1. Hardware setup: [Deskripsi singkat]
2. Firmware upload: `pio run -t upload`
3. Run: [Prosedur pengoperasian]

## Specifications
- Motor: [Model, voltage, RPM, torque]
- Driver: [IC, max current]
- Controller: ESP32 / Arduino Mega
- Performance: [List key metrics achieved]

## Features
- [x] Closed-loop speed control
- [x] Current monitoring & protection
- [x] [Feature spesifik aplikasi]

## Test Results
- Accuracy: ±X mm / X%
- Efficiency: X%
- MTBF: X hours

## Video Demo
[Link ke YouTube / file lokal]

## Authors
[Nama] - [NIM] - [Email]
```

---

## CONTOH INSPIRASI

### Example 1: Conveyor Sync System
**Highlights:**
- Master-slave control dengan komunikasi I2C
- Current-based load balancing (jika motor 1 arus tinggi, motor 2 assist)
- Speed compensation algorithm: 
  $$RPM_2 = RPM_1 \times (1 + K \times error)$$

**Demo:** https://youtube.com/example1 (contoh placeholder)

---

### Example 2: AGV Differential Drive
**Highlights:**
- Odometry calculation:
  $$x = \frac{(d_L + d_R)}{2} \cos(\theta)$$
  $$y = \frac{(d_L + d_R)}{2} \sin(\theta)$$
  $$\theta = \frac{(d_R - d_L)}{wheelbase}$$
  
- Trajectory tracking dengan look-ahead distance
- Encoder calibration untuk wheel diameter & wheelbase

**Demo:** Robot bergerak lurus 2 meter, pivot 90°, lurus lagi 2 meter → deviation diukur

---

### Example 3: Linear Actuator
**Highlights:**
- Homing sequence: 
  1. Move reverse until limit switch trigger
  2. Stop & set position = 0
  3. Move forward 5mm (clear switch)
  
- Point-to-point motion dengan trapezoidal velocity:
  ```
  Velocity
     ▲
     │    /‾‾‾\
     │   /     \
     │  /       \
     └─┴─────────┴──► Time
       Accel Const Decel
  ```

**Performance:** 50 cycles pick-and-place → repeatability ±0.15 mm (measured dengan dial indicator)

---

## KONSULTASI & SUPPORT

**Office Hours:**
- Senin & Rabu, 13:00-15:00 (Lab Robotika)
- Online: [Link Discord / Zoom]

**Milestone Check:**
- **Week 1 Friday:** Design review (bawa schematic + motor data)
- **Week 2 Wednesday:** Progress check (demo partial functionality)

**Contact:**
- Email: [instructor@university.edu]
- WhatsApp Group: [Link]

---

## FAQ

**Q: Boleh pakai motor servo atau stepper?**  
A: Boleh, tapi pastikan requirements tetap terpenuhi. Motor servo sudah ada closed-loop internal, jadi less challenging.

**Q: Boleh kerjasama kelompok?**  
A: Max 2 orang. Jika kelompok, requirements dinaikkan (e.g., 3 motor untuk Opsi A).

**Q: Bagaimana jika motor yang tersedia berbeda spec-nya?**  
A: Konsultasikan dulu. Sesuaikan requirements dengan motor capability.

**Q: Boleh pakai library pihak ketiga?**  
A: Boleh untuk non-core function (e.g., INA219 library, LCD library). Core control harus implementasi sendiri.

**Q: Jika tidak punya oscilloscope?**  
A: Gunakan serial plotter untuk monitoring waveform. Untuk FFT analysis, bisa capture data via serial → analyze di Python.

---

## GRADING SUMMARY

| Component                    | Weight | Due Date          |
|------------------------------|--------|-------------------|
| Design Document              | 10%    | Week 1 Friday     |
| Motor Characterization       | 15%    | Week 1 Friday     |
| Firmware Implementation      | 30%    | Week 2 Friday     |
| Performance Testing          | 20%    | Week 2 Friday     |
| Technical Report             | 10%    | Week 2 Friday     |
| Presentation                 | 10%    | Week 2 Friday     |
| Code Quality & Documentation | 5%     | Week 2 Friday     |
| **Bonus**                    | +15%   | -                 |
| **TOTAL**                    | **100% (+15%)** | |

**Keterlambatan:**
- 1-24 jam: -10%
- 24-48 jam: -20%
- >48 jam: tidak diterima (kecuali force majeure)

---

**Good luck! Show us your engineering creativity and problem-solving skills!** 🚀

---

*Last updated: [Date]*
