# JOBSHEET MODUL 11
## ESP32 ↔ ROS2 BRIDGE - COMMUNICATION RELIABILITY

---

## 📋 INFORMASI PRAKTIKUM

**Modul**: 11  
**Judul**: ESP32 ↔ ROS2 Bridge - Communication Reliability  
**Durasi**: 3-4 jam  
**Tingkat Kesulitan**: ⭐⭐⭐⭐ (Advanced)

---

## 🎯 TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:
1. Memilih protokol komunikasi yang sesuai dengan requirements
2. Merancang format pesan (JSON) dengan CRC validation
3. Mengimplementasikan MQTT publisher di ESP32 firmware
4. Mengimplementasikan MQTT-ROS2 bridge di Ubuntu
5. Mengimplementasikan acknowledge & retry mechanism
6. Mengukur dan menganalisis latency komunikasi
7. Mengimplementasikan multi-device communication
8. Menangani error dan connection recovery

---

## 🛠️ ALAT & BAHAN

### Hardware:
1. ESP32 development board dengan firmware (dari Batch 2)
2. Komputer Ubuntu 20.04/22.04 dengan ROS2
3. USB cable untuk serial connection (optional)
4. WiFi router / network connection

### Software:
1. Arduino IDE / PlatformIO (untuk firmware)
2. ROS2 Humble/Iron
3. Python 3.10+
4. MQTT Broker (mosquitto)
5. MQTT Client libraries:
   - PubSubClient (Arduino)
   - Paho MQTT (Python)
   - ArduinoJson (Arduino)

### Installation:
```bash
# Install MQTT broker
sudo apt update
sudo apt install mosquitto mosquitto-clients

# Start broker
sudo systemctl start mosquitto
sudo systemctl enable mosquitto

# Install Python MQTT client
pip install paho-mqtt

# Install Arduino libraries (via Library Manager)
# - PubSubClient by Nick O'Leary
# - ArduinoJson by Benoit Blanchon
```

---

## 📝 LANGKAH KERJA

## PERCOBAAN 1: PROTOCOL SELECTION & MESSAGE DESIGN

### Tujuan:
Membandingkan 3 protokol komunikasi dan memilih yang paling sesuai untuk aplikasi robot.

### Langkah:

#### 1.1 Analisis Requirements
Tentukan requirements komunikasi untuk robot Anda:
- **Latency requirement**: < 100ms (untuk real-time control)
- **Bandwidth**: 10+ messages/second per device
- **Number of devices**: 2 motor + 1 sensor = 3 devices
- **Distance**: ESP32 dan Ubuntu dalam network yang sama
- **Reliability**: Command harus reliable (99%+), sensor data best-effort ok

#### 1.2 Protocol Comparison

Buat tabel perbandingan:

| Kriteria | MQTT | Micro-ROS | Serial | Weight |
|----------|------|-----------|--------|--------|
| **Latency** | 25ms | 20ms | 8ms | 40% |
| **Setup Complexity** | Medium (3/5) | High (5/5) | Low (1/5) | 20% |
| **Scalability** | Excellent (5/5) | Good (4/5) | Poor (1/5) | 20% |
| **Resource Usage** | Low | High | Very Low | 10% |
| **Learning Curve** | Easy | Steep | Very Easy | 10% |
| **Weighted Score** | **3.85** | 3.50 | 2.42 | - |

**Kesimpulan**: Pilih **MQTT** karena:
- Latency acceptable untuk robot control (< 100ms)
- Setup relatif simple
- Excellent scalability untuk future expansion
- Low resource usage di ESP32

#### 1.3 Message Format Design

Definisikan 3 message types:

**Type 1: Sensor Data**
```json
{
  "type": "sensor_data",
  "sensor_id": 1,
  "sensor_type": "imu",
  "timestamp_ms": 1234567890,
  "data": {
    "accel_x": 9.81,
    "accel_y": 0.0,
    "accel_z": 0.0
  },
  "seq": 42,
  "crc": "a1b2c3d4"
}
```

**Type 2: Motor Command**
```json
{
  "type": "motor_command",
  "motor_id": 1,
  "target_angle": 90.0,
  "speed": 1.0,
  "seq": 15,
  "timestamp_ms": 1234567900
}
```

**Type 3: Acknowledgment**
```json
{
  "type": "ack",
  "seq": 15,
  "status": "success",
  "error_code": 0,
  "timestamp_ms": 1234567905
}
```

