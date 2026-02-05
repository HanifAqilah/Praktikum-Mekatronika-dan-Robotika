# JOBSHEET PRAKTIKUM
## Modul 3: Power & EMC Praktis untuk Robot/IIoT

**Durasi:** 300 menit (5 jam)  
**Prasyarat:** Modul 1 (Arsitektur Sistem), Modul 2 (Safety)

---

## TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa diharapkan mampu:
1. Mendesain power distribution system untuk robotika
2. Mengimplementasi teknik filtering untuk noise reduction
3. Melakukan pengukuran noise dengan oscilloscope
4. Menerapkan proper grounding dan cable routing
5. Menganalisis pengaruh noise terhadap sistem kontrol

---

## PERALATAN & KOMPONEN

### Hardware
- [x] ESP32 DevKit
- [x] Power supply 24V/2A (SMPS)
- [x] DC-DC converter 24V→12V (LM2596)
- [x] DC-DC converter 24V→5V atau LDO 5V (AMS1117)
- [x] Motor DC 12V dengan encoder
- [x] Motor driver L298N
- [x] Analog sensor (potensiometer atau LDR)
- [x] Capacitors: 100µF, 10µF, 100nF, 10nF (various)
- [x] Ferrite beads (berbagai ukuran)
- [x] Resistors: 100Ω, 1kΩ, 10kΩ
- [x] Oscilloscope (2 channel minimum)
- [x] Multimeter
- [x] Breadboard + kabel jumper
- [x] Shielded cable (untuk comparison)

### Software
- [x] PlatformIO IDE
- [x] Serial Plotter
- [x] Python dengan matplotlib (untuk analisis)

---

## KESELAMATAN KERJA

⚠️ **PERHATIAN:**
1. Hati-hati dengan tegangan 24V DC
2. Matikan power saat melakukan wiring
3. Jangan short-circuit power supply
4. Gunakan kabel dengan rating yang sesuai
5. Perhatikan polaritas saat memasang komponen

---

## PERCOBAAN 1: POWER TREE DESIGN & MEASUREMENT
**Durasi:** 60 menit

### Tujuan
Mendesain dan mengimplementasi power distribution dengan multiple voltage levels.

### Skema Rangkaian

```
                    24V Input (from SMPS)
                           │
                           │
              ┌────────────┼────────────┐
              │            │            │
         ┌────┴────┐  ┌────┴────┐  ┌────┴────┐
         │ LM2596  │  │ LM2596  │  │ Direct  │
         │ 24V→12V │  │ 24V→5V  │  │   24V   │
         └────┬────┘  └────┬────┘  └────┬────┘
              │            │            │
           12V DC       5V DC        24V DC
              │            │            │
         ┌────┴────┐  ┌────┴────┐  ┌────┴────┐
         │  Motor  │  │  ESP32  │  │  Relay  │
         │ Driver  │  │         │  │ Module  │
         └─────────┘  └────┬────┘  └─────────┘
                           │
                      3.3V (internal)
                           │
                      ┌────┴────┐
                      │ Sensors │
                      └─────────┘
```

### Kode Program

