/**
 * ============================================================
 * PROGRAM 01: LCD I2C OLED HELLO WORLD
 * ============================================================
 * 
 * DESKRIPSI:
 * Program ini mendemonstrasikan penggunaan LCD I2C OLED 0.96" 128x64
 * dengan ESP32 S2 Mini. Program akan menampilkan teks "Hello World"
 * pada layar OLED sebagai langkah awal pengenalan interface display
 * untuk robot line follower.
 * 
 * TUJUAN PEMBELAJARAN:
 * - Memahami komunikasi I2C antara ESP32 dan OLED
 * - Mengenal library Adafruit GFX dan SSD1306
 * - Belajar menampilkan teks dan grafis pada OLED
 * - Memahami konsep buffer dan refresh display
 * - Membuat animasi sederhana pada OLED
 * 
 * KOMPONEN YANG DIGUNAKAN:
 * - ESP32 S2 Mini (Microcontroller)
 * - LCD OLED I2C 0.96" 128x64 pixel (SSD1306 Controller)
 * - Koneksi I2C: SDA = GPIO 33, SCL = GPIO 35
 * 
 * CARA KERJA LCD OLED I2C:
 * 1. OLED (Organic Light Emitting Diode) menggunakan teknologi
 *    display mandiri yang memancarkan cahaya tanpa backlight
 * 2. Controller SSD1306 berkomunikasi melalui protokol I2C
 * 3. Alamat I2C default adalah 0x3C (dapat diubah ke 0x3D)
 * 4. Resolusi 128x64 berarti 128 pixel horizontal, 64 vertical
 * 5. Setiap pixel dapat dikontrol secara individual (monokrom)
 * 6. Data dikirim dalam bentuk buffer yang di-refresh ke layar
 * 
 * OUTLINE PROGRAM:
 * 1. INISIALISASI
 *    - Setup Serial untuk debugging
 *    - Inisialisasi I2C dengan pin kustom (SDA=33, SCL=35)
 *    - Set I2C clock ke 400kHz (fast mode)
 *    - Inisialisasi LCD OLED dengan alamat 0x3C
 *    - Error handling jika LCD tidak terdeteksi
 * 
 * 2. SPLASH SCREEN
 *    - Tampilkan judul "Line Follower"
 *    - Tampilkan info modul dan board
 *    - Delay 2 detik untuk pembacaan
 * 
 * 3. LOOP UTAMA (Update setiap 500ms)
 *    - Bersihkan buffer display
 *    - Tampilkan teks "Hello World" ukuran besar
 *    - Tampilkan waktu running sistem
 *    - Tampilkan nomor frame animasi
 *    - Tampilkan status sistem
 *    - Gambar animasi lingkaran bergerak
 *    - Refresh layar dengan lcd.display()
 *    - Debug output ke Serial Monitor
 * 
 * 4. ANIMASI
 *    - Lingkaran bergerak melingkar menggunakan sin/cos
 *    - Update posisi berdasarkan frame counter
 *    - Smooth motion dengan perhitungan trigonometri
 * 
 * FUNGSI PENTING:
 * - lcd.clearDisplay(): Bersihkan buffer (belum tampil di layar)
 * - lcd.setTextSize(n): Set ukuran font (1=kecil, 2=sedang, 3=besar)
 * - lcd.setCursor(x,y): Set posisi kursor untuk teks
 * - lcd.print(): Tulis teks ke buffer
 * - lcd.fillCircle(x,y,r,color): Gambar lingkaran penuh
 * - lcd.display(): Kirim buffer ke layar (wajib dipanggil!)
 * 
 * TROUBLESHOOTING:
 * - LCD tidak menyala: Cek koneksi SDA/SCL dan power 3.3V
 * - Alamat I2C salah: Coba 0x3C atau 0x3D
 * - Tampilan corrupt: Cek kecepatan I2C (turunkan jika perlu)
 * - Teks terpotong: Perhatikan koordinat dan ukuran font
 * 
 * INCLUDE LIBRARY:
 * - Wire.h: Komunikasi I2C bawaan ESP32
 * - Adafruit_GFX.h: Library grafis dasar (text, shape, bitmap)
 * - Adafruit_SSD1306.h: Driver khusus untuk OLED SSD1306
 * - config.h: Konfigurasi pin dan konstanta sistem
 * 
 * AUTHOR: Tim Praktikum Mekatronika dan Robotika
 * DATE: 2026-05-10
 * VERSION: 1.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../../config.h"

// Inisialisasi objek LCD OLED
// Parameter: lebar=128, tinggi=64, I2C interface, reset pin=-1 (tidak digunakan)
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variabel untuk animasi
int animationFrame = 0;
unsigned long lastUpdate = 0;
const int updateInterval = 500; // 500ms

void setup() {
  // Inisialisasi Serial Monitor untuk debugging
  Serial.begin(115200);
  Serial.println("==========================================");
  Serial.println("PROGRAM 01: LCD I2C OLED HELLO WORLD");
  Serial.println("==========================================");
  Serial.println("ESP32 S2 Mini + OLED 128x64 I2C");
  Serial.println("");

  // Inisialisasi I2C dengan pin kustom (SDA=33, SCL=35)
  Wire.begin(LCD_SDA, LCD_SCL);
  Wire.setClock(400000); // Set I2C clock ke 400kHz (fast mode)
  
  Serial.print("Menginisialisasi LCD OLED di alamat 0x");
  Serial.println(LCD_ADDRESS, HEX);
  
  // Inisialisasi LCD OLED
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    Serial.println("ERROR: LCD OLED tidak ditemukan!");
    Serial.println("Periksa:");
    Serial.println("1. Koneksi SDA (GPIO 33) dan SCL (GPIO 35)");
    Serial.println("2. Alamat I2C (coba 0x3C atau 0x3D)");
    Serial.println("3. Power supply (3.3V)");
    while (1) {
      delay(1000); // Halt program jika LCD tidak terdeteksi
    }
  }
  
  Serial.println("LCD OLED berhasil diinisialisasi!");
  
  // Bersihkan layar
  lcd.clearDisplay();
  lcd.setTextColor(WHITE); // Set warna teks (WHITE = pixel nyala)
  
  // Tampilan awal - Splash Screen
  lcd.setTextSize(2);
  lcd.setCursor(20, 10);
  lcd.print("Line");
  lcd.setCursor(15, 30);
  lcd.print("Follower");
  lcd.setTextSize(1);
  lcd.setCursor(25, 50);
  lcd.print("Modul 10 - ESP32 S2");
  lcd.display();
  delay(2000);
  
  Serial.println("Setup selesai. Masuk ke loop utama...");
}

void loop() {
  // Update setiap 500ms
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    animationFrame++;
    
    // Bersihkan layar
    lcd.clearDisplay();
    
    // === BARIS 1: JUDUL ===
    lcd.setTextSize(2);
    lcd.setCursor(15, 5);
    lcd.print("Hello");
    lcd.setCursor(20, 25);
    lcd.print("World");
    
    // === BARIS 2: INFORMASI SISTEM ===
    lcd.setTextSize(1);
    
    // Tampilkan waktu running
    lcd.setCursor(0, 45);
    lcd.print("Time: ");
    lcd.print(millis() / 1000);
    lcd.print("s");
    
    // Tampilkan frame animasi
    lcd.setCursor(80, 45);
    lcd.print("F:");
    lcd.print(animationFrame);
    
    // === BARIS 3: STATUS SYSTEM ===
    lcd.setCursor(0, 55);
    lcd.print("ESP32 S2 Mini Ready");
    
    // === ANIMASI SEDERHANA: BERGERAK BULAT ===
    int x = 64 + 20 * sin(animationFrame * 0.1);
    int y = 40 + 10 * cos(animationFrame * 0.1);
    lcd.fillCircle(x, y, 3, WHITE);
    
    // Update layar (wajib dipanggil setelah menggambar)
    lcd.display();
    
    // Debug ke Serial Monitor
    if (animationFrame % 2 == 0) {
      Serial.print("Frame: ");
      Serial.print(animationFrame);
      Serial.print(" | Time: ");
      Serial.print(millis());
      Serial.println(" ms");
    }
  }
}
