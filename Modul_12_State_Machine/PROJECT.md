# PROJECT MODUL 12
## ROBOT STATE MACHINE CONTROLLER

---

## 📋 INFORMASI PROJECT

**Nama Project**: Comprehensive Robot State Machine Controller  
**Durasi**: 2 minggu  
**Tipe**: Individual / Group (max 2 orang)  
**Bobot**: 25% dari nilai Batch 4

---

## 🎯 TUJUAN PROJECT

Mahasiswa mengimplementasikan **complete state machine controller** untuk robot manipulator dengan:

1. **Multi-level hierarchy**: Super-states dan sub-states
2. **Event-driven architecture**: Async event queue dengan prioritization
3. **Safety system**: Guard conditions dan fault recovery
4. **ROS2 integration**: State control via ROS2 topics/services
5. **Monitoring**: Real-time state visualization dan logging
6. **Testing**: Comprehensive unit dan integration tests

---

## 🏗 ARSITEKTUR SISTEM

```
┌─────────────────────────────────────────────────────────────┐
│                     WEB DASHBOARD                           │
│              (Modul 13 - Optional Integration)              │
└────────────────────────┬────────────────────────────────────┘
                         │ HTTP/WebSocket
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                  ROS2 STATE MACHINE NODE                    │
│  ┌─────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │Event Queue  │  │State Machine │  │Safety Monitor│      │
│  │(Priority)   │─→│  Controller  │←─│   (Watchdog) │      │
│  └─────────────┘  └──────────────┘  └──────────────┘      │
│         ▲                │                    │             │
│         │                ▼                    ▼             │
│    ROS2 Topics      State Logger      Fault Handler        │
└─────────┬───────────────┬────────────────────┬─────────────┘
          │               │                    │
          │               │                    ▼
          │               │         ┌────────────────────┐
          │               │         │  Error Recovery    │
          │               │         └────────────────────┘
          │               │
          │               ▼
          │      ┌─────────────────┐
          │      │   InfluxDB      │ (Modul 14 Integration)
          │      │  Time-series DB │
          │      └─────────────────┘
          │
          ▼
┌─────────────────────────────────────────────────────────────┐
│              ROBOT HARDWARE / SIMULATION                    │
│  (ESP32 via MQTT atau Gazebo/RViz Simulation)              │
└─────────────────────────────────────────────────────────────┘
```

---

## 📊 FUNCTIONAL REQUIREMENTS

### FR-1: State Definitions (Bobot: 10%)

Sistem harus implement **minimal 8 states**:

#### Level 1: Super-States
1. **OFFLINE**: System powered off atau disconnected
2. **OPERATIONAL**: Normal operation mode
3. **MAINTENANCE**: Maintenance atau calibration mode
4. **EMERGENCY**: Emergency stop condition

#### Level 2: OPERATIONAL Sub-States
5. **IDLE**: Standby, motors disabled
6. **HOMING**: Finding home position
7. **READY**: Ready untuk command
8. **MOVING**: Executing trajectory
9. **WORKING**: Performing task (pick, place, weld, etc)

#### State Specifications:

**IDLE**
- Entry action: Disable motors, reset error flags
- Exit action: Log start timestamp
- Valid events: `start_command`, `shutdown`
- Transitions to: HOMING, OFFLINE

**HOMING**
- Entry action: Enable motors, start homing sequence
- Exit action: Save home position, publish ready status
- Valid events: `reached_home`, `homing_timeout`, `emergency_stop`
- Transitions to: READY, ERROR
- Timeout: 30 seconds

**READY**
- Entry action: Publish availability status
- Valid events: `move_command`, `task_command`, `return_idle`, `emergency_stop`
- Transitions to: MOVING, WORKING, IDLE, EMERGENCY
- Guard conditions:
  - move_command: `motors_enabled AND path_clear AND no_error`
  - task_command: `tool_ready AND workspace_clear`

**MOVING**
- Entry action: Start trajectory execution, log target
- Exit action: Stop motors, log completion
- Valid events: `reached_target`, `path_blocked`, `abort_command`, `emergency_stop`
- Transitions to: READY, WORKING, EMERGENCY
- Timeout: Based on trajectory duration + 20%

