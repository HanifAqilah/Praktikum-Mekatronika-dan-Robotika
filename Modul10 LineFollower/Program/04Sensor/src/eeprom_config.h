#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

// Magic number untuk validasi EEPROM
#define EEPROM_MAGIC_NUMBER 0xABCD
#define EEPROM_VERSION 1

// Struktur konfigurasi robot line follower
struct LineFollowerConfig {
  uint16_t magic;
  uint8_t version;  
  // Sensor configuration
  uint16_t sensor_threshold;
  bool line_type_black;
  uint16_t sensor_min[SENSOR_COUNT];
  uint16_t sensor_max[SENSOR_COUNT];
  uint8_t sensor_smoothing;  
  // PID configuration
  float pid_kp;
  float pid_ki;
  float pid_kd;
  uint8_t pid_max_output;  
  // Motor configuration
  uint8_t base_speed;
  uint8_t max_speed;
  uint8_t min_speed;
  uint8_t turn_speed;
  bool motor_direction_left;
  bool motor_direction_right;  
  // Path planning configuration
  uint8_t default_v_base;
  uint8_t default_v_turbo;
  uint32_t default_timer_turbo;
  uint8_t default_rem_speed;
  uint32_t default_timer_rem;  
  // Advanced settings
  uint8_t button_debounce;
  uint8_t lcd_contrast;
  uint16_t lcd_update_interval;
  uint16_t lost_line_timeout;
  uint8_t search_speed;  
  uint16_t checksum;
};

extern LineFollowerConfig config;

void initEEPROM();
bool loadConfig();
bool saveConfig();
void resetToDefaults();
bool validateConfig();
uint16_t calculateChecksum(const LineFollowerConfig& cfg);

// Adjustment functions
void adjustSensorThreshold(int16_t delta);
void adjustPIDKp(float delta);
void adjustPIDKi(float delta);
void adjustPIDKd(float delta);
void adjustBaseSpeed(int16_t delta);
void adjustMaxSpeed(int16_t delta);
void adjustTurnSpeed(int16_t delta);
void toggleLineType();

// Calibration
void startSensorCalibration();
void updateSensorCalibration(uint16_t* sensorValues);
bool isCalibrating();
void endSensorCalibration();

// Debug
void printConfig();

#endif
