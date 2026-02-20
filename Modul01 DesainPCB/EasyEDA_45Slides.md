# EasyEDA Tutorial — Rangkuman 45 Slide

**Sumber:** EasyEDA-Tutorial_v6.4.32.pdf | **Mata Kuliah:** Desain PCB – Mekatronika & Robotika

---

## SLIDE 01 — JUDUL

# EasyEDA: Panduan Lengkap Desain PCB

**Versi 6.4.32 | 2022**

- Web-based EDA (Electronics Design Automation)
- URL Editor: https://easyeda.com/editor
- Tersedia desktop client & versi online

---

## SLIDE 02 — APA ITU EasyEDA?

### Definisi

- Tool EDA berbasis cloud, gratis, tanpa instalasi
- Berjalan di browser HTML5 (Chrome/Firefox)
- Platform: Windows, Mac, Linux

### Ekosistem

- EasyEDA Editor → desain online
- LCSC → pembelian komponen
- JLCPCB → fabrikasi PCB

---

## SLIDE 03 — FITUR UTAMA EasyEDA

| Fitur               | Keterangan                        |
| ------------------- | --------------------------------- |
| Schematic Capture   | Pembuatan skematik profesional    |
| PCB Layout          | Layout hingga 6 layer             |
| LTSpice Simulation  | Simulasi rangkaian berbasis spice |
| Library 1M+         | Simbol & footprint siap pakai     |
| Cloud Collaboration | Kerja tim real-time               |
| Gerber Export       | File produksi PCB                 |
| Import/Export       | Altium, Eagle, KiCAD              |

---

## SLIDE 04 — ALUR DESAIN (Design Flow)

```
Buat Proyek
    ↓
Desain Skematik
    ↓
Assign Footprint
    ↓
Generate Netlist / Convert ke PCB
    ↓
Layout PCB
    ↓
Design Rule Check (DRC)
    ↓
Generate Gerber
    ↓
Order PCB (JLCPCB)
```

---

## SLIDE 05 — PERBANDINGAN DENGAN TOOL LAIN

| Aspek      | EasyEDA             | Altium   | KiCAD     |
| ---------- | ------------------- | -------- | --------- |
| Harga      | Gratis              | Berbayar | Gratis    |
| Platform   | Web/Desktop         | Windows  | Multi-OS  |
| Library    | 1M+ komponen        | Luas     | Komunitas |
| Kolaborasi | Real-time           | Terbatas | Terbatas  |
| Fabrikasi  | Terintegrasi JLCPCB | Manual   | Manual    |

---

## SLIDE 06 — ANTARMUKA PENGGUNA (UI)

### Komponen UI EasyEDA

1. **Top Menu** – File, Edit, Design, Route, Tools, dll.
2. **Left Navigation Panel** – Project, EELib, Library, LCSC
3. **Canvas** – Area desain utama
4. **Right Properties Panel** – Atribut objek terpilih
5. **Wiring/Drawing Tools** – Toolbar alat gambar
6. **Layer Toolbar** – Pemilihan layer aktif

---

## SLIDE 07 — PANEL NAVIGASI KIRI

- **Project** – Semua proyek (private/public/fork)
- **EElib** – Simbol siap simulasi
- **Library** – Simbol & footprint publik + milik sendiri
- **LCSC** – Katalog komponen untuk dibeli
- **Design Manager** – Cek komponen, net, DRC
- **Filter**: Ketik kata kunci → hasil instan (Shift+F)

---

## SLIDE 08 — MEMBUAT PROYEK BARU

**Langkah:**

1. Login ke EasyEDA
2. `File > New > Create a new project`
3. Isi: **Title**, **Path** (unik, tidak bisa diubah), **Description**
4. Pilih **Owner** (akun pribadi / tim)
5. Atur visibilitas: **Private** (default) atau **Public**

> 💡 Gunakan Markdown di kolom Description untuk tampilan lebih rapi

---

## SLIDE 09 — MEMBUAT FILE DI PROYEK

Dalam satu proyek dapat dibuat:

- **Schematic** – Desain skematik elektronik
- **PCB** – Layout PCB
- **Symbol** – Simbol komponen kustom
- **Footprint** – Jejak PCB kustom

`File > New > Schematic / PCB / Symbol / Footprint`

---

## SLIDE 10 — PENGATURAN CANVAS SKEMATIK

**Klik area kosong canvas → Properties muncul di kanan**

