# PROJECT MODUL 4: SENSOR FUSION SYSTEM DESIGN FOR ROBUST LOCALIZATION

---

## OVERVIEW

**Judul**: Sensor Fusion System Design for Robust Localization

**Deskripsi**: Merancang dan mengimplementasikan sistem sensor fusion lengkap untuk robot localization (AGV atau robot arm), menggunakan multiple sensor (encoder, IMU, reference), multiple filter technique (moving average, low-pass, complementary, Kalman), dan sensor redundancy voting untuk fault tolerance.

**Durasi**: 2-3 minggu

---

## SPESIFIKASI SYSTEM

### Functional Requirements

1. **Multi-Sensor Input**:
   - Encoder (rotary encoder, minimum 500 PPR)
   - IMU 6-axis (accelerometer + gyroscope) atau 9-axis (+ magnetometer)
   - Reference sensor (optional: potentiometer, ultrasonic, atau second encoder)

2. **Sensor Fusion**:
   - Implement complementary filter (encoder + accel)
   - Implement Kalman filter basic (1D atau 2D motion)
   - Compare performance: accuracy, latency, robustness

3. **Fault Tolerance**:
   - Sensor redundancy (minimum 2 sensor, recommended 3)
   - Voting logic (2-out-of-3 atau weighted voting)
   - Fault detection & alarm

4. **Performance Target**:
   - Position accuracy: ±1mm (atau ±0.5° untuk rotary)
   - Noise reduction: > 70% (RMS)
   - Latency: < 50ms (end-to-end sensor → fused output)
   - Fault detection rate: > 95%
   - False positive rate: < 5%

### Non-Functional Requirements

1. **Real-time**: sampling rate 100Hz minimum
2. **Efficiency**: CPU usage < 50% (allow headroom untuk control loop)
3. **Maintainability**: modular code, easy parameter tuning
4. **Scalability**: design support add sensor baru (modular architecture)

---

## DELIVERABLE

### 1. Sensor Noise Analysis Report

**Content**:
- Noise source identification (EMI, thermal, mechanical)
- Frequency spectrum analysis (FFT plot)
- Noise characteristic: type (white, 1/f, impulse), amplitude (RMS), frequency
- Sampling rate selection justification (Nyquist theorem)

**Format**:
- Document: `01_Sensor_Noise_Analysis.md`
- Data: `sensor_data.csv`
- Images: `encoder_fft.png`, `accel_fft.png`, `noise_time_domain.png`

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Noise source identification (correct & complete) | 20% | /20 |
| FFT analysis (correct frequency identification) | 30% | /30 |
| RMS calculation (accurate) | 20% | /20 |
| Sampling rate justification (Nyquist applied) | 20% | /20 |
| Report quality (clear, structured) | 10% | /10 |
| **Total** | 100% | /100 |

---

### 2. Filter Comparison Document

**Content**:
- Theory: moving average, low-pass, complementary, Kalman
- Implementation code untuk each filter
- Measurement result:
  - Noise reduction (%)
  - Latency (ms)
  - Computational cost (CPU usage, memory)
- Performance comparison table
- Recommendation: filter selection untuk different scenario

**Format**:
- Document: `02_Filter_Comparison.md`
- Code: `filter.h`, `kalman.h`
- Data: `filter_performance.csv`
- Images: `filter_comparison_plot.png`, `latency_analysis.png`

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Theory explanation (correct & complete) | 15% | /15 |
| Code implementation (all 4 filters working) | 30% | /30 |
| Measurement data (accurate & complete) | 25% | /25 |
| Comparison analysis (insightful) | 20% | /20 |
| Recommendation (justified) | 10% | /10 |
| **Total** | 100% | /100 |

---

### 3. Complementary Filter Design

**Content**:
- Block diagram (sensor path, filter path, fusion point)
- Weight optimization process (tuning log)
- Final parameter: W_encoder, W_accel, filter cutoff
- Performance measurement:
  - Drift test (position error over time)
  - Noise reduction (RMS before/after)
  - Response time (step input → settle)
- Comparison: encoder-only vs accel-only vs fused

