# TUGAS VIDEO — Modul 10: Line Follower Robot

**Mata Kuliah**: Praktikum Mekatronika dan Robotika  
**Program Studi**: Sarjana Terapan Teknologi Rekayasa Otomasi  
**Dosen Pengampu**: Rofiq Cahyo Prayogo, S.T., M.T.  
**SKS / Durasi**: 2 SKS / 5 jam 40 menit  
**Deadline Pengumpulan**: **2 minggu setelah praktikum**  
**Platform**: YouTube (akun pribadi masing-masing mahasiswa)

---

## 1. Deskripsi Tugas

Setiap mahasiswa **wajib membuat dan mengunggah video dokumentasi** proses belajar Modul 10 Line Follower ke YouTube masing-masing. Video ini merupakan bukti pemahaman dan kemampuan praktik, bukan sekadar rekaman akhir.

Video harus merekam **seluruh proses belajar**: dari pemahaman teori, setup hardware, proses coding dan debugging, proses tuning, hingga robot berhasil berjalan. Bukan hanya hasil akhir.

---

## 2. Format Video

### 2.1 Identitas Wajib

Tampilkan di awal video (min. 30 detik):
- Nama lengkap
- NIM
- Program Studi: Sarjana Terapan Teknologi Rekayasa Otomasi
- Mata Kuliah: Praktikum Mekatronika dan Robotika
- Modul: Modul 10 — Line Follower Robot
- Dosen: Rofiq Cahyo Prayogo, S.T., M.T.
- Tanggal praktikum

### 2.2 Judul Video YouTube

Format wajib:
```
[Modul10] Line Follower — NamaMahasiswa — NIM — PraktikumMekatronika
```

Contoh:
```
[Modul10] Line Follower — Budi Santoso — 2024001234 — PraktikumMekatronika
```

### 2.3 Deskripsi YouTube

Wajib dicantumkan di kolom deskripsi video:
```
Mata Kuliah  : Praktikum Mekatronika dan Robotika
Modul        : Modul 10 — Line Follower Robot
Mahasiswa    : [Nama] — [NIM]
Program Studi: Sarjana Terapan Teknologi Rekayasa Otomasi
Dosen        : Rofiq Cahyo Prayogo, S.T., M.T.
Tanggal      : [tanggal praktikum]

Daftar Isi:
00:00 — Pembukaan dan identitas
01:00 — Penjelasan teori
05:00 — Setup hardware dan wiring
10:00 — Install dan build program
15:00 — Uji OLED, button, motor, sensor
25:00 — Tuning PID (proses)
35:00 — Robot berjalan di lintasan
40:00 — Analisis dan kesimpulan
```

---

## 3. Struktur Konten Video

Video dibagi dalam **6 bagian utama** dengan waktu yang disarankan:

### Bagian 1 — Pembukaan dan Teori (10–15 menit)

**Wajib ada:**

1. **Identitas diri** — sebut nama, NIM, mata kuliah, modul.
2. **Penjelasan komponen** — tunjukkan semua hardware yang digunakan:
   - ESP32 S2 Mini: jelaskan fungsi, spesifikasi dasar.
   - Sensor IR 8 channel: cara kerja, mengapa 8 channel.
   - Motor DC + driver H-Bridge: jelaskan prinsip PWM.
   - OLED 128×64: fungsi sebagai display feedback.
   - Push button 4 buah: fungsi masing-masing.
3. **Konsep Line Follower** — jelaskan:
   - Bagaimana sensor membaca garis.
   - Konsep error dan setpoint (setpoint = 3500).
   - Prinsip kontrol PID dalam 1–2 menit.
4. **Penjelasan rumus PID**:
   ```
   error = position - setpoint
   correction = Kp×error + Ki×integral + Kd×derivative
   leftSpeed  = baseSpeed + correction
   rightSpeed = baseSpeed - correction
   ```
   Jelaskan makna setiap variabel.

### Bagian 2 — Setup dan Instalasi (5–10 menit)

**Wajib ada:**

1. **Screen record** proses instalasi (atau penjelasan jika sudah terinstal):
   - VS Code + PlatformIO.
   - Buka folder project `Program/`.
2. **Tunjukkan struktur folder**:
   - Folder 01OLED sampai 06PathPlanning.
   - Jelaskan fungsi masing-masing folder.
3. **Tunjukkan `config.h`** — baca dan jelaskan parameter penting:
   - Pin sensor S0–S7.
   - Pin motor kiri dan kanan.
   - Nilai default PID.
