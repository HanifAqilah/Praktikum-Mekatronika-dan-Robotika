/**
 * sensor.h - Sensor Reading Functions
 * Modul 4: Sensor Fusion & Conditioning
 * 
 * Handles:
 * - Encoder reading (with interrupt)
 * - IMU reading (I2C communication)
 * - Reference sensor (if available)
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"
#include <Wire.h>

// ============================================================================
// ENCODER
// ============================================================================

// Global variables (for interrupt handler)
volatile long encoder_ticks = 0;
volatile int encoder_direction = 1;        // 1: forward, -1: backward

/**
 * Encoder ISR (Interrupt Service Routine)
 * Called on encoder pin change
 */
void IRAM_ATTR encoderISR() {
  // Read both encoder pins
  int A = digitalRead(ENCODER_PIN_A);
  int B = digitalRead(ENCODER_PIN_B);
  
  // Determine direction (simple quadrature decoding)
  // Rising edge on A: if B=0 → forward, if B=1 → backward
  if (A == HIGH) {
    encoder_direction = (B == LOW) ? 1 : -1;
  } else {
    encoder_direction = (B == HIGH) ? 1 : -1;
  }
  
  encoder_ticks += encoder_direction * ENCODER_DIRECTION;
}

/**
 * Initialize encoder
 */
void initEncoder() {
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  
  // Attach interrupt to both pins (for full quadrature)
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoderISR, CHANGE);  // Optional: decode both edges
  
  encoder_ticks = 0;
  encoder_direction = 1;
}

/**
 * Read encoder position in mm
 * @return Position in mm
 */
float readEncoderPosition() {
  // Disable interrupts briefly to read volatile variable safely
  noInterrupts();
  long ticks = encoder_ticks;
  interrupts();
  
  // Convert ticks to mm
  float position_mm = (float)ticks / ENCODER_TICKS_PER_MM;
  
  return position_mm;
}

/**
 * Read encoder ticks (raw)
 * @return Tick count
 */
long readEncoderTicks() {
  noInterrupts();
  long ticks = encoder_ticks;
  interrupts();
  return ticks;
}

/**
 * Reset encoder position to zero
 */
void resetEncoder() {
  noInterrupts();
  encoder_ticks = 0;
  interrupts();
}

// ============================================================================
// IMU (MPU6050 or compatible)
// ============================================================================

// MPU6050 I2C address
#define MPU6050_ADDR 0x68

// MPU6050 registers
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_GYRO_XOUT_H 0x43

// IMU data struct
struct IMUData {
  float accel_x;                           // m/s²
  float accel_y;
  float accel_z;
  float gyro_x;                            // rad/s
  float gyro_y;
  float gyro_z;
  unsigned long timestamp;
};

IMUData imu_data;

/**
 * Write byte to MPU6050 register
 */
void MPU6050_write(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 * Read multiple bytes from MPU6050
 */
void MPU6050_read(uint8_t reg, uint8_t* buffer, int length) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);             // Keep connection open
  
  Wire.requestFrom(MPU6050_ADDR, length);
  for (int i = 0; i < length && Wire.available(); i++) {
    buffer[i] = Wire.read();
  }
}

/**
 * Initialize IMU (MPU6050)
 */
bool initIMU() {
  Wire.begin(IMU_SDA, IMU_SCL);
  Wire.setClock(400000);                   // 400kHz I2C speed
  
  // Wake up MPU6050 (reset PWR_MGMT_1)
  MPU6050_write(MPU6050_REG_PWR_MGMT_1, 0x00);
  delay(100);
  
  // Verify communication
  Wire.beginTransmission(MPU6050_ADDR);
  if (Wire.endTransmission() != 0) {
    return false;  // MPU6050 not found
  }
  
  // Configure accelerometer range: ±2g (default)
  // Configure gyroscope range: ±250°/s (default)
  // (Using default settings, no need to write)
  
  return true;
}

