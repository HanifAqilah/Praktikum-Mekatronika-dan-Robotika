# PROJECT MODUL 6: PRECISION MOTION CONTROL SYSTEM

## INFORMASI PROJECT
- **Judul:** Precision Motion Control System with Advanced PID
- **Durasi:** 2-3 minggu
- **Tim:** 2-3 orang
- **Deliverable:** Controller implementation, tuning report, performance analysis, video demo

---

## 1. DESKRIPSI PROJECT

### 1.1 Latar Belakang
Kontrol gerak presisi adalah inti dari sistem robotika modern. Dari robot arm industri hingga CNC machine, kemampuan untuk mengontrol posisi dan kecepatan dengan akurasi tinggi sangat critical. Project ini menantang mahasiswa untuk merancang, mengimplementasi, dan mengoptimasi sistem motion control berbasis PID untuk aplikasi nyata.

### 1.2 Tujuan Project
1. Implement PID controller dengan anti-windup dan derivative filtering
2. Apply systematic tuning method (Ziegler-Nichols, Cohen-Coon, atau auto-tuning)
3. Achieve specified performance metrics (settling time, overshoot, steady-state error)
4. Implement trajectory planning (velocity profiling)
5. Demonstrate robustness terhadap disturbance

---

## 2. SPESIFIKASI REQUIREMENTS

### 2.1 Application Scenario (Pilih Salah Satu)

#### Option A: Position Control - Rotary System
- **Hardware:** DC motor dengan encoder (dari Modul 5)
- **Target:** Multi-point positioning dengan accuracy < 1°
- **Trajectory:** Smooth motion tanpa overshoot
- **Disturbance rejection:** External torque (up to 20% rated)

**Performance Criteria:**
| Parameter | Target |
|-----------|--------|
| Position accuracy | < 1° (< 0.018 rad) |
| Settling time (2%) | < 500 ms |
| Overshoot | < 5% |
| Steady-state error | < 0.5° |

#### Option B: Speed Control - Variable Load
- **Hardware:** DC motor dengan encoder
- **Target:** Constant speed regulation despite load variation
- **Speed range:** 10-200 RPM
- **Load variation:** 0-100% rated torque

**Performance Criteria:**
| Parameter | Target |
|-----------|--------|
| Speed accuracy | < 2% of setpoint |
| Speed ripple | < 5% peak-to-peak |
| Load recovery time | < 300 ms |
| Speed range ratio | 1:20 |

#### Option C: Dual-Axis Coordinated Control
- **Hardware:** 2x DC motor dengan encoder
- **Target:** Synchronized motion untuk X-Y positioning
- **Application:** 2D drawing atau pick-and-place
- **Coordination error:** < 2 mm

**Performance Criteria:**
| Parameter | Target |
|-----------|--------|
| Individual axis accuracy | < 1 mm |
| Synchronization error | < 2 mm |
| Path tracking error | < 3 mm |
| Contour error (circular) | < 5 mm |

---

## 3. DELIVERABLE

### 3.1 System Design Document
**Format:** PDF, 5-8 pages

**Isi:**
- Application scenario dan requirements analysis
- System block diagram (plant + controller)
- Hardware schematic
- Control strategy selection justification
- Expected performance calculation

### 3.2 Controller Implementation
**Format:** Source code (PlatformIO project)

**Requirements:**
```cpp
// Minimum feature yang harus diimplementasi:
class PIDController {
    // Basic PID dengan discrete-time implementation
    float compute(float setpoint, float feedback);
    
    // Anti-windup implementation
    void setOutputLimits(float min, float max);
    
    // Derivative filtering
    void setDerivativeFilter(float alpha);
    
    // Tuning parameters
    void setTunings(float Kp, float Ki, float Kd);
    
    // Feedforward option
    void setFeedforward(float ff_gain);
};
```

### 3.3 Tuning Report
**Format:** PDF, 5-10 pages

