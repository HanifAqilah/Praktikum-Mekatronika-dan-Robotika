/**
 * ============================================================
 * PROGRAM 03: MOTOR DC TEST DENGAN KONTROL TOMBOL
 * ============================================================
 * 
 * DESKRIPSI:
 * Program ini menguji kontrol motor DC menggunakan driver H-Bridge
 * dengan sinyal PWM (Pulse Width Modulation). Program mendukung
 * 3 mode operasi: Manual, PID, dan Tuning untuk testing dan
 * kalibrasi motor sebelum implementasi line follower.
 * 
 * TUJUAN PEMBELAJARAN:
 * - Memahami kontrol motor DC dengan PWM
 * - Mengenal driver H-Bridge (L298N/L293D)
 * - Implementasi kontrol arah dan kecepatan motor
 * - Safety timeout untuk mencegah motor jalan terus
 * - Interface tuning parameter PID
 * 
 * KONSEP MOTOR DC DAN H-BRIDGE:
 * 1. Motor DC memerlukan arus besar (>500mA)
 * 2. ESP32 hanya mampu supply ~40mA per pin
 * 3. H-Bridge driver sebagai power amplifier
 * 4. PWM mengatur kecepatan (duty cycle 0-100%)
 * 5. Arah diatur dengan polaritas tegangan
 * 
 * CARA KERJA PWM:
 * - Frekuensi: 5000 Hz (dari config.h)
 * - Resolusi: 8-bit (0-255)
 * - Duty cycle 0% = motor stop
 * - Duty cycle 50% = motor setengah kecepatan
 * - Duty cycle 100% = motor kecepatan penuh
 * - Nilai negatif = arah terbalik
 * 
 * KOMPONEN HARDWARE:
 * - ESP32 S2 Mini (Microcontroller)
 * - 2x Motor DC Gearbox (6V-12V)
 * - Driver H-Bridge L298N atau L293D
 * - 4x Push Button untuk kontrol
 * - LCD OLED I2C untuk display
 * - Power supply terpisah untuk motor (7.4V Li-Po)
 * 
 * KONEKSI MOTOR (dari config.h, mode MOTOR_MODE_2PIN):
 * - Motor Kiri:
 *   * MOTOR_LEFT_EN:  GPIO 11 (Enable/PWM)
 *   * MOTOR_LEFT_IN1: GPIO 10 (IN1/Arah)
 *   * MOTOR_LEFT_IN2: GPIO  9 (IN2/Arah)
 * - Motor Kanan:
 *   * MOTOR_RIGHT_EN:  GPIO 8 (Enable/PWM)
 *   * MOTOR_RIGHT_IN1: GPIO 7 (IN3/Arah)
 *   * MOTOR_RIGHT_IN2: GPIO 6 (IN4/Arah)
 * 
 * OUTLINE PROGRAM:
 * 1. INISIALISASI
 *    - Setup Serial untuk debugging
 *    - Inisialisasi I2C dan LCD OLED
 *    - Setup motor pins dengan motorSetup()
 *    - Set motor ke posisi stop (safety)
 *    - Inisialisasi variabel mode dan status
 * 
 * 2. STRUKTUR DATA
 *    - MotorStatus: Menyimpan status motor
 *      * leftSpeed: Kecepatan motor kiri (-255 to 255)
 *      * rightSpeed: Kecepatan motor kanan (-255 to 255)
 *      * direction: String arah (MAJU/KIRI/KANAN/STOP)
 *      * lastCommandTime: Timestamp untuk timeout
 * 
 * 3. MODE OPERASI (3 Jenis)
 *    A. MODE_MANUAL:
 *       - Kontrol langsung dengan tombol
 *       - B2: Maju (kedua motor forward)
 *       - B3: Belok kiri (kiri mundur, kanan maju)
 *       - B4: Belok kanan (kiri maju, kanan mundur)
 *       - Auto-stop jika tombol dilepas
 *       - Safety timeout 5 detik
 *    
 *    B. MODE_PID:
 *       - Mode untuk testing path planning
 *       - Eksekusi path dengan fase turbo-base-brake
 *       - Integrasi dengan sensor (TODO)
 *       - Automatic speed control
 *    
 *    C. MODE_TUNING:
 *       - Interface untuk tuning parameter PID
 *       - B2: Ganti parameter (Kp/Ki/Kd/Speed)
 *       - B3: Kurangi nilai parameter
 *       - B4: Tambah nilai parameter
 *       - Motor stop saat tuning
 * 
 * 4. FUNGSI KONTROL MOTOR
 *    - setMotors(left, right):
 *      * Set kecepatan dan arah kedua motor
 *      * Nilai positif = maju, negatif = mundur
 *      * Range: -255 to 255
 *      * Menggunakan PWM untuk kontrol kecepatan
 * 
 * 5. SAFETY FEATURES
 *    - Motor timeout: Auto-stop setelah 5 detik
 *    - Emergency stop saat ganti mode
 *    - Debouncing tombol (200ms)
 *    - Range limiting untuk speed
 * 
 * 6. PATH PLANNING (MODE_PID)
 *    - Fase TURBO: Kecepatan tinggi di awal
 *    - Fase BASE: Kecepatan normal
 *    - Fase BRAKE: Pengereman dengan mundur
 *    - Fase DONE: Selesai, lanjut path berikutnya
 * 
 * 7. DISPLAY LCD
 *    - Header: Mode operasi saat ini
 *    - Manual: Tampilkan arah dan kecepatan
 *    - Tuning: Tampilkan parameter PID
 *    - Real-time update setiap 50ms
 * 
 * ALGORITMA KONTROL MOTOR:
 * 1. Baca input tombol dengan debouncing
 * 2. Tentukan arah dan kecepatan berdasarkan mode
 * 3. Kirim sinyal PWM ke driver H-Bridge
 * 4. Update status motor dan timestamp
 * 5. Cek timeout untuk safety
 * 6. Update display LCD
 * 
 * KECEPATAN YANG DIGUNAKAN:
 * - SPEED_FORWARD: 150 (kecepatan maju normal)
 * - SPEED_TURN_SLOW: 100 (motor dalam saat belok)
 * - SPEED_TURN_FAST: 150 (motor luar saat belok)
 * - Range tuning: 0-255
 * 
 * KONTROL TOMBOL:
 * - B1: Ganti mode (Manual → PID → Tuning → Manual)
 * - B2: Maju (Manual) / Next parameter (Tuning)
 * - B3: Belok Kiri (Manual) / Kurangi nilai (Tuning)
 * - B4: Belok Kanan (Manual) / Tambah nilai (Tuning)
 * 
 * PARAMETER TUNING:
 * - Kp: Proportional gain (increment 0.01)
 * - Ki: Integral gain (increment 0.0001)
 * - Kd: Derivative gain (increment 0.05)
 * - Speed: Base speed (increment 5)
 * 
 * TROUBLESHOOTING:
 * - Motor tidak bergerak: Cek power supply dan koneksi
 * - Motor bergerak terbalik: Tukar pin1 dan pin2
 * - Kecepatan tidak stabil: Cek PWM frequency
 * - Motor panas: Kurangi duty cycle atau cek beban
 * 
 * APLIKASI UNTUK LINE FOLLOWER:
 * - Manual mode untuk testing hardware
 * - Tuning mode untuk optimasi PID
 * - PID mode untuk autonomous navigation
 * 
 * AUTHOR: Tim Praktikum Mekatronika dan Robotika
 * DATE: 2026-05-10
 * VERSION: 1.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../../config.h"

Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

enum OpMode { MODE_MANUAL, MODE_PID, MODE_TUNING };
OpMode currentMode = MODE_MANUAL;

struct MotorStatus {
  int leftSpeed;
  int rightSpeed;
  String direction;
  unsigned long lastCommandTime;
};
MotorStatus motor = {0, 0, "STOP", 0};

// PID Tuning
float tuneKp = PID_KP;
float tuneKi = PID_KI;
float tuneKd = PID_KD;
int tuneSpeed = PID_BASE_SPEED;
enum TuningParam { TUNE_KP, TUNE_KI, TUNE_KD, TUNE_SPEED };
TuningParam currentParam = TUNE_KP;

// Path Planning
PathPoint pathTable[MAX_PATH_POINTS];
uint8_t pathCount = 0;
uint8_t currentPathIdx = 0;
bool isPathRunning = false;
uint32_t pathPhaseStart = 0;
enum PathPhase { PATH_TURBO, PATH_BASE, PATH_BRAKE, PATH_DONE };
PathPhase pathPhase = PATH_DONE;

const int SPEED_FORWARD = 150;
const int SPEED_TURN_SLOW = 100;
const int SPEED_TURN_FAST = 150;
const unsigned long MOTOR_TIMEOUT = 5000;
unsigned long lastButtonPress[4] = {0};

void runManualMode() {
  if (readButton(BUTTON2_PIN)) {
    motor.leftSpeed = SPEED_FORWARD;
    motor.rightSpeed = SPEED_FORWARD;
    motor.direction = "MAJU";
    motor.lastCommandTime = millis();
  } else if (readButton(BUTTON3_PIN)) {
    motor.leftSpeed = -SPEED_TURN_SLOW;
    motor.rightSpeed = SPEED_TURN_FAST;
    motor.direction = "KIRI";
    motor.lastCommandTime = millis();
  } else if (readButton(BUTTON4_PIN)) {
    motor.leftSpeed = SPEED_TURN_FAST;
    motor.rightSpeed = -SPEED_TURN_SLOW;
    motor.direction = "KANAN";
    motor.lastCommandTime = millis();
  } else {
    if (motor.leftSpeed != 0 || motor.rightSpeed != 0) {
      motor.leftSpeed = 0;
      motor.rightSpeed = 0;
      motor.direction = "STOP";
    }
  }
  setMotors(motor.leftSpeed, motor.rightSpeed);
}

void runTuningMode() {
  if (readButton(BUTTON2_PIN) && millis() - lastButtonPress[1] > 200) {
    lastButtonPress[1] = millis();
    currentParam = (TuningParam)((currentParam + 1) % 4);
  }
  if (readButton(BUTTON3_PIN) && millis() - lastButtonPress[2] > 200) {
    lastButtonPress[2] = millis();
    switch (currentParam) {
      case TUNE_KP: tuneKp = max(0.0f, tuneKp - 0.01f); break;
      case TUNE_KI: tuneKi = max(0.0f, tuneKi - 0.0001f); break;
      case TUNE_KD: tuneKd = max(0.0f, tuneKd - 0.05f); break;
      case TUNE_SPEED: tuneSpeed = max(0, tuneSpeed - 5); break;
    }
  }
  if (readButton(BUTTON4_PIN) && millis() - lastButtonPress[3] > 200) {
    lastButtonPress[3] = millis();
    switch (currentParam) {
      case TUNE_KP: tuneKp += 0.01f; break;
      case TUNE_KI: tuneKi += 0.0001f; break;
      case TUNE_KD: tuneKd += 0.05f; break;
      case TUNE_SPEED: tuneSpeed = min(255, tuneSpeed + 5); break;
    }
  }
  setMotors(0, 0);
  motor.leftSpeed = 0;
  motor.rightSpeed = 0;
  motor.direction = "TUNING";
}

void runPathPlanning() {
  if (!isPathRunning) return;
  PathPoint p = pathTable[currentPathIdx];
  uint32_t elapsed = millis() - pathPhaseStart;

  switch (pathPhase) {
    case PATH_TURBO:
      setMotors(p.v_turbo, p.v_turbo);
      if (elapsed >= p.timer_turbo) {
        pathPhase = PATH_BASE;
        pathPhaseStart = millis();
      }
      break;
    case PATH_BASE:
      setMotors(p.v_base, p.v_base);
      // TODO: Read sensor and check if target position is reached
      // if (linePosition == p.sensorPos) {
      //   pathPhase = PATH_BRAKE;
      //   pathPhaseStart = millis();
      // }
      break;
    case PATH_BRAKE:
      setMotors(-p.v_brake, -p.v_brake);
      if (elapsed >= p.timer_brake) {
        pathPhase = PATH_DONE;
        setMotors(0, 0);
        currentPathIdx++;
        if (currentPathIdx >= pathCount) {
          isPathRunning = false;
        } else {
          pathPhase = PATH_TURBO;
          pathPhaseStart = millis();
        }
      }
      break;
    default:
      break;
  }
}

void displayLCD() {
  lcd.clearDisplay();
  lcd.setTextSize(1);
  lcd.setCursor(0, 0);
  lcd.print("Motor - ");
  switch (currentMode) {
    case MODE_MANUAL: lcd.print("MANUAL"); break;
    case MODE_PID: lcd.print("PID"); break;
    case MODE_TUNING: lcd.print("TUNING"); break;
  }
  lcd.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);

  if (currentMode == MODE_TUNING) {
    lcd.setCursor(0, 12);
    lcd.print("Edit: ");
    switch (currentParam) {
      case TUNE_KP: lcd.print("Kp"); break;
      case TUNE_KI: lcd.print("Ki"); break;
      case TUNE_KD: lcd.print("Kd"); break;
      case TUNE_SPEED: lcd.print("Speed"); break;
    }
    lcd.setCursor(0, 24); lcd.print("Kp: "); lcd.print(tuneKp, 3);
    lcd.setCursor(0, 34); lcd.print("Ki: "); lcd.print(tuneKi, 4);
    lcd.setCursor(0, 44); lcd.print("Kd: "); lcd.print(tuneKd, 2);
    lcd.setCursor(0, 54); lcd.print("Spd: "); lcd.print(tuneSpeed);
  } else {
    lcd.setTextSize(2);
    lcd.setCursor(0, 15);
    if (motor.direction == "MAJU") lcd.print(">> MAJU <<");
    else if (motor.direction == "KIRI") lcd.print("< KIRI <");
    else if (motor.direction == "KANAN") lcd.print("> KANAN >");
    else lcd.print("-- STOP --");
    
    lcd.setTextSize(1);
    lcd.setCursor(0, 55);
    lcd.print("L:"); lcd.print(motor.leftSpeed);
    lcd.print(" R:"); lcd.print(motor.rightSpeed);
  }
  lcd.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(LCD_SDA, LCD_SCL);
  Wire.setClock(400000);
  motorSetup();
  setMotors(0, 0);

  if (!lcd.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    while (1) delay(1000);
  }
  lcd.clearDisplay();
  lcd.setTextColor(WHITE);
}

void loop() {
  if (readButton(BUTTON1_PIN) && millis() - lastButtonPress[0] > 200) {
    lastButtonPress[0] = millis();
    currentMode = (OpMode)((currentMode + 1) % 3);
    setMotors(0, 0);
    motor = {0, 0, "STOP", 0};
  }

  switch (currentMode) {
    case MODE_MANUAL: runManualMode(); break;
    case MODE_PID: runPathPlanning(); break;
    case MODE_TUNING: runTuningMode(); break;
  }

  if ((motor.leftSpeed != 0 || motor.rightSpeed != 0) && millis() - motor.lastCommandTime > MOTOR_TIMEOUT) {
    setMotors(0, 0);
    motor = {0, 0, "AUTO-STOP", 0};
  }

  displayLCD();
  delay(50);
}
