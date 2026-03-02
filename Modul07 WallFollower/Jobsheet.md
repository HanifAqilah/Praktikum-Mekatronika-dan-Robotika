# JOBSHEET MODUL 07: ROBOT WALL FOLLOWER

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 07 – Wall Follower  
**Pertemuan:** 13–14 (2 × 2 SKS)  
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

1. Memasang dan mengkalibrasi sensor ultrasonik HC-SR04 pada robot
2. Membaca jarak dari 3 sensor ultrasonik (depan, kiri, kanan) secara berurutan
3. Mengimplementasikan filter moving average untuk stabilisasi pembacaan
4. Mengimplementasikan algoritma wall following dengan P-control dan PD-control
5. Mengimplementasikan obstacle avoidance dasar
6. Merancang Finite State Machine (FSM) untuk perilaku robot multi-state
7. Mengintegrasikan line follower + wall follower dalam satu robot

---

## B. ALAT DAN BAHAN

| No | Komponen | Keterangan |
|----|---------|-----------|
| 1 | Robot Line Follower (Modul 06) | Sudah terakit dan berfungsi |
| 2 | Sensor HC-SR04 × 3 | Depan, kiri, kanan |
| 3 | Bracket sensor (3D print) | Mounting HC-SR04 |
| 4 | Kabel jumper Female-Female | Koneksi sensor |
| 5 | Dinding labirin (kardus/papan) | Minimal 3 sisi, tinggi 15 cm |
| 6 | Penggaris / meteran | Kalibrasi jarak |
| 7 | Laptop + Arduino IDE | Pemrograman |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: PEMASANGAN DAN TEST SENSOR HC-SR04

**Estimasi waktu: 20 menit**

1. Pasang 3 sensor HC-SR04 pada bracket:
   - **Depan**: menghadap lurus ke depan robot
   - **Kiri**: menghadap 90° ke kiri
   - **Kanan**: menghadap 90° ke kanan
2. Hubungkan ke ESP32:

| Sensor | TRIG | ECHO | VCC | GND |
|--------|------|------|-----|-----|
| Depan | GPIO 12 | GPIO 13 | 5V | GND |
| Kiri | GPIO 4 | GPIO 16 | 5V | GND |
| Kanan | GPIO 17 | GPIO 2 | 5V | GND |

3. Upload dan test:

```cpp
void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT); pinMode(13, INPUT);
  pinMode(4, OUTPUT);  pinMode(16, INPUT);
  pinMode(17, OUTPUT); pinMode(2, INPUT);
}

float readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long dur = pulseIn(echoPin, HIGH, 25000);
  if (dur == 0) return 999.0;
  return dur * 0.0343 / 2.0;
}

void loop() {
  float dF = readDistance(12, 13); delay(30);
  float dL = readDistance(4, 16);  delay(30);
  float dR = readDistance(17, 2);  delay(30);
  Serial.printf("F:%.1f L:%.1f R:%.1f cm\n", dF, dL, dR);
}
```

**Kalibrasi:** Letakkan objek di jarak 10, 20, 30 cm dari masing-masing sensor:

| Jarak Objek Aktual (cm) | Sensor Depan | Sensor Kiri | Sensor Kanan |
|--------------------------|-------------|-------------|-------------|
| 10 | | | |
| 20 | | | |
| 30 | | | |

**Checkpoint ✅:** 3 sensor ultrasonik terbaca dengan benar (±2 cm).

---

### PERCOBAAN 2: FILTER MOVING AVERAGE

**Estimasi waktu: 15 menit**

```cpp
#define FILTER_SIZE 5
float buf[3][FILTER_SIZE];
int fIdx = 0;

float filtered(int sid, float val) {
  buf[sid][fIdx % FILTER_SIZE] = val;
  float sum = 0;
  for (int i = 0; i < FILTER_SIZE; i++) sum += buf[sid][i];
  return sum / FILTER_SIZE;
}

float dF, dL, dR;

void readAll() {
  dF = filtered(0, readDistance(12, 13)); delay(30);
  dL = filtered(1, readDistance(4, 16));  delay(30);
  dR = filtered(2, readDistance(17, 2));  delay(30);
  fIdx++;
}
```

