# Materi Modul 07: ROS YOLO untuk Robotika

Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi  
Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum  
Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.

## 1. Tujuan Pembelajaran

Setelah mempelajari materi ini, praktikan mampu:

1. Menjelaskan hubungan computer vision, neural network, CNN, object detection, dan YOLO.
2. Menjelaskan alur dataset: akuisisi citra, anotasi, split data, training, evaluasi, deployment.
3. Membaca format label YOLO dan menghitung koordinat bounding box ternormalisasi.
4. Melatih model YOLOv8 menggunakan dataset custom melalui Google Colab, VSCode Colab, atau GPU lokal.
5. Mengintegrasikan model YOLO ke ROS 2 untuk deteksi objek real-time dari kamera.
6. Mengevaluasi performa deteksi memakai precision, recall, IoU, AP, mAP, loss, FPS, dan confusion matrix.
7. Merancang proyek robotika berbasis deteksi objek secara ilmiah dan terukur.

---

## 2. Peta Konsep

```text
Citra digital
  -> dataset
  -> anotasi bounding box
  -> neural network
  -> convolutional neural network
  -> object detection
  -> YOLO
  -> training dan evaluasi
  -> model .pt
  -> ROS 2 node
  -> kamera robot
  -> keputusan robotika
```

YOLO bukan hanya program pendeteksi objek. YOLO adalah sistem deteksi satu tahap yang mempelajari pola visual dari data, memprediksi lokasi objek, kelas objek, dan tingkat keyakinan dalam satu proses inferensi cepat.

---

## 3. Citra Digital

Citra digital adalah matriks angka. Untuk citra grayscale, tiap piksel berisi intensitas:

```text
I(x,y) ∈ [0,255]
```

Untuk citra RGB:

```text
I(x,y) = [R(x,y), G(x,y), B(x,y)]
```

Jika ukuran gambar adalah lebar `W`, tinggi `H`, dan kanal `C`, maka tensor citra:

```text
X ∈ R^(H×W×C)
```

Contoh gambar 640×480 RGB:

```text
X ∈ R^(480×640×3)
```

Normalisasi umum sebelum masuk neural network:

```text
x_norm = x / 255
```

Tujuannya agar nilai input berada pada rentang 0 sampai 1 sehingga training lebih stabil.

---

## 4. Machine Learning dan Deep Learning

Machine learning mencari fungsi prediksi:

```text
y_hat = f(x; θ)
```

Keterangan:

- `x`: input, misalnya gambar.
- `y_hat`: prediksi model.
- `θ`: parameter model, bobot dan bias.
- `f`: fungsi yang dipelajari.

Training bertujuan mencari parameter terbaik:

```text
θ* = argmin_θ L(y, f(x; θ))
```

`L` adalah loss function, yaitu ukuran kesalahan prediksi terhadap label sebenarnya.

Deep learning memakai banyak lapisan transformasi nonlinier sehingga mampu mempelajari fitur kompleks dari data visual.

---

## 5. Neuron Buatan

Neuron menerima input `x1, x2, ..., xn`, bobot `w1, w2, ..., wn`, dan bias `b`.

Rumus neuron:

```text
z = w1x1 + w2x2 + ... + wnxn + b
z = w^T x + b
a = φ(z)
```

Keterangan:

- `z`: nilai linear.
- `a`: output aktivasi.
- `φ`: fungsi aktivasi.

Contoh fungsi aktivasi:

### Sigmoid

```text
σ(z) = 1 / (1 + e^(-z))
```

Rentang output 0 sampai 1. Cocok untuk probabilitas, tetapi rawan vanishing gradient.

### Tanh

```text
tanh(z) = (e^z - e^(-z)) / (e^z + e^(-z))
```

Rentang output -1 sampai 1.

### ReLU

```text
ReLU(z) = max(0,z)
```

Sederhana dan cepat, umum pada CNN.

### Leaky ReLU

```text
LeakyReLU(z) = max(αz,z)
```

Biasanya `α = 0.01`, mengurangi masalah neuron mati.

