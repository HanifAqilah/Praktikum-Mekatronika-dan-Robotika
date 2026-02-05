# MATERI PEMBELAJARAN
## Modul 6: Motion Control & PID

**Tujuan Pembelajaran:**
- Memahami prinsip dasar feedback control system
- Menguasai teori dan implementasi PID controller
- Mampu melakukan tuning PID dengan metode sistematis
- Memahami anti-windup dan derivative filtering
- Mengimplementasikan trajectory planning untuk motion control

---

## 1. DEFINISI & KONSEP DASAR

### 1.1 Closed-Loop Control System

Sistem kontrol closed-loop (feedback control) adalah sistem yang menggunakan pengukuran output aktual untuk memperbaiki error terhadap setpoint yang diinginkan.

**Komponen Utama:**
```
         ┌─────────────────────────────────────┐
         │                                     │
         │                           Feedback  │
         ▼                                     │
    ┌────────┐      ┌──────────┐      ┌──────┴──────┐
    │Setpoint├─(+)─►│Controller├─────►│   Plant     │
    │  r(t)  │  │   │  C(s)    │ u(t) │   G(s)      │
    └────────┘  │   └──────────┘      └──────┬──────┘
                │                            │ y(t)
                │      ┌─────────┐           │
                └─(-)──┤ Sensor  │◄──────────┘
                       │  H(s)   │
                       └─────────┘
         error e(t) = r(t) - y(t)
```

**Keuntungan Closed-Loop:**
- Mengurangi efek disturbance
- Meningkatkan akurasi
- Menstabilkan sistem yang inherently unstable
- Kompensasi variasi parameter (aging, temperature)

**Kelemahan:**
- Kompleksitas tinggi
- Biaya sensor
- Potensi instabilitas jika tuning buruk

---

### 1.2 PID Controller Fundamentals

PID (Proportional-Integral-Derivative) adalah algoritma kontrol paling umum di industri (~95% control loops).

**Persamaan PID dalam Time Domain:**

$$u(t) = K_p e(t) + K_i \int_0^t e(\tau) d\tau + K_d \frac{de(t)}{dt}$$

Dimana:
- $u(t)$ = control signal (output controller)
- $e(t)$ = error = setpoint - measurement
- $K_p$ = Proportional gain
- $K_i$ = Integral gain
- $K_d$ = Derivative gain

**Bentuk Diskrit (untuk implementasi digital):**

$$u[k] = K_p e[k] + K_i \sum_{i=0}^{k} e[i] \Delta t + K_d \frac{e[k] - e[k-1]}{\Delta t}$$

Dimana:
- $k$ = time step index
- $\Delta t$ = sample period (seconds)

---

### 1.3 PID Components - Physical Interpretation

#### Proportional Term ($P$)
**Formula:** $u_p = K_p \cdot e(t)$

**Karakteristik:**
- Respons proporsional terhadap error saat ini
- Makin besar error, makin besar output
- Memberikan respons cepat
- **Kelemahan:** Tidak bisa mengeliminasi steady-state error

**Analogi:** Seperti sopir yang memutar setir sebanding dengan jarak mobil dari center lane.

**Tuning Guideline:**
- $K_p$ terlalu rendah → respons lambat, error besar
- $K_p$ terlalu tinggi → overshoot, oscillation
- $K_p$ optimal → fast response tanpa excessive overshoot

---

#### Integral Term ($I$)
**Formula:** $u_i = K_i \int_0^t e(\tau) d\tau$

**Karakteristik:**
- Akumulasi error sepanjang waktu
- Mengeliminasi steady-state error
- "Memory" dari sistem
- **Kelemahan:** Integral windup (akumulasi saat saturasi)

**Analogi:** Sopir yang mengingat total deviasi dan koreksi bertahap.

**Tuning Guideline:**
- $K_i$ = 0 → steady-state error permanen
- $K_i$ terlalu rendah → eliminasi error lambat
- $K_i$ terlalu tinggi → overshoot besar, oscillation
- $K_i$ optimal → zero steady-state error tanpa overshoot berlebih

---

#### Derivative Term ($D$)
**Formula:** $u_d = K_d \frac{de(t)}{dt}$

**Karakteristik:**
- Respons terhadap *rate of change* error
- Prediksi error masa depan
- Damping → mengurangi overshoot & oscillation
- **Kelemahan:** Amplifikasi noise measurement

**Analogi:** Sopir yang memprediksi lintasan masa depan berdasarkan kecepatan deviasi.

