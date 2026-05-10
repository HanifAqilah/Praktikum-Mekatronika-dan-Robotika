# Project Praktikum Modul 09: 10 Variasi Project PLC AI Modbus

**Program studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata kuliah:** Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum  
**Dosen pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.

---

## Aturan Umum Project

Setiap kelompok memilih atau mengembangkan minimal satu project. Project harus memakai konsep utama modul: kamera, AI vision/MediaPipe, ROS 2/Python, GUI atau monitoring, Modbus TCP, dan PLC/V-BOX.

Setiap project wajib memiliki:

1. Tujuan sistem.
2. Diagram blok.
3. Mapping data AI ke coil/register PLC.
4. Program yang dapat dijalankan.
5. Indikator keberhasilan.
6. Pengujian minimal 10 percobaan.
7. Analisis delay dan akurasi.
8. Video demonstrasi.

---

## Project 1: Kontrol 5 Lampu PLC Berbasis Gestur 5 Jari

### Deskripsi

Setiap jari mengendalikan satu lampu atau satu coil PLC. Jari terbuka berarti output ON, jari tertutup berarti output OFF.

### Mapping

| Jari | Coil | Output |
|---|---:|---|
| Jempol | 0 | Lampu 1 |
| Telunjuk | 1 | Lampu 2 |
| Tengah | 2 | Lampu 3 |
| Manis | 3 | Lampu 4 |
| Kelingking | 4 | Lampu 5 |

### Tantangan

Tambahkan debounce 3 frame agar output tidak berkedip.

### Indikator berhasil

Minimal 90% percobaan gestur menghasilkan output benar.

---

## Project 2: Gesture Start-Stop Motor Simulator

### Deskripsi

Gestur tertentu dipakai sebagai start, stop, dan reset. Output PLC mengendalikan motor simulator atau lampu motor.

### Contoh aturan

| Gestur | Aksi |
|---|---|
| Jempol | Start |
| Kepal | Stop |
| Telunjuk + tengah | Reset fault |
| Semua jari | Emergency soft stop |

### Logika PLC

Gunakan latch:

\[
Motor=(Start+Motor)\cdot \overline{Stop}\cdot Safety
\]

### Tantangan

Tambahkan interlock agar motor tidak start jika gestur tidak stabil selama 1 detik.

---

## Project 3: Conveyor Sorting Simulator dengan Jumlah Jari

### Deskripsi

Jumlah jari terbuka menentukan jalur sortir conveyor. PLC menyalakan output diverter berdasarkan jumlah jari.

### Mapping

| Jumlah jari | Aksi PLC |
|---:|---|
| 0 | Stop conveyor |
| 1 | Jalur A |
| 2 | Jalur B |
| 3 | Jalur C |
| 4 | Reject |
| 5 | Reset sistem |

### Rumus

\[
N_{open}=\sum_{i=1}^{5}f_i
\]

### Tantangan

Buat register holding `D0` berisi jumlah jari terbuka.

---

## Project 4: Password Gestur untuk Mengaktifkan Mesin

### Deskripsi

Sistem hanya aktif jika praktikan memasukkan urutan gestur benar, misalnya 1-3-5-2 jari.

### Contoh password

```text
1 jari -> 3 jari -> 5 jari -> 2 jari
```

### State machine

```text
IDLE -> STEP1 -> STEP2 -> STEP3 -> UNLOCK -> TIMEOUT/RESET
```

### Tantangan

Tambahkan timeout 5 detik. Jika waktu habis, password diulang dari awal.

---

## Project 5: Human-Machine Interface Mini Berbasis GUI dan PLC

### Deskripsi

GUI menampilkan status kamera, status jari, status koneksi PLC, coil aktif, dan readback output. Praktikan menambahkan tombol manual override.

### Fitur wajib

1. Connect/disconnect PLC.
2. Indikator status 5 jari.
3. Indikator coil PLC.
4. Manual ON/OFF tiap coil.
5. Save/load mapping YAML.

### Tantangan

Tambahkan logging CSV untuk semua perubahan coil.

---

## Project 6: Monitoring Kualitas Sinyal Gestur

### Deskripsi

Project berfokus pada analisis performa AI. Sistem menghitung FPS, jumlah frame valid, confidence, error gesture, dan noise.

