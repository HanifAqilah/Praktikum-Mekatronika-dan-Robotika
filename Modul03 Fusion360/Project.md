# PROJECT MODUL 03: DESAIN MEKANIK LENGKAP ROBOT LINE FOLLOWER SIAP PRODUKSI

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 03 – Fusion 360 Desain Mekanik  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. DESKRIPSI PROJECT

### Soal Cerita

> Tim robotik kampus Anda dipercaya merancang **Robot Line Follower dengan kemampuan deteksi rintangan** untuk kompetisi antar-universitas. Robot harus mampu mengikuti garis hitam di atas latar putih dengan kecepatan tinggi, berbelok tajam, dan menghindari rintangan menggunakan 3 sensor ultrasonik HC-SR04.
>
> Semua komponen mekanik — chassis, bracket, dudukan sensor, dudukan servo, dan dudukan baterai — **diproduksi mandiri** menggunakan mesin yang tersedia di lab: **3D printer (PLA)** dan **mesin laser cutting (akrilik 3 mm)**. Tidak ada komponen mekanik yang dipesan secara online. **Hanya PCB yang diorder ke JLCPCB.**
>
> Robot harus **modular**: chassis box metal satu-piece yang kokoh, semua bracket mudah dilepas dan dipasang ulang tanpa merusak komponen. Kelompok Anda harus menghasilkan desain Fusion 360 yang lengkap, dapat difabrikasi di lab, dan terintegrasi dengan PCB MAIN serta PCB Sensor yang sudah didesain di Modul 01.

---

## B. SOAL PROJECT

### Soal 1 — Desain Model Box Chassis Single-Piece Metal (Sheet Metal Workspace — Referensi Assembly)

> Chassis robot menggunakan **bodi box metal single-piece (aluminium alloy)** yang sudah jadi. Ini adalah satu komponen terintegrasi berupa kotak dengan dinding samping, bawah, dan atas yang merupakan satu lipatan pelat. Tugas Anda adalah **memodelkan chassis box ini di Fusion 360** menggunakan workspace **Sheet Metal** sebagai referensi assembly.
>
> **Sebelum mulai:** Ukur chassis fisik dengan penggaris (panjang, lebar, tinggi dinding samping, tebal pelat, posisi semua lubang aktual).
>
> Spesifikasi model:
> - Workspace: **Sheet Metal** (bukan Solid Modeling biasa)
> - Sheet Metal Rules: Thickness = tebal pelat aktual, Material = Aluminium
> - Komponen: **1 component** bernama `Chassis_Box` (single-piece box)
> - Bottom plate: outline aktual + slot motor kiri + kanan
> - Dinding samping: **Flange** ke-4 sisi, tinggi = tinggi box aktual
> - Lubang poros motor di dinding kiri + kanan
> - Lubang mounting PCB (4×M3 di atas), lubang caster (2×M3)
> - **Flat Pattern** terverifikasi (satu pola lipat untuk seluruh box)
>
> **Output:** File `.f3d` component `Chassis_Box` (Sheet Metal single-piece, referensi assembly).

### Soal 2 — Model 3D Motor DC Gearbox + Roda (Referensi Assembly)

> Motor DC Gearbox dan Roda sudah ada secara fisik. Tugas Anda adalah **memodelkan keduanya secara 3D** di Fusion 360 sebagai referensi dimensi dan clearance dalam assembly.
>
> **Sebelum mulai:** Ukur motor (panjang body, lebar, tinggi, diameter poros) dan roda (diameter luar, lebar, diameter lubang poros) dengan penggaris.
>
> **Motor DC Gearbox:**
> - Component: `Motor_DC_Gearbox`
> - Body utama motor + blok transmisi gearbox
> - Poros output: silinder diameter aktual
> - Appearance: abu-abu/kuning sesuai motor fisik
>
> **Roda:**
> - Component: `Roda`
> - Profil silinder: diameter luar + lebar ban + lubang poros aktual
> - Metode: Sketch profil → **Revolve** 360°
> - Appearance: hitam (ban) + abu-abu (hub)
> - Buat versi kiri dan kanan (Mirror)
>
> **Output:** File `.f3d` component `Motor_DC_Gearbox` dan `Roda` (referensi assembly — tidak diproduksi).

