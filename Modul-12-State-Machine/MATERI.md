# MODUL 12: STATE MACHINE & SEQUENCING
## ROBOT OPERATION LOGIC

---

## 📋 TUJUAN PEMBELAJARAN

Setelah menyelesaikan modul ini, mahasiswa mampu:
1. Memahami konsep finite state machine (FSM) untuk robot control
2. Merancang state diagram dan transition table
3. Mengimplementasikan event-driven architecture
4. Menggunakan guard conditions untuk safety
5. Mengimplementasikan hierarchical state machine
6. Menangani fault detection dan recovery
7. Testing state machine secara comprehensive

---

## 1. DEFINISI

### State Machine (Finite State Machine - FSM)
**State machine** adalah model komputasi abstract yang terdiri dari:
- **Finite number of states**: Sejumlah terbatas kondisi/mode operasi
- **Transitions**: Aturan perpindahan antar state
- **Events**: Trigger yang menyebabkan transition
- **Actions**: Operasi yang dieksekusi saat transition atau dalam state

### State
**State** merepresentasikan kondisi atau mode operasi sistem pada waktu tertentu. Dalam konteks robot:
- **IDLE**: Robot standby, motor disabled
- **HOMING**: Robot mencari home position
- **READY**: Robot siap menerima command
- **MOVING**: Robot executing trajectory
- **WORKING**: Robot melakukan task (pick, place, etc)
- **ERROR**: Robot dalam kondisi fault

### Event
**Event** adalah kejadian yang dapat memicu state transition:
- **External events**: User command, sensor trigger
- **Internal events**: Timer timeout, computation complete
- **System events**: Error detected, resource available

### Transition
**Transition** adalah perpindahan dari satu state ke state lain, triggered by event dan optional guard condition.

---

## 2. KONSEP INTI

### 2.1 Moore vs Mealy Machine

#### Moore Machine
**Output bergantung HANYA pada current state**

```
State: MOVING
Output: motor_enabled = TRUE (selalu, selama di state MOVING)
```

**Characteristics**:
- Output stable selama di state
- Easier to analyze
- Simpler implementation

#### Mealy Machine
**Output bergantung pada current state DAN input event**

```
State: READY + Event: move_command → Output: start_trajectory()
State: READY + Event: stop_command → Output: disable_motors()
```

**Characteristics**:
- More flexible
- Faster response (output saat transition)
- Lebih compact (fewer states)

### 2.2 State Diagram Notation

```
┌─────────────┐
│    IDLE     │  ← State (circle/rectangle)
└─────────────┘
      │
      │ start_command [motor_enabled] / enable_motors()
      │    ↑              ↑                    ↑
      │  Event         Guard               Action
      ▼
┌─────────────┐
│   HOMING    │
└─────────────┘
```

**Components**:
- **Circle/Box**: State
- **Arrow**: Transition
- **Label format**: `event [guard] / action`

### 2.3 Hierarchical State Machine

**Hierarchical state** memungkinkan state mengandung sub-state machine, mengurangi complexity untuk sistem besar.

```
┌─────────────────────────────────────┐
│          OPERATIONAL                │  ← Super-state
│  ┌────────┐    ┌────────┐           │
│  │  IDLE  │───→│ MOVING │           │  ← Sub-states
│  └────────┘    └────────┘           │
└─────────────────────────────────────┘
         │
         │ emergency_stop
         ▼
┌─────────────────────────────────────┐
│           ERROR                     │
└─────────────────────────────────────┘
```

**Benefits**:
- Reduce diagram complexity
- Group related states
- Share transitions (emergency_stop dari any sub-state)

### 2.4 Event Queue

**Event queue** adalah FIFO (First In First Out) buffer untuk store events.

```python
from queue import Queue

event_queue = Queue()

# Producer: post events
event_queue.put({'type': 'start_command', 'motor_enabled': True})
event_queue.put({'type': 'sensor_trigger', 'value': 100})

# Consumer: process events
while not event_queue.empty():
    event = event_queue.get()
    state_machine.process_event(event)
```