- **Background Color** – Warna latar (hex/palette)
- **Grid Style** – Line / Dot
- **Grid Size** – Disarankan: 10, 20, atau 100 pixel
- **Snap = Yes** – Wajib aktif untuk keselarasan komponen
- **ALT Snap** – Snap lebih halus (tahan tombol ALT)
- **Canvas Size** – Atur lebar × tinggi atau pilih frame preset (A4, dst)

---

## SLIDE 11 — WIRING TOOLS — PENGANTAR

Tools khusus untuk elektronik (bukan sekadar gambar garis):

| Ikon/Hotkey | Fungsi                  |
| ----------- | ----------------------- |
| W           | Gambar Wire (sambungan) |
| B           | Bus                     |
| U           | Bus Entry               |
| N           | Net Label               |
| P           | Place Pin               |
| Ctrl+G      | NetFlag GND             |
| Ctrl+Q      | NetFlag VCC             |

---

## SLIDE 12 — MENGGAMBAR WIRE (SAMBUNGAN)

**3 Cara Masuk Mode Wire:**

1. Klik tombol Wire di toolbar
2. Tekan hotkey `W`
3. Klik ujung pin komponen (titik abu-abu muncul)

**Fitur Auto-Connect:**

- Letakkan resistor/kapasitor di atas wire → wire otomatis terhubung ke kedua pin
- Rubber band: saat komponen dipindah, wire ikut bergerak

---

## SLIDE 13 — NET LABEL & NET FLAG

### Net Label (N)

- Memberi nama pada wire
- Dua wire dengan nama label **sama = terhubung**
- Ganti nama: double-click netlabel

### Net Flag

- `Ctrl+G` → GND
- `Ctrl+Q` → VCC (+5V)
- Ubah nama (mis. +3.3V, VDD) di Properties panel

> ⚠️ Hanya gunakan karakter huruf Inggris & angka Arab

---

## SLIDE 14 — BUS & BUS ENTRY

### Bus (B)

- Representasi kumpulan sinyal (misal: data bus 8-bit)
- Hanya sebagai **indikasi visual**, bukan koneksi otomatis

### Bus Entry (U)

- Penghubung wire individual ke bus
- Harus tetap dilabeli dengan **Net Label** di setiap bus entry

**Contoh Penggunaan:**

```
DB[0..7] ──┬── Bus Entry ──── Wire ──── Pin IC
            ├── Bus Entry ──── Wire ──── Pin IC
            └── ...
```

---

## SLIDE 15 — NET PORT & NO CONNECT FLAG

### Net Port

- Alternatif pengganti wire panjang
- Membuat skematik lebih rapi
- Beri nama port = nama net

### No Connect Flag (X)

- Dipasang di pin yang **sengaja tidak disambung**
- Menghilangkan error "unconnected pin" di Design Manager
- Hanya berlaku jika langsung menyentuh pin simbol

---

## SLIDE 16 — VOLTAGE PROBE & SIMULASI DASAR

- **Voltage Probe**: Diletakkan di wire untuk merekam tegangan saat simulasi
- Jalankan simulasi: `F8` atau `Menu > Design > Run Simulation`
- Pengaturan simulasi: `Ctrl+J`
- EasyEDA menggunakan engine **LTSpice** → hanya komponen dengan model SPICE yang bisa disimulasikan
- Output: **Waveform viewer** (bisa export CSV)

---

## SLIDE 17 — LIBRARY KOMPONEN (EElib)

- Kumpulan simbol komponen lengkap dengan model simulasi
- Tersedia gaya **US** dan **EU**
- Cara akses: Panel kiri → EELib
- Filter cepat: ketik nama/nilai (misal: `0603`, `NE555`, `1k`)
- Klik *drop-down* untuk melihat footprint & parameter tersedia

---

## SLIDE 18 — LIBRARY UMUM (1 JUTA+ Komponen)

- Akses: Panel kiri → **Library** | Hotkey: `Shift+F`
- Mencakup simbol dari: **KiCAD**, **Eagle** (open source)
- Langkah pencarian:
  1. Pilih tipe (Symbol / Footprint)
  2. Ketik kata kunci (mis. `ATmega328 DIP`)
  3. Klik Search
  4. Pilih komponen → Preview muncul
  5. Klik **Place** untuk memasang ke canvas

---

## SLIDE 19 — INTEGRASI LCSC

- LCSC (**Love Components Save Cost**), satu grup dengan EasyEDA & JLCPCB
- Langsung akses dari panel kiri: **LCSC**
- **Keuntungan:**
  - Simbol + footprint sudah terverifikasi
  - Minimum order rendah, global shipping
  - 25.000+ jenis komponen asli
  - Link langsung ke pembelian dari skematik

---

