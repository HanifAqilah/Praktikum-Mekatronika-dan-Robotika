# Project Modul 08: ROS Robot ARM Dobot Magician

## Identitas

- Program studi: Sarjana Terapan Teknologi Rekayasa Otomasi
- Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum
- Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.
- Modul: ROS Robot ARM Dobot Magician

## Tujuan Project

Project dirancang agar praktikan tidak hanya menjalankan command, tetapi mampu merancang proses otomasi robot arm, membuat program ROS 2, menguji performa, menganalisis error, dan mendokumentasikan hasil.

## Ketentuan Umum

1. Project dikerjakan berkelompok sesuai arahan dosen/asisten.
2. Setiap kelompok memilih 1 dari 10 variasi project atau mengusulkan variasi setara.
3. Program harus memakai ROS 2 dan stack Dobot pada workspace `ROS_DOBOT`.
4. Robot wajib menjalankan homing sebelum percobaan.
5. Kecepatan awal dibatasi `velocity_ratio <= 0.5` dan `acceleration_ratio <= 0.5`.
6. Setiap project diuji minimal 10 percobaan.
7. Wajib menyertakan video proses praktik dan screen record penjelasan program.
8. Wajib menyertakan data, metrik, analisis kegagalan, dan kesimpulan.

## Metrik Umum

Gunakan metrik berikut sesuai project:

```text
Success Rate = jumlah percobaan berhasil / jumlah seluruh percobaan × 100%
```

```text
T_cycle = waktu selesai satu siklus - waktu mulai satu siklus
```

```text
T_avg = ΣT_i / N
```

```text
Error posisi = sqrt((x_actual-x_target)^2 + (y_actual-y_target)^2 + (z_actual-z_target)^2)
```

```text
Repeatability = error rata-rata + 3 × standar deviasi error
```

```text
Throughput = jumlah objek berhasil / total waktu operasi
```

## Project 1: Pick and Place Dua Titik

### Deskripsi

Robot mengambil objek dari titik A dan memindahkannya ke titik B memakai gripper atau suction cup.

### Tujuan

- Memahami sequence dasar robot arm.
- Menggunakan action `/PTP_action`.
- Menggunakan service end effector.
- Mengukur success rate dan cycle time.

### Fitur Minimal

1. Homing.
2. Move ke posisi aman.
3. Move ke atas titik pick.
4. Turun ke titik pick.
5. Aktifkan end effector.
6. Naik ke posisi aman.
7. Move ke atas titik place.
8. Turun ke titik place.
9. Lepas objek.
10. Kembali home.

### Pengembangan Lanjut

- Tambahkan retry jika pick gagal.
- Tambahkan mode cepat dan mode aman.
- Tambahkan logging CSV.

### Output

Video robot berhasil memindahkan objek minimal 10 kali dengan tabel success rate dan rata-rata cycle time.

## Project 2: Sorting Objek Berdasarkan Posisi

### Deskripsi

Robot memindahkan objek dari beberapa posisi awal ke beberapa zona tujuan berdasarkan urutan posisi.

### Tujuan

- Membuat mapping posisi pick dan place.
- Menggunakan list target dan loop program.
- Menguji konsistensi multi-target.

### Fitur Minimal

1. Minimal 3 titik pick.
2. Minimal 3 titik place.
3. Sequence otomatis dari objek 1 sampai 3.
4. Logging status setiap objek.
5. Percobaan minimal 10 siklus.

### Pengembangan Lanjut

- Tambahkan parameter jumlah objek.
- Tambahkan sorting berdasarkan input keyboard.
- Tambahkan layout grid 2x3.

### Output

Program sorting berbasis daftar koordinat dan laporan titik mana yang paling sering gagal.

## Project 3: Palletizing Mini 2x2 atau 3x3

### Deskripsi

Robot menyusun objek ke pola pallet 2x2 atau 3x3 dengan jarak antar posisi tetap.

### Tujuan

- Memahami generasi koordinat otomatis.
- Menerapkan pola grid.
- Menghitung jarak antar titik dan workspace.

### Rumus Grid

```text
x_i = x_0 + i × dx
y_j = y_0 + j × dy
z_k = z_0 + k × dz
```

