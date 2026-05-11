# PROJECT — Modul 10: Line Follower Robot

**Mata Kuliah**: Praktikum Mekatronika dan Robotika  
**Program Studi**: Sarjana Terapan Teknologi Rekayasa Otomasi  
**Dosen Pengampu**: Rofiq Cahyo Prayogo, S.T., M.T.  
**SKS / Durasi**: 2 SKS / 5 jam 40 menit  
**Deadline**: 2 minggu setelah praktikum

---

## Petunjuk Umum

Pilih **satu** proyek dari 10 pilihan di bawah. Proyek dikerjakan dalam kelompok (2–3 orang) sesuai pembagian dosen. Setiap proyek harus didokumentasikan dalam video YouTube yang menampilkan proses pengembangan, pengujian, hasil akhir, dan analisis.

**Tingkat kesulitan:**
- ⭐ Pemula — dasar, selesai dalam 1 sesi
- ⭐⭐ Menengah — butuh modifikasi kode
- ⭐⭐⭐ Lanjutan — butuh desain dan integrasi baru
- ⭐⭐⭐⭐ Kompetisi — standar kompetisi robotika

---

## Proyek 1 — Line Follower Kecepatan Optimal ⭐

### Deskripsi
Optimalkan parameter PID sehingga robot dapat menyelesaikan lintasan oval/lingkaran dalam waktu tercepat dengan **zero exit** (tidak keluar lintasan selama 3 lap berturut-turut).

### Tujuan Pembelajaran
- Memahami trade-off antara kecepatan dan stabilitas.
- Menguasai teknik tuning PID secara sistematis.
- Membaca dan menganalisis data Serial Monitor.

### Spesifikasi Lintasan
- Lintasan oval/lingkaran sederhana, lebar garis 2 cm.
- Total panjang lintasan ≥ 2 meter.

### Spesifikasi Robot
- Menggunakan firmware **05PID** dari folder Program.
- Parameter harus di-tuning (tidak boleh default).

### Langkah Pengerjaan
1. Tuning PID di lintasan sederhana (lurus dan tikungan).
2. Catat setiap percobaan dalam tabel tuning (min. 10 percobaan).
3. Uji robot di lintasan oval, ukur waktu 5 lap.
4. Optimalkan parameter untuk kecepatan maksimal.
5. Uji final: 3 lap berturut-turut tanpa keluar lintasan.
6. Dokumentasikan video percobaan dan hasil akhir.

### Output yang Dihasilkan
- Tabel tuning (min. 10 baris).
- Grafik waktu vs percobaan.
- Video 3 lap tanpa exit.
- Analisis parameter terbaik.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| Robot menyelesaikan 3 lap tanpa exit | 40% |
| Tabel tuning sistematis ≥ 10 percobaan | 25% |
| Analisis pengaruh parameter | 20% |
| Kualitas video dan dokumentasi | 15% |

---

## Proyek 2 — Line Follower dengan Adaptive Speed ⭐⭐

### Deskripsi
Implementasikan kontrol kecepatan adaptif: robot berjalan cepat di lintasan lurus dan otomatis memperlambat di tikungan berdasarkan nilai error PID.

### Tujuan Pembelajaran
- Memahami hubungan error PID dengan geometri lintasan.
- Implementasi logika kondisional dalam sistem kontrol.
- Analisis performa dengan variabel kecepatan.

### Konsep Kunci
```cpp
// Kecepatan adaptif berdasarkan error absolut
int absError = abs(error);
int adaptSpeed;
if (absError < 500)       adaptSpeed = 200;  // Lurus: cepat
else if (absError < 1500) adaptSpeed = 160;  // Tikungan ringan
else if (absError < 2500) adaptSpeed = 120;  // Tikungan sedang
else                      adaptSpeed = 90;   // Tikungan tajam
```

### Langkah Pengerjaan
1. Modifikasi firmware 05PID untuk menambah logika adaptive speed.
2. Definisikan 3–4 zona kecepatan berdasarkan threshold error.
3. Uji di lintasan dengan variasi tikungan.
4. Bandingkan performa dengan fixed speed (waktu dan stabilitas).
5. Dokumentasikan perbedaan perilaku robot.

### Output yang Dihasilkan
- Kode modifikasi dengan komentar jelas.
- Video perbandingan fixed vs adaptive speed.
- Tabel perbandingan waktu lap dan jumlah exit.
- Analisis trade-off kecepatan vs stabilitas.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| Implementasi adaptive speed berjalan | 35% |
| Perbandingan data fixed vs adaptive | 25% |
| Analisis kelebihan/kekurangan | 20% |
| Kualitas kode dan dokumentasi | 20% |

