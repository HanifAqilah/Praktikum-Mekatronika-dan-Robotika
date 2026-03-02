![GEARBOX - Motor DC Gearbox 3V - 6V Kuning Smart Car Gear | Lazada Indonesia]()

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
| -- | ---- | --- |
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:

1. Mengoperasikan Autodesk Fusion 360 untuk pemodelan 3D parametrik
2. Membuat sketch 2D dengan constraints dan dimensi yang tepat
3. Menggunakan operasi solid modeling (extrude, fillet, hole, pattern, mirror)
4. Mengukur komponen fisik dengan penggaris ukur dan mengaplikasikan hasil ukur ke desain
5. Memodelkan **chassis box metal single-piece** yang sudah jadi menggunakan workspace **Sheet Metal** di Fusion 360 sebagai referensi assembly
6. **Memodelkan Motor DC Gearbox dan Roda secara 3D** sebagai komponen referensi dalam assembly
7. Mendesain dudukan PCB MAIN berbasis 3D print (standoff)
8. Mendesain **cover/enclosure 3D print** untuk PCB Sensor Garis
9. Mendesain dudukan 3 sensor jarak HC-SR04 dengan metode **Sheet Metal** berbahan **akrilik yang di-bending**, orientasi: depan lurus (0°), kiri/kanan 20–25° dari arah maju
10. Mendesain dudukan 1 servo SG90
11. Mendesain dudukan baterai 2×18650 dan **memodelkan baterai 18650 secara 3D** sebagai referensi
12. **Mendesain Gripper 3D print** menggunakan 1× servo SG90 — parallel jaw gripper, bukaan ±60 mm, grip benda ~5 cm
13. **Mendesain aksesoris 3D bebas** untuk mempercantik robot (spoiler, dekorasi sensor, dll)
14. Melakukan assembly lengkap semua komponen dan joint
15. Mengimport PCB MAIN dan PCB Sensor dari EasyEDA ke Fusion 360 dan verifikasi fitment
16. Mengeksport file STL (3D print) dan DXF (laser cut + bending)
17. Memverifikasi Gerber file (PCB MAIN dan PCB Sensor) dan melakukan order ke JLCPCB

---

## B. ALAT DAN BAHAN

### Perangkat Lunak

| No | Software                            | Fungsi                         |
| -- | ----------------------------------- | ------------------------------ |
| 1  | **Autodesk Fusion 360**       | Desain 3D mekanik dan assembly |
| 2  | **EasyEDA**                   | Export file PCB (STEP/DXF)     |
| 3  | **JLCPCB Gerber Viewer**      | Validasi Gerber sebelum order  |
| 4  | **Slicer (Cura/PrusaSlicer)** | Preview STL untuk 3D print     |

### Perangkat Keras

| No | Alat                           | Keterangan                                                  |
| -- | ------------------------------ | ----------------------------------------------------------- |
| 1  | Laptop/PC                      | Minimal RAM 8 GB, GPU dedicated disarankan                  |
| 2  | **Penggaris Ukur 30 cm** | **Wajib dibawa** — pengukuran dimensi komponen fisik |
| 3  | Jangka Sorong (opsional)       | Pengukuran presisi diameter baterai, poros servo            |
| 4  | Motor DC + Roda                | Referensi dimensi motor mount                               |
| 5  | Servo SG90 × 1                | Referensi dimensi dudukan servo                             |
| 6  | Sensor HC-SR04 × 3            | Referensi dimensi dudukan sensor jarak                      |
| 7  | Baterai 18650 × 2             | Referensi dimensi dudukan baterai                           |
| 8  | PCB MAIN + PCB Sensor          | Referensi posisi lubang mounting PCB                        |

---

## C. DASAR TEORI SINGKAT

### C.1 Alur Desain Mekanik Robot

```
Ukur Komponen Fisik → Buat Sketch 2D → Solid Modeling 3D
→ Assembly & Joint → Interference Check → Export (STL/DXF)
→ Fabrikasi (3D Print / Laser Cut)
```

### C.2 Panduan Dimensi Cepat

> **⚠️ WAJIB UKUR ULANG** — Dimensi di bawah adalah estimasi awal. Gunakan **penggaris ukur** untuk mengukur dimensi aktual komponen fisik Anda sebelum memulai desain.