**Tuning Guideline:**
- $K_d$ = 0 → overshoot tinggi, oscillatory
- $K_d$ terlalu rendah → masih ada overshoot
- $K_d$ terlalu tinggi → sluggish response, noise amplification
- $K_d$ optimal → critically damped (minimal overshoot, fast settling)

---

### 1.4 Control Response Metrics

**Step Response Characteristics:**

```
Output y(t)
   ▲
   │      ╱‾‾‾‾‾‾‾‾‾‾‾‾‾‾  ← Steady-state
   │     ╱   ╱╲ Overshoot
   │    ╱   ╱  ╲
   │   ╱   ╱    ╲___________
   │  ╱   ╱  
   │ ╱___╱ Rise time
   │╱
   └─────────────────────────► Time
     Tr  Tp  Ts
```

**Key Metrics:**

1. **Rise Time ($T_r$):**
   - Waktu dari 10% → 90% setpoint
   - Makin kecil $K_p$ → makin lambat rise time

2. **Peak Time ($T_p$):**
   - Waktu mencapai overshoot pertama
   - Terkait dengan natural frequency sistem

3. **Overshoot (OS):**
   $$OS = \frac{y_{peak} - y_{steady}}{y_{steady}} \times 100\%$$
   - Acceptable: < 10% untuk banyak aplikasi
   - Critical systems: < 5%

4. **Settling Time ($T_s$):**
   - Waktu hingga output tetap dalam ±2% (atau ±5%) dari steady-state
   - Target: Sesingkat mungkin tanpa overshoot berlebih

5. **Steady-State Error ($e_{ss}$):**
   $$e_{ss} = \lim_{t \to \infty} |r(t) - y(t)|$$
   - Ideal: 0 (dicapai dengan I-term)

---

### 1.5 PID Tuning Methods

#### Manual Tuning
Prosedur iteratif:
1. Set $K_i = 0$, $K_d = 0$
2. Naikkan $K_p$ hingga oscillation konstan → $K_p^* $
3. Kurangi $K_p$ ke 50-70% dari $K_p^*$
4. Naikkan $K_i$ hingga steady-state error hilang
5. Tambahkan $K_d$ untuk kurangi overshoot (optional)

---

#### Ziegler-Nichols Method (Closed-Loop)
**Langkah:**
1. Set $K_i = 0$, $K_d = 0$
2. Naikkan $K_p$ hingga sistem oscillate konstan (marginal stability)
   - $K_p = K_u$ (ultimate gain)
   - $T_u$ = periode oscillation
3. Hitung gain berdasarkan tabel:

| Controller Type | $K_p$      | $K_i$         | $K_d$         |
|-----------------|------------|---------------|---------------|
| P               | $0.5 K_u$  | -             | -             |
| PI              | $0.45 K_u$ | $1.2K_p/T_u$  | -             |
| PID             | $0.6 K_u$  | $2K_p/T_u$    | $K_p T_u / 8$ |

**Catatan:** Metode ini agresif, sering menghasilkan overshoot besar. Perlu fine-tuning.

---

#### Ziegler-Nichols Method (Open-Loop / Reaction Curve)
**Langkah:**
1. Terapkan step input ke plant (open-loop)
2. Ukur response curve
3. Cari dua parameter:
   - $L$ = dead time (lag sebelum respons)
   - $T$ = time constant (63% rise time)
4. Hitung:

| Controller Type | $K_p$       | $T_i$ (reset time) | $T_d$ (derivative time) |
|-----------------|-------------|--------------------|-------------------------|
| P               | $T/(L \cdot K)$ | -              | -                       |
| PI              | $0.9T/(L \cdot K)$ | $L/0.3$     | -                       |
| PID             | $1.2T/(L \cdot K)$ | $2L$        | $0.5L$                  |

Dimana $K$ = plant gain = $\frac{\Delta y}{\Delta u}$

---

#### Cohen-Coon Method
Lebih akurat untuk sistem dengan large dead time.

$$K_p = \frac{T}{L \cdot K} \left(1 + \frac{L}{3T}\right)$$

$$T_i = L \left(\frac{30 + 3(L/T)}{9 + 20(L/T)}\right)$$

$$T_d = L \left(\frac{4}{11 + 2(L/T)}\right)$$

---

### 1.6 Anti-Windup

**Masalah Integral Windup:**
Saat output controller saturate (mencapai limit fisik, e.g., PWM 255), error terus terakumulasi di integral term. Ketika setpoint tercapai, butuh waktu lama untuk "unwind" integral → overshoot besar.

