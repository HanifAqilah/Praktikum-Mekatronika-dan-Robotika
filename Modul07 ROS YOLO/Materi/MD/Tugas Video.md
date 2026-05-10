# Tugas Video Modul 07: ROS YOLO

Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi  
Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum  
Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.  
Deadline: 2 minggu setelah praktikum

## 1. Tujuan Tugas

Tugas ini meminta praktikan merekam seluruh proses belajar dan praktik dari awal sampai akhir dalam satu video YouTube. Video bukan hanya bukti hasil akhir, tetapi bukti proses berpikir, proses setup, proses error, proses perbaikan, dan pemahaman teori YOLO serta ROS 2.

**WAJIB: Upload ke YouTube dengan durasi minimal 15 menit**

Video harus mencakup:

1. Penjelasan teori dasar neural network, CNN, object detection, dan YOLO.
2. Proses instalasi, dataset, training, evaluasi, dan running ROS YOLO.
3. Penjelasan program/node ROS yang digunakan.
4. Bukti demo sistem berhasil running.
5. Refleksi kendala dan solusi.

---

## 2. Bentuk Pengumpulan

**HANYA SUBMIT LINK YOUTUBE**

Format pengumpulan:

```text
Nama Kelompok: [Nama]
Anggota: [Daftar nama]
Link YouTube: https://youtu.be/[video_id]
Durasi: [menit:detik]
Tanggal upload: [tanggal]
```

**TIDAK ADA TUGAS LAPORAN, KETIK, ATAU PRINT**

---

## 3. Ketentuan Video YouTube

### Durasi

**WAJIB MINIMAL 15 MENIT**

Rekomendasi durasi:

```text
15-30 menit
```

Jika proses asli sangat panjang, percepat bagian menunggu training, tetapi jangan hilangkan tahapan penting.

### Format Video

1. **Satu video utuh** yang menggabungkan teori dan praktik.
2. Upload ke YouTube (public atau unlisted).
3. Judul video: `[Kelompok XX] Modul 07 ROS YOLO - Praktikum Mekatronika`.
4. Deskripsi video berisi: nama kelompok, anggota, objek yang dideteksi, tools yang dipakai.

### Isi Wajib Video

Video harus menunjukkan:

**Bagian 1: Teori (3-5 menit)**

Praktikan menjelaskan dengan suara sendiri:

1. Apa itu citra digital dan neural network.
2. Rumus neuron: `z = w^T x + b`, `a = φ(z)`.
3. Fungsi aktivasi ReLU: `ReLU(z) = max(0,z)`.
4. Apa itu CNN dan konvolusi.
5. Apa itu object detection.
6. Format bounding box YOLO: `class_id x_center y_center width height`.
7. Rumus normalisasi bounding box.
8. Apa itu IoU: `IoU = Area intersection / Area union`.
9. Precision, recall, mAP.
10. Prinsip YOLO: You Only Look Once.

**Bagian 2: Praktik (10-25 menit)**

1. Identitas kelompok.
2. Cek environment awal.
3. Akuisisi gambar dataset.
4. Proses anotasi bounding box.
5. Export dataset YOLO.
6. Validasi struktur dataset.
7. Training YOLOv8.
8. Evaluasi hasil training.
9. Copy model `best.pt` ke ROS workspace.
10. `colcon build`.
11. `source install/setup.bash`.
12. Running node YOLO.
13. Demo deteksi objek real-time.
14. Penjelasan program `camera_node`, `yolo_node`, atau launch file.
15. Kendala/error yang muncul dan cara menyelesaikannya.
16. Kesimpulan hasil.

---

## 4. Skenario Video yang Disarankan

Gunakan struktur berikut agar video runtut.

### Bagian 1: Pembukaan (1 menit)

Tampilkan slide atau terminal berisi:

```text
Modul 07 ROS YOLO
Nama kelompok:
Anggota:
Objek yang dideteksi:
Tool anotasi:
Platform training:
```

### Bagian 2: Teori (3-5 menit)

Jelaskan konsep dasar dengan screen recording atau slide:

1. Neural network dan rumus neuron.
2. CNN dan konvolusi.
3. Object detection dan YOLO.
4. Format label YOLO.
5. IoU, precision, recall, mAP.

### Bagian 3: Environment (1 menit)

Rekam terminal:

```bash
ros2 --version
python3 --version
python3 -c "from ultralytics import YOLO; print('YOLO ready')"
```

