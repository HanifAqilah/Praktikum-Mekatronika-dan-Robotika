# JOBSHEET PRAKTIKUM
## Modul 6: Motion Control & PID

**Durasi:** 300 menit (5 jam)  
**Prasyarat:** Modul 4 (Sensor Fusion - encoder), Modul 5 (Motor Drive)

---

## TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa diharapkan mampu:
1. Mengimplementasikan PID controller untuk speed dan position control
2. Memahami efek individual P, I, dan D term terhadap response
3. Melakukan tuning PID menggunakan metode Ziegler-Nichols
4. Mengimplementasikan anti-windup untuk mencegah integral saturation
5. Menerapkan derivative filtering untuk mengurangi noise amplification
6. Mengukur dan menganalisis step response metrics (rise time, overshoot, settling time)
7. Menguji robustness controller terhadap disturbance

---

## PERALATAN & KOMPONEN

### Hardware
- [x] ESP32 DevKit / Arduino Mega 2560
- [x] Motor DC 12V dengan encoder (dari Modul 5)
- [x] Motor driver L298N / DRV8833
- [x] Power supply 12V 2A
- [x] Potensiometer 10kΩ (untuk setpoint manual)
- [x] Beban variable: Massa gantung (100g, 200g, 500g) atau friction brake
- [x] Oscilloscope (optional, untuk real-time monitoring)
- [x] Breadboard + kabel jumper

### Software
- [x] PlatformIO IDE
- [x] Python 3.x dengan libraries: numpy, matplotlib, pandas
- [x] Serial plotter untuk real-time monitoring
- [x] Git untuk version control

---

## TEORI SINGKAT

**PID Controller Formula (Diskrit):**

$$u[k] = K_p e[k] + K_i \sum_{i=0}^{k} e[i] \Delta t + K_d \frac{e[k] - e[k-1]}{\Delta t}$$

Dimana:
- $u[k]$ = control output (PWM value)
- $e[k]$ = error = setpoint - feedback
- $K_p$ = Proportional gain
- $K_i$ = Integral gain
- $K_d$ = Derivative gain
- $\Delta t$ = sample period

**Step Response Metrics:**
- **Rise Time ($T_r$):** Waktu dari 10% → 90% setpoint
- **Overshoot (OS):** $\frac{peak - steady\_state}{steady\_state} \times 100\%$
- **Settling Time ($T_s$):** Waktu hingga ± 2% dari steady-state
- **Steady-State Error ($e_{ss}$):** $|setpoint - final\_value|$

---

## PERCOBAAN 1: BASIC PID IMPLEMENTATION

**Tujuan:** Mengimplementasikan PID dan menguji efek P, I, D secara independen.

### Langkah Kerja

#### 1.1 Setup Hardware
1. Hubungkan motor & encoder (gunakan setup dari Modul 5)
2. Pastikan encoder counting stabil (test dengan serial monitor)
3. Kalibrasi encoder: 1 revolution = berapa ticks?

#### 1.2 Code Structure
Upload kode `exp1_pid_basic.cpp`.

**PID Class Structure:**
```cpp
class PID {
private:
    float Kp, Ki, Kd;
    float integral;
    float error_prev;
    unsigned long last_time;
    
public:
    PID(float p, float i, float d) : Kp(p), Ki(i), Kd(d) {
        integral = 0;
        error_prev = 0;
        last_time = millis();
    }
    
    float compute(float setpoint, float feedback) {
        unsigned long now = millis();
        float dt = (now - last_time) / 1000.0;  // seconds
        last_time = now;
        
        float error = setpoint - feedback;
        
        // P term
        float p_term = Kp * error;
        
        // I term
        integral += error * dt;
        float i_term = Ki * integral;
        
        // D term
        float d_term = Kd * (error - error_prev) / dt;
        error_prev = error;
        
        return p_term + i_term + d_term;
    }
    
    void reset() {
        integral = 0;
        error_prev = 0;
    }
};
```

#### 1.3 Test P-Only Control

1. Set $K_i = 0$, $K_d = 0$
2. Test berbagai nilai $K_p$: 0.5, 1.0, 2.0, 4.0
3. Untuk setiap $K_p$:
   - Set setpoint = 300 RPM
   - Tunggu hingga steady-state (10 detik)
   - Catat: rise time, overshoot, steady-state RPM

