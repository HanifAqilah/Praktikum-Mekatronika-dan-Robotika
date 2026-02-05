# MODUL 11: ESP32 ↔ ROS2 BRIDGE
## COMMUNICATION RELIABILITY

---

## 📋 TUJUAN PEMBELAJARAN

Setelah menyelesaikan modul ini, mahasiswa mampu:
1. Memahami dan memilih protokol komunikasi yang tepat (MQTT vs micro-ROS vs serial)
2. Merancang format pesan (JSON, binary, protobuf)
3. Mengimplementasikan reliable message exchange dengan acknowledge & retry
4. Mengukur dan mengoptimasi latency komunikasi
5. Menangani error dan recovery pada communication bridge
6. Mengimplementasikan multi-device communication

---

## 1. DEFINISI

### Communication Bridge
**Communication bridge** adalah sistem yang menghubungkan dua platform berbeda yang menggunakan protokol komunikasi yang tidak kompatibel secara native. Dalam konteks ini, bridge menghubungkan:
- **Embedded system** (ESP32 dengan firmware C/C++)
- **High-level system** (Ubuntu dengan ROS2 Python/C++)

### Protocol
**Protocol** adalah sekumpulan aturan yang mendefinisikan:
- **Format pesan**: struktur data yang dikirim
- **Message exchange**: cara pesan dikirim dan diterima
- **Error handling**: cara menangani pesan yang hilang atau corrupt

### Reliability
**Reliability** adalah jaminan bahwa pesan yang dikirim akan sampai ke tujuan dengan benar. Terdapat dua pendekatan:
- **Best-effort**: kirim pesan tanpa jaminan (cepat tapi bisa hilang)
- **Reliable delivery**: jaminan pesan sampai (lebih lambat tapi aman)

---

## 2. KONSEP INTI

### 2.1 MQTT (Message Queuing Telemetry Transport)

**MQTT** adalah protokol publish-subscribe yang lightweight dan widely adopted untuk IoT.

#### Karakteristik MQTT:
- **Lightweight**: protokol sederhana, overhead kecil
- **Publish-Subscribe**: decoupled communication (publisher tidak tahu siapa subscriber)
- **Broker-based**: central hub yang routing message
- **Low bandwidth**: efisien untuk network terbatas

#### Komponen MQTT:
1. **MQTT Broker**: Central server (mosquitto, HiveMQ, AWS IoT Core)
2. **Publisher**: Device yang mengirim message
3. **Subscriber**: Device yang menerima message
4. **Topic**: Hierarchical name untuk categorize message

#### MQTT Topic Hierarchy
```
robot/
├── sensor/
│   ├── imu
│   ├── pressure
│   └── temperature
├── motor/
│   ├── 1/position
│   ├── 1/velocity
│   ├── 2/position
│   └── 2/velocity
├── command
└── ack
```

#### QoS (Quality of Service) Levels
| Level | Name | Description | Use Case |
|-------|------|-------------|----------|
| 0 | Fire and Forget | Tidak ada acknowledgment | Sensor data non-critical |
| 1 | At Least Once | Guarantee delivery, mungkin duplicate | Command execution |
| 2 | Exactly Once | Guarantee delivery, no duplicate | Critical transaction |

### 2.2 Micro-ROS

**Micro-ROS** adalah implementasi ROS2 untuk microcontroller, menggunakan DDS sebagai middleware native.

#### Karakteristik:
- **Native ROS2**: langsung menggunakan ROS2 message & services
- **DDS-based**: menggunakan DDS (Data Distribution Service) protocol
- **More heavyweight**: membutuhkan lebih banyak resource
- **Seamless integration**: tidak perlu message conversion

#### Trade-off vs MQTT:
- ✅ **Advantage**: Native ROS2, no conversion overhead
- ❌ **Disadvantage**: Lebih complex setup, resource intensive

### 2.3 Serial Communication

