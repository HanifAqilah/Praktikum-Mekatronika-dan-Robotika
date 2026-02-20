# JOBSHEET MODUL 01: PERANCANGAN PCB DARI SKEMATIK HINGGA PRODUKSI

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 01 – Desain PCB  
**Software:** EasyEDA Standard / Pro  
**Manufaktur:** JLCPCB  
**Pertemuan:** 1–2 (2 × 2 SKS)  
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

1. Menjelaskan konsep dasar PCB, jenis, material, dan fungsinya dalam sistem elektronika
2. Menggunakan software EasyEDA untuk membuat skematik dan layout PCB secara terintegrasi
3. Memanfaatkan library LCSC yang terintegrasi di EasyEDA untuk pemilihan komponen
4. Melakukan assignment footprint yang sesuai dengan komponen fisik
5. Merancang layout PCB sesuai design rules JLCPCB
6. Melakukan routing trace PCB secara manual maupun semi-otomatis
7. Memanfaatkan fitur Copper Area, 3D View, dan Photo View EasyEDA
8. Menjalankan ERC dan DRC, serta memperbaiki error yang ditemukan
9. Menghasilkan file Gerber dan BOM yang siap produksi
10. Melakukan order PCB ke JLCPCB langsung dari EasyEDA

---

## B. KOMPETENSI YANG DICAPAI

| Kode | Kompetensi |
|------|-----------|
| K1 | Mampu membuat skematik lengkap menggunakan EasyEDA dengan library LCSC |
| K2 | Mampu melakukan footprint assignment dan membuat footprint kustom |
| K3 | Mampu melakukan placement dan routing PCB sesuai kaidah teknis |
| K4 | Mampu menggunakan fitur Copper Area, Net Inspector, dan 3D View |
| K5 | Mampu menghasilkan Gerber, BOM, dan simulasi order JLCPCB |

---

## C. ALAT DAN BAHAN

### Perangkat Lunak

| No | Software | Cara Akses | Fungsi |
|----|---------|-----------|--------|
| 1 | **EasyEDA Standard** | https://easyeda.com/editor (browser) | Desain skematik + PCB |
| 2 | **EasyEDA Pro** (opsional) | https://pro.easyeda.com atau install desktop | Desain offline |
| 3 | **JLCPCB Gerber Viewer** | https://gerber-viewer.jlcpcb.com | Validasi file Gerber |
| 4 | **LCSC** | https://lcsc.com | Referensi harga dan stok komponen |

> EasyEDA tidak memerlukan instalasi. Cukup buat akun di https://easyeda.com → langsung bisa desain.

### Referensi Komponen Proyek

| No | Komponen | LCSC Part# (referensi) | Package |
|----|---------|----------------------|--------|
| 1 | ESP32-S2-WROOM-1 | Cari: `ESP32-S2` | Module |
| 2 | L293D (Motor Driver) | C601581 | SOIC-16 |
| 3 | LM2596-5.0 | C347481 | TO-263-5 |
| 4 | Photodioda | Cari: `TEPT4400` / `SFH309` | 3mm |
| 5 | LED IR 940nm | Cari: `IR LED 940nm` | 5mm |
| 6 | HC-SR04 (konektor) | — | Header 4-pin |
| 7 | SG90 (konektor servo) | — | Header 3-pin |
| 8 | OLED 0.96" (konektor) | — | Header 4-pin |
| 9 | Push Button 6×6mm | C393942 | Through Hole |

### Peralatan Pendukung

