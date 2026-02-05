/**
 * @file main.cpp
 * @brief Main program untuk demonstrasi arsitektur sistem
 * @date 2026-02-05
 * 
 * Program ini mendemonstrasikan:
 * - Sensor-Controller-Actuator loop
 * - State machine implementation
 * - Safety system (E-stop, timeout)
 * - Communication protocol (Serial & MQTT)
 */

#include <Arduino.h>
#include "config.h"

// ===== GLOBAL VARIABLES =====
SystemState currentState = STATE_IDLE;
unsigned long lastControlUpdate = 0;
unsigned long lastCommReceived = 0;
bool estopActive = false;

// ===== FUNCTION DECLARATIONS =====
void setupPins();
void setupSerial();
void setupWiFi();
void checkEstop();
void updateStateMachine();
void controlLoop();
void sendTelemetry();

// ===== SETUP FUNCTION =====
void setup() {
    // Initialize serial communication
    setupSerial();
    Serial.println("=== ESP32 System Architecture Demo ===");
    Serial.println("Initializing system...");
    
    // Initialize pins
    setupPins();
    Serial.println("✓ Pins configured");
    
    // Setup WiFi (optional, comment out jika tidak pakai)
    // setupWiFi();
    
    // Initialize state
    currentState = STATE_IDLE;
    Serial.println("✓ System initialized");
    Serial.println("System state: IDLE");
    Serial.println("Press E-Stop button or send command via Serial");
    Serial.println("Commands: START, STOP, STATUS");
}

// ===== MAIN LOOP =====
void loop() {
    unsigned long currentTime = millis();
    
    // 1. Check E-Stop (highest priority)
    checkEstop();
    
    // 2. Update state machine
    updateStateMachine();
    
    // 3. Execute control loop at fixed frequency
    if (currentTime - lastControlUpdate >= CONTROL_PERIOD_MS) {
        lastControlUpdate = currentTime;
        controlLoop();
    }
    
    // 4. Check communication timeout
    if (currentTime - lastCommReceived > COMM_TIMEOUT_MS) {
        if (currentState == STATE_RUNNING) {
            Serial.println("⚠ Communication timeout!");
            currentState = STATE_FAULT;
        }
    }
    
    // 5. Send telemetry (every 1 second)
    static unsigned long lastTelemetry = 0;
    if (currentTime - lastTelemetry >= 1000) {
        lastTelemetry = currentTime;
        sendTelemetry();
    }
    
    // 6. Check for serial commands
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        lastCommReceived = currentTime;
        
        if (cmd == "START") {
            if (currentState == STATE_IDLE || currentState == STATE_READY) {
                currentState = STATE_RUNNING;
                Serial.println("→ System state: RUNNING");
            }
        }
        else if (cmd == "STOP") {
            currentState = STATE_IDLE;
            Serial.println("→ System state: IDLE");
        }
        else if (cmd == "STATUS") {
            Serial.print("Current state: ");
            Serial.println(currentState);
            Serial.print("E-Stop active: ");
            Serial.println(estopActive);
        }
        else {
            Serial.print("Unknown command: ");
            Serial.println(cmd);
        }
    }
}

// ===== SETUP FUNCTIONS =====
void setupPins() {
    // Motor pins as output
    pinMode(MOTOR_LEFT_PWM, OUTPUT);
    pinMode(MOTOR_LEFT_DIR, OUTPUT);
    pinMode(MOTOR_RIGHT_PWM, OUTPUT);
    pinMode(MOTOR_RIGHT_DIR, OUTPUT);
    
    // PWM setup
    ledcSetup(PWM_CHANNEL_LEFT, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL_RIGHT, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(MOTOR_LEFT_PWM, PWM_CHANNEL_LEFT);
    ledcAttachPin(MOTOR_RIGHT_PWM, PWM_CHANNEL_RIGHT);
    
    // Encoder pins as input
    pinMode(ENCODER_LEFT_A, INPUT_PULLUP);
    pinMode(ENCODER_LEFT_B, INPUT_PULLUP);
    pinMode(ENCODER_RIGHT_A, INPUT_PULLUP);
    pinMode(ENCODER_RIGHT_B, INPUT_PULLUP);
    
    // Sensor pins
    pinMode(SENSOR_BUMPER, INPUT_PULLUP);
    
    // Safety pins
    pinMode(PIN_ESTOP, INPUT_PULLUP);
    pinMode(PIN_LED_STATUS, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    
    // Initialize outputs to safe state
    digitalWrite(MOTOR_LEFT_DIR, LOW);
    digitalWrite(MOTOR_RIGHT_DIR, LOW);
    ledcWrite(PWM_CHANNEL_LEFT, 0);
    ledcWrite(PWM_CHANNEL_RIGHT, 0);
    digitalWrite(PIN_LED_STATUS, LOW);
    digitalWrite(PIN_BUZZER, LOW);
}

void setupSerial() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);
}

