# Tugas Video Modul 08: ROS Robot ARM Dobot Magician

## Identitas

- Program studi: Sarjana Terapan Teknologi Rekayasa Otomasi
- Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum
- Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.
- Modul: ROS Robot ARM Dobot Magician
- Deadline: 2 minggu setelah praktikum

## Tujuan Tugas

Tugas ini memastikan praktikan memahami teori, instalasi, setup, pemrograman, eksperimen, analisis data, serta keselamatan kerja pada praktik ROS Robot ARM. Seluruh proses belajar dan praktik wajib direkam agar terlihat proses berpikir, proses troubleshooting, dan keberhasilan akhir.

## Bentuk Tugas

Setiap kelompok wajib mengumpulkan:

1. Video gabungan proses praktik robot fisik dan screen record penjelasan.
2. Upload ke YouTube dengan durasi minimal 15 menit.
3. Video harus mencakup teori, instalasi, setup, program, eksperimen, dan analisis.
4. Tidak ada tugas laporan tertulis, ketik, atau print.
5. Pengumpulan hanya berupa link YouTube video.

## Ketentuan Video YouTube

Video adalah gabungan rekaman praktik robot fisik dan screen record penjelasan yang diedit menjadi satu video utuh dan diupload ke YouTube.

### Isi Minimal Video

1. Pembukaan dan identitas kelompok.
2. Penjelasan teori robot arm, DOF, frame, kinematika, workspace, trajectory, ROS 2.
3. Tampilan alat: Dobot, laptop, tool, objek, area kerja.
4. Safety check sebelum robot dinyalakan.
5. Screen record instalasi, build, source, setup MAGICIAN_TOOL dan dialout.
6. Proses launch control stack.
7. Proses homing.
8. Screen record monitoring topic, service, action.
9. Proses mengirim target PTP.
10. Proses mengaktifkan gripper atau suction cup.
11. Screen record penjelasan program project baris per baris.
12. Proses menjalankan program project.
13. Minimal 10 percobaan atau siklus.
14. Dokumentasi kegagalan jika ada.
15. Penjelasan penyebab dan solusi kegagalan.
16. Analisis data: success rate, cycle time, error.
17. Kesimpulan dan pembelajaran.
18. Penutup.

### Syarat Teknis Video

- Durasi minimal 15 menit.
- Upload ke YouTube (public atau unlisted).
- Robot dan end effector harus terlihat jelas saat praktik.
- Screen record harus menampilkan terminal, kode, dan hasil ROS 2.
- Audio narasi harus terdengar jelas.
- Tidak boleh hanya menampilkan hasil akhir.
- Harus memperlihatkan proses dari awal sampai berhasil running.
- Gabungkan rekaman praktik dan screen record dengan editing sederhana.
- Jika video diedit, jangan menghapus bagian troubleshooting penting.

## Struktur Video yang Disarankan

Video YouTube harus menggabungkan praktik fisik dan screen record dalam satu alur cerita yang runtut.

### Bagian 1: Pembukaan (1-2 menit)

- Identitas kelompok, anggota, mata kuliah, dosen.
- Tujuan praktikum.
- Project yang dipilih.

### Bagian 2: Teori (3-4 menit)

- Screen record penjelasan robot arm, DOF, frame, TCP, workspace.
- Penjelasan kinematika maju dan balik.
- Penjelasan trajectory dan ROS 2.
- Penjelasan struktur workspace dan package Dobot.

### Bagian 3: Setup dan Instalasi (2-3 menit)

- Screen record command instalasi ROS 2, dependency, build workspace.
- Penjelasan setup serial, dialout, MAGICIAN_TOOL.
- Tampilkan hasil `ros2 pkg list | grep dobot`.

### Bagian 4: Running Robot (4-5 menit)

- Rekaman praktik: tampilan robot, area kerja, safety check.
- Screen record launch stack.
- Rekaman praktik: proses homing.
- Screen record monitoring topic, service, action.
- Rekaman praktik: mengirim PTP action dan end effector.

### Bagian 5: Program dan Eksperimen (3-4 menit)

- Screen record penjelasan kode program project.
- Rekaman praktik: menjalankan program minimal 10 percobaan.
- Dokumentasi kegagalan dan troubleshooting.

### Bagian 6: Analisis dan Kesimpulan (2-3 menit)

- Screen record data percobaan dalam tabel.
- Perhitungan success rate, cycle time, error.
- Analisis penyebab kegagalan dan solusi.
- Kesimpulan pembelajaran.
- Penutup.

## Tips Membuat Video Berkualitas

