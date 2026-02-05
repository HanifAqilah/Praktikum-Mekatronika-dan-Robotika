# MATERI PEMBELAJARAN
## Modul 2: Safety & Commissioning Sistem

**Tujuan Pembelajaran:**
- Memahami prinsip keselamatan sistem robotika dan mekatronika
- Mampu merancang dan mengimplementasi E-Stop system
- Memahami interlock logic dan fault state management
- Mampu melakukan commissioning prosedur yang sistematis

---

## 1. PENDAHULUAN SAFETY SYSTEM

### 1.1 Mengapa Safety Critical?

Sistem robotika dan mekatronika melibatkan:
- **Energi mekanik tinggi** (motor, aktuator, bagian bergerak)
- **Energi listrik** (tegangan tinggi, arus besar)
- **Interaksi dengan manusia** (collaborative robots, operator)
- **Lingkungan industri** (pabrik, warehouse, outdoor)

**Statistik Kecelakaan Industri:**
- 61% kecelakaan robot terjadi selama maintenance/commissioning
- 26% terjadi saat operasi normal
- 13% terjadi saat programming/teaching

### 1.2 Hierarki Safety Control

```
┌─────────────────────────────────────┐
│     LEVEL 4: Emergency Stop         │  ← Hardware interlock
│     (E-Stop, Safety PLCs)           │
├─────────────────────────────────────┤
│     LEVEL 3: Safety Functions       │  ← Speed/force limiting
│     (Safe torque off, safe speed)   │
├─────────────────────────────────────┤
│     LEVEL 2: Protective Devices     │  ← Light curtain, safety mat
│     (Presence sensing)              │
├─────────────────────────────────────┤
│     LEVEL 1: Mechanical Guards      │  ← Physical barriers
│     (Fences, enclosures)            │
├─────────────────────────────────────┤
│     LEVEL 0: Inherent Safety        │  ← Design for safety
│     (Low voltage, limited force)    │
└─────────────────────────────────────┘
```

---

## 2. EMERGENCY STOP (E-STOP) SYSTEM

### 2.1 E-Stop Requirements (IEC 60204-1)

**Karakteristik E-Stop:**
- **Category 0:** Uncontrolled stop - immediate power removal
- **Category 1:** Controlled stop - decelerate then power off
- **Category 2:** Controlled stop - power maintained for braking

**Wiring E-Stop:**
```
              +24V
               │
               ▼
         ┌─────────┐
         │  E-Stop │ (Normally Closed)
         │  Button │
         └────┬────┘
              │
              ├──────────────────┬───────────────────┐
              │                  │                   │
              ▼                  ▼                   ▼
         ┌─────────┐        ┌─────────┐        ┌─────────┐
         │ Motor   │        │ Safety  │        │ MCU     │
         │ Contactor│       │ Relay   │        │ Input   │
         └─────────┘        └─────────┘        └─────────┘
              │                  │                   │
              ▼                  ▼                   ▼
           MOTOR OFF        ALL POWER OFF       SOFTWARE
                                                 STOP
```

### 2.2 E-Stop Implementation

**Hardware Requirements:**
1. **E-Stop Button:** Red mushroom head, yellow background, NC contact
2. **Safety Relay:** Dual-channel monitoring, force-guided contacts
3. **Contactor:** For power interruption, rated for load
4. **Reset Button:** Separate from E-Stop, requires manual action