**Pengamatan:** Bandingkan output Serial tanpa filter vs dengan filter pada jarak 20 cm:

| Parameter | Tanpa Filter | Dengan Filter (size=5) |
|-----------|-------------|----------------------|
| Nilai min | | |
| Nilai max | | |
| Standar deviasi | | |

**Checkpoint ✅:** Output sensor lebih stabil dengan filter.

---

### PERCOBAAN 3: WALL FOLLOWING – P CONTROL

**Estimasi waktu: 30 menit**

Buat dinding lurus minimal 1 meter. Letakkan robot sejajar dinding di sisi kiri.

```cpp
float targetDist = 15.0;
float Kp_w = 5.0;
int baseSpd = 120;

void wallFollowP() {
  readAll();
  
  if (dF < 15.0) {
    setMotors(100, -100); delay(400);
    return;
  }
  
  float error = targetDist - dL;
  float cor = Kp_w * error;
  setMotors(baseSpd - cor, baseSpd + cor);
  
  Serial.printf("L:%.1f err:%.1f cor:%.1f\n", dL, error, cor);
}

void loop() { wallFollowP(); }
```

**Tuning Kp:**

| No | Kp | baseSpeed | Perilaku |
|----|----|----------|---------|
| 1 | 2.0 | 120 | |
| 2 | 5.0 | 120 | |
| 3 | 8.0 | 120 | |
| 4 | 12.0 | 120 | |

**Checkpoint ✅:** Robot mengikuti dinding lurus dengan P-control.

---

### PERCOBAAN 4: WALL FOLLOWING – PD CONTROL

**Estimasi waktu: 30 menit**

```cpp
float Kp_w = 5.0, Kd_w = 15.0;
float lastErr_w = 0;

void wallFollowPD() {
  readAll();
  
  if (dF < 15.0) {
    setMotors(100, -100); delay(400);
    return;
  }
  
  float error = targetDist - dL;
  float deriv = error - lastErr_w;
  float cor = Kp_w * error + Kd_w * deriv;
  
  setMotors(baseSpd - cor, baseSpd + cor);
  lastErr_w = error;
  
  Serial.printf("err:%.1f d:%.1f cor:%.1f\n", error, deriv, cor);
}
```

**Tuning PD:**

| No | Kp | Kd | Perilaku |
|----|----|----|---------|
| 1 | 5.0 | 5.0 | |
| 2 | 5.0 | 15.0 | |
| 3 | 5.0 | 25.0 | |
| 4 | 8.0 | 20.0 | |

Bandingkan P-only vs PD:

| Aspek | P-only | PD |
|-------|--------|-----|
| Osilasi | | |
| Settling time | | |
| Kestabilan | | |

**Checkpoint ✅:** PD-control lebih stabil dari P-only.

---

### PERCOBAAN 5: OBSTACLE AVOIDANCE

**Estimasi waktu: 20 menit**

Letakkan rintangan (kotak) di jalur robot.

```cpp
void obstacleAvoid() {
  readAll();
  
  if (dF > 30) {
    setMotors(baseSpd, baseSpd);
  } else if (dF < 15) {
    if (dL > dR) setMotors(-80, 120);
    else setMotors(120, -80);
  } else {
    int spd = map(dF, 15, 30, 60, baseSpd);
    setMotors(spd, spd);
  }
}

void loop() { obstacleAvoid(); }
```

**Test:** Letakkan 3 rintangan di jalur lurus.

| Rintangan | Jarak Deteksi | Arah Belok | Berhasil? |
|-----------|-------------|-----------|-----------|
| 1 | | | |
| 2 | | | |
| 3 | | | |

**Checkpoint ✅:** Robot menghindari rintangan tanpa tabrakan.

---

### PERCOBAAN 6: FINITE STATE MACHINE (FSM)

**Estimasi waktu: 40 menit**

Buat labirin sederhana (minimal 3 belokan, 1 dead-end):

