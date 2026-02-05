# JOBSHEET PRAKTIKUM
## Modul 5: Motor & Drive Systems

**Durasi:** 240 menit (4 jam)  
**Prasyarat:** Modul 1 (Arsitektur Sistem), Modul 4 (Sensor Fusion - encoder reading)

---

## TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa diharapkan mampu:
1. Mengidentifikasi karakteristik motor DC, servo, dan stepper
2. Mengimplementasikan PWM untuk kontrol kecepatan motor
3. Melakukan karakterisasi motor (torque-speed curve)
4. Mengintegrasikan motor driver (H-bridge) dengan mikrokontroler
5. Menggunakan encoder untuk closed-loop speed control
6. Mengimplementasikan current sensing untuk proteksi dan monitoring

---

## PERALATAN & KOMPONEN

### Hardware
- [x] ESP32 DevKit / Arduino Mega 2560
- [x] Motor DC 12V dengan encoder (500-1024 PPR)
- [x] Motor driver L298N / DRV8833 / TB6612FNG
- [x] Power supply 12V 2A
- [x] INA219 current sensor module (optional)
- [x] Potensiometer 10kΩ
- [x] Oscilloscope (untuk analisis PWM)
- [x] Tachometer / Laser RPM meter (optional)
- [x] Beban motor: Pulley + tali + massa (100g, 200g, 500g)
- [x] Breadboard + kabel jumper
- [x] Multimeter

### Software
- [x] PlatformIO IDE
- [x] Python 3.x dengan libraries: numpy, matplotlib, pandas
- [x] Serial plotter / monitor
- [x] Oscilloscope software (optional)

---

## PERCOBAAN 1: KARAKTERISTIK PWM & MOTOR RESPONSE

**Tujuan:** Memahami pengaruh duty cycle PWM terhadap kecepatan motor dan menganalisis spektrum frekuensi PWM.

### Langkah Kerja

#### 1.1 Setup Hardware
1. Hubungkan motor driver L298N:
   - `ENA` → ESP32 GPIO 25 (PWM)
   - `IN1` → ESP32 GPIO 26
   - `IN2` → ESP32 GPIO 27
   - `Motor output` → Motor terminals
   - `12V` → Power supply (+)
   - `GND` → Common ground (ESP32 + PSU)

2. Hubungkan encoder motor:
   - `Encoder A` → ESP32 GPIO 34 (interrupt)
   - `Encoder B` → ESP32 GPIO 35 (interrupt)
   - `Vcc` → 5V
   - `GND` → GND

3. Verifikasi koneksi dengan multimeter (pastikan tidak ada short circuit).

#### 1.2 Konfigurasi Firmware
Edit `config.h`:
```cpp
#define PWM_FREQUENCY 20000    // 20 kHz
#define PWM_RESOLUTION 8       // 8-bit (0-255)
#define MOTOR_PWM_PIN 25
#define MOTOR_IN1_PIN 26
#define MOTOR_IN2_PIN 27
#define ENCODER_PPR 600        // Sesuaikan dengan encoder Anda
```

Upload kode `exp1_pwm_sweep.cpp`.

#### 1.3 Prosedur Pengukuran

1. **PWM Sweep Test:**
   - Program akan mensweep duty cycle dari 0% → 100% → 0%
   - Step: 5%, delay 2 detik per step
   - Monitor serial plotter: `duty_cycle`, `speed_rpm`, `current_mA`

2. **Oscilloscope Analysis:**
   - Probe CH1: PWM pin (GPIO 25)
   - Probe CH2: Motor terminal voltage
   - Set timebase: 50 μs/div
   - Capture waveform untuk duty cycle: 25%, 50%, 75%, 100%

3. **Speed Measurement:**
   - Gunakan encoder untuk ukur RPM sebenarnya
   - Alternatif: Laser tachometer (cross-check)

#### 1.4 Data yang Dicatat

| Duty Cycle (%) | PWM Value | Speed (RPM) | Current (mA) | Voltage (V) |
|----------------|-----------|-------------|--------------|-------------|
| 0              |           |             |              |             |
| 25             |           |             |              |             |
| 50             |           |             |              |             |
| 75             |           |             |              |             |
| 100            |           |             |              |             |