**Format**:
- Document: `03_Complementary_Filter_Design.md`
- Code: `fusion.cpp`
- Data: `complementary_tuning_log.csv`
- Images: `complementary_block_diagram.png`, `weight_optimization.png`, `fusion_result.png`

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Block diagram (clear & correct) | 15% | /15 |
| Weight optimization (systematic process) | 25% | /25 |
| Performance measurement (complete data) | 30% | /30 |
| Comparison analysis (encoder vs accel vs fused) | 20% | /20 |
| Final parameter justification | 10% | /10 |
| **Total** | 100% | /100 |

---

### 4. Kalman Filter Implementation

**Content**:
- State model definition (position, velocity, atau extended)
- State transition matrix (A), measurement matrix (H)
- Process noise covariance (Q) tuning process
- Measurement noise covariance (R) estimation
- Kalman gain evolution plot
- Estimate error analysis (vs ground truth)
- Q/R sensitivity analysis

**Format**:
- Document: `04_Kalman_Filter_Implementation.md`
- Code: `kalman.h`
- Data: `kalman_tuning.csv`, `kalman_gain.csv`
- Images: `kalman_gain_evolution.png`, `kalman_estimate_vs_raw.png`, `QR_sensitivity.png`

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| State model definition (correct) | 15% | /15 |
| Code implementation (working Kalman filter) | 30% | /30 |
| Q/R tuning (systematic process) | 25% | /25 |
| Kalman gain analysis (convergence verified) | 15% | /15 |
| Performance vs complementary filter | 15% | /15 |
| **Total** | 100% | /100 |

---

### 5. Sensor Voting Logic

**Content**:
- Voting algorithm design (2-out-of-3 atau variant)
- Fault scenario test:
  - Offset fault
  - Spike fault
  - Timeout fault
  - Multi-fault
- Fault detection accuracy measurement
- False positive rate analysis
- Tolerance parameter tuning
- Recommendation: voting strategy untuk production

**Format**:
- Document: `05_Sensor_Voting_Logic.md`
- Code: `voting_3sensor()` di `fusion.cpp`
- Data: `voting_test_results.csv`
- Images: `voting_architecture.png`, `fault_detection_plot.png`

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Voting algorithm (correct logic) | 25% | /25 |
| Fault scenario testing (comprehensive) | 30% | /30 |
| Detection accuracy (> 95%) | 20% | /20 |
| False positive analysis | 15% | /15 |
| Tolerance tuning justification | 10% | /10 |
| **Total** | 100% | /100 |

---

### 6. Performance Report

**Content**:
- Accuracy comparison table (all filters vs raw sensor)
- Latency measurement (phase lag analysis)
- Robustness test (with disturbance: vibration, EMI inject)
- Trade-off analysis: accuracy vs latency vs computational cost
- Filter selection matrix (decision tree atau scoring matrix)
- Recommendation: best filter untuk different application scenario

**Format**:
- Document: `06_Performance_Report.md`
- Data: `performance_summary.csv`
- Images: `accuracy_comparison.png`, `latency_comparison.png`, `tradeoff_matrix.png`

**Example performance comparison table**:
| Filter | Accuracy (mm RMS) | Latency (ms) | CPU (%) | Memory (bytes) | Score |
|--------|-------------------|--------------|---------|----------------|-------|
| Raw (no filter) | 5.2 | 0 | 1% | 0 | 2/10 |
| Moving Avg (W=5) | 2.1 | 20 | 5% | 20 | 7/10 |
| Low-Pass (fc=5Hz) | 1.8 | 18 | 3% | 8 | 8/10 |
| Complementary | 1.2 | 22 | 8% | 50 | 9/10 |
| Kalman | 0.9 | 15 | 15% | 200 | 10/10 |

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Performance data (complete & accurate) | 30% | /30 |
| Trade-off analysis (insightful) | 25% | /25 |
| Filter selection matrix (practical) | 20% | /20 |
| Recommendation (justified & actionable) | 15% | /15 |
| Report quality (professional) | 10% | /10 |
| **Total** | 100% | /100 |

---

### 7. Production Parameter Recommendation

**Content**:
- Final filter selection untuk production
- Final parameter values (window size, α, W_enc, W_accel, Q, R, tolerance)
- Justification untuk each parameter
- Performance guarantee (spec sheet style)
- Installation guide
- Tuning procedure (for field adjustment)

**Format**:
- Document: `07_Production_Parameter_Recommendation.md`
- File: `config.h` (production-ready parameter)