- Laptop/PC dengan browser (Chrome/Firefox/Edge disarankan)
- Akun EasyEDA (daftar gratis di https://easyeda.com)
- Koneksi internet (untuk LCSC library dan cloud save)

---

## D. DASAR TEORI SINGKAT

### D.1 EasyEDA dan Ekosistemnya

EasyEDA adalah software EDA berbasis cloud yang terintegrasi dengan **LCSC** (toko komponen) dan **JLCPCB** (pabrikan PCB). Dari satu platform:
- Cari komponen → lihat harga + stok LCSC real-time
- Desain selesai → order PCB ke JLCPCB 1 klik
- BOM dari EasyEDA → langsung untuk JLCPCB SMT Assembly

> **Tips:** Selalu pilih komponen dari tab LCSC di EasyEDA agar tersedia LCSC Part Number. Kode ini penting untuk SMT Assembly.

### D.2 Alur Kerja di EasyEDA

```
Buat Project → New Schematic → Tambah Komponen (LCSC)
→ Wire + Net Label → ERC → Footprint → Update PCB
→ Board Shape → Design Rules → Placement → Routing
→ Copper Area → DRC → 3D View → Gerber Export
→ BOM Export → Order JLCPCB
```

### D.3 Panduan Track Width Cepat (1 oz copper)

| Arus | Lebar Minimum | Disarankan |
|------|--------------|-----------|
| < 1A | 0.2 mm | 0.3 mm |
| 1–2A | 0.5 mm | 0.8 mm |
| 2–3A | 0.8 mm | 1.2 mm |
| > 3A | 1.5+ mm | 2.0+ mm |

> **Tips:** Selalu tambahkan margin 20–30% dari kalkulasi minimum. Gunakan kalkulator resmi: https://jlcpcb.com/pcb-impedance-and-trace-width-calculator

### D.4 Aturan Routing

- Sudut **45°**, bukan 90°
- **Decoupling capacitor** (100nF) < 3 mm dari pin VCC setiap IC
- **Ground plane** di layer bawah (Copper Area → GND)
- **Jalur power lebih tebal** dari jalur sinyal
- **Pisahkan jalur analog dan digital**
- Jangan routing di bawah antena WiFi/RF

---

## E. LANGKAH KERJA

---

### SESI 1: PERSIAPAN DAN SKEMATIK

---

#### LANGKAH 1: Membuat Akun dan Project Baru di EasyEDA

**Estimasi waktu: 10 menit**

1. Buka https://easyeda.com → klik **Sign Up** jika belum punya akun
2. Isi email, username, password → verifikasi email
3. Login → dashboard EasyEDA muncul
4. Klik **New Project** → isi:
   - Name: `PCB_RobotOtonom_[NamaKelompok]`
   - Description: `Praktikum Modul 01 - PCB Main dan Sensor Line`
5. Klik **Save**
6. Di dalam project: klik **New Schematic** → Schematic Editor terbuka

> **Tips Half Offline:** Jika internet tidak stabil, aktifkan **Settings > Offline Mode**. EasyEDA akan bekerja dengan cache lokal. Simpan lebih sering dengan `Ctrl+S`.

**Checkpoint ✅:** Project dan schematic berhasil dibuat, Schematic Editor terbuka.

---

#### LANGKAH 2: Konfigurasi Canvas Skematik

**Estimasi waktu: 5 menit**

1. Di Schematic Editor: **Settings > Canvas Settings**
2. Atur:
   - Grid: 100 mil (default skematik)
   - Unit: mil atau mm (konsisten)
3. Cek toolbar atas: pastikan mode **Select**, bukan mode lain
4. Familiarisasi shortcut: `G` (tambah komponen), `W` (wire), `N` (net label), `P` (power port)

---

#### LANGKAH 3: Menambahkan Komponen dari Library LCSC

**Estimasi waktu: 45–60 menit**

1. Panel kiri → tab **LCSC**
2. Ketik nama komponen di search bar (contoh: `ESP32-S2-WROOM-1`)
3. Dari hasil pencarian, klik komponen untuk melihat preview simbol dan footprint
4. Verifikasi:
   - Status: **In Stock** di LCSC
   - Package sesuai yang diinginkan
   - Ada model 3D (opsional tapi disarankan)
5. Klik **Place** → klik di canvas untuk menempatkan
6. Tekan `R` untuk rotate, atau edit properties dengan klik komponen → panel kanan

**Komponen yang harus ditambahkan (sesuaikan dengan desain):**
- [ ] ESP32-S2 (modul atau chip)
- [ ] L293D Motor Driver
- [ ] LM2596 Step-down
- [ ] Induktor 100µH
- [ ] Dioda Schottky
- [ ] Kapasitor elektrolit dan SMD
- [ ] Resistor (berbagai nilai)
- [ ] Push Button
- [ ] LED status
- [ ] Konektor (JST, pin header)
- [ ] Switch On/Off

> **Tips LCSC:** Jika komponen tidak ditemukan di LCSC, coba tab **EasyEDA** → filter "Official". Atau ketik LCSC Part Number langsung (contoh: `C601581`).

**Checkpoint ✅:** Semua komponen utama berhasil ditempatkan di canvas.

---

#### LANGKAH 4: Menambahkan Komponen Kustom (Jika Diperlukan)

**Estimasi waktu: 15–30 menit (jika diperlukan)**

Jika ada komponen yang tidak ada di library LCSC/EasyEDA:

1. **Tools > Symbol Wizard**
2. Isi: Name, Reference (misal `U`), Pin count
3. Masukkan nama dan nomor pin per sisi
4. Klik **OK** → simbol terbuat otomatis
5. Klik kanan simbol → **Edit Symbol** untuk penyesuaian
6. Klik kanan di canvas → **Save to My Library** untuk simpan

Untuk footprint kustom:
1. **Tools > New Footprint**
2. Di Footprint Editor: **Place > Pad** → atur ukuran dari datasheet
3. **Place > Text** → tambah Ref di layer F.SilkS
4. Simpan ke **My Footprint Library**

**Checkpoint ✅:** Komponen kustom tersedia di My Library.

---

#### LANGKAH 5: Menghubungkan Komponen dengan Wire dan Net Label

**Estimasi waktu: 30–45 menit**

1. **Tambahkan Power Port terlebih dahulu:**
   - Tekan `P` → pilih `+5V` → klik posisi di dekat pin VCC komponen
   - Tekan `P` → pilih `GND` → klik di dekat pin GND
   - Tambahkan `+3V3`, `VBAT` sesuai kebutuhan

2. **Hubungkan pin yang berdekatan dengan Wire:**
   - Tekan `W` → klik pin awal → klik pin tujuan → `Esc`

3. **Gunakan Net Label untuk koneksi yang jauh:**
   - Tekan `N` → ketik nama net → `Enter` → klik ujung wire/pin
   - Gunakan nama yang **sama persis** di titik lain yang ingin dihubungkan

4. **Tandai pin yang tidak digunakan:**
   - **Place > No Connect** → klik pada pin yang sengaja dibiarkan floating
   - Tanda `X` akan muncul → ERC tidak akan mengangap ini sebagai error

**Konvensi penamaan net yang digunakan:**

| Net | Arti |
|-----|------|
| `+5V` | Supply 5V output LM2596 |
| `+3V3` | Supply 3.3V (dari modul ESP32 atau LDO) |
| `VBAT` | Tegangan baterai langsung |
| `GND` | Ground digital |
| `MOTOR_PWM_A` | PWM enable motor kanan |
| `MOTOR_IN1` | Arah motor kanan 1 |
| `SR04_TRIG_F` | Trigger HC-SR04 depan |
| `SR04_ECHO_F` | Echo HC-SR04 depan |
| `I2C_SDA` | I2C data bus |
| `I2C_SCL` | I2C clock bus |
| `SERVO_1` | PWM servo 1 |

> **Tips:** Buat semua power port dan GND terlebih dahulu, lalu hubungkan ke pin-pin secara sistematis per section (power section → MCU → sensor → output).

**Checkpoint ✅:** Semua komponen terhubung, tidak ada wire yang menggantung.

---

#### LANGKAH 6: Menjalankan ERC (Electrical Rules Check)

**Estimasi waktu: 10–20 menit**

1. **Design > ERC**
2. Klik **Check**
3. Screenshot hasil ERC awal (jumlah error dan warning)

4. Perbaiki setiap error:

| Error ERC | Lokasi | Solusi yang Dilakukan |
|----------|--------|----------------------|
| Pin not connected | | |
| Net with only one pin | | |
| Duplicate ref | | |
| (Tulis error yang ditemukan) | | |

5. Jalankan ERC lagi → target: **0 error merah**
6. Screenshot hasil ERC akhir

> **Tips:** Klik setiap error di panel ERC Result → canvas otomatis zoom ke lokasi masalah. ERC Warning (kuning) boleh ada asal dipahami penyebabnya.

**Checkpoint ✅:** ERC berjalan tanpa error merah.

---

#### LANGKAH 7: Verifikasi Footprint

**Estimasi waktu: 15–20 menit**

1. **Tools > Footprint Manager**
2. Review daftar semua komponen beserta footprint yang ter-assign
3. Verifikasi setiap footprint sesuai komponen fisik:
   - Kapasitor elektrolitik: package radial (diameter dan pitch sesuai)
   - Resistor 0805 / 0603: ukuran benar
   - IC L293D: SOIC-16 atau DIP-16, sesuai yang dibeli
   - Konektor: pitch 2.54 mm untuk pin header

4. Untuk mengubah footprint:
   - Klik nama footprint di baris komponen → Footprint Chooser terbuka
   - Cari footprint → klik **Apply**
5. Klik **OK** untuk konfirmasi semua

**Tabel verifikasi footprint:**

| Ref | Komponen | Footprint Saat Ini | Status |
|-----|---------|-------------------|--------|
| U1 | ESP32-S2 | | ☐ Benar ☐ Perlu ubah |
| U2 | LM2596 | | ☐ Benar ☐ Perlu ubah |
| U3 | L293D | | ☐ Benar ☐ Perlu ubah |
| R1–Rn | Resistor | | ☐ Benar ☐ Perlu ubah |
| C1–Cn | Kapasitor | | ☐ Benar ☐ Perlu ubah |
| J1–Jn | Konektor | | ☐ Benar ☐ Perlu ubah |

**Checkpoint ✅:** Semua komponen memiliki footprint yang benar.

---

### SESI 2: PCB LAYOUT, ROUTING, DAN PRODUKSI

---

#### LANGKAH 8: Transfer Skematik ke PCB Editor

**Estimasi waktu: 10 menit**

1. Di Schematic Editor: **Design > Update PCB**
2. Dialog perubahan muncul → klik **Update**
3. Buka PCB Editor (dari project tree atau tab)
4. Semua footprint muncul disertai **Ratsnest** (garis tipis koneksi)
5. Zoom out (`Ctrl+Scroll`) untuk melihat semua komponen

> **Tips:** Semua footprint biasanya tertumpuk di satu titik. Geser dan sebar dulu sebelum mulai placement agar ratsnest bisa diamati.

**Checkpoint ✅:** Semua footprint muncul di PCB Editor dengan ratsnest.

---

#### LANGKAH 9: Setup Board Outline dan Design Rules

**Estimasi waktu: 10–15 menit**

**Board Outline:**
1. Pilih layer **Board Outline** di panel layer
2. **Place > Board Outline > Rect**
3. Klik dua titik sudut → outline board terbentuk
4. Target dimensi (catat di tabel pengamatan):
   - PCB MAIN: ≤ 120 × 80 mm
   - PCB Sensor Line: ≤ 135 × 30 mm

**Design Rules:**
1. **Design > Design Rule**
2. Isi parameter:
   - Min Track Width: **0.2 mm**
   - Min Clearance: **0.2 mm**
   - Min Via Outer Diameter: **0.8 mm**
   - Min Via Drill: **0.4 mm**
   - Min Copper to Board Edge: **0.3 mm**
3. Klik **Save**

> **Tips:** Ukuran board ≤ 10×10 cm mendapatkan harga base JLCPCB (~$5 untuk 5 pcs). Desain board seefisien mungkin dalam batasan ini.

---

#### LANGKAH 10: Placement Komponen

**Estimasi waktu: 30–45 menit**

**Strategi placement:**
1. **Konektor di tepi board** → mudah diakses saat pakai
2. **LM2596 (power section)** → satu sisi, jauh dari MCU dan sensor analog
3. **ESP32** → area tengah dengan akses ke semua section
4. **L293D** → dekat konektor motor
5. **Kapasitor decoupling** → dekat pin VCC setiap IC (< 3 mm!)
6. **Push button** → area yang mudah dijangkau user
7. **OLED konektor** → sisi depan board, searah dengan tampilan

**Cara memindahkan komponen:**
- Klik komponen → drag
- `R` untuk rotate
- `X` untuk flip ke layer bawah (jika diperlukan)

**Amati ratsnest** saat memindahkan komponen:
- Posisi yang baik = ratsnest menjadi pendek dan tidak banyak silang

**Tabel placement:**

| Komponen | Posisi (kira-kira) | Alasan Penempatan |
|---------|-------------------|------------------|
| J1 (baterai) | | |
| SW1 (on/off) | | |
| U2 (LM2596) | | |
| U1 (ESP32) | | |
| U3 (L293D) | | |
| SW2-SW5 (button) | | |
| J4 (OLED) | | |

> **Tips:** Gunakan fitur **Highlight Net** (klik trace/ratsnest) untuk melihat jalur mana yang paling kompleks, lalu prioritaskan placement untuk jalur tersebut.

**Checkpoint ✅:** Semua komponen berada dalam board outline, decoupling cap dekat IC.

---

#### LANGKAH 11: Routing PCB

**Estimasi waktu: 60–90 menit**

**Urutan routing yang disarankan:**

**A. Route jalur power terlebih dahulu:**
1. Pilih **Route > Route Single Track** (atau `W`)
2. Set track width: **0.8–1.0 mm** untuk jalur 5V/VBAT
3. Hubungkan: LM2596 output → pin VCC ESP32, L293D, Servo header
4. Gunakan sudut 45° (tekan `Space` untuk ganti sudut di EasyEDA)

**B. Buat Ground Plane terlebih dahulu (lebih efisien):**
1. Pilih layer **B.Cu** (BottomLayer)
2. Tekan `E` atau **Place > Copper Area**
3. Klik pojok board → klik kanan → **End Drawing**
4. Properties:
   - Net: `GND`
   - Clearance: 0.3 mm
   - Fill Style: Solid
   - Thermal Relief: Yes
5. Klik kanan copper area → **Rebuild Copper Area**

**C. Route jalur sinyal:**
1. Set track width: **0.2–0.3 mm**
2. Route: I2C SDA/SCL ke OLED, GPIO ke servo, PWM ke L293D
3. Route: TRIG/ECHO HC-SR04 (tambahkan voltage divider untuk ECHO)
4. Gunakan via (`Space` saat routing) untuk ganti layer jika perlu

**D. Cek ratsnest:**
- Semua garis ratsnest harus hilang (semua terroute)
- **Design > Net Inspector** → pastikan 0 unrouted nets

> **Tips Routing:**
> - Aktifkan **View > Snap to Pad** agar kursor snap ke pad secara akurat
> - Klik **Design > Design Manager** untuk overview net yang belum terroute
> - Gunakan **Route > Auto Router** untuk routing awal, lalu perbaiki jalur kritis manual
> - Setelah routing selesai, selalu **Rebuild Copper Area** (`Shift+B`)

**Checklist routing:**
- [ ] Sudut 45° di semua belokan trace
- [ ] Tidak ada sudut 90°
- [ ] Track width sesuai arus di setiap jalur
- [ ] Copper area GND sudah di-rebuild
- [ ] Tidak ada ratsnest tersisa

**Checkpoint ✅:** Semua ratsnest hilang, ground plane terbentuk.

---

#### LANGKAH 12: DRC dan Perbaikan Error

**Estimasi waktu: 20–30 menit**

1. **Design > DRC**
2. Klik **Run DRC Check**
3. **Screenshot hasil DRC awal** (lampirkan di laporan)
4. Catat setiap error:

| No | Error DRC | Kategori | Lokasi | Solusi |
|----|----------|---------|--------|--------|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |

5. Perbaiki error → jalankan DRC lagi
6. **Screenshot hasil DRC akhir** (target: 0 error kritis)
7. Gunakan **Net Inspector** untuk verifikasi tidak ada unrouted net

> **Tips:** Klik dua kali setiap error di DRC Result → canvas langsung zoom ke lokasi error. Ini jauh lebih cepat daripada mencari manual.

**Checkpoint ✅:** DRC berjalan dengan 0 error kritis.

---

#### LANGKAH 13: Silkscreen dan Labeling

**Estimasi waktu: 15–20 menit**

1. Pilih layer **F.SilkS** (TopSilkLayer)
2. Pastikan reference designator semua komponen terlihat dan tidak tumpang tindih
3. **Place > Text** untuk menambahkan:
   - Label konektor: `+5V`, `GND`, `SERVO1`, `MOTOR_L`, `SR04_F`, dll.
   - Versi board: `PCB MAIN v1.0`
   - Nama kelompok dan tanggal
4. Tanda polaritas pada kapasitor elektrolit dan LED
5. Cek: **View > Photo View** untuk melihat tampilan realistis

**Standar label:**
- Tinggi teks minimum: 0.8 mm
- Ketebalan stroke: 0.15 mm minimum
- Teks **tidak boleh berada di atas pad**

---

#### LANGKAH 14: 3D View dan Pemeriksaan Visual

**Estimasi waktu: 10 menit**

1. **View > 3D View**
2. Rotate dan zoom untuk melihat dari berbagai sudut
3. Cek:
   - [ ] Orientasi IC (pin 1 benar)
   - [ ] Polaritas kapasitor dan LED
   - [ ] Posisi dan kesetaraan tinggi komponen
   - [ ] Tidak ada komponen yang overlapping secara fisik
4. **Screenshot dari atas (top view)** dan **sudut isometrik**

---

#### LANGKAH 15: Generate Gerber File dan BOM

**Estimasi waktu: 10 menit**

**Gerber:**
1. **Fabrication > PCB Fabrication File (Gerber)**
2. Semua layer sudah otomatis tercentang (JLCPCB preset)
3. Klik **Generate Gerber** → file `.zip` ter-download
4. Simpan di folder project dengan nama: `[NamaPCB]_Gerber_v1.zip`

**BOM:**
1. **Fabrication > BOM**
2. Pastikan kolom **LCSC** (Part Number) tersedia dan terisi
3. Klik **Export BOM** → download CSV
4. Simpan sebagai: `[NamaPCB]_BOM_v1.csv`

**Pick and Place (untuk SMT Assembly):**
1. **Fabrication > Pick and Place File**
2. Export → simpan CSV

---

#### LANGKAH 16: Validasi Gerber dan Simulasi Order

**Estimasi waktu: 10–15 menit**

**Validasi:**
1. Buka https://gerber-viewer.jlcpcb.com
2. Upload file `.zip` Gerber
3. Cek setiap layer satu per satu
4. Screenshot setiap layer untuk lampiran

**Checklist validasi:**
- [ ] Top copper benar
- [ ] Bottom copper benar (termasuk ground plane)
- [ ] Soldermask: hanya pad yang terbuka
- [ ] Silkscreen tidak menutupi pad
- [ ] Outline board tertutup sempurna
- [ ] Drill holes ada dan posisi benar

**Simulasi order JLCPCB:**
1. Buka https://jlcpcb.com → **Order Now**
2. Upload `.zip` Gerber → sistem auto-detect dimensi
3. Atur parameter:
   - Base Material: FR4
   - Layers: 2
   - PCB Thickness: 1.6 mm
   - Color: Green
   - Surface Finish: HASL Lead-free
   - Copper Weight: 1 oz
   - Quantity: 5
4. Screenshot halaman estimasi harga
5. Catat estimasi harga (USD) dan waktu produksi

**Atau order langsung dari EasyEDA:**
1. **Fabrication > Order at JLCPCB**
2. EasyEDA otomatis upload Gerber ke JLCPCB → lanjutkan di halaman JLCPCB

---

## F. TABEL PENGAMATAN DAN DATA

### F.1 Spesifikasi Desain

| Parameter | PCB MAIN | PCB Sensor Line |
|-----------|----------|----------------|
| Dimensi board (mm) | × | × |
| Jumlah layer | | |
| Jumlah komponen total | | |
| Jumlah net | | |
| Jumlah via | | |
| Lebar track power | mm | mm |
| Lebar track sinyal | mm | mm |
| Warna PCB dipilih | | |
| Surface finish dipilih | | |

### F.2 Hasil ERC

| Info | Nilai |
|------|-------|
| Jumlah error ERC awal | |
| Jumlah warning ERC awal | |
| Jumlah error ERC akhir | |
| Jumlah warning ERC akhir | |
| Status akhir ERC | ✅ Bersih / ⚠️ Ada warning |

### F.3 Daftar Error ERC dan Penanganan

| No | Error | Penyebab | Solusi |
|----|-------|---------|-------|
| 1 | | | |
| 2 | | | |
| 3 | | | |

### F.4 Hasil DRC

| Info | Nilai |
|------|-------|
| Jumlah error DRC awal | |
| Jumlah warning DRC awal | |
| Jumlah error DRC akhir | |
| Jumlah warning DRC akhir | |
| Status akhir DRC | ✅ Bersih / ⚠️ Ada warning |

### F.5 Daftar Error DRC dan Penanganan

| No | Error | Kategori | Solusi |
|----|-------|---------|-------|
| 1 | | | |
| 2 | | | |
| 3 | | | |

### F.6 Simulasi Order JLCPCB

| Parameter | PCB MAIN | PCB Sensor Line |
|----------|---------|----------------|
| Ukuran (auto detected) | | |
| Layer count | 2 | 2 |
| Quantity | 5 | 5 |
| Harga PCB (USD) | | |
| Ongkir (USD) | | |
| **Total (USD)** | | |
| Estimasi produksi | hari | hari |
| Estimasi tiba | | |

---

## G. PERTANYAAN ANALISIS

1. **Jelaskan** perbedaan antara menggunakan komponen dari tab LCSC vs tab EasyEDA di EasyEDA. Mengapa komponen LCSC lebih disarankan untuk proyek yang akan dipesan di JLCPCB?

   **Jawaban:**
   _______________________________________________

2. **Hitunglah** lebar track minimum untuk jalur dari LM2596 ke ESP32 yang membawa arus 1.5A, menggunakan copper 1 oz dan kenaikan suhu yang diizinkan 10°C. Tampilkan perhitungan atau screenshot kalkulator JLCPCB.

   **Jawaban:**
   _______________________________________________

3. **Jelaskan** fungsi Copper Area (Ground Plane) yang Anda buat di layer B.Cu. Apa yang terjadi jika ground plane tidak dibuat?

   **Jawaban:**
   _______________________________________________

4. **Bandingkan** tampilan 3D View dan Photo View di EasyEDA. Kapan masing-masing lebih berguna dalam proses desain?

   **Jawaban:**
   _______________________________________________

5. **Evaluasi** hasil Gerber Viewer. Apakah ada perbedaan antara tampilan di EasyEDA PCB Editor dengan tampilan di JLCPCB Gerber Viewer? Jelaskan apa yang Anda perhatikan!

   **Jawaban:**
   _______________________________________________

---

## H. KESIMPULAN

Tuliskan kesimpulan praktikum dalam 4–5 poin:

1. _______________________________________________
2. _______________________________________________
3. _______________________________________________
4. _______________________________________________
5. _______________________________________________

---

## I. LAMPIRAN (Screenshot Wajib)

- [ ] Screenshot skematik lengkap (zoom out, semua komponen terlihat)
- [ ] Screenshot ERC result awal (ada error)
- [ ] Screenshot ERC result akhir (0 error)
- [ ] Screenshot Footprint Manager (semua komponen ter-assign)
- [ ] Screenshot PCB layout top view (setelah routing)
- [ ] Screenshot PCB layout bottom view (ground plane)
- [ ] Screenshot DRC result awal
- [ ] Screenshot DRC result akhir (0 error kritis)
- [ ] Screenshot 3D View (top + isometrik)
- [ ] Screenshot Photo View (tampilan realistis)
- [ ] Screenshot JLCPCB Gerber Viewer (top copper + board outline)
- [ ] Screenshot halaman order JLCPCB (dengan estimasi harga)
- [ ] Screenshot BOM yang diekspor

---

## J. REFERENSI

1. EasyEDA Documentation. https://docs.easyeda.com
2. JLCPCB PCB Capabilities. https://jlcpcb.com/capabilities/Capabilities
3. JLCPCB Trace Width Calculator. https://jlcpcb.com/pcb-impedance-and-trace-width-calculator
4. LCSC Component Search. https://lcsc.com
5. IPC-2221B – Generic Standard on Printed Board Design
6. EasyEDA YouTube Channel. https://www.youtube.com/@EasyEDA

---

*Jobsheet ini merupakan dokumen resmi praktikum. Isi dengan lengkap dan jujur.*  
*Nilai ditentukan berdasarkan kelengkapan jobsheet, kualitas desain, dan pemahaman analitis.*

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