Jika memakai GPU:

```bash
nvidia-smi
```

Atau di Colab:

```python
import torch
print(torch.cuda.is_available())
```

### Bagian 4: Dataset (2-3 menit)

Tampilkan:

1. Folder gambar mentah.
2. Contoh gambar.
3. Jumlah gambar per class.
4. Variasi kondisi pengambilan.

### Bagian 5: Anotasi (2-3 menit)

Tampilkan Roboflow/CVAT/LabelStudio:

1. Membuat project.
2. Membuat label.
3. Menggambar bounding box.
4. Export YOLOv8.
5. Download dataset.

### Bagian 6: Training (3-4 menit)

Tampilkan:

```python
from ultralytics import YOLO
model = YOLO('yolov8n.pt')
model.train(data='data.yaml', epochs=50, imgsz=640, batch=16)
```

Tunjukkan output training dan lokasi `best.pt`.

### Bagian 7: Evaluasi (1-2 menit)

Tunjukkan:

```python
metrics = model.val()
```

Catat precision, recall, mAP50, mAP50-95.

### Bagian 8: ROS YOLO (2-3 menit)

Tampilkan:

```bash
colcon build --symlink-install
source install/setup.bash
ros2 run yolo_ros yolo_camera_node
```

### Bagian 9: Demo (2-4 menit)

Tampilkan objek di depan kamera:

1. Bounding box muncul.
2. Label class benar.
3. Confidence terlihat.
4. Beberapa jarak dan sudut.
5. Multi-objek jika memungkinkan.

### Bagian 10: Penutup (1-2 menit)

Jelaskan hasil, kendala, solusi, dan kesimpulan.

---

## 5. Informasi yang Harus Ada di Deskripsi Video YouTube

Tulis di deskripsi video YouTube:

```text
Modul 07: ROS YOLO untuk Praktikum Mekatronika

Nama kelompok: [nama]
Anggota:
1. [nama]
2. [nama]
3. [nama]

Objek yang dideteksi: [list class]
Tool anotasi: [Roboflow/CVAT/LabelStudio]
Platform training: [Google Colab/VSCode Colab/GPU Lokal]
Model YOLO: [YOLOv8n/YOLOv8s/dll]

Jumlah gambar:
- Train: [jumlah]
- Valid: [jumlah]
- Test: [jumlah]

Hasil evaluasi:
- Precision: [nilai]
- Recall: [nilai]
- mAP50: [nilai]
- mAP50-95: [nilai]

Kendala utama: [jelaskan]
Solusi: [jelaskan]

Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi
Dosen: Rofiq Cahyo Prayogo, S.T., M.T.
```

---

## 6. Rubrik Penilaian Video YouTube

Total nilai: 100.

| Aspek | Bobot | Indikator |
|---|---:|---|
| Kelengkapan proses video | 20 | Semua tahap dari setup, dataset, training, evaluasi, sampai ROS running terekam jelas |
| Pemahaman teori | 15 | Mampu menjelaskan neural network, CNN, YOLO, bbox, IoU, precision, recall, mAP dengan benar |
| Dataset dan anotasi | 15 | Dataset cukup, variasi baik, bounding box rapi, label konsisten, struktur YOLO valid |
| Training dan evaluasi | 15 | Training berjalan, metrik ditampilkan, hasil dianalisis, `best.pt` tersedia |
| Integrasi ROS 2 | 15 | Workspace berhasil dibuild, node berjalan, topic aktif, deteksi real-time muncul |
| Penjelasan program | 8 | Menjelaskan node, topic, alur image, cv_bridge/OpenCV/YOLO, dan parameter utama |
| Analisis kendala dan solusi | 7 | Error/kendala dijelaskan dan ada solusi teknis yang masuk akal |
| Kualitas komunikasi video | 5 | Audio jelas, layar terbaca, alur rapi, durasi minimal 15 menit terpenuhi |

---

## 7. Level Penilaian Per Aspek

### A. Kelengkapan Proses Video, 20 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 17-20 | Semua tahapan terekam runtut, ada bukti command dan hasil |
| Baik | 13-16 | Mayoritas tahapan terekam, ada bagian kecil kurang detail |
| Cukup | 9-12 | Hanya sebagian proses, terlalu fokus hasil akhir |
| Kurang | 0-8 | Tidak menunjukkan proses nyata atau banyak tahap hilang |

