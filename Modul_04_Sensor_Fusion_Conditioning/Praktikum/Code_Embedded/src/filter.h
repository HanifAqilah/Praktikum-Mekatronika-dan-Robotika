/**
 * filter.h - Filter Implementations
 * Modul 4: Sensor Fusion & Conditioning
 * 
 * Contains:
 * - Moving Average Filter (FIR)
 * - Low-Pass Digital Filter (IIR)
 */

#ifndef FILTER_H
#define FILTER_H

#include "config.h"

// ============================================================================
// MOVING AVERAGE FILTER (FIR)
// ============================================================================

class MovingAverageFilter {
private:
  float buffer[20];                        // Buffer for samples (max window=20)
  int window_size;
  int buffer_idx;
  float sum;
  int count;                               // Number of samples collected (until window full)
  
public:
  /**
   * Constructor
   * @param window Window size (3, 5, 10, 20)
   */
  MovingAverageFilter(int window = 5) {
    window_size = (window > 20) ? 20 : window;  // Limit to max 20
    buffer_idx = 0;
    sum = 0;
    count = 0;
    
    for (int i = 0; i < 20; i++) {
      buffer[i] = 0;
    }
  }
  
  /**
   * Update filter with new sample
   * @param new_value New sensor reading
   * @return Filtered value (average of window)
   */
  float update(float new_value) {
    // Remove oldest value from sum
    sum -= buffer[buffer_idx];
    
    // Add new value to buffer and sum
    buffer[buffer_idx] = new_value;
    sum += new_value;
    
    // Move buffer index (circular buffer)
    buffer_idx = (buffer_idx + 1) % window_size;
    
    // Track number of samples collected
    if (count < window_size) {
      count++;
    }
    
    // Return average (avoid divide by zero at start)
    return sum / count;
  }
  
  /**
   * Reset filter state
   */
  void reset() {
    buffer_idx = 0;
    sum = 0;
    count = 0;
    for (int i = 0; i < 20; i++) {
      buffer[i] = 0;
    }
  }
  
  /**
   * Get current window size
   */
  int getWindowSize() {
    return window_size;
  }
  
  /**
   * Get group delay (latency in samples)
   */
  float getDelay() {
    return (window_size - 1) / 2.0;  // samples
  }
};

// ============================================================================
// LOW-PASS DIGITAL FILTER (1st Order IIR)
// ============================================================================

class LowPassFilter {
private:
  float alpha;                             // Filter coefficient (0-1)
  float filtered_value;
  bool initialized;
  
public:
  /**
   * Constructor
   * @param cutoff_freq Cutoff frequency (Hz)
   * @param sample_rate Sampling rate (Hz)
   */
  LowPassFilter(float cutoff_freq, float sample_rate) {
    // Calculate alpha from cutoff frequency
    // Formula: α = 2πfc·dt / (2πfc·dt + 1)
    float dt = 1.0 / sample_rate;
    float rc = 1.0 / (2.0 * PI * cutoff_freq);
    alpha = dt / (rc + dt);
    
    // Alternatively (equivalent formula):
    // alpha = (2 * PI * cutoff_freq * dt) / (2 * PI * cutoff_freq * dt + 1);
    
    filtered_value = 0;
    initialized = false;
  }
  
  /**
   * Update filter with new sample
   * Formula: y[n] = α·x[n] + (1-α)·y[n-1]
   * 
   * @param raw_value New sensor reading
   * @return Filtered value
   */
  float update(float raw_value) {
    if (!initialized) {
      // Initialize dengan first value (avoid startup transient)
      filtered_value = raw_value;
      initialized = true;
    }
    
    // Apply filter formula
    filtered_value = alpha * raw_value + (1.0 - alpha) * filtered_value;
    
    return filtered_value;
  }
  
  /**
   * Reset filter state
   */
  void reset() {
    filtered_value = 0;
    initialized = false;
  }
  
  /**
   * Get alpha coefficient
   */
  float getAlpha() {
    return alpha;
  }
  