### Data yang dicatat

| Data | Keterangan |
|---|---|
| FPS | frame per second kamera |
| Valid frame | frame dengan tangan terdeteksi |
| Lost frame | frame tangan tidak terdeteksi |
| Gesture error | status salah |
| Delay | waktu gestur ke output |

### Rumus

\[
ValidRate=\frac{N_{valid}}{N_{total}}\times100\%
\]

### Tantangan

Buat grafik performa sebelum dan sesudah pencahayaan diperbaiki.

---

## Project 7: Kontrol Lampu Lalu Lintas Mini dengan Gestur

### Deskripsi

PLC mengendalikan lampu merah, kuning, hijau. Gestur memilih mode operasi.

### Mode

| Gestur | Mode |
|---|---|
| 1 jari | Manual merah |
| 2 jari | Manual kuning |
| 3 jari | Manual hijau |
| 4 jari | Auto cycle |
| 5 jari | Flashing warning |

### Tantangan

Tambahkan timer PLC untuk mode otomatis:

```text
Merah 5 s -> Hijau 5 s -> Kuning 2 s -> Merah
```

---

## Project 8: Gesture-Based Pick and Place Command Simulator

### Deskripsi

Gestur dipakai untuk mengirim perintah robot pick and place sederhana ke PLC. Output dapat berupa indikator step sequence.

### Sequence

| Step | Aksi |
|---:|---|
| 1 | Home |
| 2 | Move to pick |
| 3 | Gripper close |
| 4 | Move to place |
| 5 | Gripper open |

### Mapping gestur

| Gestur | Command |
|---|---|
| Jempol | Next step |
| Kepal | Pause |
| Semua jari | Reset home |

### Tantangan

Tambahkan interlock: command next hanya diterima setelah gestur dilepas.

---

## Project 9: Sistem Watchdog Komunikasi AI ke PLC

### Deskripsi

Project menekankan keandalan komunikasi. Python mengirim heartbeat berkala ke PLC. PLC mematikan output jika heartbeat berhenti.

### Prinsip

Heartbeat toggle:

\[
HB_k=\overline{HB_{k-1}}
\]

Fault:

\[
Fault=1 \quad jika \quad t_{now}-t_{lastHB}>T_{watchdog}
\]

### Output

| Kondisi | Output |
|---|---|
| Heartbeat normal | sistem enable |
| Heartbeat putus | semua output OFF |
| Reconnect | perlu reset manual |

### Tantangan

Simulasikan kabel jaringan dicabut lalu analisis waktu fault.

---

## Project 10: Dashboard Data Logging AI-PLC

### Deskripsi

Sistem menyimpan data praktikum ke file CSV: timestamp, status jari, kode gestur, coil, koneksi, dan delay.

### Format CSV

| timestamp | thumb | index | middle | ring | pinky | code | coil0 | coil1 | coil2 | coil3 | coil4 | connected |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|

### Rumus kode gestur

\[
G=f_1 2^0+f_2 2^1+f_3 2^2+f_4 2^3+f_5 2^4
\]

### Tantangan

Buat ringkasan otomatis: total percobaan, akurasi, rata-rata delay, error komunikasi.

---

## Konten Video Project

Video YouTube minimal 15 menit harus berisi:

1. Judul project dan anggota kelompok.
2. Latar belakang dan tujuan.
3. Diagram sistem dan arsitektur.
4. Hardware dan software yang digunakan.
5. Mapping Modbus (jari ke coil/register).
6. Penjelasan alur program.
7. Demo hasil running.
8. Data pengujian (minimal 10 percobaan).
9. Analisis delay dan akurasi.
10. Kendala dan solusi troubleshooting.
11. Kesimpulan.

**Tidak ada laporan tertulis.** Semua dokumentasi ada di video.

---

## Kriteria Project Terbaik

Project terbaik adalah project yang:

1. Berjalan stabil.
2. Memiliki mapping data jelas.
3. Menggunakan prinsip keselamatan.
4. Mempunyai data uji kuantitatif.
5. Memiliki dokumentasi video lengkap.
6. Menunjukkan pemahaman teori, bukan hanya menjalankan program.
7. Mampu menjelaskan error dan solusi secara teknis.
