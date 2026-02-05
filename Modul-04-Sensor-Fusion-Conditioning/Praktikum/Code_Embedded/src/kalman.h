/**
 * kalman.h - Kalman Filter Implementation
 * Modul 4: Sensor Fusion & Conditioning
 * 
 * Basic Kalman filter for 1D motion tracking
 * State: [position, velocity]
 */

#ifndef KALMAN_H
#define KALMAN_H

#include "config.h"

// ============================================================================
// KALMAN FILTER 1D (Position + Velocity)
// ============================================================================

class KalmanFilter1D {
private:
  // State estimate: x = [position, velocity]^T
  float x_pos;                             // Position estimate (mm)
  float x_vel;                             // Velocity estimate (mm/s)
  
  // Error covariance matrix P (2×2)
  // P = [ P00  P01 ]
  //     [ P10  P11 ]
  float P[2][2];
  
  // Process noise covariance Q (2×2)
  float Q[2][2];
  
  // Measurement noise covariance R (scalar, position only)
  float R;
  
  // Sampling time
  float dt;
  
  // Kalman gain (for monitoring)
  float K[2];
  
public:
  /**
   * Constructor
   * @param sampling_time Time step (seconds)
   * @param process_noise Process noise covariance (Q)
   * @param measurement_noise Measurement noise covariance (R)
   */
  KalmanFilter1D(float sampling_time, float process_noise, float measurement_noise) {
    dt = sampling_time;
    R = measurement_noise;
    
    // Initialize state estimate to zero
    x_pos = 0;
    x_vel = 0;
    
    // Initialize error covariance P to identity
    P[0][0] = 1.0;  P[0][1] = 0.0;
    P[1][0] = 0.0;  P[1][1] = 1.0;
    
    // Initialize process noise Q
    Q[0][0] = process_noise;
    Q[0][1] = 0.0;
    Q[1][0] = 0.0;
    Q[1][1] = process_noise;
    
    // Initialize Kalman gain
    K[0] = 0;
    K[1] = 0;
  }
  
  /**
   * Prediction step (Time Update)
   * 
   * State transition model (constant velocity):
   * x[k+1] = A · x[k]
   * where A = [ 1   dt ]
   *           [ 0   1  ]
   * 
   * Covariance prediction:
   * P[k+1] = A · P[k] · A^T + Q
   */
  void predict() {
    // State prediction
    float x_pos_pred = x_pos + x_vel * dt;
    float x_vel_pred = x_vel;
    
    // Covariance prediction: P_pred = A · P · A^T + Q
    // Manual matrix multiplication (2×2)
    float P_pred[2][2];
    
    // A · P
    float AP[2][2];
    AP[0][0] = P[0][0] + dt * P[1][0];
    AP[0][1] = P[0][1] + dt * P[1][1];
    AP[1][0] = P[1][0];
    AP[1][1] = P[1][1];
    
    // (A · P) · A^T
    P_pred[0][0] = AP[0][0] + dt * AP[0][1];
    P_pred[0][1] = AP[0][1];
    P_pred[1][0] = AP[1][0] + dt * AP[1][1];
    P_pred[1][1] = AP[1][1];
    
    // Add Q
    P_pred[0][0] += Q[0][0];
    P_pred[0][1] += Q[0][1];
    P_pred[1][0] += Q[1][0];
    P_pred[1][1] += Q[1][1];
    
    // Update state & covariance
    x_pos = x_pos_pred;
    x_vel = x_vel_pred;
    P[0][0] = P_pred[0][0];  P[0][1] = P_pred[0][1];
    P[1][0] = P_pred[1][0];  P[1][1] = P_pred[1][1];
  }
  
