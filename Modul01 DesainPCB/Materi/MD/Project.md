# PROJECT MODUL 01: PERANCANGAN PCB ROBOT OTONOM (LINE FOLLOWER + OBSTACLE AVOIDANCE)

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 01 – Desain PCB  
**Software:** EasyEDA Standard / Pro  
**Manufaktur:** JLCPCB  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

> ⚠️ **BATAS WAKTU ORDER PCB KE JLCPCB: 6 MARET 2026**  
> Order melalui EasyEDA → Fabrication → Order at JLCPCB  
> Kelompok yang terlambat order tidak dapat melanjutkan ke modul berikutnya

---

## A. DESKRIPSI PROYEK

Setiap kelompok merancang **dua buah PCB** untuk robot otonom sederhana yang mampu:
- Mengikuti garis hitam di atas latar putih (Line Follower)
- Mendeteksi dan menghindari rintangan di sisi kiri, depan, dan kanan (Obstacle Avoidance)

Desain PCB menggunakan **EasyEDA** dengan komponen bersumber dari **LCSC Electronics** dan diproduksi oleh **JLCPCB**.

---

## B. DESKRIPSI DAN SPESIFIKASI KEDUA PCB

### PCB 1 — PCB MAIN (Board Utama)

| Parameter | Spesifikasi |
|----------|------------|
| Dimensi | 120 mm × 80 mm |
| Layer | 2-layer (Top + Bottom) |
| Material | FR4, tebal 1.6 mm |
| Surface Finish | HASL Lead-free |
| Copper Weight | 1 oz |
| Solder Mask | Hijau |
| Silkscreen | Putih |

**Komponen utama pada PCB MAIN:**

| Komponen | Fungsi | Catatan |
|---------|--------|--------|
| ESP32-S2-WROOM-1 | MCU utama, WiFi | 3.3V logic |
| L293D | Motor Driver DC | 4 channel, Vsupply hingga 36V |
| LM2596-5.0 | Step-down 7.4V → 5V | Butuh induktor 68µH, kapasitor filter |
| AMS1117-3.3 | LDO 5V → 3.3V | Untuk supply ESP32 |
| HC-SR04 × 3 | Sensor ultrasonik | Level shifting: 5V ECHO → 3.3V |
| SG90 × 3 | Servo untuk mekanik | Konektor JST 3-pin |
| OLED 0.96" I2C | Display status | Konektor 4-pin (VCC, GND, SDA, SCL) |
| Push Button × 4 | Input manual | Pull-up eksternal 10kΩ |
| LED Indikator × 4 | Status power, mode | Resistor seri 100Ω |
| Buzzer | Peringatan / tone | Transistor NPN (2N2222) sebagai driver |
| Kapasitor dekoupling | Stabilisasi suplai | 100nF dekat setiap IC |

**Catatan desain PCB MAIN:**

- Level shifting HC-SR04: pin ECHO (5V output) diturunkan ke 3.3V menggunakan **voltage divider**  
  (R1 = 1kΩ seri, R2 = 2kΩ ke GND → output = 3.3V untuk input ESP32)
- Konektor motor DC: terminal block 2-pin (kiri/kanan), pastikan rating arus ≥ 3A
- Ground plane: buat Copper Area GND pada layer Bottom
- Decoupling cap: letakkan ≤ 2 mm dari pin VCC setiap IC

---

### PCB 2 — PCB Sensor Line (Board Sensor Garis)

| Parameter | Spesifikasi |
|----------|------------|
| Dimensi | 130 mm × 30 mm |
| Layer | 2-layer |
| Material | FR4, tebal 1.6 mm |
| Surface Finish | HASL Lead-free |
| Copper Weight | 1 oz |
| Solder Mask | Hitam (opsional) |
| Silkscreen | Putih |

**Komponen utama pada PCB Sensor Line:**

| Komponen | Fungsi | Catatan |
|---------|--------|--------|
| Photodioda × 12 | Sensor reflektif garis | Infrared, 5mm pitch |
| LED Infrared × 12 | Emitter garis | Resistor seri 68Ω |
| 74HC165 × 2 | Shift Register 8-bit parallel-in → serial-out | Multiplexing 12 sensor ke 3 pin SPI |
| Konektor IDC 10-pin | Interface ke PCB MAIN | VCC, GND, SPI (CLK, MISO, CS) |
| Resistor pull-down × 12 | Bias photodioda | 10kΩ |
| Trimpot × 2 | Threshold sensitivity | 10kΩ, opsional per 8 sensor |

**Catatan desain PCB Sensor Line:**

