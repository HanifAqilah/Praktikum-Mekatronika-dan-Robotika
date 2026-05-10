# Jobsheet Modul 07: Praktikum ROS YOLO

Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi  
Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum  
Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.

## 1. Identitas Praktikum

| Komponen | Keterangan |
|---|---|
| Modul | 07 |
| Topik | ROS 2 dan YOLO untuk Object Detection Robotika |
| Durasi | 5 jam 40 menit |
| Mode kerja | Berkelompok atau individu sesuai arahan dosen/asisten |
| Output utama | Video YouTube minimal 15 menit berisi teori, praktik, dan demo ROS YOLO |
| Deadline tugas | 2 minggu setelah praktikum |

---

## 2. Capaian Praktikum

Praktikan mampu:

1. Menyiapkan environment YOLO dan ROS 2.
2. Mengambil gambar objek robotika.
3. Memberi anotasi bounding box dengan Roboflow, CVAT, atau LabelStudio.
4. Mengekspor dataset format YOLOv8.
5. Melatih YOLOv8 dengan Google Colab, VSCode Colab, atau GPU lokal.
6. Mengevaluasi model dengan precision, recall, mAP50, dan mAP50-95.
7. Menjalankan model YOLO pada ROS 2 secara real-time.
8. Mendokumentasikan proses praktik dalam video dan catatan eksperimen.

---

## 3. Perangkat dan Bahan

### Hardware

1. Laptop Ubuntu 22.04 direkomendasikan.
2. Kamera laptop atau USB camera.
3. GPU NVIDIA opsional untuk training lokal.
4. Objek latihan: kotak merah, silinder hijau, bola biru, botol, gelas, komponen robotika, atau benda lain.
5. Koneksi internet.

### Software

1. Ubuntu 22.04.
2. ROS 2 Humble.
3. Python 3.
4. OpenCV.
5. Ultralytics YOLOv8.
6. Google Colab atau VSCode.
7. Roboflow/CVAT/LabelStudio.
8. Git, colcon, rosdep.

---

## 4. Keselamatan dan Etika

1. Pastikan kabel kamera tidak mengganggu area kerja.
2. Jangan menaruh objek tajam atau berbahaya pada area pengujian.
3. Jangan merekam wajah orang tanpa izin.
4. Jangan mengunggah dataset pribadi/sensitif ke platform publik.
5. Jika sistem dihubungkan ke aktuator robot, gunakan emergency stop.
6. Jalankan robot dengan kecepatan rendah saat pengujian awal.

---

## 5. Alur Praktikum 5 Jam 40 Menit

| Waktu | Kegiatan |
|---:|---|
| 00:00-00:20 | briefing, teori singkat YOLO dan ROS |
| 00:20-00:50 | cek instalasi, workspace, kamera |
| 00:50-01:30 | akuisisi gambar dataset |
| 01:30-02:20 | anotasi dataset |
| 02:20-02:45 | export dan validasi dataset YOLO |
| 02:45-03:45 | training YOLOv8 |
| 03:45-04:20 | evaluasi dan testing model |
| 04:20-05:10 | integrasi model ke ROS 2 |
| 05:10-05:40 | demo, troubleshooting, checkpoint tugas video |

---

## 6. Struktur Workspace

Gunakan workspace berikut:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
tree -L 3 src/
```

Struktur yang diharapkan:

```text
src/
└── yolo_ros/
    ├── package.xml
    ├── setup.py
    ├── resource/
    │   └── yolo_ros
    ├── launch/
    │   └── yolo.launch.py
    ├── config/
    │   └── yolo_params.yaml
    ├── models/
    │   └── yolov8n.pt
    ├── yolo_ros/
    │   └── scripts/
    │       ├── yolo_node.py
    │       ├── camera_node.py
    │       ├── yolo_camera_node.py
    │       ├── training_node.py
    │       └── rviz_display.py
    ├── training_data/
    └── roboflow_data/
```

---

## 7. Tahap 1: Cek ROS 2 dan Tools

Jalankan:

```bash
ros2 --version
python3 --version
colcon --help
```

Jika ROS 2 belum tersedia, install ROS 2 Humble sesuai dokumentasi resmi:

```bash
sudo apt update
sudo apt install software-properties-common curl gnupg lsb-release
sudo add-apt-repository universe
sudo apt update
sudo apt install ros-humble-desktop python3-colcon-common-extensions python3-rosdep
```

Source ROS:

```bash
source /opt/ros/humble/setup.bash
```

Opsional agar otomatis:

```bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