4. **Tunjukkan proses build dan upload** (screen record):
   - `pio run` → berhasil.
   - `pio run -t upload` → berhasil.
   - Buka Serial Monitor.

### Bagian 3 — Praktikum Bertahap (15–20 menit)

**Wajib ada (minimal 4 dari 6 sub-praktikum):**

1. **Uji OLED (Program 01)**:
   - Screen record proses upload.
   - Video OLED menampilkan tulisan.
   - Jelaskan alamat I2C dan pin SDA/SCL.

2. **Uji Push Button (Program 02)**:
   - Video tombol mengubah tampilan OLED.
   - Jelaskan konsep debouncing.
   - Tunjukkan output Serial Monitor.

3. **Uji Motor (Program 03)**:
   - Video motor bergerak maju, mundur, berhenti.
   - Jelaskan mode motor yang digunakan.
   - Tunjukkan cara mengubah kecepatan PWM.

4. **Kalibrasi Sensor (Program 04)**:
   - Video proses kalibrasi (sensor di atas putih, lalu hitam).
   - Tampilkan tabel nilai ADC yang dicatat.
   - Hitung threshold: `T = (ADC_putih + ADC_hitam) / 2`.
   - Tunjukkan nilai position saat garis di kiri, tengah, kanan.

5. **PID Line Follower (Program 05)**:
   - Video robot berjalan mengikuti garis.
   - **Proses tuning** — rekam percobaan berturut-turut (min. 5).
   - Jelaskan efek mengubah Kp, Kd.

6. **Path Planning (Program 06)**:
   - Video robot melewati minimal 1 persimpangan.
   - Jelaskan konsep sensor mask.
   - Tunjukkan konfigurasi path di kode.

### Bagian 4 — Proses Tuning PID (10–15 menit)

Bagian ini **sangat penting** dan harus direkam secara nyata:

**Wajib ada:**

1. **Rekam minimal 5 percobaan tuning** secara berturut-turut.
   - Setiap percobaan: sebut nilai Kp, Ki, Kd, Speed sebelum run.
   - Rekam perilaku robot saat berjalan.
   - Sebut observasi: "robot zig-zag karena Kp terlalu besar".
   - Sebut keputusan: "turunkan Kp, naikkan Kd".

2. **Tampilkan tabel tuning** yang sudah diisi:

   | Percobaan | Kp | Ki | Kd | Speed | Hasil |
   |---:|---:|---:|---:|---:|---|
   | 1 | 0.05 | 0 | 0 | 120 | Keluar di tikungan |
   | 2 | 0.08 | 0 | 0 | 120 | Zig-zag cepat |
   | 3 | 0.08 | 0 | 1.0 | 120 | Hampir stabil |
   | 4 | 0.08 | 0 | 1.5 | 120 | Stabil |
   | 5 | 0.08 | 0 | 1.5 | 130 | Lebih cepat, stabil |

3. **Jelaskan parameter akhir** yang dipilih dan alasannya.

### Bagian 5 — Demonstrasi Hasil Akhir (5–10 menit)

**Wajib ada:**

1. **Video robot berjalan mengikuti garis** — minimal 2 lap penuh tanpa keluar.
   - Kamera harus bisa melihat seluruh lintasan atau diikuti dari atas.
   - Lintasan minimal memiliki 1 tikungan.

2. **OLED** menampilkan informasi (posisi, error, parameter).

3. **Tunjukkan Serial Monitor** saat robot berjalan — tampilkan data real-time.

4. (Opsional) Video robot menjalankan path planning di persimpangan.

### Bagian 6 — Analisis dan Penutup (5–10 menit)

**Wajib ada:**

1. **Analisis hasil kalibrasi sensor**:
   - Berapa nilai ADC sensor paling kiri vs paling kanan.
   - Apakah threshold sudah optimal.

2. **Analisis parameter PID akhir**:
   - Mengapa Kp dipilih sebesar itu.
   - Mengapa Kd dipilih sebesar itu.
   - Apakah Ki = 0 sudah cukup, mengapa.

3. **Kendala yang dihadapi** dan cara mengatasinya:
   - Contoh: "Motor kiri lebih kuat → atur MOTOR_RIGHT_OFFSET".
   - Contoh: "Upload gagal → tekan BOOT saat upload".

4. **Kesimpulan** — apa yang dipelajari dari modul ini.

5. **Penutup** — ucapan terima kasih dan info kontak/link GitHub (opsional).

---