**WORKING**
- Entry action: Execute task sequence (pick/place/weld/etc)
- Exit action: Log task result, cleanup
- Valid events: `task_complete`, `task_failed`, `emergency_stop`
- Transitions to: IDLE, MOVING, EMERGENCY
- Timeout: Based on task type

**MAINTENANCE**
- Entry action: Disable all safety features, enter manual mode
- Valid events: `calibration_command`, `maintenance_complete`, `shutdown`
- Transitions to: OPERATIONAL (IDLE), OFFLINE

**EMERGENCY**
- Entry action: Emergency stop all motors, trigger alarm, save state
- Valid events: `reset_command`
- Transitions to: MAINTENANCE (after fault cleared)
- Guard for reset: `fault_cleared AND operator_acknowledged`

---

### FR-2: Event System (Bobot: 15%)

#### Event Queue Implementation

**Requirements**:
- Priority queue (0=highest, 5=lowest)
- Thread-safe implementation
- Max queue size: 200 events
- Overflow handling: Drop lowest priority events

**Event Structure**:
```python
{
    'type': 'move_command',
    'timestamp': 1234567890.123,
    'priority': 1,
    'source': 'user_interface',
    'data': {
        'target': [100, 50, 20],
        'speed': 1.0,
        'acceleration': 0.5
    },
    'correlation_id': 'uuid-12345'
}
```

**Event Types**:

| Category | Events | Priority |
|----------|--------|----------|
| Emergency | emergency_stop, fault_detected | 0 |
| System | timeout, state_entered, state_exited | 1 |
| Control | move_command, task_command, abort | 2 |
| Status | reached_target, task_complete | 3 |
| User | start_command, stop_command | 4 |
| Monitor | status_request, ping | 5 |

**Event Sources**:
- ROS2 topics/services
- MQTT messages (dari ESP32)
- Internal timers
- Safety watchdog
- User interface (CLI/GUI/Web)

---

### FR-3: Transition Logic (Bobot: 15%)

#### Transition Table Implementation

**Requirements**:
- Config-driven transitions (YAML file)
- Runtime validation
- Transition logging
- Guard condition support

**Example Transition Config** (`transitions.yaml`):
```yaml
transitions:
  - from: IDLE
    to: HOMING
    event: start_command
    guard: motors_enabled
    action: log_start
    
  - from: HOMING
    to: READY
    event: reached_home
    guard: all_joints_homed
    action: save_home_position
    
  - from: READY
    to: MOVING
    event: move_command
    guards:
      - motors_enabled
      - path_clear
      - target_valid
      - battery_sufficient
    actions:
      - validate_trajectory
      - start_motion_planner
      
  - from: ANY
    to: EMERGENCY
    event: emergency_stop
    action: emergency_stop_all
```

**Guard Functions**:
```python
def guard_motors_enabled(context):
    return context.get('motors_enabled', False)

def guard_path_clear(context):
    # Check sensor data, collision detection
    return not context.get('obstacle_detected', False)

def guard_target_valid(event, context):
    target = event.get('data', {}).get('target', [])
    return validate_workspace_limits(target)

def guard_battery_sufficient(context):
    return context.get('battery_level', 0) > 20
```

---

### FR-4: Safety System (Bobot: 15%)

#### Safety Requirements

**1. Emergency Stop**
- Dari ANY state → EMERGENCY
- Max response time: 50ms
- Hardware E-stop integration (optional)

**2. Watchdog Timer**
- Monitor state machine heartbeat
- Timeout: 1 second
- Action: Trigger emergency stop

**3. Fault Detection**
```python
fault_types = {
    'SENSOR_DISCONNECTED': ('warning', 'use_last_value'),
    'MOTOR_OVERHEAT': ('critical', 'emergency_stop'),
    'COMMUNICATION_LOST': ('warning', 'retry_connection'),
    'POSITION_ERROR': ('critical', 'stop_and_recalibrate'),
    'UNEXPECTED_STATE': ('critical', 'emergency_stop')
}
```

**4. Recovery Procedures**
- Automatic recovery untuk warning-level faults
- Operator intervention untuk critical faults
- Recovery state machine:
  ```
  FAULT_DETECTED → DIAGNOSING → RECOVERING → VALIDATING → RESOLVED
  ```

**5. Safety Constraints**
- Velocity limits: max 1.5 m/s
- Acceleration limits: max 2.0 m/s²
- Workspace boundaries: defined in config
- Joint limits: soft limits (warning) + hard limits (emergency)