## SLIDE 20 — MEMBUAT SYMBOL KUSTOM

**Cara 1: Symbol Wizard** (`Shift+T`)

- Cepat membuat simbol DIP, QFP, SIP
- Isi jumlah pin, nama, prefix → otomatis digenerasi

**Cara 2: Dari Nol**

- `File > New > Symbol`
- Gambar badan simbol dengan Drawing Tools
- Tambahkan Pin (`P`) – wajib untuk menghubungkan wire
- Simpan → tampil di Library > Workspace

**Cara 3: Copy & Edit**

- Fork simbol publik → edit → simpan ke library pribadi

---

## SLIDE 21 — PIN PROPERTIES (SIMBOL)

Saat membuat simbol, setiap pin memiliki atribut:

- **Name** – Nama pin (mis. VCC, GND, D0)
- **Number** – Nomor urut pin
- **Electrical Type** – Input, Output, BiDirect, Power, dsb.
- **Pin Length** – Panjang garis pin
- Tampilkan bar inverter: tambahkan `#` di akhir nama pin

---

## SLIDE 22 — FOOTPRINT MANAGER

- Akses: `Alt+F` atau `Menu > Tools > Footprint Manager`
- Assosasikan setiap simbol dengan footprint PCB yang tepat
- **Langkah:**
  1. Pilih komponen di skematik
  2. Klik Footprint Manager
  3. Cari footprint (mis. `R_0603`)
  4. Klik **Update**
- Validasi assignment sebelum convert ke PCB

---

## SLIDE 23 — MULTI-SHEET SCHEMATIC

- EasyEDA mendukung **multiple sheets** dalam satu proyek
- **Hierarchy tidak didukung** – sheets berdiri sendiri
- Sambungkan antar sheet menggunakan **Net Label** / **Net Port** yang sama
- Tambah sheet: klik ikon `+` di tab bawah editor
- Rename sheet: double-click tab atau klik kanan → Modify

---

## SLIDE 24 — KONVERSI SKEMATIK → PCB

**Prasyarat:**

1. Semua komponen sudah punya footprint valid
2. Tidak ada *prefix conflict* (misal, dua komponen nama sama)
3. Tidak ada footprint invalid

**Langkah:**

- `Menu > Design > Convert to PCB`

**Update PCB (saat ada perubahan skematik):**

- `Menu > Design > Update PCB`
- Atau dari PCB Editor: `Menu > Design > Import Changes`

---

## SLIDE 25 — PCB EDITOR — PENGANTAR

### Tampilan Dasar PCB Canvas

- **Layers Toolbar** – Pilih layer aktif (Top, Bottom, Inner, Silkscreen, dll.)
- **PCB Tools** – Track, Via, Pad, Copper Area, dsb.
- **Design Manager** – Cek komponen, net, pad
- **Layer Manager**: `Top Menu > Tools > Layer Manager` (Ctrl+L)

> Saran unit: gunakan **mm** untuk proyek profesional
> Ubah satuan: `Q` di keyboard

---

## SLIDE 26 — LAYER PCB

| Layer              | Fungsi                    |
| ------------------ | ------------------------- |
| Top Copper         | Jalur tembaga sisi atas   |
| Bottom Copper      | Jalur tembaga sisi bawah  |
| Inner 1–4         | Layer dalam (multilayer)  |
| Top/Bot Silkscreen | Label & outline komponen  |
| Top/Bot SolderMask | Masker solder (hijau)     |
| Board Outline      | Batas board PCB           |
| Mechanical         | Garis dimensi/dokumentasi |
| Drill              | Titik lubang bor          |

---

## SLIDE 27 — MENEMPATKAN KOMPONEN DI PCB

**Tips penempatan:**

- Gunakan **Drag mode** (`D`) agar lebih mudah memilih footprint
- Layer bottom: komponen otomatis ditempatkan di bawah jika layer aktif = Bottom
- Pindah ke bottom: pilih komponen → ubah layer di Properties panel
- Gunakan **Ratline** (garis putus-putus) sebagai panduan koneksi yang belum dihubungkan

---

## SLIDE 28 — ROUTING TRACK (JALUR PCB)

**Hotkey: `W`** (dalam PCB editor)

**Pilihan routing:**

- Manual routing – klik titik awal → titik akhir
- **Auto Router** – `Menu > Route > Auto Route`

**Pengaturan Lebar Track:**

- `Shift+W` – tampilkan daftar lebar favorit
- `Shift+G` – tampilkan panjang track real-time
- Ubah sudut routing: `L` atau `Ctrl+Shift+Space`

