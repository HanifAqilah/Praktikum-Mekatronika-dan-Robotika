# TUGAS VIDEO MODUL 03: FUSION 360 – DESAIN MEKANIK LINE FOLLOWER & ORDER PCB

**Program Studi:** Teknologi Rekayasa Otomasi
**Mata Kuliah:** Praktikum Mekatronika dan Robotika
**Modul:** 03 – Fusion 360 Desain Mekanik
**Sifat Tugas:** Individual
**Durasi Video:** 15–25 menit
**Format:** MP4 (minimal 720p)
**Platform Upload:** LMS / Google Drive / YouTube Unlisted

---

## A. KETENTUAN UMUM

1. Video dikerjakan **per orang** (bukan kelompok).
2. Wajib menampilkan **nama lengkap dan NIM** di awal video (tampil di layar minimal 5 detik).
3. **Bagian 1 & 2**: rekam layar (screen recording) saat registrasi dan desain di Fusion 360 Online.
4. **Bagian 2 (pengukuran)**: rekam kamera/webcam langsung — tangan dan penggaris harus terlihat jelas.
5. **Webcam** wajib terlihat di sudut layar pada seluruh bagian screen recording.
6. Narasi menggunakan Bahasa Indonesia yang runtut.
7. **Semua komponen mekanik diproduksi sendiri** (3D print + laser cut) — tidak ada yang diorder online.
8. Seluruh desain dikerjakan di **Fusion 360 Online** (fusion.online.autodesk.com) menggunakan akun Education SSO kampus.

---

## B. URUTAN WAJIB ISI VIDEO

---

### BAGIAN 1 — REGISTRASI FUSION 360 ONLINE DAN PENJELASAN (Maks. 3 menit)

> **Wajib screen recording** — rekam proses dari awal login sampai Fusion 360 Online terbuka.

- [ ] Buka browser → pergi ke **https://autodesk.com/education**
- [ ] Klik *"Get Free Education Software"* → pilih **Fusion 360**
- [ ] Klik **"Sign In"** atau **"Get started"** → pilih *"Sign in with SSO"*
- [ ] Masukkan **domain email kampus** (contoh: `universitasxyz.ac.id`) → klik Continue
- [ ] Login dengan **email SSO kampus** + password institusi
- [ ] Verifikasi status mahasiswa jika diminta (upload kartu mahasiswa / KRS)
- [ ] Setelah akun aktif, buka **https://fusion.online.autodesk.com**
- [ ] Tunjukkan halaman Fusion 360 Online telah terbuka — antarmuka browser
- [ ] Jelaskan singkat perbedaan Fusion 360 **Desktop** vs **Online** (cloud, tidak perlu install, akses dari browser)
- [ ] Perkenalkan antarmuka: toolbar kiri, browser, timeline, view cube

---

### BAGIAN 2 — VIDEO PENGUKURAN KOMPONEN FISIK DENGAN PENGGARIS (Maks. 4 menit)

> **Wajib rekam kamera/webcam** — tangan dan penggaris harus terlihat jelas di frame.
> Catat semua hasil ukur di tabel — tabel ini akan digunakan saat desain di Bagian 3.

Ukur dan tampilkan di kamera:

- [ ] **Roda** — ukur diameter roda dengan penggaris (target: 66 mm); lebar roda
- [ ] **Motor DC TT** — ukur panjang, lebar, tinggi body; jarak sumbu as ke tepi bawah
- [ ] **Chassis (referensi)** — ukur panjang dan lebar alas robot jika sudah ada referensi fisik
- [ ] **Servo SG90** — ukur panjang, lebar, tinggi body; jarak antar lubang flange; tinggi flange dari body
- [ ] **Sensor HC-SR04** — ukur lebar PCB (45 mm), tinggi komponen; jarak antar lubang mounting
- [ ] **Baterai 18650** — ukur diameter (Ø18.5 mm) dan panjang (65 mm) dengan penggaris/jangka sorong
- [ ] **PCB MAIN** — ukur panjang, lebar, posisi 4 lubang mounting (jarak antar lubang)
- [ ] **PCB Sensor Garis** — ukur panjang, lebar, posisi lubang mounting

Tunjukkan **tabel hasil pengukuran** yang sudah diisi (boleh tulis tangan atau digital):

| Komponen                 | Dimensi Aktual (mm)  | Keterangan |
| ------------------------ | -------------------- | ---------- |
| Roda                     | Ø... × lebar...    |            |
| Motor DC TT              | ...×...×...        |            |
| Servo SG90 body          | ...×...×...        |            |
| Servo SG90 lubang flange | jarak ...            |            |
| HC-SR04 PCB              | 45 × ...            |            |
| HC-SR04 lubang mounting  | c-t-c ...            |            |
| Baterai 18650            | Ø... × ...         |            |
| PCB MAIN                 | ...×..., lubang ... |            |
| PCB Sensor               | ...×...             |            |

