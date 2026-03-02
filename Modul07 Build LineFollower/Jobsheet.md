# JOBSHEET MODUL 06: BUILD DAN PROGRAM ROBOT LINE FOLLOWER

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 06 – Build Line Follower  
**Pertemuan:** 11–12 (2 × 2 SKS)  
**Tanggal:** ___________________  
**Nama Kelompok:** ___________________  
**Anggota:**

| No | Nama | NIM |
|----|------|-----|
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

1. Merakit (assembly) robot line follower dari PCB dan komponen mekanik yang sudah dibuat
2. Membaca array sensor garis infrared menggunakan shift register 74HC165
3. Mengontrol motor DC menggunakan H-Bridge L293D dengan PWM
4. Mengimplementasikan algoritma line following ON/OFF (bang-bang)
5. Mengimplementasikan kontrol PID untuk line following
6. Melakukan tuning parameter PID untuk kestabilan optimal
7. Mengimplementasikan kalibrasi sensor otomatis

---

## B. ALAT DAN BAHAN

| No | Komponen | Keterangan |
|----|---------|-----------|
| 1 | PCB Main + komponen tersolder | Dari Modul 01 |
| 2 | PCB Sensor Line + komponen | Dari Modul 01 |
| 3 | Chassis (laser cut / 3D print) | Dari Modul 03 |
| 4 | Motor mount + bracket | Dari Modul 03 |
| 5 | Motor DC TT × 2 | Dengan gearbox |
| 6 | Roda × 2 + caster wheel | Sesuai desain |
| 7 | Baterai LiPo 2S (7.4V) | Terisi penuh |
| 8 | Kabel ribbon 10-pin | Penghubung PCB |
| 9 | Toolkit (obeng, tang, solder) | Assembly |
| 10 | Arena line follower | Garis hitam latar putih |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: ASSEMBLY MEKANIK ROBOT

**Estimasi waktu: 45 menit**

1. Siapkan semua komponen mekanik (chassis, motor mount, bracket, spacer)
2. Ikuti urutan assembly:
   - Motor → motor mount → chassis bawah
   - Roda → poros motor
   - Caster wheel → belakang chassis
   - PCB Sensor → bracket → depan chassis
   - PCB Main → chassis bawah (baut M3)
   - Hubungkan kabel ribbon PCB Sensor ↔ PCB Main
   - Hubungkan motor ke terminal block
   - Spacer → chassis atas → baterai
3. Verifikasi semua koneksi dengan multimeter (continuity test)

**Tabel Verifikasi Assembly:**

| Item | Status | Catatan |
|------|--------|---------|
| Motor kiri terpasang kokoh | | |
| Motor kanan terpasang kokoh | | |
| Roda berputar bebas | | |
| Caster wheel bergerak halus | | |
| PCB Sensor terpasang horizontal | | |
| Kabel ribbon terhubung benar | | |
| Baterai terpasang aman | | |
| Power ON → LED indikator nyala | | |

**Checkpoint ✅:** Robot terakit, power ON berhasil.

---

### PERCOBAAN 2: TEST PEMBACAAN SENSOR ARRAY

**Estimasi waktu: 20 menit**

```cpp
#define LOAD_PIN  5
#define CLK_PIN   18
#define DATA_PIN  19

void setup() {
  Serial.begin(115200);
  pinMode(LOAD_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT);
}

uint16_t readSensors() {
  digitalWrite(LOAD_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(LOAD_PIN, HIGH);
  delayMicroseconds(5);
  
  uint16_t data = 0;
  for (int i = 15; i >= 0; i--) {
    data |= (digitalRead(DATA_PIN) << i);
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(5);
  }
  return data & 0x0FFF; // 12 bit sensor
}

void loop() {
  uint16_t sensors = readSensors();
  for (int i = 0; i < 12; i++) {
    Serial.print((sensors >> i) & 1);
    Serial.print(" ");
  }
  Serial.println();
  delay(100);
}
```

**Test:** Gerakkan tangan atau kertas hitam di bawah sensor.

| Kondisi | Output Expected |
|---------|----------------|
| Semua di atas putih | 1 1 1 1 1 1 1 1 1 1 1 1 |
| Sensor tengah di atas hitam | 1 1 1 1 0 0 0 1 1 1 1 1 |
| Sensor kiri di atas hitam | 0 0 1 1 1 1 1 1 1 1 1 1 |