```cpp
// File: experiment1_power_monitor.cpp

#include <Arduino.h>

// ADC pins for voltage monitoring (dengan voltage divider)
#define V24_SENSE_PIN   34    // 24V rail (divide by 10: 24V→2.4V)
#define V12_SENSE_PIN   35    // 12V rail (divide by 5: 12V→2.4V)
#define V5_SENSE_PIN    36    // 5V rail (direct atau divide by 2)
#define V3V3_SENSE_PIN  39    // 3.3V rail (direct)

// Voltage divider ratios
#define V24_RATIO   10.0    // R1=90k, R2=10k
#define V12_RATIO   5.0     // R1=40k, R2=10k
#define V5_RATIO    2.0     // R1=10k, R2=10k
#define V3V3_RATIO  1.0     // Direct connection

// ADC calibration
#define ADC_MAX     4095.0
#define ADC_VREF    3.3

float readVoltage(int pin, float ratio) {
    int raw = analogRead(pin);
    float voltage = (raw / ADC_MAX) * ADC_VREF * ratio;
    return voltage;
}

void setup() {
    Serial.begin(115200);
    Serial.println("=== POWER MONITORING ===");
    
    // Configure ADC
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
}

void loop() {
    float v24 = readVoltage(V24_SENSE_PIN, V24_RATIO);
    float v12 = readVoltage(V12_SENSE_PIN, V12_RATIO);
    float v5 = readVoltage(V5_SENSE_PIN, V5_RATIO);
    float v3v3 = readVoltage(V3V3_SENSE_PIN, V3V3_RATIO);
    
    Serial.println("=== Voltage Rails ===");
    Serial.print("24V Rail: "); Serial.print(v24, 2); Serial.println(" V");
    Serial.print("12V Rail: "); Serial.print(v12, 2); Serial.println(" V");
    Serial.print("5V Rail:  "); Serial.print(v5, 2); Serial.println(" V");
    Serial.print("3.3V Rail:"); Serial.print(v3v3, 2); Serial.println(" V");
    
    // Check for under/over voltage
    if (v24 < 22.0 || v24 > 26.0) Serial.println("⚠️ 24V OUT OF RANGE!");
    if (v12 < 11.0 || v12 > 13.0) Serial.println("⚠️ 12V OUT OF RANGE!");
    if (v5 < 4.75 || v5 > 5.25) Serial.println("⚠️ 5V OUT OF RANGE!");
    if (v3v3 < 3.1 || v3v3 > 3.5) Serial.println("⚠️ 3.3V OUT OF RANGE!");
    
    Serial.println();
    delay(1000);
}
```

### Prosedur Percobaan

1. **Build Power Tree:**
   - Hubungkan SMPS 24V sebagai input
   - Pasang DC-DC converter 24V→12V
   - Pasang DC-DC converter 24V→5V
   - Tambahkan voltage divider untuk monitoring

2. **Measure No-Load Voltage:**
   - Ukur output setiap rail tanpa beban
   - Catat ripple voltage dengan oscilloscope (AC coupling)

3. **Measure Under Load:**
   - Tambahkan beban pada setiap rail
   - Ukur voltage drop dan ripple

4. **Efficiency Calculation:**
   - Ukur input current (24V rail)
   - Ukur output current setiap rail
   - Hitung efisiensi converter

### Tabel Data

**Voltage Rail Measurement:**
| Rail | No Load (V) | With Load (V) | Ripple (mVpp) | Target | Pass/Fail |
|------|-------------|---------------|---------------|--------|-----------|
| 24V | | | | 24.0 ±5% | |
| 12V | | | | 12.0 ±5% | |
| 5V | | | | 5.0 ±5% | |
| 3.3V | | | | 3.3 ±5% | |

**Efficiency Calculation:**
| Converter | Input Power (W) | Output Power (W) | Efficiency (%) |
|-----------|-----------------|------------------|----------------|
| 24V→12V | | | |
| 24V→5V | | | |

---

## PERCOBAAN 2: NOISE MEASUREMENT & FILTERING
**Durasi:** 75 menit

### Tujuan
Mengukur noise dari motor PWM dan mengimplementasi filtering.

### Setup

```
    ┌─────────────────────────────────────────────────────────┐
    │                 NOISE MEASUREMENT SETUP                 │
    └─────────────────────────────────────────────────────────┘
    
    ESP32                    Motor Driver              Motor
    ┌─────┐                  ┌─────────┐             ┌─────┐
    │GPIO │───PWM───────────►│  L298N  ├────────────►│  DC │
    │  21 │                  │         │             │Motor│
    └─────┘                  └─────────┘             └─────┘
                                  │
                             ┌────┴────┐
                             │  12V    │
                             │  Rail   │
                             └────┬────┘
                                  │
                              Scope CH1
                             (Measure Noise)
    
    Analog                   Filter                  ESP32
    Sensor                   Circuit                 ADC
    ┌─────┐   ┌─────────────────────────┐          ┌─────┐
    │ POT │──►│ R=10k ─┬─ R=10k ─┬────────────────►│GPIO │
    │     │   │        │         │                 │  34 │
    └─────┘   │       C=100nF   C=10nF             └─────┘
              │        │         │
              └────────┴─────────┴─── GND
                     2-Stage RC Filter
```