**Tambahan (Oscilloscope):**
- Screencapture waveform PWM untuk setiap duty cycle
- Ukur: Frequency, Period, Duty cycle (verify), Rise time, Fall time

### Analisis

1. Plot grafik **Duty Cycle vs Speed (RPM)**
   - Apakah hubungannya linear? Jika tidak, jelaskan penyebabnya.
   
2. Hitung **Dead-zone** motor:
   - Duty cycle minimum agar motor mulai berputar
   - Penyebab: Friction, back-EMF threshold
   
3. FFT analysis pada waveform oscilloscope:
   - Fundamental frequency = PWM freq
   - Harmonics: 2f₀, 3f₀, ...
   - Identifikasi noise di spektrum

4. **Pertanyaan:**
   - Apa pengaruh frekuensi PWM terhadap torsi ripple?
   - Berapa frekuensi PWM minimum agar motor tidak audible (tidak terdengar)?

---

## PERCOBAAN 2: DIRECTIONAL CONTROL (H-BRIDGE)

**Tujuan:** Mengimplementasikan kontrol arah motor menggunakan H-bridge dan memahami mode braking.

### Langkah Kerja

#### 2.1 Forward-Reverse Control
Upload kode `exp2_direction_control.cpp`.

Program akan menjalankan sequence:
1. Forward (PWM 60%) - 3 detik
2. Brake (short brake) - 1 detik
3. Reverse (PWM 60%) - 3 detik
4. Coast (free-wheeling) - 1 detik
5. Repeat

#### 2.2 Braking Comparison

**Mode 1: Coast (Free-wheeling)**
```cpp
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
// Motor coasts to stop (slow deceleration)
```

**Mode 2: Short Brake**
```cpp
digitalWrite(IN1, HIGH);
digitalWrite(IN2, HIGH);
// Motor terminals shorted (fast deceleration)
```

**Prosedur:**
1. Jalankan motor pada 80% PWM hingga stabil
2. Trigger brake mode
3. Ukur waktu dari max speed → 0 RPM untuk kedua mode
4. Plot speed decay curve

#### 2.3 Dead-Time Testing

Modifikasi kode untuk eksperimen dead-time:
```cpp
// No dead-time (DANGEROUS - monitor current!)
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);

// With dead-time (SAFE)
digitalWrite(IN2, LOW);
delayMicroseconds(2);  // 2 μs dead-time
digitalWrite(IN1, HIGH);
```

**WARNING:** Jangan jalankan tanpa dead-time terlalu lama! Risiko shoot-through.

Monitor current dengan INA219 saat transisi. Bandingkan spike current dengan/tanpa dead-time.

### Data yang Dicatat

| Brake Mode    | Initial Speed (RPM) | Stop Time (ms) | Deceleration (RPM/s) |
|---------------|---------------------|----------------|----------------------|
| Coast         |                     |                |                      |
| Short Brake   |                     |                |                      |

### Analisis

1. Hitung energi kinetik motor:
   $$E_k = \frac{1}{2} J \omega^2$$
   Dimana $J$ = moment of inertia rotor (dari datasheet)

2. Estimasi braking torque:
   $$T_{brake} = J \alpha = J \frac{\Delta \omega}{\Delta t}$$

3. **Pertanyaan:**
   - Mengapa short brake lebih cepat dari coast?
   - Apa risiko shoot-through jika dead-time terlalu pendek?
   - Kapan menggunakan regenerative braking (jika driver support)?

---

## PERCOBAAN 3: MOTOR CHARACTERIZATION (TORQUE-SPEED CURVE)

**Tujuan:** Mengukur karakteristik motor untuk membuat torque-speed curve dan menentukan operating point optimal.

### Langkah Kerja

#### 3.1 Setup Beban Mekanik

1. Pasang pulley kecil (diameter 20-30 mm) pada shaft motor
2. Ikat tali pada pulley, gantung massa beban
3. Pastikan tali tidak slip (gunakan rubber band jika perlu)

#### 3.2 Pengukuran No-Load Speed

1. Jalankan motor tanpa beban (massa = 0)
2. Set PWM 100% (full voltage)
3. Tunggu hingga kecepatan stabil (5-10 detik)
4. Catat RPM maksimum → ini adalah **ω_no-load**

#### 3.3 Load Testing

