# JOBSHEET MODUL 03: FUSION 360 – DESAIN MEKANIK PROJECT LINE FOLLOWER & ORDER PCB

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 03 – Fusion 360 Desain Mekanik  
**Software:** Autodesk Fusion 360 (Education License)  
**Pertemuan:** 5–6 (2 × 2 SKS)  
**Tanggal:** ___________________  
**Nama Kelompok:** ___________________  
**Anggota:**

| No | Nama | NIM |
|----|------|-----|
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:

1. Mengoperasikan Autodesk Fusion 360 untuk pemodelan 3D parametrik
2. Membuat sketch 2D dengan constraints dan dimensi yang tepat
3. Menggunakan operasi solid modeling (extrude, fillet, hole, pattern)
4. Mendesain chassis robot line follower dengan dimensi yang sesuai
5. Mendesain bracket motor, sensor, dan servo
6. Melakukan assembly dan joint antar komponen
7. Mengimport model PCB dari EasyEDA ke Fusion 360
8. Mengeksport file STL (3D print) dan DXF (laser cut)
9. Memverifikasi Gerber file dan melakukan order PCB ke JLCPCB

---

## B. ALAT DAN BAHAN

### Perangkat Lunak

| No | Software | Fungsi |
|----|---------|--------|
| 1 | **Autodesk Fusion 360** | Desain 3D mekanik dan assembly |
| 2 | **EasyEDA** | Export file PCB (STEP/DXF) |
| 3 | **JLCPCB Gerber Viewer** | Validasi Gerber sebelum order |
| 4 | **Slicer (Cura/PrusaSlicer)** | Preview STL untuk 3D print |

### Perangkat Keras

| No | Alat | Keterangan |
|----|------|-----------|
| 1 | Laptop/PC | Minimal RAM 8 GB, GPU dedicated disarankan |
| 2 | Penggaris/Jangka Sorong | Pengukuran komponen fisik |
| 3 | Motor DC + Roda | Referensi dimensi mounting |
| 4 | Servo SG90 | Referensi dimensi bracket |
| 5 | Sensor HC-SR04 | Referensi dimensi bracket |

---

## C. DASAR TEORI SINGKAT

### C.1 Alur Desain Mekanik Robot

```
Ukur Komponen Fisik → Buat Sketch 2D → Solid Modeling 3D
→ Assembly & Joint → Interference Check → Export (STL/DXF)
→ Fabrikasi (3D Print / Laser Cut)
```

### C.2 Panduan Dimensi Cepat

| Komponen | Dimensi Kunci |
|---------|--------------|
| Motor DC TT | 70 × 22 × 18 mm, poros 5 mm |
| SG90 Servo | 32.5 × 12 × 32.5 mm (dengan flange) |
| HC-SR04 | 45 × 20 × 15 mm |
| PCB MAIN | 120 × 80 × 1.6 mm |
| PCB Sensor | 130 × 30 × 1.6 mm |
| Roda | Ø65 × 25 mm |
| Baut M3 | Ø3.2 mm (clearance hole) |

---

## D. LANGKAH KERJA

---

### PERCOBAAN 1: PENGENALAN ANTARMUKA DAN SKETCH DASAR

**Estimasi waktu: 30 menit**

1. Buka Fusion 360, login dengan akun Education
2. **File > New Design** → simpan sebagai `Robot_LineFollower_[NamaKelompok]`
3. Familiarisasi antarmuka:
   - Toolbar (SOLID, SURFACE, SHEET METAL)
   - Data Panel (kiri)
   - Timeline (bawah)
   - ViewCube (kanan atas)
4. **Create > New Component** → nama: `Latihan_Sketch`
5. **Create Sketch** → pilih bidang XY
6. Latihan menggambar:
   - Persegi panjang 100 × 60 mm menggunakan `R` (Rectangle)
   - Lingkaran Ø20 mm di tengah menggunakan `C` (Circle)
   - Fillet pada keempat sudut R=5 mm
   - Tambahkan dimensi (`D`) pada semua elemen
7. Pastikan sketch berwarna **hitam** (fully constrained)
8. **Finish Sketch**
9. **Extrude** (`E`) sketch tersebut: tinggi 5 mm

**Checkpoint ✅:** Body 3D berhasil dibuat dari sketch.

---

### PERCOBAAN 2: DESAIN CHASSIS BAWAH

**Estimasi waktu: 45 menit**

1. **Create > New Component** → nama: `Chassis_Bottom`
2. **Create Sketch** pada bidang XY
3. Gambar outline: Rectangle 150 × 120 mm
4. Fillet keempat sudut: R=10 mm
5. Tambahkan lubang-lubang:
   - 4× lubang mounting PCB MAIN (Ø3.2 mm) → posisi sesuai desain PCB Modul 01
   - 2× slot motor mount (sesuai dimensi motor DC TT)
   - 2× lubang caster wheel di belakang (Ø3.2 mm, jarak 30 mm)
   - 2× lubang routing kabel (oval 10 × 5 mm)
6. **Finish Sketch** → pastikan fully constrained
7. **Extrude**: 3 mm (simulasi tebal akrilik)
8. **Fillet** edge atas: 0.5 mm