### Soal 3 — Motor Mount Custom (PLA, 3D Print)

> Motor DC TT yang digunakan tidak memiliki lubang mounting standar. Desain motor mount custom:
>
> - Bentuk: L-shape atau U-clamp
> - Genggam body motor dengan pocket atau clamp
> - 2× lubang M3 mounting ke chassis bawah
> - Material: **PLA 3D print**, tebal dinding ≥ 4 mm
> - Tambahkan **rib penguat** (tulang) pada sudut dalam
> - Tersedia versi kiri dan kanan (**Mirror feature** di Fusion 360)
> - **Wajib ukur motor aktual** dengan penggaris sebelum desain
>
> **Output:** `Motor_Mount_L.stl` dan `Motor_Mount_R.stl` siap 3D print.

### Soal 4 — Dudukan 3 Sensor Jarak HC-SR04 (Akrilik, Sheet Metal, Laser Cut + Bending)

> Robot dilengkapi 3 sensor ultrasonik HC-SR04 untuk deteksi rintangan dari tiga arah. Desain dudukan menggunakan **metode Sheet Metal di Fusion 360** — akrilik kemudian **di-bending secara aktual** menggunakan heat gun.
>
> **Konfigurasi sudut sensor:**
>
> ```
>            [DEPAN 0°]
>                 |
>    [Kiri 20-25°]|[Kanan 20-25°]
>          \      |      /
>           \     |     /
>        ----[CHASSIS]----
>
>   Depan : 0° — lurus ke depan
>   Kiri  : 20–25° dari arah maju ke kiri
>   Kanan : 20–25° dari arah maju ke kanan (mirror dari kiri)
> ```
>
> **Alasan sudut 20–25°:** Sudut efektif HC-SR04 adalah < 15°. Sudut > 45° menyebabkan pantulan gelombang tidak kembali ke penerima dengan akurat. Sudut 20–25° memberikan coverage ke sisi tanpa mengorbankan akurasi pembacaan jarak.
>
> Spesifikasi tiap bracket:
> - Material: **akrilik 3 mm** (laser cut + bending)
> - Metode desain: **Sheet Metal workspace** → Flat Pattern (dengan bend line) → Export DXF → laser cut → heat gun bending
> - Bracket depan: **1 piece** L-shape, tekukan 90°, sensor 0° (lurus)
> - Bracket kiri/kanan: **1 piece** dengan tekukan 65–70° per bracket, sudut sensor 20–25°
> - Fillet ≥ R1.5 mm pada semua sudut dalam lubang (cegah retak akrilik)
> - K-factor: 0.33 di Sheet Metal Rules
>
> **Output:** `Bracket_Sensor_Depan.dxf`, `Bracket_Sensor_Kiri.dxf`, `Bracket_Sensor_Kanan.dxf` — semua siap laser cut + bending.

### Soal 5 — Dudukan PCB MAIN + Cover PCB Sensor Garis (PLA, 3D Print)

> **Standoff PCB MAIN (×4):**
> - Silinder berlubang: outer Ø7 mm, inner Ø3.2 mm
> - Tinggi: ukur komponen tertinggi di PCB MAIN + 5 mm clearance (minimal 15 mm)
> - Flange bawah: Ø12 mm, tebal 2 mm
> - Posisi: sesuai 4 lubang mounting PCB MAIN yang diukur aktual dengan penggaris
>
> **Bracket + Cover PCB Sensor Garis:**
> - L-bracket untuk menahan PCB sensor pada ketinggian 5–10 mm dari lantai (slot adjustable)
> - **Cover/enclosure 3D print** yang menutupi PCB sensor dari atas:
>   - Dimensi dalam: panjang PCB + 1 mm × lebar PCB + 1 mm
>   - Tinggi dinding: setinggi komponen tertinggi + 2 mm
>   - Bagian bawah: terbuka (sensor menghadap lantai)
>   - Slot kabel pada sisi
> - Material: **PLA 3D print**, tebal dinding 2 mm
>
> **Output:** `Standoff_PCB.stl` (×4), `Bracket_Sensor_Garis.stl`, `Cover_PCB_Sensor.stl`.