**Advantages**:
- **Asynchronous processing**: Events tidak block sender
- **Ordered execution**: Events processed dalam order
- **Prevent race condition**: Sequential processing
- **Buffering**: Handle burst events

### 2.5 Guard Conditions

**Guard** adalah boolean condition yang harus TRUE untuk transition execute.

```python
# Transition: READY → MOVING
# Guard: path_clear AND motor_enabled AND no_error

def can_transition_to_moving(context):
    return (context['path_clear'] and 
            context['motor_enabled'] and 
            not context['has_error'])

# Transition only if guard returns True
if can_transition_to_moving(robot_context):
    transition_to(MOVING)
```

**Use Cases**:
- **Safety check**: Pastikan safe sebelum motor move
- **Resource check**: Pastikan resource available
- **Precondition**: Pastikan prerequisite terpenuhi

### 2.6 Entry/Exit Actions

**Actions** dapat execute pada:
1. **Entry action**: Saat entering state
2. **Exit action**: Saat leaving state
3. **Transition action**: During transition

```python
class MovingState(State):
    def on_enter(self):
        """Execute when entering MOVING state"""
        self.start_trajectory()
        self.enable_motors()
        self.log_event("Entered MOVING state")
    
    def on_exit(self):
        """Execute when leaving MOVING state"""
        self.stop_motors()
        self.log_event("Exited MOVING state")
```

### 2.7 Internal Transitions

**Internal transition** handle event tanpa change state (no entry/exit action).

```python
class IdleState(State):
    def handle_event(self, event):
        if event['type'] == 'status_request':
            # Internal transition - handle without changing state
            self.send_status_response()
            return None  # Stay in current state
        
        elif event['type'] == 'start_command':
            # External transition - change to HOMING
            return StateType.HOMING
```

---

## 3. ROBOT STATE MACHINE DESIGN

### 3.1 State Definitions

#### IDLE
**Description**: Robot standby, waiting for command  
**Entry action**: Disable motors, reset variables  
**Exit action**: Log start time  
**Valid transitions**: → HOMING, → ERROR

#### HOMING
**Description**: Robot finding home position  
**Entry action**: Start homing sequence, enable motors  
**Exit action**: Save home position  
**Valid transitions**: → READY (success), → ERROR (failure)

#### READY
**Description**: Robot ready untuk command  
**Entry action**: Publish ready status  
**Exit action**: None  
**Valid transitions**: → MOVING, → IDLE, → ERROR

#### MOVING
**Description**: Robot executing trajectory  
**Entry action**: Start trajectory execution  
**Exit action**: Stop motors  
**Valid transitions**: → WORKING (reached), → READY (abort), → ERROR

#### WORKING
**Description**: Robot performing task (pick, place, weld, etc)  
**Entry action**: Start task sequence  
**Exit action**: Log task completion  
**Valid transitions**: → IDLE, → MOVING, → ERROR

#### ERROR
**Description**: Robot dalam fault condition  
**Entry action**: Emergency stop, trigger alarm  
**Exit action**: Clear error flags  
**Valid transitions**: → IDLE (after reset)

### 3.2 Complete State Diagram

```
        ┌─────────────────────┐
        │  POWER ON / RESET   │
        └──────────┬──────────┘
                   │
                   ▼
             ┌─────────┐
        ┌───→│  IDLE   │←──────────┐
        │    └─────────┘           │
        │         │                │
        │         │ start_command  │
        │         │ [motor_enabled]│
        │         ▼                │
        │    ┌─────────┐           │
        │    │ HOMING  │           │
        │    └─────────┘           │
        │         │                │
        │         │ reached_home   │
        │         │ [all_homed]    │
        │         ▼                │
        │    ┌─────────┐           │
        │    │  READY  │           │ task_complete
        │    └─────────┘           │
        │         │                │
        │         │ move_command   │
        │         │ [path_clear]   │
        │         ▼                │
        │    ┌─────────┐           │
        │    │ MOVING  │           │
        │    └─────────┘           │
        │         │                │
        │         │ reached_target │
        │         ▼                │
        │    ┌─────────┐           │
        │    │ WORKING │───────────┘
        │    └─────────┘
        │         │
        │         │ fault_detected (from ANY state)
        │         ▼
        │    ┌─────────┐
        └────│  ERROR  │
             └─────────┘
                   │
                   │ reset_command [fault_cleared]
                   │
                   └──────→ (back to IDLE)
```

