# Jobsheet Modul 10: Praktikum Robot Line Follower

## Identitas Praktikum

- Program studi: Sarjana Terapan Teknologi Rekayasa Otomasi
- Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum
- Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.
- Modul: Line Follower
- Platform: ESP32 S2 Mini / Wemos LOLIN S2 Mini
- Folder program: `Program/01LcdI2c` sampai `Program/07PathPlanningAdvanced`

## Tujuan Praktikum

Praktikan mampu merakit, menguji, memprogram, menjalankan, dan menganalisis robot line follower berbasis ESP32 S2 Mini, OLED I2C, push button, motor DC, sensor garis 8 channel, kontrol PID, dan path planning.

## Durasi

Total praktikum: 5 jam 40 menit.

Rekomendasi pembagian waktu:

| Tahap | Kegiatan | Waktu |
|---|---|---:|
| 1 | Briefing, keselamatan, cek alat | 20 menit |
| 2 | Instalasi software dan setup awal | 40 menit |
| 3 | Pengenalan pin dan wiring | 40 menit |
| 4 | Uji OLED dan tombol | 40 menit |
| 5 | Uji motor dan driver | 50 menit |
| 6 | Uji sensor dan kalibrasi | 50 menit |
| 7 | Uji PID line follower | 70 menit |
| 8 | Uji path planning | 50 menit |
| 9 | Evaluasi, dokumentasi, backup data | 40 menit |

---

# 1. Alat dan Bahan

## 1.1 Hardware

1. ESP32 S2 Mini / Wemos LOLIN S2 Mini.
2. Sensor garis infrared array 8 channel.
3. OLED I2C 0.96 inch SSD1306 128 x 64.
4. Driver motor H-bridge, misalnya L298N, L293D, TB6612FNG, atau DRV8833.
5. 2 motor DC gearbox.
6. 2 roda utama.
7. 1 caster wheel atau ball caster.
8. Chassis robot.
9. 4 push button.
10. Resistor jika tombol tidak memakai pull-up internal.
11. Baterai motor sesuai driver dan motor.
12. Kabel jumper.
13. Breadboard atau PCB.
14. Kabel USB data.
15. Multimeter.
16. Lintasan garis hitam di atas putih atau putih di atas hitam, sesuai konfigurasi.
17. Laptop.

## 1.2 Software

1. Visual Studio Code.
2. Extension PlatformIO IDE.
3. Driver USB untuk board jika diperlukan.
4. Browser untuk dokumentasi.
5. Aplikasi perekam layar untuk tugas video.
6. Kamera HP/kamera eksternal untuk merekam proses praktik.

---

# 2. Keselamatan Kerja

1. Matikan daya baterai saat merangkai kabel.
2. Jangan menghubungkan motor langsung ke pin ESP32.
3. Gunakan driver motor sebagai penguat arus.
4. Pastikan GND ESP32, driver motor, sensor, dan baterai tersambung bersama.
5. Periksa polaritas baterai sebelum menyalakan robot.
6. Jangan menyentuh rangkaian ketika terjadi panas berlebih.
7. Angkat roda dari lantai saat pengujian motor pertama.
8. Hindari short circuit pada terminal baterai.
9. Gunakan tegangan sesuai spesifikasi board dan modul.
10. Rapikan kabel agar tidak tersangkut roda.

---

# 3. Struktur Folder Program

Folder utama modul:

```
/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul10 LineFollower
```

Folder program:

```
Program/
├── 01LcdI2c/
├── 02PushButton/
├── 03Motor/
├── 04Sensor/
├── 03Sensor/
├── 05PID/
├── 06PathPlanning/
├── 07PathPlanningAdvanced/
└── config.h
```

Fungsi setiap folder:

| Folder | Fungsi |
|---|---|
| 01LcdI2c | Menguji OLED I2C |
| 02PushButton | Menguji push button active LOW |
| 03Motor | Menguji arah dan PWM motor |
| 04Sensor | Membaca sensor analog/digital dan posisi garis |
| 03Sensor | Kalibrasi otomatis sensor dengan EEPROM dan switch case posisi |
| 05PID | Menjalankan line follower PID |
| 06PathPlanning | Menjalankan rute berbasis sensor mask dan aksi |
| 07PathPlanningAdvanced | Path planning dengan turbo, brake, dan kontrol motor 1 PWM + 1 DIR |
| config.h | Konfigurasi pin, sensor, motor, PWM, PID, helper function |

---

# 4. Konfigurasi Pin

Berdasarkan `Program/config.h`:

## 4.1 OLED I2C

| Modul OLED | ESP32 S2 Mini |
|---|---|
| VCC | 3.3 V |
| GND | GND |
| SDA | GPIO 33 |
| SCL | GPIO 35 |