| Komponen            | Dimensi Estimasi                | Catatan Pengukuran                       |
| ------------------- | ------------------------------- | ---------------------------------------- |
| Motor DC TT         | 70 × 22 × 18 mm, poros Ø5 mm | Ukur panjang, lebar, diameter poros      |
| Roda                | Ø65 × 25 mm                   | Verifikasi dengan roda aktual            |
| Chassis             | 150 × 120 mm                   | Verifikasi dengan desain awal            |
| Servo SG90 × 1     | 32.5 × 12 × 30.5 mm           | Ukur body + flange + posisi baut flange  |
| Sensor HC-SR04 × 3 | 45 × 20 × 15 mm               | Ukur PCB + tinggi transduser             |
| Baterai 18650 × 2  | Ø18.5 × 65 mm                 | Ukur diameter dan panjang aktual         |
| PCB MAIN            | 100 × 80 × 1.6 mm             | Ukur + catat posisi 4 lubang mounting M3 |
| PCB Sensor Garis    | 130 × 30 × 1.6 mm             | Ukur + catat posisi lubang mounting      |
| Baut M3             | Ø3.2 mm (clearance hole)       | Standar, tidak perlu diukur ulang        |
| Baut M2             | Ø2.2 mm (clearance hole)       | Untuk flange servo                       |

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

### PERCOBAAN 2: DESAIN MODEL BOX CHASSIS (SHEET METAL — REFERENSI ASSEMBLY)

**Estimasi waktu: 45 menit**

> **Catatan:** Chassis robot menggunakan **bodi box metal single-piece (aluminium alloy)**. Ini adalah satu komponen terintegrasi — bawah, samping, dan atas merupakan satu lipatan pelat. **Ukur chassis fisik terlebih dahulu** dengan penggaris, catat dimensi kotak (panjang, lebar, tinggi dinding), tebal pelat, dan posisi semua lubang aktual. Model ini **hanya referensi assembly** — chassis tidak perlu diproduksi.

1. Ganti workspace: klik dropdown `SOLID` → pilih **`SHEET METAL`**
2. **Sheet Metal > Sheet Metal Rules** → atur:
   - Thickness = *tebal pelat aktual* (contoh: 1.5 mm)
   - Material: Aluminium Alloy
   - Bend Radius: 1× tebal pelat
3. **Create > New Component** → nama: `Chassis_Box`
4. **Create Sketch** pada bidang XY — gambar profil **alas kotak** (bottom plate):
   - Outline panjang × lebar sesuai aktual
   - Slot motor pada sisi kiri dan kanan (posisi poros motor)
   - Lubang-lubang cutout sesuai fisik (kabel, akses)
5. **Sheet Metal > Flange** (Base Flange) → jadikan plat dasar
6. Tambahkan **Flange** ke-4 sisi sebagai dinding samping:
   - **Sheet Metal > Flange** → edge kiri + kanan → tinggi dinding = tinggi kotak aktual
   - **Sheet Metal > Flange** → edge depan + belakang
7. Pada **dinding kiri + kanan**: sketch circle Ø poros motor + clearance → **Cut** (lubang poros)
8. Pada **dinding depan**: tambahkan lubang untuk bracket sensor dan mounting gripper
9. **Sheet Metal > Flat Pattern** → verifikasi single flat pattern (semua bend line terlihat)

| Fitur                  | Dimensi (ukur aktual)         | Jumlah |
| ---------------------- | ----------------------------- | ------ |
| Panjang bottom plate   | \_\_\_ mm                   | 1      |
| Lebar bottom plate     | \_\_\_ mm                   | 1      |
| Tinggi dinding samping | \_\_\_ mm                   | 4 sisi |
| Tebal pelat            | \_\_\_ mm                   | 1      |
| Lubang poros motor     | Ø\_\_\_ mm                  | 2      |
| Lubang mounting PCB    | Ø3.2 mm                      | 4      |
| Lubang caster (M3)     | Ø3.2 mm                      | 2      |

**Checkpoint ✅:** Model 3D box chassis single-piece selesai, Flat Pattern terverifikasi, semua lubang aktual terdokumentasi.

---

### PERCOBAAN 3: DESAIN GRIPPER SG90 (3D PRINT PLA)

**Estimasi waktu: 45 menit**

> **Deskripsi:** Gripper menggunakan mekanisme **parallel jaw** — 2 rahang bergerak simetris melalui linkage yang dihubungkan ke servo horn SG90. Tulang/jaw terbuat dari PLA 3D print. Total ada **4 komponen** (1 base + 2 jaw + 1 linkage).

**A. Gripper_Base (Mount + Dudukan Servo):**

1. **Create > New Component** → nama: `Gripper_Base`
2. Sketch profil base:
   - Lebar = lebar depan robot ± 10 mm
   - Tinggi: 30 mm
   - Pocket/slot untuk servo SG90 body: 32.5 × 12 mm
   - 4× lubang M3 untuk mounting ke dinding depan chassis
3. **Extrude** base → tebal 5 mm
4. Tambahkan **lip** penahan servo di kedua sisi slot

**B. Gripper_Jaw_L dan Jaw_R (Rahang):**

