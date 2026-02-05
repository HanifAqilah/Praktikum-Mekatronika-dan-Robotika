# JOBSHEET MODUL 4: SENSOR FUSION & CONDITIONING (ADVANCED)

---

## 1. TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa diharapkan mampu:

1. **Menganalisis** karakteristik noise sensor (time-domain & frequency-domain)
2. **Mengimplementasikan** berbagai jenis filter (moving average, low-pass digital)
3. **Menerapkan** complementary filter untuk sensor fusion
4. **Menggunakan** Kalman filter basic untuk estimasi state 1D motion
5. **Merancang** sensor redundancy voting logic untuk fault tolerance
6. **Mengukur** performance filter vs teori (noise reduction, latency, accuracy)
7. **Menganalisis** trade-off antara filtering effectiveness dengan control responsiveness

---

## 2. ALAT & BAHAN

### Hardware
- [ ] Robot platform dengan encoder + accelerometer (IMU 3-axis)
- [ ] Oscilloscope (untuk measure analog signal)
- [ ] ESP32 atau Arduino Mega dengan ADC (10-bit atau 12-bit)
- [ ] Motor DC dengan encoder (minimum 500 PPR)
- [ ] Kabel probe oscilloscope
- [ ] Power supply (sesuai motor voltage)

### Software
- [ ] PlatformIO IDE (VS Code extension)
- [ ] Python 3.8+ dengan libraries:
  - `numpy` (numerical computation)
  - `scipy` (signal processing, FFT)
  - `matplotlib` (plotting)
  - `pandas` (data handling)
- [ ] Serial monitor atau plotter
- [ ] Git untuk version control

### Optional
- [ ] External vibration source (untuk simulate vibration noise)
- [ ] Signal generator (untuk inject known noise frequency)

---

## 3. LANGKAH KERJA

---

### PERCOBAAN 1: SENSOR NOISE ANALYSIS

**Tujuan**: Memahami karakteristik noise sensor, mengidentifikasi sumber & frequency noise.

#### Langkah Percobaan

**a. Setup Hardware**
1. Pasang encoder pada motor shaft
2. Hubungkan encoder output ke oscilloscope (channel 1) dan ESP32 input pin
3. Pasang accelerometer via I2C/SPI ke ESP32
4. Pastikan motor power supply terpisah dari sensor power (noise isolation)

**b. Collect Raw Sensor Data**
1. Upload firmware untuk logging sensor data (encoder tick count, accelerometer x/y/z)
2. Sampling rate: 100Hz (10ms per sample)
3. Duration: 10 detik (1000 samples)
4. Motor condition:
   - **Test 1**: Motor OFF (baseline noise)
   - **Test 2**: Motor ON, PWM 50% (EMI noise from motor)
   - **Test 3**: Motor ON, apply external vibration (mechanical noise)

**c. Save Data ke CSV**
```cpp
// Firmware snippet untuk logging
void loop() {
  unsigned long timestamp = millis();
  int encoder_ticks = readEncoder();
  float accel_x = readAccel_X();
  
  Serial.print(timestamp);
  Serial.print(",");
  Serial.print(encoder_ticks);
  Serial.print(",");
  Serial.println(accel_x);
  
  delay(10); // 100Hz sampling
}
```

**d. Plot Time-Domain Waveform**
- Import CSV ke Excel atau Python
- Plot: encoder value vs time, accel vs time
- Identify: noise amplitude (peak-to-peak)

**Python code untuk plot**:
```python
import pandas as pd
import matplotlib.pyplot as plt

# Load data
data = pd.read_csv('sensor_data.csv', names=['time', 'encoder', 'accel_x'])

# Plot time-domain
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(data['time'], data['encoder'], label='Encoder Ticks')
plt.xlabel('Time (ms)')
plt.ylabel('Encoder Ticks')
plt.title('Raw Encoder Signal (Time Domain)')
plt.grid()
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(data['time'], data['accel_x'], label='Accel X', color='orange')
plt.xlabel('Time (ms)')
plt.ylabel('Acceleration (m/s²)')
plt.title('Raw Accelerometer Signal (Time Domain)')
plt.grid()
plt.legend()

plt.tight_layout()
plt.savefig('output/sensor_raw_time_domain.png')
plt.show()
```

**e. Calculate FFT (Frequency Spectrum)**
```python
import numpy as np
from scipy.fft import fft, fftfreq

# FFT calculation
N = len(data)
T = 0.01  # sampling period (100Hz → 10ms)
encoder_fft = fft(data['encoder'].values)
freq = fftfreq(N, T)[:N//2]
amplitude = 2.0/N * np.abs(encoder_fft[:N//2])

# Plot frequency domain
plt.figure(figsize=(12, 6))
plt.plot(freq, amplitude)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title('Encoder Signal - Frequency Spectrum (FFT)')
plt.grid()
plt.xlim(0, 50)  # Focus on 0-50Hz
plt.savefig('output/encoder_fft_spectrum.png')
plt.show()
```