### Deliverable Percobaan 1:
- ✅ Protocol comparison table dengan weighted scoring
- ✅ Protocol selection justification (1-2 paragraf)
- ✅ Message format specification untuk 3 message types
- ✅ Field description document

---

## PERCOBAAN 2: MQTT BROKER SETUP & FIRMWARE PUBLISH

### Tujuan:
Install MQTT broker dan implement sensor data publisher di ESP32.

### Langkah:

#### 2.1 MQTT Broker Setup

```bash
# Install mosquitto
sudo apt install mosquitto mosquitto-clients

# Start broker
sudo systemctl start mosquitto

# Verify broker running
sudo systemctl status mosquitto

# Test broker dengan mosquitto_sub
mosquitto_sub -h localhost -v -t '#'
# Biarkan terminal ini terbuka untuk monitoring
```

#### 2.2 Configure WiFi Credentials

Create file `secrets.h` di firmware project:
```cpp
// secrets.h
#ifndef SECRETS_H
#define SECRETS_H

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "192.168.1.100";  // Ubuntu IP address
const int mqtt_port = 1883;

#endif
```

⚠️ **Important**: Tambahkan `secrets.h` ke `.gitignore`!

#### 2.3 ESP32 Firmware - MQTT Publisher

File: `mqtt_publisher.ino`

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

// MQTT Configuration
WiFiClient espClient;
PubSubClient client(espClient);

// Global variables
unsigned long seq_num = 0;
unsigned long last_publish = 0;
const unsigned long PUBLISH_INTERVAL = 100;  // 100ms = 10Hz

// Function prototypes
void setup_wifi();
void reconnect();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
float read_accelerometer();
String calculate_crc(String message);

void setup() {
    Serial.begin(115200);
    setup_wifi();
    
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqtt_callback);
    
    Serial.println("MQTT Publisher Setup Complete");
}

void loop() {
    // Ensure MQTT connection
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    
    // Publish sensor data at 10Hz
    if (millis() - last_publish >= PUBLISH_INTERVAL) {
        last_publish = millis();
        
        // Read sensor
        float accel_x = read_accelerometer();
        
        // Create JSON message
        StaticJsonDocument<300> doc;
        doc["type"] = "sensor_data";
        doc["sensor_id"] = 1;
        doc["sensor_type"] = "imu";
        doc["timestamp_ms"] = millis();
        doc["data"]["accel_x"] = accel_x;
        doc["data"]["accel_y"] = 0.0;
        doc["data"]["accel_z"] = 0.0;
        doc["seq"] = seq_num++;
        
        // Serialize to string (without CRC first)
        String message;
        serializeJson(doc, message);
        
        // Calculate and add CRC
        String crc = calculate_crc(message);
        doc["crc"] = crc;
        
        // Serialize final message
        message = "";
        serializeJson(doc, message);
        
        // Publish to MQTT
        bool success = client.publish("robot/sensor/imu", message.c_str());
        
        if (success) {
            Serial.print("Published: ");
            Serial.println(message);
        } else {
            Serial.println("Publish failed!");
        }
    }
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        
        // Create unique client ID
        String clientId = "ESP32_";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            // Subscribe to command topic
            client.subscribe("robot/command");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retry in 5 seconds");
            delay(5000);
        }
    }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    
    // Parse JSON payload
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }
    
    // Handle different message types
    const char* type = doc["type"];
    if (strcmp(type, "motor_command") == 0) {
        // Will implement in Percobaan 4
        Serial.println("Motor command received");
    }
}

float read_accelerometer() {
    // Placeholder - replace with actual sensor reading
    // Example: MPU6050, ADXL345, etc.
    return random(-100, 100) / 10.0;  // Simulate -10.0 to 10.0
}

String calculate_crc(String message) {
    // Simple CRC placeholder - use real CRC32 library for production
    unsigned long crc = 0;
    for (int i = 0; i < message.length(); i++) {
        crc += message[i];
    }
    return String(crc, HEX);
}
```

#### 2.4 Test MQTT Publishing

Di terminal Ubuntu:
```bash
# Subscribe to sensor topic
mosquitto_sub -h localhost -t "robot/sensor/imu"