**Example config.h**:
```cpp
// config.h - Production Parameter
// Sensor Fusion System v1.0

// === FILTER SELECTION ===
#define FILTER_TYPE COMPLEMENTARY  // Options: MA, LOWPASS, COMPLEMENTARY, KALMAN

// === COMPLEMENTARY FILTER PARAMETER ===
#define W_ENCODER 0.7
#define W_ACCEL 0.3
#define ENCODER_LOWPASS_CUTOFF 5.0  // Hz
#define ACCEL_HIGHPASS_CUTOFF 1.0   // Hz

// === KALMAN FILTER PARAMETER (if selected) ===
#define KALMAN_Q 0.001
#define KALMAN_R 10.0

// === SENSOR VOTING ===
#define ENABLE_VOTING true
#define VOTING_TOLERANCE 5.0  // mm
#define FAULT_ALARM_THRESHOLD 10  // consecutive fault count

// === SAMPLING ===
#define SAMPLE_RATE 100  // Hz
#define SAMPLE_PERIOD_MS 10
```

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Parameter selection (justified) | 40% | /40 |
| Performance guarantee (realistic & measurable) | 25% | /25 |
| Installation guide (clear) | 15% | /15 |
| Tuning procedure (step-by-step) | 20% | /20 |
| **Total** | 100% | /100 |

---

### 8. Code Organization (GitHub)

**Repository structure**:
```
sensor-fusion-project/
├── README.md                     # Overview, how to build & run
├── docs/
│   ├── 01_Sensor_Noise_Analysis.md
│   ├── 02_Filter_Comparison.md
│   ├── 03_Complementary_Filter_Design.md
│   ├── 04_Kalman_Filter_Implementation.md
│   ├── 05_Sensor_Voting_Logic.md
│   ├── 06_Performance_Report.md
│   └── 07_Production_Parameter_Recommendation.md
├── firmware/
│   ├── src/
│   │   ├── main.cpp
│   │   ├── config.h
│   │   ├── filter.h
│   │   ├── kalman.h
│   │   └── fusion.cpp
│   ├── platformio.ini
│   └── README.md
├── data/
│   ├── sensor_data.csv
│   ├── filter_performance.csv
│   ├── kalman_tuning.csv
│   └── voting_test_results.csv
├── analysis/
│   ├── noise_analysis.ipynb      # Jupyter notebook (Python)
│   ├── filter_comparison.ipynb
│   ├── kalman_tuning.ipynb
│   └── requirements.txt
├── output/
│   ├── plots/                    # All PNG images
│   └── videos/                   # Demo videos
└── .gitignore
```

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Code organization (modular, clean) | 30% | /30 |
| Documentation (README complete) | 25% | /25 |
| Git history (meaningful commits) | 15% | /15 |
| Reproducibility (easy to build & run) | 20% | /20 |
| Code quality (comments, naming) | 10% | /10 |
| **Total** | 100% | /100 |

---

### 9. Video Demo

**Content** (10-15 menit):
1. **Intro** (1 min): project overview, goal, spec
2. **Hardware setup** (2 min): sensor placement, wiring
3. **Noise analysis demo** (2 min): oscilloscope showing noise, FFT plot
4. **Filter comparison** (3 min): raw vs MA vs lowpass vs complementary vs Kalman (overlay plot)
5. **Sensor fusion result** (3 min): encoder-only vs accel-only vs fused position (demo motion)
6. **Voting logic demo** (2 min): inject fault, voting detect & compensate
7. **Performance summary** (1 min): metric table, recommendation
8. **Outro** (1 min): lessons learned, future work

**Format**:
- Video file: `sensor_fusion_demo.mp4`
- Upload ke: YouTube (unlisted) atau Google Drive
- Include subtitle (bahasa Indonesia)

**Rubrik**:
| Criteria | Weight | Points |
|----------|--------|--------|
| Content completeness (cover all points) | 30% | /30 |
| Demonstration quality (clear, visible) | 25% | /25 |
| Explanation clarity (easy to understand) | 20% | /20 |
| Video quality (audio, visual) | 15% | /15 |
| Time management (not too long/short) | 10% | /10 |
| **Total** | 100% | /100 |

---

## BONUS FEATURES (Optional)

### BONUS 1: Extended Kalman Filter (EKF) untuk Nonlinear System

**Challenge**: Apply Kalman filter ke nonlinear motion model.