### Kode Program

```cpp
// File: experiment2_noise_analysis.cpp

#include <Arduino.h>

// Pins
#define PWM_PIN         21
#define MOTOR_IN1       18
#define MOTOR_IN2       19
#define ADC_RAW_PIN     34    // Direct sensor (no filter)
#define ADC_FILT_PIN    35    // Filtered sensor

// PWM settings
#define PWM_FREQ        20000   // 20kHz
#define PWM_RESOLUTION  8       // 8-bit (0-255)
#define PWM_CHANNEL     0

// Sampling
#define SAMPLE_COUNT    1000
#define SAMPLE_PERIOD   100     // µs (10kHz sampling)

int rawSamples[SAMPLE_COUNT];
int filtSamples[SAMPLE_COUNT];

void setup() {
    Serial.begin(115200);
    Serial.println("=== NOISE ANALYSIS ===");
    
    // Motor pins
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    
    // PWM setup
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);
    
    // ADC setup
    analogReadResolution(12);
    
    Serial.println("Commands:");
    Serial.println("  0-9: Set PWM duty (0=0%, 9=90%)");
    Serial.println("  M: Measure noise");
    Serial.println("  S: Stop motor");
}

void loop() {
    if (Serial.available()) {
        char cmd = Serial.read();
        
        if (cmd >= '0' && cmd <= '9') {
            int duty = (cmd - '0') * 28;  // 0-252
            ledcWrite(PWM_CHANNEL, duty);
            Serial.print("PWM duty: ");
            Serial.print(duty * 100 / 255);
            Serial.println("%");
        }
        else if (cmd == 'M' || cmd == 'm') {
            measureNoise();
        }
        else if (cmd == 'S' || cmd == 's') {
            ledcWrite(PWM_CHANNEL, 0);
            Serial.println("Motor stopped");
        }
    }
}

void measureNoise() {
    Serial.println("\nCapturing samples...");
    
    // Capture samples
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        rawSamples[i] = analogRead(ADC_RAW_PIN);
        filtSamples[i] = analogRead(ADC_FILT_PIN);
        delayMicroseconds(SAMPLE_PERIOD);
    }
    
    // Calculate statistics for raw signal
    float rawMean = 0, rawMin = 4095, rawMax = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        rawMean += rawSamples[i];
        if (rawSamples[i] < rawMin) rawMin = rawSamples[i];
        if (rawSamples[i] > rawMax) rawMax = rawSamples[i];
    }
    rawMean /= SAMPLE_COUNT;
    
    float rawVariance = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        rawVariance += (rawSamples[i] - rawMean) * (rawSamples[i] - rawMean);
    }
    rawVariance /= SAMPLE_COUNT;
    float rawStdDev = sqrt(rawVariance);
    
    // Calculate statistics for filtered signal
    float filtMean = 0, filtMin = 4095, filtMax = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        filtMean += filtSamples[i];
        if (filtSamples[i] < filtMin) filtMin = filtSamples[i];
        if (filtSamples[i] > filtMax) filtMax = filtSamples[i];
    }
    filtMean /= SAMPLE_COUNT;
    
    float filtVariance = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        filtVariance += (filtSamples[i] - filtMean) * (filtSamples[i] - filtMean);
    }
    filtVariance /= SAMPLE_COUNT;
    float filtStdDev = sqrt(filtVariance);
    
    // Print results
    Serial.println("\n=== NOISE ANALYSIS RESULTS ===");
    Serial.println("\nRAW SIGNAL (unfiltered):");
    Serial.print("  Mean:     "); Serial.println(rawMean, 1);
    Serial.print("  Min:      "); Serial.println(rawMin);
    Serial.print("  Max:      "); Serial.println(rawMax);
    Serial.print("  Peak-Peak:"); Serial.println(rawMax - rawMin);
    Serial.print("  Std Dev:  "); Serial.println(rawStdDev, 2);
    Serial.print("  SNR:      "); Serial.print(rawMean / rawStdDev, 1); Serial.println(" dB");
    
    Serial.println("\nFILTERED SIGNAL:");
    Serial.print("  Mean:     "); Serial.println(filtMean, 1);
    Serial.print("  Min:      "); Serial.println(filtMin);
    Serial.print("  Max:      "); Serial.println(filtMax);
    Serial.print("  Peak-Peak:"); Serial.println(filtMax - filtMin);
    Serial.print("  Std Dev:  "); Serial.println(filtStdDev, 2);
    Serial.print("  SNR:      "); Serial.print(filtMean / filtStdDev, 1); Serial.println(" dB");
    
    Serial.println("\nNOISE REDUCTION:");
    float reduction = (rawStdDev - filtStdDev) / rawStdDev * 100;
    Serial.print("  Reduction: "); Serial.print(reduction, 1); Serial.println("%");
    
    // Output raw data for plotting (CSV format)
    Serial.println("\n=== RAW DATA (first 100 samples) ===");
    Serial.println("Index,Raw,Filtered");
    for (int i = 0; i < 100; i++) {
        Serial.print(i);
        Serial.print(",");
        Serial.print(rawSamples[i]);
        Serial.print(",");
        Serial.println(filtSamples[i]);
    }
}
```