**Software Implementation:**
```cpp
// E-Stop state machine
enum class SafetyState {
    NORMAL,
    E_STOP_ACTIVE,
    FAULT,
    RECOVERY
};

class SafetySystem {
private:
    SafetyState state = SafetyState::NORMAL;
    bool e_stop_pressed = false;
    bool reset_requested = false;
    
public:
    void update() {
        // Read E-Stop input (active LOW due to NC contact)
        e_stop_pressed = !digitalRead(E_STOP_PIN);
        
        switch (state) {
            case SafetyState::NORMAL:
                if (e_stop_pressed) {
                    executeEmergencyStop();
                    state = SafetyState::E_STOP_ACTIVE;
                }
                break;
                
            case SafetyState::E_STOP_ACTIVE:
                if (!e_stop_pressed && reset_requested) {
                    if (checkSafeConditions()) {
                        state = SafetyState::RECOVERY;
                    }
                }
                break;
                
            case SafetyState::RECOVERY:
                // Controlled power-up sequence
                if (recoveryComplete()) {
                    state = SafetyState::NORMAL;
                }
                break;
                
            case SafetyState::FAULT:
                // Requires manual intervention
                break;
        }
    }
    
    void executeEmergencyStop() {
        // 1. Disable all motor drives
        disableAllMotors();
        
        // 2. Apply brakes (if available)
        engageBrakes();
        
        // 3. Log event
        logSafetyEvent("E-STOP ACTIVATED");
        
        // 4. Notify operator
        activateAlarm();
    }
};
```

---

## 3. LIMIT SWITCH & HOMING

### 3.1 Limit Switch Types

| Type | Pros | Cons | Application |
|------|------|------|-------------|
| **Mechanical** | Reliable, no power needed | Wear, contact bounce | End-of-travel |
| **Inductive** | No contact, long life | Metal only, distance limited | Metal detection |
| **Capacitive** | Any material | Sensitive to environment | Non-metal detection |
| **Optical** | High speed, precision | Dust sensitive | Encoder index |

### 3.2 Homing Procedure

**Tujuan Homing:**
- Establish absolute position reference
- Required after power-up atau setelah fault
- Calibrate encoder/position sensor

**Homing Algorithm:**
```
┌─────────────────────────────────────────────────────┐
│                  HOMING SEQUENCE                    │
└─────────────────────────────────────────────────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ 1. Move toward home   │
              │    switch (slow)      │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ 2. Wait for switch    │
              │    activation         │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ 3. Reverse direction  │
              │    (very slow)        │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ 4. Wait for switch    │
              │    deactivation       │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ 5. Move to encoder    │
              │    index (if avail)   │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ 6. Set position = 0   │
              │    (or home offset)   │
              └───────────┴───────────┘
```

**Implementation:**
```cpp
enum class HomingState {
    IDLE,
    APPROACH_FAST,
    BACK_OFF,
    APPROACH_SLOW,
    FIND_INDEX,
    COMPLETE,
    ERROR
};

class HomingController {
private:
    HomingState state = HomingState::IDLE;
    float home_speed_fast = 50.0;   // RPM
    float home_speed_slow = 5.0;    // RPM
    
public:
    bool execute() {
        switch (state) {
            case HomingState::IDLE:
                state = HomingState::APPROACH_FAST;
                motor.setVelocity(-home_speed_fast);  // Toward home
                break;
                
            case HomingState::APPROACH_FAST:
                if (limitSwitch.isTriggered()) {
                    motor.stop();
                    state = HomingState::BACK_OFF;
                    motor.setVelocity(home_speed_slow);  // Away from home
                }
                break;
                
            case HomingState::BACK_OFF:
                if (!limitSwitch.isTriggered()) {
                    state = HomingState::APPROACH_SLOW;
                    motor.setVelocity(-home_speed_slow);  // Toward home
                }
                break;
                
            case HomingState::APPROACH_SLOW:
                if (limitSwitch.isTriggered()) {
                    motor.stop();
                    encoder.reset();
                    state = HomingState::COMPLETE;
                    return true;  // Homing successful
                }
                break;
                
            case HomingState::ERROR:
                return false;
        }
        
        // Timeout check
        if (millis() - start_time > HOMING_TIMEOUT) {
            motor.stop();
            state = HomingState::ERROR;
            return false;
        }
        
        return false;  // Still in progress
    }
};
```

---

