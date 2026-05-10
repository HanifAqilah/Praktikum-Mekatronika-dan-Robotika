# TUTORIAL LENGKAP CVAT - COMPUTER VISION ANNOTATION TOOL

## PENDAHULUAN
CVAT (Computer Vision Annotation Tool) adalah tool open-source gratis dari Intel untuk annotasi data computer vision. Support berbagai task termasuk object detection untuk YOLO.

---

## LANGKAH 1: MENGAKSES CVAT

### 1.1 Via Web (Recommended - Paling Mudah)
1. Buka https://www.cvat.ai di browser
2. Klik **"Get Started"** atau **"Sign Up"**
3. Daftar dengan:
   - **Google Account** (paling cepat)
   - **GitHub Account**
   - **Email** (manual registration)
4. Setelah login, Anda masuk ke CVAT Dashboard

### 1.2 Install Lokal (Opsional - Advanced)
```bash
# Install Docker
sudo apt update
sudo apt install docker.io docker-compose

# Download CVAT
git clone https://github.com/opencv/cvat.git
cd cvat

# Run CVAT
docker-compose up -d

# Akses di http://localhost:8080
```

---

## LANGKAH 2: MEMBUAT PROJECT BARU

1. Di Dashboard CVAT, klik **"Create new project"**
2. Isi detail project:
   - **Name**: `YOLO_Modul07_Detection`
   - **Labels**: Klik **"Add label"** untuk menambah class
     ```
     Label 1: kotak_merah (color: red)
     Label 2: silinder_hijau (color: green)
     Label 3: bola_biru (color: blue)
     ```
   - **Select task categories**: Pilih **"Object Detection"**
3. Klik **"Submit"** untuk membuat project

### Tips Menambah Label:
- Klik icon **"+"** di sebelah Labels
- Isi **Name**: nama class
- Pilih **Color**: warna untuk visualisasi
- Klik **"Done"**

---

## LANGKAH 3: MENGUPLOAD GAMBAR

1. Masuk ke project yang baru dibuat
2. Klik tombol **"Upload files"** (pojok kanan atas)
3. Pilih metode upload:
   - **Local files**: Upload dari komputer
   - **Remote files**: Dari URL
   - **Cloud Storage**: Google Drive, AWS, dll
4. Pilih file gambar:
   - Klik **"Select files"**
   - Pilih semua gambar (bisa multiple select)
   - Atau drag & drop folder gambar
5. Klik **"Upload"**
6. Tunggu proses upload selesai

---

## LANGKAH 4: MELAKUKAN ANNOTASI

### 4.1 Membuka Job Annotation
1. Setelah upload selesai, akan muncul **"Job"**
2. Klik pada job tersebut untuk masuk ke halaman annotasi

### 4.2 Interface CVAT
```
| Toolbar Kiri | Canvas (Gambar) | Panel Kanan |
|--------------|-----------------|-------------|
| - Bounding Box| - View gambar  | - Labels    |
| - Polygon    | - Annotations  | - Attributes|
| - Polyline   |                 | - Object info|
```

### 4.3 Cara Membuat Bounding Box (Object Detection)

#### Metode 1: Menggunakan Toolbar
1. Klik ikon **"Rectangle/Bounding Box"** (kotak) di toolbar kiri
2. Di panel kanan, pilih label (contoh: `kotak_merah`)
3. Di kanvas gambar:
   - Klik di sudut kiri atas objek
   - Drag ke sudut kanan bawah
   - Lepas klik untuk menyelesaikan
4. Bounding box akan muncul dengan label yang dipilih

#### Metode 2: Keyboard Shortcut
1. Tekan tombol **`N`** untuk memilih Rectangle tool
2. Tekan angka untuk memilih label:
   - `1` untuk label pertama
   - `2` untuk label kedua, dst
3. Gambar bounding box seperti metode 1

### 4.4 Keyboard Shortcuts Penting
| Shortcut | Fungsi |
|----------|--------|
| `N` | Rectangle tool |
| `1`, `2`, `3`... | Pilih label (urutan) |
| `Del` | Hapus object terpilih |
| `Ctrl`+`Z` | Undo |
| `Ctrl`+`Shift`+`Z` | Redo |
| `A` | Gambar sebelumnya |
| `D` | Gambar berikutnya |
| `M` | Toggle side panel |
| `F` | Fullscreen mode |
| `Ctrl`+`S` | Save annotation |

### 4.5 Edit Bounding Box
- **Resize**: Klik pada bounding box, drag sudut/titik tengah
- **Move**: Drag di dalam bounding box
- **Change Label**: Di panel kanan, pilih label baru
- **Change Attributes**: Tambah atribut (jika ada) di panel kanan

### 4.6 Multiple Objects
- Satu gambar bisa memiliki banyak bounding box
- Ulangi proses untuk setiap objek
- Pastikan setiap objek memiliki label yang tepat

### 4.7 Navigasi Antar Gambar
- Klik panah **kiri/kanan** di bawah kanvas
- Atau tekan **A** (sebelumnya) / **D** (berikutnya)
- Progress akan tersimpan otomatis

---

## LANGKAH 5: MENYIMPAN DAN MENGEKSPORT

### 5.1 Save Annotation
- Tekan **Ctrl + S** secara berkala
- Atau klik ikon **Save** (disk) di pojok kiri atas
- CVAT auto-save setiap beberapa menit