### Prosedur Percobaan

1. **Baseline Measurement (Motor OFF):**
   - Ukur noise pada ADC dengan motor off
   - Catat baseline noise level

2. **Motor Running (No Filter):**
   - Jalankan motor dengan PWM 50%
   - Ukur noise yang couple ke sensor
   - Catat pada tabel

3. **Apply Filtering:**
   - Pasang RC filter (single stage)
   - Ulangi pengukuran
   - Bandingkan hasilnya

4. **Try Different Filter Values:**
   - Coba berbagai nilai C (10nF, 100nF, 1µF)
   - Observe trade-off: noise vs response time

### Tabel Data

**Noise Measurement:**
| Condition | Mean | Std Dev | Peak-Peak | SNR (dB) |
|-----------|------|---------|-----------|----------|
| Motor OFF, No Filter | | | | |
| Motor OFF, With Filter | | | | |
| Motor 50% PWM, No Filter | | | | |
| Motor 50% PWM, With Filter | | | | |
| Motor 100% PWM, No Filter | | | | |
| Motor 100% PWM, With Filter | | | | |

**Filter Comparison:**
| Filter Config | Cutoff (Hz) | Noise Reduction (%) | Response Delay (ms) |
|---------------|-------------|---------------------|---------------------|
| No filter | ∞ | 0% | 0 |
| R=10k, C=10nF | 1.6kHz | | |
| R=10k, C=100nF | 160Hz | | |
| R=10k, C=1µF | 16Hz | | |

---

## PERCOBAAN 3: GROUNDING TECHNIQUES
**Durasi:** 60 menit

### Tujuan
Membandingkan berbagai teknik grounding dan pengaruhnya terhadap noise.

### Setup Percobaan

**Configuration A: Common Ground (Poor)**
```
    Power      Signal     Motor
      │          │          │
      └──────────┼──────────┘
                 │
           Common GND point
           (creates ground loop)
```

**Configuration B: Star Ground (Better)**
```
    Power      Signal     Motor
      │          │          │
      │          │          │
      └────┬─────┴────┬─────┘
           │          │
         Star Point   │
           │          │
          GND       Separate
                    Motor GND
```

**Configuration C: Isolated Ground (Best)**
```
    Power           Signal          Motor
      │               │               │
      │               │               │
     GND1            GND2            GND3
                      │
                   Isolator
                   (Optocoupler)
```

### Kode Program