**f. Identify Noise Frequency Peak**
- Observe FFT plot, identify peak frequency
- Example: jika peak di 20kHz → likely PWM frequency atau harmonic
- Example: jika peak di 50/60Hz → power line interference

**g. Estimate Noise Amplitude**
- **Peak-to-peak**: max value - min value
- **RMS**: $\sigma = \sqrt{\frac{1}{N}\sum(x_i - \bar{x})^2}$

```python
# RMS calculation
encoder_mean = data['encoder'].mean()
encoder_std = data['encoder'].std()
print(f"Encoder - Mean: {encoder_mean}, RMS Noise: {encoder_std}")
```

#### Data Pengamatan

| Sensor | Motor State | Noise Type | Peak-to-Peak (unit) | Frequency (Hz) | RMS |
|--------|------------|-----------|---------------------|-----------------|-----|
| Encoder | OFF | Baseline | ___ | ___ | ___ |
| Encoder | ON (PWM 50%) | EMI? | ___ | ___ | ___ |
| Encoder | ON + Vibration | Mechanical | ___ | ___ | ___ |
| Accel X | OFF | Baseline | ___ | ___ | ___ |
| Accel X | ON (PWM 50%) | EMI? | ___ | ___ | ___ |
| Accel X | ON + Vibration | Mechanical | ___ | ___ | ___ |

#### Deliverable
- [ ] CSV file: `sensor_data.csv` (raw sensor log)
- [ ] Plot image: `sensor_raw_time_domain.png`
- [ ] Plot image: `encoder_fft_spectrum.png`, `accel_fft_spectrum.png`
- [ ] Analysis summary: noise source identification, frequency analysis

#### Pertanyaan Analisis
1. Apakah noise tipe white noise (flat spectrum) atau ada peak frequency tertentu?
2. Noise amplitude meningkat berapa % saat motor ON vs OFF?
3. Frekuensi noise dominan berasal dari apa? (PWM, power line, vibration?)
4. Apakah sampling rate 100Hz cukup? (Check Nyquist theorem)

---

### PERCOBAAN 2: MOVING AVERAGE FILTER

**Tujuan**: Implementasi simple filter, memahami trade-off delay vs smoothing.

#### Langkah Percobaan

**a. Implementasi Moving Average Filter**

Code firmware (`filter.h`):
```cpp
// filter.h
#ifndef FILTER_H
#define FILTER_H

#define MAX_WINDOW_SIZE 20

class MovingAverageFilter {
private:
  float buffer[MAX_WINDOW_SIZE];
  int window_size;
  int buffer_idx;
  float sum;
  
public:
  MovingAverageFilter(int window) {
    window_size = window;
    buffer_idx = 0;
    sum = 0;
    for (int i = 0; i < MAX_WINDOW_SIZE; i++) {
      buffer[i] = 0;
    }
  }
  
  float update(float new_value) {
    // Remove oldest value from sum
    sum -= buffer[buffer_idx];
    // Add new value
    buffer[buffer_idx] = new_value;
    sum += new_value;
    // Move index (circular buffer)
    buffer_idx = (buffer_idx + 1) % window_size;
    // Return average
    return sum / window_size;
  }
  
  void reset() {
    buffer_idx = 0;
    sum = 0;
    for (int i = 0; i < MAX_WINDOW_SIZE; i++) {
      buffer[i] = 0;
    }
  }
};

#endif
```

**b. Test dengan Multiple Window Size**

Test scenario:
1. Window size = 3 (minimal filtering)
2. Window size = 5 (light filtering)
3. Window size = 10 (moderate filtering)
4. Window size = 20 (heavy filtering)

**c. Apply Filter ke Data dari Percobaan 1**

Python code untuk offline analysis:
```python
import pandas as pd
import matplotlib.pyplot as plt

def moving_average(data, window_size):
    """Moving average filter implementation"""
    return data.rolling(window=window_size, center=False).mean()

# Load raw data
data = pd.read_csv('sensor_data.csv', names=['time', 'encoder', 'accel_x'])

# Apply multiple window sizes
data['ma_3'] = moving_average(data['encoder'], 3)
data['ma_5'] = moving_average(data['encoder'], 5)
data['ma_10'] = moving_average(data['encoder'], 10)
data['ma_20'] = moving_average(data['encoder'], 20)

# Plot comparison
plt.figure(figsize=(14, 8))
plt.plot(data['time'], data['encoder'], label='Raw', alpha=0.5, linewidth=0.5)
plt.plot(data['time'], data['ma_3'], label='Window=3', linewidth=1.5)
plt.plot(data['time'], data['ma_5'], label='Window=5', linewidth=1.5)
plt.plot(data['time'], data['ma_10'], label='Window=10', linewidth=1.5)
plt.plot(data['time'], data['ma_20'], label='Window=20', linewidth=1.5)
plt.xlabel('Time (ms)')
plt.ylabel('Encoder Ticks')
plt.title('Moving Average Filter Comparison')
plt.legend()
plt.grid()
plt.savefig('output/moving_average_comparison.png')
plt.show()
```