## 4. Teknis Produksi Video

### 4.1 Durasi
- **Minimum**: 30 menit
- **Maksimum**: 60 menit
- **Disarankan**: 40–50 menit

### 4.2 Kualitas Video
- **Resolusi**: minimal 720p (HD), disarankan 1080p.
- **Audio**: suara jelas, hindari noise berlebihan.
- **Pencahayaan**: cukup terang, robot dan lintasan terlihat jelas.

### 4.3 Screen Record
- Gunakan OBS Studio, Kazam, atau tools bawaan OS.
- Pastikan teks di VS Code / Serial Monitor terbaca.
- Kursor mouse terlihat saat menjelaskan kode.

### 4.4 Video Robot
- Posisi kamera: dari atas atau samping yang menunjukkan gerakan robot.
- Pastikan garis dan sensor robot terlihat.
- Stabilkan kamera (jangan handheld bergetar).

### 4.5 Editing (Opsional tapi Nilai Plus)
- Tambahkan teks/subtitle pada bagian penting.
- Potong bagian menunggu yang terlalu lama.
- Tambahkan music background yang tidak mengganggu penjelasan.

---

## 5. Pengumpulan Tugas

### 5.1 Link Pengumpulan
- Unggah video ke YouTube (akun pribadi, tidak perlu akun institusi).
- Visibilitas: **Publik** atau **Tidak Terdaftar** (bukan Privat).
- Submit link YouTube ke form pengumpulan yang disediakan dosen.

### 5.2 Deadline
- **2 minggu** setelah sesi praktikum berlangsung.
- Keterlambatan dikenakan pengurangan nilai 10% per hari.
- Pengumpulan setelah minggu ke-3: nilai maksimum 60%.

### 5.3 Yang Dikumpulkan
1. Link YouTube video (wajib).
2. Tabel kalibrasi sensor (foto atau screenshot).
3. Tabel tuning PID (foto atau screenshot).
4. Parameter PID final yang digunakan.

---

## 6. Rubrik Penilaian

**Total Nilai**: 100 poin

### 6.1 Kelengkapan Konten (40 poin)

| Komponen | Nilai Maksimum | Keterangan |
|---|---:|---|
| Identitas diri dan penjelasan modul | 5 | Nama, NIM, mata kuliah, dosen |
| Penjelasan teori komponen | 5 | Min. 3 komponen dijelaskan dengan benar |
| Proses build dan upload | 3 | Screen record build dan upload berhasil |
| Uji OLED | 3 | Video OLED menyala, penjelasan pin I2C |
| Uji Push Button | 3 | Video tombol berfungsi + debouncing |
| Uji Motor | 3 | Video motor maju/mundur/berhenti |
| Kalibrasi Sensor (data tabel) | 5 | Tabel ADC dan threshold per sensor |
| PID Line Follower berjalan | 5 | Video robot mengikuti garis |
| Proses tuning PID (min. 5 percobaan) | 5 | Rekam live tuning, tabel diisi |
| Demonstrasi hasil akhir | 3 | Robot 2 lap tanpa exit |

### 6.2 Kualitas Penjelasan (30 poin)

| Komponen | Nilai Maksimum | Keterangan |
|---|---:|---|
| Penjelasan rumus PID | 8 | Benar, mudah dipahami |
| Analisis proses tuning | 8 | Hubungkan perubahan parameter dengan perilaku |
| Analisis hasil kalibrasi | 6 | Interpretasi data ADC dan threshold |
| Kendala dan solusi | 5 | Minimal 2 kendala nyata yang dialami |
| Kesimpulan | 3 | Relevan dan logis |

### 6.3 Kualitas Teknis Video (15 poin)

| Komponen | Nilai Maksimum | Keterangan |
|---|---:|---|
| Resolusi dan kecerahan | 4 | Min. 720p, robot terlihat jelas |
| Audio jelas | 3 | Penjelasan verbal terdengar baik |
| Screen record terbaca | 3 | Teks kode terbaca |
| Deskripsi YouTube lengkap | 3 | Format sesuai panduan |
| Judul YouTube sesuai format | 2 | Format wajib terpenuhi |

### 6.4 Ketepatan Waktu (10 poin)

| Pengumpulan | Nilai |
|---|---:|
| Tepat waktu (≤ 2 minggu) | 10 |
| Terlambat 1–3 hari | 7 |
| Terlambat 4–7 hari | 5 |
| Terlambat > 7 hari | 3 |
| Tidak mengumpulkan | 0 |

