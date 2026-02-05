# JOBSHEET PRAKTIKUM
## Modul 2: Safety & Commissioning Sistem

**Durasi:** 300 menit (5 jam)  
**Prasyarat:** Modul 1 (Arsitektur Sistem)

---

## TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa diharapkan mampu:
1. Merancang dan mengimplementasi E-Stop system yang reliable
2. Mengimplementasi limit switch dan homing procedure
3. Memahami dan menerapkan interlock logic
4. Melakukan fault detection dan handling
5. Melaksanakan commissioning prosedur yang sistematis
6. Membuat dokumentasi safety system

---

## PERALATAN & KOMPONEN

### Hardware
- [x] ESP32 DevKit / Arduino Mega 2560
- [x] E-Stop button (NC contact, mushroom head)
- [x] Limit switch (mechanical) - 2 buah
- [x] Safety relay module (optional)
- [x] LED indicator: Red, Yellow, Green
- [x] Buzzer/alarm
- [x] Motor DC dengan encoder (dari Modul 1)
- [x] Motor driver L298N
- [x] Push button (reset button)
- [x] Power supply 12V & 5V
- [x] Breadboard + kabel jumper

### Software
- [x] PlatformIO IDE
- [x] Serial Monitor
- [x] Git untuk version control

---

## KESELAMATAN KERJA

⚠️ **PERHATIAN KHUSUS:**
1. Pastikan E-Stop button berfungsi SEBELUM mengaktifkan motor
2. Jangan bypass safety circuit untuk testing
3. Selalu ada supervisor saat commissioning
4. Gunakan tegangan rendah (12V) untuk praktikum
5. Matikan power saat melakukan wiring

---

## PERCOBAAN 1: E-STOP IMPLEMENTATION
**Durasi:** 60 menit

### Tujuan
Mengimplementasi emergency stop system dengan hardware dan software integration.

### Skema Rangkaian

```
                    +5V
                     │
                     ├─────────────────┐
                     │                 │
                ┌────┴────┐       ┌────┴────┐
                │ E-STOP  │       │  RESET  │
                │   NC    │       │   NO    │
                └────┬────┘       └────┬────┘
                     │                 │
                     │ 10kΩ            │ 10kΩ
                     │ pull-down       │ pull-down
                     │                 │
              GPIO34 ◄─────────────────┘
              (E-STOP)          GPIO35 (RESET)
                     │                 │
                    GND               GND

Status LEDs:
GPIO25 ──► Green LED (NORMAL)
GPIO26 ──► Yellow LED (WARNING)
GPIO27 ──► Red LED (E-STOP/FAULT)
GPIO32 ──► Buzzer (ALARM)
```

### Kode Program