---

## 8. Tahap 2: Install Dependensi Workspace

Masuk workspace:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
```

Install dependensi ROS:

```bash
sudo rosdep init
rosdep update
rosdep install -i --from-path src --rosdistro humble -y
```

Jika `sudo rosdep init` sudah pernah dijalankan dan muncul error, lanjutkan ke:

```bash
rosdep update
```

Install dependensi Python:

```bash
pip3 install --upgrade pip
pip3 install ultralytics opencv-python numpy pyyaml
```

Verifikasi YOLO:

```bash
python3 -c "from ultralytics import YOLO; print('YOLO siap')"
```

---

## 9. Tahap 3: Cek Kamera

Cek device kamera:

```bash
ls /dev/video*
```

Cek kamera dengan OpenCV:

```bash
python3 - <<'PY'
import cv2
cap = cv2.VideoCapture(0)
print('Camera opened:', cap.isOpened())
ret, frame = cap.read()
print('Frame read:', ret, 'Shape:', None if frame is None else frame.shape)
cap.release()
PY
```

Jika kamera tidak terbuka:

1. Coba indeks `1` atau `2`.
2. Cabut dan pasang ulang kamera USB.
3. Cek izin device.
4. Tutup aplikasi lain yang memakai kamera.

---

## 10. Tahap 4: Akuisisi Gambar Dataset

Buat folder gambar mentah:

```bash
mkdir -p "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/dataset_raw"
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/dataset_raw"
```

Buat script capture:

```python
import cv2
from pathlib import Path

out = Path('images')
out.mkdir(exist_ok=True)
cap = cv2.VideoCapture(0)
count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        print('Kamera gagal dibaca')
        break
    cv2.imshow('Capture: SPACE=simpan, Q=keluar', frame)
    key = cv2.waitKey(1) & 0xFF
    if key == ord(' '):
        name = out / f'image_{count:04d}.jpg'
        cv2.imwrite(str(name), frame)
        print('Disimpan:', name)
        count += 1
    elif key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