## 4. INTERLOCK LOGIC

### 4.1 Definisi Interlock

**Interlock** adalah mekanisme yang mencegah operasi berbahaya dengan memastikan kondisi tertentu terpenuhi sebelum aksi diizinkan.

**Contoh Interlock:**
- Pintu harus tertutup sebelum motor bisa jalan
- Air pressure harus cukup sebelum gripper aktif
- Homing harus complete sebelum gerakan otomatis

### 4.2 Interlock Implementation

```cpp
// Interlock conditions
struct InterlockConditions {
    bool door_closed;
    bool air_pressure_ok;
    bool homing_complete;
    bool e_stop_released;
    bool communication_ok;
    bool temperature_ok;
};

class InterlockManager {
private:
    InterlockConditions conditions;
    
public:
    bool canOperateMotor(int motor_id) {
        // All conditions must be met
        return conditions.door_closed &&
               conditions.e_stop_released &&
               conditions.homing_complete &&
               conditions.communication_ok;
    }
    
    bool canActivateGripper() {
        return conditions.air_pressure_ok &&
               conditions.e_stop_released;
    }
    
    bool canStartAutomaticMode() {
        return canOperateMotor(0) &&
               canActivateGripper() &&
               conditions.temperature_ok;
    }
    
    void updateConditions() {
        conditions.door_closed = digitalRead(DOOR_SENSOR) == HIGH;
        conditions.air_pressure_ok = analogRead(PRESSURE_SENSOR) > PRESSURE_MIN;
        conditions.e_stop_released = digitalRead(E_STOP_PIN) == HIGH;
        conditions.temperature_ok = readTemperature() < TEMP_MAX;
        // etc.
    }
    
    String getBlockingReason() {
        if (!conditions.door_closed) return "Door open";
        if (!conditions.air_pressure_ok) return "Low air pressure";
        if (!conditions.e_stop_released) return "E-Stop active";
        if (!conditions.homing_complete) return "Homing required";
        if (!conditions.temperature_ok) return "Temperature too high";
        return "OK";
    }
};
```

---

## 5. FAULT STATE MANAGEMENT

### 5.1 Fault Categories

| Category | Severity | Response | Recovery |
|----------|----------|----------|----------|
| **Warning** | Low | Log, continue | Automatic |
| **Fault** | Medium | Safe stop, notify | Manual reset |
| **Critical** | High | E-Stop, alarm | Manual intervention |
| **Fatal** | Highest | Power off | Service required |

### 5.2 Fault Detection

**Common Faults to Monitor:**
1. **Motor faults:** Overcurrent, overtemperature, stall
2. **Sensor faults:** Out of range, disconnected, noisy
3. **Communication faults:** Timeout, checksum error
4. **Mechanical faults:** Collision, jam, slip
5. **Power faults:** Under/overvoltage, phase loss