**Serial communication** adalah komunikasi point-to-point langsung via UART.

#### Karakteristik:
- **Direct connection**: tidak butuh network atau broker
- **Low latency**: komunikasi langsung, overhead minimal
- **Simple protocol**: mudah implement
- **Limited range**: terbatas pada panjang kabel

#### Konfigurasi Serial:
```
Baud rate: 115200 bps (recommended untuk ESP32)
Data bits: 8
Parity: None
Stop bits: 1
Flow control: None
```

### 2.4 Message Format

#### JSON Format
**Advantages**:
- Human-readable (mudah debug)
- Flexible schema
- Widely supported libraries

**Disadvantages**:
- Verbose (ukuran besar)
- Parsing overhead

**Example**:
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
  "crc": "a1b2"
}
```

#### Binary Format
**Advantages**:
- Compact (ukuran kecil)
- Fast parsing

**Disadvantages**:
- Not human-readable
- Rigid schema

**Example** (struct format):
```python
import struct
# Format: 'I' = unsigned int (4 bytes), 'f' = float (4 bytes)
# sensor_id, timestamp, accel_x, accel_y, accel_z
data = struct.pack('IIfff', 1, 1234567890, 9.81, 0.0, 0.0)
```

### 2.5 Data Serialization & Deserialization

**Serialization**: Convert data structure → byte stream untuk transport
**Deserialization**: Convert byte stream → data structure untuk processing

#### JSON Serialization (Python):
```python
import json
data = {"sensor_id": 1, "value": 9.81}
message = json.dumps(data)  # Serialize
```

#### JSON Deserialization (Python):
```python
received = '{"sensor_id": 1, "value": 9.81}'
data = json.loads(received)  # Deserialize
```

#### JSON Serialization (C++/Arduino):
```cpp
#include <ArduinoJson.h>
StaticJsonDocument<200> doc;
doc["sensor_id"] = 1;
doc["value"] = 9.81;
String message;
serializeJson(doc, message);  // Serialize
```

### 2.6 CRC (Cyclic Redundancy Check)

**CRC** adalah checksum untuk mendeteksi message corruption selama transport.

#### CRC Calculation (Python):
```python
import binascii

def calculate_crc(message):
    return hex(binascii.crc32(message.encode()) & 0xFFFFFFFF)

message = '{"sensor_id": 1, "value": 9.81}'
crc = calculate_crc(message)
```

#### CRC Verification:
```python
def verify_message(message, received_crc):
    calculated_crc = calculate_crc(message)
    return calculated_crc == received_crc
```

### 2.7 Acknowledge & Retry

**Acknowledge (ACK)**: Confirmation message bahwa pesan telah diterima  
**Retry**: Mengirim ulang pesan jika ACK tidak diterima dalam timeout

#### Acknowledge Flow:
```
Sender                          Receiver
  |                                |
  |--- Command (seq=1) ----------->|
  |                                | Process command
  |<---------- ACK (seq=1) --------|
  |                                |
```

#### Timeout & Retry Flow:
```
Sender                          Receiver
  |                                |
  |--- Command (seq=2) ----X       | (Lost)
  |                                |
  | Wait timeout (1 sec)           |
  |                                |
  |--- Retry Command (seq=2) ----->|
  |                                | Process command
  |<---------- ACK (seq=2) --------|
  |                                |
```

### 2.8 Connection State Management

State machine untuk connection status:

```
DISCONNECTED
    ↓ connect()
CONNECTING
    ↓ connection_success
CONNECTED ←→ (send/receive messages)
    ↓ connection_lost
ERROR
    ↓ reconnect()
CONNECTING
```

#### Auto-reconnect Implementation:
```python
def ensure_connected(self):
    if not self.mqtt_client.is_connected():
        try:
            self.mqtt_client.reconnect()
            self.get_logger().info("Reconnected to MQTT broker")
        except Exception as e:
            self.get_logger().error(f"Reconnection failed: {e}")