Upload kode `exp3_load_test.cpp`.

**Prosedur:**
1. Set PWM = 100% (voltage konstan)
2. Tambahkan beban secara bertahap: 0g → 100g → 200g → 300g → ... hingga motor stall
3. Untuk setiap beban:
   - Tunggu kecepatan stabil (10 detik)
   - Catat: RPM, Current (mA), Voltage
   - Hitung torsi: $T = r \times m \times g$ (r = radius pulley)

**Contoh:**
- Pulley radius: 15 mm = 0.015 m
- Massa: 200 g = 0.2 kg
- Torsi: $T = 0.015 \times 0.2 \times 9.81 = 0.029 \text{ N·m}$

### Data yang Dicatat

| Beban (g) | Torque (N·m) | Speed (RPM) | Current (mA) | Power Out (W) | Efficiency (%) |
|-----------|--------------|-------------|--------------|---------------|----------------|
| 0         |              |             |              |               |                |
| 100       |              |             |              |               |                |
| 200       |              |             |              |               |                |
| 300       |              |             |              |               |                |
| ...       |              |             |              |               |                |
| Stall     |              | 0           |              |               |                |

**Formula:**
- $P_{out} = T \times \omega = T \times \frac{RPM \times 2\pi}{60}$
- $P_{in} = V \times I$
- $\eta = \frac{P_{out}}{P_{in}} \times 100\%$

### Analisis

1. **Plot Torque-Speed Curve:**
   - X-axis: Speed (RPM)
   - Y-axis: Torque (N·m)
   - Fit linear regression: $\omega = a - bT$
   - Identifikasi: ω_no-load, T_stall

2. **Plot Power Curve:**
   - X-axis: Speed (RPM)
   - Y-axis: Power (W)
   - Titik power maksimum terjadi di mana?

3. **Plot Efficiency Curve:**
   - X-axis: Torque (N·m)
   - Y-axis: Efficiency (%)
   - Pada torsi berapa efisiensi maksimum?

4. **Determine Motor Constants:**
   - Back-EMF constant: $K_e = \frac{V_{supply} - I_{no-load} R_a}{\omega_{no-load}}$
   - Torque constant: $K_t \approx K_e$ (dalam SI units)
   - Armature resistance: $R_a = \frac{V_{supply}}{I_{stall}}$

5. **Pertanyaan:**
   - Pada beban berapa motor beroperasi paling efisien?
   - Apa dampak overvoltage/undervoltage terhadap torque-speed curve?
   - Bagaimana memilih motor untuk aplikasi conveyor (torsi tinggi, kecepatan rendah)?

---

## PERCOBAAN 4: CURRENT SENSING & OVERCURRENT PROTECTION

**Tujuan:** Mengimplementasikan monitoring arus motor untuk proteksi dan estimasi torsi.

### Langkah Kerja

#### 4.1 Setup INA219 Current Sensor

1. Hubungkan INA219:
   - `VCC` → 3.3V
   - `GND` → GND
   - `SDA` → ESP32 GPIO 21
   - `SCL` → ESP32 GPIO 22
   - `V+` → Motor driver output (+)
   - `V-` → Motor terminal (+)

2. Kalibrasi sensor (jalankan kode kalibrasi):
   ```cpp
   ina219.setCalibration_16V_400mA();  // Sesuaikan range
   ```

#### 4.2 Current Profiling

Upload kode `exp4_current_monitoring.cpp`.

**Test Scenarios:**

1. **No-Load Current:**
   - Jalankan motor tanpa beban, berbagai PWM duty cycle
   - Catat baseline current

2. **Loaded Current:**
   - Tambahkan beban bertahap (100g, 200g, 500g)
   - Observe current increase

3. **Stall Current:**
   - Block motor shaft manually (hati-hati!)
   - Catat peak stall current
   - **Batasi durasi < 2 detik untuk hindari overheat!**

4. **Inrush Current:**
   - Start motor dari 0 PWM → 100% PWM instantly
   - Capture current spike dengan serial plotter (sample rate tinggi)

### Data yang Dicatat