**d. Measure Noise Reduction**

Calculate RMS before & after:
```python
# RMS calculation
rms_raw = data['encoder'].std()
rms_ma3 = data['ma_3'].std()
rms_ma5 = data['ma_5'].std()
rms_ma10 = data['ma_10'].std()
rms_ma20 = data['ma_20'].std()

# Noise reduction percentage
noise_reduction_3 = (1 - rms_ma3/rms_raw) * 100
noise_reduction_5 = (1 - rms_ma5/rms_raw) * 100
noise_reduction_10 = (1 - rms_ma10/rms_raw) * 100
noise_reduction_20 = (1 - rms_ma20/rms_raw) * 100

print(f"Noise Reduction:")
print(f"  Window=3:  {noise_reduction_3:.1f}%")
print(f"  Window=5:  {noise_reduction_5:.1f}%")
print(f"  Window=10: {noise_reduction_10:.1f}%")
print(f"  Window=20: {noise_reduction_20:.1f}%")
```

**e. Measure Latency**

Latency (group delay) = $\frac{N-1}{2}$ samples

For 100Hz sampling (10ms per sample):
- Window=3 → delay = 1 sample = 10ms
- Window=5 → delay = 2 samples = 20ms
- Window=10 → delay = 4.5 samples = 45ms
- Window=20 → delay = 9.5 samples = 95ms

#### Data Pengamatan

| Window Size | Noise Reduction (%) | Latency (ms) | RMS Error | Recommendation |
|-------------|---------------------|--------------|-----------|----------------|
| 3 | ___ | 10 | ___ | Fast response, light smoothing |
| 5 | ___ | 20 | ___ | **Balanced (recommended)** |
| 10 | ___ | 45 | ___ | Heavy smoothing, slow |
| 20 | ___ | 95 | ___ | Very smooth, very slow |

#### Deliverable
- [ ] Code: `filter.h` (moving average class)
- [ ] Plot: `moving_average_comparison.png`
- [ ] Analysis document: trade-off analysis, recommendation

#### Pertanyaan Analisis
1. Berapa noise reduction untuk window=5 vs window=20? (theoretical vs measurement)
2. Latency 45ms (window=10) apakah acceptable untuk control loop 10Hz?
3. Window size optimal untuk aplikasi ini? (consider speed vs accuracy)

---

### PERCOBAAN 3: LOW-PASS DIGITAL FILTER

**Tujuan**: Implementasi exponential moving average (equivalent RC filter digital).

#### Langkah Percobaan

**a. Design Digital Low-Pass Filter**

**Formula**:
$$y[n] = \alpha \cdot x[n] + (1-\alpha) \cdot y[n-1]$$

**Calculate $\alpha$ from cutoff frequency**:
$$\alpha = \frac{2\pi f_c \Delta t}{2\pi f_c \Delta t + 1}$$

dimana:
- $f_c$ = cutoff frequency (Hz)
- $\Delta t$ = sampling period (s)

**Example calculation**:
- Cutoff = 5Hz, sampling rate = 100Hz → $\Delta t = 0.01s$
- $\alpha = \frac{2\pi \times 5 \times 0.01}{2\pi \times 5 \times 0.01 + 1} = \frac{0.314}{1.314} = 0.239$

**b. Implementasi di Firmware**

Code (`filter.h`, add to existing file):
```cpp
class LowPassFilter {
private:
  float alpha;
  float filtered_value;
  bool initialized;
  
public:
  LowPassFilter(float cutoff_freq, float sample_rate) {
    float dt = 1.0 / sample_rate;
    alpha = (2 * PI * cutoff_freq * dt) / (2 * PI * cutoff_freq * dt + 1);
    filtered_value = 0;
    initialized = false;
  }
  
  float update(float raw_value) {
    if (!initialized) {
      filtered_value = raw_value;  // Initialize dengan first value
      initialized = true;
    }
    filtered_value = alpha * raw_value + (1 - alpha) * filtered_value;
    return filtered_value;
  }
  
  void reset() {
    filtered_value = 0;
    initialized = false;
  }
  
  float getAlpha() {
    return alpha;
  }
};
```

**c. Test dengan 3 Cutoff Frequency**