/**
 * Read IMU data (accel + gyro)
 * @return IMUData struct with latest readings
 */
IMUData readIMU() {
  uint8_t buffer[14];                      // 6 accel + 2 temp + 6 gyro = 14 bytes
  
  // Read all sensor data in one transaction (efficient)
  MPU6050_read(MPU6050_REG_ACCEL_XOUT_H, buffer, 14);
  
  // Parse accelerometer (16-bit signed, big-endian)
  int16_t ax_raw = (buffer[0] << 8) | buffer[1];
  int16_t ay_raw = (buffer[2] << 8) | buffer[3];
  int16_t az_raw = (buffer[4] << 8) | buffer[5];
  
  // Parse gyroscope
  int16_t gx_raw = (buffer[8] << 8) | buffer[9];
  int16_t gy_raw = (buffer[10] << 8) | buffer[11];
  int16_t gz_raw = (buffer[12] << 8) | buffer[13];
  
  // Convert to physical units
  // Accel: ±2g range → 16384 LSB/g
  imu_data.accel_x = (ax_raw / 16384.0) * 9.81 - ACCEL_BIAS_X;  // m/s²
  imu_data.accel_y = (ay_raw / 16384.0) * 9.81 - ACCEL_BIAS_Y;
  imu_data.accel_z = (az_raw / 16384.0) * 9.81 - ACCEL_BIAS_Z;
  
  // Gyro: ±250°/s range → 131 LSB/(°/s)
  imu_data.gyro_x = (gx_raw / 131.0) * (PI / 180.0);  // rad/s
  imu_data.gyro_y = (gy_raw / 131.0) * (PI / 180.0);
  imu_data.gyro_z = (gz_raw / 131.0) * (PI / 180.0);
  
  imu_data.timestamp = millis();
  
  return imu_data;
}

/**
 * Read only accelerometer (faster if gyro not needed)
 * @return Acceleration in m/s² (x-axis only)
 */
float readAccelX() {
  uint8_t buffer[2];
  MPU6050_read(MPU6050_REG_ACCEL_XOUT_H, buffer, 2);
  
  int16_t ax_raw = (buffer[0] << 8) | buffer[1];
  float accel_x = (ax_raw / 16384.0) * 9.81 - ACCEL_BIAS_X;
  
  return accel_x;
}

// ============================================================================
// REFERENCE SENSOR (Potentiometer or Second Encoder)
// ============================================================================

/**
 * Initialize reference sensor (ADC)
 */
void initReferenceSensor() {
  pinMode(REFERENCE_SENSOR_PIN, INPUT);
  
  #ifdef BOARD_ESP32
    // ESP32: set ADC resolution
    analogReadResolution(12);              // 12-bit (0-4095)
    analogSetAttenuation(ADC_11db);        // 0-3.3V range
  #endif
}

/**
 * Read reference sensor
 * @return Position in mm (calibrated)
 */
float readReferenceSensor() {
  int adc_value = analogRead(REFERENCE_SENSOR_PIN);
  
  // Map ADC value to position (mm)
  float position_mm = ((float)(adc_value - REF_SENSOR_MIN) / 
                       (REF_SENSOR_MAX - REF_SENSOR_MIN)) * REF_SENSOR_RANGE_MM;
  
  return position_mm;
}

// ============================================================================
// SENSOR VALIDATION
// ============================================================================

/**
 * Check if sensor value is within valid range
 * @param value Sensor reading
 * @return true if valid, false if out of range
 */
bool isSensorValid(float value) {
  return (value >= MIN_POSITION_MM && value <= MAX_POSITION_MM);
}

/**
 * Detect sensor timeout (no update for SENSOR_TIMEOUT_MS)
 * @param last_update_time Last sensor update timestamp (millis)
 * @return true if timeout detected
 */
bool isSensorTimeout(unsigned long last_update_time) {
  return (millis() - last_update_time > SENSOR_TIMEOUT_MS);
}

#endif // SENSOR_H