**Solusi: Conditional Integration**

```cpp
// Only accumulate integral if NOT saturated
if (output >= min_output && output <= max_output) {
    integral += error * dt;
} else {
    // Clamp integral (back-calculation)
    integral = (output - (Kp * error + Kd * derivative)) / Ki;
}
```

**Alternatif: Integral Clamping**
```cpp
integral += error * dt;
integral = constrain(integral, integral_min, integral_max);
```

---

### 1.7 Derivative Filtering

**Masalah:** Derivative term amplifies high-frequency noise.

**Solusi: Low-pass filter pada derivative**

**Bentuk Diskrit:**
$$D_{filtered}[k] = \alpha \cdot D_{raw}[k] + (1 - \alpha) \cdot D_{filtered}[k-1]$$

Dimana:
$$\alpha = \frac{\Delta t}{\Delta t + \tau}$$

$\tau$ = filter time constant (typical: 0.01 - 0.1 seconds)

**Implementasi:**
```cpp
float derivative = (error - last_error) / dt;
derivative_filtered = alpha * derivative + (1 - alpha) * derivative_filtered;
output_d = Kd * derivative_filtered;
```

---

## 2. KONSEP INTI

### 2.1 Velocity vs Position Control

#### Velocity Control
**Setpoint:** Target speed (RPM, m/s)  
**Sensor:** Encoder (calculated speed from delta ticks)  
**Aplikasi:** Conveyor, fan, pump

**PID Formula:**
$$u = K_p (\omega_{target} - \omega_{actual}) + K_i \int ... + K_d \frac{d(\omega_{target} - \omega_{actual})}{dt}$$

**Karakteristik:**
- Derivative of velocity = acceleration
- Kd term helps smooth acceleration/deceleration

---

#### Position Control
**Setpoint:** Target position (degrees, mm)  
**Sensor:** Encoder (integrated ticks → position)  
**Aplikasi:** Robot arm, CNC, actuator

**PID Formula:**
$$u = K_p (x_{target} - x_{actual}) + K_i \int ... + K_d \frac{d(x_{target} - x_{actual})}{dt}$$

**Karakteristik:**
- Derivative of position = velocity
- Kd term provides velocity damping
- Often requires velocity feedforward for tracking moving setpoint

---

### 2.2 Feedforward Control

Menambahkan komponen control yang *tidak* bergantung pada error, tapi prediksi kebutuhan based on model.

**Kombinasi PID + Feedforward:**
$$u = u_{ff} + u_{pid}$$

**Contoh: Velocity Control dengan Feedforward**
$$u = K_{ff} \cdot \omega_{target} + K_p (\omega_{target} - \omega_{actual}) + ...$$

$K_{ff}$ dihitung dari karakterisasi motor: inverse of motor gain.

**Keuntungan:**
- Mengurangi beban PID (terutama I-term)
- Respons lebih cepat untuk tracking setpoint berubah
- Steady-state error lebih kecil

---

### 2.3 Cascade Control

Dua loop PID tersarang: Inner loop (fast) + Outer loop (slow).

**Contoh: Position Control dengan Velocity Inner Loop**

```
Position    ┌────────┐     Velocity    ┌────────┐     PWM
Setpoint───►│PID_pos ├────►Setpoint───►│PID_vel ├────►Motor
   (mm)     └───┬────┘        (RPM)    └───┬────┘
                │                          │
             Position                   Velocity
             Feedback◄──────────────Feedback
             (encoder)              (encoder derivative)
```

**Keuntungan:**
- Inner loop cepat → handling disturbance langsung
- Outer loop lambat → tracking posisi akurat
- Lebih stabil dibanding single PID untuk high-order plant

**Tuning:**
1. Tune inner loop (velocity) dulu hingga stabil
2. Tune outer loop (position) dengan inner loop active

---

### 2.4 Trajectory Planning

Alih-alih step command (instant change), gunakan trajectory smooth untuk mencegah mechanical shock.

#### Trapezoidal Velocity Profile

```
Velocity
   ▲
   │    ┌─────────────┐  ← Vmax
   │   ╱│             │╲
   │  ╱ │             │ ╲
   │ ╱  │             │  ╲
   └┴───┴─────────────┴───┴──► Time
   Accel Constant    Decel
     T1    T2          T3
```

**Fase:**
1. **Acceleration:** $v(t) = a \cdot t$
2. **Constant velocity:** $v(t) = v_{max}$
3. **Deceleration:** $v(t) = v_{max} - a \cdot (t - T_2)$

