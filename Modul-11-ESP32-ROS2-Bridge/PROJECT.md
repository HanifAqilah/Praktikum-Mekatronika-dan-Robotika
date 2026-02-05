# PROJECT MODUL 11
## ROBUST ROBOT COMMUNICATION SYSTEM WITH ERROR HANDLING

---

## 📋 PROJECT OVERVIEW

**Judul**: Robust Robot Communication System with Error Handling  
**Durasi**: 2-3 minggu  
**Team Size**: 2-3 orang  
**Complexity**: ⭐⭐⭐⭐ (Advanced)

---

## 🎯 TUJUAN PROJECT

Membangun sistem komunikasi yang robust dan reliable untuk menghubungkan ESP32 firmware dengan ROS2, dengan fokus pada:
- Protocol implementation (MQTT recommended)
- Bidirectional communication
- Error handling & recovery
- Performance optimization
- Multi-device support

---

## 📐 SPESIFIKASI TEKNIS

### Functional Requirements:

#### FR-1: Protocol Implementation
- ✅ Implement MQTT communication (primary)
- ✅ Support QoS 0 (sensor data) dan QoS 1 (commands)
- ✅ JSON message format dengan CRC validation
- ⭐ Optional: Binary format untuk high-frequency data

#### FR-2: Bidirectional Communication
- ✅ **ESP32 → ROS2**: Sensor data publishing (10+ Hz)
  - IMU data (accel, gyro)
  - Motor position feedback
  - System status
- ✅ **ROS2 → ESP32**: Motor control commands
  - Target position/velocity
  - Emergency stop
  - Configuration updates

#### FR-3: Reliability Mechanism
- ✅ Acknowledge (ACK) untuk all commands
- ✅ Automatic retry (max 3 attempts)
- ✅ Configurable timeout (default 1 second)
- ✅ Sequence number tracking
- ✅ Duplicate detection

#### FR-4: Connection Management
- ✅ Auto-connect on startup
- ✅ Auto-reconnect on connection lost
- ✅ Exponential backoff untuk retry delay
- ✅ Connection state monitoring
- ✅ Heartbeat/keepalive mechanism

#### FR-5: Performance
- ✅ Average latency < 50ms (target)
- ✅ Maximum latency < 100ms (requirement)
- ✅ Message success rate > 99%
- ✅ Support 10+ messages/second per device

#### FR-6: Multi-Device Support
- ✅ Minimum 2 motors + 3 sensors
- ✅ Independent topic per device
- ✅ State aggregation node
- ✅ Synchronized timestamp

#### FR-7: Error Handling
- ✅ CRC validation untuk data integrity
- ✅ Timeout detection & recovery
- ✅ Malformed message rejection
- ✅ Connection loss recovery
- ✅ Error logging & reporting

### Non-Functional Requirements:

#### NFR-1: Code Quality
- ✅ Modular design (separate classes/modules)
- ✅ Comprehensive comments
- ✅ Follow naming conventions
- ✅ Error handling di all functions

#### NFR-2: Testing
- ✅ Unit tests untuk message parsing
- ✅ Integration tests untuk end-to-end flow
- ✅ Performance tests (latency, throughput)
- ✅ Stress tests (connection loss, high load)

#### NFR-3: Documentation
- ✅ README dengan setup instructions
- ✅ API documentation (message formats)
- ✅ Architecture diagram
- ✅ Troubleshooting guide

#### NFR-4: Deployment
- ✅ Easy setup process
- ✅ Configuration via config file
- ✅ Logging untuk debugging
- ⭐ Optional: Docker container

---

## 🏗️ ARSITEKTUR SISTEM

```
┌────────────────────────────────────────────────────────────────┐
│                         MQTT BROKER                            │
│                        (mosquitto)                             │
│                     localhost:1883                             │
└────────────────────────────────────────────────────────────────┘
          ▲                                      ▲
          │ WiFi                                 │ TCP
          │                                      │
┌─────────┴─────────┐                 ┌─────────┴──────────┐
│   ESP32 FIRMWARE  │                 │   ROS2 BRIDGE      │
│                   │                 │                    │
│ Publishers:       │                 │ Subscribers:       │
│ - sensor/imu      │                 │ - sensor/imu       │
│ - motor/1/pos     │                 │ - motor/+/pos      │
│ - motor/2/pos     │                 │                    │
│ - status          │                 │ Publishers:        │
│                   │                 │ - command          │
│ Subscribers:      │                 │                    │
│ - command         │                 │ ROS2 Topics:       │
│                   │                 │ - /robot/imu       │
│ Functions:        │                 │ - /robot/state     │
│ - ACK handler     │                 │                    │
│ - CRC validation  │                 │ Functions:         │
│ - Auto-reconnect  │                 │ - ACK tracking     │
└───────────────────┘                 │ - Retry handler    │
                                      │ - State aggregator │
                                      └────────────────────┘
```

