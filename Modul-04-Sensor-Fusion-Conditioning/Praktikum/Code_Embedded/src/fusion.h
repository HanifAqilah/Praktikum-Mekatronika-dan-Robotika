/**
 * fusion.h - Sensor Fusion & Voting Logic
 * Modul 4: Sensor Fusion & Conditioning
 * 
 * Contains:
 * - Complementary filter
 * - Sensor voting logic (2-out-of-3)
 */

#ifndef FUSION_H
#define FUSION_H

#include "config.h"
#include "filter.h"

// ============================================================================
// COMPLEMENTARY FILTER
// ============================================================================

class ComplementaryFilter {
private:
  // Low-pass filter for encoder path
  LowPassFilter encoder_lpf;
  
  // Integration state for accelerometer
  float accel_velocity;
  float accel_position;
  
  // Weights
  float w_encoder;
  float w_accel;
  
  // Flags
  bool initialized;
  
public:
  /**
   * Constructor
   * @param weight_encoder Weight for encoder (0-1)
   * @param weight_accel Weight for accelerometer (0-1)
   * @param encoder_cutoff Encoder low-pass cutoff (Hz)
   * @param sample_rate Sampling rate (Hz)
   */
  ComplementaryFilter(float weight_encoder, float weight_accel, 
                      float encoder_cutoff, float sample_rate)
    : encoder_lpf(encoder_cutoff, sample_rate) {
    
    // Normalize weights
    float total = weight_encoder + weight_accel;
    w_encoder = weight_encoder / total;
    w_accel = weight_accel / total;
    
    // Initialize integration state
    accel_velocity = 0;
    accel_position = 0;
    initialized = false;
  }
  
  /**
   * Update complementary filter
   * 
   * @param encoder_pos Encoder position (mm)
   * @param accel Accelerometer reading (m/s²)
   * @param dt Time step (seconds)
   * @return Fused position estimate (mm)
   */
  float update(float encoder_pos, float accel, float dt) {
    // Low-pass filter encoder (stable path)
    float encoder_filtered = encoder_lpf.update(encoder_pos);
    
    // Integrate accelerometer (responsive path)
    // Convert m/s² to mm/s²
    float accel_mm = accel * 1000.0;
    
    // Numerical integration: v = v + a·dt, x = x + v·dt
    accel_velocity += accel_mm * dt;
    accel_position += accel_velocity * dt;
    
    // First time: initialize accel position to encoder
    if (!initialized) {
      accel_position = encoder_pos;
      initialized = true;
    }
    
    // Complementary fusion
    float position_fused = w_encoder * encoder_filtered + w_accel * accel_position;
    
    // Optional: sync accel position to fused (prevent drift)
    // This creates implicit high-pass behavior for accel
    accel_position += 0.01 * (encoder_filtered - accel_position);  // Slow sync
    
    return position_fused;
  }
  
  /**
   * Reset filter state
   */
  void reset() {
    encoder_lpf.reset();
    accel_velocity = 0;
    accel_position = 0;
    initialized = false;
  }
  
  /**
   * Get accel position (for debugging)
   */
  float getAccelPosition() {
    return accel_position;
  }
  
  /**
   * Get accel velocity (for debugging)
   */
  float getAccelVelocity() {
    return accel_velocity;
  }
  
  /**
   * Get weights (for monitoring)
   */
  float getEncoderWeight() { return w_encoder; }
  float getAccelWeight() { return w_accel; }
};

// ============================================================================
// SENSOR VOTING LOGIC
// ============================================================================

struct VotingResult {
  float value;                             // Voted value
  bool valid;                              // Is voting successful?
  int fault_sensor_id;                     // -1: no fault, 0/1/2: faulty sensor, -2: multi-fault
  int num_agreeing;                        // Number of sensors agreeing
};

/**
 * 2-out-of-3 Voting Logic
 * 
 * Algorithm:
 * - Check pairwise difference between sensors
 * - If 2 sensors agree (within tolerance), use their average
 * - Identify outlier sensor (if any)
 * - If all disagree → critical error
 * 
 * @param s1 Sensor 1 reading
 * @param s2 Sensor 2 reading
 * @param s3 Sensor 3 reading
 * @param tolerance Agreement tolerance (same unit as sensor)
 * @return VotingResult struct
 */
VotingResult voting_3sensor(float s1, float s2, float s3, float tolerance) {
  VotingResult result;
  result.valid = false;
  result.fault_sensor_id = -1;
  result.num_agreeing = 0;
  result.value = 0;
  
  // Calculate pairwise differences
  float diff_12 = abs(s1 - s2);
  float diff_13 = abs(s1 - s3);
  float diff_23 = abs(s2 - s3);
  
  // Check agreement patterns
  bool agree_12 = (diff_12 < tolerance);
  bool agree_13 = (diff_13 < tolerance);
  bool agree_23 = (diff_23 < tolerance);
  
  // Case 1: All three agree
  if (agree_12 && agree_13 && agree_23) {
    result.value = (s1 + s2 + s3) / 3.0;
    result.valid = true;
    result.fault_sensor_id = -1;           // No fault
    result.num_agreeing = 3;
  }
  // Case 2: s1 and s2 agree, s3 is outlier
  else if (agree_12 && !agree_13 && !agree_23) {
    result.value = (s1 + s2) / 2.0;
    result.valid = true;
    result.fault_sensor_id = 2;            // s3 fault
    result.num_agreeing = 2;
  }
  // Case 3: s1 and s3 agree, s2 is outlier
  else if (agree_13 && !agree_12 && !agree_23) {
    result.value = (s1 + s3) / 2.0;
    result.valid = true;
    result.fault_sensor_id = 1;            // s2 fault
    result.num_agreeing = 2;
  }
  // Case 4: s2 and s3 agree, s1 is outlier
  else if (agree_23 && !agree_12 && !agree_13) {
    result.value = (s2 + s3) / 2.0;
    result.valid = true;
    result.fault_sensor_id = 0;            // s1 fault
    result.num_agreeing = 2;
  }
  // Case 5: No consensus (all disagree or conflicting agreements)
  else {
    // Critical error: cannot determine valid value
    result.value = 0;
    result.valid = false;
    result.fault_sensor_id = -2;           // Multi-fault or no consensus
    result.num_agreeing = 0;
  }
  
  return result;
}

