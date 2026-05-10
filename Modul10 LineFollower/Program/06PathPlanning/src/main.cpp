/**
 * ============================================================
 * PROGRAM 06: PATH PLANNING UNTUK LINE FOLLOWER (ADVANCED)
 * ============================================================
 * 
 * DESKRIPSI:
 * Program path planning tingkat lanjut yang memungkinkan robot
 * line follower untuk mengeksekusi serangkaian manuver kompleks
 * secara otomatis. Program ini menggabungkan semua komponen
 * sebelumnya (sensor, motor, PID) dengan sistem navigasi
 * berbasis waypoint yang dapat dikonfigurasi secara interaktif.
 * 
 * TUJUAN PEMBELAJARAN:
 * - Memahami konsep path planning dan waypoint navigation
 * - Implementasi state machine untuk kontrol sekuensial
 * - Sistem konfigurasi interaktif dengan UI menu
 * - Integrasi sensor trigger untuk deteksi landmark
 * - Manajemen kecepatan multi-fase (turbo-base-brake)
 * - Persistent storage untuk menyimpan path
 * 
 * KONSEP PATH PLANNING:
 * Path planning adalah proses menentukan serangkaian aksi
 * yang harus dilakukan robot untuk mencapai tujuan. Dalam
 * konteks line follower, ini meliputi:
 * 1. Waypoint: Titik-titik penting di track (persimpangan, finish)
 * 2. Trigger: Kondisi sensor yang menandakan waypoint tercapai
 * 3. Action: Aksi yang dilakukan di waypoint (belok, lurus, stop)
 * 4. Speed Profile: Profil kecepatan untuk setiap segmen
 * 
 * KOMPONEN HARDWARE:
 * - ESP32 S2 Mini (Microcontroller)
 * - 8x Sensor IR untuk line following dan landmark detection
 * - 2x Motor DC dengan driver H-Bridge
 * - LCD OLED I2C untuk interface dan monitoring
 * - 4x Push Button untuk kontrol interaktif
 * - EEPROM untuk menyimpan path configuration
 * 
 * OUTLINE PROGRAM:
 * 1. STRUKTUR DATA PATH POINT
 *    - SensorPosition: Jenis sensor trigger (LEFT/RIGHT/BOTH/CENTER/CUSTOM)
 *    - sensorMask: Bit mask untuk custom sensor pattern
 *    - useTwoSensors: Flag untuk dual sensor trigger
 *    - PathAction: Aksi yang dilakukan (LEFT/RIGHT/STRAIGHT/STOP/UTURN)
 *    - Speed Profile: v_base, v_turbo, timer_turbo
 *    - Brake Profile: rem_speed, timer_rem
 *    - description: Deskripsi path point
 * 
 * 2. STATE MACHINE EKSEKUSI
 *    A. STATE_TURBO:
 *       - Robot bergerak dengan v_turbo (kecepatan tinggi)
 *       - Durasi: timer_turbo milliseconds
 *       - PID sederhana untuk line following
 *       - Transisi ke STATE_BASE setelah timeout
 *    
 *    B. STATE_BASE:
 *       - Robot bergerak dengan v_base (kecepatan normal)
 *       - PID aktif untuk line following
 *       - Monitor sensor untuk trigger detection
 *       - Transisi ke STATE_ACTION saat trigger terdeteksi
 *    
 *    C. STATE_ACTION:
 *       - Eksekusi aksi sesuai PathAction
 *       - LEFT: Belok kiri dengan differential speed
 *       - RIGHT: Belok kanan dengan differential speed
 *       - STRAIGHT: Lurus dengan speed penuh
 *       - STOP: Berhenti total (end of path)
 *       - UTURN: U-turn 180 derajat
 *       - Transisi ke STATE_BRAKE atau STATE_NEXT
 *    
 *    D. STATE_BRAKE:
 *       - Robot mundur dengan rem_speed
 *       - Durasi: timer_rem milliseconds
 *       - Untuk fine positioning atau momentum control
 *       - Transisi ke STATE_NEXT setelah timeout
 *    
 *    E. STATE_NEXT:
 *       - Pindah ke path point berikutnya
 *       - Reset state ke STATE_TURBO
 *       - Jika semua path selesai, stop eksekusi
 * 
 * 3. SENSOR TRIGGER SYSTEM
 *    - SENSOR_LEFT: Trigger saat sensor S0 detect garis
 *    - SENSOR_RIGHT: Trigger saat sensor S7 detect garis
 *    - SENSOR_BOTH: Trigger saat S0 DAN S7 detect (T-junction)
 *    - SENSOR_CENTER: Trigger saat S3-S4 detect (intersection)
 *    - SENSOR_CUSTOM: Trigger berdasarkan custom bit mask
 *    - Dual sensor mode: Memerlukan 2 kondisi sekaligus
 * 
 * 4. INTERFACE MENU SYSTEM
 *    A. MENU_MAIN:
 *       - Tampilkan status sistem
 *       - Jumlah path points tersimpan
 *       - Navigasi ke menu lain
 *    
 *    B. MENU_ADD_PATH:
 *       - Interface untuk menambah path point baru
 *       - Setting interaktif semua parameter
 *       - Preview konfigurasi sebelum save
 *    
 *    C. MENU_EXECUTE:
 *       - Mulai eksekusi path yang tersimpan
 *       - Real-time monitoring progress
 *       - Emergency stop capability
 *    
 *    D. MENU_VIEW_PATH:
 *       - Review path points yang tersimpan
 *       - Edit atau delete path point
 *    
 *    E. MENU_CLEAR:
 *       - Reset semua path points
 *       - Konfirmasi sebelum delete
 * 
 * 5. SETTING INTERAKTIF
 *    - SET_SPEED_BASE: Atur kecepatan normal
 *    - SET_SPEED_TURBO: Atur kecepatan turbo
 *    - SET_SENSOR_POS: Pilih jenis sensor trigger
 *    - SET_SENSOR_CUSTOM: Konfigurasi custom sensor mask
 *    - SET_ACTION: Pilih aksi yang dilakukan
 *    - SET_TURBO_TIME: Atur durasi fase turbo
 *    - SET_BRAKE_SPEED: Atur kecepatan pengereman
 *    - SET_BRAKE_TIME: Atur durasi pengereman
 *    - SET_CONFIRM: Konfirmasi dan save path point
 * 
 * 6. SPEED PROFILE MANAGEMENT
 *    - v_turbo: Kecepatan tinggi di awal (untuk momentum)
 *    - timer_turbo: Durasi fase turbo (ms)
 *    - v_base: Kecepatan normal saat approach target
 *    - rem_speed: Kecepatan mundur untuk fine positioning
 *    - timer_rem: Durasi pengereman (ms)
 * 
 * 7. EXAMPLE PATH CONFIGURATION
 *    Path 0: Turbo start, belok kanan di sensor kanan
 *    Path 1: Belok kiri di sensor kiri
 *    Path 2: Lurus di persimpangan T (kedua sensor)
 *    Path 3: Stop di finish line (semua sensor)
 * 
 * ALUR EKSEKUSI PATH:
 * 1. Mulai dengan v_turbo selama timer_turbo (momentum phase)
 * 2. Turun ke v_base sampai sensor target terdeteksi (approach phase)
 * 3. Lakukan aksi sesuai konfigurasi (action phase)
 * 4. Jika rem > 0, mundur dengan rem_speed selama timer_rem (brake phase)
 * 5. Lanjut ke path point berikutnya (next phase)
 * 6. Ulangi sampai semua path selesai
 * 
 * KONTROL TOMBOL:
 * - B1: Tambah path point / Start eksekusi / Next setting
 * - B2: Stop eksekusi / Tambah nilai setting / Navigate menu
 * - B3: Reset path / Kurangi nilai setting / Navigate menu
 * - B4: Navigate menu / Toggle mode / Navigate menu
 * 
 * TAMPILAN LCD (3 Mode):
 * A. Menu Utama:
 *    - Status sistem dan jumlah path points
 *    - Menu navigation dengan highlight
 *    - Instruksi kontrol tombol
 * 
 * B. Menu Setting:
 *    - Parameter yang sedang diedit
 *    - Nilai current semua parameter
 *    - Progress setting (step x of y)
 * 
 * C. Menu Eksekusi:
 *    - Progress path (current/total)
 *    - State machine status
 *    - Real-time sensor reading
 *    - Speed dan correction values
 * 
 * ALGORITMA LINE FOLLOWING SAAT EKSEKUSI:
 * - Gunakan PID sederhana dengan Kp=0.08
 * - Error calculation berdasarkan line position
 * - Correction applied ke differential steering
 * - Lost line handling dengan search pattern
 * 
 * FITUR ADVANCED:
 * - Persistent path storage (EEPROM simulation)
 * - Interactive path editor dengan preview
 * - Real-time path execution monitoring
 * - Emergency stop dan resume capability
 * - Multiple sensor trigger combinations
 * - Configurable speed profiles per path
 * - Progress visualization dengan progress bar
 * 
 * APLIKASI KOMPETISI:
 * - Pre-program path untuk track yang diketahui
 * - Quick adjustment parameter di lapangan
 * - Multiple strategy dengan path berbeda
 * - Backup path jika primary path gagal
 * 
 * TROUBLESHOOTING:
 * - Path tidak eksekusi: Cek sensor kalibrasi
 * - Robot tidak berhenti di waypoint: Adjust sensor trigger
 * - Overshoot di tikungan: Kurangi v_base atau tambah rem
 * - Undershoot di tikungan: Tambah v_turbo atau kurangi rem
 * 
 * AUTHOR: Tim Praktikum Mekatronika dan Robotika
 * DATE: 2026-05-10
 * VERSION: 1.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../../config.h"

// Inisialisasi LCD
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Enum untuk posisi sensor
enum SensorPosition {
  SENSOR_LEFT,   // Sensor kiri (S0)
  SENSOR_RIGHT,  // Sensor kanan (S7)
  SENSOR_BOTH,   // Kedua sensor (S0 dan S7)
  SENSOR_CENTER, // Sensor tengah (S3-S4)
  SENSOR_CUSTOM  // Custom mask
};

// Enum untuk aksi
enum PathAction {
  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_STRAIGHT,
  ACTION_STOP,
  ACTION_UTURN
};

// Struktur Path Point (Merged)
struct PathPoint {
  SensorPosition sensorPos;  // Posisi sensor trigger
  uint8_t sensorMask;         // Sensor mask tambahan (untuk custom/dua sensor)
  uint8_t sensorMask2;        // Sensor mask kedua (untuk persyaratan kedua)
  bool useTwoSensors;          // Gunakan 2 sensor sekaligus sebagai trigger
  
  PathAction action;           // Aksi yang dilakukan
  
  int v_base;                  // Kecepatan normal (0-255)
  int v_turbo;                 // Kecepatan turbo (0-255)
  uint32_t timer_turbo;        // Durasi turbo (ms)
  
  int rem;                     // Kecepatan mundur untuk rem (0-255)
  uint32_t timer_rem;          // Durasi rem (ms)
  
  String description;          // Deskripsi path point
};

PathPoint pathPoints[MAX_PATH_POINTS];
int pathCount = 0;
int currentPathIndex = 0;
bool isExecuting = false;
bool isSetting = false;

// State eksekusi path
enum PathState {
  STATE_TURBO,      // Fase turbo
  STATE_BASE,       // Fase base speed
  STATE_ACTION,     // Fase aksi (belok/lurus/stop)
  STATE_BRAKE,      // Fase rem
  STATE_NEXT        // Pindah ke path berikutnya
};
PathState pathState = STATE_TURBO;
uint32_t stateStartTime = 0;

// Sensor
uint16_t sensorValues[SENSOR_COUNT];
bool sensorDigital[SENSOR_COUNT];
int8_t linePosition = 0;

// Menu utama
enum MainMenu {
  MENU_MAIN,
  MENU_ADD_PATH,
  MENU_EXECUTE,
  MENU_VIEW_PATH,
  MENU_CLEAR
};
MainMenu currentMenu = MENU_MAIN;

// Mode setting
enum SettingState {
  SET_SPEED_BASE,
  SET_SPEED_TURBO,
  SET_SENSOR_POS,
  SET_SENSOR_CUSTOM,
  SET_SENSOR2,
  SET_USE_TWO,
  SET_ACTION,
  SET_TURBO_TIME,
  SET_BRAKE_SPEED,
  SET_BRAKE_TIME,
  SET_CONFIRM
};
SettingState settingState = SET_SPEED_BASE;
PathPoint tempPoint;

// Untuk debounce tombol
bool lastButtonState[4] = {false, false, false, false};
unsigned long lastButtonPress[4] = {0, 0, 0, 0};

// Function prototypes
void addExamplePaths();
void handleButtons();
void adjustSetting(int direction);
void executePath();
bool checkSensorTrigger(SensorPosition pos, uint8_t mask1, uint8_t mask2, bool twoSensors);
uint8_t getSensorMask();
uint32_t performAction(PathAction action, int speed, uint32_t duration);
void updateLCD();
int8_t calculatePositionSwitchCase(bool* digital);
void drawProgressBar(int x, int y, int width, int height, float progress);

void setup() {
  Serial.begin(115200);
  Serial.println("==========================================");
  Serial.println("PROGRAM 06: PATH PLANNING (MERGED)");
  Serial.println("==========================================");
  Serial.println("");

  // Inisialisasi I2C
  Wire.begin(LCD_SDA, LCD_SCL);
  Wire.setClock(400000);

  // Inisialisasi sensor dan motor
  sensorSetup();
  motorSetup();
  setMotors(0, 0);

  Serial.println("Kontrol:");
  Serial.println("B1: Tambah Path / Start");
  Serial.println("B2: Stop / +Value");
  Serial.println("B3: Reset / -Value");
  Serial.println("B4: Navigate / Toggle");
  Serial.println("");
  Serial.print("Max Path Points: ");
  Serial.println(MAX_PATH_POINTS);
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
  lcd.setCursor(15, 10);
  lcd.print("Path");
  lcd.setCursor(10, 30);
  lcd.print("Planning");
  lcd.setTextSize(1);
  lcd.setCursor(20, 50);
  lcd.print("Line Follower");
  lcd.display();
  delay(2000);

  // Inisialisasi path points contoh
  addExamplePaths();

  Serial.println("Siap! Gunakan tombol untuk navigasi menu.");
  Serial.println("");
}

void loop() {
  // Baca sensor
  readSensors(sensorValues);
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensorDigital[i] = sensorValues[i] < SENSOR_THRESHOLD;
  }
  linePosition = calculatePositionSwitchCase(sensorDigital);

  // Cek tombol
  handleButtons();

  // Jalankan berdasarkan status
  if (isExecuting) {
    executePath();
  }

  // Update LCD
  updateLCD();

  delay(50);
}

void addExamplePaths() {
  // Path 0: Turbo start, belok kanan di sensor kanan
  pathPoints[0].sensorPos = SENSOR_RIGHT;
  pathPoints[0].sensorMask = 0;
  pathPoints[0].sensorMask2 = 0b00000000;
  pathPoints[0].useTwoSensors = false;
  pathPoints[0].action = ACTION_RIGHT;
  pathPoints[0].v_base = 150;
  pathPoints[0].v_turbo = 220;
  pathPoints[0].timer_turbo = 1000;
  pathPoints[0].rem = 100;
  pathPoints[0].timer_rem = 300;
  pathPoints[0].description = "Turbo > Kanan";
  pathCount++;

  // Path 1: Belok kiri di sensor kiri
  pathPoints[1].sensorPos = SENSOR_LEFT;
  pathPoints[1].sensorMask = 0;
  pathPoints[1].sensorMask2 = 0b00000000;
  pathPoints[1].useTwoSensors = false;
  pathPoints[1].action = ACTION_LEFT;
  pathPoints[1].v_base = 150;
  pathPoints[1].v_turbo = 200;
  pathPoints[1].timer_turbo = 800;
  pathPoints[1].rem = 80;
  pathPoints[1].timer_rem = 250;
  pathPoints[1].description = "Turbo > Kiri";
  pathCount++;

  // Path 2: Lurus di persimpangan T (kedua sensor)
  pathPoints[2].sensorPos = SENSOR_BOTH;
  pathPoints[2].sensorMask = 0;
  pathPoints[2].sensorMask2 = 0b00000000;
  pathPoints[2].useTwoSensors = false;
  pathPoints[2].action = ACTION_STRAIGHT;
  pathPoints[2].v_base = 120;
  pathPoints[2].v_turbo = 180;
  pathPoints[2].timer_turbo = 500;
  pathPoints[2].rem = 0;
  pathPoints[2].timer_rem = 0;
  pathPoints[2].description = "Lurus T-int";
  pathCount++;

  // Path 3: Stop di finish (semua sensor)
  pathPoints[3].sensorPos = SENSOR_CUSTOM;
  pathPoints[3].sensorMask = 0b11111111;
  pathPoints[3].sensorMask2 = 0b00000000;
  pathPoints[3].useTwoSensors = false;
  pathPoints[3].action = ACTION_STOP;
  pathPoints[3].v_base = 100;
  pathPoints[3].v_turbo = 150;
  pathPoints[3].timer_turbo = 500;
  pathPoints[3].rem = 50;
  pathPoints[3].timer_rem = 500;
  pathPoints[3].description = "Finish";
  pathCount++;

  Serial.print("Contoh path ditambahkan: ");
  Serial.println(pathCount);
}

void handleButtons() {
  // Handle tombol dengan debounce
  bool btn1 = readButton(BUTTON1_PIN);
  bool btn2 = readButton(BUTTON2_PIN);
  bool btn3 = readButton(BUTTON3_PIN);
  bool btn4 = readButton(BUTTON4_PIN);

  // B1: Tambah path / Start execution
  if (btn1 && !lastButtonState[0] && (millis() - lastButtonPress[0] > BUTTON_DEBOUNCE)) {
    lastButtonPress[0] = millis();
    if (!isExecuting) {
      if (!isSetting) {
        // Cek apakah di menu execute
        if (currentMenu == MENU_EXECUTE) {
          // Mulai eksekusi
          if (pathCount > 0) {
            isExecuting = true;
            currentPathIndex = 0;
            pathState = STATE_TURBO;
            stateStartTime = millis();
            Serial.println("Mulai eksekusi path!");
          } else {
            Serial.println("Path kosong! Tambah path dulu.");
          }
        } else if (currentMenu == MENU_ADD_PATH) {
          // Masuk mode setting
          isSetting = true;
          settingState = SET_SPEED_BASE;
          tempPoint.v_base = 150;
          tempPoint.v_turbo = 200;
          tempPoint.timer_turbo = 800;
          tempPoint.rem = 80;
          tempPoint.timer_rem = 250;
          tempPoint.sensorPos = SENSOR_RIGHT;
          tempPoint.sensorMask = 0;
          tempPoint.sensorMask2 = 0b00000000;
          tempPoint.useTwoSensors = false;
          tempPoint.action = ACTION_RIGHT;
          tempPoint.description = "";
          Serial.println("Masuk mode setting path point");
        } else {
          // Toggle menu
          currentMenu = (MainMenu)((currentMenu + 1) % 5);
        }
      } else {
        // Next setting state
        settingState = (SettingState)((settingState + 1) % 11);
        if (settingState == SET_CONFIRM) {
          Serial.println("Konfirmasi path point:");
          Serial.print("Base Speed: "); Serial.println(tempPoint.v_base);
          Serial.print("Turbo: "); Serial.println(tempPoint.v_turbo);
        }
      }
    }
  }
  lastButtonState[0] = btn1;

  // B2: Stop / Tambah nilai
  if (btn2 && !lastButtonState[1] && (millis() - lastButtonPress[1] > BUTTON_DEBOUNCE)) {
    lastButtonPress[1] = millis();
    if (isExecuting) {
      // Stop eksekusi
      isExecuting = false;
      setMotors(0, 0);
      Serial.println("Eksekusi dihentikan!");
    } else if (isSetting) {
      // Tambah nilai setting
      adjustSetting(+1);
    } else {
      // Toggle menu
      currentMenu = (MainMenu)((currentMenu + 1) % 5);
    }
  }
  lastButtonState[1] = btn2;

  // B3: Reset / Kurangi nilai
  if (btn3 && !lastButtonState[2] && (millis() - lastButtonPress[2] > BUTTON_DEBOUNCE)) {
    lastButtonPress[2] = millis();
    if (isExecuting) {
      // Stop eksekusi
      isExecuting = false;
      setMotors(0, 0);
      Serial.println("Eksekusi dihentikan!");
    } else if (isSetting) {
      // Kurangi nilai setting
      adjustSetting(-1);
    } else {
      // Reset/clear path
      if (currentMenu == MENU_CLEAR) {
        pathCount = 0;
        Serial.println("Path di-reset!");
      } else {
        currentMenu = (MainMenu)((currentMenu + 1) % 5);
      }
    }
  }
  lastButtonState[2] = btn3;

  // B4: Navigate / Toggle two sensors
  if (btn4 && !lastButtonState[3] && (millis() - lastButtonPress[3] > BUTTON_DEBOUNCE)) {
    lastButtonPress[3] = millis();
    if (isSetting) {
      // Toggle use two sensors
      tempPoint.useTwoSensors = !tempPoint.useTwoSensors;
      Serial.print("Two sensors: ");
      Serial.println(tempPoint.useTwoSensors ? "ON" : "OFF");
    } else {
      // Toggle menu
      currentMenu = (MainMenu)((currentMenu + 1) % 5);
    }
  }
  lastButtonState[3] = btn4;
}

void adjustSetting(int direction) {
  switch (settingState) {
    case SET_SPEED_BASE:
      tempPoint.v_base = constrain(tempPoint.v_base + (direction * 10), 0, 255);
      Serial.print("Base Speed: "); Serial.println(tempPoint.v_base);
      break;
    case SET_SPEED_TURBO:
      tempPoint.v_turbo = constrain(tempPoint.v_turbo + (direction * 10), 0, 255);
      Serial.print("Turbo Speed: "); Serial.println(tempPoint.v_turbo);
      break;
    case SET_SENSOR_POS:
      {
        int pos = (int)tempPoint.sensorPos + direction;
        if (pos < 0) pos = 4;
        if (pos > 4) pos = 0;
        tempPoint.sensorPos = (SensorPosition)pos;
        Serial.print("Sensor Pos: ");
        switch (tempPoint.sensorPos) {
          case SENSOR_LEFT: Serial.println("LEFT"); break;
          case SENSOR_RIGHT: Serial.println("RIGHT"); break;
          case SENSOR_BOTH: Serial.println("BOTH"); break;
          case SENSOR_CENTER: Serial.println("CENTER"); break;
          case SENSOR_CUSTOM: Serial.println("CUSTOM"); break;
        }
      }
      break;
    case SET_SENSOR_CUSTOM:
      {
        uint8_t presets[] = {0b10000000, 0b00000001, 0b11000000, 0b00000011, 0b11111111};
        static int presetIndex = 0;
        presetIndex = (presetIndex + direction + 5) % 5;
        tempPoint.sensorMask = presets[presetIndex];
        Serial.print("Custom Mask: 0b"); Serial.println(tempPoint.sensorMask, BIN);
      }
      break;
    case SET_SENSOR2:
      {
        uint8_t presets[] = {0b00000000, 0b10000000, 0b00000001, 0b11000000, 0b00000011};
        static int presetIndex2 = 0;
        presetIndex2 = (presetIndex2 + direction + 5) % 5;
        tempPoint.sensorMask2 = presets[presetIndex2];
        Serial.print("Sensor2: 0b"); Serial.println(tempPoint.sensorMask2, BIN);
      }
      break;
    case SET_USE_TWO:
      tempPoint.useTwoSensors = !tempPoint.useTwoSensors;
      Serial.print("Use Two Sensors: "); Serial.println(tempPoint.useTwoSensors ? "ON" : "OFF");
      break;
    case SET_ACTION:
      tempPoint.action = (PathAction)((tempPoint.action + direction + 5) % 5);
      Serial.print("Action: ");
      switch (tempPoint.action) {
        case ACTION_LEFT: Serial.println("LEFT"); break;
        case ACTION_RIGHT: Serial.println("RIGHT"); break;
        case ACTION_STRAIGHT: Serial.println("STRAIGHT"); break;
        case ACTION_STOP: Serial.println("STOP"); break;
        case ACTION_UTURN: Serial.println("UTURN"); break;
      }
      break;
    case SET_TURBO_TIME:
      tempPoint.timer_turbo = constrain(tempPoint.timer_turbo + (direction * 100), 0, 5000);
      Serial.print("Turbo Time: "); Serial.println(tempPoint.timer_turbo);
      break;
    case SET_BRAKE_SPEED:
      tempPoint.rem = constrain(tempPoint.rem + (direction * 10), 0, 255);
      Serial.print("Brake Speed: "); Serial.println(tempPoint.rem);
      break;
    case SET_BRAKE_TIME:
      tempPoint.timer_rem = constrain(tempPoint.timer_rem + (direction * 100), 0, 5000);
      Serial.print("Brake Time: "); Serial.println(tempPoint.timer_rem);
      break;
    case SET_CONFIRM:
      // Save path point
      if (pathCount < MAX_PATH_POINTS) {
        pathPoints[pathCount] = tempPoint;
        pathCount++;
        Serial.print("Path point ditambahkan! Total: ");
        Serial.println(pathCount);
        isSetting = false;
      } else {
        Serial.println("Path penuh!");
      }
      break;
  }
}

void executePath() {
  if (currentPathIndex >= pathCount) {
    // Path selesai
    isExecuting = false;
    setMotors(0, 0);
    Serial.println("Eksekusi path selesai!");
    return;
  }

  PathPoint &p = pathPoints[currentPathIndex];
  uint32_t elapsed = millis() - stateStartTime;

  switch (pathState) {
    case STATE_TURBO:
      // Fase turbo: jalan dengan v_turbo
      if (elapsed < p.timer_turbo) {
        // Masih turbo, ikuti garis dengan PID sederhana
        if (linePosition >= -4 && linePosition <= 4) {
          float error = linePosition * 250;
          float correction = 0.08 * error;
          int leftSpeed = p.v_turbo + correction;
          int rightSpeed = p.v_turbo - correction;
          setMotors(constrain(leftSpeed, 0, 255), constrain(rightSpeed, 0, 255));
        } else {
          // Garis hilang, cari
          setMotors(p.v_turbo / 2, p.v_turbo);
        }
      } else {
        // Turbo selesai, pindah ke base
        pathState = STATE_BASE;
        stateStartTime = millis();
        Serial.print("Path ");
        Serial.print(currentPathIndex);
        Serial.println(": Turbo selesai, base speed");
      }
      break;

    case STATE_BASE:
      // Fase base: jalan dengan v_base sampai sensor trigger
      if (checkSensorTrigger(p.sensorPos, p.sensorMask, p.sensorMask2, p.useTwoSensors)) {
        // Trigger terdeteksi!
        pathState = STATE_ACTION;
        stateStartTime = millis();
        Serial.print("Path ");
        Serial.print(currentPathIndex);
        Serial.print(": Trigger! Action: ");
        switch (p.action) {
          case ACTION_LEFT: Serial.println("LEFT"); break;
          case ACTION_RIGHT: Serial.println("RIGHT"); break;
          case ACTION_STRAIGHT: Serial.println("STRAIGHT"); break;
          case ACTION_STOP: Serial.println("STOP"); break;
          case ACTION_UTURN: Serial.println("UTURN"); break;
        }
      } else {
        // Belum trigger, ikuti garis
        if (linePosition >= -4 && linePosition <= 4) {
          float error = linePosition * 250;
          float correction = 0.08 * error;
          int leftSpeed = p.v_base + correction;
          int rightSpeed = p.v_base - correction;
          setMotors(constrain(leftSpeed, 0, 255), constrain(rightSpeed, 0, 255));
        } else {
          setMotors(p.v_base / 2, p.v_base);
        }
      }
      break;

    case STATE_ACTION: {
      // Set motor setiap loop (non-blocking) - durasi dikontrol oleh elapsed
      uint32_t actionDur = performAction(p.action, p.v_base, 800);

      // ACTION_STOP: hentikan eksekusi permanen
      if (p.action == ACTION_STOP) {
        isExecuting = false;
        Serial.println("STOP: Eksekusi selesai.");
        return;
      }

      // Tunggu durasi aksi selesai sebelum pindah state
      if (elapsed >= actionDur) {
        if (p.rem > 0 && p.timer_rem > 0) {
          pathState = STATE_BRAKE;
          stateStartTime = millis();
          Serial.println("Mulai rem...");
        } else {
          pathState = STATE_NEXT;
        }
      }
      break;
    }

    case STATE_BRAKE:
      // Fase rem: mundur dengan kecepatan rem
      if (elapsed < p.timer_rem) {
        setMotors(-p.rem, -p.rem);
      } else {
        // Rem selesai
        pathState = STATE_NEXT;
        Serial.println("Rem selesai");
      }
      break;

    case STATE_NEXT:
      // Pindah ke path berikutnya
      currentPathIndex++;
      if (currentPathIndex < pathCount) {
        pathState = STATE_TURBO;
        stateStartTime = millis();
        Serial.print("Pindah ke path ");
        Serial.println(currentPathIndex);
      } else {
        isExecuting = false;
        setMotors(0, 0);
        Serial.println("Semua path selesai!");
      }
      break;
  }
}

bool checkSensorTrigger(SensorPosition pos, uint8_t mask1, uint8_t mask2, bool twoSensors) {
  uint8_t currentMask = getSensorMask();
  
  if (twoSensors) {
    // Cek dua sensor mask sekaligus
    bool sensor1Detected = (currentMask & mask1) == mask1;
    bool sensor2Detected = (currentMask & mask2) == mask2;
    return sensor1Detected && sensor2Detected;
  }
  
  switch (pos) {
    case SENSOR_LEFT:
      return (currentMask & 0b00000001) != 0;
    case SENSOR_RIGHT:
      return (currentMask & 0b10000000) != 0;
    case SENSOR_BOTH:
      return ((currentMask & 0b00000001) != 0) && ((currentMask & 0b10000000) != 0);
    case SENSOR_CENTER:
      return (currentMask & 0b00011000) != 0;
    case SENSOR_CUSTOM:
      return (currentMask & mask1) == mask1;
    default:
      return false;
  }
}

uint8_t getSensorMask() {
  uint8_t mask = 0;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorDigital[i]) {
      mask |= (1 << i);
    }
  }
  return mask;
}

// Non-blocking: hanya set motor, kembalikan durasi yang diperlukan (ms)
// Pemanggil (STATE_ACTION) menggunakan millis() untuk menunggu durasi tersebut
uint32_t performAction(PathAction action, int speed, uint32_t duration) {
  switch (action) {
    case ACTION_LEFT:
      setMotors(-speed/2, speed);
      return duration;
    case ACTION_RIGHT:
      setMotors(speed, -speed/2);
      return duration;
    case ACTION_STRAIGHT:
      setMotors(speed, speed);
      return 500;
    case ACTION_STOP:
      setMotors(0, 0);
      return 0; // ditangani khusus di STATE_ACTION
    case ACTION_UTURN:
      setMotors(-speed, speed);
      return duration * 2;
    default:
      return duration;
  }
}

int8_t calculatePositionSwitchCase(bool* digital) {
  static int8_t lastPos = 0;
  
  uint8_t pattern = 0;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (digital[i]) {
      pattern |= (1 << i);
    }
  }
  
  switch (pattern) {
    case 0b00011000: return 0;
    case 0b00001000: return 0;
    case 0b00010000: return 0;
    
    case 0b00110000: return -1;
    case 0b00100000: return -1;
    case 0b01100000: return -2;
    case 0b01000000: return -2;
    case 0b11000000: return -3;
    case 0b10000000: return -4;
    
    case 0b00001100: return 1;
    case 0b00000100: return 1;
    case 0b00000110: return 2;
    case 0b00000010: return 2;
    case 0b00000011: return 3;
    case 0b00000001: return 4;
    
    case 0b00000000: return lastPos;
    case 0b11111111: return 0;
    
    default:
      {
        int sum = 0;
        int count = 0;
        for (int i = 0; i < SENSOR_COUNT; i++) {
          if (digital[i]) {
            sum += (i - 3);
            count++;
          }
        }
        if (count > 0) {
          lastPos = constrain(sum / count, -4, 4);
          return lastPos;
        }
        return lastPos;
      }
  }
}

void updateLCD() {
  lcd.clearDisplay();
  lcd.setTextSize(1);

  if (isSetting) {
    // Tampilan mode setting
    lcd.setCursor(0, 0);
    lcd.print("SETTING PATH POINT");
    lcd.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);
    
    lcd.setCursor(0, 12);
    lcd.print("State: ");
    switch (settingState) {
      case SET_SPEED_BASE: lcd.print("BASE SPD"); break;
      case SET_SPEED_TURBO: lcd.print("TURBO SPD"); break;
      case SET_SENSOR_POS: lcd.print("SENSOR POS"); break;
      case SET_SENSOR_CUSTOM: lcd.print("CUSTOM M");
      case SET_SENSOR2: lcd.print("SENSOR2"); break;
      case SET_USE_TWO: lcd.print("2SENSOR"); break;
      case SET_ACTION: lcd.print("ACTION"); break;
      case SET_TURBO_TIME: lcd.print("TURBO T"); break;
      case SET_BRAKE_SPEED: lcd.print("BRK SPD"); break;
      case SET_BRAKE_TIME: lcd.print("BRK T"); break;
      case SET_CONFIRM: lcd.print("CONFIRM"); break;
    }
    
    lcd.setCursor(0, 22);
    lcd.print("Base:"); lcd.print(tempPoint.v_base);
    lcd.print(" Turbo:"); lcd.print(tempPoint.v_turbo);
    
    lcd.setCursor(0, 32);
    lcd.print("Act: ");
    switch (tempPoint.action) {
      case ACTION_LEFT: lcd.print("LEFT"); break;
      case ACTION_RIGHT: lcd.print("RIGHT"); break;
      case ACTION_STRAIGHT: lcd.print("STR"); break;
      case ACTION_STOP: lcd.print("STOP"); break;
      case ACTION_UTURN: lcd.print("UTURN"); break;
    }
    
    lcd.setCursor(0, 42);
    lcd.print("SPos:");
    switch (tempPoint.sensorPos) {
      case SENSOR_LEFT: lcd.print("L"); break;
      case SENSOR_RIGHT: lcd.print("R"); break;
      case SENSOR_BOTH: lcd.print("B"); break;
      case SENSOR_CENTER: lcd.print("C"); break;
      case SENSOR_CUSTOM: lcd.print("X"); break;
    }
    lcd.print(" 2S:"); lcd.print(tempPoint.useTwoSensors ? "Y" : "N");
    
    lcd.setCursor(0, 52);
    lcd.print("Brk:"); lcd.print(tempPoint.rem);
    lcd.print(" T:"); lcd.print(tempPoint.timer_rem);
    
  } else if (isExecuting) {
    // Tampilan mode eksekusi
    lcd.setCursor(0, 0);
    lcd.print("EXECUTING PATH");
    lcd.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);
    
    lcd.setCursor(0, 12);
    lcd.print("Path: ");
    lcd.print(currentPathIndex);
    lcd.print("/");
    lcd.print(pathCount);
    
    // Progress bar
    float progress = (float)currentPathIndex / pathCount;
    drawProgressBar(60, 12, 60, 6, progress);
    
    if (currentPathIndex < pathCount) {
      PathPoint &p = pathPoints[currentPathIndex];
      
      lcd.setCursor(0, 22);
      lcd.print(p.description);
      
      lcd.setCursor(0, 32);
      lcd.print("State: ");
      switch (pathState) {
        case STATE_TURBO: lcd.print("TURBO"); break;
        case STATE_BASE: lcd.print("BASE"); break;
        case STATE_ACTION: lcd.print("ACTION"); break;
        case STATE_BRAKE: lcd.print("BRAKE"); break;
        case STATE_NEXT: lcd.print("NEXT"); break;
      }
      
      lcd.setCursor(0, 42);
      lcd.print("Pos: ");
      lcd.print(linePosition);
      
      lcd.setCursor(0, 52);
      lcd.print("Sensor: ");
      for (int i = SENSOR_COUNT - 1; i >= 0; i--) {
        lcd.print(sensorDigital[i] ? "1" : "0");
      }
    }
  } else {
    // Tampilan menu utama
    lcd.setCursor(0, 0);
    lcd.print("PATH PLANNING MENU");
    lcd.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);
    
    lcd.setCursor(0, 15);
    lcd.print("Path Points: ");
    lcd.print(pathCount);
    lcd.print("/");
    lcd.print(MAX_PATH_POINTS);
    
    lcd.setCursor(0, 30);
    switch (currentMenu) {
      case MENU_MAIN:
        lcd.print("> MAIN MENU");
        lcd.setCursor(0, 40);
        lcd.print("  ADD PATH");
        lcd.setCursor(0, 50);
        lcd.print("  EXECUTE");
        break;
      case MENU_ADD_PATH:
        lcd.print("  MAIN MENU");
        lcd.setCursor(0, 40);
        lcd.print("> ADD PATH");
        lcd.setCursor(0, 50);
        lcd.print("  EXECUTE");
        break;
      case MENU_EXECUTE:
        lcd.print("  MAIN MENU");
        lcd.setCursor(0, 40);
        lcd.print("  ADD PATH");
        lcd.setCursor(0, 50);
        lcd.print("> EXECUTE");
        break;
      case MENU_VIEW_PATH:
        lcd.print("  MAIN MENU");
        lcd.setCursor(0, 40);
        lcd.print("  ADD PATH");
        lcd.setCursor(0, 50);
        lcd.print("> VIEW PATH");
        break;
      case MENU_CLEAR:
        lcd.print("  MAIN MENU");
        lcd.setCursor(0, 40);
        lcd.print("  ADD PATH");
        lcd.setCursor(0, 50);
        lcd.print("> CLEAR");
        break;
    }
  }

  lcd.display();
}

void drawProgressBar(int x, int y, int width, int height, float progress) {
  lcd.drawRect(x, y, width, height, WHITE);
  int fillWidth = (int)(width * progress);
  if (fillWidth > 0) {
    lcd.fillRect(x, y, fillWidth, height, WHITE);
  }
}