### 3.3 Transition Table

| Current State | Event | Guard Condition | Action | Next State |
|---------------|-------|-----------------|--------|------------|
| IDLE | start_command | motor_enabled = TRUE | log("Starting"), publish("ready") | HOMING |
| IDLE | shutdown_command | - | save_state(), power_off() | IDLE |
| HOMING | reached_home | all_joints_homed = TRUE | save_home_pos() | READY |
| HOMING | homing_timeout | - | log_error("Homing timeout") | ERROR |
| READY | move_command | path_clear AND motor_enabled | start_trajectory() | MOVING |
| READY | return_home | - | - | HOMING |
| READY | idle_command | - | - | IDLE |
| MOVING | reached_target | position_error < 0.1 | - | WORKING |
| MOVING | abort_command | - | stop_motors() | READY |
| MOVING | path_blocked | - | emergency_stop() | ERROR |
| WORKING | task_complete | - | log_result() | IDLE |
| WORKING | task_failed | - | log_error() | ERROR |
| ANY | fault_detected | - | emergency_stop(), alarm() | ERROR |
| ERROR | reset_command | fault_cleared = TRUE | reset_flags() | IDLE |

---

## 4. EVENT-DRIVEN ARCHITECTURE

### 4.1 Event Types

```python
from enum import Enum

class EventType(Enum):
    # User commands
    START_COMMAND = "start_command"
    MOVE_COMMAND = "move_command"
    STOP_COMMAND = "stop_command"
    RESET_COMMAND = "reset_command"
    
    # System events
    REACHED_HOME = "reached_home"
    REACHED_TARGET = "reached_target"
    TASK_COMPLETE = "task_complete"
    
    # Error events
    FAULT_DETECTED = "fault_detected"
    TIMEOUT = "timeout"
    PATH_BLOCKED = "path_blocked"
```

### 4.2 Event Structure

```python
event = {
    'type': EventType.MOVE_COMMAND,
    'timestamp': time.time(),
    'data': {
        'target_x': 100.0,
        'target_y': 200.0,
        'speed': 1.0
    },
    'source': 'user_interface',
    'priority': 1  # 0=highest, 5=lowest
}
```

### 4.3 Event Queue Implementation

```python
from queue import PriorityQueue
import threading

class EventQueue:
    def __init__(self):
        self.queue = PriorityQueue()
        self.lock = threading.Lock()
    
    def post_event(self, event, priority=5):
        """Add event to queue"""
        with self.lock:
            self.queue.put((priority, time.time(), event))
    
    def get_event(self, timeout=None):
        """Get next event (blocking)"""
        try:
            priority, timestamp, event = self.queue.get(timeout=timeout)
            return event
        except:
            return None
    
    def has_events(self):
        """Check if queue has events"""
        return not self.queue.empty()
```

---

## 5. FAULT RECOVERY

### 5.1 Fault Types

| Fault Type | Severity | Recovery Action |
|------------|----------|-----------------|
| Sensor disconnected | Warning | Use last known value, log warning |
| Motor overheat | Critical | Emergency stop, cool down |
| Communication timeout | Warning | Retry, switch to safe mode |
| Position error | Critical | Stop, recalibrate |
| Unexpected state | Critical | Emergency stop, reset |

### 5.2 Recovery State Machine

```
┌──────────┐
│  NORMAL  │
└──────────┘
     │
     │ fault_detected
     ▼
┌──────────┐
│ WARNING  │  ← Fault detected, still operational
└──────────┘
     │
     │ fault_persists
     ▼
┌──────────┐
│  ALERT   │  ← Critical fault, emergency stop
└──────────┘
     │
     │ recovery_initiated
     ▼
┌──────────┐
│RECOVERING│  ← Running recovery procedure
└──────────┘
     │
     │ recovery_complete
     ▼
┌──────────┐
│ RESOLVED │  ← Back to normal
└──────────┘
```

