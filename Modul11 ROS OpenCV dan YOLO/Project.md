# PROJECT MODUL 10: SMART ROBOT VISION SYSTEM

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 10 – ROS OpenCV dan YOLO  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Sebuah gudang automated membutuhkan **robot pendeteksi dan penyortir** barang berdasarkan warna dan jenis. Robot bergerak di area gudang dan harus:
> - Mendeteksi **3 jenis objek** (kotak merah, silinder hijau, bola biru) menggunakan kombinasi color detection + YOLO
> - Mendekati objek terdekat menggunakan visual servoing
> - Menghitung jumlah setiap jenis objek yang terlihat
> - Menampilkan **dashboard monitoring** di layar terpisah
>
> Semua dilakukan dalam simulasi Gazebo + ROS 2.

---

## B. SOAL PROJECT

### Soal 1 — Multi-Object Color Detection

> Deteksi 3 jenis objek berdasarkan warna (merah, hijau, biru) secara simultan. Setiap objek ditandai dengan bounding box dan label warna.
>
> **Output:** Node ROS 2, screenshot deteksi 3 warna simultan.

### Soal 2 — YOLO Custom Class

> Train atau fine-tune YOLOv8 pada dataset custom (minimal 3 class: box, cylinder, ball) menggunakan minimal 50 gambar per class dari Gazebo.
>
> Alternatif: gunakan pre-trained model + filter hanya class tertentu.
>
> **Output:** Model terlatih / filter konfigurasi, screenshot deteksi.

### Soal 3 — Visual Servoing: Approach Object

> Robot mendekati objek terdeteksi:
> - Pilih objek terdekat (area terbesar di frame)
> - Navigasi menuju objek sampai jarak threshold (area > target)
> - Berhenti di depan objek, tampilkan "Object reached: [nama]"
>
> **Output:** Video robot mendekati objek, kode node.

### Soal 4 — Object Counter Dashboard

> Buat node dashboard (atau terminal display) yang menampilkan:
> - Total objek terdeteksi per class (hitung unik, bukan per frame)
> - Objek yang sudah "dikunjungi" vs belum
> - Map posisi objek (opsional: simple grid di terminal)
>
> **Output:** Screenshot dashboard, kode node.

### Soal 5 — Full Integration

> Buat launch file yang menjalankan seluruh sistem:
> - Gazebo + world + objek berwarna
> - Robot URDF + kamera
> - YOLO detection node
> - Visual servoing node
> - Dashboard node
> - RViz2
>
> **Output:** Launch file, video demo lengkap 2+ menit.

---

## C. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | Multi-color detection | 20 |
| 2 | YOLO detection | 20 |
| 3 | Visual servoing | 20 |
| 4 | Object counter dashboard | 15 |
| 5 | Full integration + launch | 15 |
| 6 | Dokumentasi | 10 |
| **TOTAL** | | **100** |

---

## D. REFERENSI

1. Bradski & Kaehler. *Learning OpenCV*. O'Reilly.
2. Ultralytics YOLOv8: https://docs.ultralytics.com
3. OpenCV Docs: https://docs.opencv.org
