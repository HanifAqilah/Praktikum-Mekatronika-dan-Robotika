# TUGAS VIDEO MODUL 01: PERANCANGAN PCB DARI SKEMATIK HINGGA PRODUKSI

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 01 – Desain PCB  
**Software:** EasyEDA Standard / Pro  
**Manufaktur:** JLCPCB  
**Batas Upload:** Sesuai jadwal dosen  
**Durasi Video:** 10–20 menit  
**Format:** MP4, resolusi minimal HD 720p  
**Platform Upload:** Google Drive / YouTube (unlisted) / LMS

---

## A. KETENTUAN UMUM

> ⚠️ **TUGAS VIDEO DIKERJAKAN SECARA INDIVIDUAL (PER ORANG)**

1. Setiap mahasiswa wajib membuat **video sendiri-sendiri** (bukan per kelompok)
2. Video menampilkan **proses desain PCB milik Anda sendiri** menggunakan EasyEDA
3. Desain yang direkam boleh bagian dari proyek kelompok (PCB MAIN atau PCB Sensor Line yang menjadi tanggung jawab Anda)
4. Tampilkan **nama lengkap dan NIM** di awal video
5. Gunakan **screen recording** yang jelas — proses nyata, bukan slideshow
6. Penjelasan dalam **Bahasa Indonesia** yang baik dan benar
7. Setiap poin yang dijelaskan harus didukung dengan **demonstrasi di EasyEDA**

---

## B. POIN-POIN YANG HARUS DIJELASKAN DALAM VIDEO

---

### BAGIAN 1: PENDAHULUAN – KONSEP DASAR PCB (Maks. 3 menit)

**Yang harus dijelaskan (tanpa harus menunjukkan EasyEDA):**

- [ ] **1.1** Definisi PCB dan fungsinya dalam sistem elektronika (robot, IoT, dsb.)
- [ ] **1.2** Perbedaan Single Layer, Double Layer, dan Multilayer — kapan masing-masing digunakan
- [ ] **1.3** Material FR4: apa itu, karakteristiknya, mengapa dipakai di JLCPCB
- [ ] **1.4** Ketebalan copper (oz): perbedaan 1 oz dan 2 oz, pengaruh terhadap kapasitas arus
- [ ] **1.5** Perbedaan antara Skematik (logika) dan Layout PCB (fisik)
- [ ] **1.6** Alur kerja desain PCB: dari spesifikasi → skematik → layout → Gerber → JLCPCB

*Panduan: Jelaskan seolah mengajar teman yang belum mengenal PCB sama sekali. Gunakan analogi jika membantu.*

---

### BAGIAN 2: PENGENALAN EASYEDA (Maks. 2 menit)

**Yang harus ditunjukkan (tampilkan layar EasyEDA):**

- [ ] **2.1** Penjelasan EasyEDA: apa itu, mengapa dipakai, hubungannya dengan LCSC dan JLCPCB
- [ ] **2.2** Perbedaan EasyEDA Standard (browser) vs EasyEDA Pro (desktop)
- [ ] **2.3** Tampilkan antarmuka EasyEDA: menu bar, panel library kiri, canvas, panel properti kanan
- [ ] **2.4** Demonstrasikan fitur **Half Offline Mode** (Settings > Offline Mode) — jelaskan fungsinya
- [ ] **2.5** Tunjukkan minimal **5 fitur EasyEDA** dari daftar berikut dengan membuka menu-nya:
  - Schematic Editor, PCB Editor
  - LCSC Library (panel kiri)
  - ERC, DRC
  - Net Inspector / Design Manager
  - Auto Router
  - Copper Area
  - 3D View, Photo View
  - Gerber Export / Order at JLCPCB
  - Symbol Wizard / Footprint Manager

---

### BAGIAN 3: SPESIFIKASI PROYEK (Maks. 1 menit)

**Yang harus dijelaskan:**