### Soal 6 — Dudukan Servo SG90 + Dudukan Baterai + Model 3D Baterai (PLA, 3D Print)

> **Dudukan Servo SG90:**
> - Bentuk U-shape / channel
> - Pocket sesuai body servo aktual + 0.3 mm clearance
> - Lubang flange Ø2.2 mm (M2) di kedua sisi samping
> - 2× lubang M3 mounting ke chassis atas
> - Slot terbuka ke atas agar servo mudah dipasang/dilepas
> - Material: **PLA 3D print**, infill ≥ 40%
> - **Wajib ukur SG90 aktual** (body + flange + jarak lubang) dengan penggaris
>
> **Dudukan Baterai 2×18650:**
> - 2 rongga silinder paralel, diameter dalam = diameter aktual baterai + 0.5 mm
> - Panjang rongga = panjang aktual baterai + 3 mm
> - Dinding luar ≥ 2 mm, dinding pemisah antar sel ≥ 2 mm
> - Retaining lip di satu ujung (bibir 2 mm)
> - Channel kabel (slot 12 × 5 mm)
> - 4× lubang M3 mounting ke chassis atas
> - Material: **PLA 3D print**, infill ≥ 40%
>
> **Model 3D Baterai 18650 (Referensi Assembly):**
> - Component: `Baterai_18650` — silinder Ø aktual × panjang aktual
> - Positive cap (+): tonjolan Ø5 mm, tinggi 1.5 mm
> - Appearance: biru/hijau sesuai baterai fisik
> - Buat 2 unit untuk assembly
>
> **Output:** `Dudukan_Servo_SG90.stl`, `Dudukan_Baterai_18650.stl`, `Baterai_18650.f3d` (referensi).

### Soal 7 — Aksesoris 3D Bebas (PLA, 3D Print — Minimum 2 Buah)

> Desain minimal **2 aksesoris 3D** untuk mempercantik dan menambah fungsi robot. Desain bebas dan kreatif.
>
> **Contoh aksesoris (pilih minimal 2):**
> - Spoiler / finn belakang (estetika, aerodinamika)
> - Cover dekoratif di sekitar sensor HC-SR04 (pelindung + estetika)
> - Shroud / diffuser roda
> - Name plate dengan teks timbul (identitas tim)
> - Bumper depan (pelindung benturan)
> - LED mount untuk LED hias
>
> **Persyaratan:**
> - Tidak menghalangi sensor (jaga clearance ≥ 15° dari arah sensor)
> - Tidak menambah berat berlebihan (desain hollow jika perlu, infill 15–20%)
> - Setiap aksesoris harus ter-joint di assembly dengan benar tanpa interference
> - Export masing-masing sebagai STL
>
> **Output:** `Aksesoris_[Nama1].stl`, `Aksesoris_[Nama2].stl` (dan lebih jika ada).

### Soal 8 — Desain Gripper SG90 (PLA, 3D Print — Parallel Jaw)

> Desain **gripper parallel jaw** yang digerakkan oleh **1× servo SG90** (sudah ada di robot). Gripper dipasang di dinding depan Chassis_Box dan berfungsi menjepit benda berukuran ~5 cm.
>
> **Spesifikasi:**
> - Servo: SG90 (1 buah, sudah ada)
> - Bukaan jaw maksimum: **60 mm** (grip benda ~50 mm)
> - Bukaan jaw minimum: **10 mm** (cengkeram penuh)
> - Mekanisme: servo horn → Linkage → pivot jaw → jaw kiri/kanan bergerak simetris
> - Material: **PLA 3D print**
> - Posisi: dinding depan Chassis_Box
>
> **Komponen (4 buah):**
> - `Gripper_Base`: mount bracket ke Chassis_Box + slot dudukan servo SG90
> - `Gripper_Jaw_L`: rahang kiri L-shape (60 mm arm, ujung finger-grip R3 mm)
> - `Gripper_Jaw_R`: mirror dari Jaw_L
> - `Gripper_Linkage`: batang penghubung servo horn ke pivot jaw (~30 mm)
>
> **Joint di assembly:**
> - Jaw ke base: **Revolute Joint**
> - Linkage ke servo horn: **Revolute Joint**
> - Linkage ke jaw: **Revolute Joint**
> - Animasikan: SG90 0° = jaw terbuka 60 mm, SG90 90° = jaw menutup
>
> **Interference Check**: 0 interference pada seluruh rentang gerak.
>
> **Output:** `Gripper_Base.stl`, `Gripper_Jaw_L.stl`, `Gripper_Jaw_R.stl`, `Gripper_Linkage.stl`