---

### BAGIAN 3 — SCREEN RECORD DESAIN SELURUH PART DI FUSION 360 ONLINE

> **Wajib screen recording** di Fusion 360 Online.
> Tampilkan dimensi aktual dari tabel pengukuran Bagian 2 saat memasukkan angka di Fusion 360.
> Setiap part: tunjukkan sketch → extrude/sheet metal → hasil 3D → export (STL atau Flat Pattern DXF).

Desain seluruh part berikut **secara berurutan**:

#### Part 1 — Model Box Chassis Single-Piece Metal (Sheet Metal Workspace — Referensi Assembly)

> Chassis robot adalah **bodi box metal single-piece (aluminium alloy)** yang sudah jadi. Tujuan: membuat model 3D box chassis sebagai referensi assembly. **Tidak ada Chassis_Top terpisah dan tidak ada Spacer** — semuanya adalah satu komponen box terintegrasi.

- [ ] Ganti workspace ke **Sheet Metal** di Fusion 360 Online
- [ ] Atur Sheet Metal Rules: Thickness = tebal pelat aktual, Material = Aluminium, Bend Radius = 1× tebal
- [ ] **Create New Component** → nama: `Chassis_Box`
- [ ] **Sketch** profil **alas kotak** (bottom plate) pada bidang XY — outline sesuai fisik, slot motor kiri + kanan
- [ ] **Sheet Metal > Flange** → jadikan base flange
- [ ] Tambahkan **Flange** ke-4 sisi sebagai dinding samping (tinggi = tinggi box aktual)
- [ ] Tambahkan lubang poros motor di dinding kiri dan kanan (Cut sketch circle)
- [ ] **Sheet Metal > Flat Pattern** → verifikasi single flat pattern seluruh box
- [ ] Jelaskan perbedaan dengan chassis 2-deck: ini adalah 1 komponen box, bukan 2 plate + spacer

#### Part 2 — Model 3D Motor DC Gearbox dan Roda (Referensi Assembly)

> Model referensi — tidak diproduksi. Tujuan: posisi dan clearance dalam assembly.

- [ ] **Motor DC Gearbox**: sketch profil body → Extrude → tambah poros output + gearbox block → Appearance abu-abu/kuning
- [ ] **Roda**: sketch profil roda (lingkaran luar + hub + lubang poros) → **Revolve** 360° → Appearance hitam
- [ ] Mirror roda → buat versi kanan dan kiri
- [ ] Tunjukkan kedua model di assembly secara bersamaan

#### Part 3 — Dudukan 3 Sensor HC-SR04 (Akrilik, Sheet Metal + Bending, Laser Cut)

- [ ] Buka workspace **Sheet Metal** di Fusion 360 Online
- [ ] Atur Sheet Metal Rules: Thickness 3 mm, **Bend Radius 1 mm**, K-factor 0.33
- [ ] **Bracket Sensor Depan** (0°, lurus): Flange L-bracket → Flat Pattern dengan **bend line** → `Bracket_Sensor_Depan.dxf`
- [ ] **Bracket Sensor Kiri** (20–25°): **1 piece** dengan tekukan 65–70° → Flat Pattern → `Bracket_Sensor_Kiri.dxf`
- [ ] **Mirror** bracket kiri → Sensor Kanan → `Bracket_Sensor_Kanan.dxf`
- [ ] Tunjukkan **garis tekukan** pada Flat Pattern, jelaskan proses bending akrilik (heat gun)
- [ ] Jelaskan alasan sudut 20–25° (effective angle HC-SR04 < 15°)

#### Part 4 — Standoff PCB MAIN + Cover PCB Sensor Garis (PLA, 3D Print)

- [ ] Standoff: silinder berlubang outer Ø7 mm, inner Ø3.2 mm, flange Ø12 mm → `Standoff_PCB.stl`
- [ ] Bracket sensor garis: L-bracket, slot adjustable → `Bracket_Sensor_Garis.stl`
- [ ] **Cover/Enclosure PCB Sensor**: ukur PCB sensor terlebih dahulu → desain cover yang menutupi PCB dari atas (dimensi dalam = PCB + 1 mm clearance, dinding 2 mm, slot kabel) → `Cover_PCB_Sensor.stl`
- [ ] Tunjukkan cover ter-join ke bracket pada posisi di atas PCB sensor