### SiLU/Swish

```text
SiLU(z) = z · σ(z)
```

Banyak dipakai pada arsitektur modern termasuk keluarga YOLO modern.

---

## 6. Multilayer Perceptron

MLP terdiri dari beberapa layer:

```text
h1 = φ(W1x + b1)
h2 = φ(W2h1 + b2)
y_hat = g(W3h2 + b3)
```

Untuk klasifikasi multi-kelas, output memakai softmax:

```text
softmax(zi) = e^(zi) / Σj e^(zj)
```

Softmax mengubah logit menjadi probabilitas setiap kelas.

Loss klasifikasi multi-kelas biasanya cross entropy:

```text
L_CE = -Σ_i y_i log(y_hat_i)
```

Jika label benar kelas ke-k, maka:

```text
L_CE = -log(y_hat_k)
```

---

## 7. Backpropagation dan Optimisasi

Training neural network memakai gradient descent. Parameter diperbarui berdasarkan turunan loss terhadap parameter:

```text
θ_(t+1) = θ_t - η ∇_θ L(θ_t)
```

Keterangan:

- `η`: learning rate.
- `∇_θ L`: gradien loss.

Stochastic Gradient Descent memakai mini-batch:

```text
L_batch = (1/m) Σ_i=1^m L_i
```

Momentum:

```text
v_t = βv_(t-1) + (1-β)∇L
θ_t = θ_(t-1) - ηv_t
```

Adam menggabungkan momentum gradien dan kuadrat gradien:

```text
m_t = β1m_(t-1) + (1-β1)g_t
v_t = β2v_(t-1) + (1-β2)g_t^2
m_hat_t = m_t / (1-β1^t)
v_hat_t = v_t / (1-β2^t)
θ_t = θ_(t-1) - η m_hat_t / (sqrt(v_hat_t)+ε)
```

Parameter penting training YOLO:

| Parameter | Fungsi |
|---|---|
| `epochs` | jumlah putaran training seluruh dataset |
| `batch` | jumlah gambar per iterasi |
| `imgsz` | ukuran input gambar, umum 640 |
| `lr0` | learning rate awal |
| `optimizer` | SGD, Adam, AdamW |
| `weight_decay` | regularisasi bobot |
| `momentum` | stabilisasi arah pembaruan bobot |

---

## 8. Convolutional Neural Network

CNN dirancang untuk data grid seperti gambar. Operasi utama CNN adalah konvolusi.

Untuk input 2D `X` dan kernel `K`:

```text
Y(i,j) = Σ_m Σ_n X(i+m, j+n) K(m,n)
```

Untuk citra multi-channel:

```text
Y(i,j,k) = Σ_c Σ_m Σ_n X(i+m, j+n, c) K(m,n,c,k) + b_k
```

Keterangan:

- `c`: channel input.
- `k`: channel output/filter.
- `K`: kernel/filter.

Ukuran output konvolusi:

```text
O = floor((I + 2P - K) / S) + 1
```

Keterangan:

- `I`: ukuran input.
- `P`: padding.
- `K`: ukuran kernel.
- `S`: stride.
- `O`: ukuran output.

Contoh: input 640, kernel 3, padding 1, stride 2:

```text
O = floor((640 + 2(1) - 3)/2) + 1 = 320
```

---

## 9. Pooling, Batch Normalization, dan Feature Map

Max pooling:

```text
Y(i,j) = max X(region i,j)
```

Average pooling:

```text
Y(i,j) = mean X(region i,j)
```

Batch normalization:

```text
μ_B = (1/m) Σ_i x_i
σ_B^2 = (1/m) Σ_i (x_i - μ_B)^2
x_hat_i = (x_i - μ_B) / sqrt(σ_B^2 + ε)
y_i = γx_hat_i + β
```

Batch normalization membuat distribusi aktivasi lebih stabil dan mempercepat training.

Feature map adalah hasil aktivasi layer CNN. Layer awal menangkap tepi dan warna, layer tengah menangkap tekstur dan bentuk, layer akhir menangkap objek kompleks.

