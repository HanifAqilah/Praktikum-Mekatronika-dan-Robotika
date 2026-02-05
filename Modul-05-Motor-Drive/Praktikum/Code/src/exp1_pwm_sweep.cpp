/**
 * exp1_pwm_sweep.cpp - PWM Sweep Test
 * Modul 5: Motor & Drive Systems
 * 
 * Sweeps PWM duty cycle from 0% to 100% and back
 * Logs: time, duty_cycle, speed_rpm, current_mA
 */

#include <Arduino.h>
#include "motor_driver.h"

// Pin definitions (adjust for your hardware)
#define MOTOR_PWM_PIN    25
#define MOTOR_IN1_PIN    26
#define MOTOR_IN2_PIN    27
#define MOTOR_ENABLE_PIN 14

#define ENCODER_A_PIN    34
#define ENCODER_B_PIN    35

// Motor & encoder specs
#define ENCODER_PPR      600
#define GEAR_RATIO       1.0

// Timing
#define SAMPLE_PERIOD_MS 100
#define STEP_DELAY_MS    2000

// Global variables
MotorDriver motor(MOTOR_PWM_PIN, MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_ENABLE_PIN);

volatile long encoder_count = 0;
volatile long last_encoder_count = 0;
unsigned long last_sample_time = 0;

float current_rpm = 0.0;
float current_mA = 0.0;  // TODO: Implement current sensing

/**
 * Encoder ISR (Interrupt Service Routine)
 */
void IRAM_ATTR encoderISR() {
    // Simple single-channel counting (for demo)
    // For full quadrature, read both A & B pins
    encoder_count++;
}

/**
 * Calculate RPM from encoder
 */
float calculateRPM() {
    unsigned long current_time = millis();
    float delta_time = (current_time - last_sample_time) / 1000.0;  // seconds
    
    if (delta_time == 0) return 0.0;
    
    long delta_count = encoder_count - last_encoder_count;
    last_encoder_count = encoder_count;
    last_sample_time = current_time;
    
    // RPM = (pulses / PPR / gear_ratio) * (60 / delta_time)
    float revolutions = (float)delta_count / (ENCODER_PPR * 4.0 * GEAR_RATIO);  // *4 for quadrature
    float rpm = (revolutions / delta_time) * 60.0;
    
    return abs(rpm);  // Absolute value for this test
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("========================================");
    Serial.println("Experiment 1: PWM Sweep Test");
    Serial.println("========================================\n");
    
    // Initialize motor
    motor.begin();
    motor.setConfig(DRIVER_L298N, MODE_SIGN_MAGNITUDE, 20000, 0);
    motor.enable();
    
    // Initialize encoder
    pinMode(ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(ENCODER_B_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), encoderISR, RISING);
    
    // CSV header
    Serial.println("time_ms,duty_cycle_pct,pwm_value,speed_rpm,current_mA");
    
    last_sample_time = millis();
}

void loop() {
    static int sweep_direction = 1;  // 1 = up, -1 = down
    static int current_duty = 0;     // 0-100%
    static unsigned long last_step_time = 0;
    
    unsigned long current_time = millis();
    
    // Update duty cycle every STEP_DELAY_MS
    if (current_time - last_step_time >= STEP_DELAY_MS) {
        last_step_time = current_time;
        
        // Increment/decrement duty cycle
        current_duty += (5 * sweep_direction);
        
        // Reverse direction at limits
        if (current_duty >= 100) {
            current_duty = 100;
            sweep_direction = -1;
        } else if (current_duty <= 0) {
            current_duty = 0;
            sweep_direction = 1;
        }
        
        // Convert duty cycle to PWM value (0-255)
        int pwm_value = map(current_duty, 0, 100, 0, 255);
        motor.setSpeed(pwm_value);
        
        Serial.print("Setting duty cycle: ");
        Serial.print(current_duty);
        Serial.println("%");
    }
    
    // Sample data every SAMPLE_PERIOD_MS
    if (current_time - last_sample_time >= SAMPLE_PERIOD_MS) {
        current_rpm = calculateRPM();
        
        // TODO: Read current sensor (INA219)
        // current_mA = ina219.getCurrent_mA();
        
        // Log CSV data
        int pwm_value = map(current_duty, 0, 100, 0, 255);
        Serial.print(current_time);
        Serial.print(",");
        Serial.print(current_duty);
        Serial.print(",");
        Serial.print(pwm_value);
        Serial.print(",");
        Serial.print(current_rpm, 2);
        Serial.print(",");
        Serial.println(current_mA, 2);
    }
}