Test parameter:
| Test | Cutoff (Hz) | α Value | Expected Behavior |
|------|-------------|---------|-------------------|
| 1 | 1 | 0.059 | Very smooth, slow response |
| 2 | 5 | 0.239 | Moderate smoothing |
| 3 | 10 | 0.386 | Light smoothing, fast response |

**d. Compare Delay vs Moving Average**

Approximate equivalence:
- Low-pass filter dengan $f_c = 5Hz$ ≈ moving average window = 5
- Latency comparison via step response test

**Step response test**:
1. Apply step input: encoder suddenly move 100 ticks
2. Measure time untuk filter output reach 90% (rise time)
3. Compare moving average window=5 vs low-pass fc=5Hz

#### Data Pengamatan

| Cutoff (Hz) | α Value | Delay vs MA(window=5) (ms) | Smoothness (subjective) | RMS Noise |
|-------------|---------|---------------------------|-------------------------|-----------|
| 1 | 0.059 | ___ | Very smooth | ___ |
| 5 | 0.239 | ~0 (baseline) | Moderate | ___ |
| 10 | 0.386 | ___ | Light | ___ |

#### Deliverable
- [ ] Code: `LowPassFilter` class di `filter.h`
- [ ] Plot: comparison low-pass vs moving average
- [ ] Analysis: filter parameter explanation (α calculation)
- [ ] Step response plot: rise time comparison

#### Pertanyaan Analisis
1. Low-pass filter fc=5Hz vs moving average window=5: mana yang lebih smooth?
2. Delay mana yang lebih kecil? (measure dari step response)
3. Computational cost: moving average vs low-pass? (CPU cycle atau memory)
4. Kapan gunakan moving average vs low-pass filter?

---

### PERCOBAAN 4: COMPLEMENTARY FILTER

**Tujuan**: Fusion encoder (precise, noisy) + accelerometer (smooth, drift).

#### Langkah Percobaan

**a. Collect Data: Encoder Position + Accelerometer**

Firmware untuk collect dual sensor:
```cpp
// main.cpp
#include "filter.h"

// Encoder reading
volatile long encoder_ticks = 0;
const float TICKS_PER_MM = 10.0;  // Calibration constant

// Accelerometer
#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;

// Fusion parameters
const float W_ENCODER = 0.7;
const float W_ACCEL = 0.3;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  
  // Encoder interrupt setup
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, CHANGE);
}

void loop() {
  // Read encoder
  float encoder_pos = encoder_ticks / TICKS_PER_MM;  // mm
  
  // Read accelerometer
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  float accel_x = ax / 16384.0 * 9.81;  // m/s² (for ±2g range)
  
  // TODO: integrate accel → velocity → position
  // TODO: apply complementary filter
  
  Serial.print(millis());
  Serial.print(",");
  Serial.print(encoder_pos);
  Serial.print(",");
  Serial.println(accel_x);
  
  delay(10);  // 100Hz
}

void encoderISR() {
  // Encoder interrupt handler (increment/decrement ticks)
  encoder_ticks++;
}
```

**b. Integrate Accelerometer → Position**

Numerical integration:
$$v[n] = v[n-1] + a[n] \cdot \Delta t$$
$$x[n] = x[n-1] + v[n] \cdot \Delta t$$

**Implementasi**:
```cpp
// Integration state
float accel_velocity = 0;
float accel_position = 0;
const float DT = 0.01;  // 100Hz → 10ms

void loop() {
  // ... (encoder & accel reading)
  
  // Integrate acceleration → velocity → position
  accel_velocity += accel_x * DT;
  accel_position += accel_velocity * DT;
  
  // ... (rest of code)
}
```

**c. Apply Low-Pass Filter ke Encoder**

```cpp
LowPassFilter encoder_lpf(5.0, 100.0);  // 5Hz cutoff, 100Hz sampling

void loop() {
  float encoder_pos_raw = encoder_ticks / TICKS_PER_MM;
  float encoder_pos_filtered = encoder_lpf.update(encoder_pos_raw);
  
  // ... (continue)
}
```

**d. Apply High-Pass Filter ke Accel Position** (implicit via complementary)

High-pass filter tidak explicit, tapi implicit via weight:
- Encoder (low-pass) weight = 0.7 → low frequency component
- Accel (high-pass) weight = 0.3 → high frequency component

**e. Complementary Filter Fusion**

```cpp
void loop() {
  // ... (encoder & accel processing)
  
  // Complementary filter
  float position_fused = W_ENCODER * encoder_pos_filtered + W_ACCEL * accel_position;
  
  // Log data
  Serial.print(encoder_pos_raw);
  Serial.print(",");
  Serial.print(accel_position);
  Serial.print(",");
  Serial.println(position_fused);
}
```

**f. Tune Weight untuk Optimal Result**