---

## 10. Object Detection

Object detection menjawab dua pertanyaan:

1. Objek apa yang ada pada gambar?
2. Di mana lokasi objek tersebut?

Output object detection:

```text
[class_id, confidence, x_min, y_min, x_max, y_max]
```

Atau format YOLO:

```text
class_id x_center y_center width height
```

Semua koordinat YOLO dinormalisasi terhadap ukuran gambar.

Konversi bounding box pixel ke YOLO:

```text
x_center = ((x_min + x_max) / 2) / W
y_center = ((y_min + y_max) / 2) / H
width = (x_max - x_min) / W
height = (y_max - y_min) / H
```

Konversi YOLO ke pixel:

```text
x_min = (x_center - width/2) · W
x_max = (x_center + width/2) · W
y_min = (y_center - height/2) · H
y_max = (y_center + height/2) · H
```

Contoh gambar 640×480, objek dari `(160,120)` ke `(320,300)`:

```text
x_center = ((160+320)/2)/640 = 0.375
y_center = ((120+300)/2)/480 = 0.4375
width = (320-160)/640 = 0.25
height = (300-120)/480 = 0.375
```

Label YOLO jika class id 0:

```text
0 0.375 0.4375 0.25 0.375
```

---

## 11. Intersection over Union

IoU mengukur tumpang tindih antara bounding box prediksi dan ground truth.

```text
IoU = Area(B_pred ∩ B_gt) / Area(B_pred ∪ B_gt)
```

Dengan:

```text
Area union = Area pred + Area gt - Area intersection
```

Jika IoU tinggi, prediksi lokasi objek bagus. Threshold umum:

| IoU | Interpretasi |
|---|---|
| < 0.3 | buruk |
| 0.5 | cukup untuk TP pada mAP50 |
| 0.75 | ketat |
| 0.5:0.95 | standar COCO |

---

## 12. Precision, Recall, F1, AP, dan mAP

Confusion dasar object detection:

- True Positive: prediksi benar, kelas benar, IoU ≥ threshold.
- False Positive: prediksi objek padahal salah/tidak ada.
- False Negative: objek ada tetapi tidak terdeteksi.

Precision:

```text
Precision = TP / (TP + FP)
```

Recall:

```text
Recall = TP / (TP + FN)
```

F1-score:

```text
F1 = 2 · (Precision · Recall) / (Precision + Recall)
```

Average Precision adalah luas area di bawah kurva Precision-Recall:

```text
AP = ∫_0^1 P(R) dR
```

Mean Average Precision:

```text
mAP = (1/N) Σ_i=1^N AP_i
```

mAP50 berarti mAP pada IoU threshold 0.50. mAP50-95 adalah rata-rata AP pada threshold 0.50 sampai 0.95 dengan step 0.05.

---

## 13. YOLO: You Only Look Once

YOLO adalah object detector satu tahap. Gambar diproses sekali oleh jaringan, lalu model langsung menghasilkan bounding box, confidence, dan kelas.

Keunggulan YOLO:

1. Cepat untuk real-time.
2. Cocok untuk robotika karena latensi rendah.
3. Pipeline sederhana.
4. Bisa dilatih ulang untuk objek custom.

Perbandingan detektor:

| Jenis | Contoh | Karakter |
|---|---|---|
| Two-stage | Faster R-CNN | akurat, lambat |
| One-stage | YOLO, SSD, RetinaNet | cepat, cocok real-time |

Ide umum YOLO klasik:

```text
Gambar -> grid S×S -> setiap grid prediksi B box + C class
```

Output per grid klasik:

```text
B · (5 + C)
```

`5` terdiri dari `x, y, w, h, confidence`.

Confidence klasik:

```text
confidence = P(object) · IoU(pred, gt)
```

Skor kelas:

```text
score(class_i) = confidence · P(class_i | object)
```

---

## 14. Evolusi YOLO

