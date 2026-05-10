/**
 * ============================================================
 * PROGRAM 02: PUSH BUTTON TEST DENGAN LCD DISPLAY
 * ============================================================
 * 
 * DESKRIPSI:
 * Program ini menguji 4 tombol push button dan menampilkan
 * statusnya secara real-time pada LCD OLED dengan 4 mode tampilan
 * berbeda. Program ini penting untuk memahami input digital dan
 * debouncing sebelum implementasi kontrol robot line follower.
 * 
 * TUJUAN PEMBELAJARAN:
 * - Memahami konsep INPUT_PULLUP pada microcontroller
 * - Implementasi debouncing untuk mengatasi bouncing mekanik
 * - Deteksi edge (rising/falling) untuk event handling
 * - Visualisasi data input dengan berbagai metode
 * - State management untuk multiple buttons
 * 
 * KONSEP PUSH BUTTON DENGAN INPUT_PULLUP:
 * 1. Menggunakan resistor pull-up internal ESP32 S2 (~45kΩ)
 * 2. Tombol terhubung antara GPIO dan GND
 * 3. Saat tidak ditekan: GPIO = HIGH (3.3V via pull-up)
 * 4. Saat ditekan: GPIO = LOW (0V, terhubung ke GND)
 * 5. Disebut "active LOW" configuration
 * 6. Hemat komponen (tidak perlu resistor eksternal)
 * 
 * DEBOUNCING:
 * Tombol mekanik memiliki fenomena bouncing (osilasi kontak)
 * selama 10-50ms saat ditekan/dilepas. Tanpa debouncing, satu
 * tekanan bisa terbaca berkali-kali. Solusi:
 * - Software debouncing: delay atau timer-based
 * - Algoritma: Baca ulang setelah delay jika ada perubahan
 * - Waktu debounce: 200ms (dari config.h)
 * 
 * KOMPONEN HARDWARE:
 * - ESP32 S2 Mini (Microcontroller)
 * - 4x Push Button (Normally Open)
 * - LCD OLED I2C 128x64
 * - Koneksi:
 *   * BUTTON1_PIN: GPIO 0 (Tombol 1)
 *   * BUTTON2_PIN: GPIO 12 (Tombol 2)
 *   * BUTTON3_PIN: GPIO 13 (Tombol 3)
 *   * BUTTON4_PIN: GPIO 15 (Tombol 4)
 * 
 * OUTLINE PROGRAM:
 * 1. INISIALISASI
 *    - Setup Serial untuk debugging
 *    - Inisialisasi I2C dan LCD OLED
 *    - Konfigurasi 4 pin button sebagai INPUT_PULLUP
 *    - Reset semua state dan counter tombol
 *    - Tampilkan splash screen
 * 
 * 2. STRUKTUR DATA
 *    - ButtonState struct untuk setiap tombol:
 *      * current: Status saat ini (pressed/released)
 *      * last: Status sebelumnya (untuk edge detection)
 *      * pressTime: Timestamp saat ditekan
 *      * count: Counter jumlah penekanan
 *    - Array lastDebounceTime untuk debouncing
 * 
 * 3. LOOP UTAMA
 *    - Baca status semua tombol (digitalRead)
 *    - Implementasi debouncing dengan timer
 *    - Deteksi falling edge (tombol baru ditekan)
 *    - Update counter dan timestamp
 *    - B1 untuk ganti mode tampilan
 *    - Update LCD sesuai mode aktif
 *    - Output debug ke Serial Monitor
 * 
 * 4. MODE TAMPILAN (4 Jenis)
 *    A. MODE_RAW:
 *       - Tampilkan status teks (PRESS/----)
 *       - Bar graph untuk visualisasi
 *       - Counter penekanan per tombol
 *    
 *    B. MODE_DIGITAL:
 *       - Tampilkan status sebagai 1/0
 *       - Font besar untuk kemudahan baca
 *       - Konversi ke binary byte (4-bit)
 *       - Useful untuk debugging digital logic
 *    
 *    C. MODE_BARS:
 *       - Bar chart vertikal untuk 4 tombol
 *       - Bar penuh jika pressed, outline jika released
 *       - Visualisasi intuitif untuk status
 *    
 *    D. MODE_POS:
 *       - Tampilkan counter penekanan
 *       - Indikator lingkaran (penuh/outline)
 *       - Total penekanan semua tombol
 * 
 * 5. DEBOUNCING ALGORITHM
 *    - Jika status berubah, catat waktu perubahan
 *    - Tunggu BUTTON_DEBOUNCE ms (200ms)
 *    - Jika masih berbeda setelah delay, terima perubahan
 *    - Ini mencegah false trigger dari bouncing
 * 
 * 6. EDGE DETECTION
 *    - Falling edge: last=HIGH, current=LOW (baru ditekan)
 *    - Rising edge: last=LOW, current=HIGH (baru dilepas)
 *    - Hanya falling edge yang increment counter
 * 
 * FUNGSI-FUNGSI:
 * - displayRawMode(): Tampilan status teks dengan bar
 * - displayDigitalMode(): Tampilan binary 1/0
 * - displayBarsMode(): Tampilan bar chart vertikal
 * - displayPosMode(): Tampilan counter dengan indikator
 * 
 * KONTROL:
 * - B1: Ganti mode tampilan (RAW → DIGITAL → BARS → POS → RAW)
 * - B2-B4: Test tombol (increment counter)
 * 
 * APLIKASI UNTUK LINE FOLLOWER:
 * - B1: Start/Stop robot
 * - B2: Mode selection (manual/auto/calibrate)
 * - B3: Speed adjustment
 * - B4: Emergency stop
 * 
 * TROUBLESHOOTING:
 * - Tombol tidak terbaca: Cek koneksi ke GND
 * - Multiple trigger: Tambah waktu debounce
 * - Counter tidak akurat: Cek edge detection logic
 * 
 * AUTHOR: Tim Praktikum Mekatronika dan Robotika
 * DATE: 2026-05-10
 * VERSION: 1.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../../config.h"

// Inisialisasi LCD OLED
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Struktur untuk menyimpan status tombol
struct ButtonState {
  bool current;      // Status saat ini
  bool last;          // Status sebelumnya
  uint32_t pressTime; // Waktu saat ditekan
  uint32_t count;     // Counter berapa kali ditekan
};

ButtonState buttons[4];
uint32_t lastDebounceTime[4] = {0, 0, 0, 0};

// Pin tombol dalam array untuk kemudahan akses
const uint8_t BUTTON_PINS[4] = {BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN};
const char* BUTTON_NAMES[4] = {"B1", "B2", "B3", "B4"};

// Mode tampilan
enum DisplayMode {
  MODE_RAW,      // Tampilkan status teks
  MODE_DIGITAL,   // Tampilkan 1/0
  MODE_BARS,      // Bar graph
  MODE_POS        // Position dengan counter
};
DisplayMode currentMode = MODE_RAW;
uint32_t lastModeChange = 0;

// Function prototypes
void displayRawMode();
void displayDigitalMode();
void displayBarsMode();
void displayPosMode();

void setup() {
  Serial.begin(115200);
  Serial.println("==========================================");
  Serial.println("PROGRAM 02: PUSH BUTTON TEST");
  Serial.println("==========================================");
  Serial.println("");

  // Inisialisasi I2C
  Wire.begin(LCD_SDA, LCD_SCL);
  Wire.setClock(400000);

  // Inisialisasi pin tombol dengan INPUT_PULLUP
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    buttons[i].current = false;
    buttons[i].last = false;
    buttons[i].pressTime = 0;
    buttons[i].count = 0;
  }

  Serial.println("Tombol dikonfigurasi (INPUT_PULLUP):");
  Serial.println("B1: GPIO 0, B2: GPIO 12, B3: GPIO 13, B4: GPIO 15");
  Serial.println("");
  Serial.println("Kontrol:");
  Serial.println("B1: Ganti mode tampilan (RAW/DIGITAL/BARS/POS)");
  Serial.println("B2-B4: Test tombol");
  Serial.println("");

  // Inisialisasi LCD
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    Serial.println("ERROR: LCD tidak ditemukan!");
    while (1) delay(1000);
  }

  lcd.clearDisplay();
  lcd.setTextColor(WHITE);
  
  // Tampilan splash screen
  lcd.setTextSize(2);
  lcd.setCursor(15, 10);
  lcd.print("Button");
  lcd.setCursor(25, 30);
  lcd.print("Test");
  lcd.setTextSize(1);
  lcd.setCursor(20, 50);
  lcd.print("Tekan tombol...");
  lcd.display();
  delay(2000);

  Serial.println("Siap! Tekan tombol untuk melihat respon.");
  Serial.println("");
}

void loop() {
  // Baca status semua tombol dengan debouncing
  bool anyPressed = false;
  
  for (int i = 0; i < 4; i++) {
    bool reading = digitalRead(BUTTON_PINS[i]) == LOW;
    
    // Cek debounce
    if (reading != buttons[i].last) {
      lastDebounceTime[i] = millis();
    }
    
    if ((millis() - lastDebounceTime[i]) > BUTTON_DEBOUNCE) {
      if (reading != buttons[i].current) {
        buttons[i].current = reading;
        
        // Tombol baru saja ditekan (falling edge)
        if (buttons[i].current) {
          buttons[i].count++;
          buttons[i].pressTime = millis();
          Serial.print("Tombol ");
          Serial.print(BUTTON_NAMES[i]);
          Serial.print(" ditekan! (Total: ");
          Serial.print(buttons[i].count);
          Serial.println(" kali)");
          
          // B1 untuk ganti mode
          if (i == 0 && (millis() - lastModeChange > 300)) {
            currentMode = (DisplayMode)((currentMode + 1) % 4);
            lastModeChange = millis();
            Serial.print("Mode ganti ke: ");
            switch (currentMode) {
              case MODE_RAW: Serial.println("RAW"); break;
              case MODE_DIGITAL: Serial.println("DIGITAL"); break;
              case MODE_BARS: Serial.println("BARS"); break;
              case MODE_POS: Serial.println("POS"); break;
            }
          }
        }
      }
    }
    
    buttons[i].last = reading;
    
    if (buttons[i].current) {
      anyPressed = true;
    }
  }

  // === TAMPILAN LCD BERDASARKAN MODE ===
  lcd.clearDisplay();
  lcd.setTextSize(1);
  
  // Header
  lcd.setCursor(0, 0);
  lcd.print("Button Test - ");
  switch (currentMode) {
    case MODE_RAW: lcd.print("RAW"); break;
    case MODE_DIGITAL: lcd.print("DIGITAL"); break;
    case MODE_BARS: lcd.print("BARS"); break;
    case MODE_POS: lcd.print("POS"); break;
  }
  lcd.drawLine(0, 10, SCREEN_WIDTH, 10, WHITE);

  switch (currentMode) {
    case MODE_RAW:
      displayRawMode();
      break;
    case MODE_DIGITAL:
      displayDigitalMode();
      break;
    case MODE_BARS:
      displayBarsMode();
      break;
    case MODE_POS:
      displayPosMode();
      break;
  }

  // Update LCD
  lcd.display();

  // Output ke Serial Monitor (setiap 500ms)
  static uint32_t lastSerialOutput = 0;
  if (millis() - lastSerialOutput > 500) {
    lastSerialOutput = millis();
    
    Serial.print("[");
    Serial.print(millis());
    Serial.print("ms] ");
    for (int i = 0; i < 4; i++) {
      Serial.print(BUTTON_NAMES[i]);
      Serial.print(": ");
      Serial.print(buttons[i].current ? "ON " : "OFF ");
    }
    Serial.print("| Counters: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(buttons[i].count);
      if (i < 3) Serial.print(",");
    }
    Serial.println("");
  }

  delay(10); // Small delay for stability
}

void displayRawMode() {
  // Tampilkan status teks untuk setiap tombol
  lcd.setCursor(0, 15);
  lcd.print("Status Tombol:");
  
  for (int i = 0; i < 4; i++) {
    int yPos = 25 + (i * 10);
    lcd.setCursor(0, yPos);
    lcd.print(BUTTON_NAMES[i]);
    lcd.print(": ");
    
    if (buttons[i].current) {
      lcd.print("PRESS ");
      // Tampilkan bar graph
      int barWidth = 50;
      lcd.fillRect(50, yPos, barWidth, 8, WHITE);
    } else {
      lcd.print("----  ");
      // Tampilkan bar kosong
      int barWidth = 50;
      lcd.drawRect(50, yPos, barWidth, 8, WHITE);
    }
    
    // Tampilkan counter
    lcd.setCursor(105, yPos);
    lcd.print(buttons[i].count);
  }
}

void displayDigitalMode() {
  // Tampilkan status digital (1/0) sebagai teks besar
  lcd.setCursor(0, 15);
  lcd.print("Digital Status:");
  
  lcd.setTextSize(2);
  lcd.setCursor(10, 30);
  
  for (int i = 0; i < 4; i++) {
    if (buttons[i].current) {
      lcd.print("1");
    } else {
      lcd.print("0");
    }
    if (i < 3) lcd.print(" ");
  }
  
  lcd.setTextSize(1);
  lcd.setCursor(0, 50);
  lcd.print("1=Press 0=Release");
  
  // Tampilkan decoded byte (4-bit)
  lcd.setCursor(0, 58);
  lcd.print("Byte: 0b");
  for (int i = 3; i >= 0; i--) {
    lcd.print(buttons[i].current ? "1" : "0");
  }
}

void displayBarsMode() {
  // Tampilkan bar chart untuk 4 tombol
  lcd.setCursor(0, 15);
  lcd.print("Bar Chart:");
  
  int barWidth = 25;
  int barSpacing = 5;
  int maxHeight = 30;
  
  for (int i = 0; i < 4; i++) {
    int x = i * (barWidth + barSpacing) + 5;
    int barHeight = buttons[i].current ? maxHeight : 5;
    int y = 55 - barHeight;
    
    // Pilih warna berdasarkan status
    if (buttons[i].current) {
      lcd.fillRect(x, y, barWidth, barHeight, WHITE);
    } else {
      lcd.drawRect(x, y, barWidth, barHeight, WHITE);
    }
    
    // Label tombol
    lcd.setCursor(x + 8, 56);
    lcd.print(BUTTON_NAMES[i]);
  }
}

void displayPosMode() {
  // Tampilkan posisi dan counter
  lcd.setCursor(0, 15);
  lcd.print("Press Counters:");
  
  for (int i = 0; i < 4; i++) {
    int yPos = 27 + (i * 9);
    lcd.setCursor(0, yPos);
    lcd.print(BUTTON_NAMES[i]);
    lcd.print(": ");
    lcd.print(buttons[i].count);
    lcd.print(" x");
    
    // Indikator status
    if (buttons[i].current) {
      lcd.fillCircle(100, yPos + 3, 3, WHITE);
    } else {
      lcd.drawCircle(100, yPos + 3, 3, WHITE);
    }
  }
  
  // Total presses
  lcd.setCursor(0, 58);
  lcd.print("Total: ");
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += buttons[i].count;
  }
  lcd.print(total);
}