```cpp
// File: experiment1_estop.cpp

#include <Arduino.h>

// Pin definitions
#define E_STOP_PIN      34    // NC contact - HIGH when not pressed
#define RESET_PIN       35    // NO contact - HIGH when pressed
#define LED_GREEN       25    // Normal operation
#define LED_YELLOW      26    // Warning
#define LED_RED         27    // E-Stop active / Fault
#define BUZZER_PIN      32    // Alarm
#define MOTOR_EN_PIN    13    // Motor driver enable

// Safety states
enum class SafetyState {
    NORMAL,
    E_STOP_ACTIVE,
    FAULT,
    RECOVERY
};

// Global variables
SafetyState currentState = SafetyState::NORMAL;
unsigned long lastStateChange = 0;
bool motorEnabled = false;

// Function prototypes
void updateStateMachine();
void updateIndicators();
void disableMotor();
void enableMotor();
bool checkSafeConditions();

void setup() {
    Serial.begin(115200);
    Serial.println("=== E-STOP SYSTEM TEST ===");
    
    // Configure inputs
    pinMode(E_STOP_PIN, INPUT);
    pinMode(RESET_PIN, INPUT);
    
    // Configure outputs
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(MOTOR_EN_PIN, OUTPUT);
    
    // Initial state
    disableMotor();
    
    // Initial E-Stop check
    if (digitalRead(E_STOP_PIN) == LOW) {
        currentState = SafetyState::E_STOP_ACTIVE;
        Serial.println("WARNING: E-Stop is active at startup!");
    }
    
    Serial.println("System initialized. Press RESET to enable.");
}

void loop() {
    updateStateMachine();
    updateIndicators();
    
    // Debug output every 500ms
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 500) {
        lastPrint = millis();
        printStatus();
    }
    
    delay(10);  // 100Hz loop
}

void updateStateMachine() {
    bool eStopPressed = (digitalRead(E_STOP_PIN) == LOW);  // NC contact
    bool resetPressed = (digitalRead(RESET_PIN) == HIGH);  // NO contact
    
    switch (currentState) {
        case SafetyState::NORMAL:
            if (eStopPressed) {
                currentState = SafetyState::E_STOP_ACTIVE;
                disableMotor();
                Serial.println(">>> E-STOP ACTIVATED <<<");
                lastStateChange = millis();
            }
            break;
            
        case SafetyState::E_STOP_ACTIVE:
            // E-Stop must be released AND reset pressed
            if (!eStopPressed && resetPressed) {
                if (checkSafeConditions()) {
                    currentState = SafetyState::RECOVERY;
                    Serial.println("Entering recovery mode...");
                    lastStateChange = millis();
                } else {
                    Serial.println("Cannot reset: unsafe conditions detected");
                }
            }
            break;
            
        case SafetyState::RECOVERY:
            // Wait 2 seconds in recovery before enabling
            if (millis() - lastStateChange > 2000) {
                enableMotor();
                currentState = SafetyState::NORMAL;
                Serial.println("System restored to NORMAL");
                lastStateChange = millis();
            }
            // Check for E-Stop during recovery
            if (eStopPressed) {
                currentState = SafetyState::E_STOP_ACTIVE;
                disableMotor();
                Serial.println(">>> E-STOP during recovery <<<");
            }
            break;
            
        case SafetyState::FAULT:
            // Only manual intervention can clear fault
            if (resetPressed && !eStopPressed) {
                currentState = SafetyState::RECOVERY;
                Serial.println("Fault acknowledged, entering recovery...");
            }
            break;
    }
}

void updateIndicators() {
    switch (currentState) {
        case SafetyState::NORMAL:
            digitalWrite(LED_GREEN, HIGH);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_RED, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            break;
            
        case SafetyState::E_STOP_ACTIVE:
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_RED, HIGH);
            // Buzzer beep pattern
            digitalWrite(BUZZER_PIN, (millis() % 500) < 250);
            break;
            
        case SafetyState::RECOVERY:
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_YELLOW, (millis() % 300) < 150);  // Blink
            digitalWrite(LED_RED, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            break;
            
        case SafetyState::FAULT:
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_YELLOW, HIGH);
            digitalWrite(LED_RED, HIGH);
            // Fast beep
            digitalWrite(BUZZER_PIN, (millis() % 200) < 100);
            break;
    }
}

void disableMotor() {
    digitalWrite(MOTOR_EN_PIN, LOW);
    motorEnabled = false;
}

void enableMotor() {
    digitalWrite(MOTOR_EN_PIN, HIGH);
    motorEnabled = true;
}

bool checkSafeConditions() {
    // Add your safety checks here
    // For example: limit switches not triggered, temperature OK, etc.
    return true;
}

void printStatus() {
    Serial.print("State: ");
    switch (currentState) {
        case SafetyState::NORMAL: Serial.print("NORMAL"); break;
        case SafetyState::E_STOP_ACTIVE: Serial.print("E-STOP"); break;
        case SafetyState::RECOVERY: Serial.print("RECOVERY"); break;
        case SafetyState::FAULT: Serial.print("FAULT"); break;
    }
    Serial.print(" | Motor: ");
    Serial.print(motorEnabled ? "ENABLED" : "DISABLED");
    Serial.print(" | E-Stop: ");
    Serial.println(digitalRead(E_STOP_PIN) == LOW ? "PRESSED" : "RELEASED");
}
```

### Prosedur Percobaan

1. **Setup Hardware:**
   - Rangkai circuit sesuai skema
   - Pastikan E-Stop menggunakan NC contact
   - Verifikasi wiring sebelum power ON

2. **Test E-Stop Function:**
   - Power ON system
   - Tekan E-Stop → verify motor disabled, red LED ON
   - Release E-Stop → verify still disabled (requires reset)
   - Tekan Reset → verify recovery sequence

3. **Test Response Time:**
   - Gunakan oscilloscope atau timing code
   - Ukur waktu dari E-Stop press hingga motor disable
   - Target: < 50ms

### Tabel Data

