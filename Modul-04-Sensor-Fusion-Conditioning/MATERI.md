# MODUL 4: SENSOR FUSION & CONDITIONING (ADVANCED)

## 1. DEFINISI

### Sensor Conditioning
**Sensor conditioning** adalah proses mempersiapkan sinyal sensor mentah (raw signal) agar siap digunakan dalam logika kontrol. Proses ini mencakup:
- **Filtering**: mengurangi noise tanpa menambah delay yang terlalu besar
- **Scaling**: konversi nilai ADC ke unit fisik (mm, rad, m/s²)
- **Linearization**: koreksi karakteristik sensor yang non-linear
- **Calibration**: offset dan gain adjustment

### Sensor Fusion
**Sensor fusion** adalah teknik menggabungkan data dari multiple sensor untuk mendapatkan estimasi yang lebih robust dan akurat dibandingkan menggunakan satu sensor saja.

**Alasan fusion diperlukan:**
- Setiap sensor punya kelebihan dan kekurangan
- Combine kelebihan masing-masing sensor
- Redundancy untuk fault tolerance

### Trade-off: Noise vs Latency
**Trade-off fundamental** dalam sensor conditioning:
- **Filtering yang baik** → less noise, but higher latency (delay)
- **Control responsiveness** → low latency, but more noise

Tuning parameter filter harus disesuaikan dengan requirement aplikasi:
- Safety-critical system → prioritize accuracy (more filtering)
- High-speed control → prioritize responsiveness (less filtering)

---

## 2. KONSEP INTI

### 2.1 Noise Source dalam Sensor System

#### A. Thermal Noise (Johnson Noise)
- **Penyebab**: gerakan random elektron akibat temperature
- **Karakteristik**: white noise (flat frequency spectrum)
- **Magnitude**: $V_n = \sqrt{4kTBR}$ dimana:
  - $k$ = Boltzmann constant (1.38 × 10⁻²³ J/K)
  - $T$ = temperature (Kelvin)
  - $B$ = bandwidth (Hz)
  - $R$ = resistance (Ω)

#### B. Electromagnetic Interference (EMI)
- **Penyebab**: motor PWM, switching power supply, wireless signal
- **Karakteristik**: periodic spike, fundamental frequency + harmonics
- **Solusi**: shielding, twisted-pair cable, ferrite bead, EMC design

#### C. Quantization Noise (ADC)
- **Penyebab**: discrete step pada analog-to-digital conversion
- **Magnitude**: $\pm \frac{LSB}{2}$ dimana LSB = $\frac{V_{ref}}{2^n}$
- **Example**: 12-bit ADC, Vref=3.3V → LSB = 0.8mV

#### D. Mechanical Vibration
- **Penyebab**: motor unbalance, gear backlash, base vibration
- **Karakteristik**: low frequency (< 100Hz), high amplitude
- **Solusi**: mechanical damping, vibration isolator, software filtering

### 2.2 Noise Characteristic

#### White Noise
- **Definisi**: noise dengan flat power spectrum (semua frequency sama)
- **Contoh**: thermal noise, ADC quantization
- **Filtering**: low-pass filter effective

#### 1/f Noise (Pink Noise)
- **Definisi**: noise dengan power $\propto \frac{1}{f}$ (dominan di low frequency)
- **Contoh**: semiconductor junction noise
- **Filtering**: high-pass filter atau complementary filter

#### Impulse Noise (Spike)
- **Definisi**: sudden burst, short duration
- **Contoh**: motor start, relay switching, electrostatic discharge (ESD)
- **Filtering**: median filter, outlier rejection

### 2.3 Sampling Theorem (Nyquist-Shannon)

**Teorema**: Untuk merekonstruksi sinyal tanpa aliasing, sampling rate minimum adalah:

$$f_s \geq 2 \times f_{max}$$

dimana:
- $f_s$ = sampling frequency
- $f_{max}$ = highest frequency component dalam sinyal

**Praktis**: gunakan $f_s \geq 5 \times f_{max}$ untuk anti-aliasing yang baik.

**Contoh**:
- Encoder dengan max speed 1000 RPM, 1000 PPR
- Tick rate max = 1000 RPM × 1000 PPR / 60 = 16,667 tick/s
- Sampling minimum = 2 × 16,667 = 33,334 Hz (33.3 kHz)
- Praktis gunakan: 50 kHz atau lebih