### Data yang Dicatat (P-Only)

| $K_p$ | $K_i$ | $K_d$ | Rise Time (s) | Overshoot (%) | Settling Time (s) | Steady-State Error (RPM) |
|-------|-------|-------|---------------|---------------|-------------------|--------------------------|
| 0.5   | 0     | 0     |               |               |                   |                          |
| 1.0   | 0     | 0     |               |               |                   |                          |
| 2.0   | 0     | 0     |               |               |                   |                          |
| 4.0   | 0     | 0     |               |               |                   |                          |

**Expected Behavior:**
- Low $K_p$ → slow response, large steady-state error
- High $K_p$ → fast response, oscillation, masih ada steady-state error

#### 1.4 Add Integral Term (PI Control)

1. Pilih $K_p$ terbaik dari test P-only (e.g., $K_p = 2.0$)
2. Set $K_d = 0$
3. Test berbagai $K_i$: 0.1, 0.5, 1.0, 2.0
4. Observe: steady-state error elimination

### Data yang Dicatat (PI Control)

| $K_p$ | $K_i$ | $K_d$ | Rise Time (s) | Overshoot (%) | Settling Time (s) | Steady-State Error (RPM) |
|-------|-------|-------|---------------|---------------|-------------------|--------------------------|
| 2.0   | 0.1   | 0     |               |               |                   |                          |
| 2.0   | 0.5   | 0     |               |               |                   |                          |
| 2.0   | 1.0   | 0     |               |               |                   |                          |
| 2.0   | 2.0   | 0     |               |               |                   |                          |

**Expected Behavior:**
- $K_i$ = 0: steady-state error persist
- Small $K_i$: slow elimination of error
- Large $K_i$: fast elimination, tapi large overshoot & oscillation

#### 1.5 Add Derivative Term (PID Control)

1. Pilih $K_p$ dan $K_i$ terbaik dari PI test
2. Test berbagai $K_d$: 0.01, 0.05, 0.1, 0.2
3. Observe: overshoot reduction

### Data yang Dicatat (PID Control)

| $K_p$ | $K_i$ | $K_d$ | Rise Time (s) | Overshoot (%) | Settling Time (s) | Steady-State Error (RPM) |
|-------|-------|-------|---------------|---------------|-------------------|--------------------------|
| 2.0   | 1.0   | 0.01  |               |               |                   |                          |
| 2.0   | 1.0   | 0.05  |               |               |                   |                          |
| 2.0   | 1.0   | 0.1   |               |               |                   |                          |
| 2.0   | 1.0   | 0.2   |               |               |                   |                          |

**Expected Behavior:**
- $K_d$ = 0: overshoot besar
- Small $K_d$: overshoot berkurang
- Large $K_d$: overdamped (sluggish response), noise amplification

### Analisis

1. **Plot semua step response** (P-only vs PI vs PID) dalam satu grafik
2. **Trade-off analysis:**
   - $K_p$ ↑ → response cepat, tapi overshoot & oscillation
   - $K_i$ ↑ → zero steady-state error, tapi overshoot & instability
   - $K_d$ ↑ → damping, tapi noise & slow response
3. **Pertanyaan:**
   - Mengapa P-only tidak bisa mengeliminasi steady-state error?
   - Apa risiko $K_i$ terlalu besar?
   - Dalam kondisi apa D-term tidak diperlukan?

---

## PERCOBAAN 2: ZIEGLER-NICHOLS TUNING

**Tujuan:** Menggunakan metode sistematis untuk mendapatkan starting point PID parameter.

### Langkah Kerja

#### 2.1 Prosedur Ziegler-Nichols (Closed-Loop)

1. **Set $K_i = 0$, $K_d = 0$ (P-only)**
2. **Naikkan $K_p$ secara bertahap:**
   - Start dari $K_p = 0.5$
   - Increment: 0.5, 1.0, 1.5, 2.0, ...
   - Untuk setiap $K_p$:
     - Apply step input (setpoint 0 → 300 RPM)
     - Observe response
     - Check jika oscillation sustained (constant amplitude)

3. **Identifikasi Ultimate Gain ($K_u$):**
   - $K_u$ = nilai $K_p$ saat sistem oscillate dengan amplitude konstan
   - Jangan terlalu tinggi (risiko hardware damage)

