# TUTORIAL LENGKAP ROBOFLOW - ANNOTASI DATA OBJEK

## PENDAHULUAN
Roboflow adalah platform untuk mengelola dataset computer vision. Tutorial ini akan memandu Anda membuat dataset custom untuk YOLOv8 dari awal hingga siap training.

---

## LANGKAH 1: MEMBUAT AKUN ROBOFLOW

1. Buka https://roboflow.com di browser
2. Klik tombol **"Sign Up"** di pojok kanan atas
3. Pilih metode pendaftaran:
   - Sign up with Email
   - Sign up with Google
   - Sign up with GitHub
4. Isi data registrasi:
   - Email: your_email@email.com
   - Password: (buat password kuat)
   - Username: (pilih username unik)
5. Verifikasi email melalui link yang dikirim ke email Anda
6. Setelah login, Anda akan masuk ke dashboard Roboflow

---

## LANGKAH 2: MEMBUAT PROJECT BARU

1. Di dashboard utama, klik tombol **"Create New Project"**
2. Isi detail project:
   - **Project Name**: `YOLO_Project_Modul07` (atau nama bebas)
   - **Project Type**: Pilih **"Object Detection"** (untuk YOLO)
   - **Annotation Group**: Biarkan default atau buat grup baru
   - **Description**: "Dataset untuk deteksi objek robotika"
3. Klik **"Create Project"**

---

## LANGKAH 3: MENGUMPULKAN GAMBAR

### 3.1 Mengambil Gambar dari Gazebo
```bash
# Buka Gazebo dengan world yang berisi objek
ros2 launch gazebo_ros gazebo.launch.py

# Buka RViz dan simpan frame kamera
# Atau gunakan node kamera untuk menyimpan gambar
```

### 3.2 Mengambil Gambar dari Kamera Laptop/USB
```python
#!/usr/bin/env python3
import cv2

cap = cv2.VideoCapture(0)  # 0 untuk webcam default
count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    cv2.imshow('Capture - Tekan SPASI untuk simpan, Q untuk keluar', frame)
    
    key = cv2.waitKey(1) & 0xFF
    if key == ord(' '):  # Spasi untuk simpan
        filename = f'image_{count:04d}.jpg'
        cv2.imwrite(filename, frame)
        print(f'Disimpan: {filename}')
        count += 1
    elif key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
```

### 3.3 Jumlah Minimal Gambar
- **50-100 gambar** per class (recommended)
- Pastikan variasi:
  - Sudut pengambilan (kiri, kanan, depan, atas)
  - Pencahayaan (terang, redup)
  - Posisi objek (dekat, jauh, miring)

---

## LANGKAH 4: UPLOAD GAMBAR KE ROBOFLOW

1. Di halaman project Roboflow, klik **"Upload Images"**
2. Pilih metode upload:
   - **Drag & Drop**: Seret folder gambar ke area upload
   - **Select Files**: Klik untuk pilih file satu per satu
   - **Upload Directory**: Upload seluruh folder sekaligus
3. Tunggu proses upload selesai
4. Klik **"Done Uploading"**

---

## LANGKAH 5: MEMBUAT ANNOTASI (LABELING)

### 5.1 Memulai Annotation
1. Klik tombol **"Annotate"** di menu atas
2. Pilih gambar pertama dari daftar
3. Anda akan masuk ke halaman annotation tool

### 5.2 Cara Melakukan Annotation
1. **Pilih Tool**: Klik ikon **"Bounding Box"** (kotak) di toolbar kiri
2. **Membuat Bounding Box**:
   - Klik dan drag di atas objek yang ingin dilabeli
   - Buat kotak yang mengelilingi seluruh objek
3. **Memberikan Label**:
   - Setelah kotak dibuat, akan muncul dialog label
   - Ketik nama class (contoh: `botol`, `kotak`, `bola`)
   - Tekan **Enter** atau klik **"Add"**
4. **Multiple Objects**:
   - Ulangi langkah 2-3 untuk setiap objek dalam gambar
   - Satu gambar bisa memiliki banyak bounding box