```

### 2.9 Latency Optimization

**Latency** adalah waktu delay dari pesan dikirim hingga diterima dan diproses.

#### Latency Components:
1. **Serialization time**: encode data → message
2. **Transport time**: kirim via network/serial
3. **Deserialization time**: decode message → data
4. **Processing time**: execute action

#### Measurement:
```python
import time

start = time.time()
send_message(data)
wait_for_ack()
latency_ms = (time.time() - start) * 1000
```

#### Optimization Strategies:
- Use binary format (vs JSON)
- Reduce message size
- Increase baud rate (serial)
- Use QoS 0 untuk non-critical data
- Batch multiple messages

---

## 3. DIAGRAM & ILUSTRASI

### 3.1 System Architecture

```
┌─────────────┐         ┌──────────────┐         ┌─────────────┐
│   ESP32     │         │ MQTT Broker  │         │   ROS2      │
│  Firmware   │◄───────►│  (mosquitto) │◄───────►│   Bridge    │
│             │  WiFi   │              │  TCP    │    Node     │
└─────────────┘         └──────────────┘         └─────────────┘
      │                                                  │
      │ Publish:                                         │ Subscribe:
      │ - sensor data                                    │ - sensor data
      │                                                  │
      │ Subscribe:                                       │ Publish:
      │ - motor command                                  │ - motor command
      │                                                  │
      └──────────────────────────────────────────────────┘
                    Bidirectional Communication
```

### 3.2 Message Flow (Sensor Data)

```
Sensor → ESP32 → JSON Encode → MQTT Publish → Broker → Subscribe → 
ROS2 Node → JSON Decode → ROS2 Message → ROS2 Topic
```

### 3.3 Message Flow (Motor Command)

```
ROS2 Application → ROS2 Service Call → ROS2 Bridge → JSON Encode → 
MQTT Publish → Broker → Subscribe → ESP32 → JSON Decode → Motor Control
```

### 3.4 Acknowledge & Retry Timeline

```
Time (ms)
0    ┌─ Send Command (seq=1)
     │
50   │  ┌─ Receive Command
     │  │
     │  └─ Process Command
     │
100  └─ Receive ACK (seq=1) ✓

     ┌─ Send Command (seq=2)
150  │
     │
250  │  (No ACK - TIMEOUT)
     │
     ├─ Retry 1: Send Command (seq=2)
300  │
     │  ┌─ Receive Command
350  │  │
     │  └─ Send ACK
     │