- [ ] **3.1** Judul dan deskripsi PCB yang Anda desain (PCB MAIN atau PCB Sensor Line)
- [ ] **3.2** Tegangan input, tegangan output, arus maksimal
- [ ] **3.3** Jumlah layer (2-layer) dan alasannya
- [ ] **3.4** Target dimensi board (mm × mm)
- [ ] **3.5** Target pabrikan: **JLCPCB** — mengapa JLCPCB dipilih

---

### BAGIAN 4: MEMBUAT SKEMATIK DI EASYEDA (Maks. 4 menit)

**Yang harus ditunjukkan (tampilkan layar EasyEDA Schematic Editor):**

- [ ] **4.1** Membuat project baru dan schematic baru (tampilkan langkah File > New > Project/Schematic)
- [ ] **4.2** Mencari dan menambahkan komponen dari **library LCSC** (demo pencarian, pilih komponen, lihat info stok+harga)
- [ ] **4.3** Menambahkan komponen dari **library EasyEDA** (tab EasyEDA di panel kiri)
- [ ] **4.4** Demonstrasikan **Symbol Wizard** atau **Edit Symbol** untuk membuat/modifikasi simbol kustom
- [ ] **4.5** Menyimpan komponen ke **My Library**
- [ ] **4.6** Penggunaan **Wire** (`W`) untuk menghubungkan pin
- [ ] **4.7** Penggunaan **Net Label** (`N`) untuk koneksi yang jauh — jelaskan convention penamaan
- [ ] **4.8** Penggunaan **Power Port** (`P`) untuk VCC, GND, +5V — jelaskan bedanya dengan net label biasa
- [ ] **4.9** Penggunaan **No Connect** mark untuk pin yang tidak digunakan
- [ ] **4.10** Menjalankan **ERC** (Design > ERC) — tampilkan error yang muncul, jelaskan penyebab, dan perbaiki

---

### BAGIAN 5: FOOTPRINT DAN PACKAGING (Maks. 2 menit)

**Yang harus dijelaskan dan ditunjukkan:**

- [ ] **5.1** Perbedaan SMD dan Through Hole — kapan masing-masing digunakan di proyek ini
- [ ] **5.2** Sebutkan minimal 3 package yang digunakan (contoh: 0805, SOIC-16, TO-263-5) dan jelaskan alasan pemilihan
- [ ] **5.3** Demonstrasikan **Footprint Manager** (Tools > Footprint Manager) — tunjukkan daftar komponen dan footprint-nya
- [ ] **5.4** Tunjukkan cara mengubah footprint komponen via Footprint Manager
- [ ] **5.5** Demonstrasikan membuat **footprint kustom** (Tools > New Footprint) — minimal tunjukkan langkah Place > Pad dan atur ukuran

---

### BAGIAN 6: KONVERSI KE PCB LAYOUT (Maks. 1 menit)

**Yang harus ditunjukkan:**

- [ ] **6.1** Proses **Update PCB** dari skematik (Design > Update PCB) — tampilkan hasil footprint + ratsnest
- [ ] **6.2** Membuat **Board Outline** (Place > Board Outline > Rect) — tunjukkan dimensi yang dipilih
- [ ] **6.3** Penjelasan layer PCB di EasyEDA: Top/Bottom Cu, SilkS, Mask, Board Outline (panel layer)
- [ ] **6.4** Setup **Design Rules** (Design > Design Rule) — tampilkan nilai yang diisi dan kaitkan dengan kemampuan JLCPCB

---

### BAGIAN 7: PLACEMENT DAN ROUTING PCB (Maks. 4 menit)

**Yang harus ditunjukkan (tampilkan PCB Editor):**

- [ ] **7.1** Strategi placement: tunjukkan posisi komponen utama dan jelaskan alasannya (konektor di tepi, decoupling cap dekat IC, power section terpisah)
- [ ] **7.2** **Perhitungan track width** untuk jalur power — tampilkan kalkulator online JLCPCB atau perhitungan manual
- [ ] **7.3** Demonstrasikan **routing manual** (Route > Route Single Track / `W`) — tunjukkan sudut 45°, bukan 90°
- [ ] **7.4** Tunjukkan cara **ganti layer** saat routing dengan `Space` → via otomatis terbentuk
- [ ] **7.5** Tunjukkan penggunaan **Interactive Router** (mode Walk Around/Shove)
- [ ] **7.6** Demonstrasikan **Auto Router** (Route > Auto Router) — jalankan, tampilkan hasilnya, jelaskan kelebihan dan kekurangannya
- [ ] **7.7** Demonstrasikan pembuatan **Copper Area** (Place > Copper Area / `E`) untuk ground plane:
  - Pilih layer B.Cu
  - Set Net = GND
  - Aktifkan Thermal Relief
  - Rebuild Copper Area
