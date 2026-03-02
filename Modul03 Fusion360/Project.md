# PROJECT MODUL 03: DESAIN MEKANIK LENGKAP ROBOT LINE FOLLOWER SIAP PRODUKSI

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 03 – Fusion 360 Desain Mekanik  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. DESKRIPSI PROJECT

### Soal Cerita

> Tim robotik kampus Anda dipercaya merancang **Robot Line Follower** untuk kompetisi antar-universitas. Panitia memberikan spesifikasi khusus: robot harus bisa mengikuti garis hitam di atas latar putih dengan kecepatan tinggi, mampu berbelok tajam, dan bisa menghindari rintangan yang diletakkan secara acak di jalur. Robot harus **modular** — artinya chassis, sensor board, dan motor harus bisa dibongkar-pasang tanpa merusak komponen lain.
>
> Sponsor kompetisi menyediakan layanan JLCPCB untuk memproduksi PCB dan layanan 3D printing untuk bracket. **Deadline desain mekanik: 2 minggu sebelum kompetisi**. Kelompok Anda harus menghasilkan desain Fusion 360 yang lengkap, dapat difabrikasi, dan terintegrasi dengan PCB yang sudah didesain di Modul 01.
>
> **Tantangan tambahan:** Panitia akan memberikan **poin bonus** bagi tim yang menambahkan fitur unik pada robot — misalnya mekanisme pengangkat objek menggunakan servo, bumper pelindung sensor, atau sistem quick-release baterai.

---

## B. SOAL PROJECT

### Soal 1 — Chassis Modular Dua Tingkat

> Sang kapten tim menginginkan chassis **dua tingkat** yang bisa dibongkar-pasang. Deck bawah menampung motor dan sensor garis, deck atas menampung PCB MAIN, baterai, dan OLED.
>
> Tentukan dimensi chassis (minimal 150 × 120 mm), desain kedua deck dengan lubang mounting M3, dan hubungkan keduanya menggunakan spacer. Pastikan ada lubang routing kabel antar-deck. Desain harus bisa di-laser cut dari akrilik 3 mm.
>
> **Output:** File Fusion 360 dengan 2 component chassis + 4 spacer, file DXF untuk laser cutting.

### Soal 2 — Motor Mount Custom

> Motor yang digunakan adalah DC TT dengan gearbox. Karena motor ini tidak memiliki lubang mounting standar, kapten meminta Anda mendesain **motor mount custom** yang:
> - Menggenggam body motor dengan clamp atau pocket
> - Memiliki lubang mounting M3 ke chassis bawah
> - Dibuat dari PLA (3D print), tebal minimal 4 mm
> - Tersedia versi kiri dan kanan (mirror)
>
> Jika motor bergetar saat kecepatan penuh, mount harus tetap kokoh. Pertimbangkan penambahan rib (tulang penguat) pada desain.
>
> **Output:** 2 file STL motor mount (kiri & kanan).

### Soal 3 — Sensor Board Bracket dengan Adjustable Height

> Praktisi berpengalaman menyarankan bahwa **ketinggian sensor line dari lantai sangat mempengaruhi pembacaan**. Jarak ideal adalah 5–10 mm, tapi tiap arena berbeda.
>
> Desain bracket sensor yang memungkinkan **pengaturan ketinggian** — misalnya menggunakan slot vertikal (bukan lubang bulat) agar sensor bisa digeser naik-turun dan dikunci dengan baut M3.
>
> **Output:** 2 file STL bracket sensor (kiri & kanan), dokumentasi range ketinggian adjustable.

### Soal 4 — Mekanisme Servo (Fitur Bonus)

> Panitia menempatkan objek kecil (kubus 3 cm) secara acak di jalur. Robot yang bisa **memindahkan objek ke sisi jalur** mendapat poin bonus. Desain sebuah mekanisme berbasis servo SG90 yang bisa mendorong atau mengangkat objek keluar jalur.
>
> Mekanisme harus:
> - Terpasang di depan robot (dekat sensor ultrasonik HC-SR04)
> - Menggunakan maksimal 2 servo SG90
> - Tidak menghalangi pembacaan sensor garis
> - Bisa dilipat saat tidak digunakan
>
> **Output:** File STL mekanisme servo, penjelasan kinematika gerakan.

### Soal 5 — Assembly Lengkap dan Verifikasi

> Seluruh komponen harus di-assembly dalam satu file Fusion 360. Lakukan:
> - Joint seluruh komponen (chassis, mount, bracket, PCB, motor, roda)
> - Interference check → 0 interference
> - Section analysis → verifikasi clearance
> - Tampilkan rendering realistis dari 3 sudut pandang
> - Buat **exploded view** (animation workspace) untuk panduan assembly
>
> **Output:** File Fusion 360 assembly lengkap, 3 screenshot rendering, 1 video/GIF exploded view.

---

## C. DELIVERABLES

| No | Deliverable | Format | Keterangan |
|----|------------|--------|-----------|
| 1 | File Fusion 360 lengkap | .f3d | Semua component + assembly |
| 2 | Chassis Bottom & Top | .dxf | Siap laser cut |
| 3 | Motor Mount (L + R) | .stl | Siap 3D print |
| 4 | Sensor Bracket (L + R) | .stl | Adjustable height |
| 5 | Servo Mechanism | .stl | Fitur bonus |
| 6 | Spacer × 4 | .stl | Siap 3D print |
| 7 | Screenshot assembly | .png | 3 sudut pandang |
| 8 | Screenshot interference | .png | 0 interference |
| 9 | Gerber ZIP PCB MAIN | .zip | Terverifikasi di JLCPCB viewer |
| 10 | Gerber ZIP PCB Sensor | .zip | Terverifikasi di JLCPCB viewer |
| 11 | Bukti order JLCPCB | Screenshot | Nomor order terlihat |

---

## D. RUBRIK PENILAIAN PROJECT (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | Desain chassis modular (2 deck + spacer) sesuai spesifikasi | 20 |
| 2 | Motor mount kokoh dan sesuai dimensi motor | 15 |
| 3 | Sensor bracket dengan adjustable height | 15 |
| 4 | Assembly lengkap, 0 interference | 15 |
| 5 | File produksi lengkap (STL + DXF) dan valid | 10 |
| 6 | Mekanisme servo (fitur bonus) | 10 |
| 7 | Gerber terverifikasi dan PCB terorder di JLCPCB | 10 |
| 8 | Dokumentasi dan kerapian | 5 |
| **TOTAL** | | **100** |

---

## E. REFERENSI

1. Autodesk Fusion 360 Documentation: https://help.autodesk.com/view/fusion360/ENU/
2. JLCPCB Manufacturing Capabilities: https://jlcpcb.com/capabilities
3. 3D Printing Design Guide: https://www.hubs.com/knowledge-base/how-design-parts-fdm-3d-printing/
4. Laser Cutting Design Guide: https://www.ponoko.com/blog/how-to-make/how-to-design-for-laser-cutting/