---

## Proyek 3 — Line Follower dengan Mode Pilihan ⭐⭐

### Deskripsi
Tambahkan 3 mode operasi yang dapat dipilih melalui push button:
- **Mode A** (B1): Kecepatan rendah, stabil (untuk pemula).
- **Mode B** (B2): Kecepatan sedang, seimbang (default).
- **Mode C** (B3): Kecepatan tinggi, agresif (untuk kompetisi).

### Tujuan Pembelajaran
- Implementasi state machine berbasis tombol.
- Manajemen konfigurasi multi-mode.
- Penggunaan OLED untuk feedback mode aktif.

### Spesifikasi Teknis
```cpp
struct DriveMode {
  const char* name;
  float kp, ki, kd;
  int baseSpeed;
};

DriveMode modes[3] = {
  {"SLOW",   0.05, 0, 0.8,  100},
  {"NORMAL", 0.08, 0, 1.5,  140},
  {"FAST",   0.12, 0, 2.0,  180},
};
int activeMode = 1;
```

### Langkah Pengerjaan
1. Tambahkan struct `DriveMode` dan array 3 mode.
2. Implementasikan logika pemilihan mode dengan B1/B2/B3.
3. Tampilkan mode aktif di OLED.
4. Tuning parameter untuk setiap mode.
5. Uji setiap mode di lintasan yang sama.
6. Dokumentasikan perbedaan perilaku ketiga mode.

### Output yang Dihasilkan
- Kode dengan 3 mode operasi berfungsi.
- OLED menampilkan mode dan parameter aktif.
- Video demo ketiga mode di lintasan yang sama.
- Tabel perbandingan waktu dan stabilitas per mode.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| 3 mode berfungsi dan dapat dipilih | 35% |
| OLED menampilkan informasi mode | 20% |
| Parameter setiap mode sudah di-tuning | 25% |
| Perbandingan performa ketiga mode | 20% |

---

## Proyek 4 — Line Follower dengan Pencatatan Data Otomatis ⭐⭐

### Deskripsi
Robot mencatat **data telemetri** secara real-time melalui Serial dan menyimpannya untuk analisis performa. Data mencakup: waktu, posisi, error, koreksi, kecepatan kiri/kanan.

### Tujuan Pembelajaran
- Implementasi logging data berbasis Serial.
- Analisis data performa menggunakan spreadsheet.
- Visualisasi error dan koreksi PID.

### Format Output Serial
```
TIME,POS,ERROR,P,D,CORR,SPD_L,SPD_R
100,3450,-50,-4.0,-75,-79,209,130
200,3510,10,0.8,90,91,130,220
```

### Langkah Pengerjaan
1. Tambahkan fungsi `logData()` yang mencetak CSV ke Serial.
2. Log setiap 100 ms (gunakan `millis()`).
3. Rekam satu lap data menggunakan Serial Monitor → Copy → Paste ke Excel.
4. Buat grafik: Error vs Waktu, Speed vs Waktu.
5. Identifikasi titik kritis (tikungan, overshoot).
6. Hubungkan analisis grafik dengan parameter PID.

### Output yang Dihasilkan
- Kode logging berfungsi, format CSV.
- File CSV data satu lap (minimal).
- Grafik Error vs Waktu dan Speed vs Waktu.
- Analisis titik kritis berdasarkan grafik.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| Logging data CSV berfungsi | 30% |
| Grafik dari data real robot | 30% |
| Analisis titik kritis dan korelasinya | 25% |
| Kualitas video dan dokumentasi | 15% |

---

## Proyek 5 — Line Follower dengan EEPROM Tuning Interface ⭐⭐

### Deskripsi
Buat antarmuka tuning interaktif menggunakan tombol dan OLED: parameter Kp, Ki, Kd, dan baseSpeed dapat diubah langsung di robot **tanpa perlu upload ulang**, dan tersimpan di EEPROM.

### Tujuan Pembelajaran
- Penggunaan EEPROM untuk persistensi konfigurasi.
- Desain antarmuka pengguna (UI) berbasis tombol.
- Real-time parameter adjustment.

### Spesifikasi UI
```
OLED tampilan:
+----------------------+
| TUNING MODE          |
| > Kp: 0.080 [EDIT]   |
|   Kd: 1.500          |
| B1:Up B2:Dn B3:Next  |
+----------------------+
```