---

### FR-5: ROS2 Integration (Bobot: 15%)

#### ROS2 Interfaces

**Topics**:

| Topic | Type | Direction | Description | QoS |
|-------|------|-----------|-------------|-----|
| `/robot/state` | `std_msgs/String` | Publish | Current state (JSON) | Reliable |
| `/robot/command` | `std_msgs/String` | Subscribe | Commands (JSON) | Reliable |
| `/robot/events` | `std_msgs/String` | Publish | Event log | Best Effort |
| `/robot/diagnostics` | `diagnostic_msgs/DiagnosticArray` | Publish | Health status | Reliable |

**Services**:

| Service | Type | Description |
|---------|------|-------------|
| `/robot/transition` | `std_srvs/SetBool` | Force state transition |
| `/robot/reset` | `std_srvs/Trigger` | Reset state machine |
| `/robot/get_state` | Custom | Get current state info |

**Custom Message Types**:

`StateInfo.msg`:
```
string current_state
string[] available_events
string[] blocked_events
KeyValue[] context_data
time last_transition
float64 uptime
```

`StateCommand.msg`:
```
string command_type
string target_state
KeyValue[] parameters
uint8 priority
```

---

### FR-6: Logging & Monitoring (Bobot: 10%)

#### Logging Requirements

**1. State Transition Log**
- Format: CSV atau JSON lines
- Fields: timestamp, from_state, to_state, event, duration_ms, success
- Rotation: Setiap 1000 transitions atau 10MB

**2. Event Log**
- All events received dan processed
- Include: source, priority, processing_time

**3. Performance Metrics**
```python
metrics = {
    'event_processing_time_ms': [],  # Histogram
    'state_duration_sec': {},         # Per-state
    'transition_count': {},           # Per-transition
    'queue_size_max': 0,
    'events_dropped': 0,
    'fault_count': {}
}
```

**4. Real-time Monitoring**
- ROS2 topic: `/robot/metrics` (published every 1 second)
- Include: current state, queue size, CPU usage, memory

---

### FR-7: Testing (Bobot: 10%)

#### Test Coverage Requirements

**1. Unit Tests** (min 80% code coverage)
- All state classes
- Event queue
- Guard functions
- Transition logic

**2. Integration Tests**
- Complete operation sequences
- Error recovery flows
- ROS2 communication
- Concurrent events

**3. Scenario Tests**
```python
scenarios = [
    'normal_operation_cycle',      # IDLE → HOMING → READY → MOVING → WORKING → IDLE
    'emergency_stop_from_moving',  # MOVING → EMERGENCY → MAINTENANCE → IDLE
    'homing_timeout_recovery',     # HOMING timeout → ERROR → reset → IDLE
    'path_blocked_during_move',    # MOVING → detect obstacle → READY
    'rapid_command_sequence',      # Multiple commands queued
    'concurrent_emergency_stops'   # Multiple E-stop sources
]
```

**4. Performance Tests**
- Event processing latency: < 10ms (p95)
- State transition time: < 50ms
- Queue handling: 100 events/sec

---

### FR-8: Configuration (Bobot: 5%)

#### Config Files

**1. `states_config.yaml`**
```yaml
states:
  IDLE:
    timeout_sec: null
    allowed_events: [start_command, shutdown]
    entry_actions: [disable_motors, reset_errors]
    exit_actions: [log_start_time]
  
  HOMING:
    timeout_sec: 30
    allowed_events: [reached_home, abort, emergency_stop]
    entry_actions: [enable_motors, start_homing_sequence]
    exit_actions: [save_home_position]
```

**2. `safety_config.yaml`**
```yaml
safety:
  velocity_limit_m_s: 1.5
  acceleration_limit_m_s2: 2.0
  workspace_bounds:
    x: [-300, 300]
    y: [-300, 300]
    z: [0, 400]
  
  watchdog_timeout_sec: 1.0
  emergency_stop_response_ms: 50
```

---

### FR-9: Visualization (Bobot: 5%)

#### State Diagram Generation

**Requirements**:
- Auto-generate diagram dari config
- Runtime state highlighting
- Transition frequency overlay

**Tools**: Graphviz, PlantUML

**Output**:
- `state_diagram.png`: Static diagram
- `state_runtime.html`: Interactive visualization dengan D3.js

