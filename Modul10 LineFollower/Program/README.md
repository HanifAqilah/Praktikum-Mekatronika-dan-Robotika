# Program Line Follower - Panduan Umum

## Daftar Program

| No | Nama Program | Tujuan | Durasi Estimasi |
|----|--------------|--------|----------------|
| 00 | SystemTest | Validasi hardware sebelum praktikum | 20 menit |
| 01 | LcdI2c | Test komunikasi I2C dengan OLED | 20 menit |
| 02 | PushButton | Test input button dengan debouncing | 20 menit |
| 03 | Motor | Test kontrol motor DC dengan PWM | 20 menit |
| 04 | Sensor | Kalibrasi sensor IR dan threshold | 40 menit |
| 05 | PID | Implementasi kontrol PID line following | 80 menit |
| 06 | PathPlanning | Path planning dengan waypoint navigation | 60 menit |

**Total waktu**: ~4-5 jam (sesuai alokasi praktikum 5 jam 40 menit)

## Struktur Folder

```
Program/
├── lib/                    # Shared libraries (akan dibuat)
│   ├── LineFollowerConfig/ # Konfigurasi terpusat
│   ├── SensorLib/          # Library sensor
│   └── MotorLib/           # Library motor
├── 00_SystemTest/          # Program validasi hardware
├── 01LcdI2c/              # Test OLED I2C
├── 02PushButton/          # Test push button
├── 03Motor/               # Test motor DC
├── 04Sensor/              # Kalibrasi sensor
├── 05PID/                 # PID line following
├── 06PathPlanning/        # Path planning advanced
├── config.h               # File konfigurasi global
└── eeprom_config.h        # Konfigurasi EEPROM
```

## Setup Awal

### 1. Install Software

**Windows/Linux/Mac**:
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install extension PlatformIO IDE
3. Restart VS Code

### 2. Install Driver

**ESP32 S2 Mini** menggunakan chip USB:
- Windows: Install driver CH340 atau CP2102
- Linux: Biasanya sudah built-in
- Mac: Install driver dari vendor

### 3. Clone/Download Project

```bash
git clone [repository-url]
cd "Modul10 LineFollower/Program"
```

### 4. Test Build

```bash
cd 01LcdI2c
pio run
```

Jika berhasil, library akan otomatis terinstall.

## Cara Menggunakan

### Build Program

```bash
cd [nama-program]
pio run
```

### Upload ke ESP32

```bash
pio run -t upload
```

**Jika gagal**: Tekan tombol BOOT saat upload

### Monitor Serial

```bash
pio device monitor -b 115200
```

Keluar: `Ctrl+C`

### Clean Build

```bash
pio run -t clean
pio run
```

## Urutan Praktikum yang Disarankan

### Sesi 1: Hardware Validation (40 menit)
1. **00_SystemTest** (20 menit)
   - Validasi semua hardware
   - Deteksi masalah awal
   
2. **01_LcdI2c** (20 menit)
   - Test OLED display
   - Pastikan I2C berfungsi

### Sesi 2: Input/Output (40 menit)
3. **02_PushButton** (20 menit)
   - Test button input
   - Implementasi debouncing
   
4. **03_Motor** (20 menit)
   - Test motor output
   - Kalibrasi arah motor

### Sesi 3: Sensor (40 menit)
5. **04_Sensor** (40 menit)
   - Kalibrasi sensor IR
   - Tentukan threshold optimal
   - Test pembacaan posisi garis

### Sesi 4: Line Following (80 menit)
6. **05_PID** (80 menit)
   - Implementasi PID controller
   - Tuning parameter Kp, Ki, Kd
   - Test di lintasan

### Sesi 5: Advanced (60 menit)
7. **06_PathPlanning** (60 menit)
   - Path planning dengan waypoint
   - Sensor trigger detection
   - Multi-speed profile

## Pin Configuration

### ESP32 S2 Mini Pinout