| Versi | Ciri utama |
|---|---|
| YOLOv1 | deteksi satu tahap berbasis grid |
| YOLOv2/YOLO9000 | anchor box, batch norm, multi-scale |
| YOLOv3 | Darknet-53, multi-scale detection |
| YOLOv4 | CSPDarknet, Mosaic, CIoU, SPP |
| YOLOv5 | PyTorch, mudah digunakan |
| YOLOv7 | optimasi real-time detector |
| YOLOv8 | Ultralytics, anchor-free, task detect/segment/classify/pose |
| YOLOv9/v10/v11 | peningkatan efisiensi, representasi, assignment, deployment |

Pada praktikum ini dipakai YOLOv8 karena instalasi mudah, dokumentasi kuat, API sederhana, dan cocok untuk integrasi ROS.

---

## 15. Arsitektur YOLO Modern

YOLO modern dapat dipahami dalam tiga blok:

```text
Input -> Backbone -> Neck -> Head -> Output
```

### Backbone

Backbone mengekstraksi fitur visual dari gambar. Contoh fitur:

- warna objek,
- tepi,
- tekstur,
- bentuk,
- bagian objek,
- pola objek utuh.

### Neck

Neck menggabungkan fitur dari berbagai skala. Objek kecil butuh resolusi fitur tinggi, objek besar butuh konteks luas.

Struktur umum:

- FPN: top-down feature fusion.
- PAN: bottom-up path aggregation.

### Head

Head menghasilkan prediksi akhir:

```text
bounding box + objectness/confidence + class probability
```

YOLOv8 memakai pendekatan anchor-free dan decoupled head sehingga prediksi box dan klasifikasi lebih terpisah.

---

## 16. Multi-Scale Detection

Objek pada gambar bisa kecil, sedang, atau besar. YOLO memprediksi pada beberapa skala feature map.

Contoh input 640×640:

| Stride | Feature map | Cocok untuk |
|---|---|---|
| 8 | 80×80 | objek kecil |
| 16 | 40×40 | objek sedang |
| 32 | 20×20 | objek besar |

Stride berarti satu cell feature map merepresentasikan beberapa pixel input.

```text
feature_size = input_size / stride
```

Untuk input 640 dan stride 8:

```text
feature_size = 640 / 8 = 80
```

---

## 17. Anchor Box dan Anchor-Free

YOLO lama memakai anchor box, yaitu ukuran box prior.

Prediksi box anchor-based umum:

```text
b_x = σ(t_x) + c_x
b_y = σ(t_y) + c_y
b_w = p_w e^(t_w)
b_h = p_h e^(t_h)
```

Keterangan:

- `c_x, c_y`: posisi grid.
- `p_w, p_h`: ukuran anchor.
- `t_x, t_y, t_w, t_h`: output network.

YOLOv8 lebih dikenal anchor-free. Model memprediksi posisi objek tanpa anchor prior eksplisit sehingga konfigurasi lebih sederhana.

---

## 18. Loss Function pada YOLO

Training YOLO menggabungkan beberapa loss:

```text
L_total = λ_box L_box + λ_cls L_cls + λ_dfl L_dfl
```

Keterangan:

- `L_box`: kesalahan lokasi bounding box.
- `L_cls`: kesalahan klasifikasi.
- `L_dfl`: Distribution Focal Loss untuk regresi box modern.

Binary Cross Entropy:

```text
BCE = -[y log(p) + (1-y) log(1-p)]
```

Categorical Cross Entropy:

```text
CE = -Σ_i y_i log(p_i)
```

Focal Loss:

```text
FL(p_t) = -α(1-p_t)^γ log(p_t)
```

Focal loss menekan contoh mudah dan memperkuat contoh sulit.

CIoU loss:

```text
L_CIoU = 1 - IoU + ρ²(b,b_gt)/c² + αν
```

Keterangan:

- `ρ²`: jarak pusat box prediksi dan ground truth.
- `c²`: diagonal box terkecil yang mencakup keduanya.
- `ν`: perbedaan aspect ratio.
- `α`: faktor penyeimbang.

---

## 19. Non-Maximum Suppression