### 2.4 Filter Type

#### A. Finite Impulse Response (FIR) Filter
- **Karakteristik**: no feedback, output = weighted sum of input history
- **Kelebihan**: always stable, linear phase response
- **Kekurangan**: higher order untuk cutoff frequency yang sama (vs IIR)
- **Contoh**: moving average filter

**Transfer function**:
$$y[n] = \sum_{i=0}^{N-1} h[i] \cdot x[n-i]$$

#### B. Infinite Impulse Response (IIR) Filter
- **Karakteristik**: ada feedback dari output sebelumnya
- **Kelebihan**: lower order (efficient), sharp cutoff
- **Kekurangan**: bisa unstable jika parameter salah
- **Contoh**: low-pass digital filter, Butterworth filter

**Transfer function (first-order)**:
$$y[n] = \alpha \cdot x[n] + (1-\alpha) \cdot y[n-1]$$

### 2.5 Moving Average Filter

**Prinsip**: rata-rata dari $N$ sample terakhir.

**Formula**:
$$y[n] = \frac{1}{N} \sum_{i=0}^{N-1} x[n-i]$$

**Karakteristik**:
- **Noise reduction**: $\frac{1}{\sqrt{N}}$ dari noise amplitude
- **Latency**: $\frac{N-1}{2}$ sample delay
- **Cutoff frequency**: $f_c \approx \frac{f_s}{N}$ (approximate)

**Trade-off**:
- Large $N$ → better smoothing, higher delay
- Small $N$ → less smoothing, lower delay

**Implementasi**:
- Circular buffer untuk efficient memory usage
- Running sum untuk avoid recompute setiap cycle

### 2.6 Low-Pass Digital Filter

**First-order IIR** (Exponential Moving Average):

$$y[n] = \alpha \cdot x[n] + (1-\alpha) \cdot y[n-1]$$

**Parameter $\alpha$ calculation** dari cutoff frequency:

$$\alpha = \frac{2\pi f_c \Delta t}{2\pi f_c \Delta t + 1}$$

dimana:
- $f_c$ = cutoff frequency (Hz)
- $\Delta t$ = sampling period (s)

**Karakteristik**:
- **Phase lag**: $\phi \approx -\arctan(2\pi f \cdot \tau)$ dimana $\tau = \frac{1-\alpha}{\alpha \cdot f_s}$
- **-3dB frequency**: $f_{3dB} = f_c$
- **Step response time constant**: $\tau$

**Equivalent analog RC filter**:
- $\alpha$ kecil → high resistance (slow filter)
- $\alpha$ besar → low resistance (fast filter)

### 2.7 Complementary Filter

**Prinsip**: combine 2 sensor dengan karakteristik berbeda:
- **Sensor 1** (e.g., encoder): low drift, high noise, high frequency accuracy
- **Sensor 2** (e.g., accelerometer): low noise, drift over time, low frequency accuracy

**Formula**:
$$x_{fused} = w_1 \cdot x_1 + w_2 \cdot x_2$$

dimana $w_1 + w_2 = 1$ (normalized weight).

**Filter implementation**:
- Sensor 1 → low-pass filter (ambil low frequency component)
- Sensor 2 → high-pass filter (ambil high frequency component)

**Block diagram**:
```
[Encoder] ─→ Low-Pass Filter ─→ [× w1] ─┐
                                          ├→ [+] → fused_position
[Accel] ─→ Integrate ─→ High-Pass Filter ─→ [× w2] ─┘
```

**Weight tuning**:
- $w_1 = 0.7, w_2 = 0.3$ (typical starting point)
- Adjust based on sensor noise level & drift rate

**Aplikasi**:
- Quadrotor attitude: gyroscope (drift) + accelerometer (no drift)
- Robot arm position: encoder (noise) + IMU (smooth tapi drift)

### 2.8 Kalman Filter (Basic)

**Definisi**: optimal filter untuk linear system dengan Gaussian noise.

**Prinsip**: combine **prediction** (model-based) dengan **measurement** (sensor-based).

