/**
 * ============================================================
 * PROGRAM 05: KONTROL PID UNTUK LINE FOLLOWER
 * ============================================================
 * 
 * DESKRIPSI:
 * Program ini mengimplementasikan kontrol PID (Proportional-Integral-
 * Derivative) untuk mengendalikan robot line follower agar dapat
 * mengikuti garis dengan smooth dan responsif. Program ini adalah
 * inti dari sistem line follower yang menggabungkan pembacaan sensor
 * dengan kontrol motor menggunakan algoritma PID.
 * 
 * TUJUAN PEMBELAJARAN:
 * - Memahami konsep dan implementasi kontrol PID
 * - Mengerti pengaruh parameter Kp, Ki, Kd terhadap performa
 * - Belajar tuning PID untuk mendapatkan respon optimal
 * - Implementasi lost line handling
 * - Real-time monitoring dan debugging sistem kontrol
 * 
 * TEORI PID CONTROLLER:
 * PID adalah algoritma kontrol feedback yang menghitung koreksi
 * berdasarkan tiga komponen:
 * 
 * 1. P (PROPORTIONAL) - Reaksi terhadap error saat ini
 *    - Semakin besar error, semakin besar koreksi
 *    - Kp terlalu kecil: respon lambat, tidak bisa follow garis tajam
 *    - Kp terlalu besar: oscillation (goyang kiri-kanan)
 *    - Formula: P = Kp × error
 * 
 * 2. I (INTEGRAL) - Eliminasi steady-state error
 *    - Mengakumulasi error dari waktu ke waktu
 *    - Mengatasi bias sistematis (motor tidak sama kuat)
 *    - Ki terlalu kecil: ada offset permanen
 *    - Ki terlalu besar: overshoot dan instability
 *    - Formula: I = Ki × ∑(error × dt)
 * 
 * 3. D (DERIVATIVE) - Prediksi error masa depan
 *    - Melihat seberapa cepat error berubah
 *    - Mengurangi overshoot dan oscillation
 *    - Kd terlalu kecil: overshoot besar
 *    - Kd terlalu besar: respon lambat, sensitif noise
 *    - Formula: D = Kd × (error - lastError) / dt
 * 
 * RUMUS PID LENGKAP:
 * correction = Kp×error + Ki×∑error×dt + Kd×(error-lastError)/dt
 * leftSpeed = baseSpeed + correction
 * rightSpeed = baseSpeed - correction
 * 
 * KOMPONEN HARDWARE:
 * - ESP32 S2 Mini (Microcontroller)
 * - 8x Sensor IR untuk deteksi garis
 * - 2x Motor DC dengan encoder (opsional)
 * - Driver motor H-Bridge
 * - LCD OLED I2C untuk monitoring
 * - 4x Push Button untuk kontrol
 * 
 * OUTLINE PROGRAM:
 * 1. INISIALISASI
 *    - Setup Serial, I2C, LCD
 *    - Inisialisasi sensor dan motor
 *    - Load parameter PID dari config/EEPROM
 *    - Reset variabel PID (error, integral, derivative)
 *    - Tampilkan splash screen
 * 
 * 2. LOOP UTAMA (100Hz untuk PID)
 *    A. Baca Sensor:
 *       - Baca 8 sensor analog
 *       - Konversi ke digital dengan threshold
 *       - Hitung posisi garis (-4 to +4)
 *    
 *    B. Hitung Error:
 *       - error = setpoint - linePosition
 *       - setpoint = 0 (garis di tengah)
 *       - error negatif = robot di kiri garis
 *       - error positif = robot di kanan garis
 *    
 *    C. Hitung PID:
 *       - P = Kp × error
 *       - I = Ki × (integral + error)
 *       - D = Kd × (error - lastError)
 *       - correction = P + I + D
 *       - Limit correction ke range yang aman
 *    
 *    D. Aplikasikan ke Motor:
 *       - leftSpeed = baseSpeed + correction
 *       - rightSpeed = baseSpeed - correction
 *       - Constrain speed ke 0-255
 *       - Kirim PWM ke motor
 *    
 *    E. Update State:
 *       - lastError = error
 *       - Cek lost line condition
 *       - Update LCD display
 * 
 * 3. MODE OPERASI (3 Jenis)
 *    A. MODE_PID:
 *       - PID controller aktif
 *       - Robot follow garis otomatis
 *       - Display real-time PID values
 *    
 *    B. MODE_MANUAL:
 *       - Kontrol manual dengan tombol
 *       - Untuk testing motor dan sensor
 *       - PID tidak aktif
 *    
 *    C. MODE_TUNING:
 *       - Interface tuning parameter PID
 *       - Ubah Kp, Ki, Kd, baseSpeed
 *       - Save ke EEPROM
 *       - Motor stop saat tuning
 * 
 * 4. LOST LINE HANDLING
 *    - Deteksi: Semua sensor tidak detect garis
 *    - Timeout: 500ms sebelum action
 *    - Recovery: Putar ke arah lastError
 *    - Jika error terakhir negatif: putar kiri
 *    - Jika error terakhir positif: putar kanan
 * 
 * 5. TUNING PID REAL-TIME
 *    - B1: Ganti mode operasi
 *    - B2: Next parameter (Kp/Ki/Kd/Speed)
 *    - B3: Kurangi nilai parameter
 *    - B4: Tambah nilai parameter
 *    - Auto-save ke EEPROM
 * 
 * 6. DISPLAY LCD
 *    - Header: Mode dan status
 *    - Line 1: Posisi garis dan error
 *    - Line 2: Correction value
 *    - Line 3: Motor speeds (L/R)
 *    - Line 4: PID parameters
 *    - Update rate: 10Hz (100ms)
 * 
 * PARAMETER PID (dari config.h atau EEPROM):
 * - PID_KP: Gain Proportional (default: 0.8)
 * - PID_KI: Gain Integral (default: 0.0)
 * - PID_KD: Gain Derivative (default: 0.1)
 * - BASE_SPEED: Kecepatan dasar (default: 150)
 * 
 * CARA TUNING PID (Metode Ziegler-Nichols):
 * 1. Set Ki=0, Kd=0, mulai dengan Kp kecil (0.1)
 * 2. Naikkan Kp sampai robot oscillate (goyang)
 * 3. Catat Kp saat mulai oscillate (Ku)
 * 4. Hitung: Kp = 0.6×Ku, Ki = 1.2×Ku/Tu, Kd = 0.075×Ku×Tu
 * 5. Fine-tune berdasarkan track dan kecepatan
 * 
 * TIPS TUNING:
 * - Mulai di track lurus untuk tuning Kp
 * - Test di tikungan tajam untuk tuning Kd
 * - Gunakan Ki hanya jika ada steady-state error
 * - Tuning berbeda untuk speed berbeda
 * - Save profile untuk track berbeda
 * 
 * TROUBLESHOOTING:
 * - Robot goyang: Kp terlalu besar atau Kd terlalu kecil
 * - Robot lambat: Kp terlalu kecil atau baseSpeed rendah
 * - Robot keluar track: Kd terlalu besar atau sensor tidak kalibrasi
 * - Overshoot: Ki terlalu besar
 * 
 * APLIKASI LANJUTAN:
 * - Adaptive PID (parameter berubah sesuai kondisi)
 * - Fuzzy PID (kombinasi fuzzy logic dan PID)
 * - Feedforward control (prediksi berdasarkan track)
 * - Multi-mode PID (parameter berbeda untuk lurus/belok)
 * 
 * AUTHOR: Tim Praktikum Mekatronika dan Robotika
 * DATE: 2026-05-10
 * VERSION: 1.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../../config.h"

// Include EEPROM config after config.h to avoid circular dependency
#include "eeprom_config.h"

// Inisialisasi LCD
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variabel untuk perhitungan PID (menggunakan config dari EEPROM)
float lastError = 0;
float integral = 0;
float derivative = 0;
float currentError = 0;
float correction = 0;

// Kecepatan motor aktual
int motorLeftSpeed = 0;
int motorRightSpeed = 0;

// Sensor
uint16_t sensorValues[SENSOR_COUNT];
bool sensorDigital[SENSOR_COUNT];
float linePosition = 0;

// Mode operasi
enum OpMode {
  MODE_PID,        // Mode PID aktif
  MODE_MANUAL,     // Mode manual dengan tombol
  MODE_TUNING      // Mode tuning parameter
};
OpMode currentOpMode = MODE_PID;

// Untuk lost line handling
bool lineLost = false;
unsigned long lineLostTime = 0;
const unsigned long LINE_LOST_TIMEOUT = 500; // 500ms
float lastValidError = 0;

// Untuk tuning via tombol
enum TuningParam {
  TUNE_KP,
  TUNE_KI,
  TUNE_KD,
  TUNE_SPEED
};
TuningParam currentTuningParam = TUNE_KP;
bool tuningEditMode = false;

// Timer
unsigned long lastLCDUpdate = 0;
unsigned long lastPIDUpdate = 0;
const unsigned long PID_INTERVAL = 10; // 10ms = 100Hz
const unsigned long LCD_INTERVAL = 100; // 100ms = 10Hz

// Function prototypes
void runPID();
void runManual();
void runTuning();
void displayLCD();

void setup() {
  Serial.begin(115200);
  Serial.println("==========================================");
  Serial.println("PROGRAM 05: KONTROL PID LINE FOLLOWER");
  Serial.println("==========================================");
  Serial.println("");

  // Inisialisasi EEPROM dan load config
  initEEPROM();

  // Inisialisasi I2C
  Wire.begin(LCD_SDA, LCD_SCL);
  Wire.setClock(400000);

  // Inisialisasi sensor
  sensorSetup();
  
  // Inisialisasi motor
  motorSetup();
  setMotors(0, 0);
  
  Serial.println("Parameter PID dari EEPROM:");
  Serial.print("Kp = "); Serial.println(config.pid_kp);
  Serial.print("Ki = "); Serial.println(config.pid_ki);
  Serial.print("Kd = "); Serial.println(config.pid_kd);
  Serial.print("Base Speed = "); Serial.println(config.base_speed);
  Serial.println("");

  Serial.println("Kontrol:");
  Serial.println("B1: Ganti mode (PID/Manual/Tuning)");
  Serial.println("B2: Di mode tuning: ganti parameter");
  Serial.println("B3: Di mode tuning: kurangi nilai");
  Serial.println("B4: Di mode tuning: tambah nilai");
  Serial.println("HOLD B2+B3: Reset ke default");
  Serial.println("HOLD B2+B4: Save ke EEPROM");
  Serial.println("");

  // Inisialisasi LCD
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    Serial.println("ERROR: LCD tidak ditemukan!");
    while (1) delay(1000);
  }

  lcd.clearDisplay();
  lcd.setTextColor(WHITE);

  // Splash screen
  lcd.setTextSize(2);
  lcd.setCursor(20, 10);
  lcd.print("PID");
  lcd.setCursor(15, 30);
  lcd.print("Control");
  lcd.setTextSize(1);
  lcd.setCursor(25, 50);
  lcd.print("Line Follower");
  lcd.display();
  delay(2000);

  Serial.println("Siap! Robot akan mulai mengikuti garis.");
  Serial.println("");
}

void loop() {
  // Baca sensor
  readSensors(sensorValues);
  readSensorsDigital(sensorDigital);
  linePosition = calculatePosition(sensorValues);

  // Cek kombinasi tombol untuk reset/save
  if (readButton(BUTTON2_PIN) && readButton(BUTTON3_PIN)) {
    delay(300);
    Serial.println("Reset ke default...");
    resetToDefaults();
    saveConfig();
    Serial.println("Config direset dan disimpan!");
    delay(500);
    return;
  }
  
  if (readButton(BUTTON2_PIN) && readButton(BUTTON4_PIN)) {
    delay(300);
    Serial.println("Menyimpan config ke EEPROM...");
    saveConfig();
    Serial.println("Config tersimpan!");
    delay(500);
    return;
  }

  // Cek tombol untuk ganti mode
  if (readButton(BUTTON1_PIN)) {
    delay(200); // Debounce
    currentOpMode = (OpMode)((currentOpMode + 1) % 3);
    Serial.print("Mode ganti ke: ");
    switch (currentOpMode) {
      case MODE_PID: Serial.println("PID"); break;
      case MODE_MANUAL: Serial.println("MANUAL"); break;
      case MODE_TUNING: Serial.println("TUNING"); break;
    }
    delay(300);
  }

  // Jalankan berdasarkan mode
  switch (currentOpMode) {
    case MODE_PID:
      runPID();
      break;
    case MODE_MANUAL:
      runManual();
      break;
    case MODE_TUNING:
      runTuning();
      break;
  }

  // Update LCD setiap 100ms
  if (millis() - lastLCDUpdate >= LCD_INTERVAL) {
    lastLCDUpdate = millis();
    displayLCD();
  }

  delay(10);
}

void runPID() {
  if (millis() - lastPIDUpdate < PID_INTERVAL) return;
  lastPIDUpdate = millis();

  // Cek apakah garis hilang (semua sensor off)
  bool allSensorsOff = true;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorDigital[i]) { allSensorsOff = false; break; }
  }

  if (allSensorsOff) {
    if (!lineLost) {
      lineLost = true;
      lineLostTime = millis();
      Serial.println("GARIS HILANG! Mencari...");
    }
    
    // Lost line handling: putar mencari garis
    if (millis() - lineLostTime < LINE_LOST_TIMEOUT) {
      // Masih dalam timeout, coba cari garis
      if (lastValidError > 0) {
        // Garis terakhir di kanan, putar kanan (L maju, R mundur)
        setMotors(config.turn_speed, -config.turn_speed);
      } else {
        // Garis terakhir di kiri, putar kiri (L mundur, R maju)
        setMotors(-config.turn_speed, config.turn_speed);
      }
    } else {
      // Timeout, berhenti
      setMotors(0, 0);
      Serial.println("Garis tidak ditemukan! Berhenti.");
    }
    return;
  }

  // Garis ditemukan
  if (lineLost) {
    lineLost = false;
    Serial.println("Garis ditemukan kembali!");
  }

  // Hitung error (setpoint = tengah = 0, range -7 sampai +7)
  currentError = linePosition;
  lastValidError = currentError;

  // Hitung komponen PID menggunakan config EEPROM
  integral += currentError * (PID_INTERVAL / 1000.0);
  integral = constrain(integral, -10000, 10000); // Anti-windup
  
  derivative = (currentError - lastError) / (PID_INTERVAL / 1000.0);  
  correction = config.pid_kp * currentError + config.pid_ki * integral + config.pid_kd * derivative;

  // Hitung kecepatan motor menggunakan config EEPROM
  motorLeftSpeed = config.base_speed + correction;
  motorRightSpeed = config.base_speed - correction;

  // Batasi kecepatan
  motorLeftSpeed = constrain(motorLeftSpeed, -config.max_speed, config.max_speed);
  motorRightSpeed = constrain(motorRightSpeed, -config.max_speed, config.max_speed);

  // Set motor
  setMotors(motorLeftSpeed, motorRightSpeed);

  // Simpan error untuk iterasi berikutnya
  lastError = currentError;

  // Debug ke Serial (setiap 200ms)
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 200) {
    lastDebug = millis();
    Serial.print("[");
    Serial.print(millis());
    Serial.print("ms] Pos:"); Serial.print(linePosition);
    Serial.print(" Err:"); Serial.print(currentError);
    Serial.print(" Corr:"); Serial.print(correction);
    Serial.print(" L:"); Serial.print(motorLeftSpeed);
    Serial.print(" R:"); Serial.println(motorRightSpeed);
  }
}

void runManual() {
  // Kontrol manual dengan tombol menggunakan config EEPROM
  if (readButton(BUTTON2_PIN)) {
    // Maju
    setMotors(config.base_speed, config.base_speed);
    motorLeftSpeed = config.base_speed;
    motorRightSpeed = config.base_speed;
  } else if (readButton(BUTTON3_PIN)) {
    // Belok kiri
    setMotors(-config.base_speed/2, config.base_speed);
    motorLeftSpeed = -config.base_speed/2;
    motorRightSpeed = config.base_speed;
  } else if (readButton(BUTTON4_PIN)) {
    // Belok kanan
    setMotors(config.base_speed, -config.base_speed/2);
    motorLeftSpeed = config.base_speed;
    motorRightSpeed = -config.base_speed/2;
  } else {
    // Stop
    setMotors(0, 0);
    motorLeftSpeed = 0;
    motorRightSpeed = 0;
  }
}

void runTuning() {
  // Motor stop saat tuning
  setMotors(0, 0);
  
  // Tuning parameter PID dengan tombol
  if (readButton(BUTTON2_PIN) && !tuningEditMode) {
    delay(200);
    // Ganti parameter yang dituning
    currentTuningParam = (TuningParam)((currentTuningParam + 1) % 4);
    tuningEditMode = false;
    Serial.print("Tuning parameter: ");
    switch (currentTuningParam) {
      case TUNE_KP: Serial.println("Kp"); break;
      case TUNE_KI: Serial.println("Ki"); break;
      case TUNE_KD: Serial.println("Kd"); break;
      case TUNE_SPEED: Serial.println("Base Speed"); break;
    }
    delay(300);
  }

  if (readButton(BUTTON3_PIN) || readButton(BUTTON4_PIN)) {
    delay(50);
    tuningEditMode = true;
    
    if (readButton(BUTTON3_PIN)) {
      // Kurangi nilai menggunakan fungsi adjustment
      switch (currentTuningParam) {
        case TUNE_KP: adjustPIDKp(-0.1f); break;
        case TUNE_KI: adjustPIDKi(-0.01f); break;
        case TUNE_KD: adjustPIDKd(-0.01f); break;
        case TUNE_SPEED: adjustBaseSpeed(-10); break;
      }
    }
    
    if (readButton(BUTTON4_PIN)) {
      // Tambah nilai menggunakan fungsi adjustment
      switch (currentTuningParam) {
        case TUNE_KP: adjustPIDKp(0.1f); break;
        case TUNE_KI: adjustPIDKi(0.01f); break;
        case TUNE_KD: adjustPIDKd(0.01f); break;
        case TUNE_SPEED: adjustBaseSpeed(10); break;
      }
    }
    
    Serial.print("Kp="); Serial.print(config.pid_kp, 2);
    Serial.print(" Ki="); Serial.print(config.pid_ki, 3);
    Serial.print(" Kd="); Serial.print(config.pid_kd, 2);
    Serial.print(" Speed="); Serial.println(config.base_speed);
    
    delay(150);
  }
}

void displayLCD() {
  lcd.clearDisplay();
  lcd.setTextSize(1);

  // Header
  lcd.setCursor(0, 0);
  switch (currentOpMode) {
    case MODE_PID: lcd.print("PID Mode Active"); break;
    case MODE_MANUAL: lcd.print("Manual Mode"); break;
    case MODE_TUNING: lcd.print("Tuning Mode"); break;
  }
  lcd.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);

  if (currentOpMode == MODE_TUNING) {
    // Tampilan tuning
    lcd.setCursor(0, 12);
    lcd.print("Edit: ");
    switch (currentTuningParam) {
      case TUNE_KP: lcd.print("Kp"); break;
      case TUNE_KI: lcd.print("Ki"); break;
      case TUNE_KD: lcd.print("Kd"); break;
      case TUNE_SPEED: lcd.print("Speed"); break;
    }
    
    lcd.setCursor(0, 22);
    if (currentTuningParam == TUNE_KP) lcd.print(">");
    lcd.print("Kp: "); lcd.print(config.pid_kp, 2);
    
    lcd.setCursor(0, 32);
    if (currentTuningParam == TUNE_KI) lcd.print(">");
    lcd.print("Ki: "); lcd.print(config.pid_ki, 3);
    
    lcd.setCursor(0, 42);
    if (currentTuningParam == TUNE_KD) lcd.print(">");
    lcd.print("Kd: "); lcd.print(config.pid_kd, 2);
    
    lcd.setCursor(0, 52);
    if (currentTuningParam == TUNE_SPEED) lcd.print(">");
    lcd.print("Spd: "); lcd.print(config.base_speed);
    
  } else {
    // Tampilan PID normal
    
    // Posisi garis
    lcd.setCursor(0, 12);
    lcd.print("Pos: ");
    lcd.print((int)linePosition);
    lcd.print("/7");
    if (lineLost) lcd.print(" LOST");
    
    // Error dan Correction
    lcd.setCursor(0, 22);
    lcd.print("E:"); lcd.print(currentError);
    lcd.print(" C:"); lcd.print(correction);
    
    // Kecepatan motor
    lcd.setCursor(0, 32);
    lcd.print("L:"); lcd.print(motorLeftSpeed);
    lcd.print(" R:"); lcd.print(motorRightSpeed);
    
    // Parameter PID dari EEPROM
    lcd.setCursor(0, 42);
    lcd.print("Kp:"); lcd.print(config.pid_kp, 2);
    lcd.print(" Ki:"); lcd.print(config.pid_ki, 3);
    lcd.print(" Kd:"); lcd.print(config.pid_kd, 1);
    
    // Status
    lcd.setCursor(0, 52);
    if (lineLost) {
      lcd.print("Status: SEARCHING");
    } else {
      lcd.print("Status: RUNNING");
    }
  }

  lcd.display();
}