### 6.5 Kreativitas dan Inovasi (5 poin)

| Komponen | Nilai Maksimum | Keterangan |
|---|---:|---|
| Editing video profesional | 2 | Subtitle, transisi, grafik |
| Konten tambahan di luar standar | 2 | Modifikasi kode, eksperimen tambahan |
| Deskripsi YouTube sangat lengkap | 1 | Timestamp, links, keterangan detail |

---

## 7. Contoh Alur Pembuatan Video

Berikut urutan yang disarankan untuk membuat video:

```
Hari 1 (Saat Praktikum):
├── Rekam identitas diri dan hardware (Bagian 1, 5 menit)
├── Screen record build + upload Program 01 (OLED)
├── Rekam OLED menyala
├── Screen record build + upload Program 02 (Button)
├── Rekam button berfungsi
├── Screen record build + upload Program 03 (Motor)
├── Rekam motor bergerak
├── Kalibrasi sensor + catat tabel
├── Rekam Program 04 berjalan
├── Start tuning PID — rekam setiap percobaan
└── Rekam robot berjalan mengikuti garis (Program 05)

Hari 2–7 (Minggu 1 setelah praktikum):
├── Lanjutkan tuning jika belum optimal
├── Rekam path planning (Program 06)
├── Screen record penjelasan kode bagian-bagian penting
└── Buat dan rekam analisis + kesimpulan

Hari 8–14 (Minggu 2 setelah praktikum):
├── Edit video: potong, gabungkan, tambah judul
├── Upload ke YouTube
├── Isi deskripsi sesuai format
└── Submit link ke form pengumpulan
```

---

## 8. FAQ (Pertanyaan yang Sering Ditanyakan)

**Q: Bolehkah video dalam satu rekaman terus tanpa edit?**  
A: Boleh, tapi pastikan konten lengkap dan audio jelas. Editing tidak wajib tapi menambah nilai.

**Q: Bolehkah video dalam bahasa daerah?**  
A: Wajib dalam Bahasa Indonesia.

**Q: Robot saya tidak berhasil 100%, bolehkah video tetap dikumpulkan?**  
A: Ya, tetap kumpulkan. Nilai diambil dari proses dan penjelasan, bukan hanya hasil akhir. Jelaskan kendala yang dihadapi.

**Q: Bolehkah video dibuat bersama dengan teman satu kelompok?**  
A: Video wajib diunggah ke akun YouTube **masing-masing**. Boleh merekam bersama, tetapi setiap orang harus memberikan penjelasan verbal sendiri di videonya.

**Q: Apakah perlu screen record semua proses coding?**  
A: Tidak semua, tapi wajib ada screen record proses build, upload, dan Serial Monitor saat robot berjalan. Penjelasan kode penting seperti config.h dan logika PID wajib ada.

**Q: Bagaimana jika YouTube saya privat karena orang tua?**  
A: Gunakan visibilitas "Tidak Terdaftar" (Unlisted) — video hanya bisa dibuka dengan link, tidak muncul di pencarian.

---

## 9. Checklist Sebelum Submit

Gunakan checklist ini sebelum mengumpulkan link video:

**Video Content:**
- [ ] Identitas diri disebutkan di awal (nama, NIM, mata kuliah, dosen, modul).
- [ ] Minimal 4 dari 6 sub-praktikum ada dalam video.
- [ ] Proses tuning PID direkam (min. 5 percobaan).
- [ ] Tabel tuning PID diisi dan ditampilkan.
- [ ] Tabel kalibrasi sensor diisi dan ditampilkan.
- [ ] Video robot mengikuti garis minimal 2 lap.
- [ ] Analisis dan kesimpulan ada di bagian akhir.
- [ ] Kendala dan solusi dijelaskan.

**YouTube:**
- [ ] Judul sesuai format: `[Modul10] Line Follower — Nama — NIM — PraktikumMekatronika`.
- [ ] Deskripsi terisi sesuai format (termasuk timestamp).
- [ ] Visibilitas: Publik atau Tidak Terdaftar (bukan Privat).
- [ ] Video bisa diakses tanpa login.

**Pengumpulan:**
- [ ] Link YouTube dicopy dan bisa dibuka.
- [ ] Foto/screenshot tabel kalibrasi dan tabel tuning sudah disiapkan.
- [ ] Submit ke form pengumpulan sebelum deadline.

---

*Tugas ini merupakan bagian dari Modul 10 Praktikum Mekatronika dan Robotika, Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi. Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.*