4. **Measure Ultimate Period ($P_u$):**
   - $P_u$ = periode satu cycle oscillation (seconds)
   - Gunakan serial plotter atau oscilloscope
   - Hitung dari peak-to-peak atau zero-crossing

5. **Hitung PID parameter:**

| Controller | $K_p$          | $K_i$              | $K_d$              |
|------------|----------------|--------------------|--------------------|
| P          | $0.5 K_u$      | -                  | -                  |
| PI         | $0.45 K_u$     | $1.2 K_p / P_u$    | -                  |
| PID        | $0.6 K_u$      | $2 K_p / P_u$      | $K_p P_u / 8$      |

**Contoh Perhitungan:**
```
Dari experiment:
Ku = 3.0 (system oscillate at Kp = 3.0)
Pu = 1.5 seconds (measured oscillation period)

PID parameter:
Kp = 0.6 × 3.0 = 1.8
Ki = 2 × 1.8 / 1.5 = 2.4
Kd = 1.8 × 1.5 / 8 = 0.3375
```

### Data yang Dicatat

**Step 1: Find Ku**

| $K_p$ | Oscillation? | Amplitude Decay? | Notes                  |
|-------|--------------|------------------|------------------------|
| 0.5   | No           | -                | Sluggish               |
| 1.0   | No           | -                | Slight overshoot       |
| 1.5   | No           | Yes              | Damped oscillation     |
| 2.0   | Yes          | Yes              | Decaying oscillation   |
| 2.5   | Yes          | Constant         | Sustained! → $K_u = 2.5$ |

**Step 2: Measure Pu**
- Screenshot oscillation waveform
- Measure period: $P_u$ = _______ seconds

**Step 3: Calculate PID Parameters**

| Control Type | $K_p$ | $K_i$ | $K_d$ | Formula Used          |
|--------------|-------|-------|-------|-----------------------|
| P (Z-N)      |       | 0     | 0     | $K_p = 0.5 K_u$       |
| PI (Z-N)     |       |       | 0     | Sesuai tabel Z-N      |
| PID (Z-N)    |       |       |       | Sesuai tabel Z-N      |

#### 2.2 Test Ziegler-Nichols Parameters

Upload PID parameter Z-N, test step response.

### Data yang Dicatat

| Metric                | P (Z-N) | PI (Z-N) | PID (Z-N) | Target   |
|-----------------------|---------|----------|-----------|----------|
| Rise Time (s)         |         |          |           | < 2.0    |
| Overshoot (%)         |         |          |           | < 10     |
| Settling Time (s)     |         |          |           | < 3.0    |
| Steady-State Error (RPM) |      |          |           | < 3      |

### Analisis

1. **Effectiveness Z-N:**
   - Apakah parameter Z-N langsung meet requirement?
   - Atau perlu fine-tuning?

2. **Limitations:**
   - Z-N sering agresif (overshoot tinggi)
   - Hanya starting point, bukan final solution

3. **Pertanyaan:**
   - Apa yang terjadi jika $K_u$ overestimate/underestimate?
   - Kapan metode Z-N tidak applicable (e.g., non-oscillatory system)?

---

## PERCOBAAN 3: MANUAL TUNING (FINE-TUNING)

**Tujuan:** Refine Z-N parameter untuk meet performance requirement.

### Langkah Kerja

#### 3.1 Tuning Strategy

**Start:** PID parameter dari Ziegler-Nichols  
**Target:**
- Overshoot < 10%
- Settling time < 2 s
- Steady-state error < 1% (3 RPM untuk setpoint 300 RPM)

**Tuning Sequence:**
1. **Adjust $K_p$ first** (set response speed)
   - If overshoot too large → decrease $K_p$ by 10-20%
   - If response too slow → increase $K_p$ by 10-20%

2. **Adjust $K_i$ second** (eliminate steady-state error)
   - If steady-state error remain → increase $K_i$ by 20-50%
   - If overshoot increase → decrease $K_i$ by 20-50%

3. **Adjust $K_d$ last** (damping)
   - If overshoot large → increase $K_d$ by 20-50%
   - If response sluggish or oscillate → decrease $K_d$ by 20-50%

#### 3.2 Iterative Tuning Log

### Data yang Dicatat

