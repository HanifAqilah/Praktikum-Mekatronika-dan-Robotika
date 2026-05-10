#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================
// KONFIGURASI PIN ESP32 S2 MINI / WEMOS LOLIN S2
// ============================================================
// 
// ESP32 S2 Mini Pinout:
// - GPIO 0-21 tersedia (beberapa memiliki fungsi khusus)
// - ADC: 13-bit resolusi (0-4095)
// - PWM: LEDC dengan 8 channel independen
// - I2C: Mendukung pin SDA/SCL yang fleksibel
//
// ============================================================

// ==================== LCD I2C OLED 0.96" 128x64 ====================
// OLED dengan controller SSD1306, komunikasi I2C
// Alamat default: 0x3C (atau 0x3D jika jumper disolder)
#define LCD_SDA         33    // Pin SDA (Data)
#define LCD_SCL         35    // Pin SCL (Clock)
#define LCD_ADDRESS     0x3C  // Alamat I2C LCD
#define SCREEN_WIDTH    128   // Lebar pixel LCD
#define SCREEN_HEIGHT   64    // Tinggi pixel LCD

// ==================== PUSH BUTTON (4 Tombol) ====================
// Menggunakan INPUT_PULLUP internal (active LOW)
// Tombol ditekan = LOW (0), tidak ditekan = HIGH (1)
#define BUTTON1_PIN     0     // Tombol 1 (GPIO 0)
#define BUTTON2_PIN     12    // Tombol 2 (GPIO 12)
#define BUTTON3_PIN     13    // Tombol 3 (GPIO 13)
#define BUTTON4_PIN     15    // Tombol 4 (GPIO 15)
#define BUTTON_DEBOUNCE 50    // Waktu debounce dalam ms

// ==================== MOTOR DC CONFIGURATION MODE ====================
// Pilih mode driver motor:
// MODE_2PIN: 2 pin per motor (IN1, IN2) - driver seperti L298N standard
// MODE_3PIN: 3 pin per motor (PWM, DIR1, DIR2) - mode 3 pin
// MODE_1PWM_1DIR: 1 PWM + 1 DIR per motor - mode inverting/non-inverting
//
// MODE_1PWM_1DIR:
// - Ketika DIR = HIGH, PWM akan inverting (motor mundur)
// - Ketika DIR = LOW, PWM akan non-inverting (motor maju)
// - Cocok untuk driver seperti DRV8833, TB6612FNG dengan mode PWM+DIR

#define MOTOR_MODE_2PIN       0
#define MOTOR_MODE_3PIN       1  
#define MOTOR_MODE_1PWM_1DIR  2

// Pilih mode yang digunakan (ubah sesuai driver motor Anda)
#define MOTOR_DRIVER_MODE  MOTOR_MODE_2PIN

// ==================== MOTOR DC (2 Motor: Kiri & Kanan) ====================

#if MOTOR_DRIVER_MODE == MOTOR_MODE_2PIN
  // Mode 2 pin: IN1 dan IN2 untuk kontrol arah, PWM di EN
  #define MOTOR_LEFT_EN   11    // Enable Motor Kiri (PWM)
  #define MOTOR_LEFT_IN1  10    // Input 1 Motor Kiri (Arah)
  #define MOTOR_LEFT_IN2  9     // Input 2 Motor Kiri (Arah)
  #define MOTOR_RIGHT_EN  8     // Enable Motor Kanan (PWM)
  #define MOTOR_RIGHT_IN1 7     // Input 1 Motor Kanan (Arah)
  #define MOTOR_RIGHT_IN2 6     // Input 2 Motor Kanan (Arah)

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_3PIN
  // Mode 3 pin: PWM, DIR1, DIR2
  #define MOTOR_LEFT_PWM  11    // PWM Motor Kiri
  #define MOTOR_LEFT_DIR1 10    // Direction 1 Motor Kiri
  #define MOTOR_LEFT_DIR2 9     // Direction 2 Motor Kiri
  #define MOTOR_RIGHT_PWM 8     // PWM Motor Kanan
  #define MOTOR_RIGHT_DIR1 7    // Direction 1 Motor Kanan
  #define MOTOR_RIGHT_DIR2 6    // Direction 2 Motor Kanan

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_1PWM_1DIR
  // Mode 1 PWM + 1 DIR: PWM untuk kecepatan, DIR untuk arah (inverting/non-inverting)
  #define MOTOR_LEFT_PWM  11    // PWM Motor Kiri
  #define MOTOR_LEFT_DIR  10    // Direction Motor Kiri (HIGH=invert, LOW=normal)
  #define MOTOR_RIGHT_PWM 8     // PWM Motor Kanan
  #define MOTOR_RIGHT_DIR 7     // Direction Motor Kanan (HIGH=invert, LOW=normal)