#### Part 5 — Dudukan Servo SG90 + Dudukan Baterai + Model Baterai (PLA, 3D Print)

- [ ] Dudukan servo: U-shape, pocket sesuai body SG90 + 0.3 mm clearance, lubang flange M2, lubang mounting M3 → `Dudukan_Servo_SG90.stl`
- [ ] **Model 3D Baterai 18650**: sketch lingkaran Ø aktual → Extrude panjang aktual → tambah positive cap → Appearance biru/hijau → Copy 2 unit
- [ ] Dudukan baterai: 2 rongga silinder Ø(aktual+0.5), retaining lip, cable channel, 4× lubang M3 → `Dudukan_Baterai_18650.stl`

#### Part 6 — Aksesoris 3D Bebas (PLA, 3D Print — Minimum 2 Buah)

- [ ] Desain aksesoris bebas pilihan sendiri (min. 2 buah)
- [ ] Tunjukkan proses desain secara singkat (sketching → extrude → fillet → Appearance)
- [ ] Jelaskan fungsi dan posisi setiap aksesoris
- [ ] Export masing-masing sebagai STL: `Aksesoris_[Nama1].stl`, `Aksesoris_[Nama2].stl`

#### Part 7 — Gripper SG90 (PLA, 3D Print — Parallel Jaw)

> Gripper parallel jaw digerakkan 1× servo SG90. Bukaan jaw: 10–60 mm, grip benda ~5 cm. Desain 4 komponen terpisah.

- [ ] **Gripper_Base**: Sketch + Extrude mount bracket (slot servo SG90 32.5×12 mm pocket, 4× lubang M3) → tebal 5 mm
- [ ] **Gripper_Jaw_L**: Sketch L-shape (arm 60 mm, ujung finger-grip R3 mm, lubang pivot Ø3.2 mm) → Extrude 4 mm
- [ ] **Gripper_Jaw_R**: Mirror dari Jaw_L terhadap bidang tengah
- [ ] **Gripper_Linkage**: Sketch batang ~30 mm, 2× lubang pivot Ø3.2 mm → Extrude 3 mm
- [ ] Assembly joint di dalam gripper sub-assembly:
  - Jaw_L ke Base: **Revolute Joint** pada sumbu pivot
  - Jaw_R ke Base: **Revolute Joint** pada sumbu pivot
  - Linkage ke servo horn: **Revolute Joint**
  - Linkage ke Jaw_L: **Revolute Joint**
- [ ] Animasikan: SG90 0° = jaw terbuka 60 mm, SG90 90° = jaw menutup ~10 mm
- [ ] Tunjukkan di kamera: gerak jaw yang mulus dan simetris
- [ ] Export: `Gripper_Base.stl`, `Gripper_Jaw_L.stl`, `Gripper_Jaw_R.stl`, `Gripper_Linkage.stl`

---

### BAGIAN 4 — SCREEN RECORD ASSEMBLY DI FUSION 360 ONLINE (Maks. 4 menit)

> **Wajib screen recording** di Fusion 360 Online.

- [ ] Buat file assembly baru — import semua component yang sudah didesain
- [ ] Import **PCB MAIN** dan **PCB Sensor** (STEP/DXF dari EasyEDA) ke assembly
- [ ] Joint seluruh komponen:
  - Motor DC Gearbox × 2 + Roda kiri/kanan → **Rigid Joint** (posisi referensi)
  - Roda → **Revolute Joint** (dapat berputar)
  - Baterai 18650 × 2 → di-joint ke dudukan baterai (referensi posisi)
  - **Gripper_Base** → **Rigid Joint** ke dinding depan Chassis_Box
  - **Gripper_Jaw_L/R** → **Revolute Joint** ke pivot di Gripper_Base
  - Semua dudukan, bracket, PCB, aksesoris → **Rigid Joint**
- [ ] Tunjukkan **aksesoris 3D** terpasang di assembly
- [ ] Tunjukkan **gripper** terpasang di depan Chassis_Box, animasikan gerakan jaw
- [ ] Jalankan **Interference Check** → tunjukkan hasil **0 interference**
- [ ] Tunjukkan **Section Analysis** — potongan dari atas untuk melihat sudut sensor kiri/kanan 20–25°
- [ ] Screenshot/export rendering dari 3 sudut: **isometric, top view, front view**

---

### BAGIAN 6 — PENUTUP (Maks. 1 menit)

- [ ] Tampilkan seluruh file output yang sudah dibuat (STL + DXF + Gerber)
- [ ] Ringkas: komponen mana yang **3D print PLA** dan mana yang **laser cut akrilik**
- [ ] Sebutkan kendala yang ditemui dan solusi
- [ ] Koneksi ke Modul 07: "File ini akan digunakan pada saat build Line Follower"