  /**
   * Measurement update step (Measurement Update)
   * 
   * Measurement model:
   * z[k] = H · x[k] + v
   * where H = [1, 0] (only position measured)
   * 
   * Kalman gain:
   * K = P · H^T / (H · P · H^T + R)
   * 
   * State update:
   * x = x_pred + K · (z - H · x_pred)
   * 
   * Covariance update:
   * P = (I - K · H) · P
   * 
   * @param measurement Sensor measurement (position in mm)
   */
  void update(float measurement) {
    // Measurement residual (innovation): y = z - H · x_pred
    // H = [1, 0], so H · x = x_pos
    float y = measurement - x_pos;
    
    // Residual covariance: S = H · P · H^T + R
    // H · P · H^T = P[0][0] (since H = [1, 0])
    float S = P[0][0] + R;
    
    // Kalman gain: K = P · H^T / S
    // H^T = [1, 0]^T = [1; 0]
    // P · H^T = [P[0][0]; P[1][0]]
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;
    
    // State update: x = x_pred + K · y
    x_pos = x_pos + K[0] * y;
    x_vel = x_vel + K[1] * y;
    
    // Covariance update: P = (I - K · H) · P
    // (I - K · H) = [ 1-K[0]  0 ]
    //               [ -K[1]   1 ]
    float P_new[2][2];
    P_new[0][0] = (1.0 - K[0]) * P[0][0];
    P_new[0][1] = (1.0 - K[0]) * P[0][1];
    P_new[1][0] = P[1][0] - K[1] * P[0][0];
    P_new[1][1] = P[1][1] - K[1] * P[0][1];
    
    // Update P
    P[0][0] = P_new[0][0];  P[0][1] = P_new[0][1];
    P[1][0] = P_new[1][0];  P[1][1] = P_new[1][1];
  }
  
  /**
   * Get position estimate
   */
  float getPosition() {
    return x_pos;
  }
  
  /**
   * Get velocity estimate
   */
  float getVelocity() {
    return x_vel;
  }
  
  /**
   * Get current Kalman gain (position)
   * Useful for monitoring convergence
   */
  float getKalmanGain() {
    return K[0];
  }
  
  /**
   * Get position error covariance (uncertainty)
   */
  float getPositionCovariance() {
    return P[0][0];
  }
  
  /**
   * Reset filter state
   * @param initial_pos Initial position (mm)
   * @param initial_vel Initial velocity (mm/s)
   */
  void reset(float initial_pos = 0, float initial_vel = 0) {
    x_pos = initial_pos;
    x_vel = initial_vel;
    
    // Reset P to identity
    P[0][0] = 1.0;  P[0][1] = 0.0;
    P[1][0] = 0.0;  P[1][1] = 1.0;
    
    K[0] = 0;
    K[1] = 0;
  }
  
  /**
   * Set process noise Q (for tuning)
   * @param q_value New Q value (diagonal elements)
   */
  void setProcessNoise(float q_value) {
    Q[0][0] = q_value;
    Q[1][1] = q_value;
  }
  
  /**
   * Set measurement noise R (for tuning)
   * @param r_value New R value
   */
  void setMeasurementNoise(float r_value) {
    R = r_value;
  }
};

// ============================================================================
// EXTENDED KALMAN FILTER (BONUS - for Nonlinear System)
// ============================================================================

/**
 * Extended Kalman Filter for 2D motion with nonlinear measurement
 * State: [x, y, vx, vy]
 * Measurement: [range, bearing] (polar coordinate)
 * 
 * NOTE: This is advanced, only implement if doing BONUS project
 */
class ExtendedKalmanFilter2D {
private:
  // State: [x, y, vx, vy]
  float state[4];
  
  // Covariance P (4×4) - simplified storage
  float P[4][4];
  
  // Process noise Q (4×4)
  float Q[4][4];
  
  // Measurement noise R (2×2 for range & bearing)
  float R[2][2];
  
  float dt;
  
public:
  ExtendedKalmanFilter2D(float sampling_time) {
    dt = sampling_time;
    // ... (initialization)
    // TODO: implement for BONUS
  }
  
  void predict() {
    // Linear prediction (same as basic Kalman)
    // TODO: implement
  }
  
  void update(float range, float bearing) {
    // Nonlinear measurement model:
    // range = sqrt(x^2 + y^2)
    // bearing = atan2(y, x)
    
    // Linearize via Jacobian matrix H
    // TODO: implement
  }
  
  float getX() { return state[0]; }
  float getY() { return state[1]; }
};

#endif // KALMAN_H