| Fungsi | Pin | Keterangan |
|--------|-----|------------|
| **Display** |
| OLED SDA | GPIO 33 | I2C Data |
| OLED SCL | GPIO 35 | I2C Clock |
| **Button** |
| Button 1 | GPIO 0 | Active LOW |
| Button 2 | GPIO 12 | Active LOW |
| Button 3 | GPIO 13 | Active LOW |
| Button 4 | GPIO 15 | Active LOW |
| **Motor Kiri** |
| EN | GPIO 11 | PWM Speed |
| IN1 | GPIO 10 | Direction |
| IN2 | GPIO 9 | Direction |
| **Motor Kanan** |
| EN | GPIO 8 | PWM Speed |
| IN1 | GPIO 7 | Direction |
| IN2 | GPIO 6 | Direction |
| **Sensor IR** |
| S0 | GPIO 1 | Sensor kiri |
| S1 | GPIO 2 | |
| S2 | GPIO 3 | |
| S3 | GPIO 4 | |
| S4 | GPIO 5 | |
| S5 | GPIO 6 | |
| S6 | GPIO 7 | |
| S7 | GPIO 8 | Sensor kanan |

**Catatan**: Pin dapat berbeda tergantung desain PCB. Cek `config.h` untuk pin aktual.

## Common Issues

### Upload Failed
```
Failed to connect to ESP32
```
**Solusi**:
1. Tekan dan tahan BOOT
2. Klik upload
3. Lepas BOOT setelah "Connecting..."

### Library Not Found
```
fatal error: Adafruit_SSD1306.h: No such file
```
**Solusi**:
```bash
pio lib install "Adafruit SSD1306"
pio lib install "Adafruit GFX Library"
```

### Serial Port Not Found
```
Could not open port
```
**Solusi**:
1. Cek kabel USB (gunakan kabel data)
2. Install driver CH340/CP2102
3. Cek Device Manager (Windows) atau `ls /dev/ttyUSB*` (Linux)

### OLED Blank
**Solusi**:
1. Cek wiring SDA/SCL
2. Cek alamat I2C (0x3C atau 0x3D)
3. Gunakan I2C scanner

### Motor Tidak Bergerak
**Solusi**:
1. Cek power baterai (min 7.4V)
2. Cek wiring driver motor
3. Test motor langsung ke power
4. Cek ground bersama

## Tips Praktikum

### Sebelum Praktikum
- [ ] Install VS Code dan PlatformIO
- [ ] Download semua program
- [ ] Test build program 01
- [ ] Baca dokumentasi modul
- [ ] Siapkan hardware (robot, baterai, kabel)

### Saat Praktikum
- [ ] Mulai dengan 00_SystemTest
- [ ] Ikuti urutan program (01 → 06)
- [ ] Catat parameter penting (threshold, PID)
- [ ] Dokumentasi dengan foto/video
- [ ] Backup code yang sudah dimodifikasi

### Setelah Praktikum
- [ ] Buat video tugas
- [ ] Lengkapi tabel tuning PID
- [ ] Dokumentasi kendala dan solusi
- [ ] Upload video ke YouTube

## Dokumentasi Tambahan

- [Panduan Tuning PID](../Panduan_Tuning_PID.md)
- [Troubleshooting Guide](../Troubleshooting.md)
- [Jobsheet](../Jobsheet.md)
- [Materi](../Materi.md)

## Support

Jika mengalami masalah:
1. Cek [Troubleshooting.md](../Troubleshooting.md)
2. Baca README.md di folder program terkait
3. Hubungi asisten praktikum
4. Bawa robot dan laptop ke lab

## Kontribusi

Jika menemukan bug atau ingin menambahkan fitur:
1. Fork repository
2. Buat branch baru
3. Commit changes
4. Submit pull request

---

**Dibuat oleh**: Tim Praktikum Mekatronika dan Robotika  
**Terakhir diupdate**: 2026-05-10  
**Versi**: 1.0