Test weight combinations:
| Test | W_Encoder | W_Accel | Expected Behavior |
|------|-----------|---------|-------------------|
| 1 | 0.9 | 0.1 | Mostly encoder (noisy tapi no drift) |
| 2 | 0.7 | 0.3 | **Balanced** |
| 3 | 0.5 | 0.5 | Equal weight |
| 4 | 0.3 | 0.7 | Mostly accel (smooth tapi drift) |

**g. Analyze Performance**

Metrics:
- **Drift**: position error over time (mm per 10s)
- **Noise RMS**: standard deviation dari position estimate
- **Response time**: step input → 90% settle time

#### Data Pengamatan

| W_Accel | W_Enc | Drift (mm over 10s) | Noise RMS (mm) | Response Time (ms) |
|---------|-------|---------------------|----------------|-------------------|
| 0.1 | 0.9 | ___ | ___ | ___ |
| 0.3 | 0.7 | ___ | ___ | ___ |
| 0.5 | 0.5 | ___ | ___ | ___ |
| 0.7 | 0.3 | ___ | ___ | ___ |

#### Deliverable
- [ ] Code: `fusion.cpp` (complementary filter implementation)
- [ ] Plot: position estimate comparison (encoder vs accel vs fused)
- [ ] Analysis: weight optimization, performance metric

#### Pertanyaan Analisis
1. Weight optimal untuk aplikasi ini? (trade-off drift vs noise)
2. Drift dari accel integration berapa mm/s?
3. Complementary filter vs encoder-only: improvement berapa %?
4. Apakah weight perlu adaptive (change based on motion condition)?

---

### PERCOBAAN 5: KALMAN FILTER BASIC

**Tujuan**: Implementasi optimal filter untuk 1D motion (position tracking).

#### Teori Singkat

**State vector**: $\mathbf{x} = \begin{bmatrix} position \\ velocity \end{bmatrix}$

**State transition** (constant velocity model):
$$\begin{bmatrix} x_{k+1} \\ v_{k+1} \end{bmatrix} = \begin{bmatrix} 1 & \Delta t \\ 0 & 1 \end{bmatrix} \begin{bmatrix} x_k \\ v_k \end{bmatrix}$$

**Measurement** (position only):
$$z_k = \begin{bmatrix} 1 & 0 \end{bmatrix} \begin{bmatrix} x_k \\ v_k \end{bmatrix}$$

#### Langkah Percobaan

**a. Setup Kalman Filter untuk 1D Motion**

Code (`kalman.h`):
```cpp
// kalman.h
#ifndef KALMAN_H
#define KALMAN_H

class KalmanFilter1D {
private:
  // State estimate: [position, velocity]
  float x_pos;
  float x_vel;
  
  // Error covariance matrix P (2x2)
  float P[2][2];
  
  // Process noise covariance Q (2x2)
  float Q[2][2];
  
  // Measurement noise covariance R (scalar, position only)
  float R;
  
  // Sampling time
  float dt;
  
public:
  KalmanFilter1D(float sampling_time, float process_noise, float measurement_noise) {
    dt = sampling_time;
    R = measurement_noise;
    
    // Initialize state
    x_pos = 0;
    x_vel = 0;
    
    // Initialize P (identity)
    P[0][0] = 1; P[0][1] = 0;
    P[1][0] = 0; P[1][1] = 1;
    
    // Initialize Q (process noise)
    Q[0][0] = process_noise;
    Q[0][1] = 0;
    Q[1][0] = 0;
    Q[1][1] = process_noise;
  }
  
  void predict() {
    // State prediction: x_pred = A * x_prev
    float x_pos_pred = x_pos + x_vel * dt;
    float x_vel_pred = x_vel;
    
    // Covariance prediction: P_pred = A * P * A^T + Q
    float P_pred[2][2];
    P_pred[0][0] = P[0][0] + dt * (P[1][0] + P[0][1]) + dt * dt * P[1][1] + Q[0][0];
    P_pred[0][1] = P[0][1] + dt * P[1][1] + Q[0][1];
    P_pred[1][0] = P[1][0] + dt * P[1][1] + Q[1][0];
    P_pred[1][1] = P[1][1] + Q[1][1];
    
    // Update state & covariance
    x_pos = x_pos_pred;
    x_vel = x_vel_pred;
    P[0][0] = P_pred[0][0]; P[0][1] = P_pred[0][1];
    P[1][0] = P_pred[1][0]; P[1][1] = P_pred[1][1];
  }
  
  void update(float measurement) {
    // Measurement residual: y = z - H * x_pred
    float y = measurement - x_pos;  // H = [1, 0]
    
    // Residual covariance: S = H * P * H^T + R
    float S = P[0][0] + R;
    
    // Kalman gain: K = P * H^T / S
    float K[2];
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;
    
    // State update: x = x_pred + K * y
    x_pos = x_pos + K[0] * y;
    x_vel = x_vel + K[1] * y;
    
    // Covariance update: P = (I - K * H) * P
    float P_new[2][2];
    P_new[0][0] = (1 - K[0]) * P[0][0];
    P_new[0][1] = (1 - K[0]) * P[0][1];
    P_new[1][0] = P[1][0] - K[1] * P[0][0];
    P_new[1][1] = P[1][1] - K[1] * P[0][1];
    
    P[0][0] = P_new[0][0]; P[0][1] = P_new[0][1];
    P[1][0] = P_new[1][0]; P[1][1] = P_new[1][1];
  }
  
  float getPosition() { return x_pos; }
  float getVelocity() { return x_vel; }
  float getKalmanGain() { return P[0][0] / (P[0][0] + R); }  // Simplified K[0]
};

#endif
```