// ============================================================================
// WEIGHTED VOTING (BONUS)
// ============================================================================

struct SensorWithConfidence {
  float value;
  float confidence;                        // 0.0 - 1.0
};

/**
 * Weighted voting based on sensor confidence
 * Higher confidence sensor has more influence
 * 
 * @param s1 Sensor 1 with confidence
 * @param s2 Sensor 2 with confidence
 * @param s3 Sensor 3 with confidence
 * @return Weighted average value
 */
float weighted_voting(SensorWithConfidence s1, SensorWithConfidence s2, SensorWithConfidence s3) {
  float total_confidence = s1.confidence + s2.confidence + s3.confidence;
  
  if (total_confidence < 0.01) {
    // All sensors have no confidence → return 0 or error
    return 0;
  }
  
  float weighted_value = (s1.value * s1.confidence + 
                          s2.value * s2.confidence + 
                          s3.value * s3.confidence) / total_confidence;
  
  return weighted_value;
}

/**
 * Update sensor confidence based on historical accuracy
 * 
 * @param sensor_value Current sensor reading
 * @param reference_value Ground truth or voted value
 * @param current_confidence Current confidence (0-1)
 * @param adaptation_rate How fast confidence adapts (0-1, typical 0.1)
 * @return Updated confidence
 */
float update_confidence(float sensor_value, float reference_value, 
                        float current_confidence, float adaptation_rate = 0.1) {
  
  // Calculate error
  float error = abs(sensor_value - reference_value);
  
  // Convert error to confidence (larger error → lower confidence)
  // Example: error=0 → conf=1.0, error=10 → conf=0.5, error=50 → conf=0.1
  float error_confidence = 1.0 / (1.0 + error / 10.0);
  
  // Exponential moving average of confidence
  float new_confidence = adaptation_rate * error_confidence + 
                         (1.0 - adaptation_rate) * current_confidence;
  
  // Clamp to [0.1, 1.0] (minimum confidence 0.1 to avoid complete distrust)
  if (new_confidence < 0.1) new_confidence = 0.1;
  if (new_confidence > 1.0) new_confidence = 1.0;
  
  return new_confidence;
}

// ============================================================================
// FAULT DETECTION TRACKER
// ============================================================================

class FaultTracker {
private:
  int fault_count[3];                      // Consecutive fault count for each sensor
  int fault_threshold;
  unsigned long last_fault_time[3];
  
public:
  FaultTracker(int threshold = FAULT_ALARM_THRESHOLD) {
    fault_threshold = threshold;
    for (int i = 0; i < 3; i++) {
      fault_count[i] = 0;
      last_fault_time[i] = 0;
    }
  }
  
  /**
   * Update fault tracking based on voting result
   * @param voting_result Result from voting_3sensor()
   * @return true if alarm should be triggered
   */
  bool update(VotingResult voting_result) {
    unsigned long now = millis();
    
    if (voting_result.fault_sensor_id >= 0 && voting_result.fault_sensor_id < 3) {
      // Specific sensor fault detected
      int fault_id = voting_result.fault_sensor_id;
      fault_count[fault_id]++;
      last_fault_time[fault_id] = now;
      
      // Reset other sensors (they're OK)
      for (int i = 0; i < 3; i++) {
        if (i != fault_id) {
          fault_count[i] = 0;
        }
      }
      
      // Check if alarm threshold reached
      if (fault_count[fault_id] >= fault_threshold) {
        return true;  // Trigger alarm
      }
    }
    else if (voting_result.fault_sensor_id == -2) {
      // Multi-fault (critical)
      return true;  // Immediate alarm
    }
    else {
      // No fault detected, decay fault counts
      for (int i = 0; i < 3; i++) {
        if (fault_count[i] > 0 && (now - last_fault_time[i] > 1000)) {
          fault_count[i] = 0;  // Reset after 1 second of no fault
        }
      }
    }
    
    return false;  // No alarm
  }
  
  /**
   * Get fault count for specific sensor
   */
  int getFaultCount(int sensor_id) {
    if (sensor_id >= 0 && sensor_id < 3) {
      return fault_count[sensor_id];
    }
    return 0;
  }
  
  /**
   * Reset all fault counts
   */
  void reset() {
    for (int i = 0; i < 3; i++) {
      fault_count[i] = 0;
      last_fault_time[i] = 0;
    }
  }
};

#endif // FUSION_H