| Test | E-Stop State | Reset State | Expected Result | Actual Result | Pass/Fail |
|------|-------------|-------------|-----------------|---------------|-----------|
| 1 | Released | Not Pressed | NORMAL, Green LED | | |
| 2 | Pressed | Not Pressed | E-STOP, Red LED | | |
| 3 | Released | Pressed | RECOVERY, Yellow blink | | |
| 4 | Released | Released (after reset) | NORMAL | | |
| 5 | Pressed during RECOVERY | - | E-STOP | | |

**Response Time Measurement:**
- E-Stop to Motor Disable: _______ ms
- Target: < 50 ms
- Pass / Fail: _______

---

## PERCOBAAN 2: LIMIT SWITCH & HOMING
**Durasi:** 75 menit

### Tujuan
Mengimplementasi homing procedure menggunakan limit switch untuk establish position reference.

### Skema Rangkaian

```
Limit Switch Wiring:
                    +5V
                     │
                ┌────┴────┐
                │  LIMIT  │
                │  SW (NC)│
                └────┬────┘
                     │
              GPIO36 ◄───┤ (dengan 10kΩ pull-down)
                     │
                    GND

Motor Setup (dari Modul 1):
ESP32          L298N         Motor
GPIO18 ────► IN1
GPIO19 ────► IN2
GPIO21 ────► ENA (PWM)
GPIO22 ────► Encoder A
GPIO23 ────► Encoder B
```

### Kode Program

```cpp
// File: experiment2_homing.cpp

#include <Arduino.h>

// Pin definitions
#define LIMIT_HOME_PIN  36    // Home limit switch
#define LIMIT_END_PIN   39    // End limit switch (optional)
#define MOTOR_IN1       18
#define MOTOR_IN2       19
#define MOTOR_PWM       21
#define ENCODER_A       22
#define ENCODER_B       23

// Homing parameters
#define HOME_SPEED_FAST     150   // PWM for fast approach
#define HOME_SPEED_SLOW     50    // PWM for slow approach
#define HOME_TIMEOUT        10000 // 10 seconds timeout

// Encoder variables
volatile long encoderCount = 0;

// Homing state machine
enum class HomingState {
    IDLE,
    APPROACH_FAST,
    BACK_OFF,
    APPROACH_SLOW,
    COMPLETE,
    ERROR
};

HomingState homingState = HomingState::IDLE;
unsigned long homingStartTime = 0;
bool homingComplete = false;

// Function prototypes
void encoderISR();
void setMotorSpeed(int speed);  // Positive = forward, negative = reverse
bool isHomeSwitchActive();
void updateHoming();
void startHoming();

void setup() {
    Serial.begin(115200);
    Serial.println("=== HOMING SYSTEM TEST ===");
    
    // Motor pins
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_PWM, OUTPUT);
    
    // Limit switch pins
    pinMode(LIMIT_HOME_PIN, INPUT);
    pinMode(LIMIT_END_PIN, INPUT);
    
    // Encoder pins with interrupt
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR, RISING);
    
    // Stop motor initially
    setMotorSpeed(0);
    
    Serial.println("Type 'H' to start homing procedure");
}

void loop() {
    // Handle serial commands
    if (Serial.available()) {
        char cmd = Serial.read();
        if (cmd == 'H' || cmd == 'h') {
            startHoming();
        }
    }
    
    // Update homing state machine
    updateHoming();
    
    // Print status
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 200) {
        lastPrint = millis();
        Serial.print("State: ");
        printHomingState();
        Serial.print(" | Encoder: ");
        Serial.print(encoderCount);
        Serial.print(" | Home SW: ");
        Serial.println(isHomeSwitchActive() ? "ACTIVE" : "inactive");
    }
    
    delay(10);
}

void IRAM_ATTR encoderISR() {
    if (digitalRead(ENCODER_B) == HIGH) {
        encoderCount++;
    } else {
        encoderCount--;
    }
}

void setMotorSpeed(int speed) {
    if (speed > 0) {
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        analogWrite(MOTOR_PWM, constrain(speed, 0, 255));
    } else if (speed < 0) {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
        analogWrite(MOTOR_PWM, constrain(-speed, 0, 255));
    } else {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
        analogWrite(MOTOR_PWM, 0);
    }
}

bool isHomeSwitchActive() {
    return digitalRead(LIMIT_HOME_PIN) == LOW;  // NC switch
}

void startHoming() {
    Serial.println(">>> Starting homing procedure <<<");
    homingState = HomingState::APPROACH_FAST;
    homingStartTime = millis();
    homingComplete = false;
}

void updateHoming() {
    // Timeout check
    if (homingState != HomingState::IDLE && 
        homingState != HomingState::COMPLETE &&
        homingState != HomingState::ERROR) {
        if (millis() - homingStartTime > HOME_TIMEOUT) {
            setMotorSpeed(0);
            homingState = HomingState::ERROR;
            Serial.println("ERROR: Homing timeout!");
            return;
        }
    }
    
    switch (homingState) {
        case HomingState::IDLE:
            // Waiting for command
            break;
            
        case HomingState::APPROACH_FAST:
            setMotorSpeed(-HOME_SPEED_FAST);  // Move toward home
            if (isHomeSwitchActive()) {
                setMotorSpeed(0);
                delay(100);  // Debounce
                homingState = HomingState::BACK_OFF;
                Serial.println("Home switch triggered - backing off");
            }
            break;
            
        case HomingState::BACK_OFF:
            setMotorSpeed(HOME_SPEED_SLOW);  // Move away from home
            if (!isHomeSwitchActive()) {
                setMotorSpeed(0);
                delay(100);  // Debounce
                homingState = HomingState::APPROACH_SLOW;
                Serial.println("Switch released - slow approach");
            }
            break;
            
        case HomingState::APPROACH_SLOW:
            setMotorSpeed(-HOME_SPEED_SLOW);  // Slow approach to home
            if (isHomeSwitchActive()) {
                setMotorSpeed(0);
                encoderCount = 0;  // Reset encoder
                homingState = HomingState::COMPLETE;
                homingComplete = true;
                Serial.println(">>> HOMING COMPLETE <<<");
                Serial.println("Encoder position reset to 0");
            }
            break;
            
        case HomingState::COMPLETE:
            // Homing done
            break;
            
        case HomingState::ERROR:
            // Error state - requires restart
            break;
    }
}

void printHomingState() {
    switch (homingState) {
        case HomingState::IDLE: Serial.print("IDLE"); break;
        case HomingState::APPROACH_FAST: Serial.print("FAST_APPROACH"); break;
        case HomingState::BACK_OFF: Serial.print("BACK_OFF"); break;
        case HomingState::APPROACH_SLOW: Serial.print("SLOW_APPROACH"); break;
        case HomingState::COMPLETE: Serial.print("COMPLETE"); break;
        case HomingState::ERROR: Serial.print("ERROR"); break;
    }
}
```