---

## 📦 DELIVERABLES

### 1. Communication Protocol Specification
**File**: `docs/protocol_spec.md`

Harus berisi:
- Protocol choice justification
- Message format schema (semua message types)
- Topic hierarchy design
- QoS level assignment
- Error codes definition

### 2. Message Format Specification
**File**: `docs/message_format.md`

Define semua message types dengan JSON schema:
```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "SensorData",
  "type": "object",
  "required": ["type", "sensor_id", "data", "seq"],
  "properties": {
    "type": { "type": "string", "const": "sensor_data" },
    "sensor_id": { "type": "integer" },
    "sensor_type": { "type": "string" },
    "timestamp_ms": { "type": "integer" },
    "data": { "type": "object" },
    "seq": { "type": "integer" },
    "crc": { "type": "string" }
  }
}
```

### 3. ESP32 Firmware Code
**Directory**: `firmware/mqtt_bridge/`

Files:
- `mqtt_bridge.ino` - Main firmware
- `mqtt_handler.cpp/h` - MQTT communication logic
- `message_handler.cpp/h` - Message parsing & creation
- `crc_utils.cpp/h` - CRC calculation
- `config.h` - Configuration parameters
- `secrets.h` - WiFi credentials (gitignored)
- `platformio.ini` - Dependencies
- `README.md` - Setup instructions

### 4. ROS2 Bridge Node
**Directory**: `ros2_bridge/robot_bridge/`

Files:
- `mqtt_bridge.py` - Main bridge node
- `message_parser.py` - Message validation & parsing
- `ack_manager.py` - ACK tracking & retry logic
- `connection_manager.py` - Connection state management
- `state_aggregator.py` - Multi-device state aggregation
- `config.yaml` - ROS2 parameters
- `package.xml` - Package metadata
- `setup.py` - Package setup

### 5. State Aggregator Node
**File**: `ros2_bridge/robot_bridge/state_aggregator.py`

Features:
- Subscribe multiple MQTT topics
- Merge device states
- Detect stale data
- Publish aggregated ROS2 message
- Health monitoring

### 6. ACK & Retry Handler
**File**: `ros2_bridge/robot_bridge/ack_manager.py`

```python
class AckManager:
    def __init__(self, max_retry=3, timeout=1.0):
        self.max_retry = max_retry
        self.timeout = timeout
        self.pending_acks = {}
        self.ack_received = set()
    
    def send_with_retry(self, message, publish_func):
        # Implementation...
        pass
    
    def register_ack(self, seq):
        # Mark sequence as acknowledged
        pass
```

### 7. Connection State Manager
**File**: `ros2_bridge/robot_bridge/connection_manager.py`

State machine untuk connection:
- DISCONNECTED → CONNECTING → CONNECTED
- Error handling
- Auto-reconnect dengan exponential backoff

### 8. Latency Measurement Tool
**File**: `tools/latency_test.py`

Features:
- Measure round-trip latency (100+ samples)
- Calculate statistics (avg, min, max, std, percentiles)
- Generate plot (time series + histogram)
- Export CSV data

### 9. Performance Test Report
**File**: `docs/performance_report.md`

Include:
- Latency test results
- Throughput test results
- Success rate statistics
- Resource usage (CPU, memory, network)
- Bottleneck analysis
- Optimization recommendations

### 10. Troubleshooting Guide
**File**: `docs/troubleshooting.md`

Cover common issues:
- Connection problems
- High latency
- Message loss
- CRC errors
- Firmware crashes

### 11. GitHub Repository
**Structure**:
```
robot-communication-bridge/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── protocol_spec.md
│   ├── message_format.md
│   ├── architecture.md
│   ├── performance_report.md
│   └── troubleshooting.md
├── firmware/
│   └── mqtt_bridge/
│       ├── mqtt_bridge.ino
│       ├── mqtt_handler.cpp/h
│       └── ...
├── ros2_bridge/
│   └── robot_bridge/
│       ├── mqtt_bridge.py
│       ├── ack_manager.py
│       └── ...
├── tools/
│   ├── latency_test.py
│   └── mqtt_monitor.py
└── tests/
    ├── test_message_parser.py
    └── test_ack_manager.py
```

