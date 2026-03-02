# NOTEBOOKLM — PROMPT GENERATOR SLIDE PRESENTASI
## Modul 03: Fusion 360 – Desain Mekanik Line Follower

---

## CARA PENGGUNAAN

1. Buka **https://notebooklm.google.com**
2. Buat Notebook baru → klik **"Add Source"**
3. Upload file sumber sesuai kelompok prompt di bawah
4. Klik ikon **"Generate"** atau buka chat, lalu **copy-paste prompt** yang sesuai
5. NotebookLM akan menghasilkan outline/konten slide — salin ke Google Slides / PowerPoint

> **Tips:** Jalankan satu prompt per sesi. Jika hasil kurang detail, tambahkan: *"Tambahkan poin-poin lebih spesifik dan gunakan data dari sumber."*

---

## SUMBER DOKUMEN

| File | Digunakan untuk |
|------|----------------|
| `Materi.md` | Slide 1–30 |
| `Jobsheet.md` | Slide 1–25 |
| `Project.md` | Slide 26–30 |
| `Tugas Video.md` | Slide 26–30 |

---

---

## PROMPT 1 — SLIDE 1 s/d 15
### Sumber: upload `Materi.md` + `Jobsheet.md`

```
Kamu adalah asisten pembuat presentasi akademik. Berdasarkan dokumen Materi.md dan Jobsheet.md yang sudah diupload, buatkan konten untuk 15 slide presentasi kuliah dengan topik "Modul 03 – Fusion 360: Desain Mekanik Robot Line Follower". Gunakan bahasa Indonesia yang jelas dan ringkas. Format setiap slide dengan: JUDUL SLIDE, 4–6 poin isi, dan 1 kalimat catatan dosen (speaker note).

Slide 1 — COVER
- Judul: Modul 03 – Fusion 360: Desain Mekanik Robot Line Follower
- Sub judul: Praktikum Mekatronika dan Robotika
- Nama program studi, tahun ajaran
- Logo kampus (placeholder)

Slide 2 — TUJUAN MODUL
- Ambil dari bagian Tujuan di Jobsheet.md (semua item tujuan praktikum)

Slide 3 — MENGAPA DESAIN MEKANIK PENTING?
- Ambil dari Materi.md Bab 1.1: kekuatan struktur, distribusi berat, kompatibilitas dimensi, manufakturabilitas

Slide 4 — PERAN CAD DAN KEUNGGULAN FUSION 360
- Ambil dari Materi.md Bab 1.2 dan 1.3: table keunggulan Fusion 360

Slide 5 — REGISTRASI FUSION 360 EDUCATION VIA SSO KAMPUS
- Ambil dari Materi.md Bab 2.2: langkah-langkah registrasi akun education dengan email SSO

Slide 6 — FUSION 360 ONLINE VS DESKTOP
- Ambil dari Materi.md Bab 2.3 dan 2.4: cara akses fusion.online.autodesk.com, perbandingan fitur, batasan versi Online

Slide 7 — ANTARMUKA FUSION 360 (WORKSPACE)
- Ambil dari Materi.md Bab 3: komponen antarmuka (toolbar, browser, timeline, view cube), cara ganti workspace (Design, Sheet Metal, Manufacturing, dll)

Slide 8 — SKETCH 2D: TOOLS DAN CONSTRAINTS
- Ambil dari Materi.md Bab 4: tools sketch (Line, Rectangle, Circle, Arc, Offset), constraint (horizontal, perpendicular, coincident, symmetric), dimension
- Tekankan: sketch wajib "Fully Constrained" (semua garis hitam) sebelum lanjut 3D

Slide 9 — SKETCH FULLY CONSTRAINED = SIAP EXTRUDE
- Perbedaan visual: sketch under-constrained (biru) vs fully constrained (hitam)
- Tips mempercepat: gunakan dimension + constraints, bukan menggeser manual

Slide 10 — SOLID MODELING: EXTRUDE, HOLE, FILLET, PATTERN
- Ambil dari Materi.md Bab 5: Extrude (New Body, Join, Cut), Hole dialog (diameter, kedalaman, type), Fillet (R=1–3mm untuk hindari stress crack), Pattern (Rectangular/Circular)
- Tips DFM: minimum wall thickness 2mm untuk PLA 3D print

Slide 11 — SHEET METAL WORKSPACE: FLAT PATTERN → DXF
- Ambil dari Materi.md Bab 8.4 dan Bab 12: alasan menggunakan Sheet Metal untuk bracket akrilik, langkah: Sheet Metal Rules (tebal 3mm, bend radius 0mm) → Base Flange → Flange → Lubang → Flat Pattern → Export DXF

Slide 12 — ASSEMBLY DAN JOINT
- Ambil dari Materi.md Bab 6: konsep assembly (component vs body), joint types (Rigid, Revolute, Slider), Interference Check, Section Analysis
- Use case: roda = Revolute joint, semua bracket/PCB = Rigid joint

Slide 13 — DESIGN FOR MANUFACTURING (DFM): 3D PRINT vs LASER CUT + BENDING vs MODEL REFERENSI
- Ambil dari Materi.md Bab 10 dan 11: tabel perbandingan 3 kategori produksi
- **3D print PLA**: motor mount, standoff PCB, bracket sensor garis, cover PCB sensor garis, dudukan servo, dudukan baterai, **gripper (Base+Jaw_L+Jaw_R+Linkage)**, aksesoris 3D
- **Laser cut + bending akrilik 3mm**: semua bracket sensor HC-SR04 (1 piece per bracket, dibending dengan heat gun)
- **Model 3D referensi (tidak diproduksi)**: chassis box metal single-piece (sudah jadi), motor DC gearbox, roda, baterai 18650
- Tekankan: chassis BUKAN 2-deck terpisah — adalah 1 bodi box terintegrasi dari metal; model hanya referensi assembly

Slide 14 — JOBSHEET: ALAT, BAHAN, DAN CARA UKUR KOMPONEN FISIK
- Ambil dari Jobsheet.md Bagian B (Alat dan Bahan): daftar alat termasuk penggaris dan jangka sorong
- Bagian C Dimensi: tabel pengukuran komponen (roda, motor, servo, sensor, baterai, PCB)
- Tekankan: UKUR FISIK DULU dengan penggaris, baru buka Fusion 360

Slide 15 — PERCOBAAN 1 & 2: SKETCH DASAR + MODEL BOX CHASSIS SHEET METAL (REFERENSI)
- Percobaan 1: sketch kotak dengan lubang M3, extrude 3mm → verifikasi fully constrained
- Percobaan 2: ganti workspace ke Sheet Metal → Sheet Metal Rules (tebal pelat aktual, material Aluminium) → sketch **alas kotak (bottom plate)** dari ukuran aktual → Base Flange → tambah **Flange ke-4 sisi** (dinding samping, tinggi = tinggi box) → lubang poros motor di dinding kiri/kanan → Flat Pattern
- Komponen: **1 komponen** saja → `Chassis_Box` (BUKAN Chassis_Bottom + Chassis_Top + Spacer terpisah)
- Tekankan: chassis metal adalah **single-piece box terintegrasi** — model hanya REFERENSI ASSEMBLY, tidak diproduksi
```