### Langkah Pengerjaan
1. Buat menu tuning berbasis state machine (layar navigasi parameter).
2. B1 = naikkan nilai, B2 = turunkan nilai, B3 = pindah parameter, B4 = simpan/run.
3. Simpan parameter ke EEPROM setiap kali B4 ditekan.
4. Load dari EEPROM saat robot dinyalakan.
5. Uji tuning langsung tanpa laptop.
6. Demonstrasikan proses tuning dan simpan 3 konfigurasi berbeda.

### Output yang Dihasilkan
- UI tuning berfungsi dengan 4 tombol.
- Parameter tersimpan di EEPROM.
- Demonstrasi tuning mandiri (tanpa laptop).
- Perbandingan 3 konfigurasi tersimpan.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| UI tuning berfungsi (navigasi + edit) | 35% |
| EEPROM save/load berfungsi | 25% |
| Demonstrasi tuning mandiri (tanpa laptop) | 25% |
| Kualitas UI dan kemudahan penggunaan | 15% |

---

## Proyek 6 — Line Follower dengan Path Planning Otomatis ⭐⭐⭐

### Deskripsi
Robot harus menavigasi lintasan dengan **4 persimpangan** secara otomatis menggunakan path planning berbasis sensor mask. Robot harus menyelesaikan rute yang ditentukan (bukan asal belok).

### Tujuan Pembelajaran
- Desain path planning berbasis sensor mask.
- Implementasi state machine kompleks.
- Debugging navigasi robot nyata.

### Spesifikasi Lintasan
- Lintasan dengan minimal 4 persimpangan:
  - 2 persimpangan T
  - 1 persimpangan silang
  - 1 marker finish (semua sensor aktif)
- Robot harus menyelesaikan rute: Kanan → Kiri → Lurus → Stop.

### Konfigurasi Path (contoh)
```cpp
PathPoint path[] = {
  {150, 0b10000000, 0,          RIGHT,    800},  // Kanan di S7
  {150, 0b00000001, 0,          LEFT,     800},  // Kiri di S0
  {130, 0b10000000, 0b00000001, STRAIGHT, 0},    // Lurus di T
  {0,   0b11111111, 0,          STOP,     0},    // Finish
};
```

### Langkah Pengerjaan
1. Desain lintasan dengan 4 persimpangan di atas kertas.
2. Buat fisik lintasan menggunakan lakban hitam.
3. Kalibrasi sensor.
4. Program path sesuai urutan persimpangan.
5. Debug dengan Serial Monitor sampai robot sukses.
6. Uji 3 kali berturut-turut.

### Output yang Dihasilkan
- Foto/sketsa desain lintasan.
- Kode path planning dengan komentar.
- Tabel sensor mask untuk setiap persimpangan.
- Video robot menyelesaikan rute 3 kali.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| Robot menyelesaikan semua 4 titik path | 40% |
| Desain lintasan terstruktur | 20% |
| Dokumentasi sensor mask | 20% |
| Keberhasilan 3 kali berturut-turut | 20% |

---

## Proyek 7 — Line Follower dengan Anti-Lost Line System ⭐⭐⭐

### Deskripsi
Implementasikan sistem **pemulihan otomatis** saat robot kehilangan garis (lost line). Robot harus bisa menemukan kembali garis dalam 3 skenario: keluar kiri, keluar kanan, dan garis terputus.

### Tujuan Pembelajaran
- Analisis kasus edge condition dalam kontrol robot.
- Implementasi state machine untuk recovery.
- Pengujian sistem dengan skenario terstruktur.

### Spesifikasi Recovery System
```cpp
enum LostMode { NONE, SEARCH_LEFT, SEARCH_RIGHT, SLOW_FORWARD };

// Logika recovery:
// 1. Jika semua sensor putih > 200ms:
//    - lastError < 0 → putar kiri (cari ke kiri)
//    - lastError > 0 → putar kanan (cari ke kanan)
// 2. Jika masih tidak ketemu > 1 detik → maju pelan
// 3. Jika masih tidak ketemu > 3 detik → STOP
```

### Langkah Pengerjaan
1. Implementasikan state machine lost line dengan 3 mode recovery.
2. Gunakan `lastError` untuk menentukan arah pencarian.
3. Uji 3 skenario: robot sengaja diangkat di kiri, kanan, dan tengah.
4. Catat waktu recovery untuk setiap skenario.
5. Optimalkan threshold waktu dan kecepatan putar.
6. Dokumentasikan video setiap skenario.

### Output yang Dihasilkan
- Kode recovery state machine dengan komentar.
- Video 3 skenario lost line dan recovery-nya.
- Tabel waktu recovery per skenario.
- Analisis keandalan sistem.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| Recovery berhasil di 3 skenario | 45% |
| Implementasi state machine tepat | 25% |
| Waktu recovery rata-rata < 3 detik | 15% |
| Dokumentasi dan analisis | 15% |