Alamat OLED default:

```
0x3C
```

## 4.2 Push Button

Tombol menggunakan `INPUT_PULLUP`, sehingga aktif saat LOW.

| Tombol | Pin |
|---|---:|
| Button 1 | GPIO 0 |
| Button 2 | GPIO 12 |
| Button 3 | GPIO 13 |
| Button 4 | GPIO 15 |

Wiring tombol:

```
Satu kaki tombol -> GPIO
Kaki lain tombol -> GND
```

## 4.3 Motor DC

| Fungsi | Pin |
|---|---:|
| Motor kiri EN | GPIO 11 |
| Motor kiri IN1 | GPIO 10 |
| Motor kiri IN2 | GPIO 9 |
| Motor kanan EN | GPIO 8 |
| Motor kanan IN1 | GPIO 7 |
| Motor kanan IN2 | GPIO 6 |

PWM:

```
Frekuensi = 5000 Hz
Resolusi = 8 bit
Channel kiri = 0
Channel kanan = 1
```

Catatan penting: pada konfigurasi yang ada, beberapa pin sensor juga memakai GPIO 6, 7, dan 8. Jika board dan wiring aktual memakai pin yang sama untuk motor dan sensor, konflik harus diselesaikan dengan memindahkan pin sensor atau motor pada `config.h`. Satu GPIO tidak boleh dipakai bersamaan untuk dua fungsi fisik yang berbeda.

## 4.4 Sensor Garis 8 Channel

| Sensor | Pin |
|---|---:|
| S0 kiri | GPIO 1 |
| S1 | GPIO 2 |
| S2 | GPIO 3 |
| S3 | GPIO 4 |
| S4 | GPIO 5 |
| S5 | GPIO 16 |
| S6 | GPIO 17 |
| S7 kanan | GPIO 18 |

Threshold default:

```
SENSOR_THRESHOLD = 2000
```

---

# 5. Instalasi Software

## 5.1 Instal Visual Studio Code

1. Unduh dan instal Visual Studio Code sesuai sistem operasi.
2. Buka VS Code.
3. Pastikan koneksi internet tersedia.

## 5.2 Instal PlatformIO IDE

1. Buka VS Code.
2. Masuk ke menu Extensions.
3. Cari `PlatformIO IDE`.
4. Klik Install.
5. Tunggu sampai selesai.
6. Restart VS Code jika diminta.

## 5.3 Cek PlatformIO

1. Buka VS Code.
2. Pastikan ikon PlatformIO muncul di sidebar.
3. Buka PlatformIO Home.
4. Pastikan tidak ada error instalasi.

## 5.4 Driver USB

Jika board tidak terdeteksi:

1. Gunakan kabel USB data, bukan kabel charge saja.
2. Cek Device Manager atau perintah port serial.
3. Instal driver USB sesuai chip USB-to-serial board.
4. Cabut dan pasang ulang board.

---

# 6. Membuka Project

1. Buka VS Code.
2. Pilih `File > Open Folder`.
3. Buka salah satu folder program, misalnya:

```
/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul10 LineFollower/Program/01LcdI2c
```

4. Tunggu PlatformIO membaca project.
5. Pastikan file `platformio.ini` tersedia di folder project.
6. Buka `src/main.cpp`.

Jika ingin menjalankan program lain, tutup folder saat ini lalu buka folder program berikutnya, misalnya `02PushButton`, `03Motor`, dan seterusnya.

---

# 7. Build dan Upload Program

## 7.1 Build

Di PlatformIO:

1. Klik ikon centang atau menu `Build`.
2. Tunggu kompilasi selesai.
3. Pastikan muncul pesan success.

Melalui terminal di folder project:

```
pio run
```

## 7.2 Upload

1. Hubungkan ESP32 S2 Mini ke laptop.
2. Pilih port jika diperlukan.
3. Klik ikon panah kanan atau menu `Upload`.

Melalui terminal:

```
pio run -t upload
```

Jika upload gagal:

1. Tekan tombol BOOT saat upload dimulai jika board memerlukannya.
2. Cek kabel USB.
3. Cek port serial.
4. Tutup Serial Monitor sebelum upload.

## 7.3 Serial Monitor

Di PlatformIO klik `Monitor`, atau jalankan:

```
pio device monitor -b 115200
```

Baudrate program:

```
115200
```

---

# 8. Praktikum 1: Uji OLED I2C

Folder:

```
Program/01LcdI2c
```

## 8.1 Tujuan

Menguji komunikasi I2C dan tampilan OLED SSD1306.

## 8.2 Langkah Kerja

1. Matikan daya board.
2. Hubungkan OLED ke ESP32:
   - VCC ke 3.3 V.
   - GND ke GND.
   - SDA ke GPIO 33.
   - SCL ke GPIO 35.