**State space model**:
$$\mathbf{x}[k+1] = \mathbf{A} \cdot \mathbf{x}[k] + \mathbf{w}[k]$$
$$\mathbf{z}[k] = \mathbf{H} \cdot \mathbf{x}[k] + \mathbf{v}[k]$$

dimana:
- $\mathbf{x}$ = state vector (e.g., [position, velocity])
- $\mathbf{A}$ = state transition matrix
- $\mathbf{H}$ = measurement matrix
- $\mathbf{w}$ = process noise (covariance $\mathbf{Q}$)
- $\mathbf{v}$ = measurement noise (covariance $\mathbf{R}$)

**Algoritma (2 langkah)**:

#### 1. Predict Step (Time Update)
$$\hat{\mathbf{x}}_{k|k-1} = \mathbf{A} \cdot \hat{\mathbf{x}}_{k-1|k-1}$$
$$\mathbf{P}_{k|k-1} = \mathbf{A} \cdot \mathbf{P}_{k-1|k-1} \cdot \mathbf{A}^T + \mathbf{Q}$$

#### 2. Update Step (Measurement Update)
$$\mathbf{K}_k = \frac{\mathbf{P}_{k|k-1} \cdot \mathbf{H}^T}{\mathbf{H} \cdot \mathbf{P}_{k|k-1} \cdot \mathbf{H}^T + \mathbf{R}}$$ (Kalman gain)

$$\hat{\mathbf{x}}_{k|k} = \hat{\mathbf{x}}_{k|k-1} + \mathbf{K}_k \cdot (\mathbf{z}_k - \mathbf{H} \cdot \hat{\mathbf{x}}_{k|k-1})$$
$$\mathbf{P}_{k|k} = (\mathbf{I} - \mathbf{K}_k \cdot \mathbf{H}) \cdot \mathbf{P}_{k|k-1}$$

**Tuning parameter**:
- **$\mathbf{Q}$** (process noise): large → trust model less, trust measurement more
- **$\mathbf{R}$** (measurement noise): large → trust measurement less, trust model more

**Example 1D motion**:
- State: $\mathbf{x} = \begin{bmatrix} position \\ velocity \end{bmatrix}$
- Model: $\mathbf{A} = \begin{bmatrix} 1 & \Delta t \\ 0 & 1 \end{bmatrix}$
- Measurement: $\mathbf{H} = \begin{bmatrix} 1 & 0 \end{bmatrix}$ (only measure position)

### 2.9 Sensor Redundancy & Voting

**Prinsip**: gunakan multiple sensor untuk fault tolerance.

**2-out-of-3 Voting Logic**:
```
Input: sensor1, sensor2, sensor3
Process:
  if |sensor1 - sensor2| < tolerance → use avg(sensor1, sensor2)
  else if |sensor1 - sensor3| < tolerance → use avg(sensor1, sensor3)
  else if |sensor2 - sensor3| < tolerance → use avg(sensor2, sensor3)
  else → flag error (no consensus)
```

**Keuntungan**:
- Fault detection: identify sensor yang error
- Robustness: system tetap jalan dengan 1 sensor fault
- Safety: critical untuk aerospace, medical, automotive

**Limitation**:
- Require 3+ sensor (cost, space, complexity)
- Voting logic bisa false positive jika 2 sensor sama-sama error

---

## 3. DIAGRAM & ILUSTRASI

### Diagram 1: Sensor Signal Before/After Filtering

**Time-domain waveform**:
```
Raw Signal (noisy):
   5.0 ─┐  ╱─╲    ╱─╲   ╱──╲
        │ ╱   ╲  ╱   ╲ ╱    ╲
   0.0 ─┼─────╲╱─────╲╱──────╲───→ time
        
Filtered Signal (smooth):
   5.0 ─┐  ╱────────╲
        │ ╱          ╲
   0.0 ─┼─────────────╲────────→ time
```

### Diagram 2: Frequency Spectrum (FFT) Before/After

```
Before Filtering (FFT):
Amplitude
   │     
   │  ██          High frequency noise
   │  ██ ██
   │  ████
   │████████
   └──────────→ Frequency (Hz)
   0   50  100

After Low-Pass Filter:
Amplitude
   │     
   │  ██          Noise removed
   │  ██
   │  ██
   │████
   └──────────→ Frequency (Hz)
   0   50  100
      ↑
    cutoff (50Hz)
```

