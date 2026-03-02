# MATERI MODUL 07: ROBOT WALL FOLLOWER

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 07 – Wall Follower  
**Hardware:** Robot Line Follower (Modul 06) + Sensor Ultrasonik HC-SR04  
**Software:** Arduino IDE / PlatformIO  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan Robot Wall Follower](#1-pendahuluan-robot-wall-follower)
2. [Sensor Ultrasonik HC-SR04](#2-sensor-ultrasonik-hc-sr04)
3. [Multi-Sensor Configuration](#3-multi-sensor-configuration)
4. [Algoritma Wall Following](#4-algoritma-wall-following)
5. [Kontrol Proporsional untuk Wall Following](#5-kontrol-proporsional-untuk-wall-following)
6. [Kontrol PD untuk Wall Following](#6-kontrol-pd-untuk-wall-following)
7. [Obstacle Avoidance](#7-obstacle-avoidance)
8. [Finite State Machine (FSM)](#8-finite-state-machine-fsm)
9. [Integrasi Line Follower + Wall Follower](#9-integrasi-line-follower--wall-follower)
10. [Referensi](#10-referensi)

---

## 1. PENDAHULUAN ROBOT WALL FOLLOWER

### 1.1 Definisi

**Robot Wall Follower** adalah robot otonom yang menjaga jarak konstan dari dinding/rintangan di satu sisi. Konsep ini fundamental dalam robotika mobile karena:

- Menjadi basis navigasi labirin (*maze solving*)
- Mengajarkan kontrol jarak menggunakan sensor ultrasonik
- Memperkenalkan *Finite State Machine* (FSM) untuk perilaku robot
- Dapat dikombinasikan dengan line following untuk arena kompleks

### 1.2 Arsitektur Sistem

```
                          ┌─────────┐
   HC-SR04 Kiri  ────────┤         │
   HC-SR04 Depan ────────┤  ESP32  ├──── Motor Kiri
   HC-SR04 Kanan ────────┤         ├──── Motor Kanan
                          └─────────┘
```

### 1.3 Dua Strategi Dasar

| Strategi | Deskripsi |
|---------|-----------|
| **Left-wall following** | Selalu jaga dinding di kiri, belok kanan jika mentok |
| **Right-wall following** | Selalu jaga dinding di kanan, belok kiri jika mentok |

Kedua strategi bisa menyelesaikan labirin *simply connected* (tanpa loop).

---

## 2. SENSOR ULTRASONIK HC-SR04

### 2.1 Spesifikasi

| Parameter | Nilai |
|----------|-------|
| Tegangan | 5V DC |
| Arus | 15 mA |
| Jarak min | 2 cm |
| Jarak max | 400 cm |
| Akurasi | ±3 mm |
| Sudut beam | 15° |
| Frekuensi | 40 kHz |

### 2.2 Prinsip Kerja

```
[Transmitter] → burst 40kHz → ~~~~ → DINDING → ~~~~ → [Receiver]
      ↑                                                    ↓
   TRIGGER                                              ECHO pulse
   (10µs HIGH)                                     (durasi ∝ jarak)
```

$$\text{Jarak (cm)} = \frac{t_{echo} \times 0.0343}{2}$$

Dimana $t_{echo}$ dalam mikrodetik, 0.0343 cm/µs = kecepatan suara pada ~20°C.

### 2.3 Kode Pembacaan

```cpp
#define TRIG_PIN 12
#define ECHO_PIN 13

float readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 25000); // timeout 25ms (~4.3m)
  if (duration == 0) return 999.0; // out of range
  return duration * 0.0343 / 2.0;
}
```

### 2.4 Keterbatasan

- **Sudut beam 15°** → tidak bisa deteksi objek tipis
- **Tidak bisa deteksi permukaan miring** (suara dipantulkan menjauhi)
- **Crosstalk** jika beberapa sensor terlalu dekat trigger bersamaan
- **Permukaan absorben** (kain, busa) mengurangi jangkauan

---

## 3. MULTI-SENSOR CONFIGURATION

### 3.1 Konfigurasi 3 Sensor

```
        ┌─────── HC-SR04 DEPAN ───────┐
        │            ↑ 0°              │
        │                              │
 HC-SR04├── ← 90° (KIRI)    (KANAN) 90° ──→ │HC-SR04
  KIRI  │                              │  KANAN
        │        ┌─────────┐           │
        └────────│  ROBOT  │───────────┘
                 └─────────┘
```

### 3.2 Pin Assignment

```cpp
// Sensor Depan
#define TRIG_F 12
#define ECHO_F 13

// Sensor Kiri
#define TRIG_L 4
#define ECHO_L 16

// Sensor Kanan
#define TRIG_R 17
#define ECHO_R 2
```

### 3.3 Pembacaan Berurutan (Menghindari Crosstalk)

```cpp
float distFront, distLeft, distRight;

void readAllSensors() {
  distFront = readDistance(TRIG_F, ECHO_F);
  delay(30); // tunggu echo selesai
  distLeft  = readDistance(TRIG_L, ECHO_L);
  delay(30);
  distRight = readDistance(TRIG_R, ECHO_R);
  delay(30);
}
```

### 3.4 Moving Average Filter

```cpp
#define FILTER_SIZE 5

float filterBuffer[3][FILTER_SIZE]; // 3 sensor
int filterIndex = 0;

float movingAverage(int sensorId, float newValue) {
  filterBuffer[sensorId][filterIndex % FILTER_SIZE] = newValue;
  float sum = 0;
  for (int i = 0; i < FILTER_SIZE; i++) sum += filterBuffer[sensorId][i];
  return sum / FILTER_SIZE;
}

void readAllFiltered() {
  distFront = movingAverage(0, readDistance(TRIG_F, ECHO_F)); delay(30);
  distLeft  = movingAverage(1, readDistance(TRIG_L, ECHO_L)); delay(30);
  distRight = movingAverage(2, readDistance(TRIG_R, ECHO_R)); delay(30);
  filterIndex++;
}
```

---

## 4. ALGORITMA WALL FOLLOWING

### 4.1 Left-Wall Following (Pseudocode)

```
LOOP:
  baca sensor depan, kiri, kanan
  
  IF depan < JARAK_AMAN:
    STOP
    BELOK_KANAN_90()  // dinding di depan → belok kanan
  
  ELSE IF kiri < JARAK_TERLALU_DEKAT:
    BELOK_KANAN_SEDIKIT()  // terlalu dekat dinding kiri
    
  ELSE IF kiri > JARAK_TERLALU_JAUH:
    BELOK_KIRI_SEDIKIT()   // terlalu jauh dari dinding kiri
    
  ELSE:
    JALAN_LURUS()           // jarak ideal
```

### 4.2 Konstanta Jarak

```cpp
#define TARGET_DISTANCE   15.0  // Jarak ideal dari dinding (cm)
#define TOO_CLOSE          8.0  // Terlalu dekat
#define TOO_FAR           25.0  // Terlalu jauh
#define FRONT_DANGER      15.0  // Rintangan di depan
```

---

## 5. KONTROL PROPORSIONAL UNTUK WALL FOLLOWING

### 5.1 Error Calculation

$$e = d_{target} - d_{actual}$$

- $e > 0$: robot terlalu jauh dari dinding → belok mendekati
- $e < 0$: robot terlalu dekat → belok menjauhi

### 5.2 Implementasi P-Control

```cpp
float targetDist = 15.0; // cm
float Kp_wall = 5.0;
int baseSpeed = 120;

void wallFollowP() {
  readAllFiltered();
  
  if (distFront < 15.0) {
    // Rintangan depan → belok kanan
    setMotors(baseSpeed, -baseSpeed);
    delay(400);
    return;
  }
  
  float error = targetDist - distLeft;
  float correction = Kp_wall * error;
  
  int speedL = baseSpeed - correction;
  int speedR = baseSpeed + correction;
  
  setMotors(speedL, speedR);
}
```

---

## 6. KONTROL PD UNTUK WALL FOLLOWING

### 6.1 Mengapa PD (Bukan PID)?

Untuk wall following:
- **P** mengoreksi jarak
- **D** meredam osilasi saat mendekati/menjauhi dinding
- **I** jarang diperlukan karena error offset biasanya kecil

$$u = K_p \cdot e + K_d \cdot \frac{de}{dt}$$

### 6.2 Implementasi PD-Control

```cpp
float Kp_wall = 5.0;
float Kd_wall = 20.0;
float lastError_wall = 0;
int baseSpeed = 120;

void wallFollowPD() {
  readAllFiltered();
  
  if (distFront < 15.0) {
    setMotors(baseSpeed, -baseSpeed);
    delay(400);
    return;
  }
  
  float error = targetDist - distLeft;
  float derivative = error - lastError_wall;
  float correction = Kp_wall * error + Kd_wall * derivative;
  
  int speedL = baseSpeed - correction;
  int speedR = baseSpeed + correction;
  
  setMotors(speedL, speedR);
  lastError_wall = error;
}
```

---

## 7. OBSTACLE AVOIDANCE

### 7.1 Strategi Obstacle Avoidance

Obstacle avoidance berbeda dari wall following — tujuannya **menghindari objek** tanpa menjaga jarak konstan.

```
State: NORMAL → jalan lurus
       ↓ (depan < 20 cm)
State: AVOID_FRONT → belok kanan/kiri (pilih sisi dengan jarak lebih jauh)
       ↓ (depan clear)
State: NORMAL
```

### 7.2 Implementasi

```cpp
void obstacleAvoid() {
  readAllFiltered();
  
  if (distFront > 30) {
    setMotors(baseSpeed, baseSpeed); // jalan lurus
  }
  else if (distFront < 15) {
    // Pilih belok ke arah yang lebih luas
    if (distLeft > distRight) {
      setMotors(-80, 120); // belok kiri
    } else {
      setMotors(120, -80); // belok kanan
    }
  }
  else {
    // Perlahan, adaptasi
    int speed = map(distFront, 15, 30, 60, baseSpeed);
    setMotors(speed, speed);
  }
}
```

---

## 8. FINITE STATE MACHINE (FSM)

### 8.1 Konsep FSM

FSM mengorganisir perilaku robot ke dalam **state** yang terdefinisi dengan **transisi** yang jelas.

```
                   ┌──────────────────────┐
                   ↓                      │
╔══════════╗   depan<15   ╔══════════╗   selesai belok
║  FOLLOW  ║ ──────────→  ║  TURN    ║ ──────────┘
║  WALL    ║              ║  RIGHT   ║
╚══════════╝              ╚══════════╝
     ↑                        │
     │    semua>30            │
     │  ╔══════════╗          │
     └──║  SEARCH  ║←────────┘
        ║  WALL    ║  kiri>30 (dinding hilang)
        ╚══════════╝
```

### 8.2 Implementasi FSM

```cpp
enum RobotState {
  FOLLOW_WALL,
  TURN_RIGHT,
  SEARCH_WALL,
  STOPPED
};

RobotState state = FOLLOW_WALL;

void loop() {
  readAllFiltered();
  
  switch (state) {
    case FOLLOW_WALL:
      wallFollowPD();
      if (distFront < 15) state = TURN_RIGHT;
      if (distLeft > 30) state = SEARCH_WALL;
      break;
      
    case TURN_RIGHT:
      setMotors(100, -100);
      delay(500);
      if (distFront > 20) state = FOLLOW_WALL;
      break;
      
    case SEARCH_WALL:
      setMotors(80, 120); // belok kiri mencari dinding
      if (distLeft < 25) state = FOLLOW_WALL;
      if (distFront < 15) state = TURN_RIGHT;
      break;
      
    case STOPPED:
      setMotors(0, 0);
      break;
  }
}
```

---

## 9. INTEGRASI LINE FOLLOWER + WALL FOLLOWER

### 9.1 Dual-Mode Robot

Robot dari Modul 06 (line follower) ditambah sensor ultrasonik menjadi robot dual-mode:

| Mode | Trigger | Perilaku |
|------|---------|---------|
| LINE_FOLLOW | Sensor garis mendeteksi garis | PID line following |
| WALL_FOLLOW | Tidak ada garis + ada dinding dekat | PD wall following |
| OBSTACLE_AVOID | Rintangan di depan saat line follow | Berhenti/belok, kembali ke garis |

### 9.2 Mode Switching

```cpp
enum Mode { LINE_MODE, WALL_MODE, AVOID_MODE };
Mode currentMode = LINE_MODE;

void loop() {
  uint16_t lineSensors = readSensors();
  readAllFiltered();
  
  float linePos = calculatePosition(lineSensors);
  bool lineDetected = (linePos >= 0);
  bool wallNearby = (distLeft < 30 || distRight < 30);
  
  if (lineDetected) {
    currentMode = LINE_MODE;
  } else if (wallNearby) {
    currentMode = WALL_MODE;
  } else {
    currentMode = AVOID_MODE;
  }
  
  switch (currentMode) {
    case LINE_MODE:  pidLineFollow(lineSensors); break;
    case WALL_MODE:  wallFollowPD(); break;
    case AVOID_MODE: obstacleAvoid(); break;
  }
}
```

---

## 10. REFERENSI

1. Choset, H. et al. (2005). *Principles of Robot Motion*. MIT Press.
2. Ang, K. H. et al. (2005). *PID Control System Analysis*. IEEE Trans. https://ieeexplore.ieee.org/abstract/document/4587662
3. Warren, J. et al. (2011). *Arduino Robotics*. O'Reilly Media.
4. HC-SR04 Datasheet: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
5. Siegwart, R. et al. (2011). *Introduction to Autonomous Mobile Robots* (2nd ed). MIT Press.

---

Dokumen ini menjadi acuan utama materi Modul 07 Robot Wall Follower.