### B. Pemahaman Teori, 15 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 13-15 | Menjelaskan konsep dan rumus dengan benar serta memberi contoh |
| Baik | 10-12 | Konsep utama benar, rumus sebagian dijelaskan |
| Cukup | 7-9 | Penjelasan umum, banyak istilah belum tepat |
| Kurang | 0-6 | Tidak mampu menjelaskan dasar YOLO/neural network |

### C. Dataset dan Anotasi, 15 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 13-15 | Dataset bervariasi, label rapi, split jelas, format valid |
| Baik | 10-12 | Dataset cukup baik tetapi variasi/anotasi belum optimal |
| Cukup | 7-9 | Dataset minim, masih ada label kurang konsisten |
| Kurang | 0-6 | Dataset tidak valid atau tidak dapat dipakai training |

### D. Training dan Evaluasi, 15 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 13-15 | Training berhasil, metrik lengkap, analisis kuat |
| Baik | 10-12 | Training berhasil, metrik ada, analisis sederhana |
| Cukup | 7-9 | Training ada tetapi evaluasi minim |
| Kurang | 0-6 | Tidak ada model valid atau training gagal tanpa solusi |

### E. Integrasi ROS 2, 15 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 13-15 | ROS build/run lancar, topic aktif, deteksi real-time stabil |
| Baik | 10-12 | ROS berjalan tetapi masih ada kendala kecil |
| Cukup | 7-9 | Hanya sebagian node berjalan |
| Kurang | 0-6 | Tidak berhasil menjalankan ROS YOLO |

### F. Penjelasan Program, 10 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 9-10 | Menjelaskan alur kode, node, topic, parameter, dan data image |
| Baik | 7-8 | Menjelaskan node/topic utama |
| Cukup | 5-6 | Penjelasan program masih umum |
| Kurang | 0-4 | Tidak memahami program yang dijalankan |

### G. Analisis Kendala, 7 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 6-7 | Kendala teknis nyata, penyebab, solusi, dan pembelajaran jelas |
| Baik | 4-5 | Kendala dan solusi disebutkan cukup jelas |
| Cukup | 2-3 | Kendala disebutkan tanpa analisis mendalam |
| Kurang | 0-1 | Tidak ada analisis kendala |

### H. Kualitas Komunikasi Video, 5 poin

| Level | Poin | Kriteria |
|---|---:|---|
| Sangat baik | 5 | Audio jelas, layar terbaca, alur rapi, durasi minimal 15 menit |
| Baik | 4 | Ada gangguan kecil tetapi masih jelas, durasi cukup |
| Cukup | 2-3 | Audio/visual kurang jelas pada beberapa bagian |
| Kurang | 0-1 | Sulit dipahami atau durasi kurang dari 15 menit |

---

## 8. Syarat Video Diterima

Video tidak diterima jika:

1. Durasi kurang dari 15 menit.
2. Hanya menampilkan hasil akhir tanpa proses.
3. Tidak ada suara penjelasan praktikan.
4. Tidak menunjukkan identitas kelompok.
5. Tidak menunjukkan training atau evaluasi model.
6. Tidak menunjukkan ROS YOLO running.
7. Link YouTube tidak dapat diakses.
8. Video hasil salinan kelompok lain.

---

## 9. Bonus Nilai

Bonus maksimal 10 poin, diberikan jika ada pengembangan berikut:

| Bonus | Poin |
|---|---:|
| Menghubungkan deteksi ke kontrol robot `/cmd_vel` atau aktuator | +4 |
| Membandingkan dua model YOLO berbeda | +2 |
| Menampilkan dashboard/log deteksi real-time | +2 |
| Export model ke ONNX/TensorRT atau optimasi FPS | +2 |

Bonus tidak menggantikan kewajiban utama.

---

## 10. Checklist Sebelum Submit

- [ ] Video YouTube minimal 15 menit.
- [ ] Audio jelas dan layar terbaca.
- [ ] Identitas kelompok ada.
- [ ] Penjelasan teori neural network, CNN, YOLO.
- [ ] Dataset dan anotasi ditunjukkan.
- [ ] Training dan evaluasi ditunjukkan.
- [ ] ROS build/run ditunjukkan.
- [ ] Demo deteksi real-time ditunjukkan.
- [ ] Deskripsi video YouTube lengkap.
- [ ] Link YouTube dapat diakses.
- [ ] Deadline 2 minggu setelah praktikum dipenuhi.