### Diagram 3: Filter Frequency Response (Bode Plot)

**Low-pass filter**: gain vs frequency

```
Gain (dB)
  0  ─┐────────────╲
      │             ╲        -20 dB/decade
 -20  │              ╲
      │               ╲
 -40  │                ╲─────
      └────────────────────→ Frequency
           fc (cutoff)
```

**Phase response**:
```
Phase (°)
  0   ─┐
      │  ╲
 -45  │───●─────  (at fc)
      │      ╲
 -90  │       ╲────────
      └────────────────→ Frequency
```

### Diagram 4: Complementary Filter Block Diagram

```
┌─────────┐     ┌──────────┐     ┌─────┐
│ Encoder │────→│Low-Pass  │────→│× w1 │──┐
│         │     │ Filter   │     └─────┘  │
└─────────┘     └──────────┘              │
                                          ├→ [+] → fused_position
┌─────────┐     ┌──────────┐     ┌─────┐  │
│  Accel  │────→│Integrate │────→│× w2 │──┘
│         │     │& Hi-Pass │     └─────┘
└─────────┘     └──────────┘
```

### Diagram 5: Kalman Filter State Transition

```
PREDICT:                    UPDATE:
┌─────────────┐            ┌──────────────┐
│ Previous    │            │ Measurement  │
│ State x[k-1]│            │ z[k]         │
└──────┬──────┘            └──────┬───────┘
       │                          │
       ↓                          ↓
  ┌────────┐                ┌─────────┐
  │A·x[k-1]│                │Compare  │
  └────┬───┘                │with H·x │
       │                    └────┬────┘
       ↓                         ↓
  ┌─────────┐              ┌──────────┐
  │x_pred   │─────────────→│ Kalman   │
  │P_pred   │              │ Gain (K) │
  └─────────┘              └────┬─────┘
                                ↓
                          ┌──────────┐
                          │x_updated │
                          │P_updated │
                          └──────────┘
```

### Diagram 6: Sensor Fusion Architecture (3-Sensor Voting)

```
┌──────────┐
│ Sensor 1 │────┐
└──────────┘    │
                │
┌──────────┐    │     ┌──────────────┐      ┌────────────┐
│ Sensor 2 │────┼────→│ 2-out-of-3   │─────→│ Best       │
└──────────┘    │     │ Voting Logic │      │ Estimate   │
                │     └──────────────┘      └────────────┘
┌──────────┐    │              │
│ Sensor 3 │────┘              ↓
└──────────┘            ┌─────────────┐
                        │Fault Detect │
                        └─────────────┘
```

### Diagram 7: Phase Lag Effect pada Control

**Scenario**: PID controller dengan filtered sensor feedback

```
Setpoint (step input):
  │    ┌──────────
  │    │
  └────┘────────→ time

Filtered feedback (dengan phase lag):
  │      ╱───────
  │     ╱
  └────╱────────→ time
     ↑
   delay

Effect: control loop compensate lambat → oscillation atau instability
```

**Kesimpulan**: filter delay harus < 10% dari system time constant untuk stable control.

---

## 4. CONTOH KASUS INDUSTRI

### Kasus 1: Robot Arm Position Control

**Problem**: Encoder noisy karena electrical interference dari motor PWM.

**Solution**: Complementary filter dengan accelerometer.

**Implementation**:
- Encoder → low-pass filter (cutoff 10Hz) → smooth position estimate
- Accelerometer → integrate → position → high-pass filter (cutoff 1Hz) → high-frequency correction
- Fused position = 0.7 × encoder_pos + 0.3 × accel_pos

**Result**:
- Noise reduction: 75% (RMS)
- Position accuracy: ±0.5mm (vs ±2mm raw encoder)
- Control stability improved: no oscillation

### Kasus 2: Automated Guided Vehicle (AGV) Localization

**Problem**: Wheel encoder drift akibat slip, LiDAR accurate tapi slow update (10Hz).

**Solution**: Sensor fusion dengan Kalman filter.

**Implementation**:
- State: [x, y, θ, vx, vy, ω] (position, orientation, velocity)
- Prediction: wheel odometry (high rate, 100Hz)
- Measurement: LiDAR SLAM (low rate, 10Hz, high accuracy)
- Kalman filter combine keduanya