- Jarak antar sensor: **10 mm** center-to-center (optimal untuk garis lebar 25 mm)
- LED IR dan photodioda dipasang **sejajar**, arah menghadap lahan (ke bawah)
- 74HC165 dipilih daripada CD4051 karena:
  - Hanya butuh 3 jalur SPI (CLK, MISO, CS/LOAD)
  - Tidak memerlukan level shifting (compatible 3.3V)
  - Dapat di-daisy-chain untuk 16+ sensor
- Hubungan ke PCB MAIN: kabel flat 10-pin (IDC connector)

---

## C. DIAGRAM BLOK SISTEM

```
                     BATERAI 2S (7.4V)
                           |
                    ┌──────┴──────┐
                    │   LM2596   │  ──► 5V ──► L293D, HC-SR04, Servo, LED IR
                    │  (Step-Down)│
                    └─────────────┘
                           │
                    ┌──────┴──────┐
                    │  AMS1117   │  ──► 3.3V ──► ESP32-S2, OLED
                    │   (3.3V)   │
                    └─────────────┘

  ESP32-S2 ────────────────────────────────────────────
     │             │           │          │          │
     │           GPIO         SPI        I2C       ADC
     │          (PWM)     (Sensor Line) (OLED)  (Battery)
   L293D      HC-SR04 ×3  74HC165 ×2   SSD1306  Voltage Div
  (Motor)     (Ultrasonik) (Photodioda)
```

---

## D. BILL OF MATERIALS (BOM)

### D.1 PCB MAIN

| No | Komponen | Nilai/Tipe | Package | LCSC Part# | Qty | Harga Est. (Rp) |
|----|---------|-----------|---------|-----------|-----|----------------|
| 1 | ESP32-S2-WROOM-1 | ESP32-S2 | Module | C701342 | 1 | 75.000 |
| 2 | L293D | Motor Driver | SOIC-16 | C5948827 | 1 | 15.000 |
| 3 | LM2596-5.0 | Step-down 5V | TO-263-5 | C347412 | 1 | 22.000 |
| 4 | AMS1117-3.3 | LDO 3.3V | SOT-223 | C6186 | 1 | 4.000 |
| 5 | Induktor 68µH | Power / SMD | CD43 | C1046 | 1 | 5.000 |
| 6 | Kapasitor 1000µF/16V | Elco filter | THT | — | 2 | 6.000 |
| 7 | Kapasitor 100nF/50V | Decoupling | 0805 SMD | C49678 | 10 | 5.000 |
| 8 | Resistor 1kΩ | Level shift HC-SR04 | 0805 SMD | C17513 | 3 | 2.000 |
| 9 | Resistor 2kΩ | Level shift HC-SR04 | 0805 SMD | C17514 | 3 | 2.000 |
| 10 | Resistor 10kΩ | Pull-up button | 0805 SMD | C17516 | 4 | 2.000 |
| 11 | Resistor 100Ω | LED indikator | 0805 SMD | C17408 | 4 | 2.000 |
| 12 | Transistor 2N2222 | Driver buzzer | SOT-23 | C223962 | 1 | 3.000 |
| 13 | Dioda 1N4007 | Flyback L293D | DO-41 | C727679 | 4 | 4.000 |
| 14 | Push Button 6×6mm | Input | THT | C2842429 | 4 | 8.000 |
| 15 | LED 3mm merah | Indikator | THT | — | 4 | 4.000 |
| 16 | Buzzer aktif 5V | Alarm | THT | C2696235 | 1 | 5.000 |
| 17 | OLED 0.96" I2C | Display | Module | — | 1 | 25.000 |
| 18 | HC-SR04 | Ultrasonik | Module | — | 3 | 45.000 |
| 19 | SG90 | Servo 9g | — | — | 3 | 45.000 |
| 20 | Terminal Block 2-pin | Motor | THT | C9900000252 | 2 | 6.000 |
| 21 | Pin Header 2.54mm | Koneksi antar-board | THT | C9900003044 | 1 paket | 10.000 |
| 22 | PCB MAIN 120×80 | Custom, JLCPCB | — | — | 5 pcs | 45.000 |
| **SUBTOTAL PCB MAIN** | | | | | | **~345.000** |

### D.2 PCB Sensor Line