### Soal 9 — Assembly Lengkap, Integrasi PCB, dan Verifikasi

> Semua komponen dirakit dalam satu file assembly Fusion 360:
>
> - **Import PCB MAIN** dari EasyEDA (STEP/DXF) → component `PCB_Main`
> - **Import PCB Sensor** dari EasyEDA (STEP/DXF) → component `PCB_Sensor`
> - **Joint** seluruh komponen (chassis, motor ref, roda ref, baterai ref, mount, bracket, gripper, PCB, aksesoris):
>   - Roda kiri + kanan: **Revolute joint** (bisa berputar)
>   - Motor DC, baterai: Rigid joint (referensi posisi)
>   - Gripper jaw kiri + kanan: **Revolute joint**
>   - Semua bracket, dudukan, PCB, aksesoris: **Rigid joint**
> - **Interference Check** → harus **0 interference** (termasuk gripper pada semua posisi)
> - **Section Analysis** dari atas:
>   - Clearance PCB MAIN ke dinding atas Chassis_Box
>   - Clearance sensor garis dari lantai (5–10 mm)
>   - Sudut sensor kiri/kanan 20–25° terlihat dari pandangan atas
> - **Rendering** dari 3 sudut: isometric, top view, front view
>
> **Output:** File `.f3d` assembly lengkap, 3 screenshot rendering, screenshot interference (0), screenshot section analysis.

---

## C. DELIVERABLES

| No | Deliverable | Format | Metode Produksi | Keterangan |
|----|------------|--------|----------------|----------|
| 1 | File Fusion 360 assembly lengkap | `.f3d` | — | Semua component + joint |
| 2 | **Chassis Box single-piece (Sheet Metal)** | `.f3d` | **Referensi** — chassis metal sudah jadi | Flat Pattern terverifikasi |
| 3 | Motor DC Gearbox (model ref) | `.f3d` | **Referensi** — tidak diproduksi | Dimensi sesuai aktual |
| 4 | Roda kiri + kanan (model ref) | `.f3d` | **Referensi** — tidak diproduksi | Revolve dari profil aktual |
| 5 | Motor Mount Kiri | `.stl` | **3D print PLA** | Sesuai dimensi motor aktual |
| 6 | Motor Mount Kanan | `.stl` | **3D print PLA** | Mirror dari kiri |
| 7 | Bracket Sensor Depan | `.dxf` | **Laser cut + bending akrilik 3 mm** | Sheet Metal Flat Pattern, 0° |
| 8 | Bracket Sensor Kiri | `.dxf` | **Laser cut + bending akrilik 3 mm** | 20–25°, 1 piece |
| 9 | Bracket Sensor Kanan | `.dxf` | **Laser cut + bending akrilik 3 mm** | Mirror kiri, 20–25° |
| 10 | Standoff PCB MAIN × 4 | `.stl` | **3D print PLA** | Tinggi sesuai PCB aktual |
| 11 | Bracket PCB Sensor Garis | `.stl` | **3D print PLA** | Slot adjustable height |
| 12 | Cover/Enclosure PCB Sensor Garis | `.stl` | **3D print PLA** | Menutupi PCB sensor dari atas |
| 13 | Dudukan Servo SG90 | `.stl` | **3D print PLA** | Infill ≥ 40% |
| 14 | Dudukan Baterai 2×18650 | `.stl` | **3D print PLA** | Infill ≥ 40% |
| 15 | Baterai 18650 (model ref) | `.f3d` | **Referensi** — tidak diproduksi | 2 unit, dimensi aktual |
| 16 | Aksesoris 3D #1 | `.stl` | **3D print PLA** | Desain bebas |
| 17 | Aksesoris 3D #2 | `.stl` | **3D print PLA** | Desain bebas |
| 18 | **Gripper_Base** | `.stl` | **3D print PLA** | Mount + dudukan servo SG90 |
| 19 | **Gripper_Jaw_L** | `.stl` | **3D print PLA** | Rahang kiri, 60 mm arm |
| 20 | **Gripper_Jaw_R** | `.stl` | **3D print PLA** | Rahang kanan, mirror kiri |
| 21 | **Gripper_Linkage** | `.stl` | **3D print PLA** | Penghubung servo horn–jaw |
| 22 | Screenshot assembly (3 sudut) | `.png` | — | Isometric, Top, Front |
| 23 | Screenshot interference check | `.png` | — | Harus menunjukkan 0 interference |
| 24 | Screenshot section analysis | `.png` | — | Sudut sensor 20–25° terlihat |
| 25 | Gerber ZIP PCB MAIN | `.zip` | **Order JLCPCB** | Terverifikasi di Gerber Viewer |
| 26 | Gerber ZIP PCB Sensor | `.zip` | **Order JLCPCB** | Terverifikasi di Gerber Viewer |
| 27 | Screenshot order JLCPCB | `.png` | — | Nomor order PCB terlihat |
---