- [ ] **7.8** Demonstrasikan **Highlight Net** (klik trace) dan **Cross Probe** (klik komponen di schematic → cek di PCB)
- [ ] **7.9** Buka **Net Inspector** (Design > Net Inspector) — tunjukkan status routing semua net

---

### BAGIAN 8: DRC (DESIGN RULE CHECK) (Maks. 2 menit)

**Yang harus ditunjukkan:**

- [ ] **8.1** Menjalankan **DRC** (Design > DRC > Run DRC Check)
- [ ] **8.2** Tampilkan error yang muncul (screenshot sebelum diperbaiki)
- [ ] **8.3** Demonstrasikan cara membaca error: klik error di panel → canvas zoom ke lokasi
- [ ] **8.4** Perbaiki minimal **1 error secara live** (rekam prosesnya)
- [ ] **8.5** Tampilkan **DRC result akhir** (0 error kritis)
- [ ] **8.6** Buka **Design Manager** (Design > Design Manager) dan jelaskan overview yang ditampilkan

---

### BAGIAN 9: SILKSCREEN, PHOTO VIEW, DAN 3D VIEW (Maks. 2 menit)

**Yang harus ditunjukkan:**

- [ ] **9.1** Tampilkan silkscreen PCB: reference designator, label konektor, versi board, nama kelompok
- [ ] **9.2** Demonstrasikan cara menambah teks di silkscreen (Place > Text, layer F.SilkS)
- [ ] **9.3** Buka **Photo View** (View > Photo View) — jelaskan apa yang ditampilkan dan kegunaannya
- [ ] **9.4** Buka **3D View** (View > 3D View) — tampilkan dari sudut atas dan isometrik
- [ ] **9.5** Komentari kondisi board dari 3D View: orientasi komponen, tinggi, clearance

---

### BAGIAN 10: GENERATE GERBER, BOM, DAN PICK & PLACE (Maks. 2 menit)

**Yang harus ditunjukkan:**

- [ ] **10.1** Buka dialog **Fabrication > PCB Fabrication File (Gerber)** — jelaskan layer yang diekspor
- [ ] **10.2** Klik **Generate Gerber** → tampilkan file ZIP yang ter-download
- [ ] **10.3** Buka **Fabrication > BOM** — tampilkan kolom LCSC Part Number yang penting
- [ ] **10.4** Klik **Export BOM** → tampilkan file CSV hasil
- [ ] **10.5** Buka **Fabrication > Pick and Place File** — jelaskan fungsinya untuk JLCPCB SMT Assembly
- [ ] **10.6** Jelaskan perbedaan antara hanya order PCB vs order PCB + SMT Assembly di JLCPCB

---

### BAGIAN 11: VALIDASI GERBER DAN ORDER JLCPCB (Maks. 2 menit)

**Yang harus ditunjukkan:**

- [ ] **11.1** Upload ZIP Gerber ke **https://gerber-viewer.jlcpcb.com** — tampilkan prosesnya
- [ ] **11.2** Cek setiap layer di Gerber Viewer (top copper, bottom copper, soldermask, silkscreen, outline, drill)
- [ ] **11.3** Buka **Fabrication > Order at JLCPCB** dari EasyEDA — tampilkan bahwa file terupload otomatis
- [ ] **11.4** Di halaman JLCPCB, tampilkan dan jelaskan setiap parameter:
  - PCB Thickness: 1.6 mm
  - PCB Color: Green
  - Surface Finish: HASL Lead-free — jelaskan bedanya dengan ENIG
  - Copper Weight: 1 oz
  - Quantity: 5 pcs