### 5.3 Recovery Procedures

```python
class ErrorRecovery:
    def recover_from_motor_error(self):
        """Recovery procedure for motor fault"""
        steps = [
            self.disable_all_motors,
            self.wait_cooldown,
            self.verify_motor_health,
            self.run_homing_sequence,
            self.verify_calibration
        ]
        
        for step in steps:
            if not step():
                return False
        
        return True
```

---

## 6. IMPLEMENTATION PATTERNS

### 6.1 State Pattern (OOP)

```python
from abc import ABC, abstractmethod

class State(ABC):
    """Abstract base class for all states"""
    
    def __init__(self, name):
        self.name = name
    
    @abstractmethod
    def on_enter(self, context):
        """Called when entering this state"""
        pass
    
    @abstractmethod
    def on_exit(self, context):
        """Called when exiting this state"""
        pass
    
    @abstractmethod
    def handle_event(self, event, context):
        """
        Handle event in this state
        Returns: next_state or None to stay
        """
        pass

class IdleState(State):
    def __init__(self):
        super().__init__("IDLE")
    
    def on_enter(self, context):
        print("Entering IDLE state")
        context['motors_enabled'] = False
    
    def on_exit(self, context):
        print("Leaving IDLE state")
    
    def handle_event(self, event, context):
        if event['type'] == 'start_command':
            if context.get('motor_enabled', False):
                return StateType.HOMING
        return None
```

### 6.2 State Machine Controller

```python
class StateMachineController:
    def __init__(self):
        self.states = {}
        self.current_state = None
        self.context = {}
        self.event_queue = EventQueue()
        self.running = False
        
        # History
        self.state_history = []
        self.transition_log = []
    
    def add_state(self, state_type, state_obj):
        """Register a state"""
        self.states[state_type] = state_obj
    
    def set_initial_state(self, state_type):
        """Set starting state"""
        self.current_state = self.states[state_type]
        self.current_state.on_enter(self.context)
    
    def post_event(self, event):
        """Add event to queue"""
        self.event_queue.post_event(event)
    
    def process_next_event(self):
        """Process one event from queue"""
        event = self.event_queue.get_event(timeout=0.1)
        
        if event:
            # Handle event in current state
            next_state_type = self.current_state.handle_event(event, self.context)
            
            # Transition if needed
            if next_state_type:
                self.transition_to(next_state_type, event)
    
    def transition_to(self, next_state_type, event=None):
        """Execute state transition"""
        old_state = self.current_state
        new_state = self.states[next_state_type]
        
        # Log transition
        self.transition_log.append({
            'timestamp': time.time(),
            'from': old_state.name,
            'to': new_state.name,
            'event': event
        })
        
        # Execute transition
        old_state.on_exit(self.context)
        self.current_state = new_state
        new_state.on_enter(self.context)
        
        print(f"Transition: {old_state.name} → {new_state.name}")
    
    def run(self):
        """Main event loop"""
        self.running = True
        
        while self.running:
            self.process_next_event()
            time.sleep(0.01)  # 100 Hz
    
    def stop(self):
        """Stop state machine"""
        self.running = False
```

---

## 7. ADVANCED PATTERNS

### 7.1 Timeout Transitions

Transition yang triggered by timer:

```python
class WaitingState(State):
    def __init__(self, timeout=5.0):
        super().__init__("WAITING")
        self.timeout = timeout
        self.enter_time = None
    
    def on_enter(self, context):
        self.enter_time = time.time()
    
    def handle_event(self, event, context):
        # Check timeout
        if time.time() - self.enter_time > self.timeout:
            return StateType.TIMEOUT_ERROR
        
        if event['type'] == 'response_received':
            return StateType.PROCESSING
        
        return None
```

### 7.2 Concurrent State Machines

Multiple independent state machines running parallel:

```python
class RobotSystem:
    def __init__(self):
        # Main operation state machine
        self.operation_sm = StateMachineController()
        
        # Safety monitor state machine (runs parallel)
        self.safety_sm = StateMachineController()
        
        # Communication state machine
        self.comm_sm = StateMachineController()
    
    def run(self):
        # Run all state machines
        threading.Thread(target=self.operation_sm.run, daemon=True).start()
        threading.Thread(target=self.safety_sm.run, daemon=True).start()
        threading.Thread(target=self.comm_sm.run, daemon=True).start()
```