# Expected output (every 100ms):
# {"type":"sensor_data","sensor_id":1,"sensor_type":"imu",...}
```

### Deliverable Percobaan 2:
- ✅ MQTT broker running
- ✅ ESP32 firmware compiled & uploaded
- ✅ WiFi connection established
- ✅ Sensor data visible di mosquitto_sub (10 messages/second)
- ✅ Screenshot mosquitto_sub output

---

## PERCOBAAN 3: ROS2 SUBSCRIBER & BRIDGE NODE

### Tujuan:
Create ROS2 node yang subscribe dari MQTT dan convert ke ROS2 message.

### Langkah:

#### 3.1 Create ROS2 Package

```bash
# Navigate to ROS2 workspace
cd ~/ros2_ws/src

# Create package
ros2 pkg create --build-type ament_python robot_bridge

# Create node file
cd robot_bridge/robot_bridge
```

#### 3.2 MQTT Bridge Node

File: `~/ros2_ws/src/robot_bridge/robot_bridge/mqtt_bridge.py`

```python
#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import paho.mqtt.client as mqtt
import json
from sensor_msgs.msg import Imu
from std_msgs.msg import Header

class MqttBridge(Node):
    def __init__(self):
        super().__init__('mqtt_bridge')
        
        # ROS2 Publishers
        self.imu_publisher = self.create_publisher(Imu, '/robot/sensor/imu', 10)
        
        # MQTT Setup
        self.mqtt_client = mqtt.Client()
        self.mqtt_client.on_connect = self.on_mqtt_connect
        self.mqtt_client.on_message = self.on_mqtt_message
        self.mqtt_client.on_disconnect = self.on_mqtt_disconnect
        
        # Connect to broker
        mqtt_host = self.declare_parameter('mqtt_host', 'localhost').value
        mqtt_port = self.declare_parameter('mqtt_port', 1883).value
        
        try:
            self.mqtt_client.connect(mqtt_host, mqtt_port, 60)
            self.mqtt_client.loop_start()
            self.get_logger().info(f"Connecting to MQTT broker at {mqtt_host}:{mqtt_port}")
        except Exception as e:
            self.get_logger().error(f"Failed to connect to MQTT broker: {e}")
    
    def on_mqtt_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.get_logger().info("MQTT connected successfully")
            # Subscribe to topics
            client.subscribe("robot/sensor/imu")
            client.subscribe("robot/motor/+/position")  # Wildcard for all motors
            self.get_logger().info("Subscribed to topics")
        else:
            self.get_logger().error(f"MQTT connection failed with code {rc}")
    
    def on_mqtt_disconnect(self, client, userdata, rc):
        if rc != 0:
            self.get_logger().warn(f"MQTT disconnected unexpectedly: {rc}")
    
    def on_mqtt_message(self, client, userdata, msg):
        try:
            # Parse JSON payload
            payload = json.loads(msg.payload.decode())
            
            # Verify CRC if present
            if 'crc' in payload:
                if not self.verify_crc(payload):
                    self.get_logger().warn(f"CRC mismatch for message: {payload.get('seq', 'unknown')}")
                    return
            
            # Route message based on type
            msg_type = payload.get('type')
            
            if msg_type == 'sensor_data':
                self.handle_sensor_data(payload)
            else:
                self.get_logger().debug(f"Unknown message type: {msg_type}")
        
        except json.JSONDecodeError as e:
            self.get_logger().error(f"JSON decode error: {e}")
        except Exception as e:
            self.get_logger().error(f"Error processing message: {e}")
    
    def handle_sensor_data(self, payload):
        """Convert MQTT sensor data to ROS2 IMU message"""
        sensor_type = payload.get('sensor_type')
        
        if sensor_type == 'imu':
            # Create ROS2 IMU message
            imu_msg = Imu()
            imu_msg.header = Header()
            imu_msg.header.stamp = self.get_clock().now().to_msg()
            imu_msg.header.frame_id = 'imu_link'
            
            # Fill acceleration data
            data = payload.get('data', {})
            imu_msg.linear_acceleration.x = data.get('accel_x', 0.0)
            imu_msg.linear_acceleration.y = data.get('accel_y', 0.0)
            imu_msg.linear_acceleration.z = data.get('accel_z', 0.0)
            
            # Publish to ROS2
            self.imu_publisher.publish(imu_msg)
            self.get_logger().debug(f"Published IMU: seq={payload.get('seq', 0)}")
    
    def verify_crc(self, payload):
        """Verify CRC checksum"""
        received_crc = payload.pop('crc')
        # Simple CRC verification (match with firmware implementation)
        message_str = json.dumps(payload, sort_keys=True)
        calculated_crc = str(sum(ord(c) for c in message_str))
        payload['crc'] = received_crc  # Restore CRC
        return received_crc == calculated_crc
    
    def destroy_node(self):
        self.mqtt_client.loop_stop()
        self.mqtt_client.disconnect()
        super().destroy_node()