1. Gunakan software editing sederhana seperti OpenShot, Kdenlive, DaVinci Resolve, atau online editor.
2. Gabungkan rekaman praktik dan screen record dengan transisi yang jelas.
3. Tambahkan text overlay untuk identitas, judul bagian, atau poin penting.
4. Pastikan audio narasi tidak tertutup noise atau musik latar yang terlalu keras.
5. Gunakan zoom atau crop untuk memperjelas terminal, kode, atau robot.
6. Tambahkan timestamp atau chapter di deskripsi YouTube.
7. Render video dengan resolusi minimal 720p.
8. Upload ke YouTube dengan judul jelas: "Modul 08 ROS Robot ARM - Nama Kelompok - Praktikum Mekatronika".
9. Set visibility public atau unlisted.
10. Aktifkan subtitle otomatis jika membantu.

## Data yang Wajib Dikumpulkan

Tabel minimal:

| Percobaan | Target/Objek | Berhasil Pick | Berhasil Place | Waktu Siklus (s) | Error/Kendala | Catatan |
|---:|---|---|---|---:|---|---|
| 1 | | | | | | |
| 2 | | | | | | |
| 3 | | | | | | |
| 4 | | | | | | |
| 5 | | | | | | |
| 6 | | | | | | |
| 7 | | | | | | |
| 8 | | | | | | |
| 9 | | | | | | |
| 10 | | | | | | |

Rumus wajib:

```text
Success Rate = jumlah berhasil / jumlah percobaan × 100%
```

```text
T_avg = ΣT_i / N
```

```text
Error posisi = sqrt((x_actual-x_target)^2 + (y_actual-y_target)^2 + (z_actual-z_target)^2)
```

Jika memakai repeatability:

```text
σ = sqrt(Σ(e_i - ē)^2/(N-1))
```

```text
Repeatability = ē + 3σ
```

## Format Pengumpulan

Setiap kelompok mengumpulkan link YouTube melalui form atau platform yang ditentukan dosen/asisten.

Format link:

```text
https://www.youtube.com/watch?v=XXXXXXXXXXX
```

Informasi yang disertakan:

```text
Nama kelompok: ...
Anggota: ...
Project yang dipilih: ...
Tool yang dipakai: gripper/suction_cup/pen/none
Durasi video: ... menit
Link YouTube: ...
Timestamp penting:
  - 00:00 Pembukaan
  - 02:00 Teori
  - 06:00 Setup
  - 10:00 Running Robot
  - 13:00 Analisis
  - 15:00 Kesimpulan
```

## Aturan Akademik

1. Semua anggota harus muncul atau bersuara di video.
2. Program boleh dikembangkan dari contoh, tetapi harus dijelaskan dan dimodifikasi sesuai project.
3. Plagiarisme video atau kode tanpa pemahaman akan mengurangi nilai besar.
4. Kegagalan eksperimen tetap dapat dinilai jika proses, troubleshooting, dan analisisnya lengkap.
5. Video harus original, bukan hasil copy atau re-upload video kelompok lain.
6. Durasi minimal 15 menit harus dipenuhi.
7. Video harus diupload ke YouTube (public atau unlisted).
8. Keterlambatan mengikuti kebijakan dosen/asisten.
9. Deadline pengumpulan adalah 2 minggu setelah praktikum.
10. Tidak ada tugas laporan tertulis, ketik, atau print.

## Rubrik Penilaian Lengkap

Total nilai 100.

### A. Pemahaman Teori Robot Arm dan ROS 2: 15 poin

| Level | Kriteria | Poin |
|---|---|---:|
| Sangat baik | Menjelaskan DOF, frame, TCP, workspace, kinematika, trajectory, ROS 2 node-topic-service-action dengan benar dan terkait praktik | 13-15 |
| Baik | Menjelaskan sebagian besar konsep benar, tetapi hubungan ke praktik belum mendalam | 10-12 |
| Cukup | Menjelaskan konsep dasar, masih ada kekeliruan kecil | 7-9 |
| Kurang | Penjelasan dangkal dan banyak konsep tidak tepat | 4-6 |
| Sangat kurang | Tidak menjelaskan teori atau hanya membaca tanpa memahami | 0-3 |

### B. Instalasi, Setup, dan Running Stack: 15 poin

| Level | Kriteria | Poin |
|---|---|---:|
| Sangat baik | Menunjukkan instalasi/dependency, build, source, dialout, MAGICIAN_TOOL, launch, node/topic/service/action dengan runtut | 13-15 |
| Baik | Setup berhasil dan dijelaskan, tetapi beberapa detail kecil hilang | 10-12 |
| Cukup | Robot berhasil running tetapi penjelasan setup kurang lengkap | 7-9 |
| Kurang | Banyak langkah setup tidak jelas atau dibantu penuh tanpa pemahaman | 4-6 |
| Sangat kurang | Tidak dapat menunjukkan stack running | 0-3 |

### C. Implementasi Program Project: 20 poin

| Level | Kriteria | Poin |
|---|---|---:|
| Sangat baik | Program terstruktur, memakai action/service dengan benar, ada safety pose, error handling/logging, dan sesuai project | 17-20 |
| Baik | Program berjalan sesuai fungsi utama, struktur cukup rapi | 13-16 |
| Cukup | Program berjalan sebagian, masih manual atau kurang robust | 9-12 |
| Kurang | Program banyak error dan hanya menjalankan command dasar | 5-8 |
| Sangat kurang | Tidak ada program yang dapat dijalankan | 0-4 |