### 12. Video Demo
**Duration**: 10-15 menit

**Content**:
1. System architecture explanation (2 min)
2. Firmware demonstration:
   - WiFi connection
   - MQTT publishing
   - Serial monitor output
3. ROS2 bridge demonstration:
   - Bridge node startup
   - Topic echo showing data
   - Multi-device aggregation
4. Reliability demonstration:
   - Send command dengan ACK
   - Timeout & retry test
   - Connection recovery test
5. Performance test:
   - Latency measurement live
   - Plot interpretation
6. Troubleshooting demo:
   - Simulate error (disconnect network)
   - Show recovery process

---

## ⭐ BONUS FEATURES

### Bonus 1: Redundant Communication (20 pts)
Implement dual-broker failover:
- Primary broker: localhost:1883
- Secondary broker: backup.local:1883
- Auto-switch pada failure

### Bonus 2: TLS Encryption (15 pts)
Secure MQTT dengan TLS/SSL:
- Generate certificates
- Configure broker untuk TLS
- Update firmware & bridge untuk use port 8883

### Bonus 3: Binary Format Optimization (15 pts)
Implement binary protocol untuk high-frequency data:
- Define binary message structure (struct)
- Implement serialization/deserialization
- Compare performance dengan JSON

### Bonus 4: Custom Protocol (20 pts)
Implement custom TCP/UDP protocol untuk low-latency:
- Raw socket communication
- Custom binary protocol
- Compare dengan MQTT

### Bonus 5: Monitoring Dashboard (10 pts)
Simple web dashboard untuk monitor communication:
- Message count per topic
- Latency graph (real-time)
- Connection status
- Error log

**Total Bonus**: 80 pts

---

## 📊 GRADING RUBRIC

### Functionality (40%)
| Criteria | Points | Description |
|----------|--------|-------------|
| MQTT broker setup | 5 | Broker running, accessible |
| Firmware publishing | 10 | Multi-topic, correct format, 10Hz |
| ROS2 bridge working | 10 | Subscribe MQTT, publish ROS2 |
| ACK & retry | 10 | Reliable delivery, timeout handling |
| Multi-device | 5 | 2+ motor, 3+ sensor working |

### Performance (20%)
| Criteria | Points | Description |
|----------|--------|-------------|
| Latency < 50ms avg | 10 | Excellent performance |
| Latency 50-100ms | 7 | Good performance |
| Latency > 100ms | 4 | Acceptable |
| Success rate > 99% | 10 | Reliable |

### Code Quality (20%)
| Criteria | Points | Description |
|----------|--------|-------------|
| Modular design | 5 | Clean separation of concerns |
| Error handling | 5 | Comprehensive error handling |
| Documentation | 5 | Comments, README, docs |
| Code style | 5 | Consistent, readable |

### Documentation (10%)
| Criteria | Points | Description |
|----------|--------|-------------|
| Protocol spec | 3 | Complete, clear |
| Architecture diagram | 2 | Visual, informative |
| Troubleshooting guide | 3 | Practical, helpful |
| Performance report | 2 | Data-driven analysis |

### Video Demo (10%)
| Criteria | Points | Description |
|----------|--------|-------------|
| System explanation | 3 | Clear, comprehensive |
| Live demonstration | 4 | All features working |
| Troubleshooting demo | 3 | Error recovery shown |

### Bonus Features (max 20%)
- Bonus features can add up to 20% extra points

**Total**: 100% + 20% bonus = **120% maximum**

---

## 📅 MILESTONES & TIMELINE

### Week 1: Foundation
- **Day 1-2**: Protocol selection, message design
- **Day 3-4**: MQTT broker setup, basic firmware publisher
- **Day 5-6**: ROS2 bridge basic implementation
- **Day 7**: Testing & debugging

**Deliverable**: Basic MQTT communication working

### Week 2: Reliability
- **Day 1-2**: ACK mechanism implementation
- **Day 3-4**: Retry logic & timeout handling
- **Day 5-6**: Connection state management
- **Day 7**: Testing & debugging

**Deliverable**: Reliable bidirectional communication

### Week 3: Optimization & Documentation
- **Day 1-2**: Latency measurement & optimization
- **Day 3**: Multi-device implementation
- **Day 4-5**: Documentation writing
- **Day 6**: Video recording & editing
- **Day 7**: Final testing & submission