  /**
   * Get time constant (tau)
   * τ = (1-α) / (α·fs)
   */
  float getTimeConstant(float sample_rate) {
    return (1.0 - alpha) / (alpha * sample_rate);
  }
  
  /**
   * Set alpha directly (for manual tuning)
   * @param new_alpha Alpha value (0-1)
   */
  void setAlpha(float new_alpha) {
    if (new_alpha >= 0.0 && new_alpha <= 1.0) {
      alpha = new_alpha;
    }
  }
};

// ============================================================================
// MEDIAN FILTER (for Spike Rejection)
// ============================================================================

class MedianFilter {
private:
  float buffer[5];
  int window_size;
  int buffer_idx;
  
  // Bubble sort untuk find median (simple, OK untuk small window)
  float findMedian(float* arr, int size) {
    float temp[5];
    for (int i = 0; i < size; i++) {
      temp[i] = arr[i];
    }
    
    // Sort
    for (int i = 0; i < size - 1; i++) {
      for (int j = 0; j < size - i - 1; j++) {
        if (temp[j] > temp[j + 1]) {
          float swap = temp[j];
          temp[j] = temp[j + 1];
          temp[j + 1] = swap;
        }
      }
    }
    
    // Return median
    return temp[size / 2];
  }
  
public:
  MedianFilter(int window = 3) {
    window_size = (window > 5) ? 5 : window;
    buffer_idx = 0;
    for (int i = 0; i < 5; i++) {
      buffer[i] = 0;
    }
  }
  
  float update(float new_value) {
    buffer[buffer_idx] = new_value;
    buffer_idx = (buffer_idx + 1) % window_size;
    return findMedian(buffer, window_size);
  }
  
  void reset() {
    buffer_idx = 0;
    for (int i = 0; i < 5; i++) {
      buffer[i] = 0;
    }
  }
};

// ============================================================================
// DERIVATIVE FILTER (for PID D-term, used in Modul 6)
// ============================================================================

class DerivativeFilter {
private:
  float prev_value;
  float prev_derivative;
  float alpha;                             // Low-pass filter coefficient
  bool initialized;
  
public:
  /**
   * Constructor
   * @param filter_alpha Alpha for derivative smoothing (0-1)
   */
  DerivativeFilter(float filter_alpha = 0.3) {
    alpha = filter_alpha;
    prev_value = 0;
    prev_derivative = 0;
    initialized = false;
  }
  
  /**
   * Compute filtered derivative
   * @param current_value Current signal value
   * @param dt Time step (seconds)
   * @return Filtered derivative (dValue/dt)
   */
  float update(float current_value, float dt) {
    if (!initialized) {
      prev_value = current_value;
      initialized = true;
      return 0;
    }
    
    // Raw derivative
    float derivative_raw = (current_value - prev_value) / dt;
    
    // Apply low-pass filter to derivative
    prev_derivative = alpha * derivative_raw + (1.0 - alpha) * prev_derivative;
    
    // Update previous value
    prev_value = current_value;
    
    return prev_derivative;
  }
  
  void reset() {
    prev_value = 0;
    prev_derivative = 0;
    initialized = false;
  }
};

// ============================================================================
// UTILITIES
// ============================================================================

/**
 * Calculate RMS (Root Mean Square) from array
 * @param data Data array
 * @param size Array size
 * @return RMS value
 */
float calculateRMS(float* data, int size) {
  float sum_sq = 0;
  for (int i = 0; i < size; i++) {
    sum_sq += data[i] * data[i];
  }
  return sqrt(sum_sq / size);
}

/**
 * Constrain value to range
 * @param value Input value
 * @param min_val Minimum limit
 * @param max_val Maximum limit
 * @return Constrained value
 */
float constrainFloat(float value, float min_val, float max_val) {
  if (value < min_val) return min_val;
  if (value > max_val) return max_val;
  return value;
}

#endif // FILTER_H
