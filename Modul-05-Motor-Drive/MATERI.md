# MATERI PEMBELAJARAN
## Modul 5: Motor & Drive Systems

**Tujuan Pembelajaran:**
- Memahami karakteristik motor DC, servo, dan stepper
- Menguasai prinsip PWM untuk kontrol kecepatan motor
- Mampu melakukan karakterisasi motor (torque-speed curve)
- Memahami arsitektur motor driver (H-bridge, half-bridge)
- Mampu mengintegrasikan encoder untuk closed-loop control

---

## 1. DEFINISI & KONSEP DASAR

### 1.1 Jenis-Jenis Motor

#### Motor DC Brushed
Motor listrik yang menggunakan komutator mekanik (sikat) untuk mengubah arah arus pada kumparan rotor.

**Karakteristik:**
- Konstruksi sederhana dan biaya rendah
- Kontrol kecepatan mudah dengan PWM
- Torsi tinggi pada kecepatan rendah
- Memerlukan perawatan sikat (brush)
- Noise elektromagnetik tinggi

**Persamaan Dasar:**
$$V = E + I_a R_a + L_a \frac{dI_a}{dt}$$

Dimana:
- $V$ = tegangan armatur (V)
- $E$ = back-EMF = $K_e \omega$ (V)
- $I_a$ = arus armatur (A)
- $R_a$ = resistansi armatur (Ω)
- $L_a$ = induktansi armatur (H)
- $K_e$ = konstanta back-EMF (V·s/rad)
- $\omega$ = kecepatan sudut (rad/s)

**Torsi Motor:**
$$T = K_t I_a$$

Dimana:
- $T$ = torsi (N·m)
- $K_t$ = konstanta torsi (N·m/A)
- $I_a$ = arus armatur (A)

#### Motor DC Brushless (BLDC)
Motor DC tanpa sikat yang menggunakan komutasi elektronik melalui controller.

**Keunggulan:**
- Efisiensi tinggi (>90%)
- Umur pakai panjang (tanpa sikat)
- Rasio torsi/berat tinggi
- Noise elektromagnetik rendah

**Kelemahan:**
- Memerlukan driver elektronik kompleks
- Biaya lebih tinggi
- Memerlukan sensor posisi rotor (Hall sensor)

#### Motor Servo
Motor DC/BLDC dengan sistem closed-loop control terintegrasi untuk kontrol posisi presisi.

**Komponen:**
- Motor DC/BLDC
- Gearbox (reduksi)
- Encoder/potentiometer (feedback)
- Controller (PID internal)

**Aplikasi:**
- Robot manipulator
- CNC machine
- Kamera pan-tilt

#### Motor Stepper
Motor yang bergerak dalam langkah diskrit (step) dengan sudut presisi.

**Karakteristik:**
- Open-loop positioning (tanpa encoder)
- Holding torque tinggi saat diam
- Presisi sudut tinggi (1.8° - 0.9° per step)
- Torsi menurun pada kecepatan tinggi

**Jenis:**
- Bipolar (4 wire) - torsi tinggi
- Unipolar (5/6 wire) - kontrol sederhana

---

### 1.2 Pulse Width Modulation (PWM)

PWM adalah teknik modulasi untuk mengatur daya rata-rata yang diterima motor dengan mengubah duty cycle sinyal on-off.

**Parameter PWM:**
- **Duty Cycle (D):** Persentase waktu sinyal HIGH
  $$D = \frac{t_{ON}}{T} \times 100\%$$
  
- **Frekuensi PWM (f):** 
  $$f = \frac{1}{T}$$
  
- **Tegangan Rata-rata:**
  $$V_{avg} = D \times V_{supply}$$

**Pemilihan Frekuensi PWM:**
- Terlalu rendah (<1 kHz): Torsi ripple, noise audible
- Optimal (10-20 kHz): Smooth operation, inaudible
- Terlalu tinggi (>50 kHz): Switching loss meningkat, EMI

**Resolusi PWM:**
Pada mikrokontroler, resolusi PWM ditentukan oleh clock dan prescaler:

$$Resolution = \frac{f_{clock}}{f_{PWM} \times prescaler}$$