| Iteration | $K_p$ | $K_i$ | $K_d$ | Overshoot (%) | Settling Time (s) | Error (RPM) | Notes                     |
|-----------|-------|-------|-------|---------------|-------------------|-------------|---------------------------|
| Z-N Start | 1.8   | 2.4   | 0.34  | 25            | 3.5               | 0.5         | Baseline (too aggressive) |
| #1        | 1.5   | 2.4   | 0.34  |               |                   |             | Reduce overshoot          |
| #2        | 1.5   | 2.0   | 0.34  |               |                   |             | Reduce Ki untuk stability |
| #3        | 1.5   | 2.0   | 0.5   |               |                   |             | Increase Kd untuk damping |
| #4        |       |       |       |               |                   |             |                           |
| ...       |       |       |       |               |                   |             |                           |
| **Final** |       |       |       | **< 10**      | **< 2.0**         | **< 3**     | **MEETS TARGET**          |

**Tips:**
- Lakukan perubahan satu parameter pada satu waktu
- Jangan lompat terlalu besar (increment 10-20% per iteration)
- Document setiap perubahan dan reasoning

### Analisis

1. **Convergence:**
   - Berapa iterasi diperlukan untuk meet target?
   - Parameter final vs Z-N starting point: berapa % difference?

2. **Sensitivity:**
   - Parameter mana yang paling sensitive terhadap perubahan?
   - Apakah ada trade-off (e.g., overshoot vs settling time)?

3. **Pertanyaan:**
   - Bagaimana tuning berbeda untuk motor berbeda (inertia, friction)?
   - Apa strategi jika tidak bisa meet semua target simultaneously?

---

## PERCOBAAN 4: ANTI-WINDUP IMPLEMENTATION

**Tujuan:** Prevent integral windup saat output saturate.

### Langkah Kerja

#### 4.1 Demonstrasi Windup Problem

1. **Setup:**
   - Gunakan PID tuned dari Percobaan 3
   - Set output saturation: PWM limit = 255 (max)

2. **Test tanpa anti-windup:**
   - Set setpoint sangat tinggi: 500 RPM (assume motor max ~400 RPM)
   - Motor akan saturate di PWM 255
   - Integral term terus mengakumulasi error (windup)
   - Ketika setpoint turun ke 300 RPM → integral perlu waktu lama untuk "unwind"
   - Observe: Large overshoot, slow settling

#### 4.2 Implementasi Anti-Windup

**Method 1: Integral Clamping**
```cpp
// Clamp integral term
const float INTEGRAL_MAX = 255 / Ki;  // Prevent windup beyond saturation
integral += error * dt;
integral = constrain(integral, -INTEGRAL_MAX, INTEGRAL_MAX);
```

**Method 2: Conditional Integration (Back-Calculation)**
```cpp
float output = p_term + i_term + d_term;

// Only integrate if NOT saturated
if (output >= PWM_MIN && output <= PWM_MAX) {
    integral += error * dt;
} else {
    // Back-calculate integral to match saturated output
    float saturated_output = constrain(output, PWM_MIN, PWM_MAX);
    integral = (saturated_output - p_term - d_term) / Ki;
}
```

#### 4.3 Comparison Test

**Test Scenario:**
1. t = 0-5s: Setpoint 0 → 500 RPM (saturate motor)
2. t = 5-10s: Hold at 500 RPM (windup accumulates)
3. t = 10s: Drop setpoint to 300 RPM
4. Observe recovery behavior

### Data yang Dicatat

| Anti-Windup Method      | Max Overshoot (%) | Settling Time (s) | Integral Max Value | Recovery Smooth? |
|-------------------------|-------------------|-------------------|--------------------|------------------|
| None (unlimited)        |                   |                   |                    |                  |
| Integral Clamping       |                   |                   |                    |                  |
| Back-Calculation        |                   |                   |                    |                  |

**Plot:** Overlay response curves (without vs with anti-windup)

### Analisis

1. **Windup Effect:**
   - Berapa besar overshoot saat integral unlimited?
   - Berapa lama settling time tanpa anti-windup?

2. **Anti-Windup Effectiveness:**
   - Method mana yang paling effective? (clamping vs back-calculation)
   - Apakah ada trade-off?

3. **Pertanyaan:**
   - Kapan windup menjadi masalah kritis?
   - Bagaimana menentukan `INTEGRAL_MAX` optimal?

---

## PERCOBAAN 5: DERIVATIVE FILTER