| Kondisi         | PWM (%) | Speed (RPM) | Current (mA) | Voltage (V) | Estimated Torque (N·m) |
|-----------------|---------|-------------|--------------|-------------|------------------------|
| No-load         | 50      |             |              |             |                        |
| No-load         | 100     |             |              |             |                        |
| Beban 200g      | 100     |             |              |             |                        |
| Beban 500g      | 100     |             |              |             |                        |
| Stall (blocked) | 50      | 0           |              |             |                        |

**Estimated Torque:**
$$T = K_t (I - I_{no-load})$$

Gunakan $K_t$ dari Percobaan 3.

#### 4.3 Overcurrent Protection Implementation

Implementasi threshold detection:
```cpp
const float CURRENT_LIMIT = 1500.0;  // mA
const int OVERCURRENT_DURATION = 500;  // ms

if (current_mA > CURRENT_LIMIT) {
    overcurrent_timer += delta_time;
    if (overcurrent_timer > OVERCURRENT_DURATION) {
        // Trigger protection
        setMotorPWM(0);
        digitalWrite(LED_FAULT, HIGH);
        Serial.println("FAULT: Overcurrent!");
    }
} else {
    overcurrent_timer = 0;
}
```

**Test:**
- Block motor shaft → harus trigger protection dalam 500 ms
- Release → motor harus bisa restart (auto-recovery atau manual reset?)

### Analisis

1. **Torque Estimation Accuracy:**
   - Bandingkan torsi estimasi (via current) dengan torsi aktual (via load cell)
   - Hitung error percentage

2. **Inrush Current Analysis:**
   - Berapa lama inrush current spike?
   - Berapa kali lipat dari steady-state current?
   - Strategi mitigasi: Soft-start (slew rate limiting)

3. **Protection Tuning:**
   - Bagaimana menentukan threshold optimal? (terlalu rendah: false trigger, terlalu tinggi: risiko damage)
   - Berapa lama delay yang aman sebelum shutdown?

4. **Pertanyaan:**
   - Apa perbedaan antara fuse, circuit breaker, dan software overcurrent protection?
   - Bagaimana mendeteksi motor stall tanpa current sensor (alternatif method)?

---

## PERCOBAAN 5: CLOSED-LOOP SPEED CONTROL (BASIC)

**Tujuan:** Mengimplementasikan simple proportional controller untuk menjaga kecepatan motor konstan terhadap perubahan beban.

### Langkah Kerja

#### 5.1 Open-Loop Baseline

Upload kode `exp5a_open_loop.cpp`.

1. Set PWM tetap = 60%
2. Jalankan motor tanpa beban → catat RPM
3. Tambahkan beban 200g → catat RPM baru
4. Hitung speed drop: $\Delta RPM = RPM_{no-load} - RPM_{loaded}$

#### 5.2 Closed-Loop Control

Upload kode `exp5b_closed_loop.cpp`.

Implementasi simple P-controller:
```cpp
float Kp = 0.1;  // Proportional gain (tuning parameter)
float setpoint_rpm = 300.0;

void loop() {
    // Read actual speed
    float actual_rpm = calculateRPM();
    
    // Calculate error
    float error = setpoint_rpm - actual_rpm;
    
    // P-control
    float control_signal = Kp * error;
    
    // Apply to motor (with saturation)
    int pwm = constrain(base_pwm + control_signal, 0, 255);
    setMotorPWM(pwm);
}
```

**Test:**
1. Set setpoint = 300 RPM
2. Jalankan motor → tunggu hingga stabil
3. Tambahkan beban 200g → observe correction
4. Lepas beban → observe correction
5. Ulangi dengan Kp berbeda: 0.05, 0.1, 0.2, 0.5

### Data yang Dicatat

| Controller Type | Kp   | Load Change | Steady-State Error (RPM) | Settling Time (s) | Overshoot (%) |
|-----------------|------|-------------|--------------------------|-------------------|---------------|
| Open-loop       | -    | 0g → 200g   |                          |                   |               |
| Closed-loop     | 0.05 | 0g → 200g   |                          |                   |               |
| Closed-loop     | 0.1  | 0g → 200g   |                          |                   |               |
| Closed-loop     | 0.2  | 0g → 200g   |                          |                   |               |
| Closed-loop     | 0.5  | 0g → 200g   |                          |                   |               |

**Plot:**
- Time-series: Setpoint vs Actual RPM (saat load step)
- Marking: Load applied, Load removed