def main(args=None):
    rclpy.init(args=args)
    node = MqttBridge()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
```

#### 3.3 Update setup.py

File: `~/ros2_ws/src/robot_bridge/setup.py`

```python
from setuptools import setup

package_name = 'robot_bridge'

setup(
    name=package_name,
    version='0.1.0',
    packages=[package_name],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Your Name',
    maintainer_email='your.email@example.com',
    description='MQTT to ROS2 Bridge',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'mqtt_bridge = robot_bridge.mqtt_bridge:main',
        ],
    },
)
```

#### 3.4 Build & Run

```bash
# Build package
cd ~/ros2_ws
colcon build --packages-select robot_bridge

# Source workspace
source install/setup.bash

# Run bridge node
ros2 run robot_bridge mqtt_bridge
```

#### 3.5 Verify ROS2 Topic

Di terminal baru:
```bash
# List topics
ros2 topic list

# Echo IMU data
ros2 topic echo /robot/sensor/imu

# Check publish rate
ros2 topic hz /robot/sensor/imu
# Expected: ~10 Hz
```

### Deliverable Percobaan 3:
- ✅ ROS2 package created & compiled
- ✅ Bridge node running tanpa error
- ✅ ROS2 topic `/robot/sensor/imu` receiving data
- ✅ Publish rate ~10 Hz verified
- ✅ Screenshot `ros2 topic echo` output

---

## PERCOBAAN 4: ACKNOWLEDGE & RETRY MECHANISM

### Tujuan:
Implement bidirectional communication dengan acknowledge dan retry untuk reliable command delivery.

### Langkah:

#### 4.1 Firmware - ACK Handler

Tambahkan ke `mqtt_publisher.ino`:

```cpp
// Add after existing code

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    
    // Parse JSON
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }
    
    const char* type = doc["type"];
    
    if (strcmp(type, "motor_command") == 0) {
        int motor_id = doc["motor_id"];
        float target_angle = doc["target_angle"];
        float speed = doc["speed"];
        int seq = doc["seq"];
        
        Serial.printf("Motor command: ID=%d, Angle=%.2f, Speed=%.2f\n", 
                      motor_id, target_angle, speed);
        
        // Execute motor command (placeholder)
        bool success = move_motor(motor_id, target_angle, speed);
        
        // Send ACK
        send_ack(seq, success ? "success" : "failed");
    }
    else if (strcmp(type, "ping") == 0) {
        // Respond to ping immediately
        int seq = doc["seq"];
        send_ack(seq, "pong");
    }
}

bool move_motor(int motor_id, float target_angle, float speed) {
    // Placeholder - implement actual motor control
    Serial.printf("Moving motor %d to %.2f deg at speed %.2f\n", 
                  motor_id, target_angle, speed);
    delay(10);  // Simulate processing
    return true;
}

void send_ack(int seq, const char* status) {
    StaticJsonDocument<200> ack;
    ack["type"] = "ack";
    ack["seq"] = seq;
    ack["status"] = status;
    ack["timestamp_ms"] = millis();
    
    String ack_msg;
    serializeJson(ack, ack_msg);
    
    client.publish("robot/ack", ack_msg.c_str());
    Serial.print("Sent ACK: ");
    Serial.println(ack_msg);
}
```

#### 4.2 ROS2 Bridge - Command with Retry

Tambahkan ke `mqtt_bridge.py`:

```python
import time
import threading

