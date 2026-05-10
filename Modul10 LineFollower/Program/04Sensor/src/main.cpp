/**
 * ============================================================
 * PROGRAM 04: SENSOR GARIS 8 CHANNEL DENGAN KALIBRASI OTOMATIS
 * ============================================================
 * 
 * DESKRIPSI:
 * Program ini membaca 8 sensor garis infrared (IR) menggunakan ADC
 * dengan fitur kalibrasi otomatis. Robot akan bergerak maju dan
 * mundur pelan untuk mengkalibrasi sensor, kemudian menyimpan
 * threshold ke EEPROM untuk digunakan pada program berikutnya.
 * 
 * TUJUAN PEMBELAJARAN:
 * - Memahami cara kerja sensor IR reflective
 * - Membaca nilai analog dengan ADC ESP32
 * - Implementasi kalibrasi sensor otomatis
 * - Konversi analog ke digital dengan threshold
 * - Menghitung posisi garis dari 8 sensor
 * - Menyimpan data kalibrasi ke EEPROM
 * 
 * KONSEP SENSOR IR REFLECTIVE:
 * 1. Sensor terdiri dari LED IR (transmitter) dan photodiode (receiver)
 * 2. LED IR memancarkan cahaya infrared ke permukaan
 * 3. Photodiode mendeteksi cahaya yang dipantulkan
 * 4. Permukaan putih memantulkan banyak cahaya (nilai ADC tinggi)
 * 5. Permukaan hitam menyerap cahaya (nilai ADC rendah)
 * 6. Output analog 0-3.3V dibaca sebagai nilai digital 0-4095 (12-bit ADC)
 * 
 * KOMPONEN HARDWARE:
 * - ESP32 S2 Mini (Microcontroller dengan 12-bit ADC)
 * - 8x Sensor IR TCRT5000 atau QTR-8A
 * - Koneksi sensor ke GPIO ADC:
 *   * S0: GPIO 36, S1: GPIO 39, S2: GPIO 34, S3: GPIO 35
 *   * S4: GPIO 32, S5: GPIO 33, S6: GPIO 25, S7: GPIO 26
 * - LCD OLED I2C untuk display
 * - 4x Push Button untuk kontrol
 * 
 * OUTLINE PROGRAM:
 * 1. INISIALISASI
 *    - Setup Serial untuk debugging
 *    - Inisialisasi EEPROM (512 bytes)
 *    - Inisialisasi I2C dan LCD OLED
 *    - Setup sensor ADC pins
 *    - Setup motor untuk kalibrasi
 *    - Load threshold dari EEPROM (jika ada)
 * 
 * 2. KALIBRASI OTOMATIS (3 Fase)
 *    A. FASE FORWARD (2 detik):
 *       - Robot maju pelan (speed 80)
 *       - Baca nilai maksimum setiap sensor (permukaan putih)
 *       - Update sensorMax[] setiap loop
 *       - Tampilkan progress bar di LCD
 *    
 *    B. FASE BACKWARD (2 detik):
 *       - Robot mundur pelan (speed -80)
 *       - Baca nilai minimum setiap sensor (garis hitam)
 *       - Update sensorMin[] setiap loop
 *       - Tampilkan progress bar di LCD
 *    
 *    C. FASE DONE:
 *       - Stop motor
 *       - Hitung threshold: (min + max) / 2
 *       - Simpan threshold ke EEPROM
 *       - Kembali ke mode normal
 * 
 * 3. PEMBACAAN SENSOR
 *    - Baca 8 sensor analog dengan analogRead()
 *    - Nilai ADC: 0-4095 (12-bit resolution)
 *    - Konversi ke digital: nilai < threshold = hitam (1)
 *    - Konversi ke digital: nilai >= threshold = putih (0)
 *    - Simpan hasil ke array sensorDigital[]
 * 
 * 4. PERHITUNGAN POSISI GARIS
 *    - Gunakan fungsi calculatePosition()
 *    - Algoritma switch-case berdasarkan sensor pattern
 *    - Output: -4 (kiri ekstrem) sampai +4 (kanan ekstrem)
 *    - 0 = garis di tengah (sensor S3-S4)
 *    - Posisi digunakan untuk kontrol PID
 * 
 * 5. PENYIMPANAN EEPROM
 *    - Magic number (0xAB) untuk validasi data
 *    - 8 threshold values (2 bytes each = 16 bytes total)
 *    - Format: [MAGIC][TH0_H][TH0_L][TH1_H][TH1_L]...[TH7_H][TH7_L]
 *    - Load saat startup, save setelah kalibrasi
 * 
 * 6. MODE TAMPILAN (4 Jenis)
 *    A. MODE_RAW:
 *       - Tampilkan nilai analog semua sensor
 *       - Highlight sensor yang detect garis (inverse color)
 *       - Format: S0:1234 S1:2345 ...
 *    
 *    B. MODE_DIGITAL:
 *       - Tampilkan status digital (1/0) ukuran besar
 *       - 1 = garis hitam terdeteksi
 *       - 0 = permukaan putih
 *       - Mudah untuk debugging sensor
 *    
 *    C. MODE_POSITION:
 *       - Tampilkan posisi garis (-4 to +4)
 *       - Visualisasi dengan lingkaran pada skala
 *       - Intuitif untuk memahami posisi robot
 *    
 *    D. MODE_THRESHOLD:
 *       - Tampilkan nilai threshold dari EEPROM
 *       - Untuk verifikasi hasil kalibrasi
 *       - Format: S0:2000 S1:1950 ...
 * 
 * ALGORITMA PEMBACAAN POSISI (SWITCH CASE):
 * Menggunakan 8-bit sensor pattern untuk menentukan posisi:
 * - 0b00011000 (S3-S4) = posisi 0 (tengah)
 * - 0b00110000 (S4-S5) = posisi -1 (sedikit kiri)
 * - 0b01100000 (S5-S6) = posisi -2 (kiri)
 * - 0b11000000 (S6-S7) = posisi -3 (kiri jauh)
 * - 0b10000000 (S7) = posisi -4 (kiri ekstrem)
 * - 0b00001100 (S2-S3) = posisi 1 (sedikit kanan)
 * - 0b00000110 (S1-S2) = posisi 2 (kanan)
 * - 0b00000011 (S0-S1) = posisi 3 (kanan jauh)
 * - 0b00000001 (S0) = posisi 4 (kanan ekstrem)
 * - 0b00000000 = garis hilang, gunakan posisi terakhir
 * - 0b11111111 = persimpangan/finish line
 * 
 * FITUR KALIBRASI OTOMATIS:
 * 1. Robot maju pelan untuk membaca nilai maksimum (putih)
 * 2. Robot mundur pelan untuk membaca nilai minimum (hitam)
 * 3. Timer ditampilkan di LCD untuk durasi kalibrasi
 * 4. Threshold dihitung otomatis: (min + max) / 2
 * 5. Data threshold disimpan ke EEPROM
 * 6. Progress bar visual untuk feedback
 * 
 * KONTROL TOMBOL:
 * - B1: Mulai kalibrasi otomatis (hanya jika tidak sedang kalibrasi)
 * - B2: Skip/Stop kalibrasi (emergency stop)
 * - B3: Reset EEPROM (kembali ke threshold default)
 * - B4: Toggle display mode (RAW/DIGITAL/POSITION/THRESHOLD)
 * 
 * TIPS KALIBRASI:
 * 1. Letakkan robot di atas garis hitam
 * 2. Pastikan sensor S3-S4 tepat di atas garis
 * 3. Tekan B1 untuk mulai kalibrasi
 * 4. Biarkan robot bergerak maju-mundur
 * 5. Jangan ganggu robot selama kalibrasi
 * 6. Setelah selesai, cek threshold di mode THRESHOLD
 * 
 * TROUBLESHOOTING:
 * - Sensor tidak detect garis: Cek jarak sensor ke permukaan (3-5mm)
 * - Nilai ADC tidak stabil: Tambahkan kapasitor 100nF di power sensor
 * - Threshold tidak tepat: Ulangi kalibrasi di track yang akan digunakan
 * - EEPROM tidak save: Cek EEPROM.commit() dipanggil
 * 
 * APLIKASI UNTUK LINE FOLLOWER:
 * - Kalibrasi wajib dilakukan sebelum kompetisi
 * - Threshold berbeda untuk track berbeda (lighting, warna)
 * - Posisi garis digunakan sebagai input PID controller
 * - Digital sensor digunakan untuk deteksi persimpangan
 * 
 * AUTHOR: Tim Praktikum Mekatronika dan Robotika
 * DATE: 2026-05-10
 * VERSION: 1.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include "../../config.h"
#include "eeprom_config.h"

// Inisialisasi LCD
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variabel sensor
uint16_t sensorValues[SENSOR_COUNT];
bool sensorDigital[SENSOR_COUNT];
int8_t linePosition =0;

// Untuk kalibrasi
uint16_t sensorMin[SENSOR_COUNT];
uint16_t sensorMax[SENSOR_COUNT];
bool calibrationActive = false;
uint32_t calibrationStart = 0;
const uint32_t CALIBRATION_FORWARD_TIME = 2000;  // 2 detik maju
const uint32_t CALIBRATION_BACKWARD_TIME = 2000; // 2 detik mundur
const int CALIBRATION_SPEED = 80;  // Kecepatan kalibrasi

enum CalibrationPhase {
  CALIB_IDLE,
  CALIB_FORWARD,
  CALIB_BACKWARD,
  CALIB_DONE
};
CalibrationPhase calibPhase = CALIB_IDLE;

// Mode tampilan
enum DisplayMode {
  MODE_RAW,
  MODE_DIGITAL,
  MODE_POSITION,
  MODE_THRESHOLD
};
DisplayMode currentMode = MODE_POSITION;
uint32_t lastModeChange = 0;

void startCalibration() {
  calibrationActive = true;
  calibPhase = CALIB_FORWARD;
  calibrationStart = millis();
  for (int i = 0; i < SENSOR_COUNT; i++) {
    config.sensor_min[i] = 4095;
    config.sensor_max[i] = 0;
  }
  setMotors(CALIBRATION_SPEED, CALIBRATION_SPEED);
}

void updateCalibration() {
  uint32_t elapsed = millis() - calibrationStart;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorValues[i] < config.sensor_min[i]) config.sensor_min[i] = sensorValues[i];
    if (sensorValues[i] > config.sensor_max[i]) config.sensor_max[i] = sensorValues[i];
  }

  switch (calibPhase) {
    case CALIB_FORWARD:
      if (elapsed >= CALIBRATION_FORWARD_TIME) {
        calibPhase = CALIB_BACKWARD;
        calibrationStart = millis();
        setMotors(-CALIBRATION_SPEED, -CALIBRATION_SPEED);
      }
      break;
    case CALIB_BACKWARD:
      if (elapsed >= CALIBRATION_BACKWARD_TIME) {
        calibPhase = CALIB_DONE;
        setMotors(0, 0);
        // Hitung threshold dan simpan ke config
        uint16_t total = 0;
        for (int i = 0; i < SENSOR_COUNT; i++) {
          total += (config.sensor_min[i] + config.sensor_max[i]) / 2;
        }
        config.sensor_threshold = total / SENSOR_COUNT;
        saveConfig();
        calibrationActive = false;
        calibPhase = CALIB_IDLE;
        Serial.println("Kalibrasi selesai! Threshold: " + String(config.sensor_threshold));
      }
      break;
    default:
      break;
  }

  // Tampilkan progress kalibrasi di LCD
  lcd.clearDisplay();
  lcd.setTextSize(1);
  lcd.setCursor(0, 0);
  lcd.print("KALIBRASI AUTO");
  lcd.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);
  
  lcd.setCursor(0, 15);
  if (calibPhase == CALIB_FORWARD) {
    lcd.print("Fase: MAJU ");
    lcd.print((CALIBRATION_FORWARD_TIME - elapsed) / 1000);
    lcd.print("s");
    lcd.fillRect(0, 35, map(elapsed, 0, CALIBRATION_FORWARD_TIME, 0, SCREEN_WIDTH), 5, WHITE);
  } else if (calibPhase == CALIB_BACKWARD) {
    lcd.print("Fase: MUNDUR ");
    lcd.print((CALIBRATION_BACKWARD_TIME - elapsed) / 1000);
    lcd.print("s");
    lcd.fillRect(0, 35, map(elapsed, 0, CALIBRATION_BACKWARD_TIME, 0, SCREEN_WIDTH), 5, WHITE);
  }
  lcd.display();
}

void displayRawValues() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    int x = (i % 2) * 64;
    int y = 12 + (i / 2) * 12;
    lcd.setCursor(x, y);
    lcd.print("S"); lcd.print(i); lcd.print(":");
    lcd.setTextColor(sensorValues[i] < SENSOR_THRESHOLD ? BLACK : WHITE, sensorValues[i] < SENSOR_THRESHOLD ? WHITE : BLACK);
    lcd.print(sensorValues[i]);
    lcd.setTextColor(WHITE);
  }
}

void displayDigitalStatus() {
  lcd.setTextSize(2);
  lcd.setCursor(0, 15);
  for (int i = 0; i < SENSOR_COUNT; i++) {
    lcd.print(sensorDigital[i] ? "1" : "0");
    if (i < SENSOR_COUNT - 1) lcd.print(" ");
  }
  lcd.setTextSize(1);
  lcd.setCursor(0, 40);
  lcd.print("1=Hitam 0=Putih");
}

void displayPosition() {
  lcd.setCursor(0, 15);
  lcd.print("Posisi: "); lcd.print(linePosition);
  lcd.setCursor(0, 30);
  lcd.print("<-4       0       +4->");
  int posX = map(linePosition, -4, 4, 10, 118);
  lcd.fillCircle(posX, 45, 3, WHITE);
}

void displayThreshold() {
  lcd.setCursor(0, 12);
  lcd.print("Threshold (EEPROM):");
  lcd.setCursor(0, 22);
  lcd.print("Global: "); lcd.print(SENSOR_THRESHOLD);
  for (int i = 0; i < SENSOR_COUNT; i++) {
    int x = (i % 2) * 64;
    int y = 32 + (i / 2) * 10;
    lcd.setCursor(x, y);
    lcd.print("S"); lcd.print(i); lcd.print(":");
    lcd.print(config.sensor_min[i]); lcd.print("/"); lcd.print(config.sensor_max[i]);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(LCD_SDA, LCD_SCL);
  Wire.setClock(400000);
  sensorSetup();
  motorSetup();
  setMotors(0, 0);
  
  // Inisialisasi EEPROM dan load config
  initEEPROM();

  if (!lcd.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    while (1) delay(1000);
  }
  lcd.clearDisplay();
  lcd.setTextColor(WHITE);
  
  Serial.println("Sensor siap dengan threshold: " + String(SENSOR_THRESHOLD));
}

void loop() {
  readSensors(sensorValues);
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensorDigital[i] = sensorValues[i] < SENSOR_THRESHOLD;
  }
  linePosition = calculatePosition(sensorValues, SENSOR_THRESHOLD);

  if (calibrationActive) {
    updateCalibration();
  }

  // Cek tombol
  static bool lastBtn[4] = {false};
  bool btn[4] = {readButton(BUTTON1_PIN), readButton(BUTTON2_PIN), readButton(BUTTON3_PIN), readButton(BUTTON4_PIN)};
  
  if (btn[0] && !lastBtn[0] && !calibrationActive) startCalibration();
  if (btn[1] && !lastBtn[1] && calibrationActive) { calibrationActive = false; calibPhase = CALIB_IDLE; setMotors(0,0); }
  if (btn[2] && !lastBtn[2] && !calibrationActive) { resetToDefaults(); saveConfig(); Serial.println("Reset EEPROM ke default!"); }
  if (btn[3] && !lastBtn[3] && !calibrationActive && millis() - lastModeChange > 500) {
    currentMode = (DisplayMode)((currentMode + 1) % 4);
    lastModeChange = millis();
  }
  for (int i = 0; i < 4; i++) lastBtn[i] = btn[i];

  if (!calibrationActive) {
    lcd.clearDisplay();
    lcd.setTextSize(1);
    lcd.setCursor(0, 0);
    lcd.print("Sensor 8ch - ");
    switch (currentMode) {
      case MODE_RAW: lcd.print("RAW"); break;
      case MODE_DIGITAL: lcd.print("DIGITAL"); break;
      case MODE_POSITION: lcd.print("POS"); break;
      case MODE_THRESHOLD: lcd.print("THR"); break;
    }
    lcd.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);
    switch (currentMode) {
      case MODE_RAW: displayRawValues(); break;
      case MODE_DIGITAL: displayDigitalStatus(); break;
      case MODE_POSITION: displayPosition(); break;
      case MODE_THRESHOLD: displayThreshold(); break;
    }
    lcd.display();
  }
  delay(50);
}