**Isi:**
1. **System Identification**
   - Step response open-loop
   - System parameters extraction (K, τ, θ)
   - Transfer function estimation

2. **Initial Tuning**
   - Method selection (Ziegler-Nichols, Cohen-Coon, etc.)
   - Calculated PID values
   - Theoretical prediction

3. **Experimental Tuning**
   - P-only test results
   - PI tuning results
   - Full PID tuning results
   - Comparison table

4. **Advanced Features**
   - Anti-windup effect demonstration
   - Derivative filter effect
   - Feedforward implementation (optional)

### 3.4 Performance Test Report
**Format:** PDF, 5-8 pages

**Isi:**
1. **Step Response Analysis**
   - Rise time measurement
   - Overshoot measurement
   - Settling time measurement
   - Steady-state error

2. **Disturbance Rejection**
   - Load disturbance test
   - Recovery time measurement
   - Steady-state error under disturbance

3. **Trajectory Tracking**
   - Multi-point positioning test
   - Velocity profile test
   - Following error analysis

4. **Comparison Table:**
| Test Condition | Target | Achieved | Pass/Fail |
|----------------|--------|----------|-----------|
| Position accuracy | < 1° | ? | |
| Settling time | < 500 ms | ? | |
| Overshoot | < 5% | ? | |
| Disturbance recovery | < 300 ms | ? | |

### 3.5 Video Demo
**Durasi:** 3-5 menit

**Konten:**
- System overview dan hardware
- Live tuning demonstration
- Performance test (step response)
- Disturbance rejection demo
- Trajectory tracking demo

---

## 4. MILESTONE & TIMELINE

### Week 1: System Identification & Basic Controller
**Tasks:**
- Hardware setup dan wiring
- Open-loop step response test
- System parameter identification
- Basic PID implementation
- Initial Ziegler-Nichols tuning

**Checkpoint:** Working PID dengan reasonable response (overshoot < 30%)

### Week 2: Advanced Features & Optimization
**Tasks:**
- Implement anti-windup
- Implement derivative filtering
- Fine-tune parameters
- Disturbance rejection test
- Trajectory planning integration

**Checkpoint:** Meet 80% of performance targets

### Week 3: Testing & Documentation
**Tasks:**
- Comprehensive testing
- Data collection dan analysis
- Report writing
- Video recording
- Final presentation preparation

**Checkpoint:** Complete deliverables submitted

---

## 5. RUBRIK PENILAIAN

### 5.1 Technical Implementation (40%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| PID Implementation | Complete dengan anti-windup, filtering, feedforward | Complete dengan anti-windup | Basic PID working | Not working |
| System Identification | Accurate model, validated | Good estimation | Basic attempt | Missing |
| Tuning Process | Systematic, documented, optimal | Systematic, good results | Trial-and-error documented | Random |
| Code Quality | Clean, modular, well-commented | Good structure | Functional | Messy |

### 5.2 Performance Achievement (30%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Position/Speed Accuracy | Exceeds target | Meets target | Within 150% of target | Fails |
| Settling Time | Exceeds target | Meets target | Within 150% of target | Fails |
| Overshoot | < 5% | < 10% | < 20% | > 20% |
| Disturbance Rejection | Excellent recovery | Good recovery | Slow recovery | No recovery |

### 5.3 Documentation (20%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Design Document | Complete, professional | Good coverage | Adequate | Incomplete |
| Tuning Report | Detailed analysis, insights | Good analysis | Basic reporting | Missing |
| Test Report | Comprehensive, validated | Good coverage | Basic tests | Incomplete |
| Code Documentation | Excellent comments, README | Good comments | Some comments | No documentation |

### 5.4 Video Demo (10%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Presentation | Professional, clear | Good explanation | Adequate | Confusing |
| Demo Quality | All features shown | Most features | Some features | Poor demo |
| Technical Depth | Deep understanding shown | Good understanding | Basic understanding | Superficial |

---

## 6. BONUS CHALLENGES