```cpp
enum State { FOLLOW_WALL, TURN_RIGHT, SEARCH_WALL, STOPPED };
State state = FOLLOW_WALL;

void loop() {
  readAll();
  
  switch (state) {
    case FOLLOW_WALL:
      wallFollowPD();
      if (dF < 15) { state = TURN_RIGHT; Serial.println("→ TURN"); }
      if (dL > 30) { state = SEARCH_WALL; Serial.println("→ SEARCH"); }
      break;
      
    case TURN_RIGHT:
      setMotors(100, -100);
      delay(500);
      if (dF > 20) { state = FOLLOW_WALL; Serial.println("→ FOLLOW"); }
      break;
      
    case SEARCH_WALL:
      setMotors(80, 120);
      if (dL < 25) { state = FOLLOW_WALL; Serial.println("→ FOLLOW"); }
      if (dF < 15) { state = TURN_RIGHT; Serial.println("→ TURN"); }
      break;
      
    case STOPPED:
      setMotors(0, 0);
      break;
  }
}
```

**Dokumentasi FSM di arena:**

| Segmen Arena | State | Transisi | Catatan |
|-------------|-------|---------|--------|
| Dinding lurus | FOLLOW_WALL | | |
| Belokan kanan | | | |
| Dead-end | | | |
| Dinding hilang | | | |

**Checkpoint ✅:** Robot menyelesaikan labirin dengan FSM.

---

### PERCOBAAN 7: INTEGRASI LINE FOLLOWER + WALL FOLLOWER

**Estimasi waktu: 30 menit**

Arena gabungan: bagian garis dan bagian dinding.

```cpp
enum Mode { LINE_MODE, WALL_MODE, AVOID_MODE };
Mode mode = LINE_MODE;

void loop() {
  uint16_t lineSens = readLineSensors();
  readAll();
  
  float linePos = calculatePosition(lineSens);
  bool hasLine = (linePos >= 0);
  bool hasWall = (dL < 30 || dR < 30);
  
  if (hasLine) mode = LINE_MODE;
  else if (hasWall) mode = WALL_MODE;
  else mode = AVOID_MODE;
  
  switch (mode) {
    case LINE_MODE:  pidLineFollow(lineSens); break;
    case WALL_MODE:  wallFollowPD(); break;
    case AVOID_MODE: obstacleAvoid(); break;
  }
  
  Serial.printf("Mode:%d dF:%.0f dL:%.0f\n", mode, dF, dL);
}
```

**Checkpoint ✅:** Robot berpindah mode line→wall→line secara otomatis.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1 – Sensor HC-SR04

1. Apakah 3 sensor memberikan akurasi yang sama? Jika tidak, mengapa?
2. Berapa jarak minimum HC-SR04 bisa terbaca? Sesuai datasheet?

### Analisa Percobaan 2 – Filter

1. Berapa persen penurunan noise (standar deviasi) setelah moving average?
2. Apa dampak negatif filter terhadap responsivitas? Bagaimana trade-off-nya?

### Analisa Percobaan 3 – P-Control

1. Pada Kp berapa robot mulai berosilasi?
2. Mengapa robot masih berosilasi walaupun Kp sudah optimal?

### Analisa Percobaan 4 – PD-Control

1. Apa peran komponen D (derivative) dalam meredam osilasi?
2. Bandingkan performa P vs PD secara kuantitatif (osilasi, settling time).

### Analisa Percobaan 5 – Obstacle Avoidance

1. Apakah logika "pilih sisi yang lebih luas" selalu optimal? Kapan bisa gagal?
2. Bagaimana jika rintangan langsung di depan sensor (sudut 90°)?

### Analisa Percobaan 6 – FSM

1. Gambar diagram state FSM yang Anda implementasikan.
2. Apakah ada transisi state yang salah/tidak diharapkan? Bagaimana memperbaikinya?

### Analisa Percobaan 7 – Integrasi

1. Bagaimana robot menangani transisi dari garis ke dinding?
2. Apa yang terjadi jika ada garis DAN dinding bersamaan?

---

## E. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] Foto sensor HC-SR04 terpasang pada robot
- [ ] Tabel kalibrasi sensor
- [ ] Video robot wall following (P dan PD)
- [ ] Video robot menyelesaikan labirin
- [ ] Diagram FSM (gambar tangan atau digital)
- [ ] Kode program lengkap

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