```cpp
// File: experiment3_grounding.cpp

#include <Arduino.h>

#define ADC_PIN         34
#define MOTOR_PWM       21
#define MOTOR_IN1       18
#define MOTOR_IN2       19
#define PWM_CHANNEL     0

#define SAMPLE_COUNT    500

void setup() {
    Serial.begin(115200);
    Serial.println("=== GROUNDING EXPERIMENT ===");
    
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    
    ledcSetup(PWM_CHANNEL, 20000, 8);
    ledcAttachPin(MOTOR_PWM, PWM_CHANNEL);
    
    Serial.println("Instructions:");
    Serial.println("1. Test dengan common ground");
    Serial.println("2. Test dengan star ground");
    Serial.println("3. Test dengan isolated ground");
    Serial.println("\nPress ENTER to capture data...");
}

void loop() {
    if (Serial.available()) {
        Serial.read();  // Clear buffer
        
        // Run motor at 50%
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        ledcWrite(PWM_CHANNEL, 128);
        
        delay(100);  // Let it stabilize
        
        // Capture samples
        float sum = 0, sumSq = 0;
        int minVal = 4095, maxVal = 0;
        
        for (int i = 0; i < SAMPLE_COUNT; i++) {
            int val = analogRead(ADC_PIN);
            sum += val;
            sumSq += val * val;
            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
            delayMicroseconds(100);
        }
        
        float mean = sum / SAMPLE_COUNT;
        float variance = (sumSq / SAMPLE_COUNT) - (mean * mean);
        float stdDev = sqrt(variance);
        
        Serial.println("\n=== RESULTS ===");
        Serial.print("Mean:      "); Serial.println(mean, 1);
        Serial.print("Std Dev:   "); Serial.println(stdDev, 2);
        Serial.print("Peak-Peak: "); Serial.println(maxVal - minVal);
        Serial.print("Noise mV:  "); Serial.println(stdDev * 3.3 / 4095 * 1000, 2);
        
        // Stop motor
        ledcWrite(PWM_CHANNEL, 0);
        
        Serial.println("\nChange grounding config and press ENTER again...");
    }
}
```

### Tabel Data

| Ground Configuration | Std Dev | Peak-Peak | Noise (mV) | Improvement |
|---------------------|---------|-----------|------------|-------------|
| Common Ground | | | | Baseline |
| Star Ground | | | | |
| Isolated Ground | | | | |

---

## PERCOBAAN 4: CABLE ROUTING & SHIELDING
**Durasi:** 60 menit

### Tujuan
Memahami pengaruh routing kabel terhadap noise coupling.

### Setup

```
    Test Configurations:
    
    A. Parallel Routing (Worst)
    ═══════════════════════════ Power
    ─────────────────────────── Signal
    
    B. 90° Crossing (Better)
    ═══════════════════════════ Power
              │
    ──────────┼─────────────── Signal
              │
    
    C. Separated + Shielded (Best)
    ═══════════════════════════ Power
    
    
    ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙ Signal (shielded)
```

### Prosedur

1. **Parallel Routing Test:**
   - Tempatkan kabel sinyal parallel dengan kabel motor (5cm jarak)
   - Ukur noise pada sinyal
   - Catat nilai

2. **90° Crossing Test:**
   - Crossing kabel sinyal dan motor pada sudut 90°
   - Ukur noise
   - Bandingkan dengan parallel

3. **Separated Test:**
   - Pisahkan kabel sinyal minimal 15cm dari motor
   - Ukur noise
   - Catat improvement

4. **Shielded Cable Test:**
   - Gunakan shielded cable untuk sinyal
   - Ground shield di satu ujung
   - Ukur noise reduction

### Tabel Data

| Routing Configuration | Distance | Noise (mV) | Improvement |
|-----------------------|----------|------------|-------------|
| Parallel, 5cm | 5cm | | Baseline |
| Parallel, 15cm | 15cm | | |
| 90° Crossing | - | | |
| Separated, 15cm | 15cm | | |
| Shielded Cable | 5cm | | |

---

## PERCOBAAN 5: POWER STABILITY UNDER DYNAMIC LOAD
**Durasi:** 45 menit

### Tujuan
Menganalisis stabilitas power rail saat motor mengalami perubahan beban.

### Kode Program