YOLO sering menghasilkan beberapa box untuk objek yang sama. NMS memilih box terbaik.

Langkah NMS:

1. Urutkan box berdasarkan confidence tertinggi.
2. Ambil box dengan confidence tertinggi.
3. Hapus box lain yang IoU-nya melebihi threshold.
4. Ulangi sampai tidak ada box tersisa.

Rumus keputusan:

```text
hapus B_j jika IoU(B_i, B_j) > threshold dan score_i > score_j
```

Parameter penting:

| Parameter | Fungsi |
|---|---|
| `conf` | confidence minimum prediksi |
| `iou` | threshold NMS |

Jika `conf` terlalu tinggi, banyak objek tidak terdeteksi. Jika terlalu rendah, false positive meningkat.

---

## 20. Dataset YOLO

Struktur dataset YOLOv8 umum:

```text
dataset/
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

Contoh `data.yaml`:

```yaml
path: /home/otomasi/dataset_yolo
train: train/images
val: valid/images
test: test/images

names:
  0: kotak_merah
  1: silinder_hijau
  2: bola_biru
```

Contoh label file `image_001.txt`:

```text
0 0.512 0.433 0.210 0.320
2 0.233 0.610 0.120 0.140
```

Artinya terdapat dua objek pada gambar tersebut.

---

## 21. Split Dataset

Pembagian umum:

```text
train: 70%
valid: 20%
test: 10%
```

Fungsi:

- Train: dipakai model belajar.
- Valid: dipakai memantau generalisasi saat training.
- Test: dipakai evaluasi akhir setelah model selesai.

Dataset baik harus bervariasi:

1. jarak dekat dan jauh,
2. sudut kiri, kanan, atas, bawah,
3. cahaya terang dan redup,
4. background berbeda,
5. objek sebagian tertutup,
6. objek dalam orientasi berbeda.

Rekomendasi minimal praktikum:

```text
50-100 gambar per kelas
```

Lebih baik:

```text
200-500 gambar per kelas
```

---

## 22. Anotasi Dataset

Tool yang dapat digunakan:

| Tool | Kelebihan | Cocok untuk |
|---|---|---|
| Roboflow | mudah, export YOLOv8 langsung | pemula, cepat |
| CVAT | open-source, kuat, profesional | dataset serius |
| LabelStudio | fleksibel, lokal, banyak format | eksperimen dan riset |

Prinsip anotasi:

1. Bounding box harus pas mengelilingi objek.
2. Jangan terlalu longgar karena model belajar background.
3. Jangan terlalu sempit karena bagian objek hilang.
4. Nama label harus konsisten.
5. Objek kecil tetap diberi label jika masih relevan.
6. Objek terpotong tetap diberi label sesuai bagian yang terlihat.
7. Review ulang sebelum training.

---

## 23. Augmentasi Data

Augmentasi membuat model lebih tahan variasi kondisi nyata.

Contoh augmentasi:

| Augmentasi | Fungsi |
|---|---|
| flip | variasi arah objek |
| rotation | variasi orientasi |
| scale | variasi ukuran |
| crop | variasi framing |
| brightness | variasi cahaya |
| blur | simulasi kamera buram |
| noise | simulasi sensor noise |
| mosaic | gabungan beberapa gambar |
| hsv | variasi warna |

Transformasi skala sederhana:

```text
x' = sx · x
y' = sy · y
```

Rotasi 2D:

```text
x' = x cosθ - y sinθ
y' = x sinθ + y cosθ
```

Augmentasi tidak boleh merusak makna label. Jika objek menjadi tidak jelas, augmentasi terlalu agresif.

---

## 24. Transfer Learning

Transfer learning memakai model yang sudah dilatih pada dataset besar seperti COCO, lalu fine-tuning pada dataset custom.

```text
θ_pretrained -> θ_custom
```

Keuntungan:

1. Butuh data lebih sedikit.
2. Training lebih cepat.
3. Akurasi awal lebih baik.
4. Fitur dasar seperti tepi dan tekstur sudah dipelajari.

Pada YOLOv8:

```python
from ultralytics import YOLO
model = YOLO('yolov8n.pt')
model.train(data='data.yaml', epochs=50, imgsz=640)
```

Model `yolov8n.pt` adalah pretrained model. Hasil terbaik tersimpan sebagai `best.pt`.

---

## 25. Training YOLOv8

Training CLI:

```bash
yolo detect train data=data.yaml model=yolov8n.pt epochs=50 imgsz=640 batch=16 device=0
```

Training Python:

```python
from ultralytics import YOLO
model = YOLO('yolov8n.pt')
results = model.train(
    data='data.yaml',
    epochs=50,
    imgsz=640,
    batch=16,
    device=0,
    name='yolov8n_custom'
)
```

Model size:

| Model | Ukuran | Speed | Akurasi | Kegunaan |
|---|---:|---|---|---|
| YOLOv8n | kecil | tercepat | cukup | praktikum, CPU ringan |
| YOLOv8s | kecil-sedang | cepat | baik | robot real-time |
| YOLOv8m | sedang | sedang | lebih baik | GPU cukup |
| YOLOv8l | besar | lambat | tinggi | riset |
| YOLOv8x | sangat besar | paling lambat | sangat tinggi | server GPU |

Untuk praktikum robotika, mulai dari YOLOv8n atau YOLOv8s.

---

## 26. Evaluasi Model

Validasi model:

```python
from ultralytics import YOLO
model = YOLO('runs/detect/train/weights/best.pt')
metrics = model.val()
print(metrics.box.map50)
print(metrics.box.map)
print(metrics.box.mp)
print(metrics.box.mr)
```

Metrik penting:

| Metrik | Makna |
|---|---|
| box_loss | kesalahan lokasi box |
| cls_loss | kesalahan kelas |
| dfl_loss | kesalahan distribusi box |
| precision | seberapa banyak prediksi yang benar |
| recall | seberapa banyak objek berhasil ditemukan |
| mAP50 | akurasi deteksi pada IoU 0.5 |
| mAP50-95 | akurasi ketat multi-IoU |
| FPS | kecepatan inferensi |

Indikasi overfitting:

```text
train loss turun, val loss naik, mAP valid stagnan/turun
```

Solusi:

1. Tambah data.
2. Tambah variasi dataset.
3. Kurangi epoch.
4. Gunakan augmentasi.
5. Gunakan model lebih kecil.
6. Perbaiki anotasi.

---

## 27. Inferensi YOLO

Inferensi gambar:

```python
from ultralytics import YOLO
model = YOLO('best.pt')
results = model.predict(source='test.jpg', conf=0.25, iou=0.45, save=True)
```

Inferensi webcam:

```python
from ultralytics import YOLO
model = YOLO('best.pt')
model.predict(source=0, show=True, conf=0.25)
```

Output penting dari hasil deteksi:

```python
boxes = results[0].boxes
xyxy = boxes.xyxy
conf = boxes.conf
cls = boxes.cls
```

---

## 28. ROS 2 dan YOLO

ROS 2 menghubungkan kamera, model YOLO, visualisasi, dan robot melalui node dan topic.

Konsep utama ROS 2:

| Konsep | Fungsi |
|---|---|
| Node | program mandiri |
| Topic | jalur komunikasi publish-subscribe |
| Publisher | pengirim data |
| Subscriber | penerima data |
| Message | format data |
| Launch | menjalankan beberapa node sekaligus |
| Package | unit proyek ROS |

Alur deteksi:

```text
camera_node -> /camera/image_raw -> yolo_node -> /yolo/annotated dan /yolo/detections
```

Topic umum:

| Topic | Isi |
|---|---|
| `/camera/image_raw` | gambar mentah kamera |
| `/yolo/annotated` | gambar dengan bounding box |
| `/yolo/detections` | informasi deteksi dalam teks/JSON |

---

## 29. Integrasi OpenCV, cv_bridge, dan YOLO

ROS memakai message gambar `sensor_msgs/Image`. OpenCV memakai array NumPy. Konversi dilakukan oleh `cv_bridge`.

Alur kode umum:

```text
ROS Image -> cv_bridge -> OpenCV image -> YOLO predict -> annotated image -> cv_bridge -> ROS Image
```

Pseudocode:

```python
image = bridge.imgmsg_to_cv2(msg, 'bgr8')
results = model.predict(image, conf=0.25)
annotated = results[0].plot()
out_msg = bridge.cv2_to_imgmsg(annotated, 'bgr8')
publisher.publish(out_msg)
```

Kecepatan sistem dipengaruhi oleh:

1. resolusi kamera,
2. model YOLO,
3. CPU/GPU,
4. jumlah objek,
5. pipeline ROS,
6. visualisasi.

---

## 30. Colcon Build dan Running

Workspace praktikum:

```text
ROS_YOLO/
└── src/
    └── yolo_ros/
        ├── package.xml
        ├── setup.py
        ├── launch/
        ├── config/
        ├── models/
        ├── yolo_ros/scripts/
        ├── training_data/
        └── roboflow_data/