**Hapus semua track:** `Menu > Route > Unroute All`

---

## SLIDE 29 — VIA, PAD & COPPER AREA

### Via

- Hotkey: `V` – koneksi antar layer
- Atur ukuran via di Properties panel

### Pad

- Hotkey: `P` – titik soldering manual

### Copper Area (Tembaga Masif)

- Hotkey: `E` – isi area tembaga (mis. ground plane)
- `Shift+B` → Rebuild semua copper area
- `Shift+M` → Hapus semua fill data sementara

---

## SLIDE 30 — BOARD OUTLINE (BATAS PCB)

- Layer: **Board Outline**
- Gambar dengan: Line, Arc, atau Rectangle
- **Import DXF** untuk outline custom: `Menu > File > Import > DXF`
- Outline bulat: gambar Arc di layer Board Outline
- Slot/cutout:
  - Gunakan **Solid Region** di layer Board Outline
  - Atau gambar track → right-click → **Convert to Board Cutout**

---

## SLIDE 31 — DESIGN RULE CHECK (DRC)

- Akses: `Menu > Design > Design Rule` atau `Ctrl+D`
- **Parameter DRC:**
  - Clearance minimum antar track
  - Minimum track width
  - Minimum via hole size
  - Solder mask expansion

**Cara pakai:**

1. Atur parameter DRC sesuai kemampuan fabrikasi (mis. JLCPCB: min 0.1mm)
2. Jalankan DRC
3. Perbaiki semua error yang terdeteksi
4. Pastikan 0 error sebelum generate Gerber

---

## SLIDE 32 — SOLDER MASK & COPPER EXPOSE

**Menambah aperture solder mask:**

1. Tambahkan layer Top/Bottom Solder Mask
2. Gambar region di layer tersebut di atas area tembaga
3. Atau: klik track → klik tombol **Expose Copper** di panel kanan

> ⚠️ Pastikan ada copper di bawah aperture mask, bukan hanya mask tanpa tembaga

---

## SLIDE 33 — TAMPILAN 3D & PHOTO VIEW

### 3D View

- `Menu > View > 3D View`
- Visualisasi board secara 3D (membutuhkan model 3D footprint)
- Berguna untuk cek tata letak sebelum pesan

### Photo View

- `Menu > View > Photo View`
- Simulasi tampilan PCB jadi seperti foto board asli
- Cocok untuk dokumentasi & presentasi proyek

---

## SLIDE 34 — MEMBUAT FOOTPRINT KUSTOM

**Langkah:**

1. `File > New > Footprint`
2. Atur layer aktif sesuai elemen yang digambar
3. Tambahkan **Pad** (`P`) sesuai datasheet komponen
4. Gambar **Silkscreen** outline
5. Tambahkan **Courtyard** area
6. Simpan → tersedia di Library > Workspace

**Tips:**

- Selalu cek datasheet untuk dimensi pad & pitch
- Gunakan grid kecil (0.1 mm) untuk akurasi

---

## SLIDE 35 — GENERATE GERBER FILE

**Langkah:**

1. Pastikan DRC 0 error
2. `Menu > Fabrication > Gerber Export`
3. Pilih layer yang ingin di-export
4. Klik **Export Gerber**
5. File `.zip` akan didownload

### Gerber Viewer

- Verifikasi Gerber sebelum order: `Menu > Fabrication > Gerber View`
- Software offline: **Gerbv** (open source) – http://gerbv.geda-project.org/

---

## SLIDE 36 — EXPORT BOM (BILL OF MATERIALS)

- `File > Export BOM` atau `Menu > Fabrication > BOM`
- BOM berisi: Referensi, Nilai, Footprint, Kuantitas
- Format: **CSV / Excel**
- Bisa langsung digunakan untuk pemesanan di **LCSC**

**Tips:**

- Pastikan semua komponen punya **LCSC Part Number** untuk order mudah
- Cek harga & stok sebelum finalisasi desain

---

## SLIDE 37 — ORDER PCB via JLCPCB

**Langkah Order:**

1. Verifikasi Gerber dengan Gerber Viewer
2. Buka https://jlcpcb.com/quote
3. Upload file Gerber `.zip`
4. Atur opsi: jumlah layer, warna mask, jumlah board
5. **Tambah ke Cart** → Bayar

**Spesifikasi umum JLCPCB:**

- Min. track: 0.1 mm / 4 mil
- Min. clearance: 0.1 mm
- Warna mask: Hijau, Merah, Biru, Hitam, Putih, Kuning

---

## SLIDE 38 — IMPORT FILE (ALTIUM / EAGLE / KICAD)

