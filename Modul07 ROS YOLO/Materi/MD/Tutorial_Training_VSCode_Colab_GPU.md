# TUTORIAL TRAINING YOLOv8 DI GOOGLE COLAB VIA VSCODE

## PENDAHULUAN
Menggunakan VSCode sebagai interface untuk Google Colab memberikan pengalaman coding yang lebih nyaman dengan fitur VSCode (auto-complete, linting, dll) sambil tetap menggunakan GPU Gratis dari Google Colab.

---

## LANGKAH 1: INSTALASI VSCODE & EXTENSION

### 1.1 Install VSCode (Jika Belum)
```bash
# Download dari: https://code.visualstudio.com
# Atau via snap (Ubuntu)
sudo snap install code --classic
```

### 1.2 Install Extension "Colab" di VSCode
1. Buka VSCode
2. Klik icon **Extensions** (Ctrl+Shift+X)
3. Search: **"Colab"**
4. Cari extension: **"Google Colab"** by Google
5. Klik **Install**

Atau install via CLI:
```bash
code --install-extension Google.colab
```

---

## LANGKAH 2: PERSIAPAN DATASET DI LOKAL

```bash
# Pastikan dataset sudah siap di lokal
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data"

# Cek struktur
tree -L 2
# Harusnya:
# ├── data.yaml
# ├── train/
# │   ├── images/
# │   └── labels/
# ├── valid/
# │   ├── images/
# │   └── labels/
# └── test/
```

### Upload Dataset ke Google Drive (Jika Belum)
```bash
# Buka browser, masuk ke https://drive.google.com
# Buat folder: YOLO_Dataset_Colab
# Upload semua isi folder training_data ke sana
```

---

## LANGKAH 3: MEMBUKA COLAB NOTEBOOK DI VSCODE

### 3.1 Metode 1: Dari VSCode
1. Buka VSCode
2. Klik icon **Google Colab** di sidebar kiri (jika sudah install extension)
3. Klik **"Sign in to Google Colab"**
4. Browser akan terbuka untuk authorize
5. Setelah authorize, kembali ke VSCode
6. Klik **"New Notebook"** atau **"Open Notebook"**

### 3.2 Metode 2: Convert dari Browser
1. Buka https://colab.research.google.com di browser
2. Buat notebook baru atau buka yang sudah ada
3. Klik **"File"** → **"Download"** → **"Download .ipynb"**
4. Buka file `.ipynb` tersebut di VSCode
5. VSCode akan otomatis mendeteksi sebagai Colab notebook

---

## LANGKAH 4: KONFIGURASI RUNTIME GPU DI VSCODE

1. Di VSCode, buka notebook Colab (`.ipynb`)
2. Klik **"Connect to a runtime"** (pojok kanan atas)
3. Pilih **"Connect to hosted runtime"**
4. Tunggu hingga status berubah menjadi **"Connected"** (hijau)

### Ganti ke GPU:
1. Klik **"Runtime"** → **"Change runtime type"**
2. **Runtime type**: Python 3
3. **Hardware accelerator**: Pilih **"GPU"**
4. Klik **"Save"**

### Verifikasi GPU:
```python
import torch
print(f"PyTorch: {torch.__version__}")
print(f"CUDA available: {torch.cuda.is_available()}")
if torch.cuda.is_available():
    print(f"GPU: {torch.cuda.get_device_name(0)}")
```
Jalankan cell dengan **Shift + Enter**

---

## LANGKAH 5: MOUNT GOOGLE DRIVE DI VSCODE

```python
from google.colab import drive
drive.mount('/content/drive')
```

1. Jalankan cell (Shift + Enter)
2. Klik link yang muncul
3. Login Google → Allow → Copy authorization code
4. Paste kode di kolom input VSCode → Enter
5. Muncul: `Mounted at /content/drive`

---

## LANGKAH 6: INSTALL ULTRALYTICS

```python
# Install YOLOv8
!pip install -q ultralytics

# Verify
from ultralytics import YOLO
print("YOLOv8 ready!")
```

---

## LANGKAH 7: COPY DATASET KE COLAB RUNTIME

```python
import os

# Path dataset di Drive
drive_path = '/content/drive/MyDrive/YOLO_Dataset_Colab'

# Copy ke local Colab (akses lebih cepat)
!cp -r {drive_path} /content/dataset_yolo

# Verifikasi
!ls /content/dataset_yolo
!cat /content/dataset_yolo/data.yaml
```

---