5. **Create > New Component** → nama: `Gripper_Jaw_L`
6. Sketch jaw kiri (L-shape):
   - Panjang arm horisontal: **60 mm**
   - Ujung jaw: profil finger-grip (lekukan R3 mm untuk menahan benda)
   - Tebal: 4 mm
   - Lubang pivot Ø3.2 mm di pangkal
7. **Extrude** jaw kiri → tebal 4 mm
8. **Mirror Component** → sumbu tengah → `Gripper_Jaw_R`

**C. Gripper_Linkage (Penghubung Servo Horn – Jaw):**

9. **Create > New Component** → nama: `Gripper_Linkage`
10. Sketch batang lurus: panjang 28–32 mm, tebal 3 mm
11. 2× lubang pivot Ø3.2 mm di masing-masing ujung
12. **Extrude** → tebal 3 mm

**D. Assembly & Joint Gripper:**

13. Gripper_Base → **Rigid Joint** ke dinding depan Chassis_Box
14. Gripper_Jaw_L/R → **Revolute Joint** ke pivot di base
15. Gripper_Linkage → Revolute ke servo horn + Revolute ke Jaw_L
16. Jaw_R: tambahkan Linkage ke-2 (mirror dari Jaw_L) atau gunakan As-Built Joint
17. Animasikan joint servo SG90: **0° → jaw terbuka 60 mm**, **90° → jaw menutup**
18. **Inspect > Interference** → 0 interference pada seluruh rentang gerak
19. Export STL: `Gripper_Base.stl`, `Gripper_Jaw_L.stl`, `Gripper_Jaw_R.stl`, `Gripper_Linkage.stl`

| Komponen | Ukuran Kunci | Material | File |
|----------|-------------|----------|------|
| Gripper_Base | Lebar robot × 30 mm | PLA | Gripper_Base.stl |
| Gripper_Jaw_L | 60 mm arm, R3 finger | PLA | Gripper_Jaw_L.stl |
| Gripper_Jaw_R | Mirror Jaw_L | PLA | Gripper_Jaw_R.stl |
| Gripper_Linkage | 30 mm, 2× pivot Ø3.2 | PLA | Gripper_Linkage.stl |

**Checkpoint ✅:** Gripper 4-part selesai, joint gripper teranimasi (0–60 mm), 4 STL terekspor.

---

### PERCOBAAN 4: DESAIN DUDUKAN KOMPONEN (MOUNT & BRACKET)

**Estimasi waktu: 90 menit**

> **Sebelum mulai:** Ukur semua komponen fisik berikut dengan **penggaris ukur** dan catat hasilnya pada tabel di bawah.

**Tabel Pengukuran Komponen Fisik:**

| Komponen         | Panjang (mm) | Lebar (mm) | Tinggi (mm) | Catatan                |
| ---------------- | ------------ | ---------- | ----------- | ---------------------- |
| Servo SG90       |              |            |             | Ukur flange terpisah   |
| Sensor HC-SR04   |              |            |             | Ukur tinggi transduser |
| Baterai 18650 #1 | Ø______     |            |             | Ukur diameter          |
| Baterai 18650 #2 | Ø______     |            |             | Ukur diameter          |
| PCB MAIN         |              |            | 1.6         | Catat posisi 4 lubang  |
| PCB Sensor       |              |            | 1.6         | Catat posisi lubang    |

---

#### A. Dudukan PCB MAIN (3D Print Standoff)

1. **Create > New Component** → nama: `Standoff_PCB`
2. Buat sketch lingkaran konsentris: outer Ø7 mm, inner Ø3.2 mm
3. **Extrude**: tinggi = *komponen tertinggi pada PCB* + 5 mm clearance (min. 15 mm)
4. Tambahkan flange bawah: Ø12 mm, tebal 2 mm (**Extrude Join** ke lingkaran outer)
5. **Copy** komponen menjadi 4 unit (sudut PCB MAIN)
6. Posisikan sesuai jarak lubang mounting PCB MAIN aktual yang sudah diukur

| Parameter       | Nilai                          |
| --------------- | ------------------------------ |
| Diameter luar   | 7 mm                           |
| Diameter dalam  | 3.2 mm                         |
| Tinggi standoff | ___ mm (isi sesuai pengukuran) |
| Diameter flange | 12 mm                          |
| Jumlah          | 4 pcs                          |

**Checkpoint ✅:** 4 standoff PCB MAIN selesai, posisi sesuai lubang PCB.

---

#### B. Dudukan 3 Sensor Jarak HC-SR04 — Akrilik Bending + Sheet Metal Method

> **Material: Akrilik 3 mm. Metode: Sheet Metal Workspace di Fusion 360, akrilik di-bending secara aktual menggunakan heat gun.**

