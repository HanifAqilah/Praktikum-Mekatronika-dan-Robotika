#include "eeprom_config.h"

LineFollowerConfig config;

void initEEPROM() {
  EEPROM.begin(512);
  
  if (!loadConfig()) {
    Serial.println("EEPROM kosong atau tidak valid, memuat default...");
    resetToDefaults();
    saveConfig();
  } else {
    Serial.println("Konfigurasi dari EEPROM berhasil dimuat");
  }
  printConfig();
}

bool loadConfig() {
  uint16_t magic;
  EEPROM.get(0, magic);
  
  if (magic != EEPROM_MAGIC_NUMBER) {
    return false;
  }
  
  EEPROM.get(0, config);
  return validateConfig();
}

bool saveConfig() {
  config.magic = EEPROM_MAGIC_NUMBER;
  config.version = EEPROM_VERSION;
  config.checksum = calculateChecksum(config);
  
  EEPROM.put(0, config);
  bool ok = EEPROM.commit();
  
  if (ok) {
    Serial.println("Konfigurasi tersimpan ke EEPROM");
  } else {
    Serial.println("Gagal menyimpan ke EEPROM");
  }
  
  return ok;
}

void resetToDefaults() {
  config.magic = EEPROM_MAGIC_NUMBER;
  config.version = EEPROM_VERSION;
  
  // Sensor defaults
  config.sensor_threshold = SENSOR_THRESHOLD;
  config.line_type_black = true;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    config.sensor_min[i] = 0;
    config.sensor_max[i] = 4095;
  }
  config.sensor_smoothing = 3;
  
  // PID defaults
  config.pid_kp = PID_KP;
  config.pid_ki = PID_KI;
  config.pid_kd = PID_KD;
  config.pid_max_output = 255;
  
  // Motor defaults
  config.base_speed = PID_BASE_SPEED;
  config.max_speed = PID_MAX_SPEED;
  config.min_speed = 50;
  config.turn_speed = 100;
  config.motor_direction_left = MOTOR_LEFT_INVERT;
  config.motor_direction_right = MOTOR_RIGHT_INVERT;
  
  // Path planning defaults
  config.default_v_base = 150;
  config.default_v_turbo = 200;
  config.default_timer_turbo = 800;
  config.default_rem_speed = 100;
  config.default_timer_rem = 500;
  
  // Advanced defaults
  config.button_debounce = BUTTON_DEBOUNCE;
  config.lcd_contrast = 128;
  config.lcd_update_interval = 100;
  config.lost_line_timeout = 2000;
  config.search_speed = 120;
  
  config.checksum = calculateChecksum(config);
  
  Serial.println("Reset ke konfigurasi default");
}

bool validateConfig() {
  if (config.magic != EEPROM_MAGIC_NUMBER) return false;
  if (config.version != EEPROM_VERSION) return false;
  
  uint16_t storedChecksum = config.checksum;
  config.checksum = 0;
  uint16_t calculatedChecksum = calculateChecksum(config);
  config.checksum = storedChecksum;
  
  return storedChecksum == calculatedChecksum;
}

uint16_t calculateChecksum(const LineFollowerConfig& cfg) {
  uint16_t checksum = 0;
  const uint8_t* data = (const uint8_t*)&cfg;
  for (size_t i = 0; i < sizeof(cfg) - sizeof(cfg.checksum); i++) {
    checksum += data[i];
  }
  return checksum;
}

void adjustSensorThreshold(int16_t delta) {
  int32_t newVal = (int32_t)config.sensor_threshold + delta;
  if (newVal < 0) newVal = 0;
  if (newVal > 4095) newVal = 4095;
  config.sensor_threshold = (uint16_t)newVal;
}

void adjustPIDKp(float delta) {
  config.pid_kp += delta;
  if (config.pid_kp < 0) config.pid_kp = 0;
}

void adjustPIDKi(float delta) {
  config.pid_ki += delta;
  if (config.pid_ki < 0) config.pid_ki = 0;
}

void adjustPIDKd(float delta) {
  config.pid_kd += delta;
  if (config.pid_kd < 0) config.pid_kd = 0;
}

void adjustBaseSpeed(int16_t delta) {
  int32_t newVal = (int32_t)config.base_speed + delta;
  if (newVal < 0) newVal = 0;
  if (newVal > 255) newVal = 255;
  config.base_speed = (uint8_t)newVal;
}

void adjustMaxSpeed(int16_t delta) {
  int32_t newVal = (int32_t)config.max_speed + delta;
  if (newVal < 0) newVal = 0;
  if (newVal > 255) newVal = 255;
  config.max_speed = (uint8_t)newVal;
}

void adjustTurnSpeed(int16_t delta) {
  int32_t newVal = (int32_t)config.turn_speed + delta;
  if (newVal < 0) newVal = 0;
  if (newVal > 255) newVal = 255;
  config.turn_speed = (uint8_t)newVal;
}

void toggleLineType() {
  config.line_type_black = !config.line_type_black;
}

static bool calibrating = false;
static uint32_t calibrationStart = 0;
static const uint32_t CALIBRATION_DURATION = 10000;

void startSensorCalibration() {
  calibrating = true;
  calibrationStart = millis();
  for (int i = 0; i < SENSOR_COUNT; i++) {
    config.sensor_min[i] = 4095;
    config.sensor_max[i] = 0;
  }
  Serial.println("Kalibrasi sensor dimulai (10 detik)...");
}

void updateSensorCalibration(uint16_t* sensorValues) {
  if (!calibrating) return;
  
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorValues[i] < config.sensor_min[i]) {
      config.sensor_min[i] = sensorValues[i];
    }
    if (sensorValues[i] > config.sensor_max[i]) {
      config.sensor_max[i] = sensorValues[i];
    }
  }
  
  if (millis() - calibrationStart >= CALIBRATION_DURATION) {
    endSensorCalibration();
  }
}

bool isCalibrating() {
  return calibrating;
}

void endSensorCalibration() {
  calibrating = false;
  config.sensor_threshold = 0;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    config.sensor_threshold += (config.sensor_min[i] + config.sensor_max[i]) / 2;
  }
  config.sensor_threshold /= SENSOR_COUNT;
  
  saveConfig();
  Serial.println("Kalibrasi selesai. Threshold baru: " + String(config.sensor_threshold));
}

void printConfig() {
  Serial.println("=== KONFIGURASI LINE FOLLOWER ===");
  Serial.println("Sensor Threshold: " + String(config.sensor_threshold));
  Serial.println("Line Type: " + String(config.line_type_black ? "Hitam" : "Putih"));
  Serial.println("PID - Kp: " + String(config.pid_kp, 3) + " Ki: " + String(config.pid_ki, 3) + " Kd: " + String(config.pid_kd, 3));
  Serial.println("Base Speed: " + String(config.base_speed));
  Serial.println("Max Speed: " + String(config.max_speed));
  Serial.println("Turn Speed: " + String(config.turn_speed));
  Serial.println("===================================");
}