| No | Komponen | Nilai/Tipe | Package | LCSC Part# | Qty | Harga Est. (Rp) |
|----|---------|-----------|---------|-----------|-----|----------------|
| 1 | Photodioda infrared | 5mm, 940nm | THT | C434455 | 12 | 24.000 |
| 2 | LED Infrared 5mm | 940nm, 100mA | THT | C2153 | 12 | 18.000 |
| 3 | 74HC165 | Shift Register | SOP-16 | C5561 | 2 | 8.000 |
| 4 | Resistor 68Ω | Limit arus LED IR | 0805 SMD | C17408 | 12 | 6.000 |
| 5 | Resistor 10kΩ | Pull-down photodioda | 0805 SMD | C17516 | 12 | 6.000 |
| 6 | Kapasitor 100nF | Decoupling | 0805 SMD | C49678 | 2 | 2.000 |
| 7 | IDC Connector 10-pin | Interface ke MAIN | THT | C225480 | 2 | 10.000 |
| 8 | PCB Sensor 130×30 | Custom, JLCPCB | — | — | 5 pcs | 35.000 |
| **SUBTOTAL PCB SENSOR** | | | | | | **~109.000** |

### D.3 Sistem dan Daya

| No | Komponen | Keterangan | Harga Est. (Rp) |
|----|---------|-----------|----------------|
| 1 | Baterai LiPo 3.7V 2000mAh × 2 | Seri = 7.4V 2000mAh | 120.000 |
| 2 | Holder baterai 2S | Atau custom mount 3D print | 25.000 |
| 3 | Charger LiPo 2S (TP4056 atau B6) | Modul charger | 35.000 |
| 4 | Motor DC 3–6V with encoder | 2 buah (kiri & kanan) | 80.000 |
| 5 | Roda + gear set | Menyesuaikan motor | 40.000 |
| 6 | Kabel ribbon 10-pin | Penghubung kedua PCB | 15.000 |
| 7 | Baut + spacer M3 | Mounting PCB ke chassis | 20.000 |
| **SUBTOTAL SISTEM** | | | **~335.000** |

---

### Estimasi Biaya Total

| Komponen | Biaya (Rp) |
|---------|-----------|
| PCB MAIN (BOM + PCB) | ~345.000 |
| PCB Sensor Line (BOM + PCB) | ~109.000 |
| Sistem & Daya | ~335.000 |
| **TOTAL PER KELOMPOK** | **~789.000** |
| **Per Orang (4 orang)** | **~197.250** |

*Harga estimasi, dapat berubah. Cek harga terkini di toko komponen dan JLCPCB saat order.*

---

## E. PEMBAGIAN TUGAS PER ANGGOTA

Setiap kelompok **wajib** mengisi tabel berikut dan menempelkannya di bagian awal laporan:

| No | Anggota | NIM | Tanggung Jawab |
|----|--------|-----|---------------|
| 1 | | | Schematik + layout PCB MAIN bagian Power (LM2596, AMS1117) |
| 2 | | | Schematik + layout PCB MAIN bagian MCU (ESP32-S2, OLED, button) |
| 3 | | | Schematik + layout PCB MAIN bagian Driver (L293D, HC-SR04, servo) |
| 4 | | | Schematik + layout penuh PCB Sensor Line (photodioda, 74HC165) |

*Setiap anggota mengerjakan bagiannya masing-masing di EasyEDA, kemudian digabungkan (Panel/merge) menjadi satu file project EasyEDA bersama.*

---

## F. ALUR KERJA PROYEK

### F.1 Tahapan

```
[Minggu 1]  Bagi tugas → masing-masing anggota mulai schematic di EasyEDA
            Cari semua komponen di LCSC, verifikasi stok dan harga
            
[Minggu 2]  Selesaikan schematic → ERC bersih
            Konversi ke PCB layout (Update PCB)
            Board Outline, Design Rules sesuai JLCPCB
            
[Minggu 3]  Placement komponen → Routing → Copper Area (ground plane)
            DRC bersih → Silkscreen → 3D View review
            Generate Gerber + BOM + Pick&Place
            
[Minggu 4]  Validasi Gerber di https://gerber-viewer.jlcpcb.com
            ⚠️ ORDER KE JLCPCB: BATAS 6 MARET 2026
            Kumpulkan file project EasyEDA + Gerber ke dosen
```

### F.2 Cara Order via EasyEDA → JLCPCB

1. Buka PCB Editor di EasyEDA
2. Klik menu **Fabrication > Order at JLCPCB**
3. EasyEDA akan otomatis mengupload Gerber ke JLCPCB
4. Di halaman JLCPCB, set parameter:
   - **PCB Qty**: minimal 5 pcs
   - **Layers**: 2
   - **Thickness**: 1.6 mm
   - **Color**: Green
   - **Surface Finish**: HASL(with lead) atau HASL Lead-free
   - **Copper Weight**: 1 oz