**Orientasi sensor (tampak atas):**

```
           [DEPAN]
              │
         0° (lurus)
              │
 [Kiri]◄──┌───────┐──►[Kanan]
      ±20-25° │CHASSIS│ ±20-25°
             └───────┘
   (diukur dari arah maju ke sisi kiri/kanan)
```

**Alasan sudut 20–25°:** Sudut ini menjamin gelombang ultrasonik tetap memantul balik ke penerima dengan akurat. Sudut > 45° menyebabkan pantulan melenceng dan pembacaan tidak valid.

**Langkah — Semua bracket menggunakan Sheet Metal + bending:**

1. Ganti workspace: klik dropdown `SOLID` → pilih **`SHEET METAL`**
2. **Sheet Metal > Sheet Metal Rules** → set Thickness: **3 mm**, Bend Radius: **1 mm**, K-factor: **0.33**
3. **Create > New Component** → nama: `Bracket_Sensor_Depan`
4. **Sheet Metal > Flange** → buat L-bracket:
   - Tab horizontal (ke chassis): 40 × 20 mm, **2× Ø3.2 mm** lubang M3
   - Tab vertikal (face sensor): 46 × 25 mm, **2× Ø2.2 mm** lubang M2 (jarak 40 mm center)
5. **Sheet Metal > Flat Pattern** → verifikasi geometri flat termasuk **bend line** pada garis tekuk
6. Klik kanan **Flat Pattern** → **Export DXF** → simpan `Bracket_Sensor_Depan.dxf`
7. Tandai **garis tekukan** pada DXF (dengan layer berbeda / engrave)

**Bracket Sensor Kiri (20–25°):**

8. **Create > New Component** → nama: `Bracket_Sensor_Kiri`
9. Desain **1 piece** dengan tekukan 65–70° (= 90° − 20–25°):
   - *Base*: 50 × 20 mm, 2× Ø3.2 mm (mounting ke chassis)
   - *Face*: 50 × 25 mm, 2× Ø2.2 mm (sensor)
   - Sudut tekukan: **65–70°** (Sheet Metal > Flange dengan angle)
10. **Flat Pattern** → Export DXF: `Bracket_Sensor_Kiri.dxf`
11. **Mirror** `Bracket_Sensor_Kiri` pada sumbu YZ → buat `Bracket_Sensor_Kanan`

> **💡 Proses bending fisik akrilik:**
> 1. Laser cut dari DXF
> 2. Panaskan garis tekuk dengan heat gun (±60–80°C) selama 30–60 detik hingga akrilik melunak
> 3. Tekuk perlahan di atas jig kayu sesuai sudut target
> 4. Tahan posisi hingga dingin (2–3 menit), cek dengan protractor

> **💡 Tips akrilik:** Tambahkan **fillet R=1.5 mm** di sudut dalam lubang agar akrilik tidak retak.

| Komponen             | Qty    | Material     | Sudut     | File                         |
| -------------------- | ------ | ------------ | --------- | ---------------------------- |
| Bracket_Sensor_Depan | 1 buah | Akrilik 3 mm | 0° lurus  | `Bracket_Sensor_Depan.dxf`  |
| Bracket_Sensor_Kiri  | 1 buah | Akrilik 3 mm | 20–25°   | `Bracket_Sensor_Kiri.dxf`   |
| Bracket_Sensor_Kanan | 1 buah | Akrilik 3 mm | 20–25°   | Mirror kiri                  |

**Checkpoint ✅:** 3 bracket sensor selesai (1 piece + bending each), DXF siap laser cut.

---

#### C. Dudukan Servo SG90 (1 Unit)

1. **Create > New Component** → nama: `Dudukan_Servo_SG90`
2. Buat pocket U-shape:
   - Rongga dalam: *lebar body servo aktual* + 0.3 mm × *panjang body servo* + 0.3 mm
   - Kedua dinding samping: 2× lubang Ø2.2 mm untuk baut flange M2
   - Bagian bawah: 2× lubang Ø3.2 mm mounting ke chassis
   - Tebal dinding: min. 3 mm
3. Tambahkan slot atas (terbuka ke atas) agar servo mudah dimasukkan/dikeluarkan
4. **Extrude** seluruh body: 4 mm wall thickness
5. Fillet pada sudut eksternal: 1 mm

| Parameter               | Nilai                                     |
| ----------------------- | ----------------------------------------- |
| Rongga servo (L × W)   | ____×____ mm + 0.3 clearance |
| Lubang flange           | Ø2.2 mm × 2                             |
| Lubang mounting chassis | Ø3.2 mm × 2                             |
| Tinggi standoff servo   | sesuai posisi poros                       |

**Checkpoint ✅:** Dudukan servo SG90 selesai, servo dapat masuk dengan mulus.