### 5.3 Tips Annotation yang Baik
- Pastikan bounding box **pas** mengelilingi objek (tidak terlalu longgar/tight)
- Jika objek terpotong di tepi gambar, tetap beri label (Roboflow akan otomatis menyesuaikan)
- Untuk objek yang tertutup sebagian (occlusion), label seperlunya saja
- Gunakan **shortcut keyboard**:
  - `A` / `D`: Navigasi gambar sebelumnya/selanjutnya
  - `Delete`: Hapus bounding box yang dipilih

### 5.4 Contoh Struktur Label
```
Class 1: kotak_merah
Class 2: silinder_hijau
Class 3: bola_biru
```

---

## LANGKAH 6: MENGEKSPORT DATASET

1. Setelah semua gambar diannotasi, klik **"Dataset"** di menu atas
2. Klik tombol **"Export Dataset"** (pojok kanan atas)
3. Pilih format export:
   - **Format**: Pilih **"YOLO v8"** (paling compatible)
   - **Version**: Pilih **"Latest"** atau versi spesifik
4. Pilih opsi tambahan:
   - ✓ **Preprocessing**: Resize (640x640) - untuk YOLOv8
   - ✓ **Augmentation**: (opsional) untuk menambah variasi data
5. Klik **"Continue"**
6. Pilih **"Download zip to computer"**
7. Tunggu proses persiapan dataset (beberapa menit)
8. File zip akan terdownload (contoh: `YOLO_Project_Modul07-3.zip`)

---

## LANGKAH 7: MENSTRUKTURKAN DATASET HASIL DOWNLOAD

Setelah download selesai:

```bash
# Extract file zip
unzip YOLO_Project_Modul07-3.zip -d ~/roboflow_dataset

# Struktur folder yang dihasilkan:
# ~/roboflow_dataset/
#   ├── data.yaml          # Konfigurasi dataset
#   ├── train/
#   │   ├── images/        # Gambar training
#   │   └── labels/        # Label YOLO format
#   ├── valid/
#   │   ├── images/
#   │   └── labels/
#   └── test/
#       ├── images/
#       └── labels/
```

### Memahami file data.yaml:
```yaml
path: /home/otomasi/roboflow_dataset
train: train/images
val: valid/images
test: test/images

names:
  0: kotak_merah
  1: silinder_hijau
  2: bola_biru
```

---

## LANGKAH 8: MEMINDAHKAN KE ROS WORKSPACE

```bash
# Copy ke folder training_data ROS
cp -r ~/roboflow_dataset/* \
  "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data/"

# Atau untuk roboflow_data folder (jika ingin terpisah)
cp -r ~/roboflow_dataset \
  "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/roboflow_data/"
```

---

## LANGKAH 9: VERIFIKASI DATASET

```python
#!/usr/bin/env python3
import yaml
import os

# Cek struktur dataset
dataset_path = '/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data'

# Baca file yaml
with open(os.path.join(dataset_path, 'data.yaml'), 'r') as f:
    data = yaml.safe_load(f)

print("Dataset Configuration:")
print(f"Path: {data['path']}")
print(f"Classes: {data['names']}")

# Hitung jumlah gambar
for split in ['train', 'val', 'test']:
    img_dir = os.path.join(dataset_path, split, 'images')
    if os.path.exists(img_dir):
        count = len([f for f in os.listdir(img_dir) if f.endswith('.jpg')])
        print(f"{split}: {count} images")

print("\nDataset siap untuk training!")
```

---

## TROUBLESHOOTING

### Error: "Images not found"
- Pastikan path di `data.yaml` sudah benar
- Gunakan path absolut (full path)

### Error: "Class names mismatch"
- Pastikan nama class di `data.yaml` sesuai dengan label saat annotasi

### Gambar terlalu sedikit
- Tambahkan augmentasi di Roboflow sebelum export
- Atau upload gambar tambahan dan lakukan annotasi lagi

---

## RINGKASAN ALUR KERJA

```
1. Daftar Roboflow → 2. Buat Project → 3. Upload Gambar → 
4. Annotasi (Bounding Box + Label) → 5. Export (YOLOv8 format) → 
6. Download ZIP → 7. Extract → 8. Copy ke ROS workspace → 
9. Verifikasi → 10. Siap Training!
```

---

## REFERENSI
- Roboflow Docs: https://docs.roboflow.com
- Roboflow Universe (dataset publik): https://universe.roboflow.com
