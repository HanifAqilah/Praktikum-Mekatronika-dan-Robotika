# TUTORIAL TRAINING YOLOv8 DI GOOGLE COLAB (RUNTIME GPU)

## PENDAHULUAN
Google Colab menyediakan akses gratis ke GPU (Tesla T4/P100) untuk training model deep learning. Tutorial ini akan memandu training YOLOv8 menggunakan Google Colab dengan runtime GPU.

---

## LANGKAH 1: PERSIAPAN DATASET

### 1.1 Pastikan Dataset Sudah Siap
Dataset harus dalam format YOLOv8:
```
dataset_yolo/
├── data.yaml
├── train/
│   ├── images/      # File gambar .jpg/.png
│   └── labels/      # File label .txt (YOLO format)
├── valid/
│   ├── images/
│   └── labels/
└── test/ (opsional)
    ├── images/
    └── labels/
```

### 1.2 Upload Dataset ke Google Drive
```bash
# Di laptop lokal, compress dataset
cd /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/ROS_YOLO/src/yolo_ros/training_data
zip -r dataset_yolo.zip .

# Upload dataset_yolo.zip ke Google Drive
# Buka https://drive.google.com → Upload file
```

Atau langsung di Google Drive:
1. Buka https://drive.google.com
2. Buat folder `YOLO_Dataset`
3. Upload semua file dataset (drag & drop)
4. Pastikan struktur folder tetap terjaga

---

## LANGKAH 2: MEMBUKA GOOGLE COLAB

1. Buka https://colab.research.google.com
2. Login dengan akun Google
3. Anda akan masuk ke halaman **"Welcome to Colab"**
4. Klik **"New notebook"** (tombol biru)

---

## LANGKAH 3: MENGGANTI RUNTIME KE GPU

1. Di menu atas, klik **"Runtime"** → **"Change runtime type"**
2. Pada **"Runtime type"**: Biarkan **Python 3**
3. Pada **"Hardware accelerator"**: Pilih **"GPU"**
4. Pada **"GPU type"**: Biarkan **"Default"** (sistem akan pilih T4/P100)
5. Klik **"Save"**

**Verifikasi GPU**:
```python
import torch
print(f"PyTorch version: {torch.__version__}")
print(f"CUDA available: {torch.cuda.is_available()}")
print(f"GPU device: {torch.cuda.get_device_name(0) if torch.cuda.is_available() else 'No GPU'}")
```
Jalankan cell dengan **Shift + Enter**. Harusnya menunjukkan CUDA available: True dan nama GPU.

---

## LANGKAH 4: MOUNT GOOGLE DRIVE

```python
from google.colab import drive
drive.mount('/content/drive')
```

1. Jalankan cell (Shift + Enter)
2. Akan muncul link authorization
3. Klik link → Pilih akun Google → Izinkan akses
4. Copy kode authorization → Paste ke kolom di Colab → Tekan Enter
5. Muncul pesan: `Mounted at /content/drive`

**Cek dataset**:
```python
!ls /content/drive/MyDrive/YOLO_Dataset/
```

---

## LANGKAH 5: INSTALL ULTRALYTICS YOLOv8

```python
# Install Ultralytics
!pip install -q ultralytics

# Verify installation
from ultralytics import YOLO
print("YOLOv8 installed successfully!")
```

---

## LANGKAH 6: COPY DATASET KE COLAB

```python
import shutil
import os

# Path dataset di Drive
drive_dataset_path = '/content/drive/MyDrive/YOLO_Dataset'

# Copy ke local Colab (lebih cepat aksesnya)
!cp -r /content/drive/MyDrive/YOLO_Dataset /content/dataset_yolo

# Atau jika dalam zip
# !unzip -q /content/drive/MyDrive/dataset_yolo.zip -d /content/

# Cek struktur
!ls /content/dataset_yolo
!cat /content/dataset_yolo/data.yaml
```

---

## LANGKAH 7: TRAINING MODEL

### 7.1 Training Dasar (Recommended untuk Pemula)
```python
from ultralytics import YOLO

# Load model YOLOv8n (nano - tercepat)
model = YOLO('yolov8n.pt')

# Train model
results = model.train(
    data='/content/dataset_yolo/data.yaml',
    epochs=50,              # Jumlah epoch (50-100 untuk dataset kecil)
    imgsz=640,              # Image size (640x640 standard)
    batch=16,               # Batch size (16-32 untuk GPU Colab)
    name='yolov8n_custom',  # Nama experiment
    save=True,              # Simpan model terbaik
    device=0                # GPU device (0 = pertama)
)

print("Training selesai!")
```

### 7.2 Training Advanced (Lebih Powerful)
```python
from ultralytics import YOLO

model = YOLO('yolov8n.pt')

results = model.train(
    data='/content/dataset_yolo/data.yaml',
    epochs=100,
    imgsz=640,
    batch=32,                  # Naikkan jika GPU memory cukup
    optimizer='Adam',          # Optimizer (SGD/Adam)
    lr0=0.01,                  # Learning rate awal
    lrf=0.01,                  # Learning rate akhir
    momentum=0.937,
    weight_decay=0.0005,
    warmup_epochs=3.0,
    warmup_momentum=0.8,
    save=True,
    save_period=10,            # Simpan setiap 10 epoch
    cache=True,                # Cache gambar (lebih cepat)
    device=0,
    project='runs/detect',
    name='yolov8n_custom_v2',
    exist_ok=True
)
```

