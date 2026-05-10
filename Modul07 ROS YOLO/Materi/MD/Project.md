# Project Modul 07: Variasi Proyek ROS YOLO

Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi  
Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.

## Tujuan

Dokumen ini berisi 10 ide proyek lanjutan agar praktikan dapat memperdalam YOLO, computer vision, ROS 2, dan aplikasi robotika. Setiap proyek dapat dikembangkan sebagai tugas kelompok, mini riset, atau portofolio.

---

## Format Proposal Mini Project

Setiap kelompok memilih satu proyek dan menulis rancangan singkat:

```text
Judul proyek:
Latar belakang:
Tujuan:
Objek/class yang dideteksi:
Dataset yang dibutuhkan:
Metode anotasi:
Model YOLO yang dipakai:
ROS node/topic yang dipakai:
Metrik keberhasilan:
Risiko/kendala:
Output akhir:
```

Metrik minimal:

1. Precision.
2. Recall.
3. mAP50.
4. mAP50-95.
5. FPS atau inference time.
6. Keberhasilan demo ROS.

---

## Project 1: Deteksi Objek Warna untuk Sorting Robot

### Deskripsi

Sistem mendeteksi objek berdasarkan bentuk dan warna, misalnya kotak merah, silinder hijau, dan bola biru. Hasil deteksi dipakai sebagai dasar pemilahan objek pada robot lengan atau conveyor.

### Class

```text
kotak_merah
kotak_biru
silinder_hijau
bola_kuning
```

### Dataset

Minimal 50 gambar per class. Variasikan posisi objek di atas meja, jarak kamera, dan pencahayaan.

### Integrasi ROS

```text
/camera/image_raw -> /yolo/detections -> node_sorting_decision
```

### Output

1. Model deteksi objek warna.
2. Node keputusan sorting.
3. Demo bounding box dan label.
4. Tabel akurasi per class.

### Pengembangan Lanjut

Hubungkan hasil deteksi ke robot arm untuk mengambil objek berdasarkan class.

---

## Project 2: Deteksi Komponen Mekatronika

### Deskripsi

Sistem mendeteksi komponen praktikum seperti motor DC, servo, sensor ultrasonik, Arduino, ESP32, kabel jumper, dan breadboard.

### Class

```text
motor_dc
servo
sensor_ultrasonik
arduino
esp32
breadboard
kabel_jumper
```

### Dataset

Ambil gambar komponen dari banyak sudut. Gunakan background meja praktikum dan background polos.

### Tantangan

1. Bentuk kecil dan mirip.
2. Kabel sering tumpang tindih.
3. Komponen memiliki tekstur kompleks.

### Output

1. Model YOLO custom komponen mekatronika.
2. Dataset rapi format YOLO.
3. Demo klasifikasi komponen real-time.

### Pengembangan Lanjut

Buat sistem inventory otomatis untuk mengecek kelengkapan kit praktikum.

---

## Project 3: Deteksi APD dan Keselamatan Laboratorium

### Deskripsi

Sistem mendeteksi penggunaan alat pelindung diri atau kondisi keselamatan laboratorium. Contoh: kacamata safety, sarung tangan, jas lab, area berbahaya, dan tangan dekat mesin.

### Class

```text
kacamata_safety
sarung_tangan
jas_lab
tangan
area_mesin
```

### Dataset

Dataset harus memperhatikan privasi. Hindari wajah jelas. Fokus pada tangan, APD, dan area kerja.

### Integrasi ROS

Jika APD tidak terdeteksi, sistem menerbitkan status warning:

```text
/safety/status
```

### Output

1. Deteksi APD real-time.
2. Warning sederhana di terminal atau GUI.
3. Analisis false positive dan false negative.

### Pengembangan Lanjut

Tambahkan alarm audio saat tangan terlalu dekat dengan area mesin.

---

## Project 4: Robot Pengikut Objek Berbasis YOLO

### Deskripsi

Robot mobile mengikuti objek tertentu, misalnya bola biru atau marker khusus. YOLO mendeteksi objek, lalu posisi bounding box dipakai untuk menentukan arah gerak.

### Class

```text
target_marker
bola_biru
orang_target
```

### Logika Kontrol

Misal lebar gambar `W`, pusat objek `cx`:

```text
error_x = cx - W/2
```

Jika `error_x > threshold`, robot belok kanan. Jika `error_x < -threshold`, robot belok kiri. Jika objek terlalu kecil, robot maju.

### Topic ROS

```text
/yolo/detections -> /cmd_vel
```

### Output

1. Robot atau simulasi bergerak mengikuti objek.
2. Grafik error posisi objek.
3. Demo video real-time.

### Pengembangan Lanjut

Gunakan PID untuk kontrol gerak halus.

---

## Project 5: Deteksi Rambu Mini untuk Mobile Robot

### Deskripsi

Membuat sistem deteksi rambu mini seperti stop, kiri, kanan, maju, dan parkir untuk robot mobile indoor.

### Class

```text
stop
belok_kiri
belok_kanan
maju
parkir
```

### Dataset