EasyEDA mendukung import dari tool EDA lain:

| Format                    | Tipe                       |
| ------------------------- | -------------------------- |
| Altium / Protel DXP ASCII | Schematic & PCB            |
| Eagle                     | Schematic, PCB, Library    |
| KiCAD                     | Schematic, PCB, Library    |
| DXF                       | Board Outline / Mechanical |

**Langkah:** `Menu > File > Import > [pilih format]`

---

## SLIDE 39 — EXPORT FILE

**Format export yang tersedia:**

- **EasyEDA Source (JSON)** – backup/share
- **Altium Designer Format** – kompatibilitas dengan Altium
- **PDF / PNG / SVG** – dokumentasi
- **Gerber** – fabrikasi PCB
- **Pick and Place File** – pemasangan SMD otomatis
- **DXF** – gambar teknik mechanical
- **Netlist** – Spice, Protel, PADS, FreePCB

---

## SLIDE 40 — VERSION CONTROL

- Setiap proyek bisa memiliki hingga **10 versi**
- Default nama versi: **master**
- Buat versi baru: `Project folder > Right-click > Version > New Version`
- Pindah versi: `Version > Switch Version` (tutup dokumen aktif dulu)
- Kelola versi: `Project Manage > Version` (edit nama, deskripsi, hapus)

> 💡 Versi aktif tidak bisa dihapus

---

## SLIDE 41 — KOLABORASI TIM (PROJECT MEMBER)

**Cara berbagi proyek:**

1. Set proyek menjadi **Private + Add Member**
2. Masukkan email anggota tim
3. Pilih peran: **Observer**, **Developer**, **Manager**

**Berbagi publik:**

- Right-click proyek → **Share → Public**
- Siapa pun bisa melihat & fork proyek

> ⚠️ Simpan terakhir **menimpa** semua simpan sebelumnya — upload bergiliran

---

## SLIDE 42 — HISTORY & DOCUMENT RECOVERY

### History Record

- Right-click file → **History**
- Daftar versi tersimpan → klik nomor untuk membuka versi lama
- Terlalu sering menyimpan = banyak history → sulit dicari

### Document Recovery (Auto-Save)

- `Menu > Advanced > Document Recovery`
- Tersimpan di **komputer lokal** (bukan server)
- Jangan bersihkan cache browser jika ada file penting
- Recovery tidak berlaku jika ganti browser/komputer

---

## SLIDE 43 — DESKTOP CLIENT EasyEDA

- Download: https://easyeda.com/page/download
- Tersedia: Windows, Mac, Linux

**Mode Operasi:**

| Mode                  | Deskripsi                           |
| --------------------- | ----------------------------------- |
| Team Work Mode        | Full fitur, project di cloud server |
| Projects Offline Mode | Project di lokal, library di cloud  |
| Full Offline Mode     | Untuk perusahaan, berbayar          |

**Setting cache/update:** Right-click start page → Setting → Remove Cache

---

## SLIDE 44 — SHORTCUT KEY PENTING

| Hotkey     | Fungsi                     |
| ---------- | -------------------------- |
| Ctrl+S     | Simpan                     |
| Ctrl+Z / Y | Undo / Redo                |
| Ctrl+A     | Select All                 |
| Shift+F    | Cari Library               |
| W          | Wire (SCH) / Track (PCB)   |
| A / Z      | Zoom In / Out              |
| D          | Drag Mode                  |
| M          | Measure Dimension (PCB)    |
| V          | Place Via (PCB)            |
| Space      | Rotate                     |
| X / Y      | Flip Horizontal / Vertical |
| Delete     | Hapus                      |
| F1         | Buka Tutorial              |

---

## SLIDE 45 — RINGKASAN & BEST PRACTICES

### Checklist Sebelum Order PCB ✅

- [ ] Semua footprint sudah di-assign & valid
- [ ] DRC = 0 error
- [ ] Board outline tertutup sempurna
- [ ] Gerber diverifikasi di Gerber Vewer
- [ ] Via, track width sesuai spec fabrikator
- [ ] BOM sudah diexport & stock komponen tersedia
- [ ] Tampilan 3D/Photo View sudah dicek

### Tips Produktivitas

- Aktifkan **Snap** sepanjang waktu
- Gunakan **Net Label** alih-alih wire panjang
- Simpan sering → manfaatkan **Version Control**
- Gunakan **LCSC Part Number** untuk integrasi mudah

---

*Dokumen ini merangkum EasyEDA Tutorial v6.4.32 untuk keperluan pembelajaran Praktikum Mekatronika & Robotika.*