3. Hubungkan ESP32 ke laptop.
4. Buka folder `01LcdI2c` di VS Code.
5. Build program.
6. Upload program.
7. Buka Serial Monitor 115200.
8. Amati OLED.

## 8.3 Hasil yang Diharapkan

OLED menampilkan splash screen:

```
Line
Follower
Modul 10 - ESP32 S2
```

Kemudian menampilkan:

- Hello World.
- Waktu running.
- Frame animasi.
- Status ESP32 S2 Mini Ready.

## 8.4 Troubleshooting

| Gejala | Solusi |
|---|---|
| OLED mati | Cek VCC dan GND |
| OLED tidak ditemukan | Cek SDA/SCL dan alamat 0x3C |
| Tampilan acak | Cek library dan resolusi 128x64 |
| Upload gagal | Cek kabel USB dan port |

## 8.5 Data yang Dicatat

- Apakah OLED berhasil tampil?
- Alamat I2C yang digunakan.
- Foto rangkaian OLED.
- Screenshot Serial Monitor.

---

# 9. Praktikum 2: Uji Push Button

Folder:

```
Program/02PushButton
```

## 9.1 Tujuan

Memahami input digital, pull-up internal, active LOW, dan debounce.

## 9.2 Langkah Kerja

1. Rangkai 4 tombol ke pin GPIO 0, 12, 13, dan 15.
2. Kaki tombol lainnya ke GND.
3. Buka folder `02PushButton`.
4. Build dan upload.
5. Buka Serial Monitor.
6. Tekan tombol satu per satu.
7. Amati output OLED dan Serial Monitor.

## 9.3 Hasil yang Diharapkan

- Saat tombol tidak ditekan, pembacaan HIGH.
- Saat tombol ditekan, pembacaan LOW.
- Program mengenali tombol sebagai input valid setelah debounce.

## 9.4 Analisis

Jawab pertanyaan berikut di catatan praktikum:

1. Mengapa tombol active LOW?
2. Apa fungsi `INPUT_PULLUP`?
3. Apa yang terjadi jika input floating?
4. Mengapa debounce diperlukan?

---

# 10. Praktikum 3: Uji Motor DC

Folder:

```
Program/03Motor
```

## 10.1 Tujuan

Menguji driver motor, arah putaran, PWM, dan fungsi `setMotors()`.

## 10.2 Langkah Wiring

1. Matikan semua sumber daya.
2. Hubungkan pin kontrol ESP32 ke driver motor sesuai konfigurasi.
3. Hubungkan motor kiri ke output motor kiri driver.
4. Hubungkan motor kanan ke output motor kanan driver.
5. Hubungkan supply motor ke driver motor.
6. Hubungkan GND baterai motor ke GND ESP32.
7. Angkat roda robot dari lantai.
8. Hubungkan USB ESP32.

## 10.3 Langkah Program

1. Buka folder `03Motor`.
2. Build dan upload.
3. Buka Serial Monitor.
4. Amati urutan gerak motor.
5. Pastikan motor kiri dan kanan dapat maju, mundur, berhenti.
6. Pastikan PWM rendah membuat motor pelan dan PWM tinggi membuat motor cepat.

## 10.4 Koreksi Arah Motor

Jika motor kiri terbalik, ubah di `config.h`:

```
#define MOTOR_LEFT_INVERT true
```

Jika motor kanan terbalik:

```
#define MOTOR_RIGHT_INVERT true
```

Atau tukar kabel motor pada output driver.

## 10.5 Hasil yang Diharapkan

- Motor kiri dapat dikendalikan.
- Motor kanan dapat dikendalikan.
- Robot maju ketika kedua motor diberi speed positif.
- Robot mundur ketika kedua motor diberi speed negatif.
- Robot berbelok ketika speed kiri dan kanan berbeda.

## 10.6 Data yang Dicatat

| Uji | Speed kiri | Speed kanan | Hasil |
|---|---:|---:|---|
| Stop | 0 | 0 | |
| Maju pelan | 80 | 80 | |
| Maju sedang | 150 | 150 | |
| Maju cepat | 220 | 220 | |
| Belok kiri | 80 | 150 | |
| Belok kanan | 150 | 80 | |
| Putar tempat | 120 | -120 | |

---

# 11. Praktikum 4: Uji Sensor Garis

Folder:

```
Program/04Sensor
```

## 11.1 Tujuan

Membaca nilai analog sensor, menentukan threshold, membaca status digital, dan menghitung posisi garis.

## 11.2 Langkah Kerja

