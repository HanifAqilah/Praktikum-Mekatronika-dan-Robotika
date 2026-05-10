# MATERI MODUL 03: FUSION 360 – DESAIN MEKANIK PROJECT LINE FOLLOWER

**Program Studi:** Teknologi Rekayasa Otomasi
**Mata Kuliah:** Praktikum Mekatronika dan Robotika
**Modul:** 03 – Fusion 360 Desain Mekanik
**Software:** Autodesk Fusion 360 (Education License) / Fusion 360 Online
**Estimasi Waktu Belajar:** 8–10 Jam

---

## DAFTAR ISI

1. [Pendahuluan – Desain Mekanik dalam Robotika](#1-pendahuluan--desain-mekanik-dalam-robotika)
2. [Pengenalan dan Setup Fusion 360](#2-pengenalan-dan-setup-fusion-360)
   - 2.1 Apa itu Fusion 360?
   - 2.2 Registrasi Lisensi Education dengan Email SSO Kampus
   - 2.3 Fusion 360 Online — Akses via Browser
   - 2.4 Perbandingan Desktop vs Online
   - 2.5 Persyaratan Sistem
3. [Workspace dan Antarmuka Fusion 360](#3-workspace-dan-antarmuka-fusion-360)
4. [Sketch (Sketsa 2D)](#4-sketch-sketsa-2d)
5. [Solid Modeling (Pemodelan 3D)](#5-solid-modeling-pemodelan-3d)
6. [Assembly dan Joint](#6-assembly-dan-joint)
7. [Desain Chassis Robot Line Follower](#7-desain-chassis-robot-line-follower)
8. [Desain Komponen, Bracket, Mount, dan Dudukan](#8-desain-komponen-bracket-mount-dan-dudukan)
   - 8.0a Model 3D Motor DC Gearbox (Referensi Assembly)
   - 8.0b Model 3D Roda (Referensi Assembly)
   - 8.1 Motor Mount (3D Print PLA)
   - 8.2 Cover/Enclosure PCB Sensor Garis (3D Print PLA)
   - 8.3 Dudukan PCB MAIN — Standoff (3D Print PLA)
   - 8.4 Bracket 3 Sensor Jarak HC-SR04 — Akrilik Bending, Sheet Metal Method, Depan 0°, Kiri/Kanan 20–25°
   - 8.5 Bracket Servo SG90 (3D Print PLA)
   - 8.6 Bracket Baterai 2×18650 + Model 3D Baterai (3D Print PLA)
   - 8.7 Aksesoris 3D Bebas (3D Print PLA)
9. [Import dan Integrasi PCB dari EasyEDA](#9-import-dan-integrasi-pcb-dari-easyeda)
10. [Design for Manufacturing (DFM) — Produksi Mandiri](#10-design-for-manufacturing-dfm--produksi-mandiri)
11. [Fabrikasi Mandiri: 3D Printing dan Laser Cutting](#11-fabrikasi-mandiri-3d-printing-dan-laser-cutting)
12. [Export dan Produksi File](#12-export-dan-produksi-file)
13. [Referensi](#14-referensi)

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

| Fitur               | Keunggulan                               |
| ------------------- | ---------------------------------------- |
| Cloud-based         | Akses dari mana saja, auto-save          |
| Education License   | Gratis penuh untuk mahasiswa             |
| Parametric + Direct | Fleksibel dalam pemodelan                |
| Assembly & Joint    | Simulasi mekanik lengkap                 |
| PCB Integration     | Import desain PCB dari EasyEDA/Eagle     |
| Manufacturing       | CAM, 3D print prep, drawing terintegrasi |
| Rendering           | Visualisasi realistis                    |

---

## 2. PENGENALAN DAN SETUP FUSION 360

### 2.1 Apa itu Fusion 360?

**Autodesk Fusion 360** adalah platform CAD/CAM/CAE terintegrasi berbasis cloud yang menggabungkan:

- **Desain 3D parametrik dan direct modeling**
- **Simulasi struktural** (stress analysis / FEA)
- **CAM** (Computer-Aided Manufacturing) untuk CNC dan 3D Print
- **Drawing** untuk gambar teknik 2D
- **Electronics** (integrasi PCB)
- **Rendering** fotorealistik dan animasi

---

### 2.2 Registrasi Lisensi Education dengan Email SSO Kampus

> **Lisensi Education Fusion 360 GRATIS penuh** untuk mahasiswa yang menggunakan email institusi/kampus. Ikuti langkah berikut:

#### Langkah 1 — Buka Halaman Education

1. Buka browser dan kunjungi: **https://www.autodesk.com/education/edu-software/fusion**
2. Klik tombol **"Get Fusion 360"** atau **"Free Access"**

#### Langkah 2 — Buat atau Login Akun Autodesk

3. Klik **"Create Account"** (jika belum punya akun Autodesk)
4. Pada form pendaftaran, **gunakan email institusi/kampus** (contoh: `nama@student.universitasanda.ac.id`)
   > ⚠️ **Penting:** Gunakan email SSO kampus, bukan Gmail/Yahoo. Autodesk akan memverifikasi status mahasiswa melalui domain email ini.
   >
5. Isi data: First Name, Last Name, Password
6. Klik **"Create Account"** dan cek email kampus untuk verifikasi

#### Langkah 3 — Verifikasi Status Mahasiswa (Education Verification)

7. Setelah login, kamu akan diarahkan ke halaman **Education Verification**
8. Pilih **"I am a Student"**
9. Isi data institusi:
   - **Country**: Indonesia
   - **Institution**: ketik nama universitas/politeknik, pilih dari daftar
   - **Role**: Student
   - **Enrollment Date** dan **Graduation Date** (perkiraan)
10. Jika institusimu **terdaftar di SheerID** (sistem verifikasi Autodesk), verifikasi otomatis selesai dalam beberapa detik
11. Jika tidak otomatis, upload **bukti mahasiswa aktif** (KTM atau surat keterangan aktif kuliah)

#### Langkah 4 — Aktivasi Lisensi

12. Setelah terverifikasi, kunjungi: **https://manage.autodesk.com** atau langsung masuk ke **Autodesk Account**
13. Lisensi Education Fusion 360 akan muncul di bawah **"Products & Services"**
14. Lisensi berlaku **1 tahun** dan dapat **diperpanjang** setiap tahun selama masih berstatus mahasiswa

#### Langkah 5 — Download Desktop (Opsional)

15. Dari halaman produk, klik **"Download"** untuk mendapatkan installer Fusion 360 desktop
16. Install dan login dengan akun Autodesk Education yang sudah dibuat

> **Catatan SSO:** Beberapa kampus menggunakan SSO (Single Sign-On) terpusat. Jika kamu login ke email kampus melalui SSO Google Workspace atau Microsoft 365, gunakan opsi **"Sign in with Google"** atau **"Sign in with Microsoft"** di halaman Autodesk — pastikan email yang terpakai adalah email SSO kampus, bukan akun pribadi.

---

### 2.3 Fusion 360 Online — Akses via Browser

**Fusion 360 Online** memungkinkan penggunaan Fusion 360 langsung dari browser tanpa perlu install software apapun.

#### Cara Mengakses

1. Buka browser (Chrome/Edge disarankan, minimal Chromium 80+)
2. Kunjungi: **https://fusion.online.autodesk.com/**
3. Login dengan akun Autodesk Education yang sudah dibuat
4. Tunggu loading sebentar — Fusion 360 akan terbuka langsung di browser

#### Panduan Penggunaan Fusion 360 Online

```
[Browser Tab] → https://fusion.online.autodesk.com/
       ↓
[Login Autodesk Account]
       ↓
[Fusion 360 Web App Loading...]
       ↓
┌─────────────────────────────────────────────┐
│  ☰  Autodesk Fusion         [User] [?]      │
├──────────┬──────────────────────────────────┤
│          │   Toolbar (SOLID/MESH/SURFACE)    │
│ Project  │                                  │
│ Browser  │        3D Viewport               │
│ (Tree)   │                                  │
│          │                                  │
├──────────┴──────────────────────────────────┤
│  Timeline (Feature History)                 │
└─────────────────────────────────────────────┘
```

#### Fitur yang Tersedia di Versi Online

| Fitur               | Online        | Desktop   |
| ------------------- | ------------- | --------- |
| Sketch 2D           | ✅            | ✅        |
| Solid Modeling      | ✅            | ✅        |
| Assembly & Joint    | ✅            | ✅        |
| Rendering           | ✅ (basic)    | ✅ (full) |
| Simulation (FEA)    | ⚠️ Terbatas | ✅        |
| Drawing (2D)        | ✅            | ✅        |
| Export STL/DXF/STEP | ✅            | ✅        |
| Auto-save Cloud     | ✅            | ✅        |
| Offline mode        | ❌            | ✅        |
| CAM (G-code)        | ❌            | ✅        |

#### Tips Fusion 360 Online

- **Gunakan Chrome/Edge** versi terbaru untuk performa terbaik
- **RAM laptop minimal 8 GB** — tab browser Fusion cukup memakan memori
- Aktifkan **Hardware Acceleration** di browser: `Settings > System > Use hardware acceleration`
- Semua file tersimpan otomatis di **Autodesk Cloud (Fusion Team)**
- Shortcut keyboard **sama persis** dengan versi desktop
- Untuk upload file lokal (STEP, DXF): gunakan **File > Open > Upload from my computer**

---

### 2.4 Perbandingan Desktop vs Online

| Aspek       | Fusion 360 Desktop      | Fusion 360 Online     |
| ----------- | ----------------------- | --------------------- |
| Instalasi   | Perlu install (~3 GB)   | Tidak perlu install   |
| Akses       | Bisa offline (terbatas) | Perlu internet        |
| Performa    | Lebih smooth/cepat      | Tergantung koneksi    |
| Fitur       | Penuh                   | Sebagian besar        |
| Ideal untuk | Proyek kompleks         | Belajar, revisi cepat |
| Platform    | Windows/macOS           | Browser manapun       |

---

### 2.5 Persyaratan Sistem

| Komponen | Minimum                | Rekomendasi            |
| -------- | ---------------------- | ---------------------- |
| OS       | Windows 10 / macOS 12  | Windows 11 / macOS 13+ |
| CPU      | Intel i5 / Ryzen 5     | Intel i7 / Ryzen 7     |
| RAM      | 4 GB                   | 8–16 GB               |
| GPU      | DirectX 11 compatible  | Dedicated GPU 2 GB+    |
| Internet | Diperlukan untuk login | Stabil ≥ 10 Mbps      |
| Storage  | 3 GB free              | SSD recommended        |

---

## 3. WORKSPACE DAN ANTARMUKA FUSION 360

### 3.1 Workspace Utama

Fusion 360 memiliki beberapa workspace:

| Workspace             | Fungsi                        |
| --------------------- | ----------------------------- |
| **Design**      | Modeling 3D, sketch, assembly |
| **Render**      | Visualisasi fotorealistik     |
| **Animation**   | Animasi assembly              |
| **Simulation**  | Analisis struktur (FEA)       |
| **Manufacture** | CAM, toolpath CNC             |
| **Drawing**     | Gambar teknik 2D              |

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

| Aksi    | Mouse                       | Touchpad         |
| ------- | --------------------------- | ---------------- |
| Orbit   | Middle click + drag         | 2 finger drag    |
| Pan     | Middle click + Shift + drag | Shift + 2 finger |
| Zoom    | Scroll wheel                | Pinch            |
| Fit All | Tekan `F`                 | Tekan `F`      |
| Home    | Klik View Cube → Home      | Klik View Cube   |

### 3.4 Shortcut Keyboard Penting

| Shortcut   | Fungsi         |
| ---------- | -------------- |
| `S`      | Search Command |
| `L`      | Line           |
| `C`      | Circle         |
| `R`      | Rectangle      |
| `D`      | Dimension      |
| `E`      | Extrude        |
| `Q`      | Press Pull     |
| `M`      | Move           |
| `J`      | Joint          |
| `F`      | Fillet         |
| `Ctrl+Z` | Undo           |
| `Ctrl+S` | Save           |

---

## 4. SKETCH (SKETSA 2D)

### 4.1 Konsep Sketch

**Sketch** adalah dasar dari semua operasi 3D di Fusion 360. Sketch adalah gambar 2D pada bidang (plane) yang kemudian di-extrude, revolve, atau operasi 3D lainnya.

**Prinsip sketch yang baik:**

- **Fully constrained** — semua garis dan titik terkunci posisinya (tidak bisa digeser)
- **Parametric** — gunakan dimensi (bukan freehand) agar mudah diubah
- **Sederhana** — hindari sketch yang terlalu kompleks, pecah menjadi beberapa fitur

### 4.2 Tools Sketch Dasar

| Tool                | Shortcut | Fungsi                            |
| ------------------- | -------- | --------------------------------- |
| Line                | `L`    | Garis lurus                       |
| Rectangle           | `R`    | Persegi panjang (2-point, center) |
| Circle              | `C`    | Lingkaran (center point, 2-point) |
| Arc                 | `A`    | Busur (3-point, tangent)          |
| Polygon             | —       | Poligon (segi-n)                  |
| Slot                | —       | Bentuk slot / oblong              |
| Spline              | —       | Kurva bebas                       |
| Point               | —       | Titik referensi                   |
| Text                | —       | Teks (untuk engraving)            |
| Mirror              | —       | Cermin sketsa                     |
| Circular Pattern    | —       | Pola melingkar                    |
| Rectangular Pattern | —       | Pola persegi                      |
| Offset              | `O`    | Offset kontur                     |
| Trim                | `T`    | Potong garis berlebih             |
| Extend              | —       | Perpanjang garis                  |
| Fillet (sketch)     | —       | Radius pada sudut sketch          |
| Chamfer (sketch)    | —       | Potong sudut miring               |

### 4.3 Constraints (Batasan Geometri)

| Constraint    | Fungsi                         |
| ------------- | ------------------------------ |
| Coincident    | Titik bertemu di satu lokasi   |
| Horizontal    | Garis sejajar sumbu X          |
| Vertical      | Garis sejajar sumbu Y          |
| Perpendicular | Dua garis tegak lurus          |
| Parallel      | Dua garis sejajar              |
| Tangent       | Garis menyinggung kurva        |
| Equal         | Dua elemen sama panjang/radius |
| Midpoint      | Titik berada di tengah garis   |
| Concentric    | Dua lingkaran sepusat          |
| Fix           | Mengunci posisi elemen         |
| Symmetric     | Simetris terhadap garis sumbu  |

### 4.4 Dimension (Ukuran)

- **Sketch Dimension (`D`)** — memberikan ukuran numerik pada garis, sudut, jarak
- Dimensi berwarna:
  - **Hitam**: constrained (terkunci)
  - **Biru**: driven (ikut berubah, tapi tidak dikontrol)
- **User Parameters** — variabel yang dapat digunakan dalam dimensi (misal: `lebar_chassis = 80`)

---

## 5. SOLID MODELING (PEMODELAN 3D)

### 5.1 Operasi 3D Utama

| Operasi                      | Fungsi                                 | Catatan                           |
| ---------------------------- | -------------------------------------- | --------------------------------- |
| **Extrude** (`E`)    | Tarik sketch menjadi solid             | New Body / Join / Cut / Intersect |
| **Revolve**            | Putar sketch mengelilingi sumbu        | Untuk silinder, kerucut           |
| **Sweep**              | Tarik profil sepanjang path            | Untuk bentuk melengkung           |
| **Loft**               | Hubungkan 2+ profil berbeda            | Transisi bentuk gradual           |
| **Hole**               | Buat lubang standar                    | Counterbore, countersink, tapped  |
| **Thread**             | Tambah ulir pada silinder              | Modeled / cosmetic                |
| **Fillet** (`F`)     | Radius pada edge 3D                    | Radius dalam mm                   |
| **Chamfer**            | Potong sudut miring 3D                 | Jarak × sudut                    |
| **Shell**              | Buat cangkang berongga                 | Tentukan ketebalan dinding        |
| **Draft**              | Miring permukaan (taper)               | Untuk injection mold              |
| **Pattern**            | Duplikasi fitur (rectangular/circular) | Efisien untuk lubang berulang     |
| **Mirror**             | Cerminkan body/fitur                   | Simetris kiri-kanan               |
| **Combine**            | Gabung/potong antar body               | Join / Cut / Intersect            |
| **Split**              | Pisahkan body                          | Menggunakan plane/face            |
| **Press Pull** (`Q`) | Modifikasi langsung face               | Direct modeling                   |

### 5.2 Bodies vs Components

| Konsep        | Body                         | Component                                   |
| ------------- | ---------------------------- | ------------------------------------------- |
| Definisi      | Bentuk solid tunggal         | Unit fungsional (bisa berisi banyak bodies) |
| Assembly      | Tidak bisa di-joint          | Bisa di-joint dan di-assembly               |
| Motion        | Tidak bisa bergerak terpisah | Bisa bergerak sesuai joint                  |
| Best practice | Modeling awal                | Setiap part yang terpisah fisik             |

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

| Joint Type            | DOF                               | Contoh Penggunaan    |
| --------------------- | --------------------------------- | -------------------- |
| **Rigid**       | 0                                 | Baut, pengikat tetap |
| **Revolute**    | 1 rotasi                          | Engsel, poros roda   |
| **Slider**      | 1 translasi                       | Rel linier           |
| **Cylindrical** | 1 rotasi + 1 translasi            | Piston               |
| **Pin-Slot**    | 1 rotasi + 1 translasi (terbatas) | Pin dalam slot       |
| **Planar**      | 2 translasi + 1 rotasi            | Permukaan datar      |
| **Ball**        | 3 rotasi                          | Ball joint           |

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

> **ℹ️ CHASSIS SUDAH JADI** — Chassis robot menggunakan **bodi box satu-piece dari metal (aluminium/baja)** seperti pada gambar. Ini adalah chassis jenis *box chassis* — satu lembar pelat yang dilipat/dibending sehingga membentuk kotak dengan dinding samping, bawah, dan atas terintegrasi dalam satu komponen. Tugas praktikan adalah **memodelkan chassis ini di Fusion 360 menggunakan workspace Sheet Metal** sebagai referensi assembly.

> **⚠️ WAJIB UKUR** — Gunakan **penggaris ukur (30 cm)** untuk mengukur dimensi aktual chassis fisik sebelum membuat model di Fusion 360.

| Parameter       | Nilai (Estimasi — ukur aktual)                 |
| --------------- | ----------------------------------------------- |
| Panjang         | 150–200 mm                                     |
| Lebar           | 120–160 mm                                     |
| Tinggi box      | 50–80 mm (satu bodi terintegrasi)              |
| Material        | **Pelat metal (aluminium alloy)**         |
| Tebal pelat     | 1–2 mm (ukur aktual)                           |
| Tipe            | **Single-piece box chassis** (1 komponen) |
| Lubang motor    | Slot pada sisi kiri + kanan                     |
| Lubang mounting | M3 (diameter 3.2 mm)                            |
| Berat target    | < 300 g (tanpa komponen)                        |

> **Perbedaan dengan chassis 2-deck:** Box chassis ≠ 2 plat terpisah + spacer. Ini adalah SATU bodi box yang sudah mencakup rangka atas, bawah, dan samping. Motor dimasukkan dari samping (poros menembus lubang di dinding samping), semua komponen dipasang di dalam atau di atas box.

### 7.2 Langkah Memodelkan Box Chassis di Fusion 360 (Sheet Metal Workspace)

> Chassis fisik sudah jadi. Tujuan langkah ini adalah **membuat model digital 3D box chassis satu-piece** sebagai referensi assembly di Fusion 360.

1. Ganti workspace: klik dropdown `SOLID` → pilih **`SHEET METAL`**
2. **Sheet Metal > Sheet Metal Rules** → atur:
   - Thickness: **tebal pelat aktual** (contoh: 1.5 mm)
   - Material: Aluminium Alloy / Steel
   - Bend Radius: 1× tebal pelat (default)
3. **Create > New Component** → nama: `Chassis_Box`
4. **Create Sketch** pada bidang XY — gambar profil **bottom plate** (alas kotak):
   - Outline sesuai ukuran aktual
   - Lubang motor: slot oval atau persegi pada posisi aktual (kiri + kanan)
   - Lubang-lubang cutout sesuai fisik (untuk berat, kabel, akses)
5. **Sheet Metal > Flange** (= Base Flange) → jadikan plat dasar
6. Tambahkan **Flange** pada ke-4 sisi sebagai dinding samping:
   - **Sheet Metal > Flange** → pilih edge kiri dan kanan → atur tinggi dinding = tinggi kotak aktual
   - **Sheet Metal > Flange** → edge depan dan belakang → sesuaikan dengan geometry box
7. Pada **dinding samping** (kiri/kanan): tambahkan lubang untuk poros motor:
   - Sketch circle Ø sesuai poros motor + sedikit clearance → **Cut**
   - Tambahkan slot/lubang lainnya sesuai fisik
8. Pada **dinding depan**: tambahkan lubang atau slot untuk bracket sensor, mounting gripper
9. Pada **top/bagian atas**: tambahkan flange atas atau biarkan terbuka (sesuai fisik)
10. **Sheet Metal > Flat Pattern** → verifikasi geometri flat (semua bend line terlihat)
11. Tidak perlu export DXF — chassis sudah jadi, model hanya untuk referensi assembly

| Fitur                  | Nilai (ukur aktual)           |
| ---------------------- | ----------------------------- |
| Panjang bottom plate   | \_\_\_ mm                     |
| Lebar bottom plate     | \_\_\_ mm                     |
| Tinggi dinding samping | \_\_\_ mm                     |
| Tebal pelat            | \_\_\_ mm                     |
| Lubang poros motor     | Ø\_\_\_ mm × 2 (kiri+kanan) |
| Lubang mounting PCB    | Ø3.2 mm × 4 (M3)            |

**Checkpoint ✅:** Model 3D box chassis selesai (1 komponen Sheet Metal), Flat Pattern terverifikasi.

---

## 8. DESAIN KOMPONEN, BRACKET, MOUNT, DAN DUDUKAN

> **Catatan:** Chassis (metal) dan roda sudah ada secara fisik. Pada bagian ini praktikan: **(1) membuat model 3D referensi** komponen fisik yang ada (motor, roda, baterai) untuk keperluan assembly dan clearance check, lalu **(2) mendesain semua bracket dan dudukan** yang akan diproduksi mandiri. **Ukur semua komponen fisik dengan penggaris sebelum mulai!**

---

### 8.0a Model 3D Motor DC Gearbox (Referensi Assembly)

Fungsi: membuat model 3D motor DC gearbox untuk referensi posisi dan clearance dalam assembly Fusion 360. Model ini **tidak diproduksi** — hanya referensi visual.

Langkah:

1. **Ukur motor DC TT** dengan penggaris: panjang body, lebar, tinggi, diameter poros, posisi transmisi
2. **Create > New Component** → nama: `Motor_DC_Gearbox`
3. Buat sketch profil samping motor (bentuk kotak dengan radius corner)
4. **Extrude** sesuai kedalaman body
5. Tambahkan detail:
   - Poros output: silinder Ø aktual, panjang poros
   - Transmisi/gearbox: blok persegi panjang di sisi motor
   - Lubang mounting (jika ada) sebagai referensi
6. Gunakan **Appearance** → atur warna abu-abu / kuning (sesuai warna motor fisik)

| Parameter      | Nilai Estimasi (ukur aktual) |
| -------------- | ---------------------------- |
| Panjang body   | 70 mm                        |
| Lebar          | 22 mm                        |
| Tinggi         | 18 mm                        |
| Diameter poros | Ø5 mm                       |

> **Tidak ada file produksi untuk komponen ini** — hanya `.f3d` referensi assembly.

---

### 8.0b Model 3D Roda (Referensi Assembly)

Fungsi: membuat model 3D roda untuk referensi dalam assembly. **Tidak diproduksi** — roda fisik sudah ada.

Langkah:

1. **Ukur roda** dengan penggaris: diameter luar, lebar ban, diameter hub, diameter lubang poros
2. **Create > New Component** → nama: `Roda`
3. **Create Sketch** → gambar profil roda:
   - Lingkaran luar: Ø aktual
   - Hub tengah: Ø aktual
   - Lubang poros: Ø aktual
4. **Revolve** 360° terhadap sumbu tengah → dapat bentuk silinder roda
5. Tambahkan detail ban jika perlu (chamfer pada tepi luar)
6. **Appearance** → atur warna hitam (ban) + abu-abu (hub)
7. **Mirror / Copy** → buat versi kiri dan kanan

| Parameter             | Nilai Estimasi (ukur aktual) |
| --------------------- | ---------------------------- |
| Diameter luar         | Ø65 mm                      |
| Lebar ban             | 25 mm                        |
| Diameter lubang poros | Ø5 mm                       |

> **Tidak ada file produksi untuk komponen ini** — hanya `.f3d` referensi assembly.

---

### 8.1 Motor Mount

Fungsi: menahan motor DC pada chassis secara rigid.

Langkah:

1. **Ukur dimensi motor DC TT** menggunakan penggaris: diameter body, panjang, posisi poros, jarak antar lubang baut
2. Buat bracket L-shape atau U-shape
3. Lubang mounting ke chassis: M3 (Ø3.2 mm)
4. Lubang pengikat motor: sesuai body motor (biasanya clamp atau sisi-baut M2)
5. Material: PLA (3D print) tebal 3–4 mm
6. Tambahkan rib penguat pada sudut bracket
7. Buat versi kiri dan kanan (**Mirror feature**)

### 8.2 Cover / Enclosure PCB Sensor Garis (3D Print PLA)

Fungsi: (1) **bracket L** untuk menahan PCB sensor garis pada ketinggian optimal dari lantai, dan (2) **cover pelindung** berupa casing 3D print yang menutupi bagian atas PCB sensor agar terlindungi dari benturan dan debu.

**Bagian A — Bracket Mount (L-shape):**

1. **Ukur PCB Sensor Garis** dengan penggaris: panjang, lebar, posisi lubang mounting M3
2. Buat bracket L-shape:
   - Sisi horizontal: mounting ke depan chassis bawah (2× M3)
   - Sisi vertikal: **slot adjustable** (bukan lubang bulat) agar ketinggian bisa digeser
   - Ketinggian sensor dari lantai: **5–10 mm**
3. Material: PLA 3D print, tebal 3 mm

**Bagian B — Cover/Casing Pelindung PCB Sensor:**

1. **Ukur PCB Sensor** (panjang, lebar, tinggi komponen tertinggi)
2. Desain box/cover berbentuk U atau kanopi yang menutupi PCB dari atas:
   - Dimensi dalam: panjang PCB + 1 mm × lebar PCB + 1 mm (clearance)
   - Tinggi dinding: minimal setinggi komponen tertinggi + 2 mm
   - Bagian bawah: **terbuka** (agar sensor masuk ke slot bracket)
   - Bagian atas: tertutup (pelindung)
3. Tambahkan clip atau lubang M2 kecil untuk mengunci cover ke bracket
4. Tambahkan **slot/ventilasi** agar kabel sensor dapat keluar
5. **Appearance** → warna cerah agar posisi sensor mudah dilihat
6. Material: PLA 3D print, tebal dinding 2 mm

| Parameter                  | Nilai                                |
| -------------------------- | ------------------------------------ |
| Bracket — slot adjustable | Tinggi slot 10 mm, lebar slot 3.5 mm |
| Cover — clearance dalam   | +1 mm dari dimensi PCB               |
| Cover — tebal dinding     | 2 mm                                 |
| Material                   | PLA 3D print                         |

### 8.3 Dudukan PCB MAIN — Standoff (3D Print PLA)

Fungsi: menopang PCB MAIN pada deck atas chassis menggunakan standoff 3D-print.

Langkah:

1. **Ukur PCB MAIN** dengan penggaris: panjang, lebar, posisi 4 lubang mounting M3
2. Buat 4 buah standoff silinder:
   - *Outer diameter*: 7 mm
   - *Inner diameter*: Ø3.2 mm (clearance M3)
   - *Tinggi*: sesuai komponen tertinggi pada PCB bawah + 3 mm clearance (min. 10 mm)
3. Bagian bawah standoff: tambahkan flange Ø12 mm, tebal 2 mm (kontak ke chassis bawah)
4. Material: PLA 3D print
5. Gunakan **Circular Pattern** atau **Copy/Paste** untuk membuat 4 unit sekaligus
6. Pasang sebagai Assembly → PCB MAIN bertumpu di atas 4 standoff

> **Estimasi dimensi:** Jika komponen tertinggi pada PCB (misal: kapasitor tegak) = 15 mm, maka tinggi standoff = 15 + 5 = 20 mm.

### 8.4 Bracket 3 Sensor Jarak HC-SR04 — Akrilik Bending, Sheet Metal Method

Fungsi: memasang 3 sensor ultrasonik HC-SR04 (depan, kiri, kanan) untuk deteksi rintangan.

**Material: Akrilik 3 mm — didesain dengan metode Sheet Metal Fusion 360, kemudian di-bending secara aktual menggunakan heat gun / bending jig**

> **Mengapa akrilik di-bending?** Bracket sensor memerlukan sudut spesifik (0°, 20–25°) dan setidaknya 1 tekukan (bend) agar sensor mengarah dengan presisi. Tekukan material dilakukan dengan memanaskan akrilik di titik tekuk menggunakan heat gun atau bending strip heater, kemudian ditekuk pada jig sederhana pada sudut yang diinginkan. Workspace **Sheet Metal** di Fusion 360 digunakan untuk desain — fitur **Flat Pattern** menghasilkan geometri 2D siap potong untuk laser cutting, dan fitur **Bend** mendokumentasikan titik-titik tekukan.

#### Orientasi Sensor

```
      TAMPAK ATAS ROBOT

             [DEPAN]
               │
          0° (lurus)
               │
  [Kiri] ◄──┌───────┐──► [Kanan]
         °20-25° │ CHASSIS │ ±20-25°
              └───────┘
               │
             [BELAKANG]

  Sensor Depan : 0° dari sumbu maju (lurus ke depan)
  Sensor Kiri  : 20–25° dari sisi kiri ke arah depan
  Sensor Kanan : 20–25° dari sisi kanan ke arah depan
              (Mirror dari sensor kiri)
```

**Penjelasan sudut:**

- **Sensor Depan — 0°**: menghadap tepat lurus ke depan. Membaca rintangan di jalur langsung.
- **Sensor Kanan — 20–25° ke kanan dari arah maju**: sensor tidak terlalu miring sehingga gelombang ultrasonik masih memantul balik dengan akurat. Sudut > 45° menyebabkan pantulan gelombang melenceng dan pembacaan jarak tidak valid.
- **Sensor Kiri — 20–25° ke kiri dari arah maju**: identik dengan sensor kanan (Mirror).

#### Dimensi HC-SR04 (dari Datasheet)

| Parameter                    | Nilai                                             |
| ---------------------------- | ------------------------------------------------- |
| Panjang PCB                  | 45 mm                                             |
| Lebar PCB                    | 20 mm                                             |
| Tinggi (termasuk transduser) | 15 mm                                             |
| Diameter transduser          | Ø16 mm                                           |
| Jarak antar lubang mounting  | 40 mm (center-to-center, sepanjang sumbu panjang) |
| Diameter lubang mounting     | Ø2 mm                                            |
| Tegangan operasi             | 5 V                                               |
| Jarak ukur                   | 2–400 cm                                         |
| Sudut efektif                | < 15°                                            |

#### Desain Sheet Metal di Fusion 360

**Alur kerja Sheet Metal untuk bracket akrilik bending:**

```
Buat Sketch profil flat → Sheet Metal > Flange → Tambah Bend (sudut tekuk)
→ Buat lubang mounting → Sheet Metal > Flat Pattern → Export DXF
→ Laser cut dari DXF → Heat gun bending pada garis tekuk
```

**Langkah detail — Bracket Sensor Depan (0°):**

1. Ganti workspace ke **SHEET METAL** (klik dropdown `SOLID` → pilih `SHEET METAL`)
2. **Sheet Metal > Sheet Metal Rules** → atur:
   - Thickness: **3 mm** (tebal akrilik)
   - Bend Radius: **1 mm** (tekukan akrilik dirumuskan lebih ketat dari logam)
   - K-factor: **0.33** (tipikal untuk akrilik)
   - Unfold Rule: *Linear*
3. **Create > New Component** → nama: `Bracket_Sensor_Depan`
4. **Sheet Metal > Flange** → buat profil L-bracket:
   - *Tab horizontal* (mounting ke chassis): panjang 40 mm × lebar 20 mm
   - *Tab vertikal* (face sensor): tinggi 25 mm × lebar 46 mm
   - Tebal otomatis mengikuti rule: 3 mm
5. Pada tab vertikal, buat **2× lubang Ø2.2 mm** posisi sesuai jarak lubang HC-SR04 (40 mm center-to-center)
6. Pada tab horizontal, buat **2× lubang Ø3.2 mm** untuk baut M3 ke chassis (jarak 30 mm)
7. **Sheet Metal > Flat Pattern** → cek geometri terbuka menjadi flat dengan benar
8. Perhatikan **garis tekukan (bend line)** pada Flat Pattern — inilah garis yang akan dipanaskan saat bending akrilik fisik
9. Klik kanan Flat Pattern → **Export DXF** → simpan `Bracket_Sensor_Depan.dxf`

> **Proses bending akrilik fisik:**
>
> 1. Laser cut sesuai DXF
> 2. Panaskan garis tekuk dengan heat gun (±60–80°C) selama 30–60 detik hingga akrilik melunak
> 3. Tekuk perlahan di atas jig kayu sesuai sudut (90° untuk bracket depan)
> 4. Tahan posisi hingga dingin (2–3 menit)
> 5. Cek sudut dengan protractor

**Langkah — Bracket Sensor Kiri (20–25°):**

1. **Create > New Component** → nama: `Bracket_Sensor_Kiri`
2. Bracket Kiri berbentuk **L-bracket dengan muka sensor miring 20–25°** dari sumbu maju:
   - Buat 1 piece flat dengan tekukan 65–70° (= 90° − 20–25°) pada garis bend
   - *Tab base*: 50 × 20 mm, 2× lubang Ø3.2 mm mounting ke chassis
   - *Flange sensor*: 50 × 25 mm, 2× lubang Ø2.2 mm mounting sensor
   - Sudut Flange terhadap base: **tekukan 90° − (20°∼25°)** = 65°∼70°
3. **Sheet Metal > Flat Pattern** → export DXF: `Bracket_Sensor_Kiri.dxf`
4. **Mirror** komponen `Bracket_Sensor_Kiri` terhadap sumbu YZ → buat `Bracket_Sensor_Kanan`

> **Tips akrilik:** Karena akrilik bersifat *brittle* (getas), hindari sudut tajam tanpa radius. Tambahkan **fillet R=1.5 mm** di setiap sudut dalam lubang untuk mencegah retak. Di laser cutting, sudut dalam secara otomatis mendapat radius kecil dari kerf laser.

#### Tabel Komponen Dudukan Sensor Jarak

| Komponen                          | Qty | Material     | Sudut       | Metode Produksi        |
| --------------------------------- | --- | ------------ | ----------- | ---------------------- |
| Bracket_Sensor_Depan              | 1   | Akrilik 3 mm | 0° (lurus) | Laser cut (1 piece)    |
| Bracket_Sensor_Kiri (base plate)  | 1   | Akrilik 3 mm | —          | Laser cut              |
| Bracket_Sensor_Kiri (face plate)  | 1   | Akrilik 3 mm | 20–25°    | Laser cut              |
| Bracket_Sensor_Kanan (base plate) | 1   | Akrilik 3 mm | —          | Mirror dari Kiri       |
| Bracket_Sensor_Kanan (face plate) | 1   | Akrilik 3 mm | 20–25°    | Mirror dari Kiri       |
| Baut M3 × 8 mm                   | 4   | —           | —          | Sambung base-face      |
| Baut M2 × 6 mm                   | 6   | —           | —          | Ikat sensor ke bracket |

### 8.5 Bracket Servo SG90 (3D Print PLA)

Fungsi: memasang 1 servo SG90 di bagian depan robot sebagai aktuator (penggerak kamera, sweeping sensor, atau pendorong rintangan).

Langkah:

1. **Ukur servo SG90** dengan penggaris:
   - Body: 32.5 × 12 × 30.5 mm
   - Flange: ±38 × 12 mm (termasuk tab)
   - Lubang flange: M2, jarak ±27 mm
   - Poros output: Ø2 mm, tinggi ~3 mm dari permukaan atas
2. Desain dudukan channel/U-shape:
   - Rongga dalam (pocket): 32.5 + 0.3 = 32.8 × 12.3 mm (clearance fit)
   - Kedua sisi: lubang untuk baut flange (M2)
   - Bagian bawah dudukan: 2× lubang M3 ke chassis
   - Tebal dinding bracket: min. 3 mm
3. Tambahkan **slot pemasangan** agar servo mudah dilepas/pasang dari atas
4. Material: PLA 3D print, infill ≥40% (menanggung beban torsi servo)

### 8.6 Bracket Baterai 2×18650 + Model 3D Baterai (3D Print PLA)

Fungsi: menampung 2 sel baterai lithium 18650 secara aman di atas deck chassis.

**Bagian A — Model 3D Baterai (Referensi Assembly):**

1. **Ukur baterai 18650** dengan penggaris: diameter Ø18.5 mm, panjang 65–68 mm
2. **Create > New Component** → nama: `Baterai_18650`
3. Sketch lingkaran Ø aktual → **Extrude** panjang aktual → silinder baterai
4. Tambahkan detail:
   - **Positive cap** (+): tonjolan kecil Ø5 mm, tinggi 1.5 mm di ujung positif
   - **Negative cap** (−): ujung datar
5. **Appearance** → warna biru/hijau (sesuai warna baterai fisik)
6. **Copy** satu baterai → buat 2 unit, posisikan side-by-side

> **Tidak ada file produksi untuk model baterai** — hanya referensi assembly.

**Bagian B — Bracket/Housing Baterai (3D Print):**

1. Desain housing dengan 2 rongga silindris:
   - Sketch 2 lingkaran diameter = *diameter baterai aktual* + 0.3 mm clearance
   - Jarak antar pusat lingkaran: *diameter* + 2 mm (dinding pemisah min. 2 mm)
   - **Extrude**: panjang baterai aktual + 3 mm
2. Buat dinding luar (tebal min. 2 mm)
3. Tambahkan **retaining lip** di satu ujung: bibir tonjokan 2 mm ke dalam
4. Tambahkan **channel kabel** (slot 12 × 5 mm) di sisi samping
5. Bagian bawah: 4× lubang M3 mounting ke chassis atas
6. Material: PLA 3D print, infill ≥40%

> **Catatan keselamatan:** Clearance +0.3–0.5 mm pada diameter cukup untuk pemasangan/pelepasan aman. Jangan terlalu ketat.

---

### 8.7 Aksesoris 3D Bebas (3D Print PLA)

Fungsi: komponen estetika dan fungsionalitas tambahan untuk mempercantik robot. Desain **bebas dan kreatif** — tidak ada template baku.

**Contoh aksesoris yang disarankan:**

| Ide Aksesoris           | Fungsi                      | Keterangan                          |
| ----------------------- | --------------------------- | ----------------------------------- |
| Spoiler / finn belakang | Estetika, aerodinamika      | Model bebas, 3D print               |
| Cover sensor depan      | Pelindung HC-SR04, estetika | Bingkai dekoratif di sekitar sensor |
| Shroud/diffuser roda    | Pelindung roda, estetika    | Cover separuh roda                  |
| Name plate / branding   | Identitas tim               | Teks timbul di body                 |
| Bumper depan            | Pelindung benturan          | Biasanya U-shape fleksibel          |
| LED mount               | Posisi LED hias             | Slot untuk LED strip / WS2812       |
| Antenna dummy           | Estetika                    | Batang kecil di atas robot          |

**Langkah umum:**

1. **Create > New Component** → nama: `Aksesoris_[NamaAksesoris]`
2. Desain bebas menggunakan alat Solid Modeling (Extrude, Loft, Fillet, chamfer)
3. Pertimbangkan:
   - Tidak menghalangi sensor (sudut ≥ 15° dari arah sensor)
   - Tidak menambah berat berlebihan (desain hollow jika perlu)
   - Infill 15–20% untuk hemat filamen
4. Material: PLA, **warna bebas** (gunakan Appearance untuk visualisasi)
5. Export **STL** masing-masing aksesoris

> **Kreativitas dinilai.** Aksesoris harus terpasang (joint) di assembly dengan benar dan tidak menimbulkan interference.

---

### 8.8 Gripper SG90 (3D Print PLA)

Fungsi: menjepit benda berukuran ±5 cm menggunakan **1× servo SG90** yang sudah ada di robot. Gripper menggunakan mekanisme **parallel jaw** — dua rahang bergerak serentak melalui linkage dari servo horn SG90.

| Parameter           | Nilai                                     |
| ------------------- | ----------------------------------------- |
| Servo               | 1× SG90 (sudah ada)                      |
| Lebar bukaan max    | 60 mm (grip benda ~5 cm dengan clearance) |
| Lebar bukaan min    | 10 mm (posisi cengkeram penuh)            |
| Panjang jaw         | 60 mm                                     |
| Tebal jaw           | 3 mm                                      |
| Material semua part | PLA 3D print                              |
| Posisi di robot     | Bagian depan, di atas bracket sensor      |

**Komponen Gripper (4 STL file):**

| Komponen            | Deskripsi                                                  | File                    |
| ------------------- | ---------------------------------------------------------- | ----------------------- |
| `Gripper_Base`    | Mount bracket ke rangka robot (4× M3), tempat servo duduk | `Gripper_Base.stl`    |
| `Gripper_Jaw_L`   | Rahang kiri (L-shape, ujung berfinger-grip)                | `Gripper_Jaw_L.stl`   |
| `Gripper_Jaw_R`   | Rahang kanan (mirror dari kiri)                            | `Gripper_Jaw_R.stl`   |
| `Gripper_Linkage` | Batang penghubung servo horn ke pivot jaw                  | `Gripper_Linkage.stl` |

**Prinsip kerja:**

- SG90 pada posisi **0°** → linkage mendorong kedua jaw terbuka (60 mm)
- SG90 pada posisi **90°** → linkage menarik kedua jaw menutup → menjepit benda
- Pivot jaw: Revolute Joint di Fusion 360
- Servo horn → Linkage → Jaw: mekanisme engkol-slider

**Langkah Desain di Fusion 360:**

1. **Create > New Component** → nama: `Gripper_Base`
2. Sketch profil base:
   - Lebar = lebar depan robot ± 10 mm
   - 4× lubang M3 untuk mounting ke rangka
   - Slot/pocket untuk servo SG90 (32.5 × 12 mm)
3. Extrude base → tebal 5 mm
4. **Create > New Component** → nama: `Gripper_Jaw_L`
5. Sketch jaw kiri (L-shape):
   - Panjang arm horisontal: **60 mm**
   - Ujung jaw: profil finger-grip (lekukan setengah lingkaran R3 untuk memegang benda)
   - Tebal: 4 mm
   - Lubang pivot Ø3.2 mm di bagian pangkal
6. Extrude jaw kiri
7. **Mirror component** → `Gripper_Jaw_R` (mirror terhadap bidang YZ)
8. **Create > New Component** → nama: `Gripper_Linkage`
9. Sketch linkage: batang lurus, panjang ~30 mm, 2× lubang pivot Ø3.2 mm di ujung
10. Extrude linkage → tebal 3 mm
11. **Assembly Joints:**
    - Servo horn ke Gripper_Base: **Rigid Joint**
    - Jaw_L ke Base: **Revolute Joint** (sumbu pivot)
    - Jaw_R ke Base: **Revolute Joint** (sumbu pivot)
    - Linkage ke Servo horn: **Revolute Joint**
    - Linkage ke Jaw_L: **Revolute Joint**
    - (Jaw_R bergerak simetris via mirror constraint atau 2nd linkage)
12. Animasikan joint: servo 0° → 90° → verifikasi gerakan jaw 60 mm → 10 mm
13. **Inspect > Interference** → harus 0 interference pada seluruh rentang gerak
14. Export: `Gripper_Base.stl`, `Gripper_Jaw_L.stl`, `Gripper_Jaw_R.stl`, `Gripper_Linkage.stl`

**Checkpoint ✅:** Gripper 4-part selesai, joint teranimasi, bukaan 10–60 mm, export 4 STL.

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

## 10. DESIGN FOR MANUFACTURING (DFM) — PRODUKSI MANDIRI

> **🔧 Prinsip Utama:** Semua komponen mekanik pada proyek ini **diproduksi sendiri.**

### 10.1 Prinsip DFM

| Prinsip                | Penjelasan                                           |
| ---------------------- | ---------------------------------------------------- |
| Minimum wall thickness | ≥ 1 mm untuk 3D print, ≥ 2 mm untuk akrilik        |
| Min hole diameter      | ≥ 1 mm (3D print), ≥ 2 mm (laser cut)              |
| Toleransi fit          | ± 0.2 mm untuk press fit, ± 0.5 mm untuk clearance |
| Overhang               | ≤ 45° tanpa support (FDM 3D print)                 |
| Fillet                 | Tambahkan fillet pada sudut tajam internal           |
| Kerf (laser)           | Kompensasi ± 0.1 mm untuk laser cutting             |

### 10.2 DFM untuk 3D Printing (FDM)

| Parameter      | Nilai Rekomendasi                       |
| -------------- | --------------------------------------- |
| Layer height   | 0.2 mm (standar)                        |
| Wall thickness | ≥ 1.2 mm (3 perimeter × 0.4 nozzle)   |
| Infill         | 20–30% (chassis), 50%+ (bracket beban) |
| Bridging max   | 30 mm tanpa support                     |
| Hole diameter  | Tambah 0.2 mm dari nominal              |
| Support        | Hindari jika memungkinkan               |

### 10.3 DFM untuk Laser Cutting + Bending Akrilik (termasuk Part Sheet Metal)

| Parameter             | Nilai                              |
| --------------------- | ---------------------------------- |
| Material              | Akrilik 3 mm                       |
| Min detail            | 1 mm                               |
| Min slot              | 3 mm (= tebal material)            |
| Kerf compensation     | 0.1–0.15 mm                       |
| Tab/slot joint        | Slot = tebal material + 0.1 mm     |
| Fillet lubang         | R ≥ 1.5 mm (cegah retak akrilik)  |
| Jarak tepi ke lubang  | ≥ 4 mm (min. 1× tebal material)  |
| Bridge/web min        | 3 mm antar lubang berdekatan       |
| Bend radius (bending) | 1–2 mm untuk akrilik 3 mm         |
| Pemanasan bending     | Heat gun ≥ 60°C pada garis tekuk |

**Prinsip desain bracket akrilik dengan Sheet Metal + Bending:**

| Prinsip                  | Penjelasan                                                                                         |
| ------------------------ | -------------------------------------------------------------------------------------------------- |
| Single-piece + bend      | Desain 1 piece dengan Flange + Bend di Fusion 360; hasil fisik 1 potongan laser cut yang dibending |
| Sheet Metal Flat Pattern | Gunakan untuk export DXF — termasuk bend allowance otomatis                                       |
| Bend line marking        | Tandai garis tekukan pada DXF sebelum laser cut (menggunakan laser engrave / spidol)               |
| K-factor akrilik         | Set K-factor 0.33 di Sheet Metal Rules                                                             |
| Thickness rule           | Set 3 mm di Sheet Metal Rules sebelum membuat Flange                                               |

### 10.4 DFM untuk Sheet Metal Logam (Metal Chassis)

| Parameter            | Nilai                                                       |
| -------------------- | ----------------------------------------------------------- |
| Material             | Pelat baja / aluminium                                      |
| Tebal pelat          | Sesuai chassis aktual (1–2 mm)                             |
| Bend radius min      | 1× tebal material                                          |
| K-factor baja        | 0.42 (default Fusion 360)                                   |
| K-factor aluminium   | 0.44                                                        |
| Min lubang           | 1.5× tebal material                                        |
| Jarak lubang ke edge | ≥ 2× tebal material                                       |
| Relief cut           | Tambahkan relief 0.5 mm di sudut bend (Fusion 360 otomatis) |

> Karena chassis sudah jadi, **tabel ini hanya referensi dokumentasi** bagi praktikan yang ingin memahami standar desain sheet metal logam.

---

## 11. FABRIKASI MANDIRI: 3D PRINTING DAN LASER CUTTING

> Semua komponen mekanik diproduksi **sendiri di lab** menggunakan mesin 3D printer dan mesin laser cutting yang tersedia.

### 11.1 Pembagian Material dan Metode Produksi

| Komponen                             | Material                    | Metode                        | Keterangan                              |
| ------------------------------------ | --------------------------- | ----------------------------- | --------------------------------------- |
| **Chassis Box (single-piece)** | **Metal (aluminium)** | **Sudah jadi**          | Model 3D box chassis referensi assembly |
| Motor DC Gearbox                     | —                          | **Model 3D referensi**  | Tidak diprod, referensi assembly        |
| Roda                                 | —                          | **Model 3D referensi**  | Tidak diprod, referensi assembly        |
| Baterai 18650                        | —                          | **Model 3D referensi**  | Tidak diprod, referensi assembly        |
| Bracket sensor depan                 | Akrilik 3 mm                | **Laser cut + bending** | Sheet Metal → Flat Pattern DXF         |
| Bracket sensor kiri/kanan            | Akrilik 3 mm                | **Laser cut + bending** | Sheet Metal → DXF, dibending           |
| Motor mount (kiri + kanan)           | PLA                         | 3D print                      | STL dari Fusion 360                     |
| Standoff PCB MAIN ×4                | PLA                         | 3D print                      | STL dari Fusion 360                     |
| Cover PCB Sensor Garis               | PLA                         | 3D print                      | STL dari Fusion 360                     |
| Bracket sensor garis (L)             | PLA                         | 3D print                      | STL dari Fusion 360                     |
| Bracket servo SG90                   | PLA                         | 3D print                      | STL dari Fusion 360                     |
| Bracket baterai 2×18650             | PLA                         | 3D print                      | STL dari Fusion 360                     |
| **Gripper_Base**               | PLA                         | 3D print                      | Mount gripper + dudukan servo           |
| **Gripper_Jaw_L + Jaw_R**      | PLA                         | 3D print                      | Rahang kiri + kanan, parallel jaw       |
| **Gripper_Linkage**            | PLA                         | 3D print                      | Penghubung servo horn ke jaw            |
| Aksesoris 3D (bebas)                 | PLA                         | 3D print                      | STL dari Fusion 360                     |

### 11.2 Proses 3D Printing (FDM)

**Perangkat lunak slicer:** Cura / PrusaSlicer

**Alur kerja:**

```
Export STL dari Fusion 360
        ↓
Import ke Cura/PrusaSlicer
        ↓
Atur: Layer height, Infill, Wall, Support
        ↓
Slice → Preview G-code
        ↓
Save ke SD Card / USB
        ↓
Print di 3D Printer Lab
        ↓
Post-process: lepas support, amplas lubang jika terlalu kecil
```

**Parameter slicer yang direkomendasikan:**

| Parameter    | Bracket/Dudukan | Standoff    | Dudukan Baterai/Servo |
| ------------ | --------------- | ----------- | --------------------- |
| Layer height | 0.2 mm          | 0.2 mm      | 0.2 mm                |
| Infill       | 30%             | 40%         | 50%                   |
| Wall lines   | 3               | 3           | 4                     |
| Support      | Hindari         | Tidak perlu | Tergantung orientasi  |
| Material     | PLA             | PLA         | PLA                   |
| Nozzle temp  | 200°C          | 200°C      | 200°C                |
| Bed temp     | 60°C           | 60°C       | 60°C                 |

**Tips 3D Printing:**

- Orientasikan part agar overhang minimal (putar 90° jika perlu)
- Lubang M3 cetak 3.4 mm — setelah print biasanya mengecil, panas baud akan sedikit melebarkan
- Tambahkan **0.2 mm tolerance** pada semua clearance fit
- Infill ≥40% untuk part yang menanggung beban (dudukan servo, baterai)

### 11.3 Proses Laser Cutting (Akrilik)

**Perangkat lunak laser:** LightBurn / LaserGRBL / RDWorks

**Alur kerja:**

```
Export DXF Flat Pattern dari Fusion 360
        ↓
Import DXF ke LightBurn
        ↓
Atur layer: Cut (merah) = potong penuh, Engrave (hitam) = ukiran
        ↓
Atur kerf compensation di LightBurn: 0.1–0.15 mm
        ↓
Set material: Akrilik 3 mm, sesuaikan power/speed mesin lab
        ↓
Test cut di sudut kecil dulu → cek hasilnya bersih atau tidak
        ↓
Potong material utama
        ↓
Post-process: lepas protective film, haluskan tepi dengan amplas 400
```

**Setting laser akrilik 3 mm (estimasi — sesuaikan dengan mesin lab):**

| Parameter | Cut (potong)    | Engrave (ukiran teks) |
| --------- | --------------- | --------------------- |
| Power     | 70–80%         | 20–30%               |
| Speed     | 15–20 mm/s     | 200–300 mm/s         |
| Passes    | 1–2            | 1                     |
| Focus     | Tepat permukaan | Tepat permukaan       |

**Tips Laser Cutting Akrilik:**

- Lepas **protective film** dari akrilik sebelum cutting untuk hasil potongan lebih bersih
- Atau biarkan film saat cutting, lepas setelahnya — mencegah goresan
- **Fillet R≥1.5 mm** di sudut dalam lubang — sudut 90° tajam rawan retak
- Uji coba (test cut) wajib dilakukan sebelum potong material utama
- Simpan sisa akrilik untuk keperluan lain

---

## 12. EXPORT DAN PRODUKSI FILE

### 12.1 Export STL (3D Print — Komponen PLA)

Digunakan untuk: motor mount, standoff PCB, dudukan servo, dudukan baterai.

1. Klik kanan body/component → **Save as STL**
2. Refinement: **High** (untuk part kecil)
3. Format: Binary STL
4. Buka di slicer (Cura, PrusaSlicer) untuk generate G-code

### 12.2 Export DXF dari Sheet Metal Flat Pattern (Bracket Akrilik)

> Ini adalah metode yang **benar dan akurat** untuk mengekspor part sheet metal untuk laser cutting. **Jangan gunakan Export biasa** karena menghasilkan geometri 3D, bukan flat pattern.

**Langkah Export Sheet Metal Flat Pattern:**

1. Pastikan Anda berada di workspace **SHEET METAL**
2. Di **Browser** (panel kiri), expand component bracket sensor
3. Klik kanan **Flat Pattern** → pilih **Activate**
   - Tampilan berubah menjadi flat/datar otomatis
4. Klik kanan **Flat Pattern** lagi → **Export to DXF**
5. Pilih folder simpan, beri nama file (contoh: `Bracket_Sensor_Depan.dxf`)
6. Klik **OK**

> **Alternatif:** Dari menu **Sheet Metal > Flat Pattern > Create Flat Pattern** jika belum ada, kemudian export.

**Daftar file DXF yang harus dihasilkan dari Sheet Metal:**

| File                         | Part                                     | Qty |
| ---------------------------- | ---------------------------------------- | --- |
| `Bracket_Sensor_Depan.dxf` | Bracket depan (L-shape single piece)     | 1   |
| `Sensor_Kiri_Base.dxf`     | Base plate bracket kiri                  | 1   |
| `Sensor_Kiri_Face.dxf`     | Face plate bracket kiri (sudut 20–25°) | 1   |
| `Sensor_Kanan_Base.dxf`    | Mirror dari kiri                         | 1   |
| `Sensor_Kanan_Face.dxf`    | Mirror dari kiri                         | 1   |
| `Chassis_Bottom.dxf`       | Chassis bawah                            | 1   |
| `Chassis_Top.dxf`          | Chassis atas                             | 1   |

### 12.3 Export DXF dari Solid Body (Chassis Akrilik)

Untuk chassis yang didesain dengan Solid Modeling (bukan Sheet Metal):

1. Klik face datar chassis → **Create Sketch From Faces**
2. Sketch akan terbentuk pada face tersebut
3. **File > Export** → format DXF
4. Atau: buat **Drawing** workspace → tambah view → export DXF

### 12.4 Export STEP (Universal)

1. **File > Export** → format STEP (.stp)
2. Berguna untuk transfer antar-CAD software
3. STEP mempertahankan geometri parametrik dasar

---


## 13. REFERENSI

1. Autodesk. (2024). *Fusion 360 Official Documentation*. https://help.autodesk.com/view/fusion360/ENU/
2. Autodesk Learning. (2024). *Design and Design to Manufacturing with Fusion 360*. https://learn.autodesk.com/paths/design-and-design-to-manufacturing-with-fusion-360
3. Autodesk. (2024). *Design for Manufacturing (DFM) in Fusion 360*. https://www.autodesk.com/products/fusion-360/resources
4. Autodesk Fusion. (2024). *PCB Design to Manufacturing*. https://learn.autodesk.com/paths/design-pcbs-with-fusion-360
5. Chua, C. K., Leong, K. F., & Lim, C. S. (2010). *Rapid Prototyping: Principles and Applications*. World Scientific.
6. Groover, M. P. (2016). *Fundamentals of Modern Manufacturing*. Wiley.

---

Dokumen ini menjadi acuan utama materi Modul 03 Fusion 360 untuk desain mekanik robot line follower.