### Bonus 1: Auto-Tuning Implementation (+10%)
Implement relay feedback auto-tuning:
- Automatic Ku dan Tu measurement
- Self-tuning capability
- Convergence analysis

### Bonus 2: Cascade Control (+10%)
Implement dual-loop cascade control:
- Inner loop: current/torque control
- Outer loop: speed/position control
- Stability analysis

### Bonus 3: Adaptive Control (+15%)
Implement gain scheduling atau MRAC:
- Parameter adaptation based on operating conditions
- Performance improvement documentation

### Bonus 4: Real-Time Monitoring Dashboard (+5%)
Create Python GUI for:
- Real-time plotting
- Parameter adjustment
- Data logging
- Performance metrics display

---

## 7. REFERENSI

### Textbooks
1. Åström, K.J. & Murray, R.M. - "Feedback Systems: An Introduction for Scientists and Engineers"
2. Franklin, G.F. et al. - "Feedback Control of Dynamic Systems"
3. Ogata, K. - "Modern Control Engineering"

### Online Resources
1. PID Controller Design - Control Tutorials for MATLAB
2. Arduino PID Library Documentation
3. Ziegler-Nichols Tuning Method - Wikipedia

### Application Notes
1. Texas Instruments - "Digital Motor Control Application Guide"
2. Maxon Motor - "Motion Control Tutorial"
3. STMicroelectronics - "Motor Control Application Notes"

---

## 8. TEMPLATE CODE

### 8.1 PID Controller Class
```cpp
// pid_controller.h
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PIDController {
private:
    float Kp, Ki, Kd;           // PID gains
    float integral;             // Integral accumulator
    float prev_error;           // Previous error for derivative
    float output_min, output_max;  // Output limits
    float alpha;                // Derivative filter coefficient
    float filtered_derivative;  // Filtered derivative term
    unsigned long prev_time;    // Previous time for dt calculation
    bool anti_windup_enabled;
    
public:
    PIDController(float kp, float ki, float kd);
    
    float compute(float setpoint, float feedback);
    void setTunings(float kp, float ki, float kd);
    void setOutputLimits(float min, float max);
    void setDerivativeFilter(float filter_alpha);
    void reset();
    
    // Getters for debugging
    float getProportional() const;
    float getIntegral() const;
    float getDerivative() const;
};

#endif
```

### 8.2 Main Control Loop Template
```cpp
// main.cpp
#include <Arduino.h>
#include "pid_controller.h"
#include "encoder.h"
#include "motor_driver.h"

// Timing
const unsigned long CONTROL_PERIOD_US = 5000;  // 5ms = 200 Hz
unsigned long last_control_time = 0;

// Controller
PIDController position_pid(1.0, 0.1, 0.05);  // Tune these!

// Hardware
Encoder encoder(ENCODER_A_PIN, ENCODER_B_PIN);
MotorDriver motor(PWM_PIN, DIR_PIN);

// Setpoint
float setpoint = 0;

void setup() {
    Serial.begin(115200);
    
    // Configure PID
    position_pid.setOutputLimits(-255, 255);
    position_pid.setDerivativeFilter(0.1);
    
    // Initialize hardware
    encoder.begin();
    motor.begin();
}

void loop() {
    unsigned long now = micros();
    
    if (now - last_control_time >= CONTROL_PERIOD_US) {
        last_control_time = now;
        
        // Read feedback
        float position = encoder.getPositionDegrees();
        
        // Compute control
        float output = position_pid.compute(setpoint, position);
        
        // Apply output
        motor.setPWM(output);
        
        // Debug output
        Serial.print(setpoint);
        Serial.print(",");
        Serial.print(position);
        Serial.print(",");
        Serial.println(output);
    }
    
    // Check for new setpoint from serial
    if (Serial.available()) {
        setpoint = Serial.parseFloat();
    }
}
```

---

**Selamat mengerjakan project!**

*"The art of control engineering is in understanding the trade-offs and finding the right balance for your specific application."*