1. Pasang sensor array di depan robot.
2. Pastikan sensor sejajar dengan lintasan.
3. Hubungkan sensor ke pin sesuai `config.h`.
4. Buka folder `04Sensor`.
5. Build dan upload.
6. Buka Serial Monitor.
7. Letakkan semua sensor di atas permukaan putih.
8. Catat nilai ADC tiap sensor.
9. Letakkan semua sensor di atas garis hitam.
10. Catat nilai ADC tiap sensor.
11. Hitung threshold.
12. Ubah `SENSOR_THRESHOLD` jika diperlukan.
13. Uji robot di atas lintasan dan amati posisi garis.

## 11.3 Tabel Kalibrasi

| Sensor | ADC putih | ADC hitam | Threshold sensor |
|---|---:|---:|---:|
| S0 | | | |
| S1 | | | |
| S2 | | | |
| S3 | | | |
| S4 | | | |
| S5 | | | |
| S6 | | | |
| S7 | | | |

Threshold sederhana:

```
Threshold = (ADC_putih + ADC_hitam) / 2
```

Jika ingin satu threshold global:

```
Threshold_global = rata-rata semua Threshold sensor
```

## 11.4 Uji Posisi Garis

Letakkan garis pada posisi berikut dan catat position:

| Posisi garis | Sensor aktif ideal | Position ideal |
|---|---|---:|
| Paling kiri | S0 | 0 |
| Kiri sedang | S1-S2 | 1500 |
| Tengah | S3-S4 | 3500 |
| Kanan sedang | S5-S6 | 5500 |
| Paling kanan | S7 | 7000 |

## 11.5 Hasil yang Diharapkan

- Nilai ADC putih dan hitam berbeda jelas.
- Status digital berubah sesuai threshold.
- Position mendekati 3500 saat garis berada di tengah robot.
- Position -1 saat garis tidak terdeteksi.

---

# 12. Praktikum 5: Kontrol PID Line Follower

Folder:

```
Program/05PID
```

## 12.1 Tujuan

Menjalankan robot line follower menggunakan kontrol PID.

## 12.2 Persiapan

1. Pastikan OLED berhasil.
2. Pastikan tombol berhasil.
3. Pastikan motor berhasil dan arah benar.
4. Pastikan sensor berhasil dan threshold benar.
5. Siapkan lintasan garis sederhana.
6. Pastikan baterai motor cukup.
7. Letakkan robot di garis start.

## 12.3 Langkah Kerja

1. Buka folder `05PID`.
2. Cek parameter awal di `config.h`:

```
#define PID_KP          0.08
#define PID_KI          0.0
#define PID_KD          0.4
#define PID_BASE_SPEED  150
```

3. Build dan upload.
4. Buka Serial Monitor.
5. Letakkan robot di atas lintasan.
6. Jalankan mode PID.
7. Amati robot mengikuti garis.
8. Amati OLED untuk position, error, correction, speed, dan parameter PID.
9. Jika robot terlalu liar, turunkan baseSpeed.
10. Jika robot lambat merespons, naikkan Kp sedikit.
11. Jika robot zig-zag, naikkan Kd sedikit atau turunkan Kp.
12. Catat parameter terbaik.

## 12.4 Rumus yang Digunakan

```
error = position - 3500
integral = integral + error x dt
derivative = (error - lastError) / dt
correction = Kp x error + Ki x integral + Kd x derivative
leftSpeed = baseSpeed + correction
rightSpeed = baseSpeed - correction
```

## 12.5 Checklist Sebelum Tuning

- [ ] Hardware sudah dirakit dan wiring sudah dicek.
- [ ] Baterai terisi penuh (min 7.4V).
- [ ] Sensor sudah dikalibrasi (Program 04).
- [ ] Threshold sudah tepat.
- [ ] Motor sudah ditest individual (Program 03).
- [ ] OLED dan button berfungsi.
- [ ] Lintasan test sudah disiapkan.
- [ ] Tabel tuning sudah disiapkan.

## 12.6 Prosedur Tuning Sistematis (Panduan Lengkap)

### Nilai Awal (Starting Point)

```cpp
float Kp = 0.05;      // Mulai dari sini
float Ki = 0.0;       // Tambahkan terakhir (optional)
float Kd = 0.0;       // Tambahkan setelah Kp
int baseSpeed = 120;  // Kecepatan dasar
```

> Nilai ini untuk sensor 8 channel dengan setpoint 3500 dan error range ±3500.

### Langkah A: Tuning Kp (Proportional Only)

Tujuan: Temukan Kp yang membuat robot mengikuti garis dengan osilasi kecil.

1. Set Ki = 0 dan Kd = 0.
2. Mulai Kp = 0.05, baseSpeed = 120.
3. Upload dan letakkan robot di garis.
4. Amati perilaku robot:

| Perilaku Robot | Diagnosis | Aksi |
|---|---|---|
| Robot zig-zag lambat, sering keluar | Kp terlalu kecil | Naikkan Kp +0.01 |
| Robot zig-zag cepat, tidak stabil | Kp terlalu besar | Turunkan Kp -0.01 |
| Robot mengikuti garis, osilasi kecil | Kp sudah bagus | Lanjut Langkah B |

5. Ulangi sampai robot mengikuti garis di lintasan lurus.

**Target**: Robot mengikuti garis di lintasan lurus dengan osilasi dapat diterima.

### Langkah B: Tuning Kd (Tambah Derivative)

Tujuan: Meredam osilasi dan meningkatkan stabilitas di tikungan.

1. Pertahankan Kp dari Langkah A.
2. Mulai Kd = 0.5.
3. Upload dan test di lintasan dengan tikungan.
4. Amati perilaku:

| Perilaku Robot | Diagnosis | Aksi |
|---|---|---|
| Masih zig-zag di tikungan | Kd terlalu kecil | Naikkan Kd +0.5 |
| Robot lambat bereaksi, keluar tikungan tajam | Kd terlalu besar | Turunkan Kd -0.2 |
| Robot stabil di lurus dan tikungan | Kd optimal | Selesai (atau Langkah C) |

**Target**: Robot mengikuti garis dengan smooth, stabil di tikungan.

### Langkah C: Tuning Ki (Optional — hanya jika ada offset)

Kapan perlu Ki:
- Robot selalu offset ke satu sisi (kiri atau kanan).
- Robot tidak bisa kembali ke center setelah tikungan.
- Ada steady-state error persisten.

Jika tidak ada masalah di atas, **SKIP langkah ini (Ki = 0)**.

1. Mulai Ki = 0.001.
2. Naikkan +0.001 jika masih offset.
3. Turunkan jika robot overshoot atau tidak stabil.

**PERINGATAN**: Ki terlalu besar membuat robot tidak stabil!

### Langkah D: Tuning Base Speed

1. Mulai dari 120.
2. Naikkan +10 sampai robot mulai tidak stabil.
3. Turunkan -5 untuk safety margin.

### Contoh Kasus Tuning Pemula

| Iterasi | Kp | Ki | Kd | Speed | Hasil |
|---:|---:|---:|---:|---:|---|
| 1 | 0.05 | 0 | 0 | 120 | Lambat, keluar di tikungan |
| 2 | 0.08 | 0 | 0 | 120 | Zig-zag cepat |
| 3 | 0.08 | 0 | 1.0 | 120 | Stabil di lurus, goyang tikungan |
| 4 | 0.08 | 0 | 1.5 | 120 | Sangat stabil |
| 5 | 0.08 | 0 | 1.5 | 130 | Stabil dan cepat |

Parameter final: Kp=0.08, Ki=0, Kd=1.5, Speed=130

### Contoh Perhitungan Manual PID

Kondisi: Kp=0.08, Kd=1.5, Ki=0, baseSpeed=130, position=1000, lastError=-2000

```
error = 1000 - 3500 = -2500
P = 0.08 × (-2500) = -200
D = 1.5 × (-2500 - (-2000)) = 1.5 × (-500) = -750
correction = -200 + (-750) = -950
leftSpeed  = 130 + (-950) = -820 → constrain → 0
rightSpeed = 130 - (-950) = 1080 → constrain → 255
```

Interpretasi: Robot di kanan garis → motor kiri stop, motor kanan full → belok kiri kembali ke tengah.

### Nilai Referensi PID

| Konfigurasi | Kp | Ki | Kd | Speed |
|---|---:|---:|---:|---:|
| Pemula (lintasan mudah) | 0.05–0.08 | 0 | 0.5–1.5 | 100–130 |
| Menengah (lintasan sedang) | 0.08–0.12 | 0 | 1.0–2.0 | 130–160 |
| Advanced (kompetisi) | 0.10–0.15 | 0–0.002 | 1.5–3.0 | 150–200 |

## 12.7 Tabel Tuning

| Percobaan | Kp | Ki | Kd | Base speed | Hasil | Catatan |
|---|---:|---:|---:|---:|---|---|
| 1 | | | | | | |
| 2 | | | | | | |
| 3 | | | | | | |
| 4 | | | | | | |
| 5 | | | | | | |

## 12.8 Uji Performa

Lakukan minimal 3 lap untuk parameter terbaik.

| Lap | Waktu tempuh | Keluar lintasan | Catatan |
|---|---:|---:|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |

Hitung rata-rata waktu:

```
t_avg = (t1 + t2 + t3) / 3
```

---

# 13. Praktikum 6: Path Planning

Folder:

```
Program/06PathPlanning
```

## 13.1 Tujuan

Menjalankan line follower dengan urutan keputusan pada persimpangan menggunakan sensor mask dan path point.

## 13.2 Konsep