---

## Proyek 8 — Line Follower dengan Display Telemetri OLED ⭐⭐⭐

### Deskripsi
Buat dashboard OLED yang menampilkan **5 layar informasi berbeda** yang dapat di-scroll menggunakan tombol: layar posisi, layar PID, layar kecepatan, layar sensor raw, dan layar statistik (waktu lap, jumlah exit).

### Tujuan Pembelajaran
- Desain UI multi-layar pada display terbatas.
- Implementasi navigasi layar berbasis tombol.
- Statistik performa real-time.

### Spesifikasi 5 Layar OLED
```
Layar 1 — Posisi:       Layar 2 — PID:
POS  : 3450             Kp: 0.080
ERR  : -50              Ki: 0.000 Kd: 1.500
CORR : -83              BASE: 140  MAX: 255
S: ||||  |||||          MODE: PID RUN

Layar 3 — Kecepatan:    Layar 4 — Sensor Raw:
LEFT : 180 PWM          S0:1200 S1:500
RIGHT: 100 PWM          S2:300  S3:100
DIFF : 80               S4:4500 S5:6000
MODE : PID RUN          S6:7200 S7:6800

Layar 5 — Statistik:
LAPS   : 3
BEST   : 12.4s
EXITS  : 1
UPTIME : 00:04:32
```

### Langkah Pengerjaan
1. Buat struct untuk data setiap layar.
2. Implementasikan navigasi layar dengan B3 (next) dan B4 (prev).
3. Update layar setiap 200 ms (gunakan millis).
4. Implementasikan penghitung lap dan statistik.
5. Uji semua 5 layar dan navigasi.
6. Dokumentasikan setiap layar dalam video.

### Output yang Dihasilkan
- Kode 5 layar OLED berfungsi.
- Navigasi tombol berjalan.
- Video demo semua layar saat robot berjalan.
- Analisis informasi paling berguna untuk tuning.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| 5 layar OLED berjalan dengan data real | 40% |
| Navigasi tombol berfungsi | 20% |
| Statistik lap tersimpan | 20% |
| Analisis manfaat setiap layar | 20% |

---

## Proyek 9 — Line Follower dengan Kalibrasi Otomatis ⭐⭐⭐

### Deskripsi
Implementasikan **prosedur kalibrasi sensor otomatis** yang berjalan di awal setiap power-on. Robot bergerak maju-mundur pelan selama 3 detik di atas lintasan untuk mengukur nilai min/max setiap sensor, kemudian menyimpan threshold ke EEPROM.

### Tujuan Pembelajaran
- Algoritma auto-kalibrasi sensor array.
- Normalisasi nilai sensor.
- Validasi threshold kalibrasi di berbagai kondisi pencahayaan.

### Algoritma Kalibrasi
```cpp
uint16_t sensorMin[8], sensorMax[8];
void calibrate() {
  for (int i = 0; i < 8; i++) { sensorMin[i] = 8191; sensorMax[i] = 0; }
  unsigned long start = millis();
  setMotors(80, 80);                        // Maju pelan 1.5 detik
  while (millis() - start < 1500) {
    for (int i = 0; i < 8; i++) {
      uint16_t v = analogRead(sensorPins[i]);
      if (v < sensorMin[i]) sensorMin[i] = v;
      if (v > sensorMax[i]) sensorMax[i] = v;
    }
  }
  setMotors(-80, -80); start = millis();    // Mundur pelan 1.5 detik
  while (millis() - start < 1500) {
    for (int i = 0; i < 8; i++) {
      uint16_t v = analogRead(sensorPins[i]);
      if (v < sensorMin[i]) sensorMin[i] = v;
      if (v > sensorMax[i]) sensorMax[i] = v;
    }
  }
  setMotors(0, 0);
  for (int i = 0; i < 8; i++)
    threshold[i] = (sensorMin[i] + sensorMax[i]) / 2;
  saveThresholdToEEPROM();
}
```

### Langkah Pengerjaan
1. Implementasikan fungsi `calibrate()` di startup.
2. Tampilkan progress kalibrasi di OLED.
3. Validasi hasil: tampilkan min, max, threshold setiap sensor.
4. Simpan ke EEPROM, load saat power-on ulang.
5. Bandingkan akurasi posisi sebelum dan sesudah kalibrasi.
6. Uji pada 3 kondisi pencahayaan berbeda (terang, normal, redup).