### Prosedur Percobaan

1. **Setup:**
   - Pasang limit switch di posisi home
   - Pastikan motor dapat bergerak bebas ke kedua arah
   - Verifikasi encoder berfungsi

2. **Test Limit Switch:**
   - Manual trigger limit switch
   - Verify reading di Serial Monitor

3. **Execute Homing:**
   - Posisikan motor di tengah range
   - Kirim command 'H' via Serial
   - Observe homing sequence

4. **Measure Repeatability:**
   - Ulangi homing 5 kali
   - Catat posisi encoder setelah homing
   - Hitung repeatability

### Tabel Data

| Test # | Start Position | Homing Time (ms) | Final Encoder | Deviation |
|--------|---------------|------------------|---------------|-----------|
| 1 | | | 0 (reference) | - |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |

**Repeatability:** _______ encoder counts (± _______ degrees)

---

## PERCOBAAN 3: INTERLOCK LOGIC
**Durasi:** 60 menit

### Tujuan
Mengimplementasi interlock system yang mencegah operasi berbahaya.

### Kode Program

```cpp
// File: experiment3_interlock.cpp

#include <Arduino.h>

// Pin definitions (kombinasi percobaan sebelumnya)
#define E_STOP_PIN      34
#define DOOR_SENSOR     35    // Simulated dengan push button
#define HOME_COMPLETE   36    // Virtual, dari homing routine
#define MOTOR_EN_PIN    13
#define LED_GREEN       25
#define LED_YELLOW      26
#define LED_RED         27

// Interlock conditions
struct InterlockStatus {
    bool eStopReleased;
    bool doorClosed;
    bool homingComplete;
    bool temperatureOK;
    bool communicationOK;
};

InterlockStatus interlock;
bool operationAllowed = false;

// Simulated conditions
bool simulatedHomingComplete = false;
bool simulatedTempOK = true;
bool simulatedCommOK = true;

void setup() {
    Serial.begin(115200);
    Serial.println("=== INTERLOCK SYSTEM TEST ===");
    
    pinMode(E_STOP_PIN, INPUT);
    pinMode(DOOR_SENSOR, INPUT);
    pinMode(MOTOR_EN_PIN, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    
    digitalWrite(MOTOR_EN_PIN, LOW);
    
    Serial.println("Commands:");
    Serial.println("  H - Toggle homing complete");
    Serial.println("  T - Toggle temperature OK");
    Serial.println("  C - Toggle communication OK");
    Serial.println("  M - Attempt motor operation");
}

void loop() {
    // Update interlock conditions
    updateInterlockStatus();
    
    // Check overall permission
    operationAllowed = checkAllInterlocks();
    
    // Update indicators
    updateIndicators();
    
    // Handle serial commands
    handleCommands();
    
    // Print status periodically
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 1000) {
        lastPrint = millis();
        printInterlockStatus();
    }
    
    delay(10);
}

void updateInterlockStatus() {
    interlock.eStopReleased = (digitalRead(E_STOP_PIN) == HIGH);
    interlock.doorClosed = (digitalRead(DOOR_SENSOR) == HIGH);
    interlock.homingComplete = simulatedHomingComplete;
    interlock.temperatureOK = simulatedTempOK;
    interlock.communicationOK = simulatedCommOK;
}

bool checkAllInterlocks() {
    return interlock.eStopReleased &&
           interlock.doorClosed &&
           interlock.homingComplete &&
           interlock.temperatureOK &&
           interlock.communicationOK;
}

String getBlockingReason() {
    if (!interlock.eStopReleased) return "E-Stop active";
    if (!interlock.doorClosed) return "Door open";
    if (!interlock.homingComplete) return "Homing required";
    if (!interlock.temperatureOK) return "Temperature too high";
    if (!interlock.communicationOK) return "Communication error";
    return "OK";
}

void updateIndicators() {
    if (operationAllowed) {
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, LOW);
    } else if (interlock.eStopReleased) {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, HIGH);  // Warning - some interlock failed
        digitalWrite(LED_RED, LOW);
    } else {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, HIGH);  // E-Stop active
    }
}

void handleCommands() {
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'H':
            case 'h':
                simulatedHomingComplete = !simulatedHomingComplete;
                Serial.print("Homing complete: ");
                Serial.println(simulatedHomingComplete ? "YES" : "NO");
                break;
                
            case 'T':
            case 't':
                simulatedTempOK = !simulatedTempOK;
                Serial.print("Temperature OK: ");
                Serial.println(simulatedTempOK ? "YES" : "NO");
                break;
                
            case 'C':
            case 'c':
                simulatedCommOK = !simulatedCommOK;
                Serial.print("Communication OK: ");
                Serial.println(simulatedCommOK ? "YES" : "NO");
                break;
                
            case 'M':
            case 'm':
                attemptMotorOperation();
                break;
        }
    }
}

void attemptMotorOperation() {
    Serial.println("\n--- Attempting motor operation ---");
    
    if (checkAllInterlocks()) {
        Serial.println("✓ All interlocks OK - Motor enabled");
        digitalWrite(MOTOR_EN_PIN, HIGH);
        delay(1000);
        digitalWrite(MOTOR_EN_PIN, LOW);
        Serial.println("Motor disabled after 1 second");
    } else {
        Serial.print("✗ Operation blocked: ");
        Serial.println(getBlockingReason());
    }
}

void printInterlockStatus() {
    Serial.println("\n=== INTERLOCK STATUS ===");
    Serial.print("E-Stop Released:   "); Serial.println(interlock.eStopReleased ? "✓" : "✗");
    Serial.print("Door Closed:       "); Serial.println(interlock.doorClosed ? "✓" : "✗");
    Serial.print("Homing Complete:   "); Serial.println(interlock.homingComplete ? "✓" : "✗");
    Serial.print("Temperature OK:    "); Serial.println(interlock.temperatureOK ? "✓" : "✗");
    Serial.print("Communication OK:  "); Serial.println(interlock.communicationOK ? "✓" : "✗");
    Serial.println("------------------------");
    Serial.print("Operation Allowed: "); Serial.println(operationAllowed ? "YES" : "NO");
    if (!operationAllowed) {
        Serial.print("Blocking reason:   "); Serial.println(getBlockingReason());
    }
}
```