**Deliverable**: Complete project submitted

---

## 🧪 TESTING REQUIREMENTS

### Test 1: Basic Communication
```bash
# Test sensor data flow
mosquitto_sub -h localhost -t 'robot/sensor/#'
ros2 topic echo /robot/sensor/imu
```
**Pass Criteria**: Data arriving at both MQTT and ROS2, 10Hz rate

### Test 2: Command ACK
```python
# Send 10 commands, verify all acknowledged
success_count = 0
for i in range(10):
    if send_command_with_retry({'type': 'motor_command', ...}):
        success_count += 1
assert success_count == 10
```
**Pass Criteria**: 100% success rate

### Test 3: Timeout & Retry
```python
# Disconnect ESP32, send command, verify retry
# Reconnect ESP32 during retry window
# Verify command eventually succeeds
```
**Pass Criteria**: Command succeeds after reconnection

### Test 4: Connection Recovery
```bash
# Stop mosquitto broker
sudo systemctl stop mosquitto

# Verify bridge detects disconnection

# Restart broker
sudo systemctl start mosquitto

# Verify bridge auto-reconnects
```
**Pass Criteria**: Auto-reconnect dalam < 10 detik

### Test 5: Latency Performance
```python
stats = measure_latency(num_samples=100)
assert stats['avg'] < 50  # Target < 50ms
assert stats['max'] < 100  # Requirement < 100ms
```
**Pass Criteria**: Meet latency targets

### Test 6: Multi-Device
```bash
# Verify all topics publishing
mosquitto_sub -h localhost -v -t 'robot/#'

# Verify state aggregator merging data
ros2 run robot_bridge state_aggregator
```
**Pass Criteria**: All devices publishing, aggregation correct

### Test 7: Load Test
```python
# Publish 100 messages/second for 60 seconds
# Measure success rate and latency under load
```
**Pass Criteria**: Success rate > 95%, latency stable

### Test 8: CRC Validation
```python
# Send message dengan incorrect CRC
# Verify message rejected
```
**Pass Criteria**: Invalid CRC detected and logged

---

## 📝 SUBMISSION REQUIREMENTS

### 1. GitHub Repository
URL: _________________________________

**Required**:
- All source code (firmware + ROS2)
- Documentation (markdown files)
- Test scripts
- README dengan clear setup instructions
- .gitignore (exclude secrets.h, build files)

### 2. Documentation Package
**Files to submit**:
- `protocol_spec.md` (min 5 pages)
- `message_format.md` dengan JSON schemas
- `architecture.md` dengan diagrams
- `performance_report.md` dengan graphs
- `troubleshooting.md`
- `user_guide.md`

### 3. Video Demonstration
**Platform**: YouTube (unlisted)  
**Duration**: 10-15 minutes  
**Quality**: 720p minimum  

**Structure**:
1. Introduction (1 min)
2. Architecture overview (2 min)
3. Code walkthrough (3 min)
4. Live demo (5 min)
5. Performance test (2 min)
6. Troubleshooting (2 min)
7. Conclusion (1 min)

### 4. Test Results
**File**: `test_results.md`

Include:
- All test cases executed
- Pass/Fail status
- Screenshots/logs
- Performance metrics
- Issues found & resolved

### 5. Presentation Slides
**Format**: PDF  
**Pages**: 15-20  

**Content**:
- Problem statement
- Solution approach
- Architecture
- Implementation highlights
- Test results
- Challenges & solutions
- Future improvements

---

## 💼 PROJECT MANAGEMENT

### Team Roles:

#### Role 1: Firmware Developer
**Responsibilities**:
- ESP32 firmware implementation
- MQTT client setup
- Sensor data publisher
- Command handler & ACK

**Deliverables**:
- Firmware code
- Testing di hardware
- Firmware documentation

#### Role 2: ROS2 Developer
**Responsibilities**:
- ROS2 bridge node
- ACK manager
- State aggregator
- ROS2 integration

**Deliverables**:
- ROS2 package
- Integration testing
- ROS2 documentation

#### Role 3: Test & Documentation
**Responsibilities**:
- Test script development
- Performance measurement
- Documentation writing
- Video production

**Deliverables**:
- Test suite
- Performance report
- Documentation
- Video demo

### Communication:
- **Daily standup**: 15 min (progress, blocker, plan)
- **Weekly review**: 30 min (demo, feedback, planning)
- **Tools**: Discord/Slack, GitHub Issues, Trello/Notion