400  └─ Receive ACK (seq=2) ✓
```

---

## 4. CONTOH KASUS INDUSTRI

### Case 1: Industrial Robot
**Scenario**: Robot arm dengan 6 motor, 3 sensor (force, position, temperature)  
**Challenge**: Reliable sensor data collection, low-latency motor command  
**Solution**: MQTT dengan QoS 1 untuk command, QoS 0 untuk sensor streaming

**Result**:
- Latency: 15-30ms average
- Reliability: 99.9% message delivery
- Scalability: Support 10+ robot dengan single broker

### Case 2: Smart Factory
**Scenario**: 100+ device (robot, conveyor, sensor) berkomunikasi  
**Challenge**: Scalability, message routing, network bandwidth  
**Solution**: MQTT broker cluster, topic hierarchy, QoS optimization

**Result**:
- Throughput: 10,000+ messages/second
- Scalability: 500+ concurrent devices
- Reliability: Broker redundancy, zero downtime

### Case 3: AGV Fleet Management
**Scenario**: 20 AGV robot bergerak, central station monitor & control  
**Challenge**: Real-time position update, command dispatch  
**Solution**: MQTT topic per-robot, publish position setiap 100ms

**Topic Structure**:
```
fleet/
├── agv_01/position
├── agv_01/battery
├── agv_01/status
├── agv_02/position
...
└── command/agv_01
```

**Result**:
- Position update rate: 10 Hz per AGV
- Command latency: < 50ms
- Network load: < 1 Mbps untuk 20 AGV

---

## 5. PROTOCOL COMPARISON

| Aspect | MQTT | Micro-ROS | Serial |
|--------|------|-----------|--------|
| **Latency** | 15-50ms | 10-30ms | 5-15ms |
| **Bandwidth** | Low | Medium | Very Low |
| **Complexity** | Medium | High | Low |
| **Scalability** | Excellent | Good | Poor (1:1 only) |
| **Reliability** | High (QoS 1-2) | High (DDS) | Medium |
| **Setup** | Broker required | Complex build | Cable only |
| **Cost** | Free (mosquitto) | Free | Cable cost |
| **Range** | Network (unlimited) | Network | 15m typical |
| **Learning curve** | Easy | Steep | Very Easy |

### Recommendation:
- **MQTT**: Best untuk most cases (balance semua aspek)
- **Micro-ROS**: Jika butuh native ROS2 integration & resource cukup
- **Serial**: Jika single device, low latency critical, network unavailable

---

## 6. MESSAGE FORMAT DESIGN

### 6.1 Sensor Data Message

```json
{
  "type": "sensor_data",
  "sensor_id": 1,
  "sensor_type": "imu",
  "timestamp_ms": 1234567890,
  "data": {
    "accel_x": 9.81,
    "accel_y": 0.0,
    "accel_z": 0.0,
    "gyro_x": 0.0,
    "gyro_y": 0.0,
    "gyro_z": 0.0
  },
  "seq": 42,
  "crc": "a1b2c3d4"
}
```

**Fields**:
- `type`: Message type identifier
- `sensor_id`: Unique sensor identifier
- `sensor_type`: Sensor category (imu, pressure, temperature)
- `timestamp_ms`: Millisecond timestamp dari ESP32
- `data`: Sensor readings (flexible structure)
- `seq`: Sequence number untuk tracking & ordering
- `crc`: Checksum untuk data integrity

### 6.2 Motor Command Message

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

### 6.3 Acknowledgment Message

```json
{
  "type": "ack",
  "seq": 15,
  "status": "success",
  "error_code": 0,
  "timestamp_ms": 1234567905
}
```

---

## 7. ERROR HANDLING & RECOVERY

### 7.1 Common Errors

1. **Connection Lost**: Network disconnect, broker down
2. **Timeout**: ACK tidak diterima dalam waktu yang ditentukan
3. **Message Corruption**: CRC mismatch
4. **Invalid Message**: JSON parse error, missing field
5. **Sequence Error**: Out-of-order message, duplicate

### 7.2 Error Recovery Strategies

#### Timeout Handling:
```python
MAX_RETRY = 3
TIMEOUT_SEC = 1.0

for attempt in range(MAX_RETRY):
    send_message(data)
    if wait_for_ack(TIMEOUT_SEC):
        return True  # Success
    else:
        log_warning(f"Timeout, retry {attempt + 1}")

return False  # All retries failed
```

#### Connection Recovery:
```python
def auto_reconnect(self):
    retry_delay = 1.0  # Start with 1 second
    max_delay = 60.0   # Max 60 seconds
    
    while not self.connected:
        try:
            self.mqtt_client.reconnect()
            self.connected = True
            retry_delay = 1.0  # Reset delay
        except Exception as e:
            log_error(f"Reconnect failed: {e}")
            time.sleep(retry_delay)
            retry_delay = min(retry_delay * 2, max_delay)  # Exponential backoff
```

#### Message Validation:
```python
def validate_message(msg_dict):
    # Check required fields
    required = ['type', 'sensor_id', 'data', 'seq', 'crc']
    for field in required:
        if field not in msg_dict:
            raise ValueError(f"Missing field: {field}")
    
    # Verify CRC
    msg_copy = msg_dict.copy()
    received_crc = msg_copy.pop('crc')
    calculated_crc = calculate_crc(json.dumps(msg_copy))
    
    if received_crc != calculated_crc:
        raise ValueError("CRC mismatch")
    
    return True
