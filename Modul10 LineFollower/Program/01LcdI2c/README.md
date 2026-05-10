# Program 01: LCD I2C Test

## Tujuan

- Memahami komunikasi I2C antara ESP32 dan OLED
- Menguji koneksi hardware OLED
- Menampilkan teks sederhana di layar OLED
- Memahami library Adafruit SSD1306

## Hardware yang Digunakan

| Komponen | Pin ESP32 | Keterangan |
|----------|-----------|------------|
| OLED SDA | GPIO 33 | Data I2C |
| OLED SCL | GPIO 35 | Clock I2C |
| OLED VCC | 3.3V | Power (JANGAN 5V!) |
| OLED GND | GND | Ground |

## Wiring Diagram

```
ESP32 S2 Mini          OLED Display
+-------------+        +------------+
|             |        |            |
|   GPIO 33   |------->| SDA        |
|   GPIO 35   |------->| SCL        |
|   3.3V      |------->| VCC        |
|   GND       |------->| GND        |
|             |        |            |
+-------------+        +------------+
```

**PENTING**: 
- Gunakan 3.3V, BUKAN 5V!
- Pastikan koneksi SDA dan SCL tidak tertukar
- Ground harus terhubung dengan baik

## Library yang Diperlukan

- Adafruit SSD1306 (v2.5.7 atau lebih baru)
- Adafruit GFX Library (v1.11.3 atau lebih baru)
- Adafruit BusIO (v1.14.1 atau lebih baru)

Library akan otomatis terinstall saat build pertama kali.

## Cara Menjalankan

### 1. Buka Project di PlatformIO

```bash
cd "01LcdI2c"
code .
```

### 2. Build Project

```bash
pio run
```

Atau klik tombol Build (✓) di PlatformIO toolbar.

### 3. Upload ke ESP32

```bash
pio run -t upload
```

Atau klik tombol Upload (→) di PlatformIO toolbar.

**Jika upload gagal**:
- Tekan dan tahan tombol BOOT di ESP32
- Klik upload
- Lepas tombol BOOT setelah muncul "Connecting..."

### 4. Monitor Serial Output

```bash
pio device monitor -b 115200
```

Atau klik tombol Serial Monitor di PlatformIO toolbar.

## Output yang Diharapkan

### Serial Monitor

```
==========================================
PROGRAM 01: LCD I2C TEST
==========================================

Inisialisasi I2C...
I2C initialized at SDA=33, SCL=35

Inisialisasi LCD...
LCD initialized successfully!

Menampilkan teks di LCD...
LCD display updated

Program siap!
```

### OLED Display

```
  Line Follower
     Robot

  ESP32 S2 Mini
   I2C Test OK
```

Layar OLED akan menampilkan:
- Judul "Line Follower Robot" di tengah atas
- "ESP32 S2 Mini" di tengah
- "I2C Test OK" di bawah

## Troubleshooting

### OLED Tidak Muncul / Blank

**Kemungkinan penyebab**:
1. Wiring salah
2. Alamat I2C tidak sesuai
3. Power tidak cukup
4. OLED rusak

**Solusi**:

1. **Cek wiring**:
   - SDA ke GPIO 33 (bukan 35)
   - SCL ke GPIO 35 (bukan 33)
   - VCC ke 3.3V (bukan 5V)
   - GND ke GND

2. **Scan alamat I2C**:
   
   Tambahkan code ini di `setup()` sebelum `lcd.begin()`:
   
   ```cpp
   Serial.println("Scanning I2C...");
   for (byte addr = 1; addr < 127; addr++) {
     Wire.beginTransmission(addr);
     byte error = Wire.endTransmission();
     if (error == 0) {
       Serial.print("Device found at 0x");
       Serial.println(addr, HEX);
     }
   }
   ```
   
   Jika alamat bukan 0x3C, ubah di code:
   ```cpp
   lcd.begin(SSD1306_SWITCHCAPVCC, 0x3D); // Ganti 0x3C dengan alamat yang ditemukan
   ```

3. **Cek power**:
   - Gunakan multimeter, cek voltage di pin VCC OLED
   - Harus 3.3V ± 0.1V
   - Jika kurang, cek koneksi atau power supply

4. **Test OLED di Arduino IDE**:
   - Gunakan contoh sketch dari library Adafruit SSD1306
   - File → Examples → Adafruit SSD1306 → ssd1306_128x64_i2c
   - Jika tetap tidak muncul, OLED mungkin rusak

### Error "LCD tidak ditemukan!"

**Serial Monitor menampilkan**:
```
ERROR: LCD tidak ditemukan!
```