**b. Tuning Process Noise (Q) dan Measurement Noise (R)**

**Tuning guideline**:
- **Q (process noise)**: seberapa percaya model? (large Q → trust model kurang)
- **R (measurement noise)**: seberapa percaya sensor? (large R → trust sensor kurang)

**Test matrix**:
| Test | Q Value | R Value | Meaning |
|------|---------|---------|---------|
| 1 | 0.001 | 10 | Trust model, distrust sensor |
| 2 | 0.01 | 10 | Moderate model trust |
| 3 | 0.001 | 1 | Trust both model & sensor |
| 4 | 0.1 | 10 | Distrust model, rely on sensor |

**c. Implement State Transition & Measurement Update**

Main loop:
```cpp
KalmanFilter1D kf(0.01, 0.001, 10.0);  // dt=10ms, Q=0.001, R=10

void loop() {
  // Step 1: Predict
  kf.predict();
  
  // Step 2: Measurement (encoder position)
  float encoder_pos = encoder_ticks / TICKS_PER_MM;
  
  // Step 3: Update
  kf.update(encoder_pos);
  
  // Get estimate
  float pos_estimate = kf.getPosition();
  float vel_estimate = kf.getVelocity();
  float kalman_gain = kf.getKalmanGain();
  
  // Log
  Serial.print(encoder_pos);      // Raw measurement
  Serial.print(",");
  Serial.print(pos_estimate);     // Kalman estimate
  Serial.print(",");
  Serial.println(kalman_gain);    // Kalman gain evolution
  
  delay(10);
}
```

**d. Analyze Kalman Gain Evolution**

Kalman gain akan **converge** setelah beberapa iteration:
- Initial: K besar (trust measurement more)
- Converged: K stabil (balance prediction & measurement)

Plot Kalman gain vs time → verify convergence.

#### Data Pengamatan

| Q Value | R Value | Position Error (mm RMS) | Convergence Time (s) | Final Kalman Gain |
|---------|---------|------------------------|----------------------|-------------------|
| 0.001 | 10 | ___ | ___ | ___ |
| 0.01 | 10 | ___ | ___ | ___ |
| 0.001 | 1 | ___ | ___ | ___ |
| 0.1 | 10 | ___ | ___ | ___ |

#### Deliverable
- [ ] Code: `kalman.h` (Kalman filter class)
- [ ] Plot: Kalman gain evolution (K vs time)
- [ ] Plot: position estimate (raw vs Kalman)
- [ ] Analysis: Q/R tuning sensitivity

#### Pertanyaan Analisis
1. Q/R ratio optimal untuk aplikasi ini?
2. Kalman filter vs complementary filter: accuracy improvement?
3. Computational cost: Kalman filter berapa kali lebih berat dari complementary?
4. Kapan gunakan Kalman vs complementary?

---

### PERCOBAAN 6: SENSOR REDUNDANCY VOTING

**Tujuan**: Fault tolerance melalui multiple sensor + voting logic.

#### Langkah Percobaan

**a. Simulate 3 Sensor**

Setup:
- **Sensor 1**: Encoder (primary)
- **Sensor 2**: IMU accelerometer integration
- **Sensor 3**: External reference (potentiometer atau second encoder)

**b. Implementasi 2-out-of-3 Voting Logic**