**Tujuan:** Reduce noise amplification dari derivative term.

### Langkah Kerja

#### 5.1 Demonstrate Noise Amplification

1. **Inject sensor noise:**
   - Simulate noise: `feedback_noisy = feedback_real + random(-10, 10)`
   - Or: Fisik vibration (tap encoder shaft gently)

2. **Observe:**
   - Raw derivative: $D_{raw} = \frac{e[k] - e[k-1]}{dt}$
   - D-term amplifies noise → motor jerky, oscillation

#### 5.2 Implement Low-Pass Filter

**Method 1: Filtered Error (before derivative)**
```cpp
// Low-pass filter on error signal
const float ERROR_FILTER_ALPHA = 0.3;
error_filtered = alpha * error + (1 - alpha) * error_filtered_prev;

// Derivative of filtered error
float derivative = (error_filtered - error_filtered_prev) / dt;
```

**Method 2: Filtered Derivative (after derivative)**
```cpp
// Raw derivative
float derivative_raw = (error - error_prev) / dt;

// Low-pass filter on derivative
const float D_FILTER_ALPHA = 0.3;
derivative_filtered = alpha * derivative_raw + (1 - alpha) * derivative_filtered_prev;
```

**Alpha Calculation:**
$$\alpha = \frac{\Delta t}{\Delta t + \tau}$$

Dimana $\tau$ = filter time constant (0.01 - 0.1 seconds typical)

#### 5.3 Comparison Test

Test with different $\alpha$ values: 0.1, 0.3, 0.5, 0.8

### Data yang Dicatat

| D-Filter $\alpha$ | Motor Jerkiness | Control Stability | Response Speed | Noise Rejection |
|-------------------|-----------------|-------------------|----------------|-----------------|
| None (raw)        | Very high       | Poor              | Fast           | None            |
| 0.1               |                 |                   |                |                 |
| 0.3               |                 |                   |                |                 |
| 0.5               |                 |                   |                |                 |
| 0.8               |                 |                   |                |                 |

**Scale:** Low/Medium/High

### Analisis

1. **Trade-off:**
   - Low $\alpha$ (strong filter) → smooth tapi sluggish response
   - High $\alpha$ (weak filter) → fast tapi noise amplification

2. **Optimal $\alpha$:**
   - Bagaimana memilih $\alpha$ optimal?
   - Tergantung: noise level, desired response speed

3. **Pertanyaan:**
   - Apa alternative selain low-pass filter? (e.g., median filter, Kalman)
   - Apakah derivative filtering mengurangi effectiveness D-term?

---

## PERCOBAAN 6: FEEDFORWARD CONTROL (OPTIONAL)

**Tujuan:** Add model-based compensation untuk improve response.

### Langkah Kerja

#### 6.1 Estimate Feedforward Gain

**For velocity control:**
$$u_{ff} = K_{ff} \times \omega_{setpoint}$$

**Estimate $K_{ff}$:**
1. Run motor open-loop at various PWM values
2. Measure steady-state speed
3. Plot PWM vs Speed → linear regression
4. $K_{ff}$ = slope of line (PWM / RPM)

**Example:**
```
PWM | Speed (RPM)
100 | 150
150 | 225
200 | 300
255 | 380

Linear fit: Speed = 1.5 × PWM
Therefore: Kff = 1 / 1.5 = 0.67 (PWM per RPM)
```

#### 6.2 Implement PID + Feedforward

```cpp
float output_ff = Kff * setpoint;
float output_pid = pid.compute(setpoint, feedback);
float output_total = output_ff + output_pid;
output_total = constrain(output_total, 0, 255);
```

### Data yang Dicatat

| Control Type        | Settling Time (s) | Steady-State Error (RPM) | PID Burden (avg error) |
|---------------------|-------------------|--------------------------|------------------------|
| PID only            |                   |                          |                        |
| PID + Feedforward   |                   |                          |                        |

### Analisis

1. **Improvement:**
   - Berapa % reduction di settling time?
   - Apakah steady-state error lebih kecil?

2. **PID Burden:**
   - Average error selama transient lebih kecil?
   - Integral accumulation berkurang?

---

## PERCOBAAN 7: STEP RESPONSE MEASUREMENT & ANALYSIS

**Tujuan:** Full characterization PID performance dengan parameter final.

### Langkah Kerja

#### 7.1 Setup