Robot tetap mengikuti garis dengan kontrol dasar, tetapi pada pola sensor tertentu robot melakukan aksi khusus.

Contoh path point:

- Sensor 7 aktif: belok kanan.
- Sensor 0 aktif: belok kiri.
- Sensor 0 dan 7 aktif: lurus pada persimpangan T.
- Semua sensor aktif: berhenti.

## 13.3 Langkah Kerja

1. Siapkan lintasan dengan persimpangan.
2. Buka folder `06PathPlanning`.
3. Build dan upload.
4. Buka Serial Monitor.
5. Amati path contoh yang otomatis ditambahkan.
6. Gunakan tombol untuk menambah, menjalankan, menghentikan, atau melihat path.
7. Jalankan robot dari start.
8. Amati apakah robot melakukan aksi sesuai urutan.
9. Jika aksi terlalu cepat atau terlambat, ubah duration.
10. Jika trigger salah, ubah sensorMask.

## 13.4 Sensor Mask

Contoh:

| Kondisi | Mask |
|---|---|
| Sensor kiri S0 | `0b00000001` |
| Sensor kanan S7 | `0b10000000` |
| Tengah S3 dan S4 | `0b00011000` |
| Semua sensor | `0b11111111` |
| Tidak ada sensor | `0b00000000` |

Deteksi:

```
(currentMask & targetMask) == targetMask
```

## 13.5 Tabel Path

| Index | Base speed | Mask 1 | Mask 2 | Aksi | Durasi | Deskripsi |
|---:|---:|---|---|---|---:|---|
| 0 | 150 | 0b10000000 | 0 | RIGHT | 800 | Kanan di S7 |
| 1 | 150 | 0b00000001 | 0 | LEFT | 800 | Kiri di S0 |
| 2 | 120 | 0b10000000 | 0b00000001 | STRAIGHT | 0 | Lurus T |
| 3 | 100 | 0b11111111 | 0 | STOP | 0 | Finish |

## 13.6 Hasil yang Diharapkan

- Robot dapat membaca persimpangan.
- Robot melakukan aksi sesuai urutan path.
- Robot berhenti pada marker finish.
- Praktikan memahami hubungan sensor mask, aksi, dan durasi.

---

# 14. Evaluasi Praktikum

## 14.1 Checklist Keberhasilan

| No | Kriteria | Ya/Tidak | Bukti |
|---:|---|---|---|
| 1 | OLED tampil normal | | Foto/video |
| 2 | Tombol terbaca benar | | Serial/OLED |
| 3 | Motor kiri dan kanan bergerak benar | | Video |
| 4 | Sensor membaca hitam-putih | | Tabel ADC |
| 5 | Threshold ditentukan dari data | | Perhitungan |
| 6 | Posisi garis terbaca | | Serial/OLED |
| 7 | PID berjalan | | Video robot |
| 8 | Tuning dilakukan minimal 5 percobaan | | Tabel tuning |
| 9 | Robot mengikuti lintasan | | Video |
| 10 | Path planning diuji | | Video dan tabel path |

## 14.2 Pertanyaan Analisis

1. Mengapa ground bersama diperlukan?
2. Mengapa sensor perlu dikalibrasi?
3. Apa pengaruh threshold terlalu tinggi?
4. Apa pengaruh threshold terlalu rendah?
5. Mengapa PID lebih halus daripada kontrol on-off?
6. Apa akibat Kp terlalu besar?
7. Apa fungsi Kd pada line follower?
8. Kapan Ki perlu digunakan?
9. Mengapa baseSpeed memengaruhi keberhasilan tikungan?
10. Bagaimana sensor mask digunakan untuk membaca persimpangan?

---

# 15. Troubleshooting Lengkap

## 15.1 Hardware: OLED Tidak Muncul / Blank

**Gejala**: LCD OLED tidak menampilkan apa-apa atau hanya putih/hitam.

Checklist:
- Cek wiring: SDA → GPIO 33, SCL → GPIO 35, VCC → **3.3 V** (bukan 5 V), GND → GND.
- Cek alamat I2C: biasanya `0x3C` atau `0x3D`.
- Test dengan I2C Scanner:

```cpp
#include <Wire.h>
void setup() {
  Serial.begin(115200);
  Wire.begin(33, 35);
}
void loop() {
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device found at 0x");
      Serial.println(addr, HEX);
    }
  }
  delay(5000);
}
```

- Jika gagal `0x3C`, coba `0x3D` di kode.
- Turunkan clock I2C jika perlu: `Wire.setClock(100000);`

## 15.2 Hardware: Motor Tidak Bergerak