---

#### D. Dudukan Baterai 2×18650

1. **Create > New Component** → nama: `Dudukan_Baterai_18650`
2. Buat housing dengan 2 rongga silindris:
   - Sketch 2 lingkaran diameter = *diameter baterai aktual* + 0.5 mm
   - Jarak antar pusat lingkaran: *diameter* + 2 mm (dinding pemisah)
   - **Extrude**: panjang baterai aktual + 3 mm
3. Buat dinding luar dengan **Shell** atau Extrude manual (tebal dinding ≥2 mm)
4. Tambahkan **retaining lip** di satu ujung:
   - Bibir menonjol ke dalam 2 mm agar baterai tidak terlepas
   - Sisi lainnya: terbuka untuk pemasangan baterai
5. Tambahkan **channel kabel** (12 × 5 mm slot) pada bagian samping untuk routing kabel
6. Bagian bawah: 4× lubang M3 untuk mounting ke chassis atas

**Model 3D Baterai 18650 (Referensi Assembly):**

7. **Create > New Component** → nama: `Baterai_18650`
8. Sketch lingkaran diameter aktual → **Extrude** panjang aktual → silinder baterai
9. Tambahkan positive cap (+): tonjolan kecil Ø5 mm, tinggi 1.5 mm di ujung positif
10. **Appearance** → warna biru/hijau, **Copy** menjadi 2 unit

| Parameter         | Nilai                         |
| ----------------- | ----------------------------- |
| Diameter rongga   | Ø____ mm (ukur aktual + 0.5) |
| Panjang rongga    | ____ mm (ukur aktual + 3)     |
| Jumlah sel        | 2                             |
| Ketebalan dinding | min. 2 mm                     |
| Lubang mounting   | 4× M3                        |

**Checkpoint ✅:** Dudukan baterai 2×18650 selesai; model 3D baterai siap sebagai referensi assembly.

---

#### E. Model 3D Motor DC Gearbox (Referensi Assembly)

1. **Create > New Component** → nama: `Motor_DC_Gearbox`
2. **Ukur motor DC TT** aktual dengan penggaris (panjang body, lebar, tinggi, diameter poros)
3. Buat profil samping motor menggunakan sketch → **Extrude** sesuai kedalaman body
4. Tambahkan detail:
   - Poros output: silinder Ø aktual, panjang poros aktual
   - Blok transmisi gearbox di sisi motor
5. **Appearance** → warna abu-abu / kuning sesuai motor fisik

| Parameter motor | Nilai (ukur aktual) |
|----------------|---------------------|
| Panjang body   | ___ mm              |
| Lebar          | ___ mm              |
| Tinggi         | ___ mm              |
| Diameter poros | ___ mm              |

**Checkpoint ✅:** Model 3D motor referensi selesai (tidak diproduksi, hanya referensi assembly).

---

#### F. Model 3D Roda (Referensi Assembly)

1. **Create > New Component** → nama: `Roda`
2. **Ukur roda** aktual dengan penggaris (diameter luar, lebar, diameter lubang poros)
3. **Create Sketch** → gambar profil roda: lingkaran luar, hub tengah, lubang poros
4. **Revolve** 360° terhadap sumbu tengah → bentuk silinder roda
5. **Appearance** → hitam (ban) + abu-abu (hub)
6. **Copy** → buat versi kiri dan kanan

| Parameter roda      | Nilai (ukur aktual) |
|--------------------|---------------------|
| Diameter luar      | ___ mm              |
| Lebar ban          | ___ mm              |
| Diameter lubang poros | ___ mm           |

**Checkpoint ✅:** Model 3D roda kiri dan kanan selesai (referensi assembly).

---

#### G. Cover / Enclosure PCB Sensor Garis (3D Print PLA)

1. **Create > New Component** → nama: `Cover_PCB_Sensor`
2. **Ukur PCB Sensor Garis** dengan penggaris (panjang, lebar, tinggi komponen tertinggi)
3. Desain cover/casing berbentuk U atau kanopi yang menutupi PCB dari atas:
   - Dimensi dalam: panjang PCB + 1 mm × lebar PCB + 1 mm (clearance)
   - Tinggi dinding: setinggi komponen tertinggi + 2 mm
   - Bagian bawah: **terbuka** (sensor menghadap ke bawah/lantai)
   - Bagian atas: tertutup (pelindung)
4. Tambahkan slot/ventilasi untuk keluaran kabel sensor
5. Tambahkan 2× lubang M2 kecil untuk mengunci cover ke bracket

| Parameter     | Nilai                        |
|--------------|------------------------------|
| Clearance dalam | PCB + 1 mm (semua sisi)   |
| Tebal dinding  | 2 mm                        |
| Lubang pengunci | 2× M2                      |