**Fault Detection Code:**
```cpp
enum class FaultType {
    NONE,
    MOTOR_OVERCURRENT,
    MOTOR_OVERTEMP,
    MOTOR_STALL,
    SENSOR_DISCONNECTED,
    SENSOR_OUT_OF_RANGE,
    COMM_TIMEOUT,
    COLLISION_DETECTED,
    POWER_UNDERVOLTAGE,
    POWER_OVERVOLTAGE
};

class FaultManager {
private:
    FaultType active_fault = FaultType::NONE;
    uint32_t fault_timestamp = 0;
    
public:
    void checkMotorFaults(MotorDriver& motor) {
        // Overcurrent check
        if (motor.getCurrent() > CURRENT_LIMIT) {
            raiseFault(FaultType::MOTOR_OVERCURRENT);
        }
        
        // Stall detection
        if (motor.getVelocity() < MIN_VELOCITY && 
            motor.getPWM() > STALL_PWM_THRESHOLD) {
            raiseFault(FaultType::MOTOR_STALL);
        }
        
        // Temperature check
        if (motor.getTemperature() > TEMP_LIMIT) {
            raiseFault(FaultType::MOTOR_OVERTEMP);
        }
    }
    
    void checkSensorFaults(Encoder& encoder) {
        // Disconnection check
        if (!encoder.isConnected()) {
            raiseFault(FaultType::SENSOR_DISCONNECTED);
        }
        
        // Range check
        if (encoder.getPosition() < POS_MIN || 
            encoder.getPosition() > POS_MAX) {
            raiseFault(FaultType::SENSOR_OUT_OF_RANGE);
        }
    }
    
    void raiseFault(FaultType fault) {
        active_fault = fault;
        fault_timestamp = millis();
        
        // Execute fault response
        switch (getFaultSeverity(fault)) {
            case Severity::WARNING:
                logWarning(fault);
                break;
            case Severity::FAULT:
                safeStop();
                notifyOperator(fault);
                break;
            case Severity::CRITICAL:
                emergencyStop();
                activateAlarm();
                break;
        }
    }
    
    bool clearFault() {
        if (checkFaultCleared()) {
            active_fault = FaultType::NONE;
            return true;
        }
        return false;
    }
};
```

---

## 6. COMMISSIONING PROCEDURE

### 6.1 Commissioning Phases

```
┌─────────────────────────────────────────────────────┐
│              COMMISSIONING WORKFLOW                 │
└─────────────────────────────────────────────────────┘

Phase 1: Pre-Power Checks
├── Visual inspection
├── Wiring verification
├── Mechanical checks
└── Ground continuity

Phase 2: Power-Up Sequence
├── Control power ON
├── Verify inputs/outputs
├── Motor power ON (no load)
└── Check rotation direction

Phase 3: Basic Function Test
├── E-Stop test
├── Limit switch test
├── Homing procedure
└── Manual jogging

Phase 4: Performance Tuning
├── PID tuning
├── Speed/acceleration limits
├── Position calibration
└── Load testing

Phase 5: Full System Test
├── Automatic cycle test
├── Edge case testing
├── Long-run test
└── Safety function verification

Phase 6: Documentation
├── Test records
├── Parameter backup
├── User training
└── Sign-off
```

### 6.2 Commissioning Checklist

```cpp
// commissioning_checklist.h
struct CommissioningChecklist {
    // Phase 1: Pre-Power
    struct PrePower {
        bool visual_inspection_ok;
        bool wiring_verified;
        bool mechanical_check_ok;
        bool grounding_verified;
        String notes;
    } pre_power;
    
    // Phase 2: Power-Up
    struct PowerUp {
        bool control_power_ok;
        bool io_verification_ok;
        bool motor_power_ok;
        bool rotation_direction_ok;
        String notes;
    } power_up;
    
    // Phase 3: Basic Function
    struct BasicFunction {
        bool e_stop_tested;
        bool limit_switches_tested;
        bool homing_ok;
        bool manual_jog_ok;
        String notes;
    } basic_function;
    
    // Phase 4: Performance
    struct Performance {
        bool pid_tuned;
        bool speed_limits_set;
        bool position_calibrated;
        bool load_tested;
        float max_speed_achieved;
        float position_accuracy;
        String notes;
    } performance;
    
    // Phase 5: Full System
    struct FullSystem {
        bool auto_cycle_ok;
        bool edge_cases_ok;
        bool long_run_ok;
        int long_run_cycles;
        bool safety_verified;
        String notes;
    } full_system;
    
    // Sign-off
    String commissioned_by;
    String date;
    String customer_signature;
};
```

---

## 7. RECOVERY PROCEDURE

### 7.1 Recovery dari E-Stop

**Standard Recovery Sequence:**
1. Identify dan remove cause of E-Stop
2. Release E-Stop button (twist atau pull)
3. Visual inspection of work area
4. Press Reset button
5. System performs self-check
6. Execute homing procedure
7. Resume operation