5. Klik **Save to Cart** → lanjutkan ke checkout
6. Screenshot halaman konfirmasi order sebagai bukti

---

## G. SPESIFIKASI DESAIN JLCPCB

Pastikan seluruh desain memenuhi **kemampuan produksi JLCPCB** berikut:

| Parameter | Nilai Minimum JLCPCB | Target Proyek |
|----------|---------------------|--------------|
| Track width | 0.09 mm (3.5 mil) | Min. 0.2 mm (sinyal), 0.5 mm (power) |
| Clearance track | 0.09 mm (3.5 mil) | Min. 0.2 mm |
| Via diameter | 0.3 mm | 0.6 mm (drill) + 1.0 mm (pad) |
| Via drill | 0.2 mm | 0.3 mm |
| Pad to edge | 0.3 mm | Min. 0.5 mm |
| Min. annular ring | 0.13 mm | 0.2 mm |
| Solder mask opening | 0.05 mm | Default EasyEDA |
| Min. silkscreen | 0.15 mm | Default EasyEDA |

*Atur Design Rules di EasyEDA: Design > Design Rule untuk memastikan DRC menggunakan nilai di atas.*

---

## H. DELIVERABLES (YANG HARUS DIKUMPULKAN)

### H.1 File Digital (dikumpulkan di LMS / Google Drive Kelompok)

| No | File | Format | Keterangan |
|----|------|--------|-----------|
| 1 | Project EasyEDA (PCB MAIN) | `.epro` / Share Link | File lengkap dari EasyEDA |
| 2 | Project EasyEDA (Sensor Line) | `.epro` / Share Link | File lengkap dari EasyEDA |
| 3 | Gerber PCB MAIN | `.zip` | Dari Fabrication > Generate Gerber |
| 4 | Gerber Sensor Line | `.zip` | Dari Fabrication > Generate Gerber |
| 5 | BOM PCB MAIN | `.csv` | Dari Fabrication > BOM |
| 6 | BOM Sensor Line | `.csv` | Dari Fabrication > BOM |
| 7 | Bukti order JLCPCB | `.png` / `.pdf` | Screenshot konfirmasi order |
| 8 | Laporan singkat kelompok | `.pdf` | Lihat H.2 |

### H.2 Isi Laporan Singkat Kelompok (Maks. 10 Halaman)

1. Cover: nama kelompok, anggota, NIM
2. Deskripsi pembagian tugas per anggota
3. Diagram blok sistem (boleh dari dokumen ini)
4. Screenshot skematik (annotated)
5. Screenshot PCB layout 2D (top layer + bottom layer)
6. Screenshot 3D View (PCB MAIN + Sensor Line)
7. Screenshot Gerber Viewer JLCPCB (semua layer)
8. Screenshot halaman order JLCPCB (sebelum/sesudah bayar)
9. BOM final (tempel tabel)
10. Kendala yang ditemui dan solusinya

---

## I. RUBRIK PENILAIAN PROJECT KELOMPOK

**Total: 100 poin**

| No | Kriteria | Bobot | Keterangan |
|----|---------|-------|-----------|
| 1 | Kelengkapan schematic (semua komponen, pin, net label, ERC bersih) | 20 | Cek jumlah komponen vs BOM |
| 2 | Kualitas PCB layout (placement logis, ground plane, track width sesuai) | 25 | PCB MAIN + Sensor Line |
| 3 | DRC bersih dari error kritis | 15 | Screenshot DRC result |
| 4 | Gerber valid di JLCPCB Viewer (semua layer benar) | 10 | Screenshot GB viewer |
| 5 | BOM lengkap dengan LCSC Part Number | 10 | File CSV |
| 6 | Order berhasil / bukti simulasi order JLCPCB | 10 | Screenshot order |
| 7 | Laporan: pembagian tugas, screenshot, kendala | 10 | PDF laporan |
| **TOTAL** | | **100** | |

---

## J. REFERENSI

- EasyEDA Editor: https://easyeda.com/editor
- EasyEDA Dokumentasi: https://docs.easyeda.com
- LCSC Electronics: https://www.lcsc.com
- JLCPCB Capabilities: https://jlcpcb.com/capabilities/pcb-capabilities
- JLCPCB Gerber Viewer: https://gerber-viewer.jlcpcb.com
- JLCPCB SMT Assembly: https://jlcpcb.com/smt-assembly

---

*Dokumen ini diperbarui untuk penggunaan EasyEDA dan JLCPCB. Hubungi dosen jika ada pertanyaan mengenai spesifikasi komponen atau jadwal order.*