```

Build:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
rosdep install -i --from-path src --rosdistro humble -y
pip3 install ultralytics opencv-python
colcon build --symlink-install
source install/setup.bash
```

Cek executable:

```bash
ros2 pkg executables yolo_ros
```

Run node:

```bash
ros2 run yolo_ros yolo_node
ros2 run yolo_ros camera_node
ros2 run yolo_ros yolo_camera_node
ros2 run yolo_ros rviz_display
```

Run launch:

```bash
ros2 launch yolo_ros yolo.launch.py
```

Run dengan model custom:

```bash
ros2 launch yolo_ros yolo.launch.py model_path:=/path/to/custom_yolov8.pt camera_topic:=/usb_cam/image_raw
```

---

## 31. Kesalahan Umum

| Masalah | Penyebab | Solusi |
|---|---|---|
| dataset not found | path `data.yaml` salah | pakai path absolut |
| labels kosong | file `.txt` tidak sejajar gambar | cek struktur folder |
| class mismatch | nama class tidak konsisten | perbaiki `names` |
| CUDA out of memory | batch terlalu besar | kecilkan batch |
| GPU tidak aktif | PyTorch CPU | install torch CUDA |
| kamera tidak terbuka | device salah/izin | cek `/dev/video*` |
| node ROS tidak ditemukan | belum source workspace | `source install/setup.bash` |
| package tidak terdeteksi | build gagal | clean build ulang |
| false positive tinggi | conf rendah/data buruk | naikkan conf/perbaiki dataset |
| objek tidak terdeteksi | data kurang/blur | tambah data dan augmentasi |