### 5.2 Export Dataset
1. Klik tombol **"Menu"** (tiga garis) di pojok kiri atas
2. Pilih **"Export task dataset"**
3. Pilih format export:
   - **YOLO 1.1** (Recommended untuk YOLOv8)
   - Atau **COCO 1.0** (lalu konversi manual)
4. Pilih opsi:
   - ✓ **Save images** (penting!)
   - ✓ **Include last/valid annotations**
5. Klik **"Export"**
6. Tunggu proses persiapan (beberapa menit)
7. File ZIP akan otomatis terdownload

---

## LANGKAH 6: MENSTRUKTURKAN DATASET

Setelah download selesai:

```bash
# Extract file zip
cd ~/Downloads
unzip cvat_export_yolo_1.1.zip -d ~/cvat_dataset

# Cek struktur
tree ~/cvat_dataset
```

Struktur yang dihasilkan:
```
cvat_dataset/
├── data.yaml          # Konfigurasi dataset
├── obj.data           # (jika ada)
├── obj.names          # (jika ada)
├── train.txt          # List gambar training
├── valid.txt          # List gambar validation
├── images/
│   ├── train/         # Gambar training
│   └── val/           # Gambar validation
└── labels/
    ├── train/         # Label YOLO format
    └── val/           # Label validation
```

### Perbaiki file data.yaml:
```yaml
path: /home/otomasi/cvat_dataset
train: images/train
val: images/val

names:
  0: kotak_merah
  1: silinder_hijau
  2: bola_biru
```

---

## LANGKAH 7: COPY KE ROS WORKSPACE

```bash
# Buat folder tujuan jika belum ada
mkdir -p /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/ROS_YOLO/src/yolo_ros/training_data

# Copy dataset
cp -r ~/cvat_dataset/* \
  /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/ROS_YOLO/src/yolo_ros/training_data/

# Atau ke folder roboflow_data (jika ingin terpisah)
cp -r ~/cvat_dataset \
  /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/ROS_YOLO/src/yolo_ros/roboflow_data/cvat_export
```

---

## LANGKAH 8: VALIDASI DATASET

```python
#!/usr/bin/env python3
import yaml
import os

dataset_path = '/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data'

# Cek file yaml
yaml_files = [f for f in os.listdir(dataset_path) if f.endswith('.yaml')]
print(f"YAML files found: {yaml_files}")

if yaml_files:
    with open(os.path.join(dataset_path, yaml_files[0]), 'r') as f:
        data = yaml.safe_load(f)
    print(f"\nDataset config:")
    print(f"Path: {data.get('path', 'NOT SET')}")
    print(f"Classes: {data.get('names', {})}")
    
    # Hitung gambar
    for split in ['train', 'val', 'valid', 'test']:
        img_dir = os.path.join(dataset_path, 'images', split)
        if os.path.exists(img_dir):
            count = len([f for f in os.listdir(img_dir) if f.endswith(('.jpg', '.png'))])
            print(f"{split}: {count} images")
```

---

## TIPS & BEST PRACTICES

### 1. Kualitas Annotation
- **Pas**: Bounding box harus pas mengelilingi objek
- **Complete**: Pastikan seluruh objek tercover
- **Consistent**: Gunakan standar yang sama untuk semua gambar

### 2. Handle Kasus Khusus
- **Occlusion** (tertutup): Label jika masih terlihat >50%
- **Truncation** (terpotong): Label sampai batas frame
- **Small objects**: Tetap label meski kecil (penting untuk training)

### 3. Review & Quality Control
- Setelah selesai, review lagi semua annotation
- Cek ada yang terlewat atau salah label
- Gunakan fitur **"Validation"** di CVAT

---

## TROUBLESHOOTING

### Error: "Export failed"
- Pastikan semua gambar sudah diannotasi
- Coba export per job (jika dataset besar)

### Error: "Images not found during training"
- Cek path di `data.yaml`, gunakan absolute path
- Pastikan folder `images` dan `labels` sejajar

### Label tidak muncul
- Pastikan sudah memilih label SEBELUM menggambar bounding box
- Cek di panel kanan apakah label terassign

---

## PERBANDINGAN CVAT vs ROBOFLOW vs LABELSTUDIO

| Fitur | CVAT | Roboflow | LabelStudio |
|-------|------|----------|-------------|
| Harga | Gratis (Open Source) | Freemium | Gratis (Open Source) |
| Web-based | ✓ | ✓ | ✓ |
| Local Install | ✓ (Docker) | ✗ | ✓ (Python) |
| YOLO Export | ✓ (YOLO 1.1) | ✓ (Native) | ✓ (via script) |
| Collaboration | ✓ | ✓ (paid) | ✓ |
| AI Assist | ✓ | ✓ (paid) | ✓ |

---

## RINGKASAN ALUR KERJA

```
1. Akses CVAT.ai → 2. Create Project (+Labels) → 3. Upload Images →
4. Annotate (Draw BBox + Select Label) → 5. Save (Ctrl+S) →
6. Export (YOLO 1.1 format) → 7. Download ZIP →
8. Extract → 9. Fix data.yaml → 10. Copy to ROS →
11. Validate → 12. Ready for Training!
```

---

## REFERENSI
- CVAT Docs: https://opencv.github.io/cvat/docs/
- CVAT GitHub: https://github.com/opencv/cvat
- Video Tutorial: https://www.youtube.com/c/CVATai