class MqttBridge(Node):
    def __init__(self):
        # ... existing code ...
        
        # ACK tracking
        self.ack_received = set()
        self.ack_lock = threading.Lock()
        self.seq_counter = 0
        
        # Subscribe to ACK topic
        # (Add to on_mqtt_connect callback)
    
    def on_mqtt_connect(self, client, userdata, flags, rc):
        # ... existing code ...
        client.subscribe("robot/ack")
    
    def on_mqtt_message(self, client, userdata, msg):
        # ... existing code ...
        
        if msg_type == 'ack':
            self.handle_ack(payload)
    
    def handle_ack(self, payload):
        """Handle acknowledgment message"""
        seq = payload.get('seq')
        status = payload.get('status')
        
        with self.ack_lock:
            self.ack_received.add(seq)
        
        self.get_logger().info(f"ACK received: seq={seq}, status={status}")
    
    def send_command_with_retry(self, command_dict, max_retry=3, timeout=1.0):
        """Send command and wait for ACK with retry"""
        for attempt in range(max_retry):
            # Assign sequence number
            seq = self.seq_counter
            self.seq_counter += 1
            command_dict['seq'] = seq
            
            # Send command
            message = json.dumps(command_dict)
            self.mqtt_client.publish("robot/command", message)
            self.get_logger().info(f"Sent command: seq={seq}, attempt={attempt+1}")
            
            # Wait for ACK
            start_time = time.time()
            while time.time() - start_time < timeout:
                with self.ack_lock:
                    if seq in self.ack_received:
                        self.get_logger().info(f"Command {seq} acknowledged")
                        self.ack_received.remove(seq)  # Cleanup
                        return True
                time.sleep(0.01)  # 10ms polling
            
            # Timeout
            self.get_logger().warn(f"Command {seq} timeout, retry {attempt+1}/{max_retry}")
        
        self.get_logger().error(f"Command failed after {max_retry} retries")
        return False
```

#### 4.3 Test ACK Mechanism

Create test script: `test_ack.py`

```python
#!/usr/bin/env python3

import rclpy
from robot_bridge.mqtt_bridge import MqttBridge
import time

def main():
    rclpy.init()
    node = MqttBridge()
    
    # Wait for MQTT connection
    time.sleep(2)
    
    # Test 1: Send motor command
    print("\n=== Test 1: Motor Command with ACK ===")
    command = {
        'type': 'motor_command',
        'motor_id': 1,
        'target_angle': 90.0,
        'speed': 1.0
    }
    success = node.send_command_with_retry(command)
    print(f"Result: {'✓ SUCCESS' if success else '✗ FAILED'}")
    
    # Test 2: Multiple commands
    print("\n=== Test 2: Multiple Commands ===")
    for i in range(5):
        command = {
            'type': 'motor_command',
            'motor_id': 1,
            'target_angle': 45.0 * i,
            'speed': 0.5
        }
        success = node.send_command_with_retry(command)
        print(f"Command {i+1}: {'✓' if success else '✗'}")
        time.sleep(0.5)
    
    # Cleanup
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

Run test:
```bash
python3 test_ack.py
```

### Deliverable Percobaan 4:
- ✅ Firmware ACK handler implemented
- ✅ ROS2 retry mechanism implemented
- ✅ Test script successful (5/5 commands acknowledged)
- ✅ Log output showing ACK received
- ✅ Timeout test (simulate no ACK - disconnect ESP32)

---

## PERCOBAAN 5: LATENCY MEASUREMENT

### Tujuan:
Measure round-trip latency dan analyze performance.

### Langkah:

#### 5.1 Latency Measurement Tool

File: `latency_test.py`