void setupWiFi() {
    // WiFi setup code here (if needed)
    Serial.println("WiFi setup not implemented in this demo");
}

// ===== SAFETY FUNCTIONS =====
void checkEstop() {
    // Read E-Stop pin (active LOW)
    bool estopPressed = (digitalRead(PIN_ESTOP) == LOW);
    
    if (estopPressed && !estopActive) {
        // E-Stop just pressed
        estopActive = true;
        currentState = STATE_ESTOP;
        
        // Immediate stop
        ledcWrite(PWM_CHANNEL_LEFT, 0);
        ledcWrite(PWM_CHANNEL_RIGHT, 0);
        
        // Alarm
        digitalWrite(PIN_BUZZER, HIGH);
        digitalWrite(PIN_LED_STATUS, HIGH);
        
        Serial.println("🛑 E-STOP ACTIVATED!");
    }
    else if (!estopPressed && estopActive) {
        // E-Stop released
        estopActive = false;
        digitalWrite(PIN_BUZZER, LOW);
        currentState = STATE_IDLE;
        Serial.println("✓ E-Stop released, returning to IDLE");
    }
}

// ===== STATE MACHINE =====
void updateStateMachine() {
    static SystemState previousState = STATE_IDLE;
    
    // Blink LED based on state
    static unsigned long lastBlink = 0;
    unsigned long currentTime = millis();
    
    switch (currentState) {
        case STATE_IDLE:
            // LED off
            digitalWrite(PIN_LED_STATUS, LOW);
            break;
            
        case STATE_READY:
            // LED slow blink (1 Hz)
            if (currentTime - lastBlink >= 500) {
                lastBlink = currentTime;
                digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
            }
            break;
            
        case STATE_RUNNING:
            // LED on solid
            digitalWrite(PIN_LED_STATUS, HIGH);
            break;
            
        case STATE_FAULT:
            // LED fast blink (5 Hz)
            if (currentTime - lastBlink >= 100) {
                lastBlink = currentTime;
                digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
            }
            break;
            
        case STATE_ESTOP:
            // LED on, buzzer on (handled in checkEstop)
            break;
            
        case STATE_RECOVERY:
            // Attempt recovery
            Serial.println("Attempting recovery...");
            delay(1000);
            currentState = STATE_IDLE;
            break;
    }
    
    // Log state change
    if (currentState != previousState) {
        Serial.print("State changed: ");
        Serial.print(previousState);
        Serial.print(" → ");
        Serial.println(currentState);
        previousState = currentState;
    }
}

// ===== CONTROL LOOP =====
void controlLoop() {
    // Only execute control if system is running
    if (currentState != STATE_RUNNING) {
        // Safe stop
        ledcWrite(PWM_CHANNEL_LEFT, 0);
        ledcWrite(PWM_CHANNEL_RIGHT, 0);
        return;
    }
    
    // Read sensors
    int sensorFront = analogRead(SENSOR_IR_FRONT);
    bool bumperHit = (digitalRead(SENSOR_BUMPER) == LOW);
    
    if (DEBUG_SENSOR) {
        Serial.print("Sensor front: ");
        Serial.print(sensorFront);
        Serial.print(" | Bumper: ");
        Serial.println(bumperHit);
    }
    
    // Simple obstacle avoidance logic
    if (bumperHit || sensorFront > OBSTACLE_THRESHOLD) {
        // Obstacle detected, stop
        ledcWrite(PWM_CHANNEL_LEFT, 0);
        ledcWrite(PWM_CHANNEL_RIGHT, 0);
        Serial.println("⚠ Obstacle detected, stopping");
    }
    else {
        // No obstacle, move forward
        digitalWrite(MOTOR_LEFT_DIR, HIGH);   // Forward
        digitalWrite(MOTOR_RIGHT_DIR, HIGH);  // Forward
        ledcWrite(PWM_CHANNEL_LEFT, 150);     // Speed 150/255
        ledcWrite(PWM_CHANNEL_RIGHT, 150);
        
        if (DEBUG_MOTOR) {
            Serial.println("→ Moving forward");
        }
    }
}

// ===== TELEMETRY =====
void sendTelemetry() {
    Serial.println("--- Telemetry ---");
    Serial.print("State: ");
    Serial.println(currentState);
    Serial.print("E-Stop: ");
    Serial.println(estopActive ? "ACTIVE" : "Inactive");
    Serial.print("Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" sec");
    Serial.println("----------------");
}
