# Tugas Video Praktikum Modul 09: PLC AI Modbus

**Program studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata kuliah:** Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum  
**Dosen pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.  
**Deadline:** 2 minggu setelah praktikum.

---

## 1. Tujuan Tugas

Tugas video bertujuan memastikan praktikan benar-benar menjalani proses belajar dari awal sampai akhir, memahami teori, mampu menjalankan praktik, mampu menjelaskan program, dan mampu menganalisis hasil integrasi AI vision, ROS 2, Modbus TCP, dan PLC.

---

## 2. Bentuk Tugas

Setiap kelompok wajib membuat video dokumentasi yang berisi:

1. Rekaman proses belajar dari awal sampai akhir.
2. Screen recording penjelasan teori dasar.
3. Screen recording penjelasan program dan konfigurasi.
4. Rekaman praktik instalasi, setup, build, dan running.
5. Rekaman demo gestur tangan mengendalikan PLC.
6. Analisis hasil uji, error, dan solusi.
7. Kesimpulan pembelajaran.

Video harus memperlihatkan wajah/suara anggota secara bergantian atau minimal suara penjelasan setiap anggota.

**PENTING:** Tugas hanya berupa video yang diupload ke YouTube. Tidak ada tugas laporan tertulis, ketik, atau print.

---

## 3. Ketentuan Umum

1. Deadline pengumpulan: 2 minggu setelah praktikum.
2. **Durasi video minimal: 15 menit.**
3. **Format: Video wajib diupload ke YouTube (public atau unlisted).**
4. Resolusi minimal: 720p.
5. Audio harus jelas.
6. Semua anggota kelompok wajib berkontribusi menjelaskan bagian tertentu.
7. Video harus memuat praktik nyata, bukan hanya slide.
8. Dilarang hanya menyalin video kelompok lain.
9. Jika memakai referensi eksternal, cantumkan sumber.
10. **Yang dikumpulkan: Link YouTube video saja. Tidak ada laporan tertulis.**

---

## 4. Struktur Video Wajib

### Bagian 1: Pembukaan dan Identitas

Durasi: 1 menit.

Isi:

1. Judul praktikum.
2. Nama kelompok dan anggota.
3. Program studi.
4. Mata kuliah.
5. Dosen pengampu.
6. Tujuan singkat praktikum.

### Bagian 2: Penjelasan Teori Dasar

Durasi: 2-4 menit.

Wajib menjelaskan:

1. Apa itu PLC.
2. Apa itu Modbus TCP.
3. Apa itu coil dan register.
4. Apa itu ROS 2 node.
5. Apa itu MediaPipe hand landmark.
6. Alur data kamera sampai PLC.
7. Rumus minimal:

\[
N_{open}=\sum_{i=1}^{5}f_i
\]

\[
G=f_1 2^0+f_2 2^1+f_3 2^2+f_4 2^3+f_5 2^4
\]

\[
T_{total}=T_{camera}+T_{processing}+T_{modbus}+T_{plc}
\]

### Bagian 3: Penjelasan Folder dan Program

Durasi: 2-3 menit.

Wajib menunjukkan:

1. Folder `ROS_PLC_AI`.
2. File `README.md`.
3. File `requirements.txt`.
4. Script `test_node.sh`.
5. Script `run_node.sh`.
6. Package `mediapipe_modbus_pkg`.
7. Node utama Python.
8. File konfigurasi YAML jika sudah terbentuk.

Jelaskan fungsi tiap bagian secara singkat.

### Bagian 4: Instalasi dan Setup

Durasi: 2-3 menit.

Wajib direkam:

1. Masuk folder workspace.
2. Install dependencies.
3. Source ROS 2.
4. Build package dengan colcon.
5. Source workspace.
6. Test node.
7. Cek kamera.
8. Cek jaringan PLC.