**Checkpoint ✅:** 12 sensor terbaca dengan benar.

---

### PERCOBAAN 3: TEST MOTOR DC

**Estimasi waktu: 20 menit**

```cpp
#define ENA 25
#define IN1 26
#define IN2 27
#define ENB 32
#define IN3 33
#define IN4 14

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcAttachPin(ENA, 0);
  ledcAttachPin(ENB, 1);
}

void setMotors(int L, int R) {
  digitalWrite(IN1, L >= 0 ? HIGH : LOW);
  digitalWrite(IN2, L >= 0 ? LOW : HIGH);
  digitalWrite(IN3, R >= 0 ? HIGH : LOW);
  digitalWrite(IN4, R >= 0 ? LOW : HIGH);
  ledcWrite(0, constrain(abs(L), 0, 255));
  ledcWrite(1, constrain(abs(R), 0, 255));
}

void loop() {
  Serial.println("Maju");
  setMotors(150, 150); delay(2000);
  Serial.println("Mundur");
  setMotors(-150, -150); delay(2000);
  Serial.println("Kiri");
  setMotors(80, 200); delay(2000);
  Serial.println("Kanan");
  setMotors(200, 80); delay(2000);
  Serial.println("Stop");
  setMotors(0, 0); delay(3000);
}
```

**Verifikasi:**

| Perintah | Motor Kiri | Motor Kanan | Arah Robot |
|---------|-----------|------------|-----------|
| Maju | CW | CW | Lurus depan |
| Mundur | CCW | CCW | Lurus belakang |
| Belok kiri | Pelan | Cepat | Belok kiri |
| Belok kanan | Cepat | Pelan | Belok kanan |

> **Penting:** Jika ada motor yang terbalik arah, tukar koneksi IN1/IN2 atau IN3/IN4.

**Checkpoint ✅:** Kedua motor berputar dengan arah dan kecepatan yang benar.

---

### PERCOBAAN 4: LINE FOLLOWING BANG-BANG (ON/OFF)

**Estimasi waktu: 30 menit**

```cpp
float calculatePosition(uint16_t sensorData) {
  float weightedSum = 0;
  float total = 0;
  for (int i = 0; i < 12; i++) {
    if (!((sensorData >> i) & 1)) {
      weightedSum += i * 1000;
      total += 1;
    }
  }
  if (total == 0) return -1;
  return weightedSum / total;
}

int baseSpeed = 120;

void loop() {
  uint16_t sensors = readSensors();
  float pos = calculatePosition(sensors);
  
  if (pos < 0) {
    setMotors(0, 0); // garis hilang → stop
    return;
  }
  
  float center = 5500;
  float error = pos - center;
  
  if (error < -1500) {
    setMotors(60, baseSpeed);   // Belok kiri keras
  } else if (error < -500) {
    setMotors(100, baseSpeed);  // Belok kiri halus
  } else if (error > 1500) {
    setMotors(baseSpeed, 60);   // Belok kanan keras
  } else if (error > 500) {
    setMotors(baseSpeed, 100);  // Belok kanan halus
  } else {
    setMotors(baseSpeed, baseSpeed); // Lurus
  }
}
```

**Test di arena:** Letakkan robot di garis → jalankan.

**Checkpoint ✅:** Robot mengikuti garis dengan metode ON/OFF.

---

### PERCOBAAN 5: LINE FOLLOWING DENGAN KONTROL PID

**Estimasi waktu: 45 menit**

```cpp
float Kp = 0.08, Ki = 0.0, Kd = 0.4;
float setpoint = 5500;
float lastError = 0;
float integral = 0;
int baseSpeed = 150;

void loop() {
  uint16_t sensors = readSensors();
  float pos = calculatePosition(sensors);
  
  if (pos < 0) {
    // Search mode: belok ke arah error terakhir
    if (lastError > 0) setMotors(-80, 150);
    else setMotors(150, -80);
    return;
  }
  
  float error = pos - setpoint;
  integral += error;
  integral = constrain(integral, -10000, 10000);
  float derivative = error - lastError;
  
  float correction = Kp * error + Ki * integral + Kd * derivative;
  
  int speedL = baseSpeed + correction;
  int speedR = baseSpeed - correction;
  
  setMotors(speedL, speedR);
  lastError = error;
  
  // Debug via Serial
  Serial.printf("Pos:%.0f Err:%.0f Cor:%.1f L:%d R:%d\n", 
                pos, error, correction, speedL, speedR);
}
```