```python
#!/usr/bin/env python3

import rclpy
from robot_bridge.mqtt_bridge import MqttBridge
import time
import numpy as np
import matplotlib.pyplot as plt

def measure_latency(node, num_samples=100):
    """Measure round-trip latency"""
    latencies = []
    failed = 0
    
    print(f"Measuring latency with {num_samples} samples...")
    
    for i in range(num_samples):
        start = time.time()
        
        # Send ping command
        success = node.send_command_with_retry(
            {'type': 'ping', 'seq': i},
            max_retry=1,
            timeout=1.0
        )
        
        if success:
            latency = (time.time() - start) * 1000  # Convert to ms
            latencies.append(latency)
        else:
            failed += 1
        
        time.sleep(0.05)  # 50ms between samples
    
    return latencies, failed

def analyze_latency(latencies):
    """Calculate latency statistics"""
    if not latencies:
        print("No valid latency measurements!")
        return None
    
    stats = {
        'count': len(latencies),
        'avg': np.mean(latencies),
        'min': np.min(latencies),
        'max': np.max(latencies),
        'std': np.std(latencies),
        'p50': np.percentile(latencies, 50),
        'p95': np.percentile(latencies, 95),
        'p99': np.percentile(latencies, 99)
    }
    
    return stats

def plot_latency(latencies, stats):
    """Plot latency distribution"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 4))
    
    # Time series
    ax1.plot(latencies, marker='o', markersize=3, linestyle='-', linewidth=0.5)
    ax1.axhline(stats['avg'], color='r', linestyle='--', label=f"Avg: {stats['avg']:.2f}ms")
    ax1.axhline(stats['p95'], color='orange', linestyle='--', label=f"P95: {stats['p95']:.2f}ms")
    ax1.set_xlabel('Sample Number')
    ax1.set_ylabel('Latency (ms)')
    ax1.set_title('Latency Time Series')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Histogram
    ax2.hist(latencies, bins=30, edgecolor='black', alpha=0.7)
    ax2.axvline(stats['avg'], color='r', linestyle='--', label=f"Avg: {stats['avg']:.2f}ms")
    ax2.set_xlabel('Latency (ms)')
    ax2.set_ylabel('Frequency')
    ax2.set_title('Latency Distribution')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('latency_analysis.png', dpi=300)
    print("Plot saved to latency_analysis.png")
    plt.show()

def main():
    rclpy.init()
    node = MqttBridge()
    
    # Wait for connection
    print("Waiting for MQTT connection...")
    time.sleep(3)
    
    # Measure latency
    latencies, failed = measure_latency(node, num_samples=100)
    
    # Analyze
    stats = analyze_latency(latencies)
    
    if stats:
        print("\n=== LATENCY STATISTICS ===")
        print(f"Samples: {stats['count']} (failed: {failed})")
        print(f"Average: {stats['avg']:.2f} ms")
        print(f"Min: {stats['min']:.2f} ms")
        print(f"Max: {stats['max']:.2f} ms")
        print(f"Std Dev: {stats['std']:.2f} ms")
        print(f"P50 (median): {stats['p50']:.2f} ms")
        print(f"P95: {stats['p95']:.2f} ms")
        print(f"P99: {stats['p99']:.2f} ms")
        print(f"Success rate: {stats['count'] / (stats['count'] + failed) * 100:.1f}%")
        
        # Plot
        plot_latency(latencies, stats)
    
    # Cleanup
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

#### 5.2 Run Latency Test

```bash
# Install matplotlib if needed
pip install matplotlib numpy

# Run test
python3 latency_test.py
```

#### 5.3 Data Collection Table

Isi tabel dengan hasil measurement:

| Protocol | Avg Latency (ms) | Min (ms) | Max (ms) | Std (ms) | P95 (ms) | Success Rate (%) |
|----------|------------------|----------|----------|----------|----------|------------------|
| MQTT WiFi | _______ | _______ | _______ | _______ | _______ | _______ |
| Serial* | _______ | _______ | _______ | _______ | _______ | _______ |

*Optional: Implement serial variant untuk comparison

### Deliverable Percobaan 5:
- ✅ Latency measurement script implemented
- ✅ 100 samples collected
- ✅ Statistics calculated (avg, min, max, std, percentiles)
- ✅ Latency plot generated
- ✅ Data collection table filled
- ✅ Analysis: latency acceptable? (< 100ms target)

---

## PERCOBAAN 6: MULTI-DEVICE COMMUNICATION

### Tujuan:
Implement communication untuk multiple devices (2 motor + 1 sensor) dan aggregate state.

### Langkah:

#### 6.1 Firmware - Multi-Device Publisher

Update `mqtt_publisher.ino`:

```cpp
// Global state
float motor1_angle = 0.0;
float motor2_angle = 0.0;
float pressure = 0.0;

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    
    if (millis() - last_publish >= PUBLISH_INTERVAL) {
        last_publish = millis();
        
        // Publish motor 1 position
        publish_motor_position(1, motor1_angle);
        
        // Publish motor 2 position
        publish_motor_position(2, motor2_angle);
        
        // Publish sensor data
        publish_sensor_data();
        
        // Update simulated values
        motor1_angle += 1.0;
        motor2_angle += 0.5;
        pressure = random(100, 120) / 10.0;  // 10.0 - 12.0
    }
}