---

---

## PROMPT 2 — SLIDE 16 s/d 30
### Sumber: upload `Materi.md` + `Jobsheet.md` + `Project.md` + `Tugas Video.md`

```
Lanjutkan membuat konten Slide 16–30. Sumber: Materi.md + Jobsheet.md (Slide 16–25) dan Project.md + Tugas Video.md (Slide 26–30). Bahasa Indonesia. Format: JUDUL SLIDE, 4–6 poin isi, 1 speaker note.

Slide 16 — PERCOBAAN 3: DESAIN GRIPPER SG90 (PLA, PARALLEL JAW)
- Gripper parallel jaw: 1× SG90, bukaan 10–60 mm, grip benda ~5 cm
- 4 komponen: `Gripper_Base` (mount+slot servo), `Gripper_Jaw_L` (arm 60mm, R3), `Gripper_Jaw_R` (mirror), `Gripper_Linkage` (pivot ~30mm)
- Joint Revolute tiap pivot; animasi SG90 0°=terbuka 60mm, 90°=menutup
- Export: 4 STL (Gripper_Base, Jaw_L, Jaw_R, Linkage)

Slide 17 — PERCOBAAN 4A: DUDUKAN SENSOR HC-SR04 (SHEET METAL + AKRILIK)
- Sheet Metal Rules: tebal 3mm, bend radius 1mm; 1 piece per bracket
- Bracket Depan: tekukan 90°, lubang M2 ×2 → DXF
- Bracket Kiri/Kanan: tekukan 65–70° (sensor 20–25°) → DXF + Mirror
- Flat Pattern + bend line → panduan bending akrilik heat gun

Slide 18 — KONFIGURASI SUDUT SENSOR: 0°/20–25°/20–25°
- Depan 0°, kiri 20–25°, kanan 20–25° (mirror)
- Alasan: coverage lateral; sudut >45° pantulan gagal kembali ke receiver
- 1 piece per bracket, dibending heat gun (bukan 2-piece terpisah)

Slide 19 — PERCOBAAN 4B: MOTOR MOUNT PLA (L-SHAPE, RIB, MIRROR)
- L-shape/U-clamp sesuai body motor DC TT, pocket menggenggam motor
- Fitur: 2× M3 ke chassis, rib sudut dalam ≥4mm, infill ≥40%
- Mirror → `Motor_Mount_R`; export 2 STL

Slide 20 — PERCOBAAN 4C: STANDOFF PCB + BRACKET SENSOR GARIS + COVER
- Standoff: Ø7/Ø3.2mm, flange 12×2mm, tinggi min 15mm
- Bracket sensor garis: L-bracket, slot adjustable, 5–10mm dari lantai
- Cover PCB Sensor: enclosure PLA, clearance 1mm, slot kabel

Slide 21 — PERCOBAAN 4D: DUDUKAN SERVO SG90 + BATERAI 18650
- Servo: U-shape, pocket SG90+0.3mm, slot terbuka atas, 2×M3 chassis
- Baterai: 2 rongga Ø(aktual+0.5mm), retaining lip 2mm, cable channel 12×5mm
- Model 3D baterai referensi assembly

Slide 22 — PERCOBAAN 5: ASSEMBLY LENGKAP — JOINT REVOLUTE & RIGID
- Ground: `Chassis_Box`; Roda+Gripper Jaw = Revolute; bracket/PCB = Rigid
- Interference Check → 0; Section Analysis: clearance PCB + sudut sensor

Slide 23 — PERCOBAAN 6: IMPORT PCB DARI EASYEDA
- Export EasyEDA: File → Export → STEP 3D Model
- Import Fusion 360: Insert STEP → konversi ke Component
- PCB MAIN → Rigid ke standoff; PCB Sensor → Rigid ke bracket

Slide 24 — EXPORT FILE PRODUKSI: STL DAN DXF
- STL: Motor_Mount, Standoff, Cover, Dudukan_Servo, Dudukan_Baterai, Gripper (4), Aksesoris
- DXF: Flat Pattern 3 bracket sensor HC-SR04 (1 DXF per bracket)
- Penamaan: underscore, tanpa spasi

Slide 25 — FABRIKASI + ORDER PCB JLCPCB
- 3D Print: layer 0.2mm, infill 30–40%, gripper jaw 50%+, support >45°
- Laser Cut akrilik: kerf 0.1mm, fillet R1.5mm, tandai bend line
- JLCPCB: Gerber ZIP, 2 layer, 1.6mm, HASL, konfirmasi order

Slide 26 — PROJECT: OVERVIEW 9 SOAL KELOMPOK
- 9 soal: Chassis_Box, Motor+Roda, Motor Mount, Sensor Bracket, Standoff+Cover, Servo+Baterai, Aksesoris, Gripper SG90, Assembly+PCB
- Semua mekanik produksi mandiri (3D print + laser cut akrilik)
- Wajib ukur fisik komponen sebelum mendesain

Slide 27 — SOAL 1–4: CHASSIS, MOTOR+RODA, MOTOR MOUNT, SENSOR BRACKET
- Soal 1: `Chassis_Box` Sheet Metal 1 komponen, Flat Pattern → `Chassis_Box.f3d` referensi
- Soal 2: Motor DC + Roda 3D referensi, Revolve roda → `.f3d`
- Soal 3: `Motor_Mount_L/R` L-shape, rib PLA → 2 STL
- Soal 4: 3 bracket HC-SR04, 1 piece per bracket, 0°/20–25° → 3 DXF

Slide 28 — SOAL 5–8: STANDOFF, SERVO+BATERAI, AKSESORIS, GRIPPER
- Soal 5: Standoff ×4 + Bracket sensor garis + Cover PCB Sensor → STL
- Soal 6: Dudukan servo (U-shape) + Dudukan baterai 2×18650 + model ref → STL
- Soal 7: Aksesoris 3D bebas min. 2 buah → STL
- Soal 8 Gripper SG90: 4 komponen PLA, Revolute, 10–60mm → 4 STL

Slide 29 — SOAL 9 + DELIVERABLES + RUBRIK PENILAIAN
- Soal 9: assembly Chassis_Box ground + PCB EasyEDA + gripper Revolute + interference 0 + rendering 3 sudut
- Deliverables: 27 item (f3d assembly, 3 DXF, 10+ STL, 2 Gerber ZIP, screenshot)
- Rubrik 100pt: chassis 10, sensor 12, gripper 10, assembly 13, JLCPCB 7, dst.

Slide 30 — TUGAS VIDEO INDIVIDUAL: 6 BAGIAN + RUBRIK
- 6 bagian: (1) Registrasi Fusion 360 SSO, (2) Ukur fisik (tangan+penggaris di kamera), (3) Desain Part 1–8 (Part 8=Gripper), (4) Assembly+animasi gripper, (5) Order JLCPCB, (6) Penutup
- Rubrik 100pt: Registrasi 10, Pengukuran 15, Desain 57 (Gripper 10), Assembly 11, JLCPCB 5, Penutup 2
- Format: MP4 ≥720p, 15–25 menit, nama: Video_Modul03_[NIM]_[Nama].mp4
```

---

---

## CATATAN TAMBAHAN

### Tips Prompt NotebookLM

- Jika slide terlalu panjang: tambahkan → *"Persingkat setiap poin menjadi maksimal 10 kata"*
- Jika ingin visual: tambahkan → *"Sarankan 1 jenis visual/diagram untuk setiap slide (tabel, diagram alur, gambar, dsb)"*
- Jika ingin speaker note lebih panjang: tambahkan → *"Perluas speaker note menjadi 3–5 kalimat untuk panduan dosen"*
- Jika ingin dalam bahasa Inggris: ganti instruksi bahasa → *"Use formal English"*

### Urutan Upload Sumber di NotebookLM

| Prompt | Upload file ini |
|--------|----------------|
| Prompt 1 (Slide 1–15) | `Materi.md` + `Jobsheet.md` |
| Prompt 2 (Slide 16–30) | `Materi.md` + `Jobsheet.md` + `Project.md` + `Tugas Video.md` |