### Fitur Minimal

1. Satu titik pick tetap.
2. Empat atau sembilan titik place.
3. Loop grid otomatis.
4. Safety height sebelum pindah titik.
5. Data cycle time tiap objek.

### Pengembangan Lanjut

- Tambahkan layer kedua.
- Tambahkan optimasi urutan path.
- Tambahkan perhitungan throughput.

### Output

Robot menyusun objek ke grid dan menghasilkan data waktu tiap posisi.

## Project 4: Drawing Robot dengan Pen Tool

### Deskripsi

Robot menggambar bentuk sederhana seperti kotak, segitiga, lingkaran pendekatan, atau huruf pada kertas.

### Tujuan

- Menggunakan lintasan Cartesian.
- Memahami interpolasi titik.
- Mengontrol tinggi pen naik/turun.

### Rumus Lingkaran

```text
x = x_c + r cosθ
y = y_c + r sinθ
θ = 0 sampai 2π
```

### Fitur Minimal

1. Tool pen dipasang.
2. Titik start aman.
3. Gerak pen turun.
4. Minimal menggambar satu bentuk.
5. Pen naik saat pindah bentuk.

### Pengembangan Lanjut

- Menggambar nama kelompok.
- Mengubah file koordinat CSV menjadi gerakan robot.
- Membandingkan error bentuk ideal dan hasil gambar.

### Output

Foto hasil gambar, video proses, dan daftar koordinat lintasan.

## Project 5: Workspace Mapping dan Visualisasi

### Deskripsi

Praktikan memetakan titik-titik yang bisa dijangkau robot lalu menampilkan batas workspace secara tabel atau visual.

### Tujuan

- Memahami workspace.
- Menguji validitas target.
- Menggunakan trajectory validator dan feedback action.

### Rumus Sampling

```text
x = x_min + i × Δx
y = y_min + j × Δy
z = z_min + k × Δz
```

Target valid jika:

```text
q_min ≤ q ≤ q_max
```

### Fitur Minimal

1. Buat grid target minimal 5x5 pada satu level z.
2. Kirim target aman atau validasi target.
3. Catat valid/tidak valid.
4. Buat peta workspace sederhana.

### Pengembangan Lanjut

- Visualisasi RViz marker.
- Heatmap titik reachable.
- Analisis singularitas sederhana.

### Output

Tabel workspace dan grafik/peta area jangkauan.

## Project 6: Pengujian Repeatability Robot

### Deskripsi

Robot bergerak ke titik yang sama berulang-ulang, lalu praktikan mengukur seberapa konsisten posisi akhir.

### Tujuan

- Memahami repeatability.
- Mengukur error statistik.
- Membandingkan pengaruh speed terhadap hasil.

### Rumus

```text
e_i = sqrt((x_i-x_ref)^2 + (y_i-y_ref)^2 + (z_i-z_ref)^2)
```

```text
σ = sqrt(Σ(e_i - ē)^2/(N-1))
```

```text
R = ē + 3σ
```

### Fitur Minimal

1. Pilih satu titik target.
2. Ulangi minimal 20 kali.
3. Catat pose akhir dari `/dobot_pose_raw`.
4. Hitung mean error, standar deviasi, repeatability.

### Pengembangan Lanjut

- Bandingkan `velocity_ratio` 0.2, 0.5, 0.8.
- Bandingkan beban ringan dan tanpa beban.
- Plot error terhadap percobaan.

### Output

Data repeatability lengkap dan kesimpulan kondisi terbaik.

## Project 7: Conveyor Simulasi Manual

### Deskripsi

Robot melakukan pick saat objek diletakkan manual pada titik masuk, lalu memindahkannya ke titik keluar seperti sistem conveyor sederhana.

### Tujuan

- Membuat proses semi-otomatis.
- Menggunakan input operator.
- Mengelola state proses.

### Fitur Minimal

1. Program menunggu input `Enter` dari operator.
2. Robot mengambil objek di posisi conveyor input.
3. Robot meletakkan objek di output.
4. Program mengulang sampai jumlah objek tercapai.
5. Logging waktu tiap siklus.

### Pengembangan Lanjut