Contoh ESP32:
- 8-bit: 0-255 (256 level)
- 10-bit: 0-1023 (1024 level)
- 16-bit: 0-65535 (65536 level)

---

### 1.3 Motor Driver (H-Bridge)

H-Bridge adalah rangkaian switching untuk mengontrol arah dan kecepatan motor DC.

**Topologi:**
```
        +Vcc
         |
      Q1   Q2
       |   |
   IN1-+   +-IN2
       |   |
    M--+---+--M  (Motor)
       |   |
      Q3   Q4
       |   |
        GND
```

**Mode Operasi:**

| IN1 | IN2 | Q1 | Q2 | Q3 | Q4 | Arah Motor |
|-----|-----|----|----|----|----|------------|
| 1   | 0   | ON | OFF| OFF| ON | Forward    |
| 0   | 1   | OFF| ON | ON | OFF| Reverse    |
| 0   | 0   | OFF| OFF| ON | ON | Brake (short)|
| 1   | 1   | ON | ON | OFF| OFF| Brake      |

**PWM pada H-Bridge:**
- **Sign-Magnitude PWM:** Satu pin PWM, satu pin direction
  - IN1 = PWM, IN2 = DIR
  - Sederhana, tapi brake kurang efektif
  
- **Locked Anti-Phase PWM:** PWM pada kedua input (IN1 dan IN2 inversi)
  - D=50%: motor berhenti
  - D>50%: forward
  - D<50%: reverse
  - Regenerative braking alami

**IC Driver Populer:**
- **L298N:** Dual H-bridge, arus maks 2A/channel, built-in dioda flyback
- **DRV8833:** Dual H-bridge, arus 1.5A, efisiensi tinggi, protection lengkap
- **TB6612FNG:** Dual H-bridge, arus 1.2A, low voltage drop
- **VNH5019:** Single H-bridge, arus 12A continuous, 30A peak

---

### 1.4 Karakterisasi Motor

#### Torque-Speed Curve
Grafik hubungan antara torsi (T) dan kecepatan (ω) motor pada tegangan konstan.

**Persamaan Linear (ideal):**
$$\omega = \omega_{no-load} - \frac{R_a}{K_e K_t} T$$

**Parameter Kunci:**
- **No-load speed ($\omega_0$):** Kecepatan tanpa beban
- **Stall torque ($T_{stall}$):** Torsi maksimum saat motor berhenti
- **Rated point:** Titik operasi optimal (biasanya 70-80% torsi maksimal)

**Metode Pengukuran:**
1. **Direct Loading:** Menggunakan dynamometer/torque sensor
2. **Indirect (Current-based):** 
   $$T = K_t (I - I_{no-load})$$
3. **Acceleration Method:** Mengukur akselerasi saat start

#### Power Curve
Daya mekanik output motor:
$$P = T \times \omega$$

**Efisiensi:**
$$\eta = \frac{P_{out}}{P_{in}} = \frac{T \omega}{V I} \times 100\%$$

**Kurva Karakteristik:**
- Daya maksimum terjadi pada $T = 0.5 \times T_{stall}$
- Efisiensi maksimum terjadi pada beban rendah (10-30% rated torque)

---

### 1.5 Current Sensing

Pengukuran arus motor untuk:
- Proteksi overcurrent
- Estimasi torsi ($T = K_t I$)
- Deteksi stall/blocking
- Monitoring efisiensi

**Metode:**

#### Shunt Resistor
Resistor presisi rendah (0.01-0.1 Ω) di jalur motor.

$$I = \frac{V_{shunt}}{R_{shunt}}$$

**Keunggulan:** Akurat, murah
**Kelemahan:** Power loss, isolasi ground

#### Hall-Effect Sensor (ACS712, INA219)
Sensor non-invasive berbasis efek Hall.

**Keunggulan:** Isolasi galvanik, zero power loss
**Kelemahan:** Offset drift, biaya tinggi

**Contoh INA219:**
- Range: ±3.2A
- Resolusi: 0.8 mA
- Interface: I2C
- Mengukur tegangan dan arus simultan

---

## 2. KONSEP INTI