- [ ] **11.5** Tampilkan **estimasi harga** dan **estimasi waktu produksi**

---

## C. RUBRIK PENILAIAN VIDEO

**Total Nilai: 100 poin**

---

### C.1 Kelengkapan Konten (40 poin)

| No | Bagian | Bobot | Skor (0–10) | Nilai |
|----|--------|-------|------------|-------|
| 1 | Bag. 1 – Konsep dasar PCB (jenis, material, copper, alur kerja) | 5 | | |
| 2 | Bag. 2 – Pengenalan EasyEDA (fitur, antarmuka, half offline) | 4 | | |
| 3 | Bag. 3 – Spesifikasi proyek | 2 | | |
| 4 | Bag. 4 – Skematik EasyEDA (LCSC search, wire, net label, ERC) | 8 | | |
| 5 | Bag. 5 – Footprint dan packaging (Footprint Manager, custom FP) | 4 | | |
| 6 | Bag. 6 – Konversi ke PCB (Update PCB, board outline, design rules) | 3 | | |
| 7 | Bag. 7 – Routing (placement, manual route, auto, copper area, cross probe) | 7 | | |
| 8 | Bag. 8 – DRC dan Design Manager | 3 | | |
| 9 | Bag. 9 – Silkscreen, Photo View, 3D View | 2 | | |
| 10 | Bag. 10 – Gerber, BOM, Pick & Place export | 2 | | |
| 11 | Bag. 11 – Validasi Gerber + simulasi order JLCPCB | 0 | | |
| **TOTAL** | | **40** | | |

*Skor per bagian: 10 = sempurna; 7–9 = baik; 4–6 = cukup; 1–3 = kurang; 0 = tidak ada*

---

### C.2 Akurasi Teknis (30 poin)

| No | Kriteria | Bobot | Skor (0–10) | Nilai |
|----|---------|-------|------------|-------|
| 1 | Konsep PCB (material, copper weight, layer) dijelaskan **secara benar** | 6 | | |
| 2 | Skematik **benar secara elektris** — ERC bersih, koneksi logis tepat | 8 | | |
| 3 | Routing PCB **sesuai aturan**: sudut 45°, track width sesuai arus, ground plane | 8 | | |
| 4 | Fitur EasyEDA **digunakan dengan benar** (bukan hanya dibuka tapi tidak dipahami) | 8 | | |
| **TOTAL** | | **30** | | |

---

### C.3 Kualitas Presentasi (20 poin)

| No | Kriteria | Bobot | Skor (0–5) | Nilai |
|----|---------|-------|-----------|-------|
| 1 | Narasi jelas, tidak terlalu cepat/lambat, mudah dipahami | 5 | | |
| 2 | Kualitas rekaman layar — resolusi HD, tidak buram, terlihat detail EasyEDA | 5 | | |
| 3 | Struktur video — ada pembukaan, alur logis urut, ada penutup | 5 | | |
| 4 | Durasi sesuai (10–20 menit) — tidak terlalu singkat maupun terlalu panjang | 5 | | |
| **TOTAL** | | **20** | | |

---

### C.4 Demonstrasi Fitur EasyEDA (10 poin)

| No | Fitur yang Wajib Didemonstrasikan | Skor (0 atau 1) |
|----|----------------------------------|----------------|
| 1 | LCSC Library Search (cari komponen + lihat info) | |
| 2 | Symbol Wizard atau Edit Symbol | |
| 3 | My Library (simpan komponen) | |
| 4 | ERC (jalankan + perbaiki) | |
| 5 | Footprint Manager | |
| 6 | Update PCB dari Schematic | |
| 7 | Copper Area (Ground Plane) + Rebuild | |
| 8 | Auto Router (jalankan dan tampilkan hasil) | |
| 9 | DRC (jalankan + perbaiki + tampilkan bersih) | |
| 10 | Photo View dan 3D View | |
| **TOTAL** | | **/10** |

---

### C.5 Rangkuman Nilai