void publish_motor_position(int motor_id, float angle) {
    String topic = "robot/motor/" + String(motor_id) + "/position";
    String message = String(angle, 2);  // 2 decimal places
    client.publish(topic.c_str(), message.c_str());
}

void publish_sensor_data() {
    StaticJsonDocument<300> doc;
    doc["type"] = "sensor_data";
    doc["sensor_id"] = 1;
    doc["sensor_type"] = "pressure";
    doc["timestamp_ms"] = millis();
    doc["data"]["pressure"] = pressure;
    doc["seq"] = seq_num++;
    
    String message;
    serializeJson(doc, message);
    client.publish("robot/sensor/pressure", message.c_str());
}
```

#### 6.2 ROS2 - State Aggregator

File: `state_aggregator.py`

```python
#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import paho.mqtt.client as mqtt
import json

class RobotStateAggregator(Node):
    def __init__(self):
        super().__init__('state_aggregator')
        
        # Robot state
        self.state = {
            'motor_1_position': 0.0,
            'motor_2_position': 0.0,
            'pressure': 0.0,
            'last_update': {}
        }
        
        # MQTT Setup
        self.mqtt_client = mqtt.Client()
        self.mqtt_client.on_connect = self.on_mqtt_connect
        self.mqtt_client.on_message = self.on_mqtt_message
        self.mqtt_client.connect("localhost", 1883, 60)
        self.mqtt_client.loop_start()
        
        # Timer untuk publish aggregated state
        self.create_timer(1.0, self.publish_state)
    
    def on_mqtt_connect(self, client, userdata, flags, rc):
        self.get_logger().info(f"MQTT connected: {rc}")
        # Subscribe to all device topics
        client.subscribe("robot/motor/1/position")
        client.subscribe("robot/motor/2/position")
        client.subscribe("robot/sensor/pressure")
    
    def on_mqtt_message(self, client, userdata, msg):
        topic = msg.topic
        
        try:
            if topic == "robot/motor/1/position":
                self.state['motor_1_position'] = float(msg.payload.decode())
                self.state['last_update']['motor_1'] = self.get_clock().now()
            
            elif topic == "robot/motor/2/position":
                self.state['motor_2_position'] = float(msg.payload.decode())
                self.state['last_update']['motor_2'] = self.get_clock().now()
            
            elif topic == "robot/sensor/pressure":
                payload = json.loads(msg.payload.decode())
                self.state['pressure'] = payload['data']['pressure']
                self.state['last_update']['pressure'] = self.get_clock().now()
        
        except Exception as e:
            self.get_logger().error(f"Error processing message: {e}")
    
    def publish_state(self):
        """Publish aggregated robot state"""
        self.get_logger().info(
            f"Robot State - "
            f"M1={self.state['motor_1_position']:.2f}°, "
            f"M2={self.state['motor_2_position']:.2f}°, "
            f"P={self.state['pressure']:.2f} bar"
        )
        
        # Check for stale data (no update > 2 seconds)
        current_time = self.get_clock().now()
        for device, last_update in self.state['last_update'].items():
            age = (current_time - last_update).nanoseconds / 1e9
            if age > 2.0:
                self.get_logger().warn(f"Stale data from {device}: {age:.1f}s old")

def main():
    rclpy.init()
    node = RobotStateAggregator()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

#### 6.3 Test Multi-Device

```bash
# Terminal 1: Run firmware (ESP32 publishing to 3 topics)

# Terminal 2: Monitor all topics
mosquitto_sub -h localhost -v -t 'robot/#'

# Terminal 3: Run state aggregator
ros2 run robot_bridge state_aggregator

# Expected output:
# Robot State - M1=45.00°, M2=22.50°, P=11.30 bar
```

### Deliverable Percobaan 6:
- ✅ Firmware publishing 3 topics (2 motor, 1 sensor)
- ✅ State aggregator receiving all topics
- ✅ Aggregated state logged every 1 second
- ✅ Stale data detection working
- ✅ Screenshot showing multi-topic data flow

---

## 📊 DATA PENGAMATAN

### 1. Protocol Selection
| Protocol | Score | Notes |
|----------|-------|-------|
| MQTT | ___ | ___________________ |
| Micro-ROS | ___ | ___________________ |
| Serial | ___ | ___________________ |
| **Selected** | MQTT | ___________________ |

### 2. Message Format Specification
- Sensor data fields: __________________
- Motor command fields: __________________
- ACK fields: __________________
- CRC algorithm: __________________

