# TUTORIAL TRAINING YOLOv8 DI LAPTOP SENDIRI (GTX/RTX)

## PENDAHULUAN
Training YOLOv8 di laptop dengan GPU NVIDIA (GTX/RTX) memberikan kontrol penuh dan tanpa batasan waktu seperti di Colab. Tutorial ini untuk Ubuntu 22.04 dengan ROS 2 Humble.

---

## LANGKAH 1: CEK GPU NVIDIA

```bash
# Cek apakah GPU terdeteksi
lspci | grep -i nvidia

# Cek driver NVIDIA
nvidia-smi
```

**Output yang diharapkan**:
```
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 535.xx       Driver Version: 535.xx       CUDA Version: 12.x     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  NVIDIA GeForce ...  Off  | 00000000:01:00.0  On |                  N/A |
|  0%   45C    P8    10W / 130W |    320MiB /  8192MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+
```

Jika `nvidia-smi` tidak ada, install driver:
```bash
sudo ubuntu-drivers autoinstall
sudo reboot
```

---

## LANGKAH 2: INSTALL CUDA & CUDNN (JIKA BELUM)

### 2.1 Install CUDA Toolkit
```bash
# Cek CUDA version yang compatible dengan driver
nvidia-smi  # Lihat CUDA Version

# Install CUDA (contoh CUDA 12.1)
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt update
sudo apt install cuda-toolkit-12-1
```

### 2.2 Tambah CUDA ke PATH
```bash
echo 'export PATH=/usr/local/cuda-12.1/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/cuda-12.1/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc

# Verify
nvcc --version
```

### 2.3 Install cuDNN (Optional tapi recommended)
```bash
# Download dari: https://developer.nvidia.com/cudnn
# Atau install via apt
sudo apt install libcudnn8 libcudnn8-dev
```

---

## LANGKAH 3: INSTALL PYTHON ENVIRONMENT

### 3.1 Buat Virtual Environment (Disarankan)
```bash
cd /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO

# Buat venv
python3 -m venv yolo_env

# Aktifkan
source yolo_env/bin/activate

# Update pip
pip install --upgrade pip
```

### 3.2 Install PyTorch dengan CUDA Support
```bash
# Install PyTorch dengan CUDA (sesuaikan versi CUDA)
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

# Verifikasi CUDA PyTorch
python3 -c "import torch; print(f'PyTorch: {torch.__version__}'); print(f'CUDA available: {torch.cuda.is_available()}'); print(f'GPU: {torch.cuda.get_device_name(0) if torch.cuda.is_available() else \"No GPU\"}')"
```

Output harus: `CUDA available: True`

---

## LANGKAH 4: INSTALL ULTRALYTICS YOLOv8

```bash
# Install YOLOv8
pip install ultralytics

# Verify
yolo version
# Atau
python3 -c "from ultralytics import YOLO; print('YOLOv8 installed!')"
```

---

## LANGKAH 5: PERSIAPAN DATASET

```bash
# Dataset harus di folder training_data
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data"

# Cek struktur
tree -L 3
```

Struktur harus seperti ini:
```
training_data/
├── data.yaml
├── train/
│   ├── images/
│   │   ├── img001.jpg
│   │   ├── img002.jpg
│   │   └── ...
│   └── labels/
│       ├── img001.txt
│       ├── img002.txt
│       └── ...
├── valid/
│   ├── images/
│   └── labels/
└── test/ (opsional)
    ├── images/
    └── labels/
```

### Jika belum ada dataset, buat struktur:
```bash
mkdir -p {train,valid,test}/{images,labels}
```

Buat/edit `data.yaml`:
```yaml
path: /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data
train: train/images
val: valid/images
test: test/images

names:
  0: kotak_merah
  1: silinder_hijau
  2: bola_biru
```

---

## LANGKAH 6: TRAINING MODEL

### 6.1 Training Dasar (YOLOv8n - Tercepat)
```bash
# Aktifkan environment (jika belum)
source /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/yolo_env/bin/activate

# Masuk ke folder dataset
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data"

# Training dengan YOLOv8n
yolo detect train \
  data=data.yaml \
  model=yolov8n.pt \
  epochs=50 \
  imgsz=640 \
  batch=16 \
  device=0 \
  name=yolov8n_local_gpu
```

### 6.2 Training dengan Python Script (Lebih Fleksibel)
```python
#!/usr/bin/env python3
from ultralytics import YOLO

# Load model
model = YOLO('yolov8n.pt')  # atau yolov8s.pt, yolov8m.pt (lebih akurat tapi lambat)

# Train
results = model.train(
    data='/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data/data.yaml',
    epochs=100,              # 50-100 untuk dataset kecil
    imgsz=640,              # Standard YOLO size
    batch=16,               # Naikkan jika VRAM cukup (32, 64)
    device=0,               # GPU 0 (atau 'cpu' jika tidak ada GPU)
    workers=8,              # Jumlah CPU threads
    optimizer='SGD',        # SGD (default) atau 'Adam'
    lr0=0.01,               # Learning rate awal
    lrf=0.01,               # Learning rate akhir
    momentum=0.937,
    weight_decay=0.0005,
    warmup_epochs=3.0,
    save=True,
    save_period=10,         # Simpan setiap 10 epoch
    cache=True,             # Cache gambar di RAM (lebih cepat)
    project='runs/detect',
    name='yolov8n_custom_local',
    exist_ok=True
)

print("Training selesai!")
print(f"Best model: {results.save_dir}/weights/best.pt")
```