| Fitur | Dimensi | Jumlah |
|-------|---------|--------|
| Outline | 150 × 120 mm, R=10 | 1 |
| Lubang PCB mount | Ø3.2 mm | 4 |
| Slot motor | sesuai motor TT | 2 |
| Lubang caster | Ø3.2 mm | 2 |
| Lubang kabel | 10 × 5 mm oval | 2 |

**Checkpoint ✅:** Chassis bawah selesai dengan semua lubang.

---

### PERCOBAAN 3: DESAIN CHASSIS ATAS DAN SPACER

**Estimasi waktu: 30 menit**

1. **Create > New Component** → nama: `Chassis_Top`
2. **Create Sketch** pada bidang XY, offset Z = 25 mm (tinggi spacer)
3. Gambar outline sama dengan chassis bawah (150 × 120 mm, R=10)
4. Lubang-lubang:
   - 4× mounting spacer (Ø3.2 mm, posisi sudut)
   - 1× slot OLED display (27 × 15 mm)
   - 1× lubang switch on/off (Ø6.5 mm)
   - 2× lubang routing kabel
5. **Extrude**: 3 mm
6. **Create > New Component** → nama: `Spacer`
7. Sketch lingkaran: outer Ø7 mm, inner Ø3.2 mm
8. **Extrude**: 25 mm
9. **Circular Pattern** atau copy spacer ke 4 posisi sudut

**Checkpoint ✅:** Chassis atas dan spacer selesai.

---

### PERCOBAAN 4: DESAIN MOTOR MOUNT DAN SENSOR BRACKET

**Estimasi waktu: 45 menit**

#### A. Motor Mount

1. **Create > New Component** → nama: `Motor_Mount_L`
2. Ukur motor DC TT yang akan digunakan
3. Desain bracket L-shape:
   - Bagian horizontal: 30 × 25 mm, lubang M3 × 2
   - Bagian vertikal: 25 × 22 mm, lubang clamp motor
4. **Extrude**: 4 mm (PLA 3D print)
5. Fillet internal: 2 mm
6. **Mirror** → buat `Motor_Mount_R`

#### B. Sensor Board Bracket

1. **Create > New Component** → nama: `Sensor_Bracket`
2. Desain bracket L-shape:
   - Sisi horizontal: 20 × 15 mm, 2× lubang M3
   - Sisi vertikal: 15 × 10 mm, 2× lubang M3 (ke sensor board)
   - Ketinggian sensor dari lantai: 5–8 mm
3. **Extrude**: 3 mm
4. Buat 2 buah (kiri dan kanan)

#### C. Servo Bracket

1. **Create > New Component** → nama: `Servo_Bracket`
2. Buat pocket sesuai body SG90 (32.5 × 12 mm)
3. Tab mounting: 2× lubang M2
4. **Extrude**: 4 mm

**Checkpoint ✅:** Semua bracket dan mount selesai.

---

### PERCOBAAN 5: ASSEMBLY DAN INTERFERENCE CHECK

**Estimasi waktu: 30 menit**

1. Pastikan semua bagian sudah menjadi **Component**
2. **Assemble > Joint** (`J`):
   - Chassis_Bottom → Ground (fixed)
   - Spacer × 4 → Rigid joint ke chassis bawah
   - Chassis_Top → Rigid joint ke spacer atas
   - Motor_Mount × 2 → Rigid joint di slot motor chassis bawah
   - Sensor_Bracket × 2 → Rigid joint di depan chassis bawah
3. **Inspect > Interference** → jalankan untuk seluruh assembly
4. Jika ada tabrakan:

| Komponen 1 | Komponen 2 | Masalah | Solusi |
|-----------|-----------|---------|--------|
|           |           |         |        |

5. Perbaiki hingga **0 interference**
6. **Inspect > Section Analysis** → cek clearance internal

**Checkpoint ✅:** Assembly lengkap tanpa interference.

---

### PERCOBAAN 6: IMPORT PCB DAN VERIFIKASI FITMENT

**Estimasi waktu: 20 menit**

1. Export PCB dari EasyEDA:
   - **File > Export > STEP 3D** (untuk model 3D) atau
   - **File > Export > DXF** (untuk outline saja)
2. Di Fusion 360: **File > Open > Upload** → pilih file STEP/DXF
3. Jika DXF:
   - Insert ke sketch pada chassis bawah
   - Extrude 1.6 mm (tebal PCB)
4. Konversi ke Component → rename `PCB_Main`
5. Joint PCB ke chassis menggunakan lubang mounting
6. Cek alignment:
   - [ ] Lubang PCB sejajar dengan lubang chassis
   - [ ] Komponen tertinggi pada PCB tidak menabrak chassis atas
   - [ ] Konektor tepi mudah diakses

**Checkpoint ✅:** PCB terintegrasi dalam assembly, fitment OK.

---

### PERCOBAAN 7: EXPORT FILE PRODUKSI DAN ORDER PCB

**Estimasi waktu: 30 menit**

#### A. Export STL