### 3. MQTT Topics List
```bash
# Command output: mosquitto_sub -h localhost -v -t '#'
robot/sensor/imu
robot/motor/1/position
robot/motor/2/position
robot/sensor/pressure
robot/command
robot/ack
```

### 4. Sample Messages
**Sensor Data**:
```json
[Paste actual message dari mosquitto_sub]
```

**Motor Command**:
```json
[Paste command message]
```

**ACK**:
```json
[Paste ACK message]
```

### 5. ACK Mechanism Test
- Total commands sent: ___
- ACK received: ___
- Timeout occurred: ___
- Success rate: ____%

### 6. Latency Measurement
- Average latency: _____ ms
- Min latency: _____ ms
- Max latency: _____ ms
- Std deviation: _____ ms
- P95: _____ ms
- P99: _____ ms

### 7. Multi-Device Communication
- Total topics: ___
- Publish rate per topic: ___ Hz
- State aggregation working: ☐ Yes ☐ No
- Stale data detection: ☐ Yes ☐ No

---

## 🔍 ANALISIS

### 1. Protocol Choice
**Question**: Apakah protokol MQTT sesuai untuk aplikasi ini?  
**Answer**: _________________________________________________

**Question**: Apa kelebihan dan kekurangan vs alternative?  
**Answer**: _________________________________________________

### 2. Message Format
**Question**: Apakah JSON format balanced antara readability dan size?  
**Answer**: _________________________________________________

**Question**: Untuk data apa yang cocok pakai binary format?  
**Answer**: _________________________________________________

### 3. Reliability
**Question**: Apakah ACK mechanism reliable (success rate 100%)?  
**Answer**: _________________________________________________

**Question**: Berapa retry count yang optimal?  
**Answer**: _________________________________________________

### 4. Latency
**Question**: Apakah latency < 100ms target tercapai?  
**Answer**: _________________________________________________

**Question**: Apa bottleneck utama latency?  
**Answer**: _________________________________________________

### 5. Multi-Device
**Question**: Apakah state aggregation berjalan stabil?  
**Answer**: _________________________________________________

**Question**: Bagaimana handle jika salah satu device offline?  
**Answer**: _________________________________________________

---

## 💡 KESIMPULAN

Tulis kesimpulan praktikum (minimum 200 kata) yang mencakup:

1. **Communication Bridge Importance**
   - Mengapa bridge diperlukan untuk ESP32-ROS2 integration?
   - Apa challenge utama dalam bridging?

2. **Protocol Selection**
   - Mengapa MQTT dipilih?
   - Trade-off apa yang dipertimbangkan?

3. **Reliability Mechanism**
   - Bagaimana ACK & retry ensure message delivery?
   - Apa impact timeout value terhadap performance?

4. **Performance**
   - Apakah latency memenuhi real-time requirement?
   - Apa yang bisa dioptimasi untuk reduce latency?

5. **Scalability**
   - Bagaimana sistem handle multiple devices?
   - Apa limitation current implementation?

6. **Integration**
   - Bagaimana modul ini integrate dengan Batch 1-3?
   - Apa next step untuk complete system?

---

**Kesimpulan Anda**:

_________________________________________________________________

_________________________________________________________________

_________________________________________________________________

_________________________________________________________________

_________________________________________________________________

---

## 📚 TUGAS TAMBAHAN

### Tugas 1: Binary Format Implementation
Implement binary message format dan compare performance dengan JSON.

### Tugas 2: Serial Communication
Implement serial bridge sebagai alternative MQTT.

### Tugas 3: TLS Encryption
Implement MQTT dengan TLS untuk secure communication.

### Tugas 4: Message Priority
Implement priority queue untuk critical messages.

---

## ✅ CHECKLIST SEBELUM SUBMIT

- ☐ Semua percobaan completed (1-6)
- ☐ Data pengamatan filled completely
- ☐ Analisis answered dengan detail
- ☐ Kesimpulan written (min 200 kata)
- ☐ Code tested dan berjalan tanpa error
- ☐ Screenshot/plot included
- ☐ Code uploaded ke GitHub dengan README
- ☐ Video demo recorded (5-10 menit)

---

**Last Updated**: February 2026  
**Module**: 11 - ESP32 ↔ ROS2 Bridge  
**Type**: Jobsheet (Praktikum)