1. Gunakan final tuned PID parameter (dari Percobaan 3)
2. Implement anti-windup & derivative filter
3. Setup data logging:
   - Sample rate: 100 Hz (10 ms period)
   - Duration: 15 seconds
   - Log: time, setpoint, feedback, error, output

#### 7.2 Test Procedure

**Step Input:**
1. t = 0-2s: Motor idle (0 RPM)
2. t = 2s: Step setpoint 0 → 300 RPM
3. t = 2-15s: Record response
4. Calculate metrics

**Metrics Calculation:**
```python
# Rise time: 10% to 90% of setpoint
t_10 = time when feedback >= 0.1 * setpoint
t_90 = time when feedback >= 0.9 * setpoint
rise_time = t_90 - t_10

# Overshoot
peak_value = max(feedback)
overshoot_pct = (peak_value - setpoint) / setpoint * 100

# Settling time: within ±2% of setpoint
settling_band = setpoint * 0.02
t_settle = time when |feedback - setpoint| < settling_band (and stays)

# Steady-state error
steady_state_error = setpoint - mean(feedback[last_1_second])
```

### Data yang Dicatat

| Metric                     | Value  | Target | Status  |
|----------------------------|--------|--------|---------|
| Rise Time (s)              |        | < 2.0  | ✓ / ✗   |
| Peak Time (s)              |        | -      | -       |
| Overshoot (%)              |        | < 10   | ✓ / ✗   |
| Settling Time (s)          |        | < 3.0  | ✓ / ✗   |
| Steady-State Error (RPM)   |        | < 3    | ✓ / ✗   |

**Plot:**
- Time-domain: Setpoint vs Feedback vs Error
- Mark: Rise time, peak time, settling time
- Annotate: Overshoot percentage

### Analisis

1. **Performance:**
   - Meet all targets? If not, which metric fail?
   - Limiting factor?

2. **Comparison:**
   - Final tuned vs Z-N starting point: improvement?

3. **Production Ready:**
   - Apakah parameter ini production-ready?
   - Safety margin?

---

## PERCOBAAN 8: ROBUSTNESS TEST

**Tujuan:** Test PID performance dengan external disturbance (load change).

### Langkah Kerja

#### 8.1 Test Scenario

**Timeline:**
```
t = 0-2s:    Motor acceleration 0 → 300 RPM
t = 2-5s:    Steady-state, no load
t = 5s:      ADD LOAD (sudden disturbance)
             - Hanging mass: 200g
             - Or: Apply friction brake
t = 5-10s:   PID compensate → recover to 300 RPM
t = 10s:     REMOVE LOAD (sudden disturbance)
t = 10-15s:  PID compensate → settle back to 300 RPM
```

#### 8.2 Measurement

Log:
- Speed dip saat load added
- Recovery time to ±5% setpoint
- Speed spike saat load removed
- Settling time

### Data yang Dicatat

| Event                   | Speed Before (RPM) | Speed Dip/Spike (RPM) | % Deviation | Recovery Time (s) |
|-------------------------|--------------------|-----------------------|-------------|-------------------|
| Add load at t=5s        | 300                |                       |             |                   |
| Remove load at t=10s    | 300                |                       |             |                   |

**Plot:** Speed vs Time (dengan marking load add/remove events)

### Analisis

1. **Disturbance Rejection:**
   - Berapa lama PID recovery dari disturbance?
   - Apakah overshoot saat recovery?

2. **Robustness:**
   - Apakah PID stable di berbagai load condition?
   - Parameter tuning perlu disesuaikan untuk different load?

3. **Real-World Applicability:**
   - Untuk aplikasi conveyor (load variation): acceptable?
   - Untuk robot arm (dynamic payload): perlu adaptive tuning?

---

## DELIVERABLES

### 1. Laporan Praktikum (PDF)
**Struktur:**
- **Cover**
- **Abstrak** (150-200 kata)
- **Pendahuluan**
  - Tujuan praktikum
  - Teori PID (ringkas)
- **Metodologi**
  - Hardware setup (diagram)
  - Firmware architecture
  - Tuning procedure
- **Hasil & Analisis** (semua percobaan)
  - Tabel data
  - Grafik step response (minimal 8 grafik)
  - Ziegler-Nichols calculation
  - Manual tuning log
  - Performance metrics
  - Robustness test result