**Checkpoint ✅:** Cover PCB Sensor Garis selesai, siap export STL.

---

#### H. Aksesoris 3D Bebas (3D Print PLA) — Minimum 2 Buah

1. **Create > New Component** → nama: `Aksesoris_[NamaAksesoris]`
2. Desain bebas menggunakan Solid Modeling — pilih minimal 2 dari:
   - Spoiler / finn belakang
   - Cover dekoratif di sekitar sensor HC-SR04
   - Shroud roda
   - Name plate / branding dengan teks timbul
   - Bumper depan
   - LED mount
3. Pertimbangkan: tidak menghalangi sensor, tidak terlalu berat, hollow jika perlu
4. Export masing-masing sebagai **STL**

| Aksesoris | Fungsi | Posisi di Assembly |
|-----------|--------|-------------------|
| \_\_\_    | \_\_\_ | \_\_\_            |
| \_\_\_    | \_\_\_ | \_\_\_            |

**Checkpoint ✅:** Minimal 2 aksesoris 3D selesai, di-joint di assembly.

---

### PERCOBAAN 5: ASSEMBLY DAN INTERFERENCE CHECK

**Estimasi waktu: 45 menit**

1. Pastikan semua bagian sudah menjadi **Component** dengan nama yang jelas:
   - `Chassis_Box` (Sheet Metal single-piece, referensi assembly)
   - `Motor_DC_Gearbox` × 2 (model referensi)
   - `Roda_Kiri`, `Roda_Kanan` (model referensi)
   - `Motor_Mount_L`, `Motor_Mount_R` (3D print)
   - `Sensor_Bracket` (sensor garis + Cover_PCB_Sensor)
   - `Cover_PCB_Sensor` (3D print)
   - `Standoff_PCB` × 4 (3D print)
   - `Bracket_Sensor_Depan`, `Bracket_Sensor_Kiri`, `Bracket_Sensor_Kanan` (akrilik bending)
   - `Dudukan_Servo_SG90` (3D print)
   - `Dudukan_Baterai_18650` (3D print)
   - `Baterai_18650` × 2 (model referensi)
   - `Gripper_Base`, `Gripper_Jaw_L`, `Gripper_Jaw_R`, `Gripper_Linkage` (3D print)
   - `Aksesoris_[Nama]` × min. 2 (3D print)
   - `PCB_Main` (import dari EasyEDA)
   - `PCB_Sensor` (import dari EasyEDA)
2. **Assemble > Joint** (`J`):
   - `Chassis_Box` → Ground (fixed)
   - `Motor_Mount_L / R` → Rigid joint di slot motor Chassis_Box
   - `Sensor_Bracket` → Rigid joint di depan bawah Chassis_Box
   - `Standoff_PCB` × 4 → Rigid joint ke lubang mounting di Chassis_Box
   - `PCB_Main` → Rigid joint ke atas 4 standoff
   - `Dudukan_Sensor_Depan` → Rigid joint di dinding depan Chassis_Box
   - `Dudukan_Sensor_Kiri/Kanan` → Rigid joint di sisi depan Chassis_Box
   - `Dudukan_Servo_SG90` → Rigid joint di posisi yang ditentukan
   - `Dudukan_Baterai_18650` → Rigid joint di dalam Chassis_Box
   - `Gripper_Base` → Rigid joint di dinding depan Chassis_Box
   - `Gripper_Jaw_L / R` → Revolute joint ke pivot di Gripper_Base
   - `Gripper_Linkage` → Revolute joint ke servo horn dan jaw
3. **Inspect > Interference** → jalankan untuk seluruh assembly
4. Jika ada tabrakan:

| Komponen 1 | Komponen 2 | Masalah | Solusi |
| ---------- | ---------- | ------- | ------ |
|            |            |         |        |

5. Perbaiki hingga **0 interference**
6. **Inspect > Section Analysis** → cek clearance internal:
   - Clearance PCB MAIN ke chassis atas
   - Clearance sensor ke lantai (5–10 mm)
   - Clearance kabel routing

**Checkpoint ✅:** Seluruh assembly lengkap tanpa interference.

---

### PERCOBAAN 6: IMPORT PCB DAN VERIFIKASI FITMENT

**Estimasi waktu: 25 menit**

1. Export PCB dari EasyEDA:
   - **PCB MAIN**: `File > Export > STEP 3D` atau `DXF`
   - **PCB Sensor Garis**: `File > Export > STEP 3D` atau `DXF`
2. Di Fusion 360: **File > Open > Upload** → pilih file STEP/DXF
3. Jika DXF: Insert ke sketch → Extrude 1.6 mm (tebal PCB)
4. Konversi ke Component → rename:
   - `PCB_Main`
   - `PCB_Sensor`