### Analisis

1. **Steady-State Error:**
   - Mengapa P-controller saja tidak bisa mencapai zero error?
   - Apa yang terjadi jika Kp terlalu tinggi? (oscillation?)

2. **Disturbance Rejection:**
   - Berapa lama waktu untuk reject load disturbance?
   - Bagaimana Kp mempengaruhi kecepatan rejection?

3. **Stability Analysis:**
   - Pada Kp berapa sistem mulai oscillate?
   - Bagaimana Kp optimal berbeda untuk motor berbeda (inertia, friction)?

4. **Pertanyaan:**
   - Apa keuntungan closed-loop dibanding open-loop?
   - Kapan open-loop masih acceptable (contoh aplikasi)?
   - Apa yang diperlukan untuk eliminasi steady-state error? (Hint: Integral term)

---

## PERCOBAAN 6: SLEW RATE LIMITING & SOFT-START

**Tujuan:** Mengimplementasikan slew rate limiting untuk mengurangi mechanical shock dan inrush current.

### Langkah Kerja

#### 6.1 Hard-Start (Baseline)

Upload kode `exp6a_hard_start.cpp`.

**Test:**
1. Motor idle (PWM = 0)
2. Instant jump → PWM = 100%
3. Monitor:
   - Current spike (via INA219)
   - Acceleration time (0 → max RPM)
   - Mechanical vibration (observasi visual/audio)

#### 6.2 Soft-Start Implementation

Upload kode `exp6b_soft_start.cpp`.

```cpp
const int SLEW_RATE = 5;  // PWM change per iteration (10ms loop)

int target_pwm = 255;
int current_pwm = 0;

void loop() {
    // Ramp up gradually
    if (current_pwm < target_pwm) {
        current_pwm += SLEW_RATE;
        if (current_pwm > target_pwm) current_pwm = target_pwm;
    }
    setMotorPWM(current_pwm);
    delay(10);
}
```

**Test berbagai slew rate:**
- Fast: 10 PWM/iteration → ramp time = 255/10 × 10ms = 255 ms
- Medium: 5 PWM/iteration → ramp time = 510 ms
- Slow: 2 PWM/iteration → ramp time = 1275 ms

### Data yang Dicatat

| Slew Rate | Ramp Time (ms) | Peak Current (mA) | Current Reduction (%) | Audible Noise |
|-----------|----------------|-------------------|-----------------------|---------------|
| Instant   | 0              |                   | 0 (baseline)          |               |
| 10        |                |                   |                       |               |
| 5         |                |                   |                       |               |
| 2         |                |                   |                       |               |

### Analisis

1. **Current Spike Reduction:**
   - Plot: Slew rate vs Peak current
   - Berapa % reduction dengan slew rate = 5?

2. **Mechanical Stress:**
   - Observasi: Apakah motor "jerking" berkurang dengan soft-start?
   - Aplikasi kritis: Gearbox protection, belt-drive system

3. **Trade-off:**
   - Pro: Lower current spike, smoother operation
   - Con: Slower response time
   - Optimal slew rate untuk aplikasi conveyor vs robot arm?

4. **Pertanyaan:**
   - Bagaimana implementasi slew rate untuk deceleration (braking)?
   - Apa bedanya slew rate limiting dengan S-curve trajectory?

---

## DELIVERABLES

### 1. Laporan Praktikum (PDF)
Struktur:
- **Cover:** Judul, nama, NIM, tanggal
- **Abstrak:** 150-200 kata
- **Pendahuluan:** Tujuan dan teori singkat
- **Metodologi:** Setup hardware + firmware (dengan diagram)
- **Hasil & Analisis:** 
  - Semua tabel data
  - Plot grafik (minimal 6 grafik)
  - Analisis setiap percobaan
  - Screenshot oscilloscope waveform
- **Kesimpulan:** Key findings + lessons learned
- **Referensi**

### 2. Source Code (Git Repository)
```
Modul-05_Motor_Drive/
├── src/
│   ├── exp1_pwm_sweep.cpp
│   ├── exp2_direction_control.cpp
│   ├── exp3_load_test.cpp
│   ├── exp4_current_monitoring.cpp
│   ├── exp5a_open_loop.cpp
│   ├── exp5b_closed_loop.cpp
│   ├── exp6a_hard_start.cpp
│   └── exp6b_soft_start.cpp
├── analysis/
│   ├── plot_torque_speed.py
│   ├── plot_current_profile.py
│   └── plot_step_response.py
└── data/
    ├── exp1_pwm_sweep.csv
    ├── exp3_load_test.csv
    └── exp5_closed_loop.csv
```