### Output yang Dihasilkan
- Kode auto-kalibrasi berfungsi.
- Tabel hasil kalibrasi 3 kondisi pencahayaan.
- Video prosedur kalibrasi otomatis.
- Analisis pengaruh pencahayaan terhadap threshold.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| Auto-kalibrasi berjalan dan akurat | 35% |
| EEPROM save/load threshold | 20% |
| Uji 3 kondisi pencahayaan | 25% |
| Analisis pengaruh pencahayaan | 20% |

---

## Proyek 10 — Line Follower Competition Ready ⭐⭐⭐⭐

### Deskripsi
Bangun line follower **siap kompetisi** dengan semua fitur terintegrasi: auto-kalibrasi, multi-mode PID, path planning, anti-lost line, logging Serial, dan dashboard OLED. Robot harus mampu menyelesaikan lintasan kompetisi (tikungan tajam, garis putus, persimpangan) dengan waktu terbaik.

### Tujuan Pembelajaran
- Integrasi sistem kompleks dan manajemen state machine besar.
- Strategi optimasi performa kompetisi.
- Dokumentasi teknis standar profesional.

### Fitur Wajib

| Fitur | Keterangan |
|---|---|
| Auto-kalibrasi | Berjalan di startup, simpan ke EEPROM |
| Multi-mode | Min. 3 mode (Slow / Normal / Fast) |
| PID optimal | Parameter di-tuning untuk lintasan kompetisi |
| Anti-lost line | Recovery otomatis saat garis hilang |
| Path planning | Min. 3 titik keputusan persimpangan |
| OLED dashboard | Status mode, posisi, kecepatan |
| Start/Stop tombol | B1 start, B4 emergency stop |
| Logging Serial | CSV untuk analisis pasca-run |

### Spesifikasi Lintasan Kompetisi
- Panjang total: ≥ 5 meter.
- Tikungan tajam: min. 2 (radius < 15 cm).
- Garis putus: 1 segmen putus ≤ 5 cm.
- Persimpangan: min. 2.
- Finish marker: semua sensor aktif.

### Arsitektur State Machine
```
POWER_ON → CALIBRATE (3 detik) → MENU (pilih mode)
  → READY (tunggu B1) → RUNNING (PID + path + anti-lost)
  → STOPPED (tampilkan statistik) → MENU (kembali)
```

### Langkah Pengerjaan
1. Buat diagram state machine sebelum coding.
2. Implementasikan setiap modul secara terpisah, uji masing-masing.
3. Integrasi semua modul.
4. Tuning parameter di lintasan latihan.
5. Uji di lintasan kompetisi simulasi.
6. Optimasi berdasarkan data logging.
7. Final test: 3 run di lintasan kompetisi, catat waktu terbaik.

### Output yang Dihasilkan
- Kode terintegrasi dengan semua fitur.
- State machine diagram.
- Data logging CSV satu run kompetisi.
- Video 3 run di lintasan kompetisi.
- Laporan analisis performa dan optimasi.

### Kriteria Keberhasilan
| Kriteria | Nilai |
|---|---|
| Semua fitur wajib berfungsi | 30% |
| Robot menyelesaikan lintasan kompetisi | 25% |
| Data logging dan analisis pasca-run | 20% |
| State machine diagram dan dokumentasi kode | 15% |
| Waktu terbaik 3 run (bonus kecepatan) | 10% |

---

## Rubrik Penilaian Umum

| Aspek | Bobot |
|---|---|
| Fungsionalitas (sesuai spesifikasi proyek) | 50% |
| Kualitas kode (terstruktur, berkomentar) | 20% |
| Analisis dan laporan (video + penjelasan) | 20% |
| Kreativitas dan inovasi tambahan | 10% |

---

## Format Video Proyek

Video diunggah ke YouTube **masing-masing anggota kelompok** dengan judul:

```
[PROYEK-X] NamaProyek — NamaMahasiswa — NIM
```

Contoh: `[PROYEK-5] EEPROM Tuning Interface — Budi Santoso — 2024001234`

Konten video minimal:
1. Perkenalan: nama, NIM, mata kuliah, judul proyek.
2. Penjelasan singkat konsep proyek (max 3 menit).
3. Demo kode dan logika utama (screen record).
4. Demo robot berjalan sesuai spesifikasi proyek.
5. Analisis hasil dan kesimpulan.
6. Kendala yang dihadapi dan cara mengatasinya.

**Durasi**: 10–20 menit.  
**Deadline**: 2 minggu setelah praktikum.

---

*Proyek ini merupakan bagian dari Modul 10 Praktikum Mekatronika dan Robotika, Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi. Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.*