**Parameter:**
- $a$ = max acceleration (m/s² atau RPM/s)
- $v_{max}$ = max velocity
- $d$ = total distance

**Algoritma:**
```cpp
float T1 = vmax / accel;               // Accel time
float d_accel = 0.5 * accel * T1 * T1; // Distance during accel
float d_const = total_distance - 2 * d_accel;
float T2 = d_const / vmax;             // Const velocity time
float T3 = T1;                         // Decel time (symmetric)
```

---

#### S-Curve (Jerk-Limited) Profile

Untuk aplikasi high-precision, limit jerk (derivative of acceleration).

**Keuntungan:**
- Smooth acceleration → less mechanical stress
- Vibration reduction
- Akurasi positioning lebih baik

**Implementasi:** Lebih kompleks, biasa via lookup table atau polynomial interpolation.

---

## 3. DIAGRAM & VISUALISASI

### 3.1 PID Block Diagram dengan Anti-Windup

```
Setpoint ─────(+)────┬────► Kp ──────┬──────(+)──────► Output
  r(t)        │      │               │       │      (saturation)
              │      ├────► Ki ──────┤       │
         (-)  │      │     (integral)│       │
              │      │         ▲     │       │
              │      │         │     │       ▼
              │      └────► Kd ──────┘   [min, max]
              │           (filtered)         │
              │                              │
        Measurement                    Anti-Windup
         y(t) ◄────────────────────Back-calculation
```

### 3.2 Step Response - Effect of Each Term

```
(a) P-only: Fast rise, steady-state error
Output
  ▲
  │    ╱‾‾‾‾‾ ← Steady-state < Setpoint
  │   ╱
  │  ╱
  └─────────► Time

(b) PI: Zero steady-state error, overshoot
Output
  ▲     ╱‾‾‾‾‾‾
  │    ╱ ╱╲
  │   ╱ ╱  ╲
  │  ╱ ╱    ‾‾‾
  └─────────► Time

(c) PID: Fast, minimal overshoot
Output
  ▲    ╱‾‾‾‾‾‾
  │   ╱╱
  │  ╱╱
  │ ╱╱
  └─────────► Time
```

### 3.3 Frequency Response (Bode Plot)

```
Magnitude (dB)
  ▲
  │        P+I (low-pass)
  │    ╱
  │   ╱
  │  ╱________ ← -20dB/dec (I-term)
  │           ╲
  │            ╲_____ ← D-term boost high-freq
  └───────────────────► Frequency (Hz)
```

**Interpretasi:**
- I-term: Gain tinggi di low freq → eliminasi steady-state error
- D-term: Gain di high freq → phase lead, stability margin

---

## 4. CONTOH APLIKASI INDUSTRI

### 4.1 Temperature Control (HVAC)

**Plant:** Heating element + thermal mass  
**Sensor:** Thermocouple / RTD  
**Actuator:** PWM to solid-state relay (SSR)

**Characteristics:**
- Slow dynamics (time constant: minutes)
- Large dead-time (thermal lag)
- Integral windup common (heater saturate 100%)

**Tuning:**
- PI controller (no D-term, noise amplification issue)
- $K_p$ = 5-10% of full power per °C error
- $K_i$ = 0.01 - 0.1 (slow integration)
- Anti-windup essential

---

### 4.2 Robot Joint Control

**Plant:** Motor + gearbox + link inertia  
**Sensor:** Encoder (position) + current sensor (torque proxy)  
**Actuator:** Motor driver (PWM)

**Characteristics:**
- Fast dynamics (time constant: milliseconds)
- Nonlinear (friction, backlash)
- Gravity torque disturbance

**Control Strategy:**
- Cascade PID: Outer (position) + Inner (velocity atau torque)
- Feedforward: Gravity compensation, inertia compensation
- Trajectory planning: Trapezoidal velocity

**Tuning:**
- Inner velocity loop: $K_p$ = 0.5-2.0, $K_i$ = 0.1-0.5, $K_d$ = 0
- Outer position loop: $K_p$ = 10-50, $K_i$ = 0, $K_d$ = 1-5

---

### 4.3 Cruise Control (Automotive)

**Plant:** Vehicle dynamics (inertia, drag, slope)  
**Sensor:** Wheel speed sensor  
**Actuator:** Throttle (electronic throttle body)

