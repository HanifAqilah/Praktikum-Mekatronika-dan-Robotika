# MATERI MODUL 06: BUILD DAN PROGRAM ROBOT LINE FOLLOWER

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 06 – Build Line Follower  
**Hardware:** PCB Robot (Modul 01+03), ESP32, Motor DC, Sensor IR Array  
**Software:** Arduino IDE / PlatformIO  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan Robot Line Follower](#1-pendahuluan-robot-line-follower)
2. [Prinsip Kerja Sensor Garis Infrared](#2-prinsip-kerja-sensor-garis-infrared)
3. [Shift Register 74HC165 untuk Multiplexing Sensor](#3-shift-register-74hc165-untuk-multiplexing-sensor)
4. [Motor DC dan H-Bridge Driver L293D](#4-motor-dc-dan-h-bridge-driver-l293d)
5. [Algoritma Line Following Dasar](#5-algoritma-line-following-dasar)
6. [Kontrol PID untuk Line Following](#6-kontrol-pid-untuk-line-following)
7. [Tuning PID](#7-tuning-pid)
8. [Assembly Hardware Robot](#8-assembly-hardware-robot)
9. [Kalibrasi Sensor](#9-kalibrasi-sensor)
10. [Optimasi Kecepatan dan Stabilitas](#10-optimasi-kecepatan-dan-stabilitas)
11. [Referensi](#11-referensi)

---

## 1. PENDAHULUAN ROBOT LINE FOLLOWER

### 1.1 Definisi

**Robot Line Follower** adalah robot otonom yang mengikuti garis (biasanya hitam di atas latar putih) menggunakan sensor cahaya infrared. Robot ini merupakan salah satu proyek fundamental dalam robotika yang mengajarkan:

- Pembacaan sensor analog/digital
- Algoritma kontrol (ON/OFF, PID)
- Aktuasi motor DC dengan PWM
- Integrasi hardware-software

### 1.2 Komponen Sistem

```
[Sensor IR Array] → [Shift Register 74HC165] → [ESP32] → [L293D] → [Motor DC L/R]
    12 sensor           2 IC (SPI)              MCU      H-Bridge     Aktuator
```

### 1.3 Prinsip Dasar

- **Garis hitam** menyerap cahaya infrared → photodioda menerima sedikit cahaya → tegangan rendah
- **Latar putih** memantulkan cahaya → photodioda menerima banyak → tegangan tinggi
- Robot mendeteksi posisi garis relatif terhadap sensor → mengoreksi arah gerakan

---

## 2. PRINSIP KERJA SENSOR GARIS INFRARED

### 2.1 Komponen Sensor

| Komponen | Fungsi |
|---------|--------|
| **LED Infrared (IR)** | Memancarkan cahaya IR ke permukaan |
| **Photodioda** | Menerima pantulan cahaya IR |
| **Resistor pull-down** | Bias photodioda, menghasilkan tegangan proporsional |

### 2.2 Rangkaian Sensor Tunggal

```
VCC (5V)
  │
  R_LED (68Ω)
  │
  LED IR → [Pantulan dari permukaan] → Photodioda
                                          │
                                       R_pulldown (10kΩ)
                                          │
                                         GND
                                          
Output: Junction Photodioda-Resistor → ke ADC/Digital Input
```

### 2.3 Interpretasi

| Permukaan | Pantulan | Tegangan Output | Digital |
|-----------|---------|----------------|---------|
| Putih | Tinggi | Tinggi (~3–4V) | 1 |
| Hitam | Rendah | Rendah (~0.5–1V) | 0 |

### 2.4 Array Sensor

12 pasang LED IR + Photodioda disusun dalam barisan horizontal:
- Jarak antar sensor: 10 mm center-to-center
- Lebar total array: ~120 mm
- Lebar garis standar: 20–25 mm
- Minimal 2–3 sensor berada di atas garis sekaligus

---

## 3. SHIFT REGISTER 74HC165 UNTUK MULTIPLEXING SENSOR

### 3.1 Mengapa Perlu Shift Register?

12 sensor memerlukan 12 pin GPIO jika koneksi langsung. Dengan shift register:
- 2 × 74HC165 (8-bit parallel-in serial-out) → hanya butuh **3 pin SPI**
- Efisien pin, scalable, kompatibel 3.3V

### 3.2 Koneksi 74HC165

```
Sensor 0-7 → 74HC165 #1 (D0-D7)
Sensor 8-11 → 74HC165 #2 (D0-D3, D4-D7 = LOW)

74HC165 #1:
  SH/LD (Pin 1) ← GPIO_LOAD (ESP32)
  CLK   (Pin 2) ← GPIO_CLK (ESP32)
  QH    (Pin 9) → SER_IN 74HC165 #2

74HC165 #2:
  SH/LD (Pin 1) ← GPIO_LOAD (ESP32)
  CLK   (Pin 2) ← GPIO_CLK (ESP32)
  QH    (Pin 9) → GPIO_DATA (ESP32)
```

### 3.3 Kode Pembacaan

```cpp
#define LOAD_PIN  5   // SH/LD
#define CLK_PIN   18  // CLK
#define DATA_PIN  19  // QH (serial data)

uint16_t readSensors() {
  // Latch: load parallel data
  digitalWrite(LOAD_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(LOAD_PIN, HIGH);
  delayMicroseconds(5);
  
  // Shift in 16 bits
  uint16_t data = 0;
  for (int i = 15; i >= 0; i--) {
    data |= (digitalRead(DATA_PIN) << i);
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(5);
  }
  return data; // bit 0-11 = sensor 0-11
}
```

---

## 4. MOTOR DC DAN H-BRIDGE DRIVER L293D

### 4.1 H-Bridge Concept

H-Bridge memungkinkan motor DC berputar **dua arah** dan dikontrol kecepatannya via **PWM**.

```
     VCC
      │
  ┌───┴───┐
  │S1   S2│
  │       │
  ├─MOTOR─┤
  │       │
  │S3   S4│
  └───┬───┘
     GND

S1+S4 ON → Motor CW
S2+S3 ON → Motor CCW
S1+S2 ON → Brake (short)
All OFF  → Coast (free)
```

### 4.2 L293D IC

| Pin L293D | Nama | Fungsi |
|----------|------|--------|
| 1 | EN 1,2 | Enable Motor A (PWM) |
| 2 | IN1 | Input 1 Motor A |
| 3 | OUT1 | Output 1 Motor A |
| 4-5 | GND | Ground + heatsink |
| 6 | OUT2 | Output 2 Motor A |
| 7 | IN2 | Input 2 Motor A |
| 8 | VS | Motor supply voltage |
| 9 | EN 3,4 | Enable Motor B (PWM) |
| 10 | IN3 | Input 1 Motor B |
| 11 | OUT3 | Output 1 Motor B |
| 12-13 | GND | Ground + heatsink |
| 14 | OUT4 | Output 2 Motor B |
| 15 | IN4 | Input 2 Motor B |
| 16 | VSS | Logic supply (5V) |

### 4.3 Kode Kontrol Motor

```cpp
#define ENA  25  // PWM Motor Kiri
#define IN1  26  // Arah Motor Kiri
#define IN2  27
#define ENB  32  // PWM Motor Kanan
#define IN3  33  // Arah Motor Kanan
#define IN4  14

void motorSetup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  ledcSetup(0, 5000, 8); // Channel 0, 5kHz, 8-bit
  ledcSetup(1, 5000, 8);
  ledcAttachPin(ENA, 0);
  ledcAttachPin(ENB, 1);
}

void setMotors(int speedL, int speedR) {
  // speedL, speedR: -255 to 255
  if (speedL >= 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    speedL = -speedL;
  }
  if (speedR >= 0) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    speedR = -speedR;
  }
  ledcWrite(0, constrain(speedL, 0, 255));
  ledcWrite(1, constrain(speedR, 0, 255));
}
```

---

## 5. ALGORITMA LINE FOLLOWING DASAR

### 5.1 Metode ON/OFF (Bang-Bang)

```cpp
void lineFollowBasic(uint16_t sensorData) {
  // sensorData: bit 0-11, 1=putih, 0=hitam(garis)
  bool leftSensors  = !(sensorData & 0x003); // sensor 0-1
  bool rightSensors = !(sensorData & 0xC00); // sensor 10-11
  bool centerLeft   = !(sensorData & 0x030); // sensor 4-5
  bool centerRight  = !(sensorData & 0x0C0); // sensor 6-7
  
  if (centerLeft && centerRight) {
    setMotors(150, 150);  // Lurus
  } else if (leftSensors) {
    setMotors(80, 180);   // Belok kiri
  } else if (rightSensors) {
    setMotors(180, 80);   // Belok kanan
  } else {
    setMotors(100, 100);  // Default maju
  }
}
```

### 5.2 Metode Weighted Position

```cpp
float calculatePosition(uint16_t sensorData) {
  float weightedSum = 0;
  float totalActive = 0;
  
  for (int i = 0; i < 12; i++) {
    bool onLine = !((sensorData >> i) & 1); // 0=on line
    if (onLine) {
      weightedSum += i * 1000; // posisi sensor × 1000
      totalActive += 1;
    }
  }
  
  if (totalActive == 0) return -1; // garis hilang
  return weightedSum / totalActive; // posisi 0-11000
  // Center = 5500
}
```

---

## 6. KONTROL PID UNTUK LINE FOLLOWING

### 6.1 Teori PID

**PID (Proportional-Integral-Derivative)** adalah algoritma kontrol yang menghitung koreksi berdasarkan:

$$u(t) = K_p \cdot e(t) + K_i \cdot \int_0^t e(\tau) d\tau + K_d \cdot \frac{de(t)}{dt}$$

Dimana:
- $e(t)$ = error (selisih posisi aktual dari setpoint)
- $K_p$ = gain proporsional
- $K_i$ = gain integral
- $K_d$ = gain derivatif
- $u(t)$ = output kontrol (koreksi steering)

### 6.2 PID Diskret

$$u[n] = K_p \cdot e[n] + K_i \cdot \sum_{k=0}^{n} e[k] \cdot \Delta t + K_d \cdot \frac{e[n] - e[n-1]}{\Delta t}$$

### 6.3 Implementasi PID Line Follower

```cpp
float Kp = 0.1, Ki = 0.0, Kd = 0.5;
float setpoint = 5500; // center position
float lastError = 0;
float integral = 0;
int baseSpeed = 150;

void pidLineFollow(uint16_t sensorData) {
  float position = calculatePosition(sensorData);
  
  if (position < 0) {
    // Garis hilang — gunakan arah terakhir
    if (lastError > 0) setMotors(-100, 200); // search right
    else setMotors(200, -100);               // search left
    return;
  }
  
  float error = position - setpoint;
  integral += error;
  integral = constrain(integral, -10000, 10000); // anti-windup
  float derivative = error - lastError;
  
  float correction = Kp * error + Ki * integral + Kd * derivative;
  
  int speedL = baseSpeed + correction;
  int speedR = baseSpeed - correction;
  
  setMotors(speedL, speedR);
  lastError = error;
}
```

### 6.4 Analogi PID

| Komponen | Peran | Analogi |
|---------|-------|---------|
| **P (Proportional)** | Koreksi sesuai besarnya error | Semakin jauh dari garis, semakin besar belok |
| **I (Integral)** | Mengoreksi error kecil yang terakumulasi | Jika sedikit melenceng terus-menerus, perlahan koreksi |
| **D (Derivative)** | Mengantisipasi perubahan error | Jika error berubah cepat (tikungan tajam), reaksi lebih kuat |

---

## 7. TUNING PID

### 7.1 Metode Ziegler-Nichols

1. Set $K_i = 0$, $K_d = 0$
2. Naikkan $K_p$ sampai robot berosilasi stabil (bergoyang kiri-kanan secara konstan)
3. Catat $K_u$ (ultimate gain) dan $T_u$ (periode osilasi)
4. Hitung:

| Controller | $K_p$ | $K_i$ | $K_d$ |
|-----------|-------|-------|-------|
| P | $0.5 K_u$ | 0 | 0 |
| PI | $0.45 K_u$ | $1.2 K_p / T_u$ | 0 |
| PID | $0.6 K_u$ | $2 K_p / T_u$ | $K_p \cdot T_u / 8$ |

### 7.2 Metode Trial and Error

1. Mulai dengan $K_p$ kecil (0.05), $K_i = 0$, $K_d = 0$
2. Naikkan $K_p$ sampai robot mengikuti garis (mungkin masih osilasi)
3. Naikkan $K_d$ untuk meredam osilasi (mulai dari $5 \times K_p$)
4. Jika ada offset steady-state, tambahkan $K_i$ kecil (0.001)
5. Fine-tune secara iteratif

### 7.3 Tips Tuning

- Mulai dengan kecepatan **rendah** → taruh teori → baru naikkan kecepatan
- $K_d$ biasanya **5–30× lebih besar** dari $K_p$ untuk line follower
- $K_i$ biasanya **sangat kecil** atau 0 untuk line follower
- Catat setiap perubahan parameter dan hasilnya

---

## 8. ASSEMBLY HARDWARE ROBOT

### 8.1 Urutan Assembly

1. Pasang motor DC ke motor mount (3D print) → kencangkan
2. Pasang motor mount ke chassis bawah → baut M3
3. Pasang roda ke poros motor
4. Pasang caster wheel di belakang chassis
5. Pasang PCB Sensor Line ke bracket depan → baut M3
6. Hubungkan kabel ribbon PCB Sensor → PCB Main (IDC 10-pin)
7. Pasang PCB Main ke chassis bawah → baut M3
8. Hubungkan motor ke terminal block PCB Main
9. Pasang spacer → chassis atas → baut M3
10. Pasang baterai di chassis atas
11. Hubungkan power → test power-on

### 8.2 Checklist Pre-Test

- [ ] Semua baut kencang
- [ ] Kabel ribbon terpasang dengan benar (tidak terbalik)
- [ ] Motor berputar arah yang benar (test per motor)
- [ ] Sensor terbaca (test per sensor)
- [ ] Baterai terisi penuh (7.4V minimal)
- [ ] Tidak ada short circuit

---

## 9. KALIBRASI SENSOR

### 9.1 Prosedur Kalibrasi

```cpp
uint16_t sensorMin[12], sensorMax[12];

void calibrate() {
  Serial.println("Kalibrasi: gerakkan robot di atas garis dan latar...");
  
  // Inisialisasi
  for (int i = 0; i < 12; i++) {
    sensorMin[i] = 4095;
    sensorMax[i] = 0;
  }
  
  // Baca selama 5 detik, gerakkan robot maju-mundur
  unsigned long start = millis();
  while (millis() - start < 5000) {
    uint16_t raw = readSensors();
    for (int i = 0; i < 12; i++) {
      bool val = (raw >> i) & 1;
      if (val < sensorMin[i]) sensorMin[i] = val;
      if (val > sensorMax[i]) sensorMax[i] = val;
    }
  }
  Serial.println("Kalibrasi selesai!");
}
```

### 9.2 Tips Kalibrasi

- Lakukan di arena kompetisi (atau permukaan serupa)
- Pastikan pencahayaan konsisten (tidak ada bayangan)
- Gerakkan robot perlahan melewati garis dan latar bergantian
- Simpan nilai kalibrasi ke EEPROM jika diperlukan

---

## 10. OPTIMASI KECEPATAN DAN STABILITAS

### 10.1 Teknik Optimasi

| Teknik | Penjelasan |
|--------|-----------|
| **Adaptive speed** | Kurangi kecepatan di tikungan, naikkan di lurus |
| **Predictive steering** | Gunakan sensor terluar untuk prediksi tikungan |
| **Anti-windup** | Batas integral agar tidak overflow |
| **Dead-band** | Ignore error kecil (noise) |
| **Exponential smoothing** | Filter noise pembacaan sensor |

### 10.2 Adaptive Speed

```cpp
void adaptiveSpeed(float error) {
  float absError = abs(error);
  if (absError < 1000) {
    baseSpeed = 200; // jalan lurus → cepat
  } else if (absError < 3000) {
    baseSpeed = 150; // sedikit belok → sedang
  } else {
    baseSpeed = 100; // tikungan tajam → pelan
  }
}
```

---

## 11. REFERENSI

1. Ang, K. H., Chong, G., & Li, Y. (2005). *PID Control System Analysis, Design, and Technology*. IEEE Transactions on Control Systems Technology. https://ieeexplore.ieee.org/abstract/document/4587662
2. Arduino. (2024). *Arduino Robotics Guides*. https://www.arduino.cc/en/Guide
3. Warren, J. et al. (2011). *Arduino Robotics*. O'Reilly Media.
4. Arduino PID Library: https://github.com/br3ttb/Arduino-PID-Library
5. Texas Instruments. (2024). *L293D Datasheet*. https://www.ti.com/product/L293D

---

Dokumen ini menjadi acuan utama materi Modul 06 Build dan Program Robot Line Follower.