```

---

## 8. LATENCY ANALYSIS

### 8.1 Latency Components

```
Total Latency = Serialization + Transport + Deserialization + Processing

Example (MQTT over WiFi):
- Serialization: 2-5 ms (JSON encoding)
- Transport: 10-40 ms (WiFi + broker routing)
- Deserialization: 2-5 ms (JSON decoding)
- Processing: 1-3 ms (ROS2 publish)
Total: 15-53 ms
```

### 8.2 Latency Measurement

```python
import numpy as np
import time

def measure_latency(num_samples=100):
    latencies = []
    
    for i in range(num_samples):
        start = time.time()
        send_command({'type': 'ping', 'seq': i})
        wait_for_ack(timeout=1.0)
        latency = (time.time() - start) * 1000  # ms
        latencies.append(latency)
    
    return {
        'avg': np.mean(latencies),
        'min': np.min(latencies),
        'max': np.max(latencies),
        'std': np.std(latencies),
        'p50': np.percentile(latencies, 50),
        'p95': np.percentile(latencies, 95),
        'p99': np.percentile(latencies, 99)
    }
```

### 8.3 Typical Latency Values

| Protocol | Avg (ms) | Min (ms) | Max (ms) | Jitter (std) |
|----------|----------|----------|----------|--------------|
| MQTT WiFi | 25 | 10 | 80 | 15 |
| MQTT Ethernet | 15 | 8 | 40 | 8 |
| Serial USB | 8 | 5 | 20 | 3 |
| Micro-ROS | 20 | 12 | 60 | 10 |

---

## 9. MULTI-DEVICE COMMUNICATION

### 9.1 Topic Design

Untuk multiple devices, gunakan hierarchical topic structure:

```
robot/
├── motor/1/position
├── motor/1/velocity
├── motor/1/current
├── motor/2/position
├── motor/2/velocity
├── motor/2/current
├── sensor/imu/data
├── sensor/pressure/data
└── sensor/temperature/data
```

### 9.2 State Aggregation

Combine data dari multiple devices menjadi single robot state:

```python
class RobotStateAggregator:
    def __init__(self):
        self.state = {
            'motor_1': {'position': 0.0, 'velocity': 0.0},
            'motor_2': {'position': 0.0, 'velocity': 0.0},
            'imu': {'accel_x': 0.0, 'accel_y': 0.0, 'accel_z': 0.0},
            'pressure': 0.0,
            'temperature': 0.0
        }
    
    def update_motor_position(self, motor_id, position):
        self.state[f'motor_{motor_id}']['position'] = position
    
    def update_sensor(self, sensor_type, value):
        self.state[sensor_type] = value
    
    def get_complete_state(self):
        return self.state
```

---

## 10. SECURITY CONSIDERATIONS

### 10.1 MQTT Security

1. **Authentication**: Username & password untuk broker
2. **Encryption**: TLS/SSL untuk message encryption
3. **Authorization**: Topic-based access control

#### TLS Configuration:
```python
import paho.mqtt.client as mqtt

client = mqtt.Client()
client.tls_set(
    ca_certs="/path/to/ca.crt",
    certfile="/path/to/client.crt",
    keyfile="/path/to/client.key"
)
client.username_pw_set("username", "password")
client.connect("mqtt.example.com", 8883)  # Port 8883 for TLS
```

### 10.2 API Key

Untuk REST API (Modul 13 integration):
```python
API_KEY = "your-secret-api-key"

def verify_api_key(request):
    provided_key = request.headers.get('X-API-Key')
    if provided_key != API_KEY:
        return False
    return True