### Tabel Data Interlock Matrix

| Condition | E-Stop | Door | Homing | Temp | Comm | Operation Allowed? | Pass/Fail |
|-----------|--------|------|--------|------|------|-------------------|-----------|
| All OK | ✓ | ✓ | ✓ | ✓ | ✓ | YES | |
| E-Stop Active | ✗ | ✓ | ✓ | ✓ | ✓ | NO | |
| Door Open | ✓ | ✗ | ✓ | ✓ | ✓ | NO | |
| No Homing | ✓ | ✓ | ✗ | ✓ | ✓ | NO | |
| Temp High | ✓ | ✓ | ✓ | ✗ | ✓ | NO | |
| Comm Error | ✓ | ✓ | ✓ | ✓ | ✗ | NO | |

---

## PERCOBAAN 4: COMMISSIONING PROCEDURE
**Durasi:** 75 menit

### Tujuan
Melaksanakan commissioning prosedur sistematis dan mendokumentasikan hasilnya.

### Checklist Commissioning

#### Phase 1: Pre-Power Checks

| # | Item | Method | Criteria | Result | Sign |
|---|------|--------|----------|--------|------|
| 1.1 | Visual inspection | Eye check | No visible damage | | |
| 1.2 | Wiring verification | Multimeter | Continuity OK | | |
| 1.3 | Ground connection | Multimeter | < 1Ω | | |
| 1.4 | Polarity check | Multimeter | Correct | | |
| 1.5 | Mechanical freedom | Manual move | Smooth motion | | |