#endif

// Invert Motor (jika arah motor terbalik dari yang diharapkan)
// true = balik arah, false = arah normal
#define MOTOR_LEFT_INVERT   false
#define MOTOR_RIGHT_INVERT  false

// ==================== PWM SETTINGS ====================
// LEDC PWM untuk kontrol kecepatan motor
#define PWM_FREQ        5000   // Frekuensi PWM (Hz)
#define PWM_RESOLUTION  8      // Resolusi PWM (8-bit = 0-255)
#define PWM_CHANNEL_L   0      // Channel PWM Motor Kiri
#define PWM_CHANNEL_R   1      // Channel PWM Motor Kanan

// ==================== SENSOR GARIS INFRARED (8 Channel) ====================
// Sensor array 8 channel ADC (Analog to Digital Converter)
// ESP32 S2 memiliki ADC 13-bit (0-4095)
// Sensor mendeteksi: Hitam = nilai rendah (< threshold), Putih = nilai tinggi (> threshold)
#define SENSOR_COUNT    8
const uint8_t SENSOR_PINS[SENSOR_COUNT] = {
  1,  // Sensor 0 (paling kiri)
  2,  // Sensor 1
  3,  // Sensor 2
  4,  // Sensor 3
  5,  // Sensor 4
  16, // Sensor 5 (GPIO 16 - menghindari konflik dengan motor)
  17, // Sensor 6 (GPIO 17)
  18  // Sensor 7 (paling kanan, GPIO 18)
};

// Threshold sensor (batas antara hitam dan putih)
// Nilai ADC < SENSOR_THRESHOLD = Garis Hitam (1/ON)
// Nilai ADC >= SENSOR_THRESHOLD = Latar Putih (0/OFF)
#define SENSOR_THRESHOLD  2000

// ==================== PID CONTROLLER PARAMETERS ====================
// PID: Proportional-Integral-Derivative
// Digunakan untuk kontrol line following yang halus
#define PID_KP          0.08   // Gain Proportional (reaksi terhadap error saat ini)
#define PID_KI          0.0    // Gain Integral (akumulasi error masa lalu)
#define PID_KD          0.4    // Gain Derivative (prediksi perubahan error)
#define PID_BASE_SPEED  150    // Kecepatan dasar robot (0-255)
#define PID_MAX_SPEED   255    // Kecepatan maksimum (PWM max)
#define PID_MIN_SPEED   -255   // Kecepatan minimum (mundur max)

// ==================== PATH PLANNING ====================
// Algoritma path planning untuk navigasi otomatis
#define MAX_PATH_POINTS 20     // Maksimal titik path yang dapat disimpan

// Sensor position detection mode for path planning
// Guard: 06PathPlanning mendefinisikan PathPoint/PathAction sendiri yang lebih lengkap
#ifndef PATH_PLANNING_ADVANCED

typedef enum {
  PATH_SENSOR_LEFT,    // Detect left sensor only
  PATH_SENSOR_RIGHT,   // Detect right sensor only
  PATH_SENSOR_BOTH     // Detect both left and right sensors
} PathSensorMode;

// Action for path point
typedef enum {
  PATH_ACTION_STRAIGHT,
  PATH_ACTION_TURN_LEFT,
  PATH_ACTION_TURN_RIGHT,
  PATH_ACTION_STOP
} PathAction;

// Path point struct
typedef struct {
  PathSensorMode sensorMode;  // Which sensor to check
  int8_t sensorPos;            // Target sensor position (-4 to 4)
  PathAction action;            // Action to take when position detected
  uint8_t v_base;              // Base speed (0-255)
  uint8_t v_turbo;             // Turbo speed (0-255)
  uint32_t timer_turbo;        // Turbo duration (ms)
  uint8_t v_brake;             // Brake speed (reverse, 0-255)
  uint32_t timer_brake;         // Brake duration (ms)
} PathPoint;

// Example path table (to be filled by user)
extern PathPoint pathTable[MAX_PATH_POINTS];
extern uint8_t pathCount;

#endif // PATH_PLANNING_ADVANCED

// ==================== FUNGSI HELPER ====================