### 2.1 Dead-Time & Shoot-Through Prevention

**Shoot-Through:** Kondisi Q1 dan Q3 (atau Q2 dan Q4) ON bersamaan, menyebabkan short circuit Vcc-GND.

**Solusi: Dead-Time**
Delay antara OFF satu transistor dan ON transistor pasangannya.

$$t_{dead} > t_{fall} + t_{rise} + safety\_margin$$

Typical: 1-2 μs

**Implementasi di firmware:**
```cpp
void setMotorPWM(int pwm) {
    if (pwm > 0) {
        digitalWrite(IN2, LOW);
        delayMicroseconds(DEAD_TIME_US);
        analogWrite(IN1, pwm);
    } else {
        digitalWrite(IN1, LOW);
        delayMicroseconds(DEAD_TIME_US);
        analogWrite(IN2, abs(pwm));
    }
}
```

---

### 2.2 Encoder Feedback untuk Kecepatan

**Quadrature Encoder:**
Menghasilkan 2 sinyal (A dan B) dengan fase 90° untuk deteksi arah dan posisi.

**Resolusi:**
$$Resolution = PPR \times 4 \times GearRatio$$

Dimana:
- PPR = Pulses Per Revolution
- ×4 = Quadrature decoding (rising & falling edge A dan B)

**Perhitungan Kecepatan:**

$$\omega = \frac{\Delta ticks}{\Delta t \times PPR \times 4} \times 2\pi \quad (rad/s)$$

$$RPM = \frac{\Delta ticks}{\Delta t \times PPR \times 4} \times 60$$

**Metode Pengukuran:**
1. **Fixed Time:** Hitung pulsa dalam waktu tetap (100 ms)
   - Akurat untuk kecepatan tinggi
   - Noise tinggi pada kecepatan rendah
   
2. **Fixed Count:** Hitung waktu untuk N pulsa
   - Akurat untuk kecepatan rendah
   - Latency tinggi
   
3. **Hybrid:** Kombinasi kedua metode berdasarkan kecepatan

---

### 2.3 Slew Rate Limiting

Membatasi laju perubahan PWM untuk mencegah:
- Spike arus berlebihan
- Mechanical shock pada gearbox
- Slip pada sistem traksi

**Implementasi:**
$$PWM_{new} = PWM_{old} + sign(PWM_{target} - PWM_{old}) \times SLEW\_RATE \times \Delta t$$

```cpp
int applySlewRate(int target, int current, int slew_rate) {
    int delta = target - current;
    int max_change = slew_rate;  // per iteration
    
    if (abs(delta) <= max_change) {
        return target;
    } else {
        return current + (delta > 0 ? max_change : -max_change);
    }
}
```

---

### 2.4 Thermal Management

Motor driver menghasilkan panas dari:
- Switching loss: $P_{sw} = V_{drop} \times I \times f_{PWM}$
- Conduction loss: $P_{cond} = I^2 R_{DS(on)}$

**Derating Curve:**
Arus maksimum menurun dengan kenaikan suhu.

**Proteksi:**
- Thermal shutdown (TSD) di IC driver
- Heatsink dengan thermal resistance: $R_{th} = \frac{\Delta T}{P}$ (°C/W)
- Active cooling (fan) untuk aplikasi high-power

---

## 3. DIAGRAM & VISUALISASI

### 3.1 Rangkaian Sistem Motor Drive

```
┌─────────────┐       ┌──────────────┐       ┌─────────┐
│ Microcon-   │ PWM   │ Motor Driver │       │  Motor  │
│ troller     ├──────►│  (H-Bridge)  ├──────►│   DC    │
│  (ESP32)    │ DIR   │   L298N      │       │         │
└──────┬──────┘       └──────┬───────┘       └────┬────┘
       │                     │                     │
       │ I2C                 │ Vsense             │ Encoder
       │                     │ Isense             │ A, B
       ▼                     ▼                     ▼
┌─────────────┐       ┌──────────────┐       ┌─────────┐
│ INA219      │       │   Shunt      │       │ Encoder │
│ Current     │       │   Resistor   │       │ Module  │
│ Sensor      │       │   0.1 Ω      │       │ 600 PPR │
└─────────────┘       └──────────────┘       └─────────┘
```