#### Phase 2: Power-Up Sequence

| # | Item | Method | Criteria | Result | Sign |
|---|------|--------|----------|--------|------|
| 2.1 | Control power ON | Power supply | 5V ± 5% | | |
| 2.2 | MCU boot | Serial monitor | "Ready" message | | |
| 2.3 | I/O test | Toggle each | Correct response | | |
| 2.4 | Motor power ON | Power supply | 12V ± 5% | | |
| 2.5 | Motor direction | Manual jog | Correct direction | | |

#### Phase 3: Safety Function Test

| # | Item | Method | Criteria | Result | Sign |
|---|------|--------|----------|--------|------|
| 3.1 | E-Stop test | Press button | Motor stops < 50ms | | |
| 3.2 | E-Stop reset | Release + Reset | Normal operation | | |
| 3.3 | Limit switch home | Trigger manual | Motor stops | | |
| 3.4 | Limit switch end | Trigger manual | Motor stops | | |
| 3.5 | Interlock test | Open door | Motor blocked | | |

#### Phase 4: Homing & Positioning

| # | Item | Method | Criteria | Result | Sign |
|---|------|--------|----------|--------|------|
| 4.1 | Homing execution | Command 'H' | Complete < 10s | | |
| 4.2 | Homing repeatability | 5 repetitions | < 1° deviation | | |
| 4.3 | Position accuracy | Move to target | < 2° error | | |
| 4.4 | Speed accuracy | Measure RPM | < 5% error | | |

#### Phase 5: Sign-Off

```
Commissioning completed by: _______________________

Date: _______________________

Supervisor: _______________________

Notes:
_________________________________________________
_________________________________________________
_________________________________________________
```

---

## TUGAS & LAPORAN

### Tugas Individu
1. Buat flowchart lengkap untuk recovery procedure dari setiap fault type
2. Desain interlock matrix untuk sistem dengan 3 motor dan 2 gripper
3. Buat risk assessment sederhana untuk praktikum ini

### Laporan Kelompok
**Format:** PDF, 10-15 halaman

**Isi:**
1. **Pendahuluan** (1 halaman)
   - Tujuan praktikum
   - Latar belakang safety system

2. **Metodologi** (2 halaman)
   - Hardware setup
   - Software architecture
   - Test procedures

3. **Hasil & Analisis** (5-7 halaman)
   - Data percobaan 1-4 dengan analisis
   - Screenshot Serial Monitor
   - Timing measurements
   - Commissioning checklist lengkap

4. **Pembahasan** (2 halaman)
   - Perbandingan dengan standar industri
   - Lessons learned
   - Improvement suggestions

5. **Kesimpulan** (1 halaman)
   - Summary hasil
   - Recommendations

**Deadline:** 1 minggu setelah praktikum

---

## KRITERIA PENILAIAN

| Aspek | Bobot | Kriteria |
|-------|-------|----------|
| E-Stop Implementation | 25% | Response time < 50ms, reliable operation |
| Homing Procedure | 25% | Repeatability < 1°, proper sequence |
| Interlock Logic | 20% | All conditions checked, correct blocking |
| Commissioning | 20% | Complete checklist, proper documentation |
| Laporan | 10% | Clear, complete, professional |

---

**Selamat mengerjakan!**

*"Safety first, because accidents last."*
