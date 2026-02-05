/**
 * motor_driver.h - Motor Driver Control Library
 * Modul 5: Motor & Drive Systems
 * 
 * Supports: L298N, DRV8833, TB6612FNG
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

// Motor driver types
enum DriverType {
    DRIVER_L298N,
    DRIVER_DRV8833,
    DRIVER_TB6612FNG
};

// Motor control modes
enum ControlMode {
    MODE_SIGN_MAGNITUDE,  // Separate PWM & direction pins
    MODE_LOCKED_ANTIPHASE // PWM on both pins (inverted)
};

class MotorDriver {
private:
    // Pin assignments
    uint8_t pwm_pin;
    uint8_t in1_pin;
    uint8_t in2_pin;
    uint8_t enable_pin;
    
    // Configuration
    DriverType driver_type;
    ControlMode control_mode;
    uint8_t pwm_channel;      // ESP32 LEDC channel
    uint32_t pwm_frequency;
    uint8_t pwm_resolution;
    
    // State
    int16_t current_pwm;      // -255 to +255
    bool enabled;
    
    // Dead-time
    uint16_t dead_time_us;
    
    // Slew rate limiting
    bool slew_rate_enabled;
    uint8_t slew_rate_limit;  // Max PWM change per update
    
public:
    /**
     * Constructor
     */
    MotorDriver(uint8_t pwm, uint8_t in1, uint8_t in2, uint8_t enable = 255) {
        pwm_pin = pwm;
        in1_pin = in1;
        in2_pin = in2;
        enable_pin = enable;
        
        current_pwm = 0;
        enabled = false;
        dead_time_us = 2;  // Default 2us
        slew_rate_enabled = false;
        slew_rate_limit = 10;
        
        driver_type = DRIVER_L298N;
        control_mode = MODE_SIGN_MAGNITUDE;
        pwm_frequency = 20000;  // 20 kHz
        pwm_resolution = 8;     // 8-bit
    }
    
    /**
     * Initialize motor driver
     */
    void begin() {
        // Set pin modes
        pinMode(in1_pin, OUTPUT);
        pinMode(in2_pin, OUTPUT);
        if (enable_pin != 255) {
            pinMode(enable_pin, OUTPUT);
        }
        
        #ifdef ESP32
            // ESP32: Use LEDC for PWM
            ledcSetup(pwm_channel, pwm_frequency, pwm_resolution);
            ledcAttachPin(pwm_pin, pwm_channel);
        #else
            // Arduino: Standard analogWrite
            pinMode(pwm_pin, OUTPUT);
        #endif
        
        // Initial state: stopped
        stop();
        enable();
    }
    
    /**
     * Enable motor driver
     */
    void enable() {
        if (enable_pin != 255) {
            digitalWrite(enable_pin, HIGH);
        }
        enabled = true;
    }
    
    /**
     * Disable motor driver (coast)
     */
    void disable() {
        if (enable_pin != 255) {
            digitalWrite(enable_pin, LOW);
        }
        enabled = false;
    }
    
    /**
     * Set motor speed and direction
     * @param speed: -255 (full reverse) to +255 (full forward)
     */
    void setSpeed(int16_t speed) {
        // Clamp to valid range
        speed = constrain(speed, -255, 255);
        
        // Apply slew rate limiting
        if (slew_rate_enabled) {
            int16_t delta = speed - current_pwm;
            if (abs(delta) > slew_rate_limit) {
                speed = current_pwm + (delta > 0 ? slew_rate_limit : -slew_rate_limit);
            }
        }
        
        current_pwm = speed;
        
        if (!enabled) return;
        
        // Apply control based on mode
        if (control_mode == MODE_SIGN_MAGNITUDE) {
            applySignMagnitude(speed);
        } else {
            applyLockedAntiPhase(speed);
        }
    }
    
    /**
     * Stop motor with brake (short motor terminals)
     */
    void brake() {
        digitalWrite(in1_pin, HIGH);
        digitalWrite(in2_pin, HIGH);
        
        #ifdef ESP32
            ledcWrite(pwm_channel, 0);
        #else
            analogWrite(pwm_pin, 0);
        #endif
        
        current_pwm = 0;
    }
    
    /**
     * Stop motor with coast (open motor terminals)
     */
    void coast() {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, LOW);
        
        #ifdef ESP32
            ledcWrite(pwm_channel, 0);
        #else
            analogWrite(pwm_pin, 0);
        #endif
        
        current_pwm = 0;
    }
    
    /**
     * Alias for brake
     */
    void stop() {
        brake();
    }
    
    /**
     * Get current PWM value
     */
    int16_t getSpeed() {
        return current_pwm;
    }
    
    /**
     * Enable/disable slew rate limiting
     */
    void enableSlewRate(bool enable, uint8_t rate = 10) {
        slew_rate_enabled = enable;
        slew_rate_limit = rate;
    }
    
    /**
     * Set dead-time (microseconds)
     */
    void setDeadTime(uint16_t us) {
        dead_time_us = us;
    }
    
    /**
     * Configure driver settings
     */
    void setConfig(DriverType type, ControlMode mode, uint32_t freq = 20000, uint8_t channel = 0) {
        driver_type = type;
        control_mode = mode;
        pwm_frequency = freq;
        pwm_channel = channel;
    }
    
private:
    /**
     * Sign-magnitude PWM mode
     * Separate PWM and direction control
     */
    void applySignMagnitude(int16_t speed) {
        uint8_t pwm_value = abs(speed);
        
        if (speed > 0) {
            // Forward
            digitalWrite(in2_pin, LOW);
            delayMicroseconds(dead_time_us);
            digitalWrite(in1_pin, HIGH);
        } else if (speed < 0) {
            // Reverse
            digitalWrite(in1_pin, LOW);
            delayMicroseconds(dead_time_us);
            digitalWrite(in2_pin, HIGH);
        } else {
            // Stop
            digitalWrite(in1_pin, LOW);
            digitalWrite(in2_pin, LOW);
        }
        
        #ifdef ESP32
            ledcWrite(pwm_channel, pwm_value);
        #else
            analogWrite(pwm_pin, pwm_value);
        #endif
    }
    
    /**
     * Locked anti-phase PWM mode
     * PWM on both pins (inverted)
     */
    void applyLockedAntiPhase(int16_t speed) {
        // Map -255..255 to 0..255 PWM range
        // 0 = full reverse, 128 = stop, 255 = full forward
        uint8_t pwm_value = (speed + 255) / 2;
        
        #ifdef ESP32
            ledcWrite(pwm_channel, pwm_value);
        #else
            analogWrite(pwm_pin, pwm_value);
        #endif
        
        // IN1 and IN2 are inverted
        digitalWrite(in1_pin, speed >= 0 ? HIGH : LOW);
        digitalWrite(in2_pin, speed >= 0 ? LOW : HIGH);
    }
};

#endif // MOTOR_DRIVER_H