// Membaca status tombol dengan debouncing sederhana
// Return: true jika tombol ditekan, false jika tidak
static bool readButton(uint8_t pin) {
  return digitalRead(pin) == LOW;
}

// Inisialisasi pin motor dan setup PWM
static void motorSetup() {
#if MOTOR_DRIVER_MODE == MOTOR_MODE_2PIN
  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_RIGHT_IN1, OUTPUT);
  pinMode(MOTOR_RIGHT_IN2, OUTPUT);
  
  ledcSetup(PWM_CHANNEL_L, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_R, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_LEFT_EN, PWM_CHANNEL_L);
  ledcAttachPin(MOTOR_RIGHT_EN, PWM_CHANNEL_R);

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_3PIN
  pinMode(MOTOR_LEFT_DIR1, OUTPUT);
  pinMode(MOTOR_LEFT_DIR2, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR1, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR2, OUTPUT);
  
  ledcSetup(PWM_CHANNEL_L, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_R, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_LEFT_PWM, PWM_CHANNEL_L);
  ledcAttachPin(MOTOR_RIGHT_PWM, PWM_CHANNEL_R);

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_1PWM_1DIR
  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);
  
  ledcSetup(PWM_CHANNEL_L, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_R, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_LEFT_PWM, PWM_CHANNEL_L);
  ledcAttachPin(MOTOR_RIGHT_PWM, PWM_CHANNEL_R);
#endif
}

// Mengatur kecepatan motor kiri
// speed: -255 sampai 255 (negatif = mundur, positif = maju)
static void setMotorLeft(int speed) {
  if (MOTOR_LEFT_INVERT) speed = -speed;
  
#if MOTOR_DRIVER_MODE == MOTOR_MODE_2PIN
  if (speed >= 0) {
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
  } else {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    speed = -speed;
  }
  ledcWrite(PWM_CHANNEL_L, constrain(speed, 0, 255));

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_3PIN
  if (speed >= 0) {
    digitalWrite(MOTOR_LEFT_DIR1, HIGH);
    digitalWrite(MOTOR_LEFT_DIR2, LOW);
  } else {
    digitalWrite(MOTOR_LEFT_DIR1, LOW);
    digitalWrite(MOTOR_LEFT_DIR2, HIGH);
    speed = -speed;
  }
  ledcWrite(PWM_CHANNEL_L, constrain(speed, 0, 255));

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_1PWM_1DIR
  // Mode 1 PWM + 1 DIR
  // DIR LOW = non-inverting (maju), DIR HIGH = inverting (mundur)
  if (speed >= 0) {
    digitalWrite(MOTOR_LEFT_DIR, LOW);   // Non-inverting
    ledcWrite(PWM_CHANNEL_L, constrain(speed, 0, 255));
  } else {
    digitalWrite(MOTOR_LEFT_DIR, HIGH);  // Inverting
    ledcWrite(PWM_CHANNEL_L, constrain(-speed, 0, 255));
  }
#endif
}

// Mengatur kecepatan motor kanan
// speed: -255 sampai 255 (negatif = mundur, positif = maju)
static void setMotorRight(int speed) {
  if (MOTOR_RIGHT_INVERT) speed = -speed;
  
#if MOTOR_DRIVER_MODE == MOTOR_MODE_2PIN
  if (speed >= 0) {
    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
  } else {
    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, HIGH);
    speed = -speed;
  }
  ledcWrite(PWM_CHANNEL_R, constrain(speed, 0, 255));

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_3PIN
  if (speed >= 0) {
    digitalWrite(MOTOR_RIGHT_DIR1, HIGH);
    digitalWrite(MOTOR_RIGHT_DIR2, LOW);
  } else {
    digitalWrite(MOTOR_RIGHT_DIR1, LOW);
    digitalWrite(MOTOR_RIGHT_DIR2, HIGH);
    speed = -speed;
  }
  ledcWrite(PWM_CHANNEL_R, constrain(speed, 0, 255));

#elif MOTOR_DRIVER_MODE == MOTOR_MODE_1PWM_1DIR
  // Mode 1 PWM + 1 DIR
  // DIR LOW = non-inverting (maju), DIR HIGH = inverting (mundur)
  if (speed >= 0) {
    digitalWrite(MOTOR_RIGHT_DIR, LOW);   // Non-inverting
    ledcWrite(PWM_CHANNEL_R, constrain(speed, 0, 255));
  } else {
    digitalWrite(MOTOR_RIGHT_DIR, HIGH);  // Inverting
    ledcWrite(PWM_CHANNEL_R, constrain(-speed, 0, 255));
  }
