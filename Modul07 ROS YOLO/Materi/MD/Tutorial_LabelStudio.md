# TUTORIAL LENGKAP LABELSTUDIO - ANNOTASI DATA OBJEK

## PENDAHULUAN
LabelStudio adalah open-source data labeling tool yang powerful dan fleksibel. Tutorial ini akan memandu penggunaan LabelStudio untuk membuat dataset YOLOv8.

---

## LANGKAH 1: INSTALASI LABELSTUDIO

### 1.1 Install Python Environment (Disarankan)
```bash
# Buat virtual environment
cd /home/otomasi
python3 -m venv labelstudio_env

# Aktifkan environment
source labelstudio_env/bin/activate

# Update pip
pip install --upgrade pip
```

### 1.2 Install LabelStudio
```bash
# Install LabelStudio
pip install label-studio

# Verifikasi instalasi
label-studio --version
```

### 1.3 Menjalankan LabelStudio
```bash
# Jalankan LabelStudio
label-studio

# Atau dengan port spesifik
label-studio --port 8080
```

Browser akan otomatis terbuka di `http://localhost:8080`

---

## LANGKAH 2: SETUP AKUN & PROJECT

### 2.1 Membuat Akun Pertama
1. Di halaman pertama, isi form registrasi:
   - **Email**: your_email@email.com
   - **Password**: (buat password)
   - **Confirm Password**: (ulangi password)
2. Klik **"Sign Up"**
3. Verifikasi email (jika diminta)

### 2.2 Membuat Project Baru
1. Klik **"Create Project"** di dashboard
2. Isi detail project:
   - **Project Name**: `YOLO_Detection_Modul07`
   - **Description**: "Object detection untuk robotika"
3. **Data Import**:
   - Klik **"Upload Files"**
   - Pilih semua gambar yang akan diannotasi
   - Atau drag & drop folder gambar
4. Klik **"Save"**

---

## LANGKAH 3: KONFIGURASI LABELING

### 3.1 Memilih Task Type
1. Di halaman project, klik **"Settings"** → **"Labeling Interface"**
2. Pilih **"Object Detection with Bounding Boxes"**
3. Atau gunakan **"Custom Template"** dengan kode berikut:

```xml
<View>
  <Image name="image" value="$image"/>
  
  <RectangleLabels name="label" toName="image">
    <Label value="kotak_merah" background="red"/>
    <Label value="silinder_hijau" background="green"/>
    <Label value="bola_biru" background="blue"/>
    <Label value="botol" background="yellow"/>
    <Label value="gelas" background="purple"/>
  </RectangleLabels>
</View>
```

### 3.2 Menambahkan Class/Label
1. Klik **"Add label"** untuk menambah class baru
2. Isi:
   - **Value**: Nama class (contoh: `kotak_merah`)
   - **Background Color**: Pilih warna untuk memudahkan identifikasi
3. Ulangi untuk semua class yang dibutuhkan
4. Klik **"Save"**

---

## LANGKAH 4: MELAKUKAN ANNOTASI

### 4.1 Memulai Annotation
1. Klik tab **"Labeling"** di menu atas
2. Gambar pertama akan muncul di layar

### 4.2 Cara Memberi Label
1. **Pilih Label**: Klik nama class di panel kanan (contoh: `kotak_merah`)
2. **Buat Bounding Box**:
   - Klik di sudut kiri atas objek
   - Drag ke sudut kanan bawah objek
   - Lepas klik untuk menyelesaikan
3. **Edit Bounding Box**:
   - Klik pada kotak yang sudah dibuat
   - Drag sudut untuk resize
   - Drag tengah kotak untuk memindah
4. **Hapus Annotation**:
   - Pilih kotak annotation
   - Tekan **Delete** di keyboard
   - Atau klik ikon **trash** di panel kanan

### 4.3 Keyboard Shortcuts
| Shortcut | Fungsi |
|----------|--------|
| `Ctrl` + `Z` | Undo |
| `Ctrl` + `Shift` + `Z` | Redo |
| `Delete` | Hapus selected region |
| `1`, `2`, `3`... | Pilih label (urutan) |
| `Ctrl` + `S` | Submit & next |
| `Ctrl` + `Enter` | Skip image |

### 4.4 Multi-Object Annotation
- Satu gambar bisa memiliki banyak bounding box
- Cukup pilih label yang berbeda untuk objek berbeda
- Pastikan setiap objek memiliki label yang tepat

### 4.5 Submit Annotation
1. Setelah selesai memberi label pada sebuah gambar, klik **"Submit"** (pojok kanan bawah)
2. Atau tekan **Ctrl + S** untuk submit dan lanjut ke gambar berikutnya
3. LabelStudio akan menyimpan progress otomatis