5. Joint PCB ke posisi yang sesuai:
   - `PCB_Main` → di atas 4 standoff PCB
   - `PCB_Sensor` → di bracket sensor garis
6. Cek alignment:
   - [ ] Lubang PCB MAIN sejajar dengan lubang standoff
   - [ ] Konektor USB/power mudah diakses dari luar chassis
   - [ ] Tinggi komponen PCB tidak menabrak chassis atas
   - [ ] PCB Sensor berada pada ketinggian 5–10 mm dari lantai

**Checkpoint ✅:** Kedua PCB terintegrasi dalam assembly, fitment OK.

---

### PERCOBAAN 7: EXPORT FILE PRODUKSI DAN ORDER PCB

**Estimasi waktu: 30 menit**

#### A. Export STL

1. Klik kanan setiap component yang akan di-3D print → **Save as STL**:
   - `Motor_Mount_L.stl`
   - `Motor_Mount_R.stl`
   - `Sensor_Bracket.stl` (bracket sensor garis)
   - `Cover_PCB_Sensor.stl`
   - `Servo_Bracket.stl`
   - `Dudukan_Baterai_18650.stl`
   - `Gripper_Base.stl`
   - `Gripper_Jaw_L.stl`
   - `Gripper_Jaw_R.stl`
   - `Gripper_Linkage.stl`
   - `Aksesoris_[Nama].stl` (min. 2 buah)
2. Setting: Refinement = High, Format = Binary
3. Buka file STL di slicer (Cura), pastikan printable

#### B. Export DXF (Bracket Akrilik Bending)

1. Untuk bracket sensor akrilik (laser cut + bending):
   - **Sheet Metal > Flat Pattern** → klik kanan → **Export DXF**
2. File DXF:
   - `Bracket_Sensor_Depan.dxf`
   - `Bracket_Sensor_Kiri.dxf`
   - `Bracket_Sensor_Kanan.dxf`

> **Catatan:** Chassis tidak perlu DXF lagi — chassis metal sudah jadi, model Fusion 360 hanya referensi assembly.

#### C. Order PCB ke JLCPCB

1. Buka EasyEDA → project PCB Modul 01
2. **Fabrication > PCB Fabrication File (Gerber)** → download ZIP
3. Upload ke https://gerber-viewer.jlcpcb.com → verifikasi semua layer
4. **Fabrication > Order at JLCPCB** → review parameter:
   - Qty: 5 pcs, Layer: 2, Thickness: 1.6 mm
   - Color: Green, Surface: HASL Lead-free
   - Copper: 1 oz
5. Submit order dan catat nomor order

| Deliverable                | Format   | Nama File                    | Keterangan                                       |
| -------------------------- | -------- | ---------------------------- | ------------------------------------------------ |
| **Chassis Box (ref)**      | F3D      | `Chassis_Box.f3d`            | **Single-piece box chassis — referensi assembly**|
| Motor DC Gearbox (ref)     | F3D/STL  | `Motor_DC_Gearbox.f3d`       | Model 3D referensi assembly                      |
| Roda (ref)                 | F3D/STL  | `Roda.f3d`                   | Model 3D referensi assembly                      |
| Motor Mount L              | STL      | `Motor_Mount_L.stl`          | 3D print PLA                                     |
| Motor Mount R              | STL      | `Motor_Mount_R.stl`          | 3D print PLA                                     |
| Sensor Bracket (garis)     | STL      | `Sensor_Bracket.stl`         | 3D print PLA                                     |
| Cover PCB Sensor Garis     | STL      | `Cover_PCB_Sensor.stl`       | **3D print PLA — cover enclosure sensor garis**  |
| Standoff PCB × 4           | STL      | `Standoff_PCB.stl`           | 3D print PLA                                     |
| Bracket Sensor Depan       | **DXF**  | `Bracket_Sensor_Depan.dxf`   | **Laser cut + bending akrilik 3 mm**             |
| Bracket Sensor Kiri        | **DXF**  | `Bracket_Sensor_Kiri.dxf`    | **Laser cut + bending akrilik 3 mm, 20–25°**    |
| Bracket Sensor Kanan       | **DXF**  | `Bracket_Sensor_Kanan.dxf`   | **Mirror kiri — laser cut + bending**            |
| Bracket Servo SG90         | STL      | `Dudukan_Servo_SG90.stl`     | 3D print PLA                                     |
| Dudukan Baterai 2×18650    | STL      | `Dudukan_Baterai_18650.stl`  | 3D print PLA                                     |
| **Gripper Base**           | STL      | `Gripper_Base.stl`           | **3D print PLA — mount servo + bracket**         |
| **Gripper Jaw L**          | STL      | `Gripper_Jaw_L.stl`          | **3D print PLA — rahang kiri**                   |
| **Gripper Jaw R**          | STL      | `Gripper_Jaw_R.stl`          | **3D print PLA — rahang kanan**                  |
| **Gripper Linkage**        | STL      | `Gripper_Linkage.stl`        | **3D print PLA — penghubung servo–jaw**          |
| Aksesoris 3D (min 2 buah)  | STL      | `Aksesoris_[Nama].stl`       | **3D print PLA — desain bebas**                  |
| Gerber PCB MAIN            | ZIP      | `Gerber_PCB_Main.zip`        | Verifikasi JLCPCB Viewer                         |
| Gerber PCB Sensor          | ZIP      | `Gerber_PCB_Sensor.zip`      | Verifikasi JLCPCB Viewer                         |

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