---

## 32. Praktik Ilmiah dalam Eksperimen YOLO

Setiap eksperimen harus dicatat:

1. nama dataset,
2. jumlah kelas,
3. jumlah gambar per split,
4. model awal,
5. epoch,
6. image size,
7. batch size,
8. optimizer,
9. learning rate,
10. hasil precision, recall, mAP50, mAP50-95,
11. screenshot hasil deteksi,
12. kendala dan solusi.

Template catatan:

```text
Eksperimen: YOLOv8n_kotak_silinder_bola_v1
Dataset: 300 gambar, 3 kelas
Split: 70/20/10
Epoch: 50
imgsz: 640
batch: 16
mAP50: ...
mAP50-95: ...
FPS: ...
Kesimpulan: ...
```

---

## 33. Etika Dataset dan Keselamatan

1. Jangan memakai dataset yang melanggar privasi.
2. Hindari merekam wajah tanpa izin.
3. Jangan mengunggah data sensitif ke platform publik.
4. Pastikan kabel kamera dan robot aman.
5. Jika robot bergerak berdasarkan deteksi, gunakan area aman dan emergency stop.
6. Verifikasi deteksi sebelum dipakai mengambil keputusan aktuator.

---

## 34. Ringkasan Praktis

Alur lengkap praktikum:

```text
Install tools
-> ambil gambar
-> anotasi di Roboflow/CVAT/LabelStudio
-> export YOLOv8
-> cek data.yaml
-> training YOLOv8
-> evaluasi model
-> copy best.pt ke ROS
-> colcon build
-> source workspace
-> run node kamera dan YOLO
-> analisis hasil
-> rekam video dan laporan
```

---

## 35. Rekomendasi Buku

1. Ian Goodfellow, Yoshua Bengio, Aaron Courville, **Deep Learning**, MIT Press, 2016.
2. Richard Szeliski, **Computer Vision: Algorithms and Applications**, 2nd Edition, Springer, 2022.
3. Christopher M. Bishop, **Pattern Recognition and Machine Learning**, Springer, 2006.
4. Kevin P. Murphy, **Probabilistic Machine Learning: An Introduction**, MIT Press, 2022.
5. Aurélien Géron, **Hands-On Machine Learning with Scikit-Learn, Keras, and TensorFlow**, O'Reilly.
6. Adrian Rosebrock, **Deep Learning for Computer Vision with Python**, PyImageSearch.
7. Joseph Howse, Joe Minichino, **Learning OpenCV 4 Computer Vision with Python**, Packt.
8. Sebastian Thrun, Wolfram Burgard, Dieter Fox, **Probabilistic Robotics**, MIT Press.
9. Morgan Quigley, Brian Gerkey, William D. Smart, **Programming Robots with ROS**, O'Reilly.
10. Lentin Joseph, **ROS Robotics Projects**, Packt.

---

## 36. Rekomendasi Paper

1. Redmon et al., **You Only Look Once: Unified, Real-Time Object Detection**, CVPR 2016.
2. Redmon and Farhadi, **YOLO9000: Better, Faster, Stronger**, CVPR 2017.
3. Redmon and Farhadi, **YOLOv3: An Incremental Improvement**, 2018.
4. Bochkovskiy et al., **YOLOv4: Optimal Speed and Accuracy of Object Detection**, 2020.
5. Jocher et al., **Ultralytics YOLOv5**, 2020.
6. Wang et al., **YOLOv7: Trainable Bag-of-Freebies Sets New State-of-the-Art for Real-Time Object Detectors**, 2022.
7. Li et al., **YOLOv6: A Single-Stage Object Detection Framework for Industrial Applications**, 2022.
8. Ge et al., **YOLOX: Exceeding YOLO Series in 2021**, 2021.
9. Lin et al., **Feature Pyramid Networks for Object Detection**, CVPR 2017.
10. Lin et al., **Focal Loss for Dense Object Detection**, ICCV 2017.
11. Rezatofighi et al., **Generalized Intersection over Union**, CVPR 2019.
12. Zheng et al., **Distance-IoU Loss: Faster and Better Learning for Bounding Box Regression**, AAAI 2020.
13. Liu et al., **SSD: Single Shot MultiBox Detector**, ECCV 2016.
14. Ren et al., **Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks**, NeurIPS 2015.
15. He et al., **Deep Residual Learning for Image Recognition**, CVPR 2016.
16. Tan and Le, **EfficientNet: Rethinking Model Scaling for Convolutional Neural Networks**, ICML 2019.
17. Dosovitskiy et al., **An Image is Worth 16x16 Words: Transformers for Image Recognition at Scale**, ICLR 2021.
18. Carion et al., **End-to-End Object Detection with Transformers**, ECCV 2020.
19. Kirillov et al., **Segment Anything**, ICCV 2023.
20. Ultralytics, **YOLOv8 Documentation and Technical Implementation Notes**.

---

## 37. Referensi Praktik

- Ultralytics Documentation: https://docs.ultralytics.com
- ROS 2 Humble Documentation: https://docs.ros.org/en/humble
- Roboflow Documentation: https://docs.roboflow.com
- CVAT Documentation: https://opencv.github.io/cvat/docs
- LabelStudio Documentation: https://labelstud.io/guide
- PyTorch Documentation: https://pytorch.org/docs/stable/index.html
- OpenCV Documentation: https://docs.opencv.org