- **Kesimpulan**
  - Key findings
  - Final PID parameter recommendation
  - Lessons learned
- **Referensi**

### 2. Source Code (Git Repository)
```
Modul-06_PID_Control/
├── src/
│   ├── exp1_pid_basic.cpp
│   ├── exp2_ziegler_nichols.cpp
│   ├── exp3_manual_tuning.cpp
│   ├── exp4_anti_windup.cpp
│   ├── exp5_derivative_filter.cpp
│   ├── exp6_feedforward.cpp
│   ├── exp7_step_response.cpp
│   └── exp8_robustness_test.cpp
├── lib/
│   └── PID/
│       ├── PID.h
│       └── PID.cpp
├── analysis/
│   ├── plot_step_response.py
│   ├── calculate_metrics.py
│   └── compare_tuning.py
└── data/
    ├── exp1_p_only.csv
    ├── exp1_pi.csv
    ├── exp1_pid.csv
    ├── exp2_ziegler_nichols.csv
    ├── exp7_final_response.csv
    └── exp8_robustness.csv
```

### 3. Video Demo (MP4, max 5 menit)
**Konten:**
- Tuning process (time-lapse)
- Step response comparison (P vs PI vs PID)
- Anti-windup demonstration
- Robustness test (load disturbance)
- Narasi key findings

### 4. Tuning Guide Document (PDF, 2-3 halaman)
**Konten:**
- Quick reference untuk tuning PID
- Flowchart tuning strategy
- Troubleshooting tips
- Recommended parameter ranges untuk motor yang digunakan

---

## RUBRIK PENILAIAN

| Aspek                          | Bobot | Kriteria                                                               |
|--------------------------------|-------|------------------------------------------------------------------------|
| **Hardware Setup**             | 5%    | Proper wiring, stable encoder reading, no electrical issues            |
| **Firmware Implementation**    | 30%   | All experiments working, code clean & modular, proper PID class        |
| **Data Collection**            | 15%   | Complete data for all 8 experiments, accurate measurement              |
| **Grafik & Visualisasi**       | 15%   | Minimal 8 grafik, clear axis labels, legend, annotations               |
| **Tuning Process**             | 15%   | Systematic tuning, documented iterations, convergence to target        |
| **Analisis & Insight**         | 15%   | Deep analysis, interpretation, answer critical questions               |
| **Documentation**              | 5%    | Clear report, reproducible, proper references                          |

**Total:** 100%

**Bonus (+15%):**
- Adaptive PID tuning (gain scheduling based on setpoint)
- Cascade control implementation (position with velocity inner loop)
- Real-time web dashboard untuk tuning interface
- Auto-tuning algorithm implementation (relay feedback method)

---

## TROUBLESHOOTING GUIDE

### PID oscillating uncontrollably
- [ ] Reduce Kp by 50%
- [ ] Check Ki - set to 0 temporarily
- [ ] Verify encoder reading (stable?)
- [ ] Check sample rate (too slow?)

### Steady-state error not eliminated
- [ ] Increase Ki gradually
- [ ] Check integral term accumulation (print to serial)
- [ ] Verify anti-windup not too aggressive

### Motor jerky/noisy
- [ ] Implement derivative filter
- [ ] Reduce Kd
- [ ] Check encoder noise (hardware filter?)
- [ ] Verify PWM frequency (should be >10 kHz)

### Cannot meet performance targets
- [ ] Re-characterize motor (different from expected?)
- [ ] Check mechanical issues (friction, binding?)
- [ ] Review target feasibility (too aggressive?)
- [ ] Consider cascade control atau feedforward

---

## REFERENSI

1. **Åström, K. J., & Hägglund, T. (2006).** *Advanced PID Control*. ISA.
2. **Ziegler, J. G., & Nichols, N. B. (1942).** "Optimum Settings for Automatic Controllers." *Transactions of the ASME*, 64, 759-768.
3. **Visioli, A. (2006).** *Practical PID Control*. Springer.
4. **Brett Beauregard - Arduino PID Library**  
   URL: https://github.com/br3ttb/Arduino-PID-Library

---

**Instruktur:** [Nama]  
**Kontak:** [Email]  
**Office Hours:** [Jadwal]

---

*Selamat praktikum! PID tuning adalah seni sekaligus sains - enjoy the learning process!* 🎯