### 7.2 Recovery dari Fault

```cpp
class RecoveryManager {
public:
    bool attemptRecovery(FaultType fault) {
        switch (fault) {
            case FaultType::MOTOR_OVERCURRENT:
                return recoverFromOvercurrent();
                
            case FaultType::MOTOR_STALL:
                return recoverFromStall();
                
            case FaultType::SENSOR_DISCONNECTED:
                return recoverFromSensorFault();
                
            case FaultType::COLLISION_DETECTED:
                return recoverFromCollision();
                
            default:
                return false;  // Manual intervention required
        }
    }
    
private:
    bool recoverFromOvercurrent() {
        // 1. Wait for cooldown
        delay(5000);
        
        // 2. Reset motor driver
        motor.reset();
        
        // 3. Reduce speed/acceleration limits temporarily
        motion.setSpeedLimit(motion.getSpeedLimit() * 0.8);
        
        // 4. Test with reduced parameters
        return motor.selfTest();
    }
    
    bool recoverFromStall() {
        // 1. Release motor
        motor.disable();
        delay(1000);
        
        // 2. Manual intervention may be needed
        // (clear obstruction)
        
        // 3. Re-enable and home
        motor.enable();
        return homing.execute();
    }
    
    bool recoverFromCollision() {
        // 1. Check for damage
        // 2. Clear obstruction
        // 3. Manual movement to safe position
        // 4. Re-home
        
        Serial.println("COLLISION RECOVERY:");
        Serial.println("1. Clear any obstructions");
        Serial.println("2. Manually move to safe position");
        Serial.println("3. Press Reset when ready");
        
        waitForReset();
        return homing.execute();
    }
};
```

---

## 8. BEST PRACTICES

### 8.1 Safety Design Principles

1. **Fail-Safe Design**
   - System harus aman saat failure
   - NC contacts untuk E-Stop
   - Spring-return untuk brake

2. **Redundancy**
   - Dual-channel safety circuits
   - Multiple sensors untuk critical functions
   - Backup power untuk safety systems

3. **Separation**
   - Safety circuit terpisah dari control circuit
   - Safety PLC terpisah dari main PLC
   - Hardwired E-Stop (tidak hanya software)

4. **Testing**
   - Regular safety function testing
   - Documented test procedures
   - Test records maintained

### 8.2 Documentation Requirements

**Minimum Documentation:**
- Safety circuit diagram
- Risk assessment
- Interlock matrix
- Fault response table
- Recovery procedures
- Commissioning checklist
- Maintenance schedule
- Training records

---

## 9. STANDAR & REGULASI

### 9.1 Relevant Standards

| Standard | Description |
|----------|-------------|
| **IEC 60204-1** | Safety of machinery - Electrical equipment |
| **IEC 62443** | Industrial cybersecurity |
| **ISO 13849** | Safety-related parts of control systems |
| **ISO 10218** | Industrial robot safety |
| **ISO/TS 15066** | Collaborative robot safety |

### 9.2 Safety Integrity Levels (SIL)

| SIL | PFD | Risk Level | Typical Application |
|-----|-----|------------|---------------------|
| SIL 1 | 10^-1 to 10^-2 | Low | Simple interlocks |
| SIL 2 | 10^-2 to 10^-3 | Medium | Standard E-Stop |
| SIL 3 | 10^-3 to 10^-4 | High | Critical safety functions |
| SIL 4 | 10^-4 to 10^-5 | Very High | Nuclear, aerospace |

---

## REFERENSI

1. IEC 60204-1:2016 - Safety of machinery
2. ISO 13849-1:2015 - Safety-related parts of control systems
3. Rockwell Automation - "Safety Function Design Guide"
4. Pilz - "Safety Compendium"
5. SICK - "Safe Robotics Guide"

---

*"Safety is not a priority, it's a prerequisite."*