### 7.3 Monitor Training
Training akan menampilkan progress:
```
Epoch   GPU_mem   box_loss   cls_loss   dfl_loss  Instances       Size
  1/50       2G       1.23       2.45       1.56         42        640
  2/50      2.1G       1.12       2.23       1.45         38        640
  ...
  Best model saved to: /content/runs/detect/yolov8n_custom/weights/best.pt
```

---

## LANGKAH 8: EVALUASI MODEL

```python
# Load model terbaik
model = YOLO('/content/runs/detect/yolov8n_custom/weights/best.pt')

# Validate model
metrics = model.val()

print(f"mAP50: {metrics.box.map50:.3f}")
print(f"mAP50-95: {metrics.box.map:.3f}")
```

---

## LANGKAH 9: TESTING MODEL

```python
# Test dengan gambar dari dataset valid
model = YOLO('/content/runs/detect/yolov8n_custom/weights/best.pt')

# Predict pada folder gambar
results = model.predict(
    source='/content/dataset_yolo/valid/images',
    save=True,
    conf=0.25,           # Confidence threshold
    iou=0.45,            # IoU threshold
    show_labels=True,
    show_conf=True
)

print("Prediction selesai! Hasil di: /content/runs/detect/predict")
```

---

## LANGKAH 10: DOWNLOAD MODEL HASIL TRAINING

### 10.1 Download Lewat Colab (Zip)
```python
# Zip hasil training
!zip -r /content/yolov8n_custom_trained.zip /content/runs/detect/yolov8n_custom

# Download (akan muncul prompt download di browser)
from google.colab import files
files.download('/content/yolov8n_custom_trained.zip')
```

### 10.2 Upload ke Google Drive
```python
# Copy ke Drive
!cp -r /content/runs/detect/yolov8n_custom /content/drive/MyDrive/YOLO_Results/

print("Model tersimpan di Google Drive: /MyDrive/YOLO_Results/yolov8n_custom")
```

---

## LANGKAH 11: MENGGUNAKAN MODEL DI ROS

Setelah download selesai di laptop:

```bash
# Extract file zip
cd ~/Downloads
unzip yolov8n_custom_trained.zip

# Copy model ke ROS workspace
cp /content/runs/detect/yolov8n_custom/weights/best.pt \
  /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/ROS_YOLO/src/yolo_ros/models/custom_yolov8.pt

# Ganti model default di node YOLO
# Edit file yolo_node.py atau yolo_camera_node.py
# Ubah: model = YOLO('yolov8n.pt')
# Menjadi: model = YOLO('custom_yolov8.pt')
```

---

## TIPS & BEST PRACTICES

### 1. Dataset Size
- Minimal 50-100 gambar per class
- Lebih banyak = lebih akurat (tapi butuh waktu training lebih lama)

### 2. Augmentation (Data Augmentation)
Ultralytics otomatis melakukan augmentation:
- Flipping (horizontal/vertical)
- Rotation
- Scaling
- Mosaic (gabungan 4 gambar)

### 3. Epoch Management
- Dataset kecil (< 500 gambar): 50-100 epoch
- Dataset sedang (500-2000): 100-200 epoch
- Dataset besar (> 2000): 200-300 epoch

### 4. Overfitting Detection
- Jika `train loss` turun tapi `val loss` naik → Overfitting
- Solusi: Stop training, kurangi epoch, atau tambah data

---

## TROUBLESHOOTING

### Error: "CUDA out of memory"
```python
# Kurangi batch size
model.train(data='data.yaml', batch=8)  # Dari 16 jadi 8
```

### Error: "Dataset not found"
- Cek path di `data.yaml` gunakan absolute path
- Pastikan file `data.yaml` terbaca dengan benar

### Training terlalu lama
- Colab free memiliki limit waktu (12 jam)
- Simpan checkpoint dengan `save_period`
- Gunakan model yang lebih kecil (yolov8n vs yolov8s/m/l/x)

---

## RINGKASAN ALUR

```
1. Persiapkan dataset (YOLO format) → 2. Upload ke Google Drive →
3. Buka Colab → 4. Ganti runtime ke GPU → 5. Mount Google Drive →
6. Install Ultralytics → 7. Copy dataset ke Colab →
8. Train model (model.train()) → 9. Evaluasi (model.val()) →
10. Download model terbaik (best.pt) → 11. Copy ke ROS workspace →
12. Update kode ROS untuk pakai model baru!
```

---

## REFERENSI
- Ultralytics Docs: https://docs.ultralytics.com
- Colab Docs: https://colab.research.google.com
- YOLOv8 Training: https://docs.ultralytics.com/models/yolov8/