---

## C. RUBRIK PENILAIAN VIDEO (100 POIN)

| No              | Komponen Penilaian                                                                                          | Bobot         | Indikator                                                                |
| --------------- | ----------------------------------------------------------------------------------------------------------- | ------------- | ------------------------------------------------------------------------ |
| 1               | Registrasi Fusion 360 Online + penjelasan antarmuka                                                         | 10            | Proses login SSO terlihat, antarmuka dijelaskan                          |
| 2               | Video pengukuran komponen fisik dengan penggaris (tangan terlihat)                                          | 15            | Semua komponen diukur, tabel terisi, dimensi aktual digunakan            |
| 3               | Part 1: Model chassis Sheet Metal (referensi) + Spacer                                                      | 5             | Sheet Metal workspace, Flat Pattern terlihat                             |
| 4               | Part 2: Model 3D Motor DC Gearbox + Roda (referensi)                                                        | 5             | Revolve roda, detail motor, Appearance                                   |
| 5               | Part 3: Motor Mount L + R (PLA, 3D print)                                                                   | 5             | STL valid, mirror feature                                                |
| 6               | Part 4: Bracket 3 sensor HC-SR04 (akrilik, Sheet Metal + bending): sudut 0°, 20–25°, 1 piece + bend line | 15            | Flat Pattern dengan bend line, K-factor diset, proses bending dijelaskan |
| 7               | Part 5: Standoff PCB + Bracket sensor garis + Cover PCB Sensor Garis (3D print)                             | 8             | Cover fit di atas PCB sensor, STL valid                                  |
| 8               | Part 6: Dudukan servo + Dudukan baterai + Model 3D Baterai (referensi)                                      | 7             | Model baterai via Revolve, STL housing valid                             |
| 9               | Part 7: Aksesoris 3D bebas (min. 2 buah, kreatif, 3D print)                                                 | 15            | Desain kreatif, tidak interference, STL valid                            |
| 10              | Assembly lengkap: semua komponen + PCB + aksesoris, 0 interference, section analysis                        | 15            | Screenshot terlampir, joint correct, sudut sensor terlihat               |
|                 |                                                                                                             |               |                                                                          |
| **TOTAL** |                                                                                                             | **100** |                                                                          |

---

## D. CHECKLIST SEBELUM SUBMIT

**Konten wajib:**

- [ ] Nama dan NIM tampil di awal video (≥5 detik)
- [ ] Screen record registrasi Fusion 360 Online via SSO kampus
- [ ] Antarmuka Fusion 360 Online dijelaskan (toolbar, browser, timeline)
- [ ] Video kamera/webcam: pengukuran SEMUA komponen fisik dengan penggaris
- [ ] Tabel ukuran terisi dan ditampilkan di video
- [ ] Screen record Part 1: **Box chassis single-piece** Sheet Metal (1 komponen `Chassis_Box`, Flat Pattern terverifikasi)
- [ ] Screen record Part 2: Model 3D Motor DC Gearbox dan Roda (referensi assembly)
- [ ] Screen record Part 3: Motor mount L + R (STL)
- [ ] Screen record Part 4: Bracket 3 sensor HC-SR04 Sheet Metal + bending — sudut 0°, 20–25°, bend line di Flat Pattern, 1 piece per bracket (DXF)
- [ ] Screen record Part 5: Standoff PCB + Bracket sensor garis + Cover PCB Sensor Garis (STL)
- [ ] Screen record Part 6: Dudukan servo SG90 + Dudukan baterai + Model 3D Baterai (referensi)
- [ ] Screen record Part 7: Aksesoris 3D bebas (min. 2 buah) — desain dan proses dijelaskan
- [ ] **Screen record Part 8: Gripper SG90** (Gripper_Base + Jaw_L + Jaw_R + Linkage), joint Revolute, animasi 0–60 mm, 4 STL diekspor
- [ ] Interference check hasilnya 0
- [ ] Section analysis — sudut sensor terlihat
- [ ] Penutup: daftar file output + rencana fabrikasi

**Kualitas teknis:**

- [ ] Resolusi minimal 720p
- [ ] Audio jelas (tidak ada background noise berlebihan)
- [ ] Durasi 15–25 menit
- [ ] Screen recording tajam — zoom in pada detail penting (sudut bracket, Flat Pattern + bend line, dimensi di kotak input)
- [ ] Webcam terlihat di sudut layar pada semua bagian screen recording

**Penamaan file:**

- [ ] `Video_Modul03_[NIM]_[NamaLengkap].mp4`
