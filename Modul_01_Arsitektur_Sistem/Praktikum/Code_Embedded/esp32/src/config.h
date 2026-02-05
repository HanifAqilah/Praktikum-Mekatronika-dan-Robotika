/**
 * @file config.h
 * @brief Configuration file untuk system architecture
 * @date 2026-02-05
 * 
 * File ini berisi semua konfigurasi pin, parameter, dan konstanta
 * UBAH PARAMETER DI SINI untuk customize sistem
 */

#ifndef CONFIG_H
#define CONFIG_H

// ===== PIN CONFIGURATION =====
// Pin assignment untuk motor control
#define MOTOR_LEFT_PWM    25    // PWM output untuk motor kiri
#define MOTOR_LEFT_DIR    26    // Direction control motor kiri
#define MOTOR_RIGHT_PWM   27    // PWM output untuk motor kanan
#define MOTOR_RIGHT_DIR   14    // Direction control motor kanan

// Pin assignment untuk encoder
#define ENCODER_LEFT_A    34    // Encoder kiri channel A (input only pin)
#define ENCODER_LEFT_B    35    // Encoder kiri channel B (input only pin)
#define ENCODER_RIGHT_A   32    // Encoder kanan channel A
#define ENCODER_RIGHT_B   33    // Encoder kanan channel B

// Pin assignment untuk sensor
#define SENSOR_IR_FRONT   36    // IR sensor depan (analog)
#define SENSOR_IR_LEFT    39    // IR sensor kiri (analog)
#define SENSOR_IR_RIGHT   34    // IR sensor kanan (analog)
#define SENSOR_BUMPER     23    // Bumper switch (digital, active LOW)

// Pin assignment untuk safety
#define PIN_ESTOP         22    // Emergency stop button (active LOW)
#define PIN_LED_STATUS    2     // LED status indicator (onboard LED)
#define PIN_BUZZER        19    // Buzzer untuk alarm

// ===== MOTOR PARAMETERS =====
#define PWM_FREQUENCY     5000  // PWM frequency (Hz)
#define PWM_RESOLUTION    8     // PWM resolution (8-bit = 0-255)
#define PWM_CHANNEL_LEFT  0     // PWM channel untuk motor kiri
#define PWM_CHANNEL_RIGHT 1     // PWM channel untuk motor kanan

#define MOTOR_MAX_SPEED   255   // Maximum PWM value
#define MOTOR_MIN_SPEED   50    // Minimum PWM value (deadband)

// ===== ENCODER PARAMETERS =====
#define ENCODER_PPR       360   // Pulses per revolution
#define WHEEL_DIAMETER_MM 65.0  // Diameter roda (mm)
#define WHEEL_BASE_MM     150.0 // Jarak antar roda (mm)

// ===== CONTROL PARAMETERS =====
// PID parameters untuk motor speed control
#define PID_KP            1.0   // Proportional gain
#define PID_KI            0.5   // Integral gain
#define PID_KD            0.1   // Derivative gain

#define CONTROL_LOOP_HZ   50    // Control loop frequency (Hz)
#define CONTROL_PERIOD_MS (1000 / CONTROL_LOOP_HZ)

// ===== COMMUNICATION PARAMETERS =====
#define SERIAL_BAUD_RATE  115200  // Serial monitor baud rate
#define WIFI_SSID         "YourWiFiSSID"  // UBAH dengan WiFi Anda
#define WIFI_PASSWORD     "YourPassword"   // UBAH dengan password WiFi

#define MQTT_SERVER       "192.168.1.100"  // MQTT broker IP
#define MQTT_PORT         1883
#define MQTT_CLIENT_ID    "ESP32_Robot_01"
#define MQTT_TOPIC_CMD    "robot/command"   // Topic untuk terima command
#define MQTT_TOPIC_STATUS "robot/status"    // Topic untuk kirim status
#define MQTT_TOPIC_SENSOR "robot/sensor"    // Topic untuk kirim sensor data

// ===== SAFETY PARAMETERS =====
#define WATCHDOG_TIMEOUT_MS   5000  // Watchdog timeout (ms)
#define COMM_TIMEOUT_MS       1000  // Communication timeout (ms)
#define SENSOR_TIMEOUT_MS     500   // Sensor timeout (ms)

#define OBSTACLE_THRESHOLD    300   // IR sensor threshold (0-4095)
#define EMERGENCY_STOP_DIST   100   // Emergency stop distance (mm)

// ===== SYSTEM STATES =====
enum SystemState {
    STATE_IDLE = 0,
    STATE_READY,
    STATE_RUNNING,
    STATE_FAULT,
    STATE_ESTOP,
    STATE_RECOVERY
};

// ===== DEBUG FLAGS =====
#define DEBUG_SERIAL      true  // Enable serial debug output
#define DEBUG_MOTOR       true  // Enable motor debug output
#define DEBUG_ENCODER     true  // Enable encoder debug output
#define DEBUG_SENSOR      false // Enable sensor debug output (verbose)

// ===== UTILITY MACROS =====
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SIGN(x) ((x) < 0 ? -1 : 1)

#endif // CONFIG_H