**Example**: 2D localization dengan [x, y, θ] state, nonlinear observation model (polar coordinate).

**Implementation**:
- Linearize motion model via Jacobian matrix
- EKF predict & update step
- Compare EKF vs basic Kalman

**Deliverable**:
- EKF implementation code
- Performance comparison (EKF vs linear Kalman)
- Analysis: when EKF necessary?

**Bonus points**: +15

---

### BONUS 2: Advanced Voting (Weighted Voting, Confidence Score)

**Challenge**: Improve voting logic dengan confidence scoring.

**Concept**:
- Each sensor punya confidence score (based on noise level, drift rate)
- Voting weighted by confidence: high confidence sensor punya weight lebih besar
- Adaptive confidence (update based on historical accuracy)

**Implementation**:
```cpp
struct SensorWithConfidence {
  float value;
  float confidence;  // 0.0 - 1.0
};

float weighted_voting(SensorWithConfidence s1, SensorWithConfidence s2, SensorWithConfidence s3) {
  float total_confidence = s1.confidence + s2.confidence + s3.confidence;
  float weighted_value = (s1.value * s1.confidence + 
                          s2.value * s2.confidence + 
                          s3.value * s3.confidence) / total_confidence;
  return weighted_value;
}
```

**Deliverable**:
- Weighted voting implementation
- Confidence score calculation method
- Performance comparison (simple voting vs weighted voting)

**Bonus points**: +10

---

### BONUS 3: Real-Time Visualization (Matplotlib Streaming atau RViz)

**Challenge**: Real-time plot sensor data & fused estimate.

**Option 1: Matplotlib Animation**
```python
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial

fig, ax = plt.subplots()
xdata, ydata = [], []
line, = ax.plot([], [])

def init():
    ax.set_xlim(0, 100)
    ax.set_ylim(-10, 10)
    return line,

def update(frame):
    # Read serial data
    data = ser.readline().decode().strip().split(',')
    xdata.append(float(data[0]))
    ydata.append(float(data[1]))
    line.set_data(xdata, ydata)
    return line,

ani = animation.FuncAnimation(fig, update, init_func=init, blit=True, interval=10)
plt.show()
```

**Option 2: RViz (ROS2 visualization)**
- Publish sensor data ke ROS2 topic
- Visualize di RViz (position marker, path trail)

**Deliverable**:
- Real-time visualization code
- Video demo (live plot)

**Bonus points**: +10

---

## TIMELINE

### Week 1: Sensor Analysis & Basic Filtering
- **Day 1-2**: Sensor noise analysis (Percobaan 1)
- **Day 3-4**: Moving average implementation & test (Percobaan 2)
- **Day 5**: Low-pass filter implementation (Percobaan 3)
- **Deliverable**: Report #1, #2 (partial)

### Week 2: Sensor Fusion
- **Day 1-2**: Complementary filter implementation & tuning (Percobaan 4)
- **Day 3-5**: Kalman filter implementation & Q/R tuning (Percobaan 5)
- **Deliverable**: Report #3, #4

### Week 3: Redundancy & Integration
- **Day 1-2**: Sensor voting logic (Percobaan 6)
- **Day 3**: Performance comparison & analysis
- **Day 4**: Production parameter optimization
- **Day 5**: Video recording & final report
- **Deliverable**: Report #5, #6, #7, video demo

---

## GRADING

### Total Score Calculation

| Deliverable | Weight | Max Points |
|-------------|--------|-----------|
| 1. Sensor Noise Analysis | 10% | 100 |
| 2. Filter Comparison | 15% | 100 |
| 3. Complementary Filter Design | 20% | 100 |
| 4. Kalman Filter Implementation | 20% | 100 |
| 5. Sensor Voting Logic | 15% | 100 |
| 6. Performance Report | 10% | 100 |
| 7. Production Parameter | 5% | 100 |
| 8. Code Organization | 10% | 100 |
| 9. Video Demo | 10% | 100 |
| **Bonus** (optional) | +5% each | +35 max |

**Total**: 115% (maximum dengan all bonus)

### Grade Conversion
- A: ≥ 85
- AB: 80-84
- B: 75-79
- BC: 70-74
- C: 65-69
- D: 50-64
- E: < 50

---

## SUBMISSION

### Format Submission

