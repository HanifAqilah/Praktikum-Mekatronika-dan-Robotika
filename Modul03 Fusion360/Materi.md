# MATERI MODUL 03: FUSION 360 – DESAIN MEKANIK PROJECT LINE FOLLOWER & ORDER PCB

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 03 – Fusion 360 Desain Mekanik  
**Software:** Autodesk Fusion 360 (Education License)  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan – Desain Mekanik dalam Robotika](#1-pendahuluan--desain-mekanik-dalam-robotika)
2. [Pengenalan Autodesk Fusion 360](#2-pengenalan-autodesk-fusion-360)
3. [Workspace dan Antarmuka Fusion 360](#3-workspace-dan-antarmuka-fusion-360)
4. [Sketch (Sketsa 2D)](#4-sketch-sketsa-2d)
5. [Solid Modeling (Pemodelan 3D)](#5-solid-modeling-pemodelan-3d)
6. [Assembly dan Joint](#6-assembly-dan-joint)
7. [Desain Chassis Robot Line Follower](#7-desain-chassis-robot-line-follower)
8. [Desain Bracket Sensor dan Motor Mount](#8-desain-bracket-sensor-dan-motor-mount)
9. [Import dan Integrasi PCB dari EasyEDA](#9-import-dan-integrasi-pcb-dari-easyeda)
10. [Design for Manufacturing (DFM)](#10-design-for-manufacturing-dfm)
11. [Export File untuk 3D Printing dan Laser Cut](#11-export-file-untuk-3d-printing-dan-laser-cut)
12. [Order PCB ke JLCPCB](#12-order-pcb-ke-jlcpcb)
13. [Referensi](#13-referensi)

---

## 1. PENDAHULUAN – DESAIN MEKANIK DALAM ROBOTIKA

### 1.1 Mengapa Desain Mekanik Penting?

Dalam proyek robotika, desain mekanik adalah fondasi fisik yang menentukan:
- **Kekuatan struktural** — chassis harus mampu menopang seluruh komponen
- **Distribusi berat** — keseimbangan robot mempengaruhi performa gerakan
- **Kemudahan assembly** — komponen harus mudah dipasang dan dilepas
- **Kompatibilitas dimensi** — PCB, motor, sensor harus pas secara fisik
- **Manufakturabilitas** — desain harus bisa diproduksi (3D print, laser cut, CNC)

### 1.2 Peran CAD dalam Robotika Modern

**CAD (Computer-Aided Design)** memungkinkan engineer:
- Membuat model 3D akurat sebelum fabrikasi
- Simulasi kekuatan dan toleransi secara virtual
- Mendeteksi tabrakan antar komponen (interference check)
- Menghasilkan file produksi (STL, DXF, STEP) langsung
- Kolaborasi tim menggunakan cloud

### 1.3 Mengapa Fusion 360?

| Fitur | Keunggulan |
|-------|-----------|
| Cloud-based | Akses dari mana saja, auto-save |
| Education License | Gratis penuh untuk mahasiswa |
| Parametric + Direct | Fleksibel dalam pemodelan |
| Assembly & Joint | Simulasi mekanik lengkap |
| PCB Integration | Import desain PCB dari EasyEDA/Eagle |
| Manufacturing | CAM, 3D print prep, drawing terintegrasi |
| Rendering | Visualisasi realistis |

---

## 2. PENGENALAN AUTODESK FUSION 360

### 2.1 Apa itu Fusion 360?

**Autodesk Fusion 360** adalah platform CAD/CAM/CAE terintegrasi berbasis cloud yang menggabungkan:
- **Desain 3D parametrik dan direct modeling**
- **Simulasi struktural** (stress analysis)
- **Generative Design** (AI-driven)
- **CAM** (Computer-Aided Manufacturing) untuk CNC
- **Drawing** untuk gambar teknik 2D
- **Electronics** (integrasi PCB)
- **Rendering** fotorealistik

### 2.2 Instalasi dan Lisensi Education

1. Buka https://www.autodesk.com/education/edu-software/overview
2. Daftar dengan email kampus (.ac.id)
3. Verifikasi status mahasiswa
4. Download Fusion 360 dari https://www.autodesk.com/products/fusion-360
5. Login dengan akun Autodesk Education
6. Lisensi aktif **1 tahun, dapat diperpanjang**

### 2.3 Persyaratan Sistem

| Komponen | Minimum | Rekomendasi |
|----------|---------|-------------|
| OS | Windows 10 / macOS 12 | Windows 11 / macOS 13+ |
| CPU | Intel i5 / Ryzen 5 | Intel i7 / Ryzen 7 |
| RAM | 4 GB | 8–16 GB |
| GPU | DirectX 11 compatible | Dedicated GPU 2 GB+ |
| Internet | Diperlukan untuk login | Stabil |
| Storage | 3 GB free | SSD recommended |

---

## 3. WORKSPACE DAN ANTARMUKA FUSION 360

### 3.1 Workspace Utama

Fusion 360 memiliki beberapa workspace:

| Workspace | Fungsi |
|-----------|--------|
| **Design** | Modeling 3D, sketch, assembly |
| **Render** | Visualisasi fotorealistik |
| **Animation** | Animasi assembly |
| **Simulation** | Analisis struktur (FEA) |
| **Manufacture** | CAM, toolpath CNC |
| **Drawing** | Gambar teknik 2D |

### 3.2 Elemen Antarmuka

```
┌─────────────────────────────────────────────────────────────┐
│ Application Bar: File | Save | Undo | Redo                  │
├─────────────────┬───────────────────────────┬───────────────┤
│   Data Panel    │     Toolbar / Ribbon      │  Inspector    │
│  (Project Tree) │  ┌─────────────────────┐  │               │
│                 │  │  SOLID | SURFACE |   │  │  Properties   │
│  Components     │  │  SHEET METAL |      │  │  Parameters   │
│  Bodies         │  │  FORM | MESH        │  │               │
│  Sketches       │  └─────────────────────┘  │               │
│  Origins        │                           │               │
│  Joints         │     3D VIEWPORT           │               │
│                 │     (Canvas Area)         │               │
├─────────────────┴───────────────────────────┴───────────────┤
│  Timeline (Feature History)                                  │
│  [Sketch1] → [Extrude1] → [Fillet1] → [Shell1] → ...       │
└─────────────────────────────────────────────────────────────┘
```

### 3.3 Navigasi Viewport

| Aksi | Mouse | Touchpad |
|------|-------|----------|
| Orbit | Middle click + drag | 2 finger drag |
| Pan | Middle click + Shift + drag | Shift + 2 finger |
| Zoom | Scroll wheel | Pinch |
| Fit All | Tekan `F` | Tekan `F` |
| Home | Klik View Cube → Home | Klik View Cube |

### 3.4 Shortcut Keyboard Penting

| Shortcut | Fungsi |
|---------|--------|
| `S` | Search Command |
| `L` | Line |
| `C` | Circle |
| `R` | Rectangle |
| `D` | Dimension |
| `E` | Extrude |
| `Q` | Press Pull |
| `M` | Move |
| `J` | Joint |
| `F` | Fillet |
| `Ctrl+Z` | Undo |
| `Ctrl+S` | Save |

---

## 4. SKETCH (SKETSA 2D)

### 4.1 Konsep Sketch

**Sketch** adalah dasar dari semua operasi 3D di Fusion 360. Sketch adalah gambar 2D pada bidang (plane) yang kemudian di-extrude, revolve, atau operasi 3D lainnya.

**Prinsip sketch yang baik:**
- **Fully constrained** — semua garis dan titik terkunci posisinya (tidak bisa digeser)
- **Parametric** — gunakan dimensi (bukan freehand) agar mudah diubah
- **Sederhana** — hindari sketch yang terlalu kompleks, pecah menjadi beberapa fitur

### 4.2 Tools Sketch Dasar

| Tool | Shortcut | Fungsi |
|------|---------|--------|
| Line | `L` | Garis lurus |
| Rectangle | `R` | Persegi panjang (2-point, center) |
| Circle | `C` | Lingkaran (center point, 2-point) |
| Arc | `A` | Busur (3-point, tangent) |
| Polygon | — | Poligon (segi-n) |
| Slot | — | Bentuk slot / oblong |
| Spline | — | Kurva bebas |
| Point | — | Titik referensi |
| Text | — | Teks (untuk engraving) |
| Mirror | — | Cermin sketsa |
| Circular Pattern | — | Pola melingkar |
| Rectangular Pattern | — | Pola persegi |
| Offset | `O` | Offset kontur |
| Trim | `T` | Potong garis berlebih |
| Extend | — | Perpanjang garis |
| Fillet (sketch) | — | Radius pada sudut sketch |
| Chamfer (sketch) | — | Potong sudut miring |

### 4.3 Constraints (Batasan Geometri)

| Constraint | Fungsi |
|-----------|--------|
| Coincident | Titik bertemu di satu lokasi |
| Horizontal | Garis sejajar sumbu X |
| Vertical | Garis sejajar sumbu Y |
| Perpendicular | Dua garis tegak lurus |
| Parallel | Dua garis sejajar |
| Tangent | Garis menyinggung kurva |
| Equal | Dua elemen sama panjang/radius |
| Midpoint | Titik berada di tengah garis |
| Concentric | Dua lingkaran sepusat |
| Fix | Mengunci posisi elemen |
| Symmetric | Simetris terhadap garis sumbu |

### 4.4 Dimension (Ukuran)

- **Sketch Dimension (`D`)** — memberikan ukuran numerik pada garis, sudut, jarak
- Dimensi berwarna:
  - **Hitam**: constrained (terkunci)
  - **Biru**: driven (ikut berubah, tapi tidak dikontrol)
- **User Parameters** — variabel yang dapat digunakan dalam dimensi (misal: `lebar_chassis = 80`)

---

## 5. SOLID MODELING (PEMODELAN 3D)

### 5.1 Operasi 3D Utama

| Operasi | Fungsi | Catatan |
|---------|--------|--------|
| **Extrude** (`E`) | Tarik sketch menjadi solid | New Body / Join / Cut / Intersect |
| **Revolve** | Putar sketch mengelilingi sumbu | Untuk silinder, kerucut |
| **Sweep** | Tarik profil sepanjang path | Untuk bentuk melengkung |
| **Loft** | Hubungkan 2+ profil berbeda | Transisi bentuk gradual |
| **Hole** | Buat lubang standar | Counterbore, countersink, tapped |
| **Thread** | Tambah ulir pada silinder | Modeled / cosmetic |
| **Fillet** (`F`) | Radius pada edge 3D | Radius dalam mm |
| **Chamfer** | Potong sudut miring 3D | Jarak × sudut |
| **Shell** | Buat cangkang berongga | Tentukan ketebalan dinding |
| **Draft** | Miring permukaan (taper) | Untuk injection mold |
| **Pattern** | Duplikasi fitur (rectangular/circular) | Efisien untuk lubang berulang |
| **Mirror** | Cerminkan body/fitur | Simetris kiri-kanan |
| **Combine** | Gabung/potong antar body | Join / Cut / Intersect |
| **Split** | Pisahkan body | Menggunakan plane/face |
| **Press Pull** (`Q`) | Modifikasi langsung face | Direct modeling |

### 5.2 Bodies vs Components

| Konsep | Body | Component |
|--------|------|-----------|
| Definisi | Bentuk solid tunggal | Unit fungsional (bisa berisi banyak bodies) |
| Assembly | Tidak bisa di-joint | Bisa di-joint dan di-assembly |
| Motion | Tidak bisa bergerak terpisah | Bisa bergerak sesuai joint |
| Best practice | Modeling awal | Setiap part yang terpisah fisik |

> **Aturan:** Setiap bagian yang akan dicetak/difabrikasi terpisah harus menjadi **Component** tersendiri.

### 5.3 User Parameters

Parameters memungkinkan desain parametrik:

```
lebar_chassis = 80 mm
panjang_chassis = 120 mm
tebal_akrilik = 3 mm
diameter_roda = 65 mm
jarak_motor = 60 mm
```

Ubah parameter → seluruh desain update otomatis.

**Cara membuat:** Design > Modify > Change Parameters

---

## 6. ASSEMBLY DAN JOINT

### 6.1 Konsep Assembly

Assembly adalah proses merangkai beberapa **component** menjadi satu sistem terintegrasi. Di Fusion 360, assembly dilakukan menggunakan **Joint** dan **As-Built Joint**.

### 6.2 Jenis Joint

| Joint Type | DOF | Contoh Penggunaan |
|-----------|-----|-------------------|
| **Rigid** | 0 | Baut, pengikat tetap |
| **Revolute** | 1 rotasi | Engsel, poros roda |
| **Slider** | 1 translasi | Rel linier |
| **Cylindrical** | 1 rotasi + 1 translasi | Piston |
| **Pin-Slot** | 1 rotasi + 1 translasi (terbatas) | Pin dalam slot |
| **Planar** | 2 translasi + 1 rotasi | Permukaan datar |
| **Ball** | 3 rotasi | Ball joint |

### 6.3 Langkah Assembly

1. Buat atau import setiap part sebagai **Component**
2. Susun secara kasar menggunakan **Move** (`M`)
3. Klik **Joint** (`J`) → pilih titik pada komponen 1 → pilih titik pada komponen 2
4. Pilih tipe joint yang sesuai
5. Atur limit motion jika perlu
6. Test pergerakan dengan **drag** atau **Motion Study**

### 6.4 Contact Set dan Motion Study

- **Contact Set** → mendeteksi tabrakan fisik antar komponen
- **Motion Study** → simulasi gerakan assembly berdasarkan joint
- Berguna untuk memastikan roda berputar tanpa menabrak chassis

---

## 7. DESAIN CHASSIS ROBOT LINE FOLLOWER

### 7.1 Spesifikasi Chassis

| Parameter | Nilai |
|-----------|-------|
| Panjang | 150–180 mm |
| Lebar | 120–150 mm |
| Tebal material | 3 mm (akrilik) atau PLA 3D print |
| Jumlah level | 2 (bawah: motor+sensor, atas: PCB+baterai) |
| Lubang mounting | M3 (diameter 3.2 mm) |
| Berat target | < 200 g (tanpa komponen) |

### 7.2 Langkah Desain Chassis Bawah

1. **New Component** → rename: `Chassis_Bottom`
2. **Create Sketch** pada bidang XY
3. Gambar outline chassis: Rectangle 150 × 120 mm
4. Tambahkan **fillet** pada sudut: R=10 mm (estetika + kekuatan)
5. Tambahkan lubang motor mount:
   - 2 slot untuk motor DC (sesuai dimensi motor)
   - Jarak antar motor = `jarak_motor` parameter
6. Tambahkan lubang mounting PCB: 4× M3 (pola 4-hole sesuai PCB MAIN)
7. Tambahkan lubang mounting sensor board: slot memanjang di depan
8. Tambahkan lubang caster wheel: 2× M3 di bagian belakang
9. **Extrude**: 3 mm (tebal akrilik)
10. **Fillet** edge atas: 0.5 mm

### 7.3 Langkah Desain Chassis Atas

1. **New Component** → rename: `Chassis_Top`
2. Sketch referensi dari chassis bawah (Project from body)
3. Gambar outline yang sama (atau sedikit offset)
4. Lubang mounting baterai: slot + penahan
5. Lubang kabel: 2× lubang oval untuk routing kabel antar-deck
6. Lubang mounting OLED display: sesuai dimensi OLED 0.96"
7. **Extrude**: 3 mm

### 7.4 Spacer dan Standoff

- Gunakan silinder tinggi 20–30 mm sebagai spacer antar deck
- Lubang M3 di tengah
- Bisa 3D print atau beli standoff M3 × 25 mm

---

## 8. DESAIN BRACKET SENSOR DAN MOTOR MOUNT

### 8.1 Motor Mount

Fungsi: menahan motor DC pada chassis secara rigid.

Langkah:
1. Ukur dimensi motor DC (diameter, panjang, posisi poros)
2. Buat bracket L-shape atau U-shape
3. Lubang mounting ke chassis: M3
4. Lubang pengikat motor: sesuai body motor (biasanya M2 atau clamp)
5. Material: PLA (3D print) tebal 3–4 mm

### 8.2 Sensor Board Bracket

Fungsi: menahan PCB sensor line pada ketinggian 5–10 mm dari lantai.

Langkah:
1. Buat bracket berbentuk L
2. Sisi horizontal: mounting ke chassis (M3)
3. Sisi vertikal: mounting sensor board PCB (M3, 2 lubang)
4. Sudut kemiringan: 0° (horizontal) atau sedikit miring 5°

### 8.3 Servo Bracket

Untuk mounting SG90 servo:
1. Dimensi SG90: 32.5 × 12 × 32.5 mm (dengan flange)
2. Buat pocket/slot sesuai body servo
3. Lubang mounting flange: M2
4. Lubang poros atas: bebas (untuk horn servo)

---

## 9. IMPORT DAN INTEGRASI PCB DARI EASYEDA

### 9.1 Export PCB dari EasyEDA

1. Di EasyEDA: **File > Export > Altium Designer** atau **File > Export > STEP 3D**
2. Jika STEP: langsung import ke Fusion 360
3. Jika belum ada 3D: export board outline sebagai DXF

### 9.2 Import ke Fusion 360

1. **File > Open > Upload** → pilih file STEP/DXF
2. STEP: muncul sebagai body 3D lengkap
3. DXF: muncul sebagai sketch → extrude sesuai tebal PCB (1.6 mm)
4. Konversi menjadi **Component** → rename: `PCB_Main`

### 9.3 Penempatan PCB di Assembly

1. Gunakan **Joint** untuk menempatkan PCB pada chassis
2. Pastikan lubang mounting PCB sejajar dengan lubang chassis
3. Cek clearance vertikal (komponen tertinggi pada PCB vs spacer)
4. Cek clearance horizontal (PCB tidak menonjol keluar chassis)

### 9.4 Section Analysis

**Inspect > Section Analysis** — potong assembly untuk melihat clearance internal:
- Jarak PCB ke chassis bawah
- Jarak komponen tinggi (kapasitor elektrolit, konektor) ke chassis atas
- Jarak sensor ke lantai

---

## 10. DESIGN FOR MANUFACTURING (DFM)

### 10.1 Prinsip DFM

| Prinsip | Penjelasan |
|---------|-----------|
| Minimum wall thickness | ≥ 1 mm untuk 3D print, ≥ 2 mm untuk akrilik |
| Min hole diameter | ≥ 1 mm (3D print), ≥ 2 mm (laser cut) |
| Toleransi fit | ± 0.2 mm untuk press fit, ± 0.5 mm untuk clearance |
| Overhang | ≤ 45° tanpa support (FDM 3D print) |
| Fillet | Tambahkan fillet pada sudut tajam internal |
| Kerf (laser) | Kompensasi ± 0.1 mm untuk laser cutting |

### 10.2 DFM untuk 3D Printing (FDM)

| Parameter | Nilai Rekomendasi |
|-----------|------------------|
| Layer height | 0.2 mm (standar) |
| Wall thickness | ≥ 1.2 mm (3 perimeter × 0.4 nozzle) |
| Infill | 20–30% (chassis), 50%+ (bracket beban) |
| Bridging max | 30 mm tanpa support |
| Hole diameter | Tambah 0.2 mm dari nominal |
| Support | Hindari jika memungkinkan |

### 10.3 DFM untuk Laser Cutting (Akrilik)

| Parameter | Nilai |
|-----------|-------|
| Material | Akrilik 3 mm |
| Min detail | 1 mm |
| Min slot | 3 mm (= tebal material) |
| Kerf compensation | 0.1–0.15 mm |
| Tab/slot joint | Slot = tebal material + 0.1 mm |

---

## 11. EXPORT FILE UNTUK 3D PRINTING DAN LASER CUT

### 11.1 Export STL (3D Print)

1. Klik kanan body/component → **Save as STL**
2. Refinement: **High** (untuk part kecil)
3. Format: Binary STL
4. Buka di slicer (Cura, PrusaSlicer) untuk generate G-code

### 11.2 Export DXF (Laser Cut)

1. Buat **Drawing** dari Fusion 360 (workspace Drawing)
2. Atau: klik face datar → **Create Sketch From Faces**
3. **File > Export** → format DXF
4. Buka di software laser (LaserGRBL, LightBurn, RDWorks)

### 11.3 Export STEP (Universal)

1. **File > Export** → format STEP (.stp)
2. Berguna untuk transfer antar-CAD software
3. STEP mempertahankan geometri parametrik dasar

---

## 12. ORDER PCB KE JLCPCB

### 12.1 Proses Order dari EasyEDA

1. Di EasyEDA: **Fabrication > Order at JLCPCB**
2. File Gerber otomatis dikirim ke JLCPCB
3. Review parameter:

| Parameter | Nilai Standar |
|-----------|--------------|
| PCB Qty | 5 pcs |
| Layers | 2 |
| Thickness | 1.6 mm |
| Color | Green |
| Surface | HASL Lead-free |
| Copper | 1 oz |
| Stencil | Opsional |

4. Submit order, bayar, tunggu produksi (3–7 hari) + pengiriman

### 12.2 Verifikasi Gerber Sebelum Order

1. Upload ke https://gerber-viewer.jlcpcb.com
2. Periksa setiap layer: top copper, bottom copper, mask, silk, outline, drill
3. Pastikan tidak ada artefak atau trace yang hilang

### 12.3 Tips Hemat Biaya

- Gabungkan PCB kecil dalam 1 panel (panelize di EasyEDA)
- Pilih warna hijau (paling murah)
- Pilih HASL (bukan ENIG) kecuali untuk fine-pitch
- Shipping: pilih yang ekonomis jika tidak urgent
- Cek kupon/promo JLCPCB untuk order pertama

---

## 13. REFERENSI

1. Autodesk. (2024). *Fusion 360 Official Documentation*. https://help.autodesk.com/view/fusion360/ENU/
2. Autodesk Learning. (2024). *Design and Design to Manufacturing with Fusion 360*. https://learn.autodesk.com/paths/design-and-design-to-manufacturing-with-fusion-360
3. Autodesk. (2024). *Design for Manufacturing (DFM) in Fusion 360*. https://www.autodesk.com/products/fusion-360/resources
4. Autodesk Fusion. (2024). *PCB Design to Manufacturing*. https://learn.autodesk.com/paths/design-pcbs-with-fusion-360
5. JLCPCB. (2024). *Manufacturing Capabilities*. https://jlcpcb.com/capabilities
6. Chua, C. K., Leong, K. F., & Lim, C. S. (2010). *Rapid Prototyping: Principles and Applications*. World Scientific.
7. Groover, M. P. (2016). *Fundamentals of Modern Manufacturing*. Wiley.

---

Dokumen ini menjadi acuan utama materi Modul 03 Fusion 360 untuk desain mekanik robot line follower.