---

## 🎁 BONUS FEATURES (Max +20%)

### Bonus 1: Web Dashboard (+8%)
- Real-time state visualization
- Command interface
- Transition history graph
- Metrics dashboard

### Bonus 2: State Persistence (+5%)
- Save state on shutdown
- Restore state on startup
- Checkpoint/rollback functionality

### Bonus 3: Simulation Integration (+5%)
- Gazebo simulation
- Visualize robot movement
- Collision detection

### Bonus 4: Machine Learning Integration (+7%)
- Predict next state
- Anomaly detection
- Optimize transition timing

---

## 📦 DELIVERABLES

### 1. Source Code (40%)

**Required Files**:
```
robot_state_machine/
├── robot_state_machine/
│   ├── __init__.py
│   ├── states/
│   │   ├── __init__.py
│   │   ├── base_state.py
│   │   ├── idle_state.py
│   │   ├── homing_state.py
│   │   ├── ready_state.py
│   │   ├── moving_state.py
│   │   ├── working_state.py
│   │   ├── emergency_state.py
│   │   └── maintenance_state.py
│   ├── state_machine_controller.py
│   ├── event_queue.py
│   ├── guards.py
│   ├── actions.py
│   ├── safety_monitor.py
│   ├── ros2_state_node.py
│   └── config_loader.py
├── config/
│   ├── states_config.yaml
│   ├── transitions.yaml
│   └── safety_config.yaml
├── tests/
│   ├── test_states.py
│   ├── test_state_machine.py
│   ├── test_event_queue.py
│   ├── test_guards.py
│   └── test_integration.py
├── launch/
│   └── state_machine.launch.py
├── package.xml
├── setup.py
├── README.md
└── requirements.txt
```

### 2. Documentation (25%)

**Required Documents**:
- `README.md`: Installation, usage, architecture
- `ARCHITECTURE.md`: Detailed system design
- `API.md`: ROS2 interfaces, function signatures
- `STATE_DIAGRAM.png`: Visual state diagram
- `TESTING.md`: Test results, coverage report

### 3. Test Results (15%)

**Test Report**:
- Unit test results (pytest output)
- Integration test scenarios
- Performance benchmarks
- Coverage report (min 80%)

### 4. Video Demo (15%)

**Requirements**:
- Duration: 5-8 minutes
- Show:
  1. Normal operation cycle
  2. Emergency stop scenario
  3. Error recovery
  4. ROS2 command interface
  5. Monitoring dashboard
- Upload ke YouTube/Drive

### 5. Presentation (5%)

**Slides**:
- Problem statement
- Architecture overview
- Key implementation details
- Demo
- Challenges & solutions
- Q&A

---

## 🧪 TESTING REQUIREMENTS

### Test Scenarios

#### Scenario 1: Normal Operation
```
IDLE → (start_command) → HOMING → (reached_home) → READY → 
(move_command) → MOVING → (reached_target) → READY → (task_command) → 
WORKING → (task_complete) → IDLE
```

**Expected**:
- All transitions successful
- Timing within limits
- No errors logged

#### Scenario 2: Emergency Stop
```
MOVING → (emergency_stop) → EMERGENCY → (reset_command, fault_cleared) → 
MAINTENANCE → (maintenance_complete) → IDLE
```

**Expected**:
- Emergency stop within 50ms
- State saved before stop
- Proper recovery sequence

#### Scenario 3: Timeout Handling
```
IDLE → (start_command) → HOMING → (30sec timeout) → ERROR → 
(reset_command) → IDLE
```

**Expected**:
- Timeout triggered correctly
- Error logged
- Safe recovery

#### Scenario 4: Guard Rejection
```
READY → (move_command, path_blocked=True) → READY (no transition)
```

**Expected**:
- Transition blocked
- Warning logged
- State unchanged

#### Scenario 5: Concurrent Events
```
Queue: [move_command, emergency_stop, status_request]
Processing order: emergency_stop (p0) → move_command (p2) → status_request (p5)
```

**Expected**:
- Priority respected
- Emergency handled first
- No event loss

---

## 📏 RUBRIK PENILAIAN

