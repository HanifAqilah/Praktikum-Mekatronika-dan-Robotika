/**
 * config.h - Configuration & Pin Definitions
 * Modul 4: Sensor Fusion & Conditioning
 * 
 * All system parameters, pin mapping, and tunable constants
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// BOARD SELECTION
// ============================================================================
// Uncomment one:
#define BOARD_ESP32
// #define BOARD_ARDUINO_MEGA

// ============================================================================
// PIN DEFINITIONS
// ============================================================================

#ifdef BOARD_ESP32
  // Encoder pins (use any GPIO with interrupt support)
  #define ENCODER_PIN_A 25
  #define ENCODER_PIN_B 26
  
  // IMU I2C pins
  #define IMU_SDA 21
  #define IMU_SCL 22
  
  // Reference sensor (optional, e.g., potentiometer ADC)
  #define REFERENCE_SENSOR_PIN 34  // ADC1_CH6
  
  // Motor control (for testing)
  #define MOTOR_PWM_PIN 23
  #define MOTOR_DIR_PIN_1 18
  #define MOTOR_DIR_PIN_2 19
  
  // LED indicators
  #define LED_STATUS 2
  #define LED_FAULT 4
  
#elif defined(BOARD_ARDUINO_MEGA)
  // Encoder pins (interrupt pins: 2, 3, 18, 19, 20, 21)
  #define ENCODER_PIN_A 2
  #define ENCODER_PIN_B 3
  
  // IMU I2C pins (default Wire: SDA=20, SCL=21)
  #define IMU_SDA 20
  #define IMU_SCL 21
  
  // Reference sensor
  #define REFERENCE_SENSOR_PIN A0
  
  // Motor control
  #define MOTOR_PWM_PIN 9
  #define MOTOR_DIR_PIN_1 7
  #define MOTOR_DIR_PIN_2 8
  
  // LED indicators
  #define LED_STATUS 13
  #define LED_FAULT 12
#endif

// ============================================================================
// SENSOR CALIBRATION
// ============================================================================

// Encoder
#define ENCODER_PPR 600                    // Pulses Per Revolution
#define ENCODER_TICKS_PER_MM 10.0          // Calibration: ticks per mm (linear motion)
#define ENCODER_DIRECTION 1                // 1 or -1 (flip if backward)

// Accelerometer (MPU6050 default ±2g range)
#define ACCEL_RANGE_G 2                    // ±2g
#define ACCEL_SCALE (9.81 / 16384.0)       // m/s² per LSB (for ±2g: 16384 LSB/g)
#define ACCEL_BIAS_X 0.0                   // Calibration offset (m/s²)
#define ACCEL_BIAS_Y 0.0
#define ACCEL_BIAS_Z 0.0

// Reference sensor (if analog potentiometer)
#define REF_SENSOR_MIN 0                   // ADC value at min position
#define REF_SENSOR_MAX 4095                // ADC value at max position (12-bit ADC)
#define REF_SENSOR_RANGE_MM 100.0          // Physical range (mm)

// ============================================================================
// SAMPLING CONFIGURATION
// ============================================================================

#define SAMPLE_RATE_HZ 100                 // Sampling rate (Hz)
#define SAMPLE_PERIOD_MS (1000 / SAMPLE_RATE_HZ)  // 10ms for 100Hz
#define SAMPLE_PERIOD_S (1.0 / SAMPLE_RATE_HZ)    // 0.01s

// ============================================================================
// FILTER SELECTION
// ============================================================================

// Filter type (uncomment one for testing)
enum FilterType {
  FILTER_NONE,
  FILTER_MOVING_AVERAGE,
  FILTER_LOWPASS,
  FILTER_COMPLEMENTARY,
  FILTER_KALMAN
};

#define FILTER_TYPE FILTER_COMPLEMENTARY   // Change this to test different filters

// ============================================================================
// MOVING AVERAGE FILTER PARAMETERS
// ============================================================================

#define MA_WINDOW_SIZE 5                   // Window size (3, 5, 10, 20)

// ============================================================================
// LOW-PASS FILTER PARAMETERS
// ============================================================================

#define LOWPASS_CUTOFF_HZ 5.0              // Cutoff frequency (Hz)
// Alpha will be calculated: α = 2πfc·dt / (2πfc·dt + 1)

// ============================================================================
// COMPLEMENTARY FILTER PARAMETERS
// ============================================================================

#define W_ENCODER 0.7                      // Weight for encoder (0.0 - 1.0)
#define W_ACCEL 0.3                        // Weight for accel (must sum to 1.0)

#define ENCODER_LOWPASS_CUTOFF 5.0         // Hz (for encoder path)
#define ACCEL_HIGHPASS_CUTOFF 1.0          // Hz (for accel path, implicit)

// ============================================================================
// KALMAN FILTER PARAMETERS
// ============================================================================

#define KALMAN_Q 0.001                     // Process noise covariance
#define KALMAN_R 10.0                      // Measurement noise covariance

// For 1D motion: state = [position, velocity]
#define KALMAN_STATE_DIM 2

// ============================================================================
// SENSOR VOTING PARAMETERS
// ============================================================================

#define ENABLE_VOTING false                // Enable 3-sensor voting
#define NUM_SENSORS 3                      // Number of sensors (2 or 3)
#define VOTING_TOLERANCE 5.0               // mm (sensors agree if within tolerance)
#define FAULT_ALARM_THRESHOLD 10           // Consecutive fault count to trigger alarm

// ============================================================================
// SERIAL COMMUNICATION
// ============================================================================

#define SERIAL_BAUD 115200                 // Baud rate
#define SERIAL_OUTPUT_CSV true             // Output format: CSV (true) or human-readable (false)

// CSV header: timestamp, encoder_raw, accel_x, encoder_filtered, position_fused, voting_result
#define CSV_HEADER "time,enc_raw,accel_x,enc_filt,pos_fused,voting"

// ============================================================================
// DEBUG & LOGGING
// ============================================================================

#define DEBUG_MODE true                    // Enable debug print
#define LOG_INTERVAL_MS 100                // Log every 100ms (in addition to sample)

// Debug macros
#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// ============================================================================
// SYSTEM LIMITS & SAFETY
// ============================================================================

#define MAX_POSITION_MM 1000.0             // Maximum valid position (mm)
#define MIN_POSITION_MM -1000.0            // Minimum valid position (mm)

#define SENSOR_TIMEOUT_MS 1000             // Sensor no update → timeout
#define WATCHDOG_TIMEOUT_MS 5000           // System watchdog (if no activity)

// ============================================================================
// FILTER TEST MODES (for Percobaan)
// ============================================================================

// Uncomment to enable specific test
// #define TEST_MODE_NOISE_ANALYSIS       // Percobaan 1: log raw sensor only
// #define TEST_MODE_MOVING_AVERAGE       // Percobaan 2: test MA filter
// #define TEST_MODE_LOWPASS              // Percobaan 3: test low-pass filter
// #define TEST_MODE_COMPLEMENTARY        // Percobaan 4: test complementary
// #define TEST_MODE_KALMAN               // Percobaan 5: test Kalman
// #define TEST_MODE_VOTING               // Percobaan 6: test voting logic

// ============================================================================
// FAULT INJECTION (for Testing Only)
// ============================================================================

// Uncomment to inject specific fault (for voting test)
// #define INJECT_FAULT_SENSOR1_OFFSET 10.0   // Offset sensor 1 by 10mm
// #define INJECT_FAULT_SENSOR2_SPIKE         // Spike at t=5s
// #define INJECT_FAULT_SENSOR3_TIMEOUT       // Freeze sensor 3

// ============================================================================
// CONSTANTS
// ============================================================================

#define PI 3.14159265359

// ============================================================================
// VERSION INFO
// ============================================================================

#define FIRMWARE_VERSION "1.0.0"
#define MODULE_NAME "Modul 4: Sensor Fusion"
#define AUTHOR "[Your Name]"

#endif // CONFIG_H