---

## LANGKAH 5: MENGEKSPORT DATASET

### 5.1 Export ke Format YOLO
1. Di halaman project, klik **"Export"** di menu atas
2. Pilih format export:
   - Scroll ke bawah cari **"YOLO"** format
   - Atau pilih **"JSON"** (nanti dikonversi manual)
3. Untuk YOLO format langsung:
   - Pilih **"YOLOv5 PyTorch"** (compatible dengan YOLOv8)
   - Klik **"Export"**
4. File akan didownload dalam format ZIP

### 5.2 Jika Export JSON (Manual Conversion)
```bash
# Extract file export
unzip labelstudio_export.json.zip

# Convert ke YOLO format menggunakan script Python
```

Script konversi JSON ke YOLO:
```python
#!/usr/bin/env python3
import json
import os

# Load LabelStudio export
with open('labelstudio_export.json', 'r') as f:
    data = json.load(f)

# Mapping label ke ID
label_map = {
    'kotak_merah': 0,
    'silinder_hijau': 1,
    'bola_biru': 2
}

# Convert setiap annotation
for item in data:
    image_name = item['data']['image'].split('/')[-1]
    image_width = item['data'].get('width', 640)
    image_height = item['data'].get('height', 480)
    
    yolo_labels = []
    
    for annotation in item.get('annotations', []):
        for result in annotation.get('result', []):
            if result['type'] == 'rectanglelabels':
                value = result['value']
                label = value['rectanglelabels'][0]
                
                # Convert ke YOLO format (normalized)
                x_center = (value['x'] + value['width']/2) / 100
                y_center = (value['y'] + value['height']/2) / 100
                width = value['width'] / 100
                height = value['height'] / 100
                
                label_id = label_map.get(label, 0)
                yolo_labels.append(f"{label_id} {x_center} {y_center} {width} {height}")
    
    # Simpan ke file .txt
    label_file = image_name.replace('.jpg', '.txt').replace('.png', '.txt')
    with open(label_file, 'w') as f:
        f.write('\n'.join(yolo_labels))
```

---

## LANGKAH 6: STRUKTUR FOLDER UNTUK YOLO

Setelah export, atur struktur folder:

```bash
# Buat struktur YOLO
mkdir -p dataset_yolo/{train,valid,test}/{images,labels}

# Copy hasil export
# Sesuaikan pembagian data (contoh: 70% train, 20% valid, 10% test)
```

Buat file `data.yaml`:
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

---

## LANGKAH 7: VALIDASI DATASET

```python
#!/usr/bin/env python3
from ultralytics import YOLO

# Cek apakah dataset valid
model = YOLO('yolov8n.pt')

# Validate dataset sebelum training
try:
    results = model.val(data='data.yaml')
    print("Dataset valid!")
except Exception as e:
    print(f"Dataset error: {e}")
```

---

## LANGKAH 8: COPY KE ROS WORKSPACE

```bash
# Copy dataset ke ROS
cp -r dataset_yolo/* \
  /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/ROS_YOLO/src/yolo_ros/training_data/
```

---

## TIPS & BEST PRACTICES

### 1. Konsistensi Label
- Gunakan nama label yang konsisten (lowercase, tanpa spasi)
- Contoh baik: `kotak_merah`, `botol_plastik`
- Contoh buruk: `Kotak Merah`, `Botol-Plastik`

### 2. Kualitas Annotation
- Pastikan bounding box **snug** (pas) dengan objek
- Jangan terlalu longgar (wastes context)
- Jangan terlalu tight (cuts off edges)

### 3. Handle Occlusion
- Jika objek tertutup sebagian, label seperlunya
- Jika objek tertutup >50%, skip atau beri label "uncertain"

### 4. Quality Control
- Review annotation secara berkala
- Gunakan fitur **"Review"** di LabelStudio untuk quality check

---

## TROUBLESHOOTING

### Error: "Port 8080 already in use"
```bash
# Gunakan port lain
label-studio --port 8081
```

### Error: "Images not loading"
- Pastikan path gambar benar saat import
- Gunakan absolute path jika perlu

### Export tidak ada format YOLO
- Gunakan format JSON, lalu konversi manual dengan script

---

## RINGKASAN ALUR

```
1. Install LabelStudio → 2. Create Project → 3. Upload Images →
4. Configure Labels → 5. Annotate (Draw BBox + Label) →
6. Export (YOLO format) → 7. Arrange Folder Structure →
8. Create data.yaml → 9. Copy to ROS → 10. Ready for Training!
```

---

## REFERENSI
- LabelStudio Docs: https://labelstud.io/guide/
- GitHub: https://github.com/HumanSignal/label-studio
- Format YOLO: https://docs.ultralytics.com/datasets/detect/
