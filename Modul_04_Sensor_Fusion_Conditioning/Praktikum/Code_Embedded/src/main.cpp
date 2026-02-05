/**
 * main.cpp - Main Program for Sensor Fusion
 * Modul 4: Sensor Fusion & Conditioning
 * 
 * Implements sensor reading, filtering, fusion, and voting logic
 */

#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "filter.h"
#include "kalman.h"
#include "fusion.h"

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

// Filter objects (instantiate based on FILTER_TYPE)
MovingAverageFilter ma_filter(MA_WINDOW_SIZE);
LowPassFilter lp_filter(LOWPASS_CUTOFF_HZ, SAMPLE_RATE_HZ);
ComplementaryFilter comp_filter(W_ENCODER, W_ACCEL, ENCODER_LOWPASS_CUTOFF, SAMPLE_RATE_HZ);
KalmanFilter1D kalman_filter(SAMPLE_PERIOD_S, KALMAN_Q, KALMAN_R);

// Voting & fault tracking
FaultTracker fault_tracker(FAULT_ALARM_THRESHOLD);

// Timing
unsigned long last_sample_time = 0;
unsigned long start_time = 0;

// Data storage (for output)
float encoder_raw = 0;
float encoder_filtered = 0;
float position_fused = 0;
float voting_result = 0;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  // Initialize serial
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  
  Serial.println("========================================");
  Serial.println(MODULE_NAME);
  Serial.println("Firmware Version: " FIRMWARE_VERSION);
  Serial.println("========================================");
  
  // Initialize sensors
  DEBUG_PRINTLN("Initializing sensors...");
  
  initEncoder();
  DEBUG_PRINTLN("  [OK] Encoder initialized");
  
  if (initIMU()) {
    DEBUG_PRINTLN("  [OK] IMU initialized");
  } else {
    DEBUG_PRINTLN("  [ERROR] IMU initialization failed!");
  }
  
  #if ENABLE_VOTING
    initReferenceSensor();
    DEBUG_PRINTLN("  [OK] Reference sensor initialized");
  #endif
  
  // Initialize LED indicators
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_FAULT, OUTPUT);
  digitalWrite(LED_STATUS, LOW);
  digitalWrite(LED_FAULT, LOW);
  
  // Print CSV header
  #if SERIAL_OUTPUT_CSV
    Serial.println(CSV_HEADER);
  #endif
  
  // Print configuration
  DEBUG_PRINT("Filter type: ");
  #if FILTER_TYPE == FILTER_NONE
    DEBUG_PRINTLN("NONE (raw sensor)");
  #elif FILTER_TYPE == FILTER_MOVING_AVERAGE
    DEBUG_PRINT("MOVING_AVERAGE (window=");
    DEBUG_PRINT(MA_WINDOW_SIZE);
    DEBUG_PRINTLN(")");
  #elif FILTER_TYPE == FILTER_LOWPASS
    DEBUG_PRINT("LOWPASS (cutoff=");
    DEBUG_PRINT(LOWPASS_CUTOFF_HZ);
    DEBUG_PRINTLN(" Hz)");
  #elif FILTER_TYPE == FILTER_COMPLEMENTARY
    DEBUG_PRINT("COMPLEMENTARY (W_enc=");
    DEBUG_PRINT(W_ENCODER);
    DEBUG_PRINT(", W_accel=");
    DEBUG_PRINT(W_ACCEL);
    DEBUG_PRINTLN(")");
  #elif FILTER_TYPE == FILTER_KALMAN
    DEBUG_PRINT("KALMAN (Q=");
    DEBUG_PRINT(KALMAN_Q);
    DEBUG_PRINT(", R=");
    DEBUG_PRINT(KALMAN_R);
    DEBUG_PRINTLN(")");
  #endif
  
  DEBUG_PRINTLN("Starting main loop in 2 seconds...");
  delay(2000);
  
  start_time = millis();
  last_sample_time = millis();
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  unsigned long current_time = millis();
  
  // Sample at fixed rate
  if (current_time - last_sample_time >= SAMPLE_PERIOD_MS) {
    last_sample_time = current_time;
    
    // Blink status LED (heartbeat)
    static bool led_state = false;
    led_state = !led_state;
    digitalWrite(LED_STATUS, led_state);
    
    // ========================================================================
    // STEP 1: READ SENSORS
    // ========================================================================
    
    // Read encoder
    encoder_raw = readEncoderPosition();
    
    // Read IMU (accelerometer)
    float accel_x = readAccelX();          // m/s²
    
    // Read reference sensor (for voting)
    #if ENABLE_VOTING
      float ref_sensor = readReferenceSensor();
    #endif
    
    // Fault injection (for testing)
    #ifdef INJECT_FAULT_SENSOR1_OFFSET
      encoder_raw += INJECT_FAULT_SENSOR1_OFFSET;
    #endif
    
    #ifdef INJECT_FAULT_SENSOR2_SPIKE
      if (current_time >= 5000 && current_time < 5100) {
        accel_x += 50.0;  // Spike at t=5s
      }
    #endif
    
    // ========================================================================
    // STEP 2: APPLY FILTER
    // ========================================================================
    
    #if FILTER_TYPE == FILTER_NONE
      // No filter, use raw sensor
      encoder_filtered = encoder_raw;
      position_fused = encoder_raw;
      
    #elif FILTER_TYPE == FILTER_MOVING_AVERAGE
      // Moving average filter
      encoder_filtered = ma_filter.update(encoder_raw);
      position_fused = encoder_filtered;
      
    #elif FILTER_TYPE == FILTER_LOWPASS
      // Low-pass digital filter
      encoder_filtered = lp_filter.update(encoder_raw);
      position_fused = encoder_filtered;
      
    #elif FILTER_TYPE == FILTER_COMPLEMENTARY
      // Complementary filter (encoder + accel)
      position_fused = comp_filter.update(encoder_raw, accel_x, SAMPLE_PERIOD_S);
      encoder_filtered = encoder_raw;      // For logging
      
    #elif FILTER_TYPE == FILTER_KALMAN
      // Kalman filter
      kalman_filter.predict();
      kalman_filter.update(encoder_raw);
      position_fused = kalman_filter.getPosition();
      encoder_filtered = encoder_raw;      // For logging
      
    #endif
    
    // ========================================================================
    // STEP 3: SENSOR VOTING (if enabled)
    // ========================================================================
    
    #if ENABLE_VOTING
      VotingResult vote = voting_3sensor(encoder_raw, position_fused, ref_sensor, VOTING_TOLERANCE);
      
      if (vote.valid) {
        voting_result = vote.value;
        
        // Check for fault
        if (vote.fault_sensor_id >= 0) {
          DEBUG_PRINT("WARNING: Sensor ");
          DEBUG_PRINT(vote.fault_sensor_id);
          DEBUG_PRINTLN(" fault detected!");
        }
        
        // Update fault tracker
        bool alarm = fault_tracker.update(vote);
        if (alarm) {
          DEBUG_PRINTLN("ALARM: Persistent sensor fault!");
          digitalWrite(LED_FAULT, HIGH);
        } else {
          digitalWrite(LED_FAULT, LOW);
        }
      } else {
        // Critical: no consensus
        DEBUG_PRINTLN("CRITICAL: Multi-sensor fault!");
        digitalWrite(LED_FAULT, HIGH);
        voting_result = 0;
        // TODO: trigger emergency stop atau safe mode
      }
    #else
      voting_result = position_fused;
    #endif
    
    // ========================================================================
    // STEP 4: OUTPUT DATA
    // ========================================================================
    
    #if SERIAL_OUTPUT_CSV
      // CSV format: time, enc_raw, accel_x, enc_filt, pos_fused, voting
      Serial.print(current_time - start_time);
      Serial.print(",");
      Serial.print(encoder_raw, 3);
      Serial.print(",");
      Serial.print(accel_x, 4);
      Serial.print(",");
      Serial.print(encoder_filtered, 3);
      Serial.print(",");
      Serial.print(position_fused, 3);
      Serial.print(",");
      Serial.println(voting_result, 3);
    #else
      // Human-readable format
      Serial.print("Time: ");
      Serial.print(current_time - start_time);
      Serial.print(" ms | Enc: ");
      Serial.print(encoder_raw, 2);
      Serial.print(" | Accel: ");
      Serial.print(accel_x, 3);
      Serial.print(" | Fused: ");
      Serial.print(position_fused, 2);
      Serial.println(" mm");
    #endif
    
    // ========================================================================
    // STEP 5: ADDITIONAL MONITORING (for Kalman filter)
    // ========================================================================
    
    #if FILTER_TYPE == FILTER_KALMAN && DEBUG_MODE
      // Log Kalman gain & velocity estimate (every 1 second)
      static unsigned long last_debug_time = 0;
      if (current_time - last_debug_time >= 1000) {
        last_debug_time = current_time;
        DEBUG_PRINT("Kalman Gain: ");
        DEBUG_PRINT(kalman_filter.getKalmanGain(), 4);
        DEBUG_PRINT(" | Velocity: ");
        DEBUG_PRINT(kalman_filter.getVelocity(), 2);
        DEBUG_PRINTLN(" mm/s");
      }
    #endif
  }
  
  // ========================================================================
  // BACKGROUND TASKS
  // ========================================================================
  
  // Watchdog (optional)
  static unsigned long last_activity = millis();
  if (millis() - last_activity > WATCHDOG_TIMEOUT_MS) {
    DEBUG_PRINTLN("WATCHDOG: No activity timeout!");
    // TODO: reset atau safe mode
    last_activity = millis();
  }
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * Print system info (call from Serial command)
 */