### 3. Video Demo (MP4, max 5 menit)
Konten:
- Setup hardware (30 detik)
- PWM sweep test (1 menit)
- Torque-speed characterization (1.5 menit)
- Closed-loop disturbance rejection (1.5 menit)
- Narasi penjelasan

### 4. Presentasi (PowerPoint, 10 slide)
- Slide 1: Title
- Slide 2-3: Teori singkat (PWM, H-bridge, torque-speed)
- Slide 4-5: Setup & metodologi
- Slide 6-8: Hasil key experiments (grafik + analisis)
- Slide 9: Kesimpulan
- Slide 10: Q&A

---

## RUBRIK PENILAIAN

| Aspek                        | Bobot | Kriteria                                                                 |
|------------------------------|-------|--------------------------------------------------------------------------|
| **Hardware Setup**           | 10%   | Wiring benar, no short circuit, proper grounding                         |
| **Firmware Implementation**  | 25%   | Semua 6 percobaan berjalan, kode terstruktur, proper error handling      |
| **Data Acquisition**         | 15%   | Data lengkap, akurat, dengan unit yang benar                             |
| **Grafik & Visualisasi**     | 15%   | Minimal 6 grafik, axis label jelas, legend, caption informatif           |
| **Analisis & Insight**       | 20%   | Analisis mendalam, interpretasi hasil, jawaban pertanyaan kritis         |
| **Laporan & Dokumentasi**    | 10%   | Struktur rapi, grammar, referensi lengkap, reproducible                  |
| **Kreativitas & Eksplorasi** | 5%    | Eksperimen tambahan, optimasi, troubleshooting unik                      |

**Total:** 100%

**Bonus (+10%):**
- Implementasi PID controller (preview Modul 6)
- Real-time web dashboard untuk monitoring (WebSocket/MQTT)
- Thermal imaging analysis untuk motor heating

---

## TROUBLESHOOTING GUIDE

### Motor tidak berputar
- [ ] Check power supply voltage (12V?)
- [ ] Check motor driver enable pin (HIGH?)
- [ ] Verify PWM signal dengan oscilloscope
- [ ] Test motor langsung dari power supply (bypass driver)

### Motor berputar tidak smooth (jerky)
- [ ] PWM frequency terlalu rendah → naikkan ke 20 kHz
- [ ] Insufficient power supply current → gunakan PSU ≥2A
- [ ] Encoder noise → tambahkan capacitor 100nF di Vcc-GND

### Encoder reading tidak akurat
- [ ] Check pull-up resistor (4.7kΩ recommended)
- [ ] Verify quadrature signal dengan oscilloscope (fase 90°?)
- [ ] Debouncing: Tambahkan hardware filter atau software debounce

### Overcurrent protection false trigger
- [ ] Threshold terlalu rendah → naikkan limit
- [ ] Inrush current spike → implementasi soft-start
- [ ] INA219 wiring error → check V+, V- orientation

### Motor overheat
- [ ] Duty cycle terlalu tinggi untuk terlalu lama
- [ ] Motor driver heatsink insufficient → tambahkan cooling
- [ ] Motor stall terlalu lama → reduce test duration

---

## REFERENSI

1. **Datasheet:**
   - L298N Dual H-Bridge Motor Driver
   - DRV8833 Dual Motor Driver Carrier
   - INA219 Current Sensor
   
2. **Application Notes:**
   - "PWM Techniques for Motor Control" - Texas Instruments (SLVA898)
   - "Brushed DC Motor Fundamentals" - Microchip (AN905)

3. **Tutorial:**
   - "ESP32 PWM Tutorial" - Random Nerd Tutorials
   - "Motor Control with Arduino" - Adafruit Learn

---

**Instruktur:** [Nama]  
**Kontak:** [Email]  
**Office Hours:** [Jadwal]

---

*Selamat praktikum! Jangan ragu bertanya jika ada kendala.*