1. Klik kanan setiap component yang akan di-3D print → **Save as STL**:
   - `Motor_Mount_L.stl`
   - `Motor_Mount_R.stl`
   - `Sensor_Bracket.stl` × 2
   - `Servo_Bracket.stl`
   - `Spacer.stl` × 4
2. Setting: Refinement = High, Format = Binary
3. Buka file STL di slicer (Cura), pastikan printable

#### B. Export DXF

1. Untuk chassis akrilik (laser cut):
   - Buat **Drawing** dari Fusion 360
   - Atau klik face atas chassis → **Create Sketch From Faces**
   - Export sketch sebagai DXF
2. File DXF:
   - `Chassis_Bottom.dxf`
   - `Chassis_Top.dxf`

#### C. Order PCB ke JLCPCB

1. Buka EasyEDA → project PCB Modul 01
2. **Fabrication > PCB Fabrication File (Gerber)** → download ZIP
3. Upload ke https://gerber-viewer.jlcpcb.com → verifikasi semua layer
4. **Fabrication > Order at JLCPCB** → review parameter:
   - Qty: 5 pcs, Layer: 2, Thickness: 1.6 mm
   - Color: Green, Surface: HASL Lead-free
   - Copper: 1 oz
5. Submit order dan catat nomor order

| Deliverable | Format | Nama File |
|------------|--------|-----------|
| Motor Mount L | STL | Motor_Mount_L.stl |
| Motor Mount R | STL | Motor_Mount_R.stl |
| Sensor Bracket | STL | Sensor_Bracket.stl |
| Servo Bracket | STL | Servo_Bracket.stl |
| Spacer | STL | Spacer.stl |
| Chassis Bottom | DXF | Chassis_Bottom.dxf |
| Chassis Top | DXF | Chassis_Top.dxf |
| Gerber PCB MAIN | ZIP | Gerber_PCB_Main.zip |
| Gerber PCB Sensor | ZIP | Gerber_PCB_Sensor.zip |

**Checkpoint ✅:** Semua file produksi diekspor dan PCB diorder ke JLCPCB.

---

## E. ANALISA PERCOBAAN

### Analisa Percobaan 1 – Pengenalan Antarmuka dan Sketch

1. Jelaskan perbedaan sketch yang **fully constrained** (hitam) vs **under-constrained** (biru). Apa dampaknya terhadap desain parametrik?
2. Mengapa penting menggunakan **dimensi** dan **constraints** daripada menggambar freehand?

### Analisa Percobaan 2 – Chassis Bawah

1. Mengapa sudut chassis diberi fillet R=10 mm? Apa manfaatnya secara struktural dan manufaktur?
2. Bagaimana Anda menentukan posisi lubang mounting PCB? Jelaskan hubungannya dengan desain PCB di Modul 01.
3. Jika chassis diperbesar 10 mm ke setiap sisi, parameter apa saja yang harus diubah?

### Analisa Percobaan 3 – Chassis Atas dan Spacer

1. Berapa tinggi spacer yang optimal? Pertimbangkan komponen tertinggi pada PCB dan clearance kabel.
2. Mengapa spacer dibuat sebagai component terpisah, bukan bagian dari chassis?

### Analisa Percobaan 4 – Motor Mount dan Bracket

1. Mengapa motor mount menggunakan tebal 4 mm sedangkan bracket sensor 3 mm? Hubungkan dengan beban yang ditanggung.
2. Berapa ketinggian ideal sensor line dari lantai? Apa yang terjadi jika terlalu tinggi atau rendah?
3. Jelaskan pertimbangan DFM (Design for Manufacturing) pada desain bracket 3D print.

### Analisa Percobaan 5 – Assembly dan Interference

1. Jika ditemukan interference, jelaskan metode yang Anda gunakan untuk memperbaikinya.
2. Mengapa analisis section penting sebelum fabrikasi?
3. Apa perbedaan Rigid Joint dan Revolute Joint? Kapan masing-masing digunakan?

### Analisa Percobaan 6 – Import PCB

1. Apa keuntungan mengimport model PCB 3D dibanding hanya outline DXF?
2. Jika lubang mounting PCB tidak sejajar dengan chassis, langkah apa yang harus dilakukan?

### Analisa Percobaan 7 – Export dan Order PCB

1. Apa perbedaan format STL dan STEP? Kapan masing-masing digunakan?
2. Jelaskan parameter JLCPCB yang Anda pilih dan alasannya.
3. Mengapa Gerber harus diverifikasi di viewer sebelum order?

---

## F. KESIMPULAN

Tuliskan minimal 5 poin kesimpulan hasil praktikum:

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## G. LAMPIRAN WAJIB

- [ ] Screenshot Fusion 360 semua component
- [ ] Screenshot assembly lengkap (isometric + top view)
- [ ] Screenshot interference check (0 interference)
- [ ] Screenshot section analysis
- [ ] Screenshot Gerber verifikasi di JLCPCB viewer
- [ ] Screenshot konfirmasi order JLCPCB
- [ ] File STL (semua bracket dan spacer)
- [ ] File DXF (chassis atas dan bawah)

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