#endif
}

// Mengatur kedua motor sekaligus
static void setMotors(int left, int right) {
  setMotorLeft(left);
  setMotorRight(right);
}

// Inisialisasi pin sensor
static void sensorSetup() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(SENSOR_PINS[i], INPUT);
  }
}

// Membaca nilai ADC 8 sensor (0-4095)
static void readSensors(uint16_t* values) {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    values[i] = analogRead(SENSOR_PINS[i]);
  }
}

// Membaca status digital sensor (ON/OFF berdasarkan threshold)
// Return: true = garis hitam terdeteksi, false = latar putih
static void readSensorsDigital(bool* digitalValues, uint16_t threshold = SENSOR_THRESHOLD) {
  uint16_t values[SENSOR_COUNT];
  readSensors(values);
  for (int i = 0; i < SENSOR_COUNT; i++) {
    digitalValues[i] = values[i] < threshold;
  }
}

// Menghitung posisi garis menggunakan switch-case 8-bit pattern
// Return: -7 to 7 (posisi garis relatif), lastPos jika pattern tidak ditemukan
// blackLine: true = garis hitam (default), false = garis putih
static int8_t calculatePosition(uint16_t* values, uint16_t threshold = SENSOR_THRESHOLD, bool blackLine = true) {
  // lastPos sebagai static local: setiap translation unit punya salinan sendiri yang konsisten
  static int8_t lastPos = 0;
  // Convert analog values to digital bits (bit0=S0, bit7=S7)
  // Jika garis hitam: nilai < threshold = 1 (ON)
  // Jika garis putih: nilai >= threshold = 1 (ON)
  uint8_t pattern = 0;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (blackLine) {
      // Garis hitam: sensor aktif jika nilai < threshold
      if (values[i] < threshold) {
        pattern |= (1 << i);
      }
    } else {
      // Garis putih: sensor aktif jika nilai >= threshold
      if (values[i] >= threshold) {
        pattern |= (1 << i);
      }
    }
  }

  // Switch-case based on 8-bit sensor pattern
  switch (pattern) {
    case 0b10000000: lastPos = -7; return -7;
    case 0b11000000: lastPos = -6; return -6;
    case 0b01000000: lastPos = -5; return -5;
    case 0b01100000: lastPos = -4; return -4;
    case 0b00100000: lastPos = -3; return -3;
    case 0b00110000: lastPos = -2; return -2;
    case 0b00010000: lastPos = -1; return -1;
    case 0b00011000: lastPos = 0; return 0;
    case 0b00001000: lastPos = 1; return 1;
    case 0b00001100: lastPos = 2; return 2;
    case 0b00000100: lastPos = 3; return 3;
    case 0b00000110: lastPos = 4; return 4;
    case 0b00000010: lastPos = 5; return 5;
    case 0b00000011: lastPos = 6; return 6;
    case 0b00000001: lastPos = 7; return 7;

    // Wide patterns & all sensors (hold last position)
    case 0b11111111:
    case 0b00111100:
    case 0b01111100:
    case 0b00111110:
    case 0b01111110:
    case 0b01111000:
    case 0b11111000:
    case 0b00011110:
    case 0b00011111:
    case 0b01111111:
    case 0b11111110:
    case 0b00111111:
    case 0b11111100:
      return lastPos;

    // No sensors active (line lost)
    case 0b00000000:
      return lastPos;

    // Default: weighted average fallback
    default:
      int sum = 0, count = 0;
      for (int i = 0; i < SENSOR_COUNT; i++) {
        bool active = blackLine ? (values[i] < threshold) : (values[i] >= threshold);
        if (active) {
          sum += (i - 3);
          count++;
        }
      }
      if (count > 0) {
        lastPos = sum / count;
        if (lastPos < -7) lastPos = -7;
        if (lastPos > 7) lastPos = 7;
        return lastPos;
      }
      return lastPos;
  }
}

// ==================== EEPROM CONFIGURATION ====================
#include <EEPROM.h>
#define EEPROM_SIZE 512
#define EEPROM_MAGIC 0xAB
#define EEPROM_ADDR_MAGIC 0
#define EEPROM_ADDR_THRESHOLD_START 1  // 8 sensors * 2 bytes = 16 bytes
#define EEPROM_ADDR_PATH_START 17       // Path data starts here

// === EEPROM INTEGRATION ===
// eeprom_config.h is now included in main.cpp files directly
// to avoid circular dependency issues

#endif // CONFIG_H