void printSystemInfo() {
  Serial.println("\n========== SYSTEM INFO ==========");
  Serial.print("Firmware: ");
  Serial.println(FIRMWARE_VERSION);
  Serial.print("Sample Rate: ");
  Serial.print(SAMPLE_RATE_HZ);
  Serial.println(" Hz");
  Serial.print("Filter Type: ");
  
  #if FILTER_TYPE == FILTER_MOVING_AVERAGE
    Serial.print("Moving Average (window=");
    Serial.print(MA_WINDOW_SIZE);
    Serial.println(")");
  #elif FILTER_TYPE == FILTER_LOWPASS
    Serial.print("Low-Pass (fc=");
    Serial.print(LOWPASS_CUTOFF_HZ);
    Serial.println(" Hz)");
  #elif FILTER_TYPE == FILTER_COMPLEMENTARY
    Serial.print("Complementary (W_enc=");
    Serial.print(W_ENCODER);
    Serial.print(", W_accel=");
    Serial.print(W_ACCEL);
    Serial.println(")");
  #elif FILTER_TYPE == FILTER_KALMAN
    Serial.print("Kalman (Q=");
    Serial.print(KALMAN_Q);
    Serial.print(", R=");
    Serial.print(KALMAN_R);
    Serial.println(")");
  #else
    Serial.println("None (raw sensor)");
  #endif
  
  Serial.print("Voting: ");
  Serial.println(ENABLE_VOTING ? "Enabled" : "Disabled");
  Serial.println("=================================\n");
}