### 3.2 Blok Diagram Closed-Loop Speed Control

```
         ┌───────────────────────────────────────┐
         │                                       │
         │                              Feedback │
         │                              (Encoder)│
         ▼                                       │
   ┌──────────┐      ┌─────┐      ┌────────┐   │   ┌────────┐
   │ Setpoint ├─(+)─►│ PID ├─────►│  PWM   ├───┼──►│ Motor  │
   │ (RPM)    │  │   │ Ctrl│      │ Driver │   │   │   +    │
   └──────────┘  │   └─────┘      └────────┘   │   │ Load   │
                 │                              │   └────────┘
                 │      ┌────────────────┐      │
                 └─(-)──┤ Speed Calc     │◄─────┘
                        │ (encoder ticks)│
                        └────────────────┘
```

### 3.3 Torque-Speed Curve dengan Operating Points

```
Torque (N·m)
   ▲
   │ Tstall ●
   │         ╲
   │          ╲  Rated Point
   │           ╲      ●
   │            ╲    /
   │             ╲  /
   │              ╲/  Max Power Point
   │               ●
   │                ╲
   │                 ╲
   │                  ╲
   └───────────────────●────────► Speed (RPM)
                    ω_no-load
```

### 3.4 PWM Waveform & Spektrum Frekuensi

```
PWM Signal (50% duty):
  ┌──┐  ┌──┐  ┌──┐
  │  │  │  │  │  │  
──┘  └──┘  └──┘  └──   (20 kHz PWM)

FFT Spectrum:
Magnitude
   ▲
   │ ●
   │ │
   │ │    ●
   │ │   / \
   │ │  /   \   ●
   │ │ /     \ / \
   └─┴──────────────────► Frequency (Hz)
     0  20k  40k  60k
     DC  f₀   2f₀  3f₀
```

---

## 4. CONTOH APLIKASI INDUSTRI

### 4.1 Automated Guided Vehicle (AGV)

**Sistem:**
- 2× DC motor 24V dengan encoder 1024 PPR
- Driver: DRV8833 dengan current sensing
- Control: ESP32 dual-core
- Komunikasi: CAN bus

**Spesifikasi:**
- Kecepatan maks: 1.5 m/s
- Payload: 500 kg
- Presisi positioning: ±5 mm
- Battery: 48V LiFePO4

**Strategi Kontrol:**
- Core 0: PID speed control @ 100 Hz
- Core 1: Path planning & komunikasi
- Anti-slip detection via current monitoring
- Regenerative braking untuk efisiensi energi

---

### 4.2 CNC Router

**Sistem:**
- 3× stepper motor NEMA23 (200 steps/rev)
- Driver: TB6600 dengan microstepping 1/8
- Resolusi: 800 steps/mm (dengan ballscrew 5mm pitch)
- Spindle: BLDC 800W dengan ESC

**Parameter:**
- Max feedrate: 3000 mm/min
- Acceleration: 500 mm/s²
- Positioning accuracy: ±0.05 mm
- Repeatability: ±0.02 mm

**Koordinasi Multi-Axis:**
- Trajectory planning dengan S-curve acceleration
- Synchronized motion untuk interpolasi linear/circular
- Backlash compensation via software

---

### 4.3 Robotic Arm (6-DOF)

**Actuator:**
- Joint 1-3: Servo motor dengan gearbox 1:50
- Joint 4-6: BLDC dengan harmonic drive 1:100
- Total payload: 5 kg

**Control Architecture:**
- Joint-space control: Independent PID per joint
- Cartesian-space control: Inverse kinematics
- Torque control: Current-mode untuk compliance

**Safety:**
- Torque limiting via current monitoring
- Collision detection (sudden torque spike)
- Emergency stop dengan dynamic braking

---

### 4.4 Conveyor System

**Komponen:**
- Motor: 3-phase induction 2.2 kW
- VFD (Variable Frequency Drive): 0-50 Hz
- Encoder linear: 0.1 mm resolution
- Load cell: 0-100 kg