| Kriteria | Excellent (90-100) | Good (75-89) | Adequate (60-74) | Poor (<60) |
|----------|-------------------|--------------|------------------|------------|
| **State Implementation** | 8+ states, proper hierarchy, clean code | 8 states, working | 6-7 states | < 6 states |
| **Event System** | Priority queue, thread-safe, robust | Priority queue works | Basic queue | No queue |
| **Transitions** | Config-driven, guards, logging | Hardcoded, works | Basic if-else | Buggy |
| **Safety** | E-stop, watchdog, recovery | E-stop works | Basic safety | No safety |
| **ROS2 Integration** | Topics, services, custom msgs | Topics work | Basic pub/sub | No ROS2 |
| **Testing** | >80% coverage, all scenarios | >60% coverage | Basic tests | No tests |
| **Documentation** | Complete, professional | Good docs | Basic README | Minimal |
| **Code Quality** | Clean, documented, modular | Good structure | Works but messy | Spaghetti |
| **Demo** | Polished, comprehensive | Good demo | Basic demo | Broken |

---

## 📅 TIMELINE

| Week | Milestone | Deliverable |
|------|-----------|-------------|
| 1 | Design | State diagram, architecture doc |
| 1 | Core Implementation | Basic state machine working |
| 2 | Event System | Queue, priority, threading |
| 2 | ROS2 Integration | Topics/services working |
| 2 | Safety | E-stop, guards, watchdog |
| 3 | Testing | Unit + integration tests |
| 3 | Documentation | Complete docs |
| 3 | Final | Video, presentation, submission |

---

## 🚀 GETTING STARTED

### Step 1: Setup
```bash
cd ~/ros2_ws/src
ros2 pkg create robot_state_machine --build-type ament_python
cd robot_state_machine
mkdir -p robot_state_machine/states config tests launch
```

### Step 2: Implement Base State
```python
# robot_state_machine/states/base_state.py
from abc import ABC, abstractmethod

class State(ABC):
    def __init__(self, name):
        self.name = name
    
    @abstractmethod
    def on_enter(self, context):
        pass
    
    @abstractmethod
    def on_exit(self, context):
        pass
    
    @abstractmethod
    def handle_event(self, event, context):
        pass
```

### Step 3: Create Config
```yaml
# config/states_config.yaml
states:
  IDLE:
    timeout_sec: null
    allowed_events: [start_command]
  # ... add more states
```

### Step 4: Build & Test
```bash
colcon build --packages-select robot_state_machine
pytest tests/
```

---

## 💡 TIPS

1. **Start Simple**: Implement 3 states first (IDLE, READY, MOVING), then expand
2. **Test Early**: Write tests alongside implementation
3. **Use Logging**: Extensive logging helps debugging
4. **Config-Driven**: Avoid hardcoding - use YAML configs
5. **Git Workflow**: Commit often, use branches
6. **Ask Questions**: Konsultasi dengan asisten jika stuck

---

## 🔗 RESOURCES

### Code Examples:
- `examples/simple_state_machine.py`
- `examples/ros2_state_node.py`
- `examples/event_queue_example.py`

### Libraries:
- `transitions`: Python state machine library
- `rclpy`: ROS2 Python client library
- `pytest`: Testing framework
- `pyyaml`: YAML parser

### Documentation:
- ROS2 Lifecycle: https://design.ros2.org/articles/node_lifecycle.html
- Python threading: https://docs.python.org/3/library/threading.html
- pytest: https://docs.pytest.org/

---

## 📝 SUBMISSION

**Method**: Git repository (GitHub/GitLab)

**Repository Structure**:
```
README.md
ARCHITECTURE.md
robot_state_machine/  (ROS2 package)
docs/
  API.md
  TESTING.md
  STATE_DIAGRAM.png
video/
  demo.mp4 (atau link YouTube)
presentation/
  slides.pdf
```

**Deadline**: 3 minggu dari assignment  
**Late Penalty**: -10% per hari

---

## 🎓 PENILAIAN AKHIR

**Total Points**: 100 + 20 (bonus)

**Breakdown**:
- Source Code: 40%
- Documentation: 25%
- Testing: 15%
- Demo Video: 15%
- Presentation: 5%
- Bonus Features: +20%

**Grading Scale**:
- A: ≥ 85
- B: 70-84
- C: 60-69
- D: 50-59
- E: < 50

---

**Good luck! 🚀**

---

**Contact**:
- **Dosen**: [Email]
- **Asisten**: [Email]
- **Office Hours**: [Schedule]