Code (`fusion.cpp`):
```cpp
// fusion.cpp
#include "fusion.h"

struct VotingResult {
  float value;
  bool valid;
  int fault_sensor_id;  // -1 if no fault, 0/1/2 if fault detected
};

VotingResult voting_3sensor(float s1, float s2, float s3, float tolerance) {
  VotingResult result;
  result.valid = false;
  result.fault_sensor_id = -1;
  
  // Check which 2 sensors agree (within tolerance)
  float diff_12 = abs(s1 - s2);
  float diff_13 = abs(s1 - s3);
  float diff_23 = abs(s2 - s3);
  
  if (diff_12 < tolerance && diff_13 < tolerance) {
    // s1, s2, s3 all agree → use average of all
    result.value = (s1 + s2 + s3) / 3.0;
    result.valid = true;
  }
  else if (diff_12 < tolerance) {
    // s1 and s2 agree, s3 is outlier
    result.value = (s1 + s2) / 2.0;
    result.valid = true;
    result.fault_sensor_id = 2;  // s3 fault
  }
  else if (diff_13 < tolerance) {
    // s1 and s3 agree, s2 is outlier
    result.value = (s1 + s3) / 2.0;
    result.valid = true;
    result.fault_sensor_id = 1;  // s2 fault
  }
  else if (diff_23 < tolerance) {
    // s2 and s3 agree, s1 is outlier
    result.value = (s2 + s3) / 2.0;
    result.valid = true;
    result.fault_sensor_id = 0;  // s1 fault
  }
  else {
    // No consensus → all disagree (critical error)
    result.value = 0;
    result.valid = false;
    result.fault_sensor_id = -2;  // Multi-fault
  }
  
  return result;
}
```

**c. Inject Sensor Fault**

Fault scenarios untuk test:
1. **Offset fault**: sensor 1 = true_value + 10mm (constant offset)
2. **Sudden spike**: sensor 2 = true_value + 50mm (1 sample spike)
3. **Timeout**: sensor 3 = 0 (sensor disconnect atau freeze)

**Inject fault via software**:
```cpp
void loop() {
  float sensor1 = readEncoder();
  float sensor2 = readIMU();
  float sensor3 = readReference();
  
  // Inject fault for testing (comment out for production)
  #ifdef TEST_FAULT_S1_OFFSET
    sensor1 += 10.0;  // Offset 10mm
  #endif
  
  #ifdef TEST_FAULT_S2_SPIKE
    if (millis() > 5000 && millis() < 5100) {
      sensor2 += 50.0;  // Spike at t=5s
    }
  #endif
  
  // Voting
  VotingResult result = voting_3sensor(sensor1, sensor2, sensor3, 5.0);  // tolerance=5mm
  
  if (result.valid) {
    // Use voted value
    float position = result.value;
    if (result.fault_sensor_id >= 0) {
      Serial.print("WARNING: Sensor ");
      Serial.print(result.fault_sensor_id);
      Serial.println(" fault detected!");
    }
  }
  else {
    // Critical error: no consensus
    Serial.println("CRITICAL: Multi-sensor fault!");
    // Trigger alarm, stop system
  }
}
```

**d. Measure Fault Detection Performance**

Metrics:
- **Detection latency**: time dari fault inject sampai voting detect
- **False positive rate**: berapa % voting flag error padahal sensor OK
- **Accuracy**: voting value vs true value saat 1 sensor fault

#### Data Pengamatan

| Sensor Fault | Detection Latency (ms) | False Positive Rate (%) | Position Error (mm) |
|--------------|------------------------|------------------------|---------------------|
| s1 offset +10mm | ___ | ___ | ___ |
| s2 sudden spike +50mm | ___ | ___ | ___ |
| s3 timeout (freeze) | ___ | ___ | ___ |
| No fault (baseline) | N/A | ___ | ___ |

#### Deliverable
- [ ] Code: `voting_3sensor()` function di `fusion.cpp`
- [ ] Test result table (fault detection accuracy)
- [ ] Fault detection analysis (latency, false positive)
- [ ] Recommendation: tolerance parameter untuk production

#### Pertanyaan Analisis
1. Tolerance 5mm vs 2mm: mana yang lebih robust? Trade-off?
2. False positive rate acceptable berapa %?
3. Multi-sensor fault scenario: apa action yang harus dilakukan?
4. Voting logic vs single sensor: cost-benefit analysis?

---

## 4. DATA PENGAMATAN (SUMMARY)

### 4.1 Raw Sensor FFT Spectrum
- [ ] Image: `encoder_fft_spectrum.png`
- [ ] Image: `accel_fft_spectrum.png`
- [ ] Noise frequency identification table

### 4.2 Filtered vs Raw Signal Plot
- [ ] Image: `moving_average_comparison.png`
- [ ] Image: `lowpass_vs_moving_average.png`
- [ ] Image: `complementary_filter_result.png`
- [ ] Image: `kalman_filter_estimate.png`

### 4.3 Filter Parameter Table

| Filter Type | Parameter | Value | Noise Reduction (%) | Latency (ms) |
|-------------|-----------|-------|---------------------|--------------|
| Moving Average | Window | 5 | ___ | 20 |
| Moving Average | Window | 10 | ___ | 45 |
| Low-Pass | Cutoff | 5Hz | ___ | ~20 |
| Low-Pass | Cutoff | 10Hz | ___ | ~10 |
| Complementary | W_enc / W_accel | 0.7 / 0.3 | ___ | ~20 |
| Kalman | Q / R | 0.001 / 10 | ___ | ~15 |