### 7.3 State Persistence

Save dan restore state untuk recovery after crash:

```python
import pickle

class PersistentStateMachine(StateMachineController):
    def __init__(self, state_file='state.pkl'):
        super().__init__()
        self.state_file = state_file
    
    def save_state(self):
        """Save current state to file"""
        state_data = {
            'current_state': self.current_state.name,
            'context': self.context,
            'timestamp': time.time()
        }
        
        with open(self.state_file, 'wb') as f:
            pickle.dump(state_data, f)
    
    def restore_state(self):
        """Restore state from file"""
        try:
            with open(self.state_file, 'rb') as f:
                state_data = pickle.load(f)
            
            # Restore state
            state_name = state_data['current_state']
            self.context = state_data['context']
            
            # Find state type from name
            for state_type, state in self.states.items():
                if state.name == state_name:
                    self.set_initial_state(state_type)
                    return True
        except FileNotFoundError:
            return False
        
        return False
```

---

## 8. TESTING STATE MACHINES

### 8.1 Unit Testing States

```python
import unittest

class TestIdleState(unittest.TestCase):
    def setUp(self):
        self.state = IdleState()
        self.context = {'motor_enabled': True}
    
    def test_on_enter(self):
        """Test entry action"""
        self.state.on_enter(self.context)
        self.assertFalse(self.context.get('motors_enabled', True))
    
    def test_start_command(self):
        """Test transition to HOMING"""
        event = {'type': 'start_command'}
        next_state = self.state.handle_event(event, self.context)
        self.assertEqual(next_state, StateType.HOMING)
```

### 8.2 Integration Testing

```python
def test_complete_sequence():
    """Test complete operation sequence"""
    sm = StateMachineController()
    # ... setup states ...
    
    # Sequence: IDLE → HOMING → READY → MOVING → WORKING → IDLE
    sm.set_initial_state(StateType.IDLE)
    assert sm.current_state.name == "IDLE"
    
    sm.post_event({'type': 'start_command', 'motor_enabled': True})
    sm.process_next_event()
    assert sm.current_state.name == "HOMING"
    
    sm.post_event({'type': 'reached_home'})
    sm.process_next_event()
    assert sm.current_state.name == "READY"
    
    # ... continue sequence ...
```

### 8.3 Coverage Testing

Pastikan semua transitions tested:

```python
def test_all_transitions():
    """Verify all transitions in transition table"""
    transition_table = [
        (StateType.IDLE, 'start_command', StateType.HOMING),
        (StateType.HOMING, 'reached_home', StateType.READY),
        (StateType.READY, 'move_command', StateType.MOVING),
        # ... all transitions ...
    ]
    
    for from_state, event_type, to_state in transition_table:
        sm = create_test_sm()
        sm.set_initial_state(from_state)
        sm.post_event({'type': event_type})
        sm.process_next_event()
        assert sm.current_state.type == to_state
```

---

## 9. VISUALIZATION

### 9.1 State Diagram Generation

Using Graphviz untuk auto-generate diagram:

```python
def generate_state_diagram(sm):
    """Generate GraphViz DOT file"""
    dot = "digraph StateMachine {\n"
    dot += "  rankdir=LR;\n"
    dot += "  node [shape=circle];\n\n"
    
    # Add states
    for state_type, state in sm.states.items():
        dot += f"  {state.name};\n"
    
    # Add transitions
    for trans in sm.get_all_transitions():
        label = f"{trans.event}"
        if trans.guard:
            label += f" [{trans.guard}]"
        dot += f"  {trans.from_state} -> {trans.to_state} [label=\"{label}\"];\n"
    
    dot += "}\n"
    
    # Save to file
    with open('state_diagram.dot', 'w') as f:
        f.write(dot)
    
    # Generate image
    os.system('dot -Tpng state_diagram.dot -o state_diagram.png')
```