**Characteristics:**
- Disturbance: Wind, road grade, traffic
- Safety-critical: Must be stable in all conditions
- Multiple operating points (speed 50-120 km/h)

**Control:**
- PI controller (D-term not needed, speed noise)
- Gain scheduling: Different PID gains for different speeds
- Adaptive: Learn road grade and adjust feedforward

---

### 4.4 Quadcopter Altitude Hold

**Plant:** Propeller thrust + air drag + gravity  
**Sensor:** Barometer + accelerometer (sensor fusion)  
**Actuator:** Motor ESC (electronic speed controller)

**Characteristics:**
- Unstable open-loop (gravity disturbance)
- Fast response required (10-50 Hz control loop)
- Sensor noise (barometer drift, vibration)

**Control:**
- Cascade: Outer (altitude) + Inner (vertical velocity)
- Complementary filter untuk sensor fusion
- Anti-windup untuk thrust saturation (max throttle)

**Tuning:**
- Inner loop: $K_p$ = 5, $K_i$ = 0.5, $K_d$ = 0.5
- Outer loop: $K_p$ = 2, $K_i$ = 0.1, $K_d$ = 1

---

## 5. RANGKUMAN

**Key Takeaways:**

1. **PID Components:**
   - **P:** Fast response, tapi steady-state error
   - **I:** Eliminasi steady-state error, tapi risk of windup & overshoot
   - **D:** Damping, reduce overshoot, tapi amplify noise

2. **Tuning Strategy:**
   - Start dengan P-only → cari $K_p$ optimal
   - Tambahkan I untuk zero steady-state error
   - Tambahkan D untuk reduce overshoot (optional)
   - Fine-tune berdasarkan step response metrics

3. **Advanced Techniques:**
   - **Anti-windup:** Prevent integral overshoot saat saturasi
   - **Derivative filtering:** Reduce noise amplification
   - **Feedforward:** Improve tracking, reduce PID burden
   - **Cascade:** Multi-loop untuk complex plant

4. **Trajectory Planning:**
   - Trapezoidal velocity: Smooth motion, reduce mechanical shock
   - S-curve: Jerk limiting untuk ultra-precision

5. **Implementation Tips:**
   - Sample rate: 5-10× bandwidth sistem (e.g., 100 Hz untuk 10 Hz plant)
   - Use fixed time-step untuk konsistensi integral/derivative
   - Log data untuk analisis offline (tuning iteratif)

**Common Pitfalls:**
- Derivative kick: Sudden setpoint change → spike di D-term (solution: derivative on measurement only)
- Integral windup: Tidak implement anti-windup
- Noise amplification: $K_d$ terlalu tinggi tanpa filtering
- Wrong sample rate: Terlalu lambat → poor control, terlalu cepat → computational burden

**Next Steps (Batch Selanjutnya):**
- Modul 7: Mechanism design untuk motion transmission
- Modul 11: Integration dengan ROS2 untuk high-level control
- Modul 12: State machine untuk multi-mode control

---

## 6. VIDEO PEMBELAJARAN (Panduan Konten)

**Durasi Target:** 15-18 menit  
**Format:** Whiteboard animation + live demo

### Segmen 1: PID Theory (5 menit)
- Whiteboard: Gambar block diagram closed-loop
- Animasi: Efek P, I, D secara terpisah pada step response
- Real example: Thermostat di rumah (relatable)
- Show persamaan PID, highlight setiap term

### Segmen 2: Tuning Demonstration (6 menit)
- Live setup: Motor + encoder + oscilloscope
- Tuning P-only: Show overshoot reduction as Kp decrease
- Add I-term: Demonstrate steady-state error elimination
- Add D-term: Show damping effect
- Plot real-time: Setpoint vs actual position

### Segmen 3: Advanced Features (4 menit)
- Integral windup demo: Saturate motor → show overshoot
- Anti-windup implementation → show improvement
- Derivative noise: Inject sensor noise → show amplification
- Low-pass filter → show smoothing

### Segmen 4: Real Application (3 menit)
- Trajectory planning: Point-to-point motion dengan trapezoidal velocity
- Compare: Step input vs smooth trajectory
- Measure: Overshoot, settling time, positioning accuracy

**Catatan Produksi:**
- Picture-in-picture: Instructor + oscilloscope screen
- Overlay graphs: Real-time plotting dengan annotation
- Slow-motion untuk transient response (overshoot peak)
- Side-by-side comparison: Bad tuning vs Good tuning

---

*Dokumen ini menjadi fondasi untuk Jobsheet praktikum dan Project.*