**Result**:
- Position accuracy: ±5cm (vs ±20cm encoder-only)
- Update rate: 100Hz (vs 10Hz LiDAR-only)
- Drift correction: automatic setiap LiDAR update

### Kasus 3: Drone Attitude Estimation

**Problem**: Gyroscope drift, accelerometer noisy saat vibration.

**Solution**: Complementary filter atau Madgwick filter.

**Implementation**:
- Gyroscope → integrate → angle (accurate short-term, drift long-term)
- Accelerometer → angle estimate dari gravity vector (noisy, no drift)
- Complementary: angle = 0.98 × gyro_angle + 0.02 × accel_angle

**Result**:
- Attitude accuracy: ±0.5° (vs ±5° gyro-only, ±10° accel-only)
- Drift: < 1°/min (vs 10°/min gyro-only)
- Update rate: 1kHz (real-time control)

### Kasus 4: CNC Machine Position Sensing

**Problem**: Safety-critical positioning, sensor failure bisa cause crash.

**Solution**: Triple redundancy dengan voting logic.

**Implementation**:
- 3× linear encoder (different brand untuk independent failure mode)
- 2-out-of-3 voting logic (tolerance ±10μm)
- Fault detection → alarm + stop

**Result**:
- Fault detection rate: 99.9%
- False positive: < 0.01%
- Downtime prevention: sensor swap tanpa stop production

---

## 5. RINGKASAN

### Key Takeaways

1. **Sensor bisa error**: noise, drift, fault → filtering & fusion essential untuk robust system.

2. **Trade-off fundamental**: filter delay vs noise reduction. Tuning parameter harus disesuaikan dengan requirement:
   - High-speed control → less filtering (low latency)
   - High-precision → more filtering (low noise)

3. **Filter selection**:
   - Simple noise → moving average atau low-pass filter
   - Drift problem → complementary filter
   - Optimal estimation → Kalman filter (tapi kompleks)

4. **Start simple**: moving average dulu, jika insufficient baru advance ke Kalman.

5. **Kalman filter powerful tapi kompleks**:
   - Require system model (A matrix)
   - Tuning Q & R parameter challenging
   - Computational cost higher
   - Benefit: optimal estimate, handle multiple sensor fusion

6. **Complementary filter** simple but effective:
   - Easy tuning (just weight)
   - Fast computation
   - Good untuk 2-sensor fusion

7. **Redundancy** important untuk safety-critical:
   - Cost trade-off: 3× sensor vs reliability
   - Voting logic detect fault automatically

### Design Flow

**Sensor Conditioning Design Flow**:
```
1. Analyze noise source (FFT, oscilloscope) → identify frequency
2. Select filter type:
   - White noise → low-pass or moving average
   - Drift → complementary or Kalman
   - Fault tolerance → redundancy + voting
3. Tune filter parameter:
   - Moving average: window size
   - Low-pass: cutoff frequency (α)
   - Complementary: weight (w1, w2)
   - Kalman: Q & R matrix
4. Validate performance:
   - Noise reduction (RMS before/after)
   - Latency (phase lag)
   - Control stability (integrate dengan PID loop)
5. Iterate until meet requirement
```

### Recommended Reading

**Papers**:
- Welch & Bishop (2006): "An Introduction to the Kalman Filter" (UNC Chapel Hill TR)
- Mahony, Hamel, Pflimlin (2008): "Nonlinear Complementary Filters on SO(3)"

**Books**:
- Simon (2006): "Optimal State Estimation: Kalman, H∞, and Nonlinear Approaches"
- Bar-Shalom et al. (2001): "Estimation with Applications to Tracking and Navigation"

---

## 6. DESKRIPSI TUGAS VIDEO (MODUL 4)

### Point yang HARUS Dijelaskan dalam Video Pembelajaran

#### 1. Sensor Noise: Dari Mana? (5 menit)
- **EMI**: motor PWM switching → spike pada sensor signal
- **Thermal noise**: resistor dan transistor → white noise
- **ADC quantization**: discrete step → ±0.5 LSB error
- **Demo**: oscilloscope showing noise pada encoder signal