Command penting yang harus terlihat:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09 PLC AI Modbus/ROS_PLC_AI"
pip3 install -r requirements.txt
colcon build --packages-select mediapipe_modbus_pkg --symlink-install
source install/setup.bash
./test_node.sh
./run_node.sh
```

### Bagian 5: Setup Modbus dan Mapping

Durasi: 2 menit.

Wajib menunjukkan:

1. IP laptop.
2. IP PLC/V-BOX.
3. Ping PLC.
4. Port 502.
5. Unit ID.
6. Scan IP dari GUI.
7. Status connected.
8. Mapping jari ke coil/register.

Contoh mapping:

| Jari | Coil |
|---|---:|
| Jempol | 0 |
| Telunjuk | 1 |
| Tengah | 2 |
| Manis | 3 |
| Kelingking | 4 |

### Bagian 6: Demo Praktik Running

Durasi: 3-5 menit.

Wajib menunjukkan:

1. Kamera mendeteksi tangan.
2. GUI menampilkan status jari.
3. PLC connected.
4. Gestur jempol mengubah coil/output.
5. Gestur telunjuk mengubah coil/output.
6. Gestur semua jari mengubah semua output.
7. Gestur kepal mematikan output.
8. Readback/indikator PLC sesuai mapping.

Pengambilan gambar ideal menampilkan layar laptop dan output PLC sekaligus.

### Bagian 7: Pengujian dan Analisis

Durasi: 2-3 menit.

Wajib menampilkan tabel hasil:

1. Uji setiap jari.
2. Uji kombinasi gestur.
3. Delay rata-rata.
4. Akurasi deteksi.
5. Error yang terjadi.
6. Solusi yang dilakukan.

Rumus akurasi:

\[
Accuracy=\frac{Jumlah\;benar}{Total\;percobaan}\times100\%
\]

Rumus delay video:

\[
Delay=\frac{N_{selisih\_frame}}{FPS_{video}}
\]

### Bagian 8: Kesimpulan

Durasi: 1 menit.

Isi:

1. Apakah sistem berhasil.
2. Apa kendala utama.
3. Apa solusi teknis.
4. Apa hal paling penting yang dipelajari.
5. Saran pengembangan.

---

## 5. Yang Dikumpulkan

**Hanya link YouTube video.**

Tidak ada laporan tertulis, tidak ada file PDF, tidak ada print out. Semua dokumentasi, penjelasan, data uji, screenshot, dan analisis harus ada di dalam video.

---

## 6. Checklist Sebelum Mengumpulkan

| No | Item | Ya/Tidak |
|---:|---|---|
| 1 | Video berisi identitas lengkap | |
| 2 | Semua anggota menjelaskan bagian | |
| 3 | Teori PLC, Modbus, ROS 2, AI dijelaskan | |
| 4 | Instalasi dan build direkam | |
| 5 | GUI berhasil running | |
| 6 | Kamera mendeteksi tangan | |
| 7 | PLC/V-BOX connected | |
| 8 | Mapping jari ke coil terlihat | |
| 9 | Output PLC berubah sesuai gestur | |
| 10 | Ada tabel hasil uji | |
| 11 | Ada analisis delay dan akurasi | |
| 12 | Ada troubleshooting | |
| 13 | Audio jelas | |
| 14 | Video diupload ke YouTube | |
| 15 | Durasi minimal 15 menit | |

---

## 7. Rubrik Penilaian

Total nilai: 100 poin.

| Aspek | Bobot | Kriteria Sangat Baik | Kriteria Cukup | Kriteria Kurang |
|---|---:|---|---|---|
| Kelengkapan identitas dan struktur video | 5 | Identitas lengkap, alur rapi, semua bagian ada | Ada bagian kecil kurang rapi | Identitas/alur tidak jelas |
| Pemahaman teori PLC dan Modbus | 15 | Menjelaskan PLC, coil, register, function code, addressing, port 502 dengan tepat | Menjelaskan sebagian konsep tetapi belum detail | Banyak konsep salah/tidak dijelaskan |
| Pemahaman AI vision dan ROS 2 | 15 | Menjelaskan kamera, MediaPipe, landmark, status jari, ROS 2 node, alur data | Menjelaskan sebagian tetapi kurang runtut | Tidak memahami hubungan AI dan ROS 2 |
| Instalasi dan setup | 10 | Semua command penting terlihat dan berhasil | Ada command tidak terlihat tetapi hasil berjalan | Tidak menunjukkan proses setup |
| Koneksi Modbus TCP ke PLC | 10 | IP, ping, port, unit ID, connect, mapping jelas | Koneksi berhasil tetapi penjelasan kurang | Tidak berhasil connect/tidak jelas |
| Demo running sistem | 15 | Gestur mengendalikan output PLC stabil dan terlihat jelas | Demo berhasil sebagian | Demo gagal atau tidak meyakinkan |
| Analisis data | 10 | Ada tabel uji, delay, akurasi, error rate, kesimpulan teknis | Ada data tetapi analisis minim | Tidak ada data kuantitatif |
| Troubleshooting | 8 | Menunjukkan masalah nyata dan solusi teknis | Menyebut kendala tanpa bukti cukup | Tidak ada troubleshooting |
| Kualitas video dan audio | 5 | Gambar jelas, audio jelas, layar terbaca | Cukup terbaca, audio kadang kurang | Sulit dilihat/didengar |
| Kontribusi anggota | 5 | Semua anggota aktif menjelaskan | Sebagian anggota dominan | Hanya satu orang bekerja |
| Ketepatan waktu | 2 | Dikumpulkan sebelum/saat deadline | Terlambat ringan | Terlambat jauh |

---

## 8. Skala Nilai

| Nilai Akhir | Predikat |
|---:|---|
| 85-100 | Sangat baik |
| 75-84 | Baik |
| 65-74 | Cukup |
| 50-64 | Kurang |
| <50 | Tidak memenuhi |

---

## 9. Penalti

Pengurangan nilai dapat diberikan jika:

1. Video tidak bisa dibuka atau bukan YouTube: -30 sampai -100.
2. Durasi kurang dari 15 menit: -20.
3. Tidak ada demo hardware/software: -30.
4. Tidak ada suara penjelasan: -15.
5. Tidak ada kontribusi anggota tertentu: -5 per anggota.
6. Tidak ada analisis data: -10.
7. Terlambat dari deadline: sesuai kebijakan dosen/asisten.
8. Terindikasi menyalin video kelompok lain: nilai dapat dibatalkan.

---

## 10. Format Judul Video YouTube

Gunakan format judul:

```text
Modul 09 PLC AI Modbus - [Nama Kelompok] - Praktikum Mekatronika
```

Contoh:
```text
Modul 09 PLC AI Modbus - Kelompok 1 - Praktikum Mekatronika
```

Pastikan video diset **public** atau **unlisted** agar dosen/asisten dapat mengakses tanpa request permission.

---

## 11. Template Pembagian Peran

| Anggota | Peran | Bagian video |
|---|---|---|
| Anggota 1 | Koordinator dan teori PLC | Pembukaan, PLC, Modbus |
| Anggota 2 | Programmer ROS 2/Python | Program, build, node |
| Anggota 3 | Jaringan dan PLC | IP, Modbus, mapping |
| Anggota 4 | Dokumentasi dan analisis | Demo, data, kesimpulan |

Peran boleh disesuaikan dengan jumlah anggota.

---

## 12. Kesimpulan Tugas

Tugas video bukan hanya bukti bahwa aplikasi berjalan, tetapi bukti proses berpikir praktikan. Video yang baik harus menunjukkan pemahaman teori, proses instalasi, praktik running, data pengujian, kemampuan troubleshooting, dan refleksi teknis terhadap sistem otomasi berbasis AI dan PLC.

**INGAT:** Tugas hanya berupa video YouTube minimal 15 menit. Tidak ada laporan tertulis, tidak ada file yang perlu diprint atau diketik.