**Solusi**:
- Ikuti langkah troubleshooting di atas
- Pastikan library sudah terinstall dengan benar
- Coba clean dan rebuild:
  ```bash
  pio run -t clean
  pio run
  ```

### Upload Gagal

**Error**: "Failed to connect to ESP32"

**Solusi**:
1. Tekan tombol BOOT saat upload
2. Cek kabel USB (gunakan kabel data, bukan kabel charging)
3. Cek port COM:
   ```bash
   pio device list
   ```
4. Install driver CH340 jika belum

### Teks Tidak Terbaca / Terlalu Kecil

**Solusi**:

Ubah ukuran teks di code:
```cpp
lcd.setTextSize(2); // Dari 1 ke 2 (lebih besar)
lcd.setCursor(10, 20);
lcd.print("Hello");
```

## Modifikasi dan Eksperimen

### 1. Ubah Teks yang Ditampilkan

```cpp
lcd.clearDisplay();
lcd.setTextSize(1);
lcd.setCursor(0, 0);
lcd.print("Nama: [Nama Anda]");
lcd.setCursor(0, 10);
lcd.print("NIM: [NIM Anda]");
lcd.display();
```

### 2. Animasi Teks Bergerak

```cpp
void loop() {
  for (int x = 128; x > -100; x--) {
    lcd.clearDisplay();
    lcd.setCursor(x, 30);
    lcd.print("Line Follower");
    lcd.display();
    delay(20);
  }
}
```

### 3. Tampilkan Grafik Sederhana

```cpp
lcd.clearDisplay();

// Gambar kotak
lcd.drawRect(10, 10, 50, 30, WHITE);

// Gambar lingkaran
lcd.drawCircle(90, 25, 15, WHITE);

// Gambar garis
lcd.drawLine(0, 50, 128, 50, WHITE);

lcd.display();
```

### 4. Tampilkan Nilai Sensor (Preview untuk Program Selanjutnya)

```cpp
void loop() {
  int sensor1 = analogRead(1); // Contoh
  int sensor2 = analogRead(2);
  
  lcd.clearDisplay();
  lcd.setCursor(0, 0);
  lcd.print("S1: ");
  lcd.print(sensor1);
  lcd.setCursor(0, 10);
  lcd.print("S2: ");
  lcd.print(sensor2);
  lcd.display();
  
  delay(100);
}
```

## Konsep yang Dipelajari

### 1. Komunikasi I2C

- **I2C** (Inter-Integrated Circuit) adalah protokol komunikasi serial 2-wire
- Menggunakan 2 pin: SDA (data) dan SCL (clock)
- Bisa menghubungkan banyak device dengan alamat berbeda
- OLED biasanya menggunakan alamat 0x3C atau 0x3D

### 2. Library Adafruit

- **Adafruit_SSD1306**: Driver untuk OLED controller SSD1306
- **Adafruit_GFX**: Library grafis untuk menggambar teks, shape, dll
- Menyediakan fungsi seperti:
  - `clearDisplay()`: Hapus layar
  - `setCursor(x, y)`: Set posisi kursor
  - `print()`: Tampilkan teks
  - `display()`: Update layar (wajib dipanggil!)

### 3. Koordinat Layar

```
(0,0)                    (127,0)
  +------------------------+
  |                        |
  |    128 x 64 pixels     |
  |                        |
  +------------------------+
(0,63)                   (127,63)
```

- Origin (0,0) di kiri atas
- X: 0-127 (128 pixels)
- Y: 0-63 (64 pixels)

## Parameter Penting

| Parameter | Nilai | Keterangan |
|-----------|-------|------------|
| I2C Address | 0x3C | Alamat OLED (bisa 0x3D) |
| Screen Width | 128 | Lebar layar dalam pixel |
| Screen Height | 64 | Tinggi layar dalam pixel |
| I2C Speed | 400kHz | Clock speed I2C |
| SDA Pin | GPIO 33 | Pin data I2C |
| SCL Pin | GPIO 35 | Pin clock I2C |

## Langkah Selanjutnya

Setelah berhasil menjalankan program ini:

1. ✅ OLED berfungsi dengan baik
2. ✅ Komunikasi I2C OK
3. ✅ Library terinstall dengan benar

**Lanjut ke**: Program 02 - Push Button Test

## Referensi

- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [ESP32 I2C Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html)

## Catatan

- Program ini adalah fondasi untuk semua program selanjutnya
- OLED akan digunakan untuk menampilkan status, parameter, dan debugging
- Pastikan program ini berjalan sempurna sebelum lanjut ke program berikutnya

---

**Dibuat oleh**: Tim Praktikum Mekatronika dan Robotika  
**Terakhir diupdate**: 2026-05-10  
**Versi**: 1.0