**Operasi:**
- Speed synchronization antar conveyor (±0.5%)
- Tension control via closed-loop torque
- Predictive maintenance berdasarkan current signature analysis

**Monitoring:**
- Vibration sensor untuk bearing health
- Thermal sensor di motor winding
- SCADA integration untuk data logging

---

## 5. RANGKUMAN

**Key Takeaways:**

1. **Motor Selection:**
   - DC brushed: Aplikasi sederhana, biaya rendah
   - BLDC: High performance, efisiensi tinggi
   - Servo: Positioning presisi dengan closed-loop terintegrasi
   - Stepper: Open-loop positioning, aplikasi low-speed high-torque

2. **PWM Control:**
   - Frekuensi optimal: 10-20 kHz (inaudible, smooth torque)
   - Resolusi: Minimal 8-bit (256 level) untuk kontrol halus
   - Dead-time: Wajib untuk mencegah shoot-through (1-2 μs)

3. **H-Bridge Driver:**
   - Sign-magnitude vs locked anti-phase PWM
   - Current sensing untuk proteksi dan estimasi torsi
   - Thermal management: derating, heatsink, TSD

4. **Motor Characterization:**
   - Torque-speed curve: Dasar pemilihan motor untuk aplikasi
   - Efisiensi maksimum pada 10-30% rated torque
   - Current sebagai proxy untuk torsi: $T = K_t I$

5. **Encoder Feedback:**
   - Quadrature decoding: 4× resolution
   - Fixed-time vs fixed-count untuk speed measurement
   - Hybrid method untuk akurasi di semua kecepatan

**Roadmap ke Modul Selanjutnya:**
- Modul 6: PID tuning untuk closed-loop speed/position control
- Modul 11: Integrasi motor control dengan ROS2 (velocity commands)
- Modul 12: State machine untuk koordinasi multi-motor

---

## 6. VIDEO PEMBELAJARAN (Panduan Konten)

**Durasi Target:** 12-15 menit  
**Format:** Screen recording + hardware demo

### Segmen 1: Pengenalan Motor & Driver (3 menit)
- Perbandingan fisik: DC brushed, servo, stepper
- Demo: Torque comparison (hand-hold test)
- Buka casing motor: Tunjukkan brush, komutator
- Datasheet walkthrough: Cara membaca spesifikasi motor

### Segmen 2: PWM & H-Bridge (4 menit)
- Oscilloscope: Tampilkan PWM 50%, 25%, 75%
- Animasi: Aliran arus di H-bridge untuk forward/reverse
- Demo: Kontrol kecepatan motor via potentiometer
- FFT analysis: Spektrum frekuensi PWM

### Segmen 3: Karakterisasi Motor (4 menit)
- Setup: Motor, pulley, tali, beban
- Prosedur: Ukur kecepatan vs beban (0-500g)
- Plot real-time: Torque-speed curve
- Analisis: Identifikasi no-load speed, stall torque, rated point

### Segmen 4: Current Sensing & Protection (3 menit)
- Rangkaian: INA219 di ESP32
- Demo: Load motor → lihat current spike
- Implementasi: Threshold detection untuk overcurrent
- Oscilloscope: Tunjukkan inrush current saat start

### Bonus: Troubleshooting (2 menit)
- Motor tidak berputar: Check power, wiring, driver enable
- Motor berputar lambat: Cek voltage drop, PWM duty cycle
- Motor overheat: Thermal imaging, cek duty cycle berlebihan
- Encoder tidak terbaca: Check pull-up resistor, noise filtering

**Catatan Produksi:**
- Gunakan 2 kamera: Close-up hardware + screen recording
- Overlay text untuk formula penting
- Slow-motion untuk PWM waveform di oscilloscope
- Grafik animasi untuk torque-speed curve build-up

---

**Referensi Video Serupa:**
- "Understanding Motor Drivers" - Adafruit (YouTube)
- "PWM Explained" - GreatScott! (YouTube)
- "How to Read a Motor Datasheet" - Pololu (YouTube)

---

*Dokumen ini menjadi fondasi untuk Jobsheet praktikum dan Project.*