### Analisa Percobaan 4 – Desain Dudukan Komponen

1. Mengapa pengukuran komponen fisik dengan penggaris penting sebelum membuat desain di Fusion 360? Apa risiko jika menggunakan dimensi estimasi tanpa verifikasi?
2. Berapa clearance yang Anda berikan pada rongga baterai 18650? Jelaskan alasan pemilihan nilai clearance tersebut.
3. Mengapa sensor kiri dan kanan dipasang dengan sudut **20–25° dari arah maju** (bukan 45° atau 90°)? Jelaskan hubungannya dengan karakteristik gelombang ultrasonik HC-SR04 (sudut efektif < 15° dan pantulan balik).
4. Jelaskan perbedaan workflow **Sheet Metal vs Solid Modeling** di Fusion 360 untuk desain bracket dari lembaran datar. Mengapa Sheet Metal lebih tepat untuk bracket akrilik laser cut?
5. Apa pertimbangan DFM (Design for Manufacturing) pada desain bracket akrilik sheet metal agar tidak retak saat dipotong laser?
6. Berapa tinggi minimal standoff PCB MAIN berdasarkan pengukuran komponen tertinggi pada PCB Anda?

### Analisa Percobaan 5 – Assembly dan Interference

1. Jika ditemukan interference antara dudukan baterai dan chassis atas, jelaskan langkah yang Anda lakukan untuk memperbaikinya.
2. Mengapa section analysis penting, terutama untuk memverifikasi clearance sensor dari lantai dan clearance baterai?
3. Apa perbedaan Rigid Joint dan Revolute Joint? Komponen mana dalam assembly ini yang menggunakan Revolute Joint?
4. Bagaimana urutan pemasangan (assembly sequence) yang logis saat merakit robot secara fisik? Apakah urutan joint di Fusion 360 mencerminkan urutan tersebut?

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

1. ---
2. ---
3. ---
4. ---
5. ---

---

## G. LAMPIRAN WAJIB

- [ ] Foto/dokumentasi **pengukuran komponen fisik** dengan penggaris (motor, roda, baterai, sensor, servo, PCB)
- [ ] Tabel pengukuran yang terisi lengkap (dari Percobaan 4)
- [ ] Screenshot Fusion 360 workspace **Sheet Metal** — model chassis (Flat Pattern terlihat)
- [ ] Screenshot **model 3D Motor DC Gearbox** dan **Roda** di Fusion 360 (referensi assembly)
- [ ] Screenshot Fusion 360 workspace **Sheet Metal** — bracket sensor (Flat Pattern + bend line terlihat)
- [ ] Screenshot **Flat Pattern** masing-masing bracket sensor (Depan, Kiri, Kanan) beserta garis tekukan
- [ ] Screenshot **Cover/Enclosure PCB Sensor Garis** dari Fusion 360 (tampak isometric)
- [ ] Screenshot **Aksesoris 3D** (min. 2 buah) dari Fusion 360 (tampak isometric)
- [ ] Screenshot assembly lengkap dari 3 sudut (isometric, top, front) dengan sudut sensor terlihat jelas
- [ ] Screenshot **Section Analysis** dari atas (tampilkan sudut 20–25° sensor kiri/kanan)
- [ ] Screenshot interference check (0 interference)
- [ ] Screenshot Gerber PCB MAIN di JLCPCB Gerber Viewer
- [ ] Screenshot Gerber PCB Sensor di JLCPCB Gerber Viewer
- [ ] Screenshot konfirmasi order JLCPCB (jika ada)
- [ ] **File DXF bracket sensor** (Bracket_Sensor_Depan.dxf, Kiri.dxf, Kanan.dxf)
- [ ] File STL semua komponen 3D print (motor mount, standoff PCB, cover sensor garis, servo, baterai, aksesoris)

---

**Tanda Tangan Dosen/Asisten:** ___________________
**Tanggal:** ___________________