```cpp
// File: experiment5_power_stability.cpp

#include <Arduino.h>

#define V12_SENSE_PIN   35
#define MOTOR_PWM       21
#define MOTOR_IN1       18
#define MOTOR_IN2       19

#define V12_RATIO       5.0
#define ADC_VREF        3.3
#define ADC_MAX         4095.0

void setup() {
    Serial.begin(115200);
    Serial.println("=== POWER STABILITY TEST ===");
    
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    
    ledcSetup(0, 20000, 8);
    ledcAttachPin(MOTOR_PWM, 0);
    
    Serial.println("Test sequence: Accel → Hold → Decel → Reverse");
    Serial.println("Monitoring 12V rail voltage...\n");
    delay(2000);
}

void loop() {
    static bool testRunning = false;
    
    if (Serial.available()) {
        Serial.read();
        testRunning = true;
    }
    
    if (testRunning) {
        Serial.println("Time(ms),Voltage(V),PWM,Phase");
        
        unsigned long startTime = millis();
        
        // Phase 1: Acceleration
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        
        for (int pwm = 0; pwm <= 255; pwm += 5) {
            ledcWrite(0, pwm);
            
            float voltage = analogRead(V12_SENSE_PIN) / ADC_MAX * ADC_VREF * V12_RATIO;
            
            Serial.print(millis() - startTime);
            Serial.print(",");
            Serial.print(voltage, 3);
            Serial.print(",");
            Serial.print(pwm);
            Serial.println(",ACCEL");
            
            delay(50);
        }
        
        // Phase 2: Hold
        for (int i = 0; i < 20; i++) {
            float voltage = analogRead(V12_SENSE_PIN) / ADC_MAX * ADC_VREF * V12_RATIO;
            
            Serial.print(millis() - startTime);
            Serial.print(",");
            Serial.print(voltage, 3);
            Serial.print(",255,HOLD");
            Serial.println();
            
            delay(50);
        }
        
        // Phase 3: Deceleration
        for (int pwm = 255; pwm >= 0; pwm -= 5) {
            ledcWrite(0, pwm);
            
            float voltage = analogRead(V12_SENSE_PIN) / ADC_MAX * ADC_VREF * V12_RATIO;
            
            Serial.print(millis() - startTime);
            Serial.print(",");
            Serial.print(voltage, 3);
            Serial.print(",");
            Serial.print(pwm);
            Serial.println(",DECEL");
            
            delay(50);
        }
        
        // Phase 4: Reverse
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
        
        for (int pwm = 0; pwm <= 200; pwm += 5) {
            ledcWrite(0, pwm);
            
            float voltage = analogRead(V12_SENSE_PIN) / ADC_MAX * ADC_VREF * V12_RATIO;
            
            Serial.print(millis() - startTime);
            Serial.print(",");
            Serial.print(voltage, 3);
            Serial.print(",");
            Serial.print(pwm);
            Serial.println(",REVERSE");
            
            delay(50);
        }
        
        // Stop
        ledcWrite(0, 0);
        
        Serial.println("\n=== TEST COMPLETE ===");
        Serial.println("Press ENTER to run again...");
        
        testRunning = false;
    }
}
```

### Analisis

Plot data dan identifikasi:
1. Voltage sag saat motor start
2. Ripple saat steady state
3. Voltage spike saat motor stop/reverse
4. Overall stability range

---

## TUGAS & LAPORAN

### Tugas Individu
1. Desain power tree untuk robot dengan:
   - 2x motor 24V/5A
   - 1x ESP32
   - 4x sensor analog 3.3V
   - 2x servo 6V
   - Calculate power budget dan pilih komponen

2. Buat schematic filter untuk ADC dengan:
   - Signal bandwidth: 100Hz
   - ADC sampling: 1kHz
   - Target SNR: > 40dB

### Laporan Kelompok
**Format:** PDF, 10-15 halaman

**Isi:**
1. **Pendahuluan** (1 halaman)
2. **Metodologi** (2 halaman)
3. **Hasil Percobaan** (5-7 halaman)
   - Data lengkap dengan grafik
   - Screenshot oscilloscope
   - Analisis statistik noise
4. **Pembahasan** (2 halaman)
   - Perbandingan teknik filtering
   - Best practices untuk implementasi
5. **Kesimpulan** (1 halaman)

**Deadline:** 1 minggu setelah praktikum

---

## KRITERIA PENILAIAN

| Aspek | Bobot | Kriteria |
|-------|-------|----------|
| Power Tree Design | 20% | Complete, efficient, properly sized |
| Noise Measurement | 25% | Accurate, well-documented |
| Filter Implementation | 25% | Effective noise reduction |
| Grounding & Routing | 20% | Proper technique, measurable improvement |
| Laporan | 10% | Clear, complete, professional |

---

**Selamat mengerjakan!**

*"Clean power is the foundation of a reliable system."*