Jalankan:
```bash
python3 train_local.py
```

---

## LANGKAH 7: MONITOR TRAINING

Training akan menampilkan progress:
```
Epoch   GPU_mem   box_loss   cls_loss   dfl_loss  Instances       Size
  1/50      2.1G       1.23       2.45       1.56         42        640: 100%|██████████| 50/50 [00:30<00:00,  1.67s/it]
  2/50      2.2G       1.12       2.23       1.45         38        640: 100%|██████████| 50/50 [00:28<00:00,  1.60s/it]
  ...
 
Best model saved to: runs/detect/yolov8n_custom_local/weights/best.pt
```

**TensorBoard** (Optional - untuk visualisasi):
```bash
tensorboard --logdir runs/detect/yolov8n_custom_local
# Buka di browser: http://localhost:6006
```

---

## LANGKAH 8: EVALUASI MODEL

```python
from ultralytics import YOLO

# Load model terbaik
model = YOLO('runs/detect/yolov8n_custom_local/weights/best.pt')

# Validate
metrics = model.val()
print(f"mAP50: {metrics.box.map50:.3f}")
print(f"mAP50-95: {metrics.box.map:.3f}")
print(f"Precision: {metrics.box.mp:.3f}")
print(f"Recall: {metrics.box.mr:.3f}")
```

---

## LANGKAH 9: TESTING MODEL

```python
from ultralytics import YOLO

model = YOLO('runs/detect/yolov8n_custom_local/weights/best.pt')

# Test pada folder gambar
results = model.predict(
    source='valid/images',
    save=True,
    conf=0.25,           # Confidence threshold
    iou=0.45,            # IoU threshold
    show_labels=True,
    show_conf=True,
    line_width=2
)

print("Hasil prediksi di: runs/detect/predict")
```

---

## LANGKAH 10: MENGGUNAKAN MODEL DI ROS

```bash
# Copy model ke folder models ROS
cp runs/detect/yolov8n_custom_local/weights/best.pt \
  "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/models/custom_yolov8.pt"

# Update kode ROS
# Edit file: yolo_camera_node.py atau yolo_node.py
# Ubah baris pemuatan model:
# Dari: self.model = YOLO('yolov8n.pt')
# Menjadi: self.model = YOLO('custom_yolov8.pt')

# Atau jalankan dengan parameter:
ros2 run yolo_ros yolo_camera_node --ros-args -p model_path:=/path/to/custom_yolov8.pt
```

---

## TIPS & BEST PRACTICES

### 1. Batch Size Berdasarkan VRAM
| GPU VRAM | Batch Size |
|----------|------------|
| 4 GB | 8 |
| 6 GB | 16 |
| 8 GB | 24 |
| 12 GB+ | 32-64 |

Jika error **CUDA out of memory**, kurangi batch size.

### 2. Model Size Selection
| Model | Size | Speed | Accuracy |
|-------|------|-------|----------|
| YOLOv8n | 6.2 MB | Sangat Cepat | Cukup |
| YOLOv8s | 22.5 MB | Cepat | Baik |
| YOLOv8m | 52.0 MB | Sedang | Lebih Baik |
| YOLOv8l | 83.7 MB | Lambat | Sangat Baik |
| YOLOv8x | 130.5 MB | Sangat Lambat | Terbaik |

### 3. Overfitting Prevention
- Jika `train loss` turun tapi `val loss` naik → Overfitting
- Solusi: Stop training lebih awal, tambah data, atau kurangi epochs

---

## TROUBLESHOOTING

### Error: "CUDA out of memory"
```python
# Kurangi batch size
model.train(data='data.yaml', batch=8)  # Dari 16 ke 8
```

### Error: "libcudart.so not found"
```bash
# Cek CUDA path
echo $LD_LIBRARY_PATH

# Tambah jika kosong
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```

### Error: "Dataset not found"
- Pastikan path di `data.yaml` menggunakan absolute path
- Cek apakah file gambar ada di folder yang disebutkan

### Training sangat lambat
- Pastikan menggunakan GPU: `torch.cuda.is_available()` harus True
- Cek `nvidia-smi` saat training, harus ada proses `python` menggunakan GPU

---

## RINGKASAN ALUR

```
1. Cek GPU (nvidia-smi) → 2. Install driver/CUDA (jika perlu) →
3. Buat Python venv → 4. Install PyTorch + CUDA → 5. Install Ultralytics →
6. Siapkan dataset (YOLO format) → 7. Train (yolo detect train) →
8. Monitor progress → 9. Evaluasi (model.val()) →
10. Copy best.pt ke ROS models → 11. Update kode ROS →
12. Testing dengan ros2 run!
```

---

## REFERENSI
- Ultralytics Docs: https://docs.ultralytics.com
- PyTorch CUDA: https://pytorch.org/get-started/locally/
- NVIDIA CUDA: https://developer.nvidia.com/cuda-toolkit
