# MATERI MODUL 01: PERANCANGAN PCB DARI SKEMATIK HINGGA PRODUKSI

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 01 – Desain PCB  
**Software:** EasyEDA (Standard / Pro)  
**Manufaktur:** JLCPCB  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan – Konsep Dasar PCB](#1-pendahuluan--konsep-dasar-pcb)
2. [Jenis dan Material PCB](#2-jenis-dan-material-pcb)
3. [Alur Kerja Desain PCB Industri](#3-alur-kerja-desain-pcb-industri)
4. [Pengenalan EasyEDA](#4-pengenalan-easyeda)
5. [Pembuatan Skematik di EasyEDA](#5-pembuatan-skematik-di-easyeda)
6. [Footprint dan Packaging](#6-footprint-dan-packaging)
7. [Konversi ke PCB Layout](#7-konversi-ke-pcb-layout)
8. [Routing PCB](#8-routing-pcb)
9. [DRC (Design Rule Check)](#9-drc-design-rule-check)
10. [Silkscreen dan Labeling](#10-silkscreen-dan-labeling)
11. [3D View dan Mechanical Check](#11-3d-view-dan-mechanical-check)
12. [Generate Gerber File dan BOM](#12-generate-gerber-file-dan-bom)
13. [Validasi Gerber](#13-validasi-gerber)
14. [Order PCB ke JLCPCB](#14-order-pcb-ke-jlcpcb)
15. [Referensi](#15-referensi)

---

## 1. PENDAHULUAN – KONSEP DASAR PCB

### 1.1 Definisi PCB

**PCB (Printed Circuit Board)** atau Papan Sirkuit Cetak adalah media berbasis lembaran padat yang digunakan untuk menghubungkan komponen-komponen elektronika secara mekanis dan elektris menggunakan jalur konduktif (trace) yang tercetak atau teretsa pada permukaannya.

PCB menggantikan pengkabelan manual (point-to-point wiring) yang tidak rapi, rentan kesalahan, dan sulit diproduksi massal. Dengan PCB:
- Rangkaian menjadi **ringkas, ringan, dan terstandarisasi**
- **Reproduksi massal** menjadi mudah dan konsisten
- **Keandalan** meningkat karena koneksi permanen
- **Troubleshooting dan pengujian** lebih mudah

### 1.2 Fungsi PCB

| Fungsi | Penjelasan |
|--------|-----------|
| Penopang mekanis | Menahan komponen secara fisik |
| Konduktor listrik | Menghantarkan sinyal dan daya antar komponen |
| Isolator | Memisahkan jalur yang berbeda potensial |
| Penyebar panas | Membantu disipasi panas komponen |

---

## 2. JENIS DAN MATERIAL PCB

### 2.1 Jenis PCB Berdasarkan Jumlah Layer

#### Single Layer (1 Layer)
- Konduktor hanya pada **satu sisi** substrat
- Paling murah, paling mudah dibuat
- Cocok untuk: charger sederhana, lampu LED, kontroler dasar
- Keterbatasan: routing terbatas, sering memerlukan jumper wire

#### Double Layer (2 Layer)
- Konduktor pada **kedua sisi** (Top dan Bottom)
- Dihubungkan dengan **via** (lubang berlapis tembaga)
- **Standar industri** untuk produk elektronika konsumer
- Cocok untuk: mikrokontroler, motor driver, sensor board
- Harga terjangkau — di JLCPCB mulai ~$5 untuk 5 pcs

#### Multilayer (4, 6, 8+ Layer)
- Lapisan tembaga dalam jumlah banyak, dipisahkan prepreg/core
- Layer tengah biasanya didedikasikan untuk **power plane** dan **ground plane**
- Cocok untuk: smartphone, laptop, server board, frekuensi tinggi
- Mahal, proses produksi kompleks

### 2.2 Material PCB

| Material | TG (°C) | Keterangan |
|----------|---------|------------|
| **FR4** | 130–170 | Material standar, glass-reinforced epoxy, **digunakan di praktikum ini** |
| CEM-1 | 105 | Lebih murah, cocok untuk single layer |
| Rogers (RO4003C) | 280+ | RF/microwave, impedance-controlled |
| Aluminium | — | Power LED, heatsink terintegrasi |
| Flexible (Polyimide) | — | PCB fleksibel, wearable device |

**FR4** dipilih karena:
- Stabil secara termal hingga 130–170°C
- Sifat dielektrik baik (Dk ≈ 4.5 pada 1 MHz)
- Tersedia di semua pabrikan termasuk JLCPCB dengan harga standar

### 2.3 Ketebalan Copper (Copper Weight)

| Copper Weight | Ketebalan (µm) | Max Arus ≈ | Penggunaan |
|---------------|----------------|-----------|-----------|
| 0.5 oz | 17.5 µm | ~1 A / 1 mm | Sinyal digital kecil |
| **1 oz** | **35 µm** | **~2 A / 1 mm** | **Standar — default JLCPCB** |
| 2 oz | 70 µm | ~4 A / 1 mm | Power supply, motor driver |
| 3 oz | 105 µm | ~6 A / 1 mm | High-current |

**Formula Track Width (IPC-2221):**

$$I = k \cdot \Delta T^{0.44} \cdot A^{0.725}$$

Dimana:
- $I$ = arus (Ampere)
- $k$ = 0.048 (external layer, metric)
- $\Delta T$ = kenaikan temperatur yang diizinkan (°C)
- $A$ = luas penampang trace (mil²)

Kalkulator online JLCPCB: https://jlcpcb.com/pcb-impedance-and-trace-width-calculator

### 2.4 Fungsi Jalur (Trace) PCB

| Jenis Jalur | Lebar Tipikal | Karakteristik |
|-------------|---------------|---------------|
| Sinyal digital | 0.1–0.3 mm | Frekuensi rendah–menengah |
| Sinyal analog | 0.1–0.25 mm | Hindari dekat jalur switching |
| Jalur power | 0.5–2+ mm | Sesuai arus, lebih tebal lebih aman |
| Ground | Copper pour | Ground plane lebih baik dari trace biasa |

### 2.5 Perbedaan Skematik dan Layout PCB

| Aspek | Skematik | Layout PCB |
|-------|----------|-----------|
| Representasi | Diagram logika/fungsi | Implementasi fisik |
| Komponen | Simbol abstrak | Footprint nyata |
| Koneksi | Net/wire logis | Trace tembaga |
| Posisi | Tidak relevan | Sangat relevan |
| Ukuran | Tidak skala nyata | Skala actual (mm) |

---

## 3. ALUR KERJA DESAIN PCB INDUSTRI

```
[Spesifikasi] → [Schematic + ERC] → [Footprint Assignment]
      ↓
[PCB Layout] → [Placement] → [Routing] → [DRC] → [3D Check]
      ↓
[Gerber + BOM Export] → [Validasi Gerber Viewer]
      ↓
[Order JLCPCB] → [Assembly] → [Testing] → [Iterasi]
```

### 3.1 Perencanaan Awal
- Tentukan **fungsi, tegangan, arus, dimensi target**
- Identifikasi komponen dan cari di **LCSC** (terintegrasi EasyEDA)
- Tentukan jumlah layer dan target pabrikan
- Buat **BOM awal** berdasarkan kandidat komponen LCSC

---

## 4. PENGENALAN EASYEDA

### 4.1 Apa itu EasyEDA?

**EasyEDA** adalah software EDA (Electronic Design Automation) berbasis cloud yang dikembangkan oleh **Shenzhen LC Technology** — perusahaan yang juga mengelola **LCSC Electronics** dan **JLCPCB**. Tiga platform ini terintegrasi dalam satu ekosistem:

```
EasyEDA (Desain) ←→ LCSC (Komponen) ←→ JLCPCB (Manufaktur)
```

Keunggulan utama:
- **Terintegrasi dengan JLCPCB** → order PCB 1 klik dari dalam EasyEDA
- **Library LCSC** → 1.4 juta+ komponen real-time dengan stok dan harga
- **Gratis** untuk penggunaan umum
- **Tidak perlu install** (versi web browser) atau tersedia offline (EasyEDA Pro)
- Tersedia dalam mode **Half Offline** untuk koneksi terbatas

### 4.2 EasyEDA Standard vs EasyEDA Pro

| Fitur | EasyEDA Standard | EasyEDA Pro |
|-------|-----------------|------------|
| Platform | Browser (web) | Desktop client (Win/Mac/Linux) |
| URL | https://easyeda.com/editor | https://pro.easyeda.com |
| Mode offline | Half Offline (cache) | Penuh offline |
| Antarmuka | Klasik | Modern (mirip Altium) |
| Format file | JSON cloud | .epro lokal |
| Simulasi SPICE | ✅ | ✅ |
| Panel design | Terbatas | Lengkap |
| Kolaborasi tim | ✅ | ✅ (lebih baik) |
| Harga | Gratis | Gratis (dengan akun) |

> **Rekomendasi Praktikum:** Gunakan **EasyEDA Standard** di browser untuk kemudahan akses tanpa instalasi.

### 4.3 Daftar Lengkap Fitur EasyEDA

| Fitur | Menu / Lokasi | Fungsi |
|-------|--------------|--------|
| **Schematic Editor** | File > New > Schematic | Menggambar skematik logis |
| **PCB Editor** | File > New > PCB | Merancang layout fisik |
| **Symbol Wizard** | Tools > Symbol Wizard | Meng-generate simbol IC kustom |
| **Symbol Editor** | Klik kanan simbol > Edit | Edit simbol komponen |
| **Footprint Manager** | Tools > Footprint Manager | Assign dan kelola footprint |
| **Footprint Editor** | Tools > New Footprint | Buat footprint kustom |
| **LCSC Library Search** | Panel kiri > LCSC tab | Cari komponen dari database LCSC |
| **EasyEDA Library** | Panel kiri > EasyEDA tab | Komponen official + user |
| **My Library** | Panel kiri > My Library | Komponen yang disimpan sendiri |
| **ERC** | Design > ERC | Cek kesalahan elektris skematik |
| **DRC** | Design > DRC | Cek pelanggaran design rule PCB |
| **Net Inspector** | Design > Net Inspector | Status routing semua net |
| **Design Manager** | Design > Design Manager | Overview komponen, net, error |
| **Interactive Router** | Route > Route Single Track | Routing manual dengan obstacle avoidance |
| **Auto Router** | Route > Auto Router | Routing otomatis |
| **Copper Area** | Place > Copper Area | Ground plane / copper pour |
| **Via** | Place > Via | Menambah via manual |
| **3D Viewer** | View > 3D View | Preview 3D model PCB |
| **Photo View** | View > Photo View | Tampilan realistis PCB (2D) |
| **Gerber Export** | Fabrication > PCB Fabrication File | Export file manufaktur |
| **BOM Export** | Fabrication > BOM | Export Bill of Materials |
| **Pick & Place Export** | Fabrication > Pick and Place File | Untuk SMT Assembly JLCPCB |
| **Order at JLCPCB** | Fabrication > Order at JLCPCB | Pesan PCB 1 klik |
| **Netlist Export** | Design > Netlist | Export netlist ke berbagai format |
| **Import** | File > Import | Import dari Eagle, KiCad, Altium |
| **SPICE Simulation** | Simulation menu | Simulasi rangkaian analog/digital |
| **Cross Probe** | Klik komponen di sch/pcb | Highlight komponen di editor lain |
| **Highlight Net** | Klik net/trace | Sorot jalur yang sama |
| **Multi-Sheet Schematic** | Add Sheet di project | Skematik multi-halaman |
| **Panelize** | Tools > Panelize | Panel beberapa PCB sekaligus |
| **Design Rule Settings** | Design > Design Rule | Atur batas aturan desain |
| **Generate Netlist** | Design > Netlist | Buat netlist dari skematik |
| **Board Outline** | Place > Board Outline | Atur shape dan ukuran PCB |
| **Measurement Tool** | Tools > Measure | Ukur jarak di PCB |
| **DXF Import** | File > Import > DXF | Import outline board dari CAD |
| **Collaboration** | Project > Share | Share desain ke tim |

### 4.4 Antarmuka EasyEDA Standard

```
┌─────────────────────────────────────────────────────────────┐
│ Menu: File | Edit | View | Place | Design | Route | ...     │
├──────────────────┬───────────────────────────┬──────────────┤
│   Panel Kiri     │                           │ Panel Kanan  │
│  ┌────────────┐  │    CANVAS (Area Kerja)    │  Properties  │
│  │ Library    │  │                           │  ┌─────────┐ │
│  │ - LCSC     │  │  Schematic / PCB Editor   │  │ Object  │ │
│  │ - EasyEDA  │  │  ditampilkan di sini      │  │ Props   │ │
│  │ - My Lib   │  │                           │  └─────────┘ │
│  ├────────────┤  │                           │              │
│  │ Footprint  │  │                           │  Layers      │
│  │ 3D Model   │  │                           │  Panel       │
│  └────────────┘  │                           │              │
├──────────────────┴───────────────────────────┴──────────────┤
│ Log | DRC Result | Find Result | Design Manager (bawah)      │
└─────────────────────────────────────────────────────────────┘
```

### 4.5 Shortcut Keyboard EasyEDA

| Shortcut | Skematik | PCB Editor |
|---------|---------|-----------|
| `G` | Place Symbol (add component) | — |
| `W` | Place Wire | Route Single Track |
| `N` | Net Label | — |
| `P` | Power Port | — |
| `R` | Rotate | Rotate |
| `X` | Mirror H | Mirror H |
| `Y` | Mirror V | Mirror V |
| `Space` | — | Flip layer (saat routing) |
| `V` | — | Insert Via |
| `E` | — | Copper Area |
| `Delete` | Hapus | Hapus |
| `Ctrl+S` | Simpan | Simpan |
| `Ctrl+Z` | Undo | Undo |
| `Ctrl+A` | Select All | Select All |
| `Ctrl+D` | Duplikat | Duplikat |
| `Scroll` | Zoom | Zoom |
| `Middle drag` | Pan | Pan |
| `Esc` | Batalkan aksi | Batalkan aksi |

### 4.6 Mode Half Offline EasyEDA

EasyEDA Standard dapat berjalan dalam mode **Half Offline**:
- File disimpan lokal + sinkron ke cloud saat online
- Library di-cache lokal untuk akses offline
- Perubahan akan sync kembali saat internet tersedia
- Aktifkan: **Settings > Offline Mode**

---

## 5. PEMBUATAN SKEMATIK DI EASYEDA

### 5.1 Membuat Project dan Schematic Baru

1. Buka https://easyeda.com → Login/Register (gratis)
2. **File > New > Project** → beri nama, beri deskripsi
3. Di dalam project: **File > New > Schematic** → editor terbuka
4. Tools > Canvas Settings → atur unit (mm), grid (50 mil default)
5. Simpan dengan `Ctrl+S`

### 5.2 Menambahkan Komponen dari Library LCSC

LCSC adalah toko komponen resmi yang terintegrasi dengan EasyEDA, dengan 1.4 juta+ komponen:

**Cara mencari dan menempatkan komponen:**
1. Panel kiri → tab **LCSC** (atau tekan `G`)
2. Ketik nama komponen (contoh: `ESP32-S2`) atau kode LCSC (contoh: `C701342`)
3. Klik komponen untuk preview simbol dan footprint
4. Klik **Place** → klik di canvas untuk menempatkan

**Informasi yang tersedia per komponen LCSC:**
| Info | Keterangan |
|------|-----------|
| LCSC Part# | Kode unik (dimulai `C...`), digunakan untuk order |
| Datasheet | Link ke datasheet resmi |
| Price | Harga real-time dari LCSC |
| Stock | Stok tersedia sekarang |
| Symbol | Preview simbol skematik |
| Footprint | Preview footprint PCB |
| 3D Model | Preview model 3D (jika tersedia) |

> **Tips:** Pilih komponen dengan status **"In Stock"** di LCSC agar bisa langsung dipesan bersama dengan PCB.

### 5.3 Menggunakan Library EasyEDA (Non-LCSC)

Untuk komponen yang tidak ada di LCSC:
1. Panel kiri → tab **EasyEDA**
2. Ketik nama komponen
3. Filter: **Official** (aman) atau **User Contributed** (review dulu)
4. **Verified** symbol/footprint lebih tepercaya

### 5.4 Membuat Custom Symbol

Jika komponen tidak ditemukan di library mana pun:

**Metode 1 – Symbol Wizard (untuk IC):**
1. **Tools > Symbol Wizard**
2. Isi: Component Name, Reference Prefix (contoh: `U`), Footprint
3. Masukkan jumlah pin dan nama pin per sisi (Left/Right/Top/Bottom)
4. Klik **OK** → simbol terbuat otomatis → tempatkan di canvas
5. Simpan ke **My Library**

**Metode 2 – Edit Manual:**
1. Place komponen existing → klik kanan → **Edit Symbol**
2. Gambar body: **Place > Rectangle**
3. Tambah pin: **Place > Pin** → atur nama, nomor, tipe (Input/Output/Power/Passive/No Connect)
4. Simpan ke **My Library**

**Tipe Pin yang Penting:**
| Tipe | Simbol | Contoh |
|------|--------|--------|
| Input | — → | GPIO input, CLK |
| Output | → — | GPIO output |
| Bidirectional | ↔ | SDA, MOSI |
| Power | Tebal | VCC, GND |
| Open Collector | ↓ | I2C dengan pull-up |
| Passive | — | Resistor, kapasitor |
| No Connect | X | Pin NC / tidak dipakai |

### 5.5 Menyimpan Komponen ke My Library

1. Klik kanan komponen di canvas → **Save to My Library**
2. Atau dari Symbol Editor: **File > Save to My Library**
3. Komponen tersimpan di tab **My Library** → tersedia di semua project
4. Bisa di-share ke tim dalam EasyEDA Pro (Team Library)

### 5.6 Wire dan Menghubungkan Komponen

**Wire:**
- Tekan `W` → klik terminal pin awal → klik terminal pin tujuan → `Esc`
- Wire menghubungkan langsung pin-pin yang berdekatan

**Cara menghindari wire panjang:**
- Gunakan **Net Label** untuk koneksi yang jauh
- Gunakan **Bus** untuk kelompokan banyak sinyal (misal data bus 8-bit)

### 5.7 Net Label dan Power Port

**Net Label:**
- Tekan `N` → ketik nama net → tempelkan ke ujung wire/pin
- Net Label yang **identik (persis sama, case-sensitive)** di seluruh skematik dianggap terhubung meskipun tidak ada wire yang menghubungkan
- Net label berbeda = net yang berbeda (bahkan jika mirip)

**Power Port:**
- Tekan `P` → pilih simbol power (`VCC`, `GND`, `+5V`, `+3V3`, dll.)
- Power Port adalah Net Label khusus dengan simbol visual yang lebih jelas
- Selalu gunakan Power Port (bukan net label biasa) untuk supply tegangan

**Konvensi penamaan net yang disarankan:**
| Net | Arti |
|-----|------|
| `+5V` | Supply 5V dari regulator |
| `+3V3` | Supply 3.3V |
| `VBAT` | Tegangan baterai langsung |
| `GND` | Ground digital/umum |
| `AGND` | Ground analog (jika terpisah) |
| `PGND` | Power ground |
| `ESP_SDA` | I2C SDA dari ESP32 |
| `ESP_SCL` | I2C SCL dari ESP32 |
| `PWM_MOTORA` | PWM motor A |
| `SR04_TRIG_F` | Trigger HC-SR04 depan |

### 5.8 ERC (Electrical Rules Check)

**Cara menjalankan:**
1. **Design > ERC**
2. Klik **Check**
3. Error muncul di panel bawah (ERC Result)
4. Klik error → zoom ke lokasi

**Error Umum dan Solusi:**

| Error ERC | Penyebab | Solusi |
|----------|---------|--------|
| `Pin not connected` | Pin floating | Tambah `No Connect` mark atau hubungkan |
| `Net with only one pin` | Net Label salah ketik | Cek ejaan, perhatikan case |
| `Output drives output` | Dua output terhubung langsung | Rancang ulang, mungkin perlu resistor |
| `Power pin not driven` | Net supply tidak ada sumber | Tambah power supply symbol |
| `Duplicate reference` | Dua komponen ref sama | Ubah reference: klik komponen → panel kanan |

**No Connect Mark:**
- Aktifkan via **Place > No Connect** → klik pada pin yang memang tidak digunakan
- Tanda `X` kecil akan muncul → ERC menganggap pin ini sengaja dibiarkan floating

### 5.9 Multi-Sheet Schematic

Untuk desain kompleks, skematik bisa dibagi ke beberapa halaman:
1. Di project tree (kiri): klik ikon `+` di sebelah Schematic → **Add Sheet**
2. Beri nama sheet (contoh: `Power`, `MCU`, `Sensor`)
3. Gunakan **Net Label** yang sama di semua sheet untuk menghubungkan antar halaman
4. Atau gunakan **Net Flag/Port** untuk koneksi antar sheet yang eksplisit

### 5.10 Simulasi SPICE (Fitur Tambahan)

EasyEDA mendukung simulasi rangkaian sebelum masuk ke PCB:
1. Buka schematic → **Simulation > Add SPICE Simulation**
2. Tambahkan **voltage source**, **probe**, dan **ground** dari library `Simulation`
3. Pilih tipe analisis: **Transient** (waktu), **AC** (frekuensi), **DC Sweep**
4. Klik **Run** → grafik hasil ditampilkan
5. Berguna untuk: filter RC/LC, op-amp, konverter switching

---

## 6. FOOTPRINT DAN PACKAGING

### 6.1 SMD vs Through Hole

| Aspek | SMD (Surface Mount) | Through Hole |
|-------|-------------------|-------------|
| Ukuran | Lebih kecil | Lebih besar |
| Kepadatan | Tinggi (bisa 2 sisi) | Rendah (1 sisi) |
| Pemasangan | Mesin reflow oven | Manual solder |
| Kekuatan mekanis | Lebih rendah | Lebih tinggi |
| Prototyping | Perlu alat/skill | Lebih mudah tangan |
| JLCPCB SMT Assembly | ✅ Bisa | ❌ Tidak tersedia |

### 6.2 Jenis Package Umum

**SMD (untuk komponen pasif):**
| Package | Ukuran (L×W) | Kemudahan Solder Manual |
|---------|-------------|------------------------|
| 0201 | 0.6×0.3 mm | ❌ Sangat sulit |
| 0402 | 1.0×0.5 mm | ❌ Sulit |
| **0603** | 1.6×0.8 mm | ✅ Bisa dengan tangan |
| **0805** | 2.0×1.25 mm | ✅ Mudah |
| 1206 | 3.2×1.6 mm | ✅ Mudah |

**SMD (untuk IC):**
| Package | Keterangan |
|---------|-----------|
| SOT-23 | Transistor/dioda kecil, regulator |
| SOIC-8/16 | IC 8/16 pin, pitch 1.27 mm |
| TSSOP | IC pin banyak, pitch 0.65 mm |
| QFN | IC dengan pad di bawah, flat |
| TQFP | IC dengan banyak pin |

**Through Hole:**
| Package | Contoh |
|---------|--------|
| DIP-8/16/28 | IC logika, op-amp |
| TO-220 | Regulator LM7805, MOSFET power |
| TO-92 | Transistor BC547, LM35 |
| Axial | Resistor karbon, dioda |
| Radial | Kapasitor elektrolitik |

### 6.3 Assign Footprint di EasyEDA

Komponen dari LCSC sudah otomatis memiliki footprint. Untuk mengubah:

1. Klik komponen di skematik
2. Panel kanan → field **Footprint** → klik ikon pencarian
3. Footprint Manager terbuka → pilih footprint → **Apply**

**Atau via Footprint Manager massal:**
1. **Tools > Footprint Manager**
2. Daftar semua komponen ditampilkan
3. Klik kolom **Footprint** per baris untuk mengubah
4. Klik **OK** untuk menerapkan semua

### 6.4 Membuat Custom Footprint di EasyEDA

1. **Tools > New Footprint** (atau dari Footprint Manager klik `+`)
2. Di Footprint Editor:
   - **Place > Pad** → atur:
     - Shape: Round/Square/Oval/Polygon
     - Width dan Height pad
     - Hole Diameter (untuk Through Hole, 0 untuk SMD)
     - Pad Number (sesuai nomor pin datasheet)
     - Layer: `F.Cu` untuk SMD, `Multi-Layer` untuk THT
   - **Place > Text** → tambah Ref di layer `F.SilkS`
   - **Place > Rect** → courtyard di layer `F.Courtyard`
   - **Place > Line** → outline komponen di layer `F.Fab`
3. Simpan ke **My Footprint Library**

**Checklist footprint yang benar:**
- [ ] Ukuran pad sesuai datasheet Land Pattern (IPC-7351)
- [ ] Jarak antar pad (pitch) sesuai datasheet
- [ ] Drill hole = diameter kaki komponen + 0.2–0.3 mm (untuk THT)
- [ ] Courtyard cukup besar (komponenn ≥ 0.25 mm dari tepi pad)
- [ ] Silk outline ada dan tidak menutupi pad
- [ ] Pin 1 ditandai dengan jelas

### 6.5 Properti Komponen yang Kritis (untuk JLCPCB)

Setiap komponen di EasyEDA memiliki properti:
| Properti | Keterangan | Wajib? |
|---------|-----------|--------|
| Reference | R1, C3, U2 | ✅ |
| Value | 10kΩ, 100nF | ✅ |
| Footprint | Package yang dipakai | ✅ |
| LCSC | Kode part LCSC (C...) | ✅ untuk SMT Assembly |
| MPN | Manufacturer Part Number | Disarankan |
| Datasheet | URL datasheet | Disarankan |

---

## 7. KONVERSI KE PCB LAYOUT

### 7.1 Update PCB dari Schematic

1. Di Schematic Editor: **Design > Update PCB**
2. Review dialog perubahan yang akan diaplikasikan
3. Klik **Update**
4. Semua footprint muncul di PCB Editor bersama **Ratsnest** (garis tipis antar pad yang belum di-route)

### 7.2 Board Shape (Edge.Cuts) di EasyEDA

1. Di PCB Editor, pilih layer **Board Outline**
2. **Place > Board Outline > Rect** (atau poligon custom)
3. Klik dua titik sudut → outline board terbentuk
4. Atau import dari DXF: **File > Import > DXF**

**Patokan dimensi vs harga JLCPCB:**
| Ukuran PCB | Harga (5 pcs, 2-layer) |
|------------|------------------------|
| ≤ 5×5 cm | ~$4 |
| ≤ 10×10 cm | ~$5 (paling ekonomis) |
| ≤ 15×15 cm | ~$8–$12 |

> **Tips hemat:** Desain board dengan ukuran ≤ 10×10 cm untuk mendapatkan harga base JLCPCB.

### 7.3 Pengaturan Layer di EasyEDA PCB Editor

| Layer | Fungsi |
|-------|--------|
| `F.Cu` / `TopLayer` | Tembaga sisi atas |
| `B.Cu` / `BottomLayer` | Tembaga sisi bawah |
| `F.SilkS` / `TopSilkLayer` | Silkscreen atas (teks, border komponen) |
| `B.SilkS` / `BottomSilkLayer` | Silkscreen bawah |
| `F.Mask` / `TopPasteMaskLayer` | Soldermask atas (negatif – pad akan terbuka) |
| `B.Mask` / `BottomPasteMaskLayer` | Soldermask bawah |
| `Board Outline` / `Edge.Cuts` | Garis potong board |
| `F.Courtyard` | Batas fisik komponen atas |
| `Document` | Layer anotasi / dokumentasi |

Aktifkan/nonaktifkan layer via panel **Layers** di kiri bawah PCB Editor. Klik huruf di sebelah layer untuk toggle.

### 7.4 Design Rules Setup di EasyEDA

**Design > Design Rule** atau klik ikon pengaturan DRC:

| Parameter | Nilai Disarankan | JLCPCB Minimum |
|-----------|-----------------|----------------|
| Min Track Width | 0.2 mm | 0.09 mm |
| Min Clearance (track-track) | 0.2 mm | 0.09 mm |
| Min Via Outer Diameter | 0.8 mm | 0.3 mm |
| Min Via Drill | 0.4 mm | 0.15 mm |
| Min Silkscreen Line Width | 0.15 mm | 0.1 mm |
| Min Copper to Board Edge | 0.3 mm | 0.2 mm |

Setelah diatur, klik **Save** untuk menyimpan template design rule.

### 7.5 Grid dan Snap di EasyEDA PCB Editor

- Grid untuk placement: **1 mm** (komponen besar), **0.5 mm** (komponen kecil)
- Grid untuk routing: **0.1 mm** atau **0.05 mm** (fine routing)
- Ubah grid: dropdown di toolbar atas atau `Settings > Canvas`
- Aktifkan **Snap to Pad** agar kursor menempel ke pad saat routing

---

## 8. ROUTING PCB

### 8.1 Perhitungan Track Width

**Panduan cepat (1 oz copper, ΔT = 10°C, external trace):**

| Arus | Minimum | Disarankan |
|------|---------|-----------|
| < 0.5 A | 0.1 mm | 0.2 mm |
| 1 A | 0.25 mm | 0.3 mm |
| 2 A | 0.5 mm | 0.8 mm |
| 3 A | 0.8 mm | 1.2 mm |
| 5 A | 1.5 mm | 2.0 mm |

Gunakan kalkulator JLCPCB: https://jlcpcb.com/pcb-impedance-and-trace-width-calculator

**Cara mengatur track width di EasyEDA:**
- Sebelum mulai route: ubah nilai **Track Width** di toolbar atas
- Atau klik kanan track → **Change Track Width**
- Bisa atur **Net Rules**: **Design > Net Class** → assign width berbeda per net

### 8.2 Manual Routing di EasyEDA

1. **Route > Route Single Track** (atau `W` di PCB Editor)
2. Klik pad/via awal
3. Gerakkan mouse → track mengikuti dengan sudut otomatis
4. Tekan `Space` untuk ganti layer (via otomatis ditambahkan)
5. Klik pad tujuan untuk selesai
6. `Esc` untuk membatalkan tanpa simpan

**Aturan routing wajib:**
- **Sudut 45°** — hindari 90° (korosi etsa tidak sempurna, efek antena EMI)
- **Decoupling capacitor** (100nF) letakkan < 3 mm dari pin VCC setiap IC
- **Jalur power lebih tebal** dari jalur sinyal
- **Ground plane** di layer bawah untuk mengurangi EMI
- **Pisahkan jalur analog dan digital** (terutama sensor)
- **Hindari routing di bawah antena** (jika ada modul WiFi/RF)

### 8.3 Interactive Router EasyEDA

EasyEDA dilengkapi **Interactive Router** yang menghindari rintangan secara otomatis:
- Aktif secara default saat Route Single Track
- Mode **Walk Around**: trace mengitari obstacle
- Mode **Shove**: mendorong trace lain untuk memberi ruang
- Ubah mode: **Route > Route Options**

### 8.4 Auto Router di EasyEDA

**Route > Auto Router:**
1. Atur parameter: active layers, default track width
2. Klik **Run**
3. Auto router akan mencoba menghubungkan semua ratsnest
4. Hasil ditampilkan → review dan perbaiki manual untuk jalur kritis

> **Penting:** Auto router tidak mempertimbangkan EMI dan arus. Selalu periksa dan perbaiki jalur power dan jalur kritis secara manual setelah auto route.

### 8.5 Via di EasyEDA

Via menghubungkan track antar layer:

**Menambah via:**
- Saat routing, tekan `Space` → layer berganti, via otomatis terbuat
- Atau `Place > Via` untuk via manual
- Klik kanan via → **Properties**: atur outer diameter dan drill size

**Ukuran via yang umum:**
| Via Type | Outer Diameter | Drill | Keterangan |
|---------|----------------|-------|-----------|
| Standard | 0.8 mm | 0.4 mm | Default, aman di JLCPCB |
| Mini | 0.6 mm | 0.3 mm | Untuk area padat |
| Micro (HDI) | < 0.4 mm | < 0.15 mm | Hanya board HDI khusus |

### 8.6 Copper Area (Ground Plane) di EasyEDA

Ground plane mengisi area kosong di layer dengan tembaga yang terhubung ke GND:

**Membuat Copper Area:**
1. Pilih layer `B.Cu` (BottomLayer)
2. Tekan `E` atau **Place > Copper Area**
3. Klik pojok-pojok area (biasanya seluruh board)
4. Klik kanan → **End Drawing** untuk menutup
5. Panel kanan → atur properties:
   - **Net**: `GND`
   - **Clearance**: 0.3 mm
   - **Fill Style**: Solid
   - **Thermal Relief**: Aktifkan (untuk THT mudah di-solder)
   - **Thermal Spoke Width**: 0.3 mm
   - **Remove Islands**: Auto

**Rebuild copper area:**
- Setelah edit routing: klik kanan Copper Area → **Rebuild Copper Area**
- Atau **Design > Rebuild All Copper Areas** (`Shift+B`)

> **Tips:** Buat copper area setelah semua routing selesai. Copper area di `B.Cu` yang terhubung GND menjadi ground plane yang sangat efektif mengurangi EMI dan menyederhanakan routing GND.

### 8.7 Highlight Net dan Cross Probe di EasyEDA

| Fitur | Cara | Fungsi |
|-------|------|--------|
| Highlight Net | Klik track/pad | Semua jalur net yang sama di-highlight |
| Cross Probe | Klik komponen di Schematic | Komponen di-highlight di PCB, begitu juga sebaliknya |
| Net Inspector | Design > Net Inspector | List semua net dan status routing |
| Design Manager | Design > Design Manager | Overview komponen, net, DRC |

Fitur Cross Probe sangat membantu saat debugging: klik komponen di skematik → langsung zoom ke footprint di PCB.

### 8.8 Pertimbangan Khusus Routing

**Jalur power switching (motor driver, step-down):**
- Track width sesuai arus (minimal 1 mm untuk 2A)
- Pendekkan jalur sebisa mungkin
- Tambah copper pour di sekitar area power

**Jalur sensor analog (photodioda):**
- Jauhkan dari jalur PWM dan switching power
- Gunakan AGND terpisah dan hubungkan ke GND di satu titik (Star Ground)
- Pertahankan trace pendek dan lurus

**Jalur I2C / SPI:**
- Panjang trace idealnya sama untuk SDA/SCL (matched length)
- Pastikan pull-up resistor dekat dengan master (ESP32)

---

## 9. DRC (DESIGN RULE CHECK)

DRC adalah pemeriksaan otomatis apakah layout PCB memenuhi semua aturan desain yang telah ditetapkan.

### 9.1 Menjalankan DRC di EasyEDA

1. **Design > DRC** (atau klik ikon DRC di toolbar)
2. Centang kategori rules yang ingin dicek
3. Klik **Run DRC Check**
4. Hasil muncul di panel **DRC Result** di bawah canvas
5. Klik setiap error → canvas otomatis zoom ke lokasi error

### 9.2 Kategori Error DRC

| Kategori | Contoh Error | Solusi |
|---------|------------|--------|
| Clearance | Track terlalu dekat track lain | Reroute, geser komponen |
| Unconnected | Ratsnest masih ada | Selesaikan routing |
| Short Circuit | Dua net berbeda bersentuhan | Hapus atau perbaiki trace |
| Copper Too Close to Board Edge | Trace / pad terlalu dekat outline | Geser (min 0.3 mm) |
| Drill Too Small | Via drill di bawah minimum | Perbesar drill via |
| Courtyard Overlap | Komponen terlalu berdekatan | Atur jarak placement |
| Silkscreen on Pad | Label teks menutupi pad | Geser silkscreen |
| Missing Annular Ring | Via/pad annular ring terlalu tipis | Perbesar pad/outer via |

### 9.3 Net Inspector

**Design > Net Inspector** menampilkan semua net beserta:
- Jumlah node per net
- Panjang total trace per net
- Status: Routed / Unrouted
- Filter untuk focus ke net tertentu

### 9.4 Alur Perbaikan DRC yang Efisien

```
Jalankan DRC → Error List → Klik error → Canvas zoom ke lokasi
→ Identifikasi penyebab → Perbaiki (reroute/geser) → DRC lagi
→ Ulangi hingga 0 error kritis
```

---

## 10. SILKSCREEN DAN LABELING

### 10.1 Elemen Wajib Silkscreen

| Elemen | Layer | Priority |
|--------|-------|---------|
| Reference Designator (R1, C3, U2) | F.SilkS | Wajib |
| Tanda pin 1 IC (titik/segitiga) | F.SilkS | Wajib |
| Polaritas kapasitor (+/-) | F.SilkS | Wajib |
| Label konektor (+5V, GND, TX, SERVO1) | F.SilkS | Wajib |
| Nama board dan versi (v1.0) | F.SilkS | Wajib |
| Nama kelompok/tanggal | F.SilkS | Wajib |
| Tanda arah motor, arah servo | F.SilkS | Disarankan |
| Logo institusi | F.SilkS | Opsional |

### 10.2 Menambah Silkscreen di EasyEDA

1. Pilih layer **F.SilkS** (TopSilkLayer) di panel layer
2. **Place > Text** → ketik teks → tentukan ukuran, ketebalan stroke
3. Minimum: tinggi teks 0.8 mm, ketebalan stroke 0.15 mm
4. Pastikan teks tidak berada di atas pad (akan hilang atau terpotong saat produksi)

**Verifikasi silkscreen:**
- **View > Photo View** untuk melihat tampilan realistis
- Aktifkan hanya layer SilkS + Mask untuk fokus cek silkscreen

### 10.3 Photo View

EasyEDA memiliki **Photo View** yang menampilkan PCB seperti foto nyata sebelum dikirim ke produksi:
- **View > Photo View**
- Bisa pilih warna soldermask (green/red/blue/black)
- Berguna untuk final visual check sebelum generate Gerber

---

## 11. 3D VIEW DAN MECHANICAL CHECK

### 11.1 Membuka 3D View di EasyEDA

1. **View > 3D View** (atau ikon 3D di toolbar)
2. Model 3D ditampilkan berdasarkan model yang terkait footprint
3. Navigasi: klik+drag untuk rotate, scroll untuk zoom, shift+drag untuk pan

### 11.2 Model 3D Komponen

- Komponen dari library LCSC umumnya sudah memiliki model 3D
- Model ditampilkan di 3D Viewer tanpa konfigurasi tambahan

**Menambah model 3D ke footprint kustom:**
1. Buka Footprint Editor → pada footprint kustom
2. Klik tab/tombol **3D Model**
3. **Add** → pilih file `.step` atau `.wrl`
4. Atur posisi dan rotasi model
5. Sumber model 3D: GrabCAD, SnapEDA, Ultra Librarian, datasheet pabrikan

### 11.3 Checklist 3D View

- [ ] Semua komponen muncul model 3D-nya
- [ ] Orientasi IC dan komponen polar benar
- [ ] Tidak ada komponen yang menabrak satu sama lain
- [ ] Tinggi komponen sesuai clearance casing
- [ ] Posisi konektor (USB, power, servo) sesuai desain mekanik
- [ ] Screenshot dari sudut atas (top) dan isometrik

---

## 12. GENERATE GERBER FILE DAN BOM

### 12.1 Standar Gerber

Gerber adalah format standar industri (RS-274X) untuk data manufaktur PCB. JLCPCB menerima Gerber yang dihasilkan langsung dari EasyEDA.

### 12.2 Layer Gerber yang Wajib Diekspor

| File | Layer EasyEDA | Ekstensi Tipikal |
|------|--------------|-----------------|
| Top Copper | F.Cu / TopLayer | .gtl |
| Bottom Copper | B.Cu / BottomLayer | .gbl |
| Top Soldermask | F.Mask | .gts |
| Bottom Soldermask | B.Mask | .gbs |
| Top Silkscreen | F.SilkS | .gto |
| Bottom Silkscreen | B.SilkS | .gbo |
| Board Outline | Board Outline | .gko |
| Drill File | Drill (NCDrill) | .drl / .xln |

### 12.3 Export Gerber di EasyEDA

**Cara langsung (rekomendasikan):**
1. **Fabrication > PCB Fabrication File (Gerber)**
2. Dialog gerber settings muncul
3. Layer yang diperlukan sudah otomatis tercentang
4. Klik **Generate Gerber** → file `.zip` otomatis ter-download
5. File sudah dikonfigurasi optimal untuk JLCPCB — tidak perlu pengaturan tambahan

> **Tips:** Jangan ubah pengaturan default Gerber di EasyEDA jika menggunakan JLCPCB. Format yang dihasilkan sudah sempurna untuk JLCPCB.

### 12.4 Export BOM (Bill of Materials)

1. **Fabrication > BOM**
2. Review daftar komponen
3. Centang kolom yang diperlukan: Reference, Value, Footprint, Quantity, **LCSC Part#**
4. **Export BOM** → download file CSV
5. BOM dengan LCSC Part# bisa langsung digunakan untuk JLCPCB SMT Assembly

### 12.5 Export Pick and Place (CPL)

Untuk JLCPCB SMT Assembly, diperlukan **Component Placement List**:
1. **Fabrication > Pick and Place File**
2. File CSV berisi: Reference, X, Y, Rotation, Layer per komponen
3. Digunakan bersamaan dengan Gerber + BOM untuk order SMT Assembly

### 12.6 Export Netlist

1. **Design > Netlist**
2. Pilih format: EasyEDA, Altium Designer, KiCad Netlist, PADS
3. Berguna untuk backup, kolaborasi, atau import ke software lain

---

## 13. VALIDASI GERBER

Selalu validasi file Gerber sebelum melakukan order:

### 13.1 Gerber Viewer

| Tool | URL | Keterangan |
|------|-----|-----------|
| **JLCPCB Gerber Viewer** | https://gerber-viewer.jlcpcb.com | Paling akurat untuk JLCPCB |
| EasyEDA Built-in Preview | Di halaman Export Gerber | Quick check |
| GerbLook | https://gerblook.org | Alternatif online |

### 13.2 Checklist Validasi Gerber

- [ ] Top copper tampil benar — trace, pad, via terlihat
- [ ] Bottom copper tampil benar
- [ ] Soldermask: tutupi semua area KECUALI pad (negatif)
- [ ] Silkscreen tidak menutupi pad
- [ ] Outline board (Edge.Cuts) tertutup sempurna tanpa celah
- [ ] Drill holes: ada semua (via dan through-hole), ukuran benar
- [ ] Dimensi board sesuai yang direncanakan
- [ ] Layer count sesuai (misal 2-layer: hanya Top dan Bottom Cu)

---

## 14. ORDER PCB KE JLCPCB

### 14.1 Ekosistem Terintegrasi EasyEDA–LCSC–JLCPCB

```
EasyEDA    →  desain schematic + PCB
LCSC       →  sumber komponen dengan Part Number
JLCPCB     →  manufaktur PCB ± SMT Assembly
```

Integrasi ini berarti:
- Pilih komponen di EasyEDA → harga LCSC langsung tampil
- Generate Gerber di EasyEDA → upload 1 klik ke JLCPCB
- LCSC Part# di BOM → JLCPCB bisa langsung rakit

### 14.2 Order Langsung dari EasyEDA

1. **Fabrication > Order at JLCPCB**
2. EasyEDA otomatis mengupload Gerber ke JLCPCB
3. Browser membuka halaman order JLCPCB dengan file sudah terupload
4. Pilih parameter produksi → tambah ke keranjang → checkout

### 14.3 Parameter Produksi JLCPCB

| Parameter | Pilihan Tersedia | Rekomendasi Proyek |
|----------|-----------------|-------------------|
| Base Material | FR4, Aluminium, Rogers | **FR4** |
| Layers | 1, 2, 4, 6, 8 | **2 Layer** |
| PCB Thickness | 0.8 / 1.0 / 1.2 / **1.6** / 2.0 mm | **1.6 mm** |
| PCB Color | Green / Red / Blue / Black / White / Yellow | **Green** (cepat + murah) |
| Silkscreen | White / Black | White |
| Surface Finish | HASL / HASL Lead-free / ENIG / OSP | **HASL Lead-free** |
| Copper Weight | **1 oz** / 2 oz / 3 oz | **1 oz** |
| Min Trace Width | 6/6 mil / 5/5 mil | Biarkan default |
| Via Covering | Tented / Untented / Plugged | **Tented** |
| Quantity | 5 / 10 / 15 / 20... | **5 pcs** (prototyping) |

### 14.4 Perbandingan Surface Finish

| Finish | Kelebihan | Kekurangan | Harga |
|--------|----------|-----------|------|
| HASL (dengan timbal) | Murah, solder mudah | Mengandung timbal | Paling murah |
| **HASL Lead-free** | Bebas timbal, solder mudah | — | Murah |
| ENIG (Au 2–4" thick) | Flat, oxidation-resistant, tahan lama | Mahal, cocok untuk QFN/BGA | Mahal |
| OSP | Flat, murah | Shelf life pendek | Murah |

> **Pilih ENIG** jika menggunakan komponen QFN atau BGA yang memerlukan permukaan flat sempurna.

### 14.5 JLCPCB SMT Assembly (Opsional)

Jika menginginkan board yang sudah dirakit:
1. Aktifkan **PCB Assembly** di halaman order
2. Upload: **Gerber** + **BOM** (dengan LCSC Part#) + **CPL** (dari EasyEDA Fabrication > Pick and Place)
3. JLCPCB memesan komponen dari LCSC dan merakit di fasilitas SMT mereka
4. Biaya tambahan: biaya rakit + biaya komponen dari LCSC

### 14.6 Estimasi Biaya JLCPCB (Referensi 2025–2026)

| Item | Estimasi |
|------|---------|
| PCB 5 pcs, ≤10×10 cm, 2-layer, HASL-LF, green | ~$5 USD |
| Shipping ke Indonesia (standar, 15–25 hari) | ~$8–$15 USD |
| Shipping ekspres DHL (5–7 hari) | ~$25–$50 USD |
| **Total estimasi (standar)** | **~$13–$20 USD ≈ Rp 210.000–320.000** |

> Pesan sebelum **6 Maret 2026**! Produksi 2–3 hari kerja + shipping ±2 minggu (standar).

---

## 15. REFERENSI

### Buku dan Standar

1. Horowitz, P., & Hill, W. (2015). *The Art of Electronics* (3rd ed.). Cambridge University Press.
2. IPC-2221B. (2012). *Generic Standard on Printed Board Design*. IPC.
3. IPC-7351C. (2017). *Generic Requirements for Surface Mount Design and Land Pattern Standard*. IPC.
4. Montrose, M. I. (2004). *EMC and the Printed Circuit Board*. Wiley-IEEE Press.

### Dokumentasi Software

5. EasyEDA Documentation. https://docs.easyeda.com
6. EasyEDA Pro Documentation. https://pro.easyeda.com/page/docs
7. EasyEDA Forum. https://easyeda.com/forum
8. LCSC Component Database. https://lcsc.com

### Dokumentasi Manufaktur

9. JLCPCB PCB Capabilities. https://jlcpcb.com/capabilities/Capabilities
10. JLCPCB SMT Assembly Guide. https://jlcpcb.com/smt-assembly
11. JLCPCB Gerber Viewer. https://gerber-viewer.jlcpcb.com
12. JLCPCB Trace Width Calculator. https://jlcpcb.com/pcb-impedance-and-trace-width-calculator
13. JLCPCB Design Guide. https://jlcpcb.com/help/article/Design-Guide

### Video Pembelajaran

14. EasyEDA Official Channel. (YouTube). https://www.youtube.com/@EasyEDA
15. Robert Feranec. (YouTube). *PCB Design Good Practices*. https://www.youtube.com/@RobertFeranec
16. Shawn Hymel / SparkFun. (YouTube). *EasyEDA Tutorial*. Tersedia di YouTube

### Artikel dan Referensi Online

17. Altium. (2024). *PCB Trace Width and Current Capacity*. https://resources.altium.com
18. GrabCAD 3D Models. https://grabcad.com/library
19. SnapEDA Component Library. https://www.snapeda.com

---

*Materi ini disusun untuk keperluan Praktikum Mekatronika dan Robotika.*  
*Software: EasyEDA | Manufaktur: JLCPCB | Versi: 2.0 | Februari 2026*