1. **GitHub repository**: public atau private (add instructor as collaborator)
2. **Report PDF**: compile all 7 documents ke 1 PDF (max 50 pages)
3. **Video**: YouTube link (unlisted) atau Google Drive
4. **Code**: ZIP file (backup, jika GitHub issue)

### Deadline

- **Week 2 checkpoint**: Deliverable #1, #2 (for feedback)
- **Final submission**: End of Week 3
- **Late penalty**: -10% per day (max 3 days, after that 0)

### Submission Link

[Google Classroom link atau email]

---

## EVALUATION CRITERIA (Detail)

### Code Quality (30% of total)

**Aspects**:
- **Modularity**: separate file untuk each filter, clear interface
- **Readability**: meaningful variable name, consistent indentation
- **Comments**: explain complex algorithm, formula reference
- **Error handling**: check sensor timeout, out-of-range value
- **Configuration**: easy parameter tuning (via `config.h`)

**Example good code**:
```cpp
// filter.h
/**
 * Low-pass digital filter (first-order IIR)
 * Formula: y[n] = α·x[n] + (1-α)·y[n-1]
 * where α = 2πfc·dt / (2πfc·dt + 1)
 * 
 * @param cutoff_freq Cutoff frequency (Hz)
 * @param sample_rate Sampling rate (Hz)
 */
class LowPassFilter {
  // ... (implementation)
};
```

### Analysis Quality (40% of total)

**Aspects**:
- **Data accuracy**: measurement correct, calculation correct
- **Insight**: understand why result happen, not just report number
- **Comparison**: meaningful comparison (apple-to-apple)
- **Justification**: recommendation based on data, not assumption
- **Critical thinking**: identify limitation, propose improvement

**Example good analysis**:
> "Complementary filter dengan weight W_enc=0.7, W_accel=0.3 menghasilkan accuracy 1.2mm RMS (improvement 77% vs raw encoder). Weight ini dipilih karena encoder noise level (2.1mm) lebih tinggi dari accel drift rate (0.5mm/s). Dalam 10 detik test, accel drift total hanya 5mm, sehingga accel weight 0.3 cukup untuk koreksi high-frequency noise tanpa introduce drift signifikan."

### Report Quality (20% of total)

**Aspects**:
- **Structure**: logical flow, easy to navigate
- **Clarity**: clear language, avoid ambiguity
- **Completeness**: cover all required points
- **Visual**: good quality images, plot, diagram
- **Professional**: formatting consistent, no typo

### Video Quality (10% of total)

**Aspects**:
- **Audio**: clear, no background noise
- **Visual**: hardware visible, plot readable
- **Pacing**: not too fast/slow
- **Explanation**: concept explained clearly
- **Demo**: show working system, not just theory

---

## FAQ & TROUBLESHOOTING

### Q1: Kalman filter diverge (estimate jadi infinity)
**A**: Check Q/R parameter. Jika R terlalu kecil (trust sensor terlalu besar), Kalman gain bisa unstable. Start dengan R besar (e.g., 100), then gradually decrease.

### Q2: Complementary filter drift over time
**A**: Accel integration introduce drift. Add high-pass filter atau bias removal. Alternative: periodic reset position saat robot stop.

### Q3: Voting logic false positive tinggi (> 10%)
**A**: Tolerance terlalu ketat. Relax tolerance (e.g., 5mm → 10mm), atau improve sensor calibration.

### Q4: CPU usage terlalu tinggi (> 50%)
**A**: Optimize code (avoid floating point division di inner loop), atau reduce sampling rate (100Hz → 50Hz).

### Q5: Filter latency terlalu besar untuk control
**A**: Reduce filter order (window size, cutoff frequency). Trade-off: accept more noise untuk lower latency.

---

## RESOURCES

### Learning Materials
- Lecture slide: [link]
- Reference code: [GitHub template]
- Kalman filter tutorial: [Welch & Bishop PDF]

### Tools
- FFT calculator: https://www.earlevel.com/main/
- Filter design tool: http://t-filter.engineerjs.com/
- PID + filter simulation: https://www.pidtuner.com/

### Contact
- Instructor: [email]
- Lab assistant: [email]
- Discussion forum: [Discord/Slack link]

---

**Prepared by**: [Instructor Name]  
**Last updated**: February 2026  
**Module**: Batch 2 - Modul 4 Project  
**Version**: 1.0  