| Komponen | Bobot | Nilai Didapat |
|---------|-------|--------------|
| Kelengkapan Konten | 40 | |
| Akurasi Teknis | 30 | |
| Kualitas Presentasi | 20 | |
| Demonstrasi Fitur EasyEDA | 10 | |
| **TOTAL** | **100** | |

**Penilai:** ___________________  
**Tanggal:** ___________________  
**Catatan:**
```
_______________________________________________
```

---

## D. PANDUAN TEKNIS PEMBUATAN VIDEO

### D.1 Tools Screen Recording

| Tool | Platform | Link | Keterangan |
|------|---------|------|-----------|
| **OBS Studio** | Win/Mac/Linux | obsproject.com | Gratis, paling lengkap |
| Windows Game Bar | Windows | `Win+G` | Built-in, mudah |
| Loom | Browser/App | loom.com | Gratis (terbatas), langsung share |
| CapScreen / ShareX | Windows | — | Gratis, banyak fitur |

### D.2 Pengaturan Recording

| Aspek | Rekomendasi |
|-------|-------------|
| Resolusi | 1920×1080 (Full HD) atau minimal 1280×720 |
| Frame rate | 30 fps (cukup untuk screen recording) |
| Audio | Headset microphone — lebih bersih dari mic internal laptop |
| Format | MP4 (H.264) |
| Ukuran file maksimal | 2 GB |

### D.3 Tips Rekaman yang Baik

1. **Zoom in** ke area yang sedang dikerjakan di EasyEDA — gunakan `Ctrl+Scroll`
2. Aktifkan **Show Mouse Clicks** di OBS agar penonton tahu di mana Anda mengklik
3. **Tutup notifikasi sistem** sebelum merekam (Do Not Disturb mode)
4. **Jelaskan sebelum melakukan** — beri tahu apa yang akan diklik sebelum mengkliknya
5. Saat menampilkan dialog/menu EasyEDA, **perlambat sedikit** agar penonton bisa membaca
6. Jika salah, **jangan dipotong** — jelaskan error yang terjadi dan cara memperbaikinya (ini justru nilai tambah)

### D.4 Struktur Video yang Disarankan

```
[0:00–0:20]  Opening: Nama, NIM, nama board yang didesain
[0:20–3:20]  Bagian 1: Teori dasar PCB
[3:20–5:20]  Bagian 2–3: EasyEDA overview + spesifikasi
[5:20–9:20]  Bagian 4–5: Skematik + footprint
[9:20–13:20] Bagian 6–7: Layout + routing + copper area
[13:20–15:20] Bagian 8–9: DRC + silkscreen + 3D + Photo View
[15:20–17:20] Bagian 10–11: Gerber export + order JLCPCB
[17:20–18:00] Penutup: ringkasan singkat
```

---

## E. CHECKLIST SEBELUM SUBMIT

**Konten:**
- [ ] Semua 11 bagian ada dalam video (cek menggunakan daftar di atas)
- [ ] Nama dan NIM tampil di awal video
- [ ] Minimal 5 fitur EasyEDA didemonstrasikan (bukan hanya disebutkan)
- [ ] ERC bersih dari error ditampilkan
- [ ] DRC bersih dari error ditampilkan
- [ ] Gerber berhasil di-generate dan ditampilkan di Gerber Viewer
- [ ] Halaman order JLCPCB ditampilkan dengan estimasi harga

**Kualitas:**
- [ ] Resolusi minimal HD 720p
- [ ] Audio jelas tanpa noise berlebihan
- [ ] Durasi 10–20 menit
- [ ] Teks/angka di layar EasyEDA bisa terbaca

**File:**
- [ ] Format MP4
- [ ] Nama file: `Video_PCB_[NIM]_[NamaLengkap].mp4`
- [ ] Diupload ke platform yang ditentukan dosen sebelum batas waktu

---

*Video yang tidak memenuhi minimal 70% kelengkapan konten tidak akan dinilai.*  
*Tugas video dikerjakan secara INDIVIDUAL — tidak boleh bergabung dengan anggota kelompok lain.*