```

---

## 11. TROUBLESHOOTING

### Common Issues & Solutions

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| Message tidak sampai | Broker down, network issue | Check broker status, ping test |
| High latency | Network congestion, large message | Reduce message size, use binary format |
| ACK timeout | ESP32 busy, processing slow | Increase timeout, optimize firmware |
| CRC mismatch | Message corruption, encoding error | Check network quality, verify encoding |
| Connection lost | WiFi unstable, broker restart | Implement auto-reconnect |
| Duplicate message | QoS 1 retry | Use sequence number untuk detect duplicate |

### Debug Tools:
```bash
# Monitor all MQTT traffic
mosquitto_sub -h localhost -v -t '#'

# Publish test message
mosquitto_pub -h localhost -t 'robot/test' -m '{"test": true}'

# Check broker status
sudo systemctl status mosquitto

# Network latency test
ping <broker-ip>

# Serial monitor
screen /dev/ttyUSB0 115200
```

---

## 12. BEST PRACTICES

### 12.1 Message Design
- ✅ Keep message compact (< 1KB ideal)
- ✅ Include sequence number untuk tracking
- ✅ Add CRC untuk data integrity
- ✅ Use consistent timestamp format
- ❌ Avoid nested structure yang terlalu dalam

### 12.2 Topic Naming
- ✅ Use hierarchical structure
- ✅ Use lowercase dengan underscore
- ✅ Be specific (robot/motor/1/position vs robot/data)
- ❌ Avoid wildcard dalam topic name

### 12.3 Error Handling
- ✅ Always validate message format
- ✅ Implement timeout & retry
- ✅ Log all errors dengan context
- ✅ Graceful degradation (fallback mode)
- ❌ Never ignore errors silently

### 12.4 Performance
- ✅ Measure latency regularly
- ✅ Monitor message loss rate
- ✅ Use appropriate QoS level
- ✅ Batch non-critical messages
- ❌ Don't publish faster than necessary

---

## 13. RINGKASAN

### Key Takeaways:

1. **MQTT** adalah protokol yang simple, reliable, dan widely adopted untuk IoT communication
2. **Protocol selection** bergantung pada requirements: latency, bandwidth, complexity, scalability
3. **Message format** mempengaruhi latency dan bandwidth (JSON vs binary trade-off)
4. **Acknowledge & retry** essential untuk robust communication
5. **Connection state management** prevent communication failure
6. **Latency measurement** penting untuk verify system performance
7. **Error handling** critical untuk production system
8. **Security** harus dipertimbangkan untuk deployment

### Integration dengan Batch lain:
- **Batch 2 (Firmware)**: Provide sensor data publisher & motor command subscriber
- **Batch 3 (ROS2)**: Subscribe sensor data, publish motor command
- **Batch 4 (Modul 13)**: REST API dapat trigger MQTT command
- **Batch 4 (Modul 14)**: MQTT data stored ke InfluxDB untuk monitoring

---

## 14. REFERENSI

### Documentation:
- MQTT Protocol Specification: https://mqtt.org/mqtt-specification/
- Paho MQTT Python: https://www.eclipse.org/paho/clients/python/
- PubSubClient Arduino: https://pubsubclient.knolleary.net/
- ArduinoJson: https://arduinojson.org/
- ROS2 Python Client: https://docs.ros.org/en/rolling/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Py-Publisher-And-Subscriber.html

### Books:
- "MQTT Essentials" - HiveMQ
- "IoT Communication Protocols" - O'Reilly

### Tools:
- MQTT Explorer: Desktop MQTT client untuk debugging
- Wireshark: Network packet analyzer
- MQTT.fx: MQTT test client

---

## 15. NEXT STEPS

Setelah menguasai Modul 11, lanjut ke:
- **Modul 12**: State Machine untuk orchestrate communication & operation logic
- **Modul 13**: Web API untuk expose MQTT data via REST
- **Modul 14**: Store MQTT data ke InfluxDB untuk long-term monitoring

---

**Last Updated**: February 2026  
**Module**: 11 - ESP32 ↔ ROS2 Bridge  
**Batch**: 4 (System Integration)