### 4.4 Complementary Filter Weight Optimization

- [ ] Optimal weight: W_enc = ___, W_accel = ___
- [ ] Drift rate: ___ mm/s
- [ ] Noise RMS: ___ mm
- [ ] Response time: ___ ms

### 4.5 Kalman Filter Q/R Tuning Sensitivity

- [ ] Optimal Q: ___
- [ ] Optimal R: ___
- [ ] Convergence time: ___ s
- [ ] Final Kalman gain: ___

### 4.6 Sensor Voting Fault Detection Accuracy

- [ ] Detection rate: ___ %
- [ ] False positive rate: ___ %
- [ ] Detection latency: ___ ms
- [ ] Recommended tolerance: ___ mm

---

## 5. ANALISIS

### PERTANYAAN WAJIB DIJAWAB:

#### 1. Filter Selection
**Q**: Mana filter yang paling sesuai untuk sistem ini? Moving average, low-pass, complementary, atau Kalman?

**Pertimbangan**:
- Computational cost (CPU usage)
- Memory requirement
- Noise reduction effectiveness
- Latency
- Ease of tuning

**Jawaban**: ___

---

#### 2. Trade-off Analysis
**Q**: Trade-off apa yang terjadi saat increase filter window size (moving average)?

**Aspek yang dianalisis**:
- Noise reduction (quantify improvement)
- Latency increase (calculate delay)
- Control stability impact (saat integrate dengan PID loop)

**Jawaban**: ___

---

#### 3. Complementary Filter Weight
**Q**: Complementary filter weight optimal berapa? Mengapa?

**Data yang dipertimbangkan**:
- Encoder noise level (RMS)
- Accel drift rate (mm/s)
- Response time requirement
- Application priority (accuracy vs speed)

**Jawaban**: ___

---

#### 4. Kalman vs Complementary
**Q**: Kalman filter vs complementary filter: kapan gunakan yang mana?

**Comparison criteria**:
- Accuracy
- Computational cost
- Tuning complexity
- Number of sensors required

**Jawaban**: ___

---

#### 5. Sensor Voting Effectiveness
**Q**: Sensor voting effectiveness untuk fault tolerance?

**Metrics**:
- Detection rate (%)
- False positive rate (%)
- Cost (3× sensor vs benefit)

**Jawaban**: ___

---

#### 6. Production Parameter Recommendation
**Q**: Parameter final untuk production system?

**Recommendation table**:
| Parameter | Value | Justification |
|-----------|-------|---------------|
| Filter type | ___ | ___ |
| Filter parameter | ___ | ___ |
| Sampling rate | ___ Hz | ___ |
| Sensor fusion method | ___ | ___ |
| Redundancy | Yes/No | ___ |

**Jawaban**: ___

---

## 6. KESIMPULAN

### 6.1 Kesimpulan Umum

Tuliskan kesimpulan berdasarkan hasil praktikum:

1. **Filter design** critical untuk system stability karena ___

2. **Kalman filter** powerful untuk ___ tapi kompleks karena ___

3. **Complementary filter** simple but effective untuk ___ karena ___

4. **Redundancy** important untuk safety-critical application karena ___

5. **Trade-off** antara noise reduction vs latency harus ___ sesuai ___

### 6.2 Lesson Learned

Apa yang dipelajari dari praktikum ini:
- ___
- ___
- ___

### 6.3 Recommendation untuk Improvement

Saran untuk perbaikan sistem:
- ___
- ___
- ___

---

## LAMPIRAN

### Lampiran A: Troubleshooting Guide

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| Filter tidak reduce noise | Window terlalu kecil | Increase window size atau decrease cutoff |
| Control oscillate dengan filter | Filter delay terlalu besar | Reduce window size, increase cutoff |
| Kalman diverge | Q/R parameter salah | Re-tune Q/R, check measurement noise estimate |
| Sensor voting false positive | Tolerance terlalu ketat | Relax tolerance, check sensor calibration |
| Accel drift severe | Integration without bias removal | Add bias compensation atau high-pass filter |

### Lampiran B: Kalman Filter Derivation (Optional)

Reference: Welch & Bishop (2006), "An Introduction to the Kalman Filter"

### Lampiran C: Code Repository

GitHub: [link to repository]

File structure:
```
Modul-04/
├── Praktikum/Code_Embedded/
│   ├── src/
│   │   ├── main.cpp
│   │   ├── filter.h
│   │   ├── kalman.h
│   │   └── fusion.cpp
│   ├── platformio.ini
│   └── README.md
└── output/
    ├── sensor_data.csv
    ├── *.png (plots)
    └── analysis_report.md
```

---

**Disusun oleh**: [Instructor Name]  
**Tanggal**: February 2026  
**Modul**: Batch 2 - Modul 4  
**Revisi**: 1.0  