- Tambahkan sensor virtual berbasis keyboard.
- Tambahkan counter produksi.
- Tambahkan alarm jika gagal pick.

### Output

Demo sistem semi-otomatis dengan counter dan throughput.

## Project 8: Mini Assembly Insertion

### Deskripsi

Robot mengambil komponen kecil dan menempatkannya ke posisi target seperti proses assembly sederhana.

### Tujuan

- Meningkatkan presisi place.
- Menggunakan approach height bertahap.
- Menganalisis error posisi.

### Fitur Minimal

1. Titik pick komponen.
2. Titik pre-insertion.
3. Titik insertion dengan z rendah.
4. Gerak lambat saat mendekati target.
5. Uji minimal 10 kali.

### Pengembangan Lanjut

- Tambahkan spiral search kecil.
- Tambahkan dua jenis komponen.
- Tambahkan jig sederhana.

### Output

Video insertion dan analisis toleransi posisi.

## Project 9: Vision-Guided Pick Sederhana

### Deskripsi

Praktikan menggunakan kamera eksternal atau input koordinat hasil deteksi manual untuk menentukan target pick.

### Tujuan

- Memahami transformasi koordinat kamera ke robot.
- Mengintegrasikan perception dan manipulator.
- Membuat pipeline semi vision.

### Rumus Transformasi

```text
p_base = T_base_camera p_camera
```

Model piksel ke koordinat sederhana pada bidang meja:

```text
x_robot = a u + b v + c
y_robot = d u + e v + f
```

### Fitur Minimal

1. Ambil gambar area kerja.
2. Tentukan koordinat objek secara manual atau deteksi warna sederhana.
3. Konversi ke koordinat robot.
4. Robot melakukan pick pada target hasil konversi.
5. Uji minimal 10 target.

### Pengembangan Lanjut

- Deteksi warna HSV.
- Kalibrasi 4 titik.
- Integrasi YOLO dari modul sebelumnya.

### Output

Demo pick berdasarkan input visual dan analisis error transformasi.

## Project 10: State Machine Otomasi Robot Arm

### Deskripsi

Robot dikendalikan dengan state machine yang jelas: INIT, HOMING, WAIT_OBJECT, PICK, PLACE, ERROR, DONE.

### Tujuan

- Membuat program robot yang terstruktur.
- Menangani error dan retry.
- Memisahkan logika proses dari fungsi gerak.

### Contoh State

```text
INIT -> HOMING -> WAIT_OBJECT -> MOVE_PICK -> GRASP -> MOVE_PLACE -> RELEASE -> DONE
```

Jika error:

```text
ANY_STATE -> ERROR -> RECOVERY -> WAIT_OBJECT
```

### Fitur Minimal

1. Minimal 6 state.
2. Log perpindahan state.
3. Timeout saat action terlalu lama.
4. Retry minimal 2 kali jika gagal.
5. Stop aman jika error berulang.

### Pengembangan Lanjut

- Simpan log ke CSV.
- Tambahkan konfigurasi YAML.
- Tambahkan mode manual override.

### Output

Program state machine, diagram state, dan video proses otomatis.

## Format Proposal Project

1. Judul project.
2. Anggota kelompok.
3. Latar belakang singkat.
4. Tujuan.
5. Diagram alur sistem.
6. Daftar titik koordinat.
7. Daftar topic, service, action yang digunakan.
8. Rencana pengujian.
9. Risiko dan mitigasi.
10. Target output.

## Format Laporan Project

1. Judul dan identitas.
2. Deskripsi sistem.
3. Dasar teori singkat.
4. Alat dan bahan.
5. Desain flowchart atau state machine.
6. Program utama.
7. Data percobaan.
8. Perhitungan metrik.
9. Analisis hasil.
10. Kendala dan solusi.
11. Kesimpulan.
12. Link video dan repository/kode.

## Penilaian Project Ringkas

| Komponen | Bobot |
|---|---:|
| Ide dan kesesuaian project | 10% |
| Implementasi ROS 2 | 20% |
| Keberhasilan robot bergerak | 20% |
| Data dan analisis | 20% |
| Keselamatan kerja | 10% |
| Dokumentasi video dan laporan | 20% |