## D. RUBRIK PENILAIAN PROJECT (100 POIN)

| No | Kriteria | Bobot | Indikator |
|----|---------|-------|-----------|
| 1 | **Box chassis single-piece** (Sheet Metal workspace), Flat Pattern valid, dimensi aktual | 10 | 1 komponen Chassis_Box, Flat Pattern terverifikasi |
| 2 | Model 3D Motor DC + Roda sebagai referensi assembly, dimensi aktual | 8 | Component ada di assembly, dimensi sesuai ukuran |
| 3 | Motor mount L + R (PLA), dimensi sesuai motor aktual yang diukur | 8 | STL valid, motor pas di pocket |
| 4 | Bracket 3 sensor HC-SR04 (akrilik, bending): depan 0°, kiri/kanan 20–25°, 1 piece per bracket | 12 | DXF Flat Pattern valid + bend line, sudut benar |
| 5 | Standoff PCB MAIN × 4 + Bracket sensor garis adjustable + Cover PCB Sensor (3D print) | 10 | STL valid, tinggi sesuai PCB aktual, cover fit |
| 6 | Dudukan servo + Dudukan baterai (3D print) + Model 3D baterai (referensi) | 8 | STL valid, clearance pas, baterai ref ada |
| 7 | Aksesoris 3D bebas (min. 2 buah): kreatif, 3D print, tidak interference | 8 | STL valid, terpasang di assembly, 0 interference |
| 8 | **Gripper SG90**: 4 komponen (Base+Jaw_L+Jaw_R+Linkage), joint teranimasi 0–60 mm, 0 interference | 10 | 4 STL valid, Revolute Joint benar, animasi OK |
| 9 | Assembly lengkap: 0 interference (semua komponen + gripper), section analysis, joint benar | 13 | Screenshot terlampir, joint correct, gripper OK |
| 10 | PCB MAIN + PCB Sensor terintegrasi, fitment dan clearance Ok | 6 | Joint dan clearance benar |
| 11 | Gerber PCB MAIN + PCB Sensor terverifikasi dan diorder ke JLCPCB | 7 | Screenshot order terlampir |
| **TOTAL** | | **100** | |

---

## E. REFERENSI

1. Autodesk Fusion 360 Documentation: https://help.autodesk.com/view/fusion360/ENU/
2. Autodesk Sheet Metal: https://help.autodesk.com/view/fusion360/ENU/?guid=SHT-OVERVIEW
3. HC-SR04 Datasheet: https://components101.com/sensors/ultrasonic-sensor-working-pinout-datasheet
4. SG90 Servo Datasheet — TowerPro Official: https://www.towerpro.com.tw/product/sg90-7/
5. JLCPCB Manufacturing Capabilities: https://jlcpcb.com/capabilities
6. LightBurn Laser Software: https://lightburnsoftware.com/
7. Ultimaker Cura Slicer: https://ultimaker.com/software/ultimaker-cura/