**Checkpoint ✅:** Robot mengikuti garis dengan PID, lebih halus dari bang-bang.

---

### PERCOBAAN 6: TUNING PID

**Estimasi waktu: 45 menit**

Lakukan tuning menggunakan metode trial and error:

**Langkah:**
1. Set Ki=0, Kd=0. Naikkan Kp secara bertahap.
2. Catat kondisi setiap perubahan:

| No | Kp | Ki | Kd | Base Speed | Hasil (deskripsi) |
|----|----|----|----|-----------|--------------------|
| 1 | 0.02 | 0 | 0 | 120 | |
| 2 | 0.05 | 0 | 0 | 120 | |
| 3 | 0.08 | 0 | 0 | 120 | |
| 4 | 0.10 | 0 | 0 | 120 | |
| 5 | 0.08 | 0 | 0.2 | 120 | |
| 6 | 0.08 | 0 | 0.4 | 120 | |
| 7 | 0.08 | 0 | 0.6 | 120 | |
| 8 | 0.08 | 0.001 | 0.4 | 120 | |
| 9 | (best) | | | 150 | |
| 10 | (best) | | | 180 | |

**Target:** Robot mengikuti garis tanpa osilasi di kecepatan tertinggi yang stabil.

**Checkpoint ✅:** Parameter PID optimal ditemukan dan dicatat.

---

### PERCOBAAN 7: KALIBRASI OTOMATIS DAN FITUR LANJUTAN

**Estimasi waktu: 30 menit**

Implementasikan:
1. **Kalibrasi otomatis** saat startup (robot bergerak kiri-kanan di atas garis selama 3 detik)
2. **Adaptive speed** — cepat di lurus, pelan di tikungan
3. **OLED display** — tampilkan posisi, error, dan PID parameter

```cpp
void autoCalibrate() {
  unsigned long start = millis();
  while (millis() - start < 3000) {
    setMotors(100, -100); // berputar kiri
    readSensors(); // update min/max
    delay(10);
    setMotors(-100, 100); // berputar kanan
    readSensors();
    delay(10);
  }
  setMotors(0, 0);
}
```

**Checkpoint ✅:** Fitur lanjutan berfungsi.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1 – Assembly

1. Apa kendala yang ditemui saat assembly? Bagaimana solusinya?
2. Mengapa keseimbangan berat (distribusi massa) penting untuk line follower?

### Analisa Percobaan 2 – Sensor Array

1. Berapa sensor yang aktif (mendeteksi hitam) saat robot tepat di atas garis lebar 25 mm?
2. Apa yang terjadi jika jarak sensor ke lantai terlalu jauh (>15 mm)?

### Analisa Percobaan 3 – Motor DC

1. Mengapa digunakan PWM untuk mengontrol kecepatan, bukan tegangan DC langsung?
2. Apa yang terjadi jika PWM terlalu rendah (misal 30)? Apakah motor berputar?

### Analisa Percobaan 4 – Bang-Bang

1. Jelaskan kelemahan utama metode ON/OFF. Mengapa robot berzig-zag?
2. Bagaimana performa bang-bang di tikungan tajam vs tikungan landai?

### Analisa Percobaan 5 – PID

1. Bandingkan performa PID vs Bang-Bang dalam hal kehalusan gerakan dan kecepatan.
2. Jelaskan peran masing-masing komponen P, I, D dalam konteks line following.
3. Apa yang terjadi jika Kp terlalu besar?

### Analisa Percobaan 6 – Tuning PID

1. Berdasarkan tabel tuning, pada nilai Kp berapa robot mulai berosilasi?
2. Bagaimana penambahan Kd mempengaruhi osilasi?
3. Mengapa Ki biasanya 0 atau sangat kecil untuk line follower?

### Analisa Percobaan 7 – Fitur Lanjutan

1. Mengapa kalibrasi otomatis lebih baik daripada nilai threshold hardcoded?
2. Bagaimana adaptive speed meningkatkan performa keseluruhan?

---

## E. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] Foto robot terakit (4 sudut pandang)
- [ ] Screenshot Serial Monitor sensor array
- [ ] Video robot mengikuti garis (bang-bang vs PID)
- [ ] Tabel tuning PID lengkap
- [ ] Screenshot OLED display (jika ada)
- [ ] Kode program lengkap

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