Checklist:
- Cek supply baterai motor (min 7.4V dengan multimeter).
- Cek wiring EN, IN1, IN2 sesuai `config.h`.
- Cek GND bersama: GND ESP32, GND driver motor, GND baterai harus terhubung.
- Cek LED indikator driver motor.
- Test motor bypass driver: hubungkan motor langsung ke baterai sebentar.

Solusi kode — test motor individual:

```cpp
ledcWrite(PWM_CHANNEL_L, 150);
digitalWrite(MOTOR_LEFT_IN1, HIGH);
digitalWrite(MOTOR_LEFT_IN2, LOW);
delay(2000);
ledcWrite(PWM_CHANNEL_L, 0);
```

## 15.3 Hardware: Sensor Tidak Stabil / Nilai Acak

Checklist:
- Kalibrasi ulang sensor (Program 04).
- Jarak sensor ke lantai: optimal **5–10 mm**.
- Bersihkan sensor dari debu.
- Hindari cahaya matahari langsung.
- Tambahkan moving average filter:

```cpp
#define FILTER_SIZE 5
uint16_t hist[SENSOR_COUNT][FILTER_SIZE];
int histIdx = 0;
void readSensorsFiltered(uint16_t* out) {
  for (int i = 0; i < SENSOR_COUNT; i++)
    hist[i][histIdx] = analogRead(sensorPins[i]);
  histIdx = (histIdx + 1) % FILTER_SIZE;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    uint32_t sum = 0;
    for (int j = 0; j < FILTER_SIZE; j++) sum += hist[i][j];
    out[i] = sum / FILTER_SIZE;
  }
}
```

## 15.4 Hardware: Push Button Tidak Responsif

Checklist:
- Satu kaki tombol ke GPIO, kaki lain ke GND.
- Gunakan `INPUT_PULLUP`.
- Tambahkan debounce 50 ms.

```cpp
bool readButton(int pin) {
  static unsigned long t = 0;
  static bool last = HIGH;
  bool cur = digitalRead(pin);
  if (cur == LOW && last == HIGH && millis() - t > 50) {
    t = millis(); last = cur; return true;
  }
  last = cur; return false;
}
```

## 15.5 Software: Upload Gagal

Error: "Failed to connect", "Timed out", "Serial port not found".

Langkah perbaikan:
1. Gunakan kabel USB **data** (bukan charge only).
2. Pilih port yang benar di PlatformIO.
3. Tutup Serial Monitor sebelum upload.
4. Tekan dan tahan BOOT → klik Upload → lepas BOOT setelah "Connecting...".
5. Linux: `sudo usermod -a -G dialout $USER` lalu logout-login ulang.
6. Cek `platformio.ini`: board harus `lolin_s2_mini`.
7. Manual boot mode: tahan BOOT → tekan RESET → lepas RESET → lepas BOOT → upload.

## 15.6 Software: Library Not Found

Error: `fatal error: Adafruit_SSD1306.h: No such file or directory`

```bash
pio lib install "Adafruit SSD1306"
pio lib install "Adafruit GFX Library"
pio run -t clean
pio run
```

Atau tambahkan di `platformio.ini`:
```ini
lib_deps =
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit GFX Library@^1.11.3
```

## 15.7 Software: Serial Monitor Blank

- Pastikan baudrate **115200** di Serial Monitor dan kode.
- Tunggu beberapa detik setelah upload.
- Tekan tombol RESET di ESP32.

## 15.8 Performa: Robot Keluar Lintasan di Tikungan

Penyebab dan solusi:

| Penyebab | Solusi |
|---|---|
| Base speed terlalu tinggi | Turunkan ke 120 |
| Kd terlalu kecil | Naikkan Kd +0.5 |
| Threshold sensor tidak tepat | Kalibrasi ulang (Program 04) |
| Sensor terlalu jauh dari lantai | Adjust ke 5–10 mm |
| Center of gravity tidak balance | Pindahkan baterai ke tengah |

## 15.9 Performa: Robot Zig-Zag Terus Menerus

| Penyebab | Solusi |
|---|---|
| Kp terlalu besar | Turunkan Kp -0.01 |
| Kd terlalu kecil | Naikkan Kd +0.5 |
| Sensor noise | Tambah smoothing filter |
| Sensor tidak terkalibrasi | Kalibrasi ulang |

Tambahkan smoothing sederhana:
```cpp
float smoothedError = 0.7 * error + 0.3 * lastError;
```

## 15.10 Performa: Robot Lambat Bereaksi

| Penyebab | Solusi |
|---|---|
| Kp terlalu kecil | Naikkan Kp +0.01 |
| Kd terlalu besar | Turunkan Kd -0.2 |
| Base speed terlalu rendah | Naikkan speed |
| Delay terlalu besar di loop | Kurangi `delay()` |

## 15.11 Performa: Robot Selalu Offset ke Satu Sisi