---

## 🚨 COMMON PITFALLS & TIPS

### Pitfall 1: WiFi Connection Issues
**Problem**: ESP32 tidak connect ke WiFi  
**Solution**: 
- Verify SSID & password correct
- Check WiFi signal strength
- Use 2.4GHz network (ESP32 tidak support 5GHz)
- Add retry dengan delay

### Pitfall 2: MQTT Broker Not Accessible
**Problem**: ESP32 tidak bisa connect ke broker  
**Solution**:
- Check broker IP address (use `ifconfig` di Ubuntu)
- Verify firewall tidak block port 1883
- Test dengan mosquitto_pub dari ESP32 network

### Pitfall 3: Message Parse Error
**Problem**: JSON parse failed di firmware  
**Solution**:
- Check message size < StaticJsonDocument size
- Verify JSON syntax valid
- Use ArduinoJson Assistant untuk calculate size

### Pitfall 4: ACK Timeout
**Problem**: ACK tidak received, retry terus  
**Solution**:
- Increase timeout value (1 → 2 second)
- Check firmware processing time
- Verify ACK topic subscription
- Debug dengan mosquitto_sub monitor ACK topic

### Pitfall 5: High Latency
**Problem**: Latency > 100ms  
**Solution**:
- Reduce message size
- Check network quality (ping test)
- Optimize JSON parsing
- Consider binary format

---

## 📚 RESOURCES

### Sample Code Repository:
- https://github.com/ros2/examples (ROS2 examples)
- https://github.com/knolleary/pubsubclient (MQTT Arduino)

### Documentation:
- [MQTT Protocol](https://mqtt.org/)
- [Paho MQTT Python](https://www.eclipse.org/paho/clients/python/)
- [ArduinoJson](https://arduinojson.org/)
- [ROS2 Python Client](https://docs.ros.org/en/rolling/Tutorials/Beginner-Client-Libraries.html)

### Tools:
- MQTT Explorer (desktop client)
- Wireshark (network analysis)
- PlotJuggler (ROS2 data visualization)

---

## ✅ FINAL CHECKLIST

### Before Submission:
- ☐ All code tested di hardware (bukan hanya simulasi)
- ☐ Latency < 100ms verified
- ☐ Success rate > 99% verified
- ☐ Multi-device working (2+ motor, 3+ sensor)
- ☐ Error recovery tested (disconnect & reconnect)
- ☐ Documentation complete & clear
- ☐ Code di GitHub dengan good README
- ☐ Video uploaded ke YouTube
- ☐ All tests passed dan documented
- ☐ Presentation slides ready
- ☐ Test results compiled

### Code Quality Check:
- ☐ No hardcoded values (use config file)
- ☐ Error handling di all network calls
- ☐ Logging informative (level: debug, info, warn, error)
- ☐ Comments explain WHY, not just WHAT
- ☐ Function names descriptive
- ☐ No magic numbers
- ☐ Secrets NOT committed to Git

### Documentation Check:
- ☐ README has clear setup steps
- ☐ All diagrams rendered correctly
- ☐ Code examples tested dan working
- ☐ Troubleshooting guide practical
- ☐ Performance report has actual data
- ☐ No broken links

---

## 🏆 SUCCESS CRITERIA

Project dianggap **EXCELLENT** jika:
- ✅ All functional requirements implemented
- ✅ Latency average < 30ms
- ✅ Success rate 100% (dalam 100 test)
- ✅ Clean, modular code
- ✅ Comprehensive documentation
- ✅ Professional video demo
- ✅ At least 1 bonus feature implemented

Project dianggap **GOOD** jika:
- ✅ All functional requirements implemented
- ✅ Latency average < 50ms
- ✅ Success rate > 99%
- ✅ Working code dengan minimal bugs
- ✅ Basic documentation complete
- ✅ Video demo clear

Project **NEEDS IMPROVEMENT** jika:
- ❌ Missing core functionality
- ❌ Latency > 100ms
- ❌ Success rate < 95%
- ❌ Poor code quality
- ❌ Insufficient documentation

---

## 📞 SUPPORT

**Questions?**
- Check troubleshooting guide first
- Search GitHub Issues
- Ask di Discord/Slack
- Office hours: [schedule]

**Emergency Contact**:
- Asisten Lab: [email/phone]

---

**Good luck with your project! 🚀**

---

**Last Updated**: February 2026  
**Module**: 11 - ESP32 ↔ ROS2 Bridge  
**Type**: Project Specification