```

Simpan sebagai `capture_dataset.py`, lalu jalankan:

```bash
python3 capture_dataset.py
```

Target minimal:

```text
3 kelas × 50 gambar = 150 gambar
```

Variasi wajib:

1. dekat, sedang, jauh,
2. sisi kiri, kanan, depan, atas,
3. pencahayaan terang dan redup,
4. background polos dan ramai,
5. objek tunggal dan multi-objek.

---

## 11. Tahap 5A: Anotasi dengan Roboflow

1. Buka https://roboflow.com.
2. Sign up atau login.
3. Klik **Create New Project**.
4. Isi:
   - Project Name: `YOLO_Project_Modul07`
   - Project Type: `Object Detection`
   - Description: dataset deteksi objek robotika.
5. Upload gambar dari folder `dataset_raw/images`.
6. Klik **Annotate**.
7. Buat bounding box untuk setiap objek.
8. Gunakan label konsisten, contoh:

```text
kotak_merah
silinder_hijau
bola_biru
```

9. Setelah selesai, buka **Dataset**.
10. Klik **Export Dataset**.
11. Pilih format **YOLO v8**.
12. Pilih download zip.
13. Extract:

```bash
unzip YOLO_Project_Modul07-*.zip -d ~/roboflow_dataset
```

14. Copy ke workspace:

```bash
cp -r ~/roboflow_dataset/* "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data/"
```

---

## 12. Tahap 5B: Anotasi dengan CVAT

1. Buka https://www.cvat.ai.
2. Login memakai Google/GitHub/email.
3. Klik **Create new project**.
4. Isi label:

```text
kotak_merah
silinder_hijau
bola_biru
```

5. Pilih task **Object Detection**.
6. Upload gambar.
7. Buka job annotation.
8. Tekan `N` untuk rectangle tool.
9. Pilih label, gambar bounding box.
10. Simpan dengan `Ctrl+S`.
11. Export dataset format **YOLO 1.1** dan aktifkan **Save images**.
12. Extract:

```bash
unzip cvat_export_yolo_1.1.zip -d ~/cvat_dataset
```

13. Pastikan `data.yaml` benar:

```yaml
path: /home/otomasi/cvat_dataset
train: images/train
val: images/val

names:
  0: kotak_merah
  1: silinder_hijau
  2: bola_biru
```

14. Copy ke workspace:

```bash
cp -r ~/cvat_dataset/* "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data/"
```

---

## 13. Tahap 5C: Anotasi dengan LabelStudio

Install:

```bash
cd /home/otomasi
python3 -m venv labelstudio_env
source labelstudio_env/bin/activate
pip install --upgrade pip
pip install label-studio
label-studio --port 8080
```

Langkah web:

1. Buka `http://localhost:8080`.
2. Buat akun pertama.
3. Klik **Create Project**.
4. Upload gambar.
5. Pilih **Object Detection with Bounding Boxes**.
6. Tambahkan label.
7. Anotasi semua gambar.
8. Export ke **YOLOv5 PyTorch** atau JSON lalu konversi.
9. Atur struktur dataset:

```text
dataset_yolo/
├── data.yaml
├── train/images
├── train/labels
├── valid/images
├── valid/labels
├── test/images
└── test/labels
```

10. Copy:

```bash
cp -r dataset_yolo/* "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data/"
```

---

## 14. Tahap 6: Validasi Dataset

Masuk folder dataset:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data"
tree -L 3
cat data.yaml
```

Struktur Roboflow/YOLOv8 yang benar:

```text
training_data/
├── data.yaml
├── train/
│   ├── images/
│   └── labels/
├── valid/
│   ├── images/
│   └── labels/
└── test/
    ├── images/
    └── labels/
```

Script validasi cepat:

```bash
python3 - <<'PY'
from pathlib import Path
import yaml
base = Path('.').resolve()
print('Base:', base)
with open('data.yaml') as f:
    data = yaml.safe_load(f)
print(data)
for split in ['train','valid','val','test']:
    for sub in ['images','labels']:
        p = base/split/sub
        if p.exists():
            print(p, len(list(p.iterdir())))
PY
```

Cek label YOLO:

```bash
python3 - <<'PY'
from pathlib import Path
for f in Path('.').rglob('labels/*.txt'):
    for i,line in enumerate(f.read_text().splitlines(),1):
        parts=line.split()
        if len(parts)!=5:
            print('Format salah:', f, i, line)
        else:
            vals=list(map(float, parts[1:]))
            if not all(0 <= v <= 1 for v in vals):
                print('Koordinat tidak normal:', f, i, line)
print('Validasi selesai')
PY
```

---

## 15. Tahap 7A: Training di Google Colab

1. Compress dataset:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data"
zip -r dataset_yolo.zip .
```

2. Upload `dataset_yolo.zip` ke Google Drive.
3. Buka https://colab.research.google.com.
4. Klik **New notebook**.
5. Pilih **Runtime > Change runtime type > GPU**.
6. Verifikasi GPU:

```python
import torch
print(torch.__version__)
print(torch.cuda.is_available())
print(torch.cuda.get_device_name(0) if torch.cuda.is_available() else 'No GPU')
```

7. Mount Drive:

```python
from google.colab import drive
drive.mount('/content/drive')
```

8. Install YOLO:

```python
!pip install -q ultralytics
from ultralytics import YOLO
```

9. Extract dataset:

```python
!mkdir -p /content/dataset_yolo
!unzip -q /content/drive/MyDrive/dataset_yolo.zip -d /content/dataset_yolo
!ls /content/dataset_yolo
!cat /content/dataset_yolo/data.yaml
```

10. Training:

```python
from ultralytics import YOLO
model = YOLO('yolov8n.pt')
results = model.train(
    data='/content/dataset_yolo/data.yaml',
    epochs=50,
    imgsz=640,
    batch=16,
    device=0,
    name='yolov8n_custom'
)
```

---

## 16. Tahap 7B: Training dengan VSCode Colab

1. Install VSCode:

```bash
sudo snap install code --classic
```

2. Install extension Colab:

```bash
code --install-extension Google.colab
```

3. Buka notebook Colab di VSCode.
4. Connect to hosted runtime.
5. Pilih GPU.
6. Mount Drive:

```python
from google.colab import drive
drive.mount('/content/drive')
```

7. Install Ultralytics:

```python
!pip install -q ultralytics
```

8. Copy dataset:

```python
!cp -r /content/drive/MyDrive/YOLO_Dataset_Colab /content/dataset_yolo
!cat /content/dataset_yolo/data.yaml
```

9. Training:

```python
from ultralytics import YOLO
model = YOLO('yolov8n.pt')
results = model.train(
    data='/content/dataset_yolo/data.yaml',
    epochs=50,
    imgsz=640,
    batch=16,
    name='yolov8n_vscode_colab',
    save=True,
    device=0,
    project='/content/runs'
)
```

---

## 17. Tahap 7C: Training Lokal dengan GPU NVIDIA

Cek GPU:

```bash
lspci | grep -i nvidia
nvidia-smi
```

Jika driver belum ada:

```bash
sudo ubuntu-drivers autoinstall
sudo reboot
```

Buat environment:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO"
python3 -m venv yolo_env
source yolo_env/bin/activate
pip install --upgrade pip
```

Install PyTorch CUDA, contoh CUDA 12.1:

```bash
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121
```

Verifikasi:

```bash
python3 -c "import torch; print(torch.cuda.is_available()); print(torch.cuda.get_device_name(0) if torch.cuda.is_available() else 'No GPU')"
```

Install YOLO:

```bash
pip install ultralytics
```

Training:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/training_data"
yolo detect train data=data.yaml model=yolov8n.pt epochs=50 imgsz=640 batch=16 device=0 name=yolov8n_local_gpu
```

Jika CUDA out of memory:

```bash
yolo detect train data=data.yaml model=yolov8n.pt epochs=50 imgsz=640 batch=8 device=0 name=yolov8n_local_gpu_b8
```

---

## 18. Tahap 8: Evaluasi Model

Di Colab atau lokal:

```python
from ultralytics import YOLO
model = YOLO('runs/detect/yolov8n_custom/weights/best.pt')
metrics = model.val()
print('mAP50:', metrics.box.map50)
print('mAP50-95:', metrics.box.map)
print('Precision:', metrics.box.mp)
print('Recall:', metrics.box.mr)
```

Testing gambar validasi:

```python
from ultralytics import YOLO
model = YOLO('runs/detect/yolov8n_custom/weights/best.pt')
results = model.predict(
    source='/content/dataset_yolo/valid/images',
    conf=0.25,
    iou=0.45,
    save=True,
    show_labels=True,
    show_conf=True
)
```

Catat hasil:

| Metrik | Nilai |
|---|---:|
| Precision | ... |
| Recall | ... |
| mAP50 | ... |
| mAP50-95 | ... |
| FPS/inference time | ... |

---

## 19. Tahap 9: Download dan Simpan Model

Dari Colab:

```python
from google.colab import files
!zip -r /content/yolov8_result.zip /content/runs/detect/yolov8n_custom
files.download('/content/yolov8_result.zip')
```

Atau salin ke Google Drive:

```python
!mkdir -p /content/drive/MyDrive/YOLO_Results
!cp -r /content/runs/detect/yolov8n_custom /content/drive/MyDrive/YOLO_Results/
```

Di laptop, copy `best.pt` ke folder model ROS:

```bash
cp best.pt "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/models/custom_yolov8.pt"
```

---

## 20. Tahap 10: Build Package ROS 2

Masuk workspace:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
```

Build:

```bash
colcon build --symlink-install
```

Jika error:

```bash
rm -rf build/ install/ log/
colcon build --symlink-install
```

Source workspace:

```bash
source install/setup.bash
```

Cek node:

```bash
ros2 pkg executables yolo_ros
```

Output yang diharapkan:

```text
yolo_ros camera_node
yolo_ros rviz_display
yolo_ros training_node
yolo_ros yolo_camera_node
yolo_ros yolo_node
```

---

## 21. Tahap 11: Running Node ROS 2

Terminal 1, kamera:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
source install/setup.bash
ros2 run yolo_ros camera_node
```

Terminal 2, YOLO detector:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
source install/setup.bash
ros2 run yolo_ros yolo_node
```

Terminal 3, kamera langsung dengan YOLO:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
source install/setup.bash
ros2 run yolo_ros yolo_camera_node
```

Terminal 4, display:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
source install/setup.bash
ros2 run yolo_ros rviz_display
```

Launch file:

```bash
ros2 launch yolo_ros yolo.launch.py
```

Launch dengan model custom:

```bash
ros2 launch yolo_ros yolo.launch.py model_path:="/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/models/custom_yolov8.pt"
```

---

## 22. Tahap 12: Cek Topic dan Output

Daftar topic:

```bash
ros2 topic list
```

Cek image raw:

```bash
ros2 topic info /camera/image_raw
```

Cek deteksi:

```bash
ros2 topic echo /yolo/detections
```

Cek annotated image:

```bash
ros2 topic info /yolo/annotated
```

Output minimal berhasil:

1. Kamera terbuka.
2. Window menampilkan gambar.
3. Bounding box muncul pada objek.
4. Label class dan confidence muncul.
5. Topic `/yolo/detections` menerbitkan data.

---

## 23. Tahap 13: Analisis Hasil Praktikum

Isi tabel berikut:

| Percobaan | Objek | Jarak | Cahaya | Conf threshold | Hasil |
|---|---|---:|---|---:|---|
| 1 | kotak_merah | 30 cm | terang | 0.25 | terdeteksi/tidak |
| 2 | silinder_hijau | 60 cm | terang | 0.25 | terdeteksi/tidak |
| 3 | bola_biru | 100 cm | redup | 0.25 | terdeteksi/tidak |
| 4 | multi-objek | 50 cm | terang | 0.25 | terdeteksi/tidak |
| 5 | objek miring | 50 cm | redup | 0.35 | terdeteksi/tidak |

Analisis:

1. Kelas mana paling mudah terdeteksi?
2. Kelas mana paling sering salah?
3. Apa pengaruh jarak?
4. Apa pengaruh pencahayaan?
5. Apakah confidence threshold perlu diubah?
6. Apakah dataset perlu ditambah?

---

## 24. Troubleshooting

| Masalah | Penyebab | Solusi |
|---|---|---|
| `ros2: command not found` | ROS belum source | `source /opt/ros/humble/setup.bash` |
| package tidak ditemukan | workspace belum source | `source install/setup.bash` |
| executable tidak muncul | build gagal | `colcon build --symlink-install` |
| kamera hitam | device salah | cek `/dev/video*`, ganti indeks kamera |
| model tidak load | path `.pt` salah | pakai path absolut |
| dataset not found | `data.yaml` salah | perbaiki `path`, `train`, `val` |
| CUDA OOM | batch terlalu besar | batch 4/8 |
| training lambat | CPU dipakai | cek `torch.cuda.is_available()` |
| anotasi kacau | bounding box tidak konsisten | review dataset |
| false positive | data kurang/conf rendah | tambah data/naikkan conf |

---

## 25. Checklist Keberhasilan

Centang sebelum praktikum selesai:

- [ ] ROS 2 Humble aktif.
- [ ] Workspace `ROS_YOLO` bisa dibuild.
- [ ] Kamera terbaca.
- [ ] Dataset minimal 3 kelas tersedia.
- [ ] Dataset sudah dianotasi.
- [ ] Dataset sudah diekspor format YOLO.
- [ ] `data.yaml` valid.
- [ ] Training YOLOv8 berjalan.
- [ ] `best.pt` berhasil dibuat.
- [ ] Model dievaluasi.
- [ ] Model disalin ke folder ROS.
- [ ] Node YOLO berjalan.
- [ ] Bounding box muncul real-time.
- [ ] Video proses praktik direkam.
- [ ] Hasil dan kendala dicatat.

---

## 26. Output yang Dikumpulkan

**HANYA SUBMIT LINK VIDEO YOUTUBE**

Format pengumpulan:

```text
Nama Kelompok: [Nama]
Anggota: [Daftar nama]
Link YouTube: https://youtu.be/[video_id]
Durasi: [menit:detik]
Tanggal upload: [tanggal]
```

**TIDAK ADA TUGAS LAPORAN, KETIK, ATAU PRINT**

Semua informasi praktikum (dataset, training, evaluasi, kendala, solusi) harus ada di:

1. Video YouTube (minimal 15 menit).
2. Deskripsi video YouTube.

---

## 27. Informasi di Deskripsi Video YouTube

Tulis di deskripsi video:

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

## 28. Kriteria Minimal Lulus Praktikum

1. Video YouTube minimal 15 menit terupload.
2. Video menunjukkan penjelasan teori neural network, CNN, YOLO.
3. Video menunjukkan proses dataset, anotasi, training, evaluasi.
4. Video menunjukkan ROS YOLO berhasil running dan demo deteksi real-time.
5. Deskripsi video YouTube lengkap dengan hasil evaluasi.
6. Link YouTube dapat diakses dan deadline terpenuhi.