### D. Keberhasilan Praktik Robot: 15 poin

| Level | Kriteria | Poin |
|---|---|---:|
| Sangat baik | Robot berhasil homing, PTP, end effector, dan menjalankan project minimal 10 percobaan dengan aman | 13-15 |
| Baik | Robot berhasil menjalankan fungsi utama dengan sedikit kegagalan | 10-12 |
| Cukup | Robot hanya berhasil sebagian fungsi utama | 7-9 |
| Kurang | Robot sering gagal dan tidak ada solusi jelas | 4-6 |
| Sangat kurang | Tidak ada bukti robot berjalan | 0-3 |

### E. Data, Perhitungan, dan Analisis: 15 poin

| Level | Kriteria | Poin |
|---|---|---:|
| Sangat baik | Data lengkap, rumus benar, success rate/cycle time/error dihitung, analisis penyebab dan solusi kuat | 13-15 |
| Baik | Data dan perhitungan ada, analisis cukup | 10-12 |
| Cukup | Data ada tetapi analisis dangkal | 7-9 |
| Kurang | Data tidak lengkap atau perhitungan banyak salah | 4-6 |
| Sangat kurang | Tidak ada data percobaan | 0-3 |

### F. Kualitas Video YouTube: 20 poin

| Level | Kriteria | Poin |
|---|---|---:|
| Sangat baik | Video 15+ menit, gabungan praktik dan screen record runtut, audio jelas, semua bagian lengkap, upload YouTube berhasil | 17-20 |
| Baik | Video memenuhi durasi, konten lengkap, tetapi editing atau audio kurang optimal | 13-16 |
| Cukup | Video cukup membuktikan praktik dan teori, durasi mendekati minimal, beberapa bagian kurang jelas | 9-12 |
| Kurang | Video kurang dari 15 menit atau banyak bagian penting hilang | 5-8 |
| Sangat kurang | Tidak ada video YouTube atau video tidak dapat diakses | 0-4 |

## Rekap Nilai

| Komponen | Bobot |
|---|---:|
| Pemahaman teori robot arm dan ROS 2 | 15 |
| Instalasi, setup, dan running stack | 15 |
| Implementasi program project | 20 |
| Keberhasilan praktik robot | 15 |
| Data, perhitungan, dan analisis | 15 |
| Kualitas video YouTube | 20 |
| Total | 100 |

## Checklist Sebelum Mengumpulkan

| Item | Status |
|---|---|
| Video gabungan praktik dan screen record sudah diedit | |
| Durasi minimal 15 menit terpenuhi | |
| Video sudah diupload ke YouTube | |
| Link YouTube dapat dibuka (public atau unlisted) | |
| Semua bagian lengkap: pembukaan, teori, setup, running, eksperimen, analisis, kesimpulan | |
| Data percobaan minimal 10 ditampilkan di video | |
| Perhitungan success rate dan cycle time ada di video | |
| Penjelasan kode program ada di video | |
| Semua anggota muncul atau bersuara | |
| Audio narasi jelas | |
| Judul video sesuai format | |
| Informasi pengumpulan lengkap | |

## Catatan Penilaian

Nilai tinggi diberikan pada kelompok yang menunjukkan proses lengkap dalam video YouTube, bukan hanya hasil akhir. Dokumentasi kegagalan, troubleshooting, dan analisis penyebab akan dinilai positif apabila disampaikan jujur, teknis, dan disertai solusi.

Video yang menunjukkan pemahaman mendalam terhadap teori, kemampuan troubleshooting, dan analisis data yang kuat akan mendapat nilai maksimal meskipun eksperimen tidak 100% berhasil.

## Contoh Judul Video YouTube

- "Modul 08 ROS Robot ARM Dobot Magician - Kelompok 1 - Praktikum Mekatronika"
- "ROS 2 Dobot Pick and Place - Kelompok Alpha - Praktikum Mekatronika 2026"
- "Robot ARM Dobot Palletizing Project - Kelompok Beta - Praktikum Mekatronika"

## Contoh Deskripsi Video YouTube

```
Modul 08: ROS Robot ARM Dobot Magician
Program Studi: Sarjana Terapan Teknologi Rekayasa Otomasi
Mata Kuliah: Praktikum Mekatronika (2 SKS)
Dosen: Rofiq Cahyo Prayogo, S.T., M.T.

Kelompok: ...
Anggota:
1. ...
2. ...
3. ...

Project: Pick and Place dengan Suction Cup

Timestamp:
00:00 - Pembukaan dan Identitas
02:00 - Teori Robot ARM dan ROS 2
06:00 - Setup dan Instalasi
10:00 - Running Robot dan Eksperimen
13:00 - Analisis Data
15:00 - Kesimpulan

Hashtags: #ROS2 #RobotARM #Dobot #Mekatronika #Otomasi
```
