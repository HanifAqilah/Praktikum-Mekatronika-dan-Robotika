#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi credentials - CHANGE THESE!
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "192.168.1.100";  // Ubuntu IP
const int mqtt_port = 1883;

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

// State variables
unsigned long seq_num = 0;
unsigned long last_publish = 0;
const unsigned long PUBLISH_INTERVAL = 100;  // 100ms = 10Hz

float motor1_angle = 0.0;
float motor2_angle = 0.0;
float imu_accel_x = 0.0;
float imu_accel_y = 0.0;
float imu_accel_z = 0.0;

// Function prototypes
void setup_wifi();
void reconnect_mqtt();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void publish_sensor_data();
void publish_motor_position(int motor_id, float angle);
void send_ack(int seq, const char* status);
bool move_motor(int motor_id, float target_angle, float speed);
float read_accelerometer_x();
String calculate_crc(const String& message);

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== ESP32 MQTT Bridge Starting ===");
    
    // Setup WiFi
    setup_wifi();
    
    // Setup MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqtt_callback);
    client.setBufferSize(512);  // Increase buffer for larger messages
    
    Serial.println("Setup complete!");
}

void loop() {
    // Maintain MQTT connection
    if (!client.connected()) {
        reconnect_mqtt();
    }
    client.loop();
    
    // Publish data at regular interval
    if (millis() - last_publish >= PUBLISH_INTERVAL) {
        last_publish = millis();
        
        // Publish sensor data
        publish_sensor_data();
        
        // Publish motor positions
        publish_motor_position(1, motor1_angle);
        publish_motor_position(2, motor2_angle);
        
        // Update simulated values
        motor1_angle += 1.0;
        if (motor1_angle >= 360.0) motor1_angle = 0.0;
        
        motor2_angle += 0.5;
        if (motor2_angle >= 360.0) motor2_angle = 0.0;
    }
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Signal strength (RSSI): ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        Serial.println("\nWiFi connection FAILED!");
        Serial.println("Please check SSID and password");
    }
}

void reconnect_mqtt() {
    int retry_delay = 1000;  // Start with 1 second
    
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection to ");
        Serial.print(mqtt_server);
        Serial.print(":");
        Serial.print(mqtt_port);
        Serial.print("...");
        
        // Create unique client ID
        String clientId = "ESP32_Robot_";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            Serial.println("connected!");
            
            // Subscribe to command topics
            client.subscribe("robot/command");
            client.subscribe("robot/motor/+/command");  // Wildcard subscription
            
            Serial.println("Subscribed to command topics");
            
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.print(" | Retry in ");
            Serial.print(retry_delay / 1000);
            Serial.println(" seconds");
            
            delay(retry_delay);
            
            // Exponential backoff (max 30 seconds)
            retry_delay = min(retry_delay * 2, 30000);
        }
    }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] Length: ");
    Serial.println(length);
    
    // Parse JSON payload
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        send_ack(-1, "parse_error");
        return;
    }
    
    // Handle message based on type
    const char* type = doc["type"];
    int seq = doc["seq"] | -1;
    
    if (strcmp(type, "motor_command") == 0) {
        // Motor command
        int motor_id = doc["motor_id"];
        float target_angle = doc["target_angle"];
        float speed = doc["speed"] | 1.0;
        
        Serial.printf("Motor Command - ID:%d, Angle:%.2f, Speed:%.2f\n", 
                      motor_id, target_angle, speed);
        
        // Execute command
        bool success = move_motor(motor_id, target_angle, speed);
        
        // Send ACK
        send_ack(seq, success ? "success" : "motor_error");
        
    } else if (strcmp(type, "ping") == 0) {
        // Ping request - respond immediately
        Serial.println("Ping received");
        send_ack(seq, "pong");
        
    } else if (strcmp(type, "config_update") == 0) {
        // Configuration update
        Serial.println("Config update received");
        // Handle configuration...
        send_ack(seq, "success");
        
    } else {
        Serial.print("Unknown message type: ");
        Serial.println(type);
        send_ack(seq, "unknown_type");
    }
}

void publish_sensor_data() {
    StaticJsonDocument<400> doc;
    
    // Message header
    doc["type"] = "sensor_data";
    doc["sensor_id"] = 1;
    doc["sensor_type"] = "imu";
    doc["timestamp_ms"] = millis();
    
    // Sensor data
    JsonObject data = doc.createNestedObject("data");
    data["accel_x"] = read_accelerometer_x();
    data["accel_y"] = random(-100, 100) / 10.0;
    data["accel_z"] = 9.81 + random(-10, 10) / 10.0;
    data["gyro_x"] = random(-50, 50) / 10.0;
    data["gyro_y"] = random(-50, 50) / 10.0;
    data["gyro_z"] = random(-50, 50) / 10.0;
    
    // Sequence number
    doc["seq"] = seq_num++;
    
    // Serialize (without CRC)
    String message;
    serializeJson(doc, message);
    
    // Calculate and add CRC
    String crc = calculate_crc(message);
    doc["crc"] = crc;
    
    // Serialize final message
    message = "";
    serializeJson(doc, message);
    
    // Publish
    bool success = client.publish("robot/sensor/imu", message.c_str(), false);
    
    if (!success) {
        Serial.println("Failed to publish sensor data!");
    }
}

void publish_motor_position(int motor_id, float angle) {
    String topic = "robot/motor/" + String(motor_id) + "/position";
    
    // Simple format - just angle value
    String message = String(angle, 2);
    
    client.publish(topic.c_str(), message.c_str());
}

void send_ack(int seq, const char* status) {
    StaticJsonDocument<200> doc;
    doc["type"] = "ack";
    doc["seq"] = seq;
    doc["status"] = status;
    doc["timestamp_ms"] = millis();
    
    String message;
    serializeJson(doc, message);
    
    bool success = client.publish("robot/ack", message.c_str(), false);
    
    Serial.print("ACK sent: ");
    Serial.print(message);
    Serial.println(success ? " [OK]" : " [FAIL]");
}

bool move_motor(int motor_id, float target_angle, float speed) {
    // Placeholder - replace with actual motor control
    Serial.printf("→ Moving motor %d to %.2f° at speed %.2f\n", 
                  motor_id, target_angle, speed);
    
    // Simulate motor movement time
    delay(20);
    
    // Update motor angle (simulation)
    if (motor_id == 1) {
        motor1_angle = target_angle;
    } else if (motor_id == 2) {
        motor2_angle = target_angle;
    } else {
        Serial.println("Invalid motor ID!");
        return false;
    }
    
    return true;
}

float read_accelerometer_x() {
    // Placeholder - replace with actual sensor reading
    // Example: MPU6050, ADXL345, etc.
    
    // Simulate accelerometer reading with some noise
    return random(-100, 100) / 10.0;  // Range: -10.0 to 10.0 m/s²
}

String calculate_crc(const String& message) {
    // Simple checksum implementation
    // For production, use CRC32 library
    unsigned long crc = 0;
    for (size_t i = 0; i < message.length(); i++) {
        crc += (unsigned char)message[i];
    }
    
    // Return as hex string
    char crc_str[9];
    sprintf(crc_str, "%08lx", crc);
    return String(crc_str);
}