Cetak rambu mini dan ambil gambar dari jarak 30 cm sampai 2 m. Tambahkan variasi blur dan pencahayaan.

### Output

1. YOLO pendeteksi rambu.
2. Node interpretasi perintah robot.
3. Demo robot merespons rambu.

### Pengembangan Lanjut

Integrasikan ke line follower sehingga rambu memengaruhi keputusan di persimpangan.

---

## Project 6: Deteksi Sampah untuk Robot Pemilah

### Deskripsi

Sistem mendeteksi jenis sampah untuk simulasi robot pemilah. Cocok untuk proyek otomasi ramah lingkungan.

### Class

```text
botol_plastik
kaleng
kertas
kardus
sampah_organik
```

### Dataset

Gunakan benda bersih dan aman. Ambil gambar di meja, lantai, dan conveyor mini.

### Output

1. Model deteksi jenis sampah.
2. Dashboard jumlah objek per class.
3. Demo pemilahan virtual atau nyata.

### Pengembangan Lanjut

Tambahkan estimasi posisi objek untuk pick-and-place.

---

## Project 7: Deteksi Cacat Produk Mini Conveyor

### Deskripsi

Mendeteksi produk normal dan cacat pada conveyor mini. Contoh cacat: label miring, tutup hilang, bentuk penyok, warna salah.

### Class

```text
produk_normal
label_miring
tutup_hilang
produk_penyok
warna_salah
```

### Dataset

Buat skenario produk normal dan cacat buatan. Ambil gambar saat diam dan bergerak di conveyor.

### Output

1. Deteksi produk cacat.
2. Counter produk normal/cacat.
3. Demo reject decision.

### Pengembangan Lanjut

Hubungkan ke aktuator pendorong untuk menolak produk cacat.

---

## Project 8: Deteksi Jalur dan Halangan Robot Indoor

### Deskripsi

YOLO dipakai untuk mendeteksi jalur, cone, penghalang, dan area aman. Proyek ini cocok untuk mobile robot indoor.

### Class

```text
jalur
cone
halangan
area_aman
area_terlarang
```

### Tantangan

1. Jalur dapat berubah bentuk.
2. Objek halangan beragam.
3. Kamera robot bergerak menghasilkan blur.

### Output

1. Deteksi objek navigasi.
2. Topic status navigasi.
3. Demo robot menghindari halangan sederhana.

### Pengembangan Lanjut

Gabungkan YOLO dengan sensor LiDAR atau ultrasonik.

---

## Project 9: Deteksi Pose Objek untuk Pick and Place

### Deskripsi

Sistem mendeteksi objek dan memperkirakan titik ambil sederhana dari bounding box. Cocok untuk dasar robot arm pick-and-place.

### Class

```text
kubus
silinder
bola
part_A
part_B
```

### Estimasi Titik Ambil

Dari bounding box:

```text
grasp_x = (x_min + x_max) / 2
grasp_y = (y_min + y_max) / 2
```

Jika kamera sudah dikalibrasi, koordinat pixel dapat dikonversi ke koordinat dunia.

### Output

1. Bounding box objek.
2. Titik pusat objek.
3. Simulasi perintah pick.

### Pengembangan Lanjut

Tambahkan kalibrasi kamera dan transformasi TF ROS.

---

## Project 10: Benchmark YOLOv8n, YOLOv8s, dan YOLOv8m untuk Robotika

### Deskripsi

Membandingkan performa beberapa ukuran model YOLO pada dataset yang sama. Fokus pada trade-off akurasi dan kecepatan.

### Model

```text
YOLOv8n
YOLOv8s
YOLOv8m
```

### Eksperimen

Latih semua model dengan parameter sama:

```text
epochs = 50
imgsz = 640
batch = sesuai GPU
```

### Tabel Hasil

| Model | Precision | Recall | mAP50 | mAP50-95 | FPS | Ukuran file |
|---|---:|---:|---:|---:|---:|---:|
| YOLOv8n | | | | | | |
| YOLOv8s | | | | | | |
| YOLOv8m | | | | | | |

### Output

1. Analisis model terbaik untuk robot real-time.
2. Grafik akurasi vs kecepatan.
3. Rekomendasi model untuk CPU/GPU.

### Pengembangan Lanjut

Export ke ONNX atau TensorRT untuk deployment lebih cepat.

---

## Kriteria Penilaian Mini Project

| Aspek | Bobot |
|---|---:|
| Ide dan relevansi robotika | 10% |
| Dataset dan anotasi | 20% |
| Training dan evaluasi | 20% |
| Integrasi ROS | 20% |
| Demo dan keberhasilan sistem | 15% |
| Analisis hasil | 10% |
| Dokumentasi | 5% |

---

## Rekomendasi Pemilihan Project

| Kondisi Kelompok | Project disarankan |
|---|---|
| Pemula | Project 1, 2, 5 |
| Minat mobile robot | Project 4, 5, 8 |
| Minat robot arm | Project 1, 6, 9 |
| Minat industri | Project 3, 7, 10 |
| Punya GPU kuat | Project 10 |
| Waktu terbatas | Project 1 atau 2 |