#### 2. Filtering: Moving Average vs Low-Pass Filter (7 menit)
- **Moving average**: simple, intuitive, fixed delay
- **Low-pass digital**: exponential smoothing, adjustable cutoff
- **Trade-off**: window size/cutoff vs latency
- **Demo**: raw sensor plot vs filtered (overlay comparison)

#### 3. Demo: Sensor Reading Raw vs Filtered (8 menit)
- **Setup**: encoder pada motor shaft, oscilloscope probe
- **Test 1**: motor OFF → noise level (baseline)
- **Test 2**: motor ON (PWM 50%) → noise increase (EMI)
- **Test 3**: apply moving average filter (window=5) → noise reduce
- **Test 4**: apply low-pass filter (fc=5Hz) → compare dengan moving average
- **Plot**: matplotlib real-time plot showing raw vs filtered

#### 4. Complementary Filter: Why Combine Encoder + Accel? (10 menit)
- **Encoder karakteristik**: precise, tapi noise dari EMI
- **Accelerometer karakteristik**: smooth, tapi drift saat integrate
- **Fusion**: encoder (low-pass) + accel (high-pass) → best of both
- **Weight tuning**: w_enc=0.7, w_accel=0.3 (starting point)
- **Demo**: compare encoder-only vs accel-only vs fused position
- **Plot**: 3 line overlay (encoder, accel, fused) vs time

#### 5. Kalman Filter: Basic Concept (10 menit)
- **Cycle**: Predict (model-based) → Observe (sensor) → Update (fusion)
- **Kalman gain**: automatic weight antara prediction vs measurement
- **Q tuning**: trust model (small Q) vs trust sensor (large Q)
- **R tuning**: sensor noise level estimate
- **Demo**: 1D motion tracking (position + velocity state)
- **Plot**: Kalman estimate vs raw measurement, Kalman gain evolution

#### 6. Sensor Fusion: Multiple Sensor Voting (5 menit)
- **3-sensor setup**: encoder, IMU, external reference
- **Voting logic**: 2-out-of-3 (2 agree → use, 1 outlier → discard)
- **Fault injection**: disconnect 1 sensor → voting detect & continue
- **Demo**: 3 sensor reading, inject fault, voting result, alarm
- **Plot**: sensor value vs time (showing fault detection moment)

#### 7. Trade-off: Filter Delay vs Responsiveness (8 menit)
- **Scenario**: PID speed control dengan filtered encoder
- **Test 1**: no filter → noisy feedback → PID oscillate
- **Test 2**: heavy filter (window=20) → smooth feedback, tapi slow response
- **Test 3**: moderate filter (window=5) → balance
- **Demo**: ubah filter window size, observe PID response (overshoot, settling time)
- **Kp gain test**: dengan heavy filter, Kp harus lebih kecil (avoid instability)

#### 8. Troubleshooting (5 menit)
- **Filter parameter tuning**: start small window, increase gradually
- **Sensor not responding**: check wiring, power, I2C/SPI communication
- **Filter tidak effective**: cek sampling rate (terlalu lambat?), noise frequency (out of filter range?)
- **Kalman diverge**: Q/R parameter salah, check measurement noise estimate
- **Voting false positive**: tolerance terlalu ketat, relax threshold

**Total durasi video**: ~60 menit

### Format Video
- **Intro**: 2 menit (overview Modul 4)
- **Teori**: 20 menit (konsep filtering, fusion, trade-off)
- **Demo hardware**: 25 menit (oscilloscope, real-time plot, sensor test)
- **Troubleshooting**: 8 menit (common issue & solution)
- **Outro**: 5 menit (summary, next module preview)

---

## REFERENSI

1. David G. Alciatore, Michael B. Histand — *Introduction to Mechatronics and Measurement Systems* (5th ed.), McGraw-Hill
2. Steven W. Smith — *Digital Signal Processing: A Practical Guide for Engineers and Scientists*, Newnes/Elsevier
3. Welch & Bishop (2006) — "An Introduction to the Kalman Filter", UNC Chapel Hill Technical Report
4. Espressif ESP32 ADC documentation: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html

---

**Prepared by**: [Instructor Name]  
**Last updated**: February 2026  
**Module**: Batch 2 - Modul 4  