### 9.2 Runtime State Visualization

Real-time display current state:

```python
def visualize_state(sm):
    """Print current state and context"""
    print("=" * 50)
    print(f"Current State: {sm.current_state.name}")
    print(f"Context: {json.dumps(sm.context, indent=2)}")
    print(f"Event Queue Size: {sm.event_queue.queue.qsize()}")
    print(f"Uptime: {time.time() - sm.start_time:.1f}s")
    print("=" * 50)
```

---

## 10. CONTOH KASUS INDUSTRI

### Case 1: Robot Arm State Machine
**Application**: 6-DOF industrial robot arm  
**States**: IDLE, HOMING, READY, MOVING, PICKING, PLACING, ERROR  
**Complexity**: 8 states, 20+ transitions

**Key Features**:
- Homing sequence dengan sub-states (per joint)
- Path planning state (calculate trajectory)
- Collision detection → ERROR state
- Recovery: Return to safe position

### Case 2: AGV Navigation
**Application**: Autonomous Guided Vehicle  
**States**: IDLE, NAVIGATING, APPROACHING_STATION, DOCKED, CHARGING, ERROR

**Hierarchical States**:
```
NAVIGATING
├── FOLLOWING_PATH
├── OBSTACLE_DETECTED
└── REROUTING
```

### Case 3: Production Cell
**Application**: Multi-robot production cell  
**Challenge**: Coordinate 3 robots dengan shared resources

**Solution**: Master state machine + 3 slave state machines
- Master: WAITING_ROBOT_1, WAITING_ROBOT_2, WAITING_ROBOT_3
- Synchronization via events

---

## 11. BEST PRACTICES

### DO's:
- ✅ Keep states focused (single responsibility)
- ✅ Minimize transitions (reduce complexity)
- ✅ Use hierarchical states untuk complex behavior
- ✅ Test all transitions
- ✅ Log all state changes
- ✅ Document guard conditions clearly
- ✅ Use timeout untuk prevent stuck states
- ✅ Implement emergency stop dari ANY state

### DON'Ts:
- ❌ Too many states (> 15 usually sign of bad design)
- ❌ Circular dependencies
- ❌ Blocking operations dalam state (use async)
- ❌ Hardcoded transitions (use configuration)
- ❌ Ignore error events
- ❌ Forget to test edge cases

---

## 12. RINGKASAN

### Key Takeaways:

1. **State machines formalize operation logic**, making system behavior predictable dan verifiable
2. **Event-driven architecture** decouples components, improves maintainability
3. **Hierarchical states** reduce complexity untuk large systems
4. **Guard conditions** ensure safety dan prevent invalid transitions
5. **Fault recovery** critical untuk production systems
6. **Testing** essential untuk verify correctness
7. **Visualization** helps understanding dan debugging

### Integration:
- **Modul 11**: Events dapat datang dari MQTT messages
- **Modul 13**: REST API dapat trigger state transitions
- **Modul 14**: State changes logged ke InfluxDB untuk monitoring

---

## 13. REFERENSI

### Books:
- "Design Patterns" - Gang of Four (State Pattern)
- "Real-Time Systems" - Hermann Kopetz
- "UML State Machine Diagrams" - OMG Specification

### Papers:
- "Statecharts: A Visual Formalism for Complex Systems" - David Harel
- "Hierarchical State Machines for Robot Control" - IEEE

### Tools:
- PlantUML (state diagram generation)
- Graphviz (graph visualization)
- YAKINDU Statechart Tools (state machine IDE)

### Online Resources:
- https://en.wikipedia.org/wiki/Finite-state_machine
- https://www.uml-diagrams.org/state-machine-diagrams.html
- https://refactoring.guru/design-patterns/state

---

## 14. NEXT STEPS

Setelah menguasai State Machine:
- **Modul 13**: State machine integrated dengan Web API (REST endpoint trigger transitions)
- **Modul 14**: State transition logged dan monitored di Grafana
- **Capstone**: State machine coordinate complex task sequences

---

**Last Updated**: February 2026  
**Module**: 12 - State Machine & Sequencing  
**Batch**: 4 (System Integration)