| Penyebab | Solusi |
|---|---|
| Sensor tidak center | Adjust posisi mounting sensor |
| Motor tidak balance | Tambahkan offset kalibrasi |
| Threshold tidak tepat | Kalibrasi ulang sensor |
| Perlu integral | Tambahkan Ki = 0.001 |

```cpp
#define MOTOR_RIGHT_OFFSET 0.95  // Koreksi jika kanan lebih kuat
void setMotors(int l, int r) {
  r = (int)(r * MOTOR_RIGHT_OFFSET);
  // ... set motor
}
```

## 15.12 Performa: Robot Berhenti / Stuck (Lost Line)

```cpp
bool allWhite() {
  for (int i = 0; i < SENSOR_COUNT; i++)
    if (sensorDigital[i]) return false;
  return true;
}
if (allWhite()) {
  lostCounter++;
  if (lostCounter > 10) {
    setMotors(lastError < 0 ? 50 : 150,
              lastError < 0 ? 150 : 50);
  }
} else { lostCounter = 0; }
```

Juga pastikan speed tidak negatif:
```cpp
leftSpeed  = constrain(leftSpeed,  0, 255);
rightSpeed = constrain(rightSpeed, 0, 255);
```

## 15.13 Performa: Robot Berputar di Tempat

Penyebab:
1. Satu motor terbalik — tukar kabel motor atau atur `MOTOR_X_INVERT true`.
2. Sensor membaca posisi salah — cek threshold.
3. Konfigurasi invert salah di `config.h`.

## 15.14 Power: Baterai Cepat Habis / Robot Restart Sendiri

- Gunakan baterai minimal 1000 mAh (lebih baik 2000 mAh).
- Tambahkan kapasitor 1000 µF di VCC driver motor untuk reduksi voltage drop.
- Pisahkan supply ESP32 dan motor untuk menghindari brownout:
  - Baterai 7.4 V untuk motor.
  - Regulator 3.3 V terpisah untuk ESP32.
- Soft start motor untuk kurangi inrush current:

```cpp
void softStart(int targetL, int targetR) {
  for (int s = 0; s <= max(targetL, targetR); s += 10) {
    setMotors(min(s, targetL), min(s, targetR));
    delay(30);
  }
}
```

## 15.15 Quick Reference: Checklist Sebelum Run

- [ ] Baterai ≥ 7.4 V.
- [ ] Semua wiring dicek.
- [ ] Sensor dikalibrasi.
- [ ] Motor test OK.
- [ ] OLED dan button berfungsi.
- [ ] Lintasan test siap.
- [ ] Kabel tidak menghalangi roda.

## 15.16 Emergency Procedures

1. **Robot tidak terkontrol**: tekan emergency stop, angkat robot, atau cabut baterai.
2. **Smoke atau bau terbakar**: matikan power segera, cabut baterai, cek short circuit.
3. **Upload error berulang**: restart ESP32, restart laptop, ganti kabel USB, ganti port USB.


# 16. Output Praktikum

Setiap kelompok/praktikan harus menghasilkan:

1. Video proses praktik dari awal sampai akhir.
2. Screen record penjelasan teori dasar dan program.
3. Link YouTube masing-masing.
4. Tabel kalibrasi sensor.
5. Tabel tuning PID.
6. Bukti robot berhasil running.
7. Penjelasan singkat kendala dan solusi pada video.

Deadline tugas: 2 minggu setelah praktikum.

---

# 17. Format Catatan Data Praktikum

## 17.1 Data Sensor

| Sensor | Putih | Hitam | Threshold |
|---|---:|---:|---:|
| S0 | | | |
| S1 | | | |
| S2 | | | |
| S3 | | | |
| S4 | | | |
| S5 | | | |
| S6 | | | |
| S7 | | | |

## 17.2 Data PID

| No | Kp | Ki | Kd | Speed | Perilaku Robot |
|---:|---:|---:|---:|---:|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |
| 5 | | | | | |

## 17.3 Data Path Planning

| No | Trigger sensor | Mask | Aksi | Durasi | Berhasil/Tidak |
|---:|---|---|---|---:|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |

---

# 18. Kriteria Praktikum Berhasil

Praktikum dinyatakan berhasil jika:

1. Praktikan dapat menjelaskan fungsi setiap komponen.
2. OLED dapat menampilkan status.
3. Tombol dapat dibaca dengan benar.
4. Motor dapat dikendalikan maju, mundur, dan berhenti.
5. Sensor dapat membedakan garis dan latar.
6. Posisi garis dapat dihitung.
7. Robot dapat mengikuti garis dengan PID.
8. Praktikan melakukan tuning parameter.
9. Robot dapat menjalankan minimal satu aksi path planning.
10. Praktikan mendokumentasikan proses dalam video sesuai instruksi tugas.