## LANGKAH 8: TRAINING MODEL

```python
from ultralytics import YOLO

# Load YOLOv8n (nano - fastest)
model = YOLO('yolov8n.pt')

# Training
results = model.train(
    data='/content/dataset_yolo/data.yaml',
    epochs=50,
    imgsz=640,
    batch=16,
    name='yolov8n_vscode_colab',
    save=True,
    device=0,  # GPU
    project='/content/runs'
)

print("Training selesai!")
```

**Monitor progress** di output cell VSCode (scroll ke bawah untuk melihat progress real-time)

---

## LANGKAH 9: EVALUASI & TESTING

```python
# Load best model
best_model = YOLO('/content/runs/yolov8n_vscode_colab/weights/best.pt')

# Validate
metrics = best_model.val()
print(f"mAP50: {metrics.box.map50:.3f}")
print(f"mAP50-95: {metrics.box.map:.3f}")

# Test predict
results = best_model.predict(
    source='/content/dataset_yolo/valid/images',
    save=True,
    conf=0.25
)
print("Hasil di: /content/runs/detect/predict")
```

---

## LANGKAH 10: DOWNLOAD HASIL TRAINING

### 10.1 Download Lewat VSCode File Explorer
1. Di VSCode, buka tab **Explorer** (Ctrl+Shift+E)
2. Navigate ke: `/content/runs/yolov8n_vscode_colab/weights/`
3. Klik kanan pada `best.pt` → **"Download"**

### 10.2 Download Lewat Code
```python
from google.colab import files

# Zip hasil training
!zip -r /content/yolov8n_trained_vscode.zip /content/runs/yolov8n_vscode_colab

# Download
files.download('/content/yolov8n_trained_vscode.zip')
```

### 10.3 Simpan ke Google Drive
```python
# Copy ke Drive untuk akses permanen
!cp -r /content/runs/yolov8n_vscode_colab /content/drive/MyDrive/YOLO_Results/

print("Model tersimpan di Drive: /MyDrive/YOLO_Results/yolov8n_vscode_colab")
```

---

## LANGKAH 11: MENGGUNAKAN MODEL DI ROS (Laptop Lokal)

```bash
# Extract file download
cd ~/Downloads
unzip yolov8n_trained_vscode.zip

# Copy model ke ROS workspace
cp best.pt \
  "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/models/custom_yolov8.pt"

# Update kode ROS untuk menggunakan model baru
# Edit file: yolo_node.py atau yolo_camera_node.py
# Ubah baris: self.model = YOLO('yolov8n.pt')
# Menjadi: self.model = YOLO('custom_yolov8.pt')
```

---

## KEUNTUNGAN MENGGUNAKAN VSCODE + COLAB

| Fitur | VSCode Colab | Browser Colab |
|-------|--------------|---------------|
| Auto-complete | ✓ (Pylance) | ✗ |
| Multiple files | ✓ (File explorer) | Terbatas |
| Git integration | ✓ | ✗ |
| Themes/Fonts | ✓ (Customizable) | Terbatas |
| Extensions | ✓ (Linter, formatter) | ✗ |
| Local editing | ✓ | ✗ |

---

## TROUBLESHOOTING

### Error: "Extension Colab not found"
- Pastikan sudah install extension **Google Colab** official
- Restart VSCode setelah install

### Error: "Runtime disconnected"
- Klik **"Reconnect"** di pojok kanan atas
- Pastikan koneksi internet stabil

### File tidak bisa didownload
- Gunakan metode zip terlebih dahulu
- Atau simpan ke Google Drive

### GPU tidak terdeteksi
- Cek: `Runtime` → `Change runtime type` → Pastikan **GPU** terpilih
- Jika tidak ada opsi GPU, akun mungkin belum verify (butuh phone verification)

---

## RINGKASAN ALUR

```
1. Install VSCode + Extension Colab → 2. Buka/create notebook Colab di VSCode →
3. Connect ke runtime → 4. Ganti runtime ke GPU → 5. Mount Google Drive →
6. Install Ultralytics → 7. Copy dataset ke Colab → 8. Training (model.train()) →
9. Evaluasi → 10. Download model (best.pt) → 11. Copy ke ROS workspace →
12. Update kode ROS untuk pakai model baru!
```

---

## REFERENSI
- VSCode Colab Extension: https://marketplace.visualstudio.com/items?itemName=Google.colab
- VSCode Docs: https://code.visualstudio.com/docs
- Colab + VSCode Guide: https://colab.research.google.com
