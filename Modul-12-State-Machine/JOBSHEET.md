# JOBSHEET MODUL 12
## STATE MACHINE & SEQUENCING

**Nama**: ___________________  
**NPM**: ___________________  
**Kelas**: ___________________  
**Tanggal**: ___________________

---

## 🎯 TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:
1. Mengimplementasikan finite state machine dengan Python
2. Merancang state diagram dan transition table untuk robot
3. Menggunakan event queue untuk async event handling
4. Implementasi guard conditions untuk safety
5. Testing state machine behavior
6. Integrate state machine dengan ROS2

---

## 🛠 ALAT DAN BAHAN

### Hardware:
- Laptop/PC dengan Ubuntu 20.04/22.04
- ESP32 DevKit (dari Modul 11)
- USB Cable

### Software:
- ROS2 Humble/Iron
- Python 3.10+
- Visual Studio Code dengan Python extension
- PlantUML (untuk state diagram visualization)

### Library Python:
```bash
pip install transitions
pip install graphviz
pip install pytest
```

---

## 📚 PERCOBAAN 1: BASIC STATE MACHINE

### Tujuan:
Membuat simple 3-state machine dengan Python class

### Langkah Kerja:

#### 1. Buat File `simple_state_machine.py`

```python
#!/usr/bin/env python3
"""
Simple 3-State Machine
States: IDLE, WORKING, DONE
"""

from enum import Enum
import time

class StateType(Enum):
    IDLE = 0
    WORKING = 1
    DONE = 2

class SimpleStateMachine:
    def __init__(self):
        self.current_state = StateType.IDLE
        self.context = {
            'work_count': 0,
            'max_work': 3
        }
    
    def process_event(self, event):
        """Process event based on current state"""
        print(f"[{self.current_state.name}] Received event: {event}")
        
        if self.current_state == StateType.IDLE:
            if event == 'start':
                self.transition_to(StateType.WORKING)
        
        elif self.current_state == StateType.WORKING:
            if event == 'work_done':
                self.context['work_count'] += 1
                
                if self.context['work_count'] >= self.context['max_work']:
                    self.transition_to(StateType.DONE)
                else:
                    print(f"  Work {self.context['work_count']}/{self.context['max_work']} completed")
            
            elif event == 'stop':
                self.transition_to(StateType.IDLE)
        
        elif self.current_state == StateType.DONE:
            if event == 'reset':
                self.context['work_count'] = 0
                self.transition_to(StateType.IDLE)
    
    def transition_to(self, new_state):
        """Change state"""
        print(f"  TRANSITION: {self.current_state.name} → {new_state.name}")
        self.current_state = new_state

def main():
    sm = SimpleStateMachine()
    
    # Test sequence
    events = ['start', 'work_done', 'work_done', 'work_done', 'reset', 'start', 'stop']
    
    for event in events:
        sm.process_event(event)
        time.sleep(0.5)
        print()

if __name__ == '__main__':
    main()
```

#### 2. Jalankan Program

```bash
chmod +x simple_state_machine.py
python3 simple_state_machine.py
```

### 📝 Observasi:

**Tabel 1: State Transitions**

| No | Event | Before State | After State | Context Change |
|----|-------|--------------|-------------|----------------|
| 1 | start | IDLE | WORKING | - |
| 2 | work_done | WORKING | WORKING | work_count: 0→1 |
| 3 | work_done | WORKING | WORKING | work_count: 1→2 |
| 4 | work_done | WORKING | DONE | work_count: 2→3 |
| 5 | reset | DONE | IDLE | work_count: 3→0 |
| 6 | start | IDLE | WORKING | - |
| 7 | stop | WORKING | IDLE | - |

### Analisis:

1. **Apa yang terjadi jika event tidak valid untuk current state?**
   
   Jawab: _______________________________________________

2. **Mengapa work_count di-store di context, bukan sebagai class variable?**
   
   Jawab: _______________________________________________

3. **Bagaimana cara menambahkan state LOG untuk logging sebelum DONE?**
   
   Jawab: _______________________________________________

---

## 📚 PERCOBAAN 2: STATE PATTERN (OOP)

### Tujuan:
Implementasi State Pattern dengan inheritance untuk better organization

### Langkah Kerja:

#### 1. Buat File `oop_state_machine.py`

```python
#!/usr/bin/env python3
"""
State Machine using State Pattern (OOP)
"""

from abc import ABC, abstractmethod
from enum import Enum
import time

class StateType(Enum):
    IDLE = 0
    HOMING = 1
    READY = 2
    MOVING = 3
    ERROR = 4

class State(ABC):
    """Abstract base class"""
    
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

class IdleState(State):
    def __init__(self):
        super().__init__("IDLE")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
        context['motors_enabled'] = False
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
    
    def handle_event(self, event, context):
        if event['type'] == 'start_command':
            return StateType.HOMING
        return None

class HomingState(State):
    def __init__(self):
        super().__init__("HOMING")
        self.homing_start_time = None
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
        context['motors_enabled'] = True
        self.homing_start_time = time.time()
        print("  Starting homing sequence...")
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
        context['home_position'] = [0, 0, 0]
    
    def handle_event(self, event, context):
        # Check timeout
        if time.time() - self.homing_start_time > 10.0:
            print("  ERROR: Homing timeout!")
            return StateType.ERROR
        
        if event['type'] == 'reached_home':
            return StateType.READY
        
        return None

class ReadyState(State):
    def __init__(self):
        super().__init__("READY")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
        print("  Robot ready for commands")
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
    
    def handle_event(self, event, context):
        if event['type'] == 'move_command':
            if context.get('path_clear', True):
                context['target_position'] = event.get('target', [100, 100, 0])
                return StateType.MOVING
            else:
                print("  WARNING: Path not clear!")
                return None
        
        elif event['type'] == 'idle_command':
            return StateType.IDLE
        
        return None

class MovingState(State):
    def __init__(self):
        super().__init__("MOVING")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
        target = context.get('target_position', [0, 0, 0])
        print(f"  Moving to: {target}")
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
        print("  Movement complete")
    
    def handle_event(self, event, context):
        if event['type'] == 'reached_target':
            return StateType.READY
        
        elif event['type'] == 'abort_command':
            return StateType.READY
        
        elif event['type'] == 'path_blocked':
            return StateType.ERROR
        
        return None

class ErrorState(State):
    def __init__(self):
        super().__init__("ERROR")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
        print("  🚨 EMERGENCY STOP 🚨")
        context['motors_enabled'] = False
        context['error_message'] = "Fault detected"
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
        context.pop('error_message', None)
    
    def handle_event(self, event, context):
        if event['type'] == 'reset_command':
            if context.get('fault_cleared', False):
                return StateType.IDLE
            else:
                print("  Cannot reset: fault not cleared")
        
        return None

class StateMachineController:
    def __init__(self):
        self.states = {
            StateType.IDLE: IdleState(),
            StateType.HOMING: HomingState(),
            StateType.READY: ReadyState(),
            StateType.MOVING: MovingState(),
            StateType.ERROR: ErrorState()
        }
        
        self.current_state = self.states[StateType.IDLE]
        self.context = {}
        self.transition_log = []
    
    def post_event(self, event):
        """Process an event"""
        print(f"\n📨 Event: {event['type']}")
        
        next_state_type = self.current_state.handle_event(event, self.context)
        
        if next_state_type:
            self.transition_to(next_state_type, event)
    
    def transition_to(self, next_state_type, event):
        """Execute transition"""
        old_state = self.current_state
        new_state = self.states[next_state_type]
        
        # Log
        self.transition_log.append({
            'timestamp': time.time(),
            'from': old_state.name,
            'to': new_state.name,
            'event': event
        })
        
        # Execute
        old_state.on_exit(self.context)
        self.current_state = new_state
        new_state.on_enter(self.context)

def main():
    sm = StateMachineController()
    sm.current_state.on_enter(sm.context)
    
    # Test sequence
    print("=" * 60)
    print("Testing State Machine")
    print("=" * 60)
    
    # Sequence: IDLE → HOMING → READY → MOVING → READY → IDLE
    sm.post_event({'type': 'start_command'})
    time.sleep(1)
    
    sm.post_event({'type': 'reached_home'})
    time.sleep(1)
    
    sm.post_event({'type': 'move_command', 'target': [50, 50, 10]})
    time.sleep(2)
    
    sm.post_event({'type': 'reached_target'})
    time.sleep(1)
    
    sm.post_event({'type': 'idle_command'})
    
    # Print transition log
    print("\n" + "=" * 60)
    print("Transition Log:")
    print("=" * 60)
    for trans in sm.transition_log:
        print(f"{trans['from']:10} → {trans['to']:10}  (event: {trans['event']['type']})")

if __name__ == '__main__':
    main()
```

#### 2. Jalankan dan Observasi

```bash
python3 oop_state_machine.py
```

### 📝 Observasi:

**Tabel 2: Entry/Exit Actions**

| Transition | Exit Action (Old State) | Entry Action (New State) |
|------------|-------------------------|--------------------------|
| IDLE → HOMING | Leaving IDLE | motors_enabled=True, start homing |
| HOMING → READY | save home_position | Print "ready for commands" |
| READY → MOVING | Leaving READY | Print target position |
| MOVING → READY | Print "Movement complete" | Robot ready |
| READY → IDLE | - | motors_enabled=False |

### Analisis:

1. **Apa keuntungan menggunakan on_enter() dan on_exit()?**
   
   Jawab: _______________________________________________

2. **Kapan transition tidak akan terjadi meskipun event valid?**
   
   Jawab: _______________________________________________

---

## 📚 PERCOBAAN 3: EVENT QUEUE

### Tujuan:
Implementasi event queue untuk async event handling

### Langkah Kerja:

#### 1. Buat File `event_queue_sm.py`

```python
#!/usr/bin/env python3
"""
State Machine with Event Queue
"""

from queue import Queue, Empty
import threading
import time
from oop_state_machine import StateMachineController, StateType

class EventDrivenStateMachine(StateMachineController):
    def __init__(self):
        super().__init__()
        self.event_queue = Queue()
        self.running = False
        self.thread = None
    
    def post_event_async(self, event):
        """Add event to queue (non-blocking)"""
        self.event_queue.put(event)
        print(f"📬 Posted event: {event['type']} (queue size: {self.event_queue.qsize()})")
    
    def run(self):
        """Main event processing loop"""
        print("🔄 State machine started\n")
        self.running = True
        self.current_state.on_enter(self.context)
        
        while self.running:
            try:
                # Get event with timeout
                event = self.event_queue.get(timeout=0.1)
                
                # Process event
                print(f"\n📨 Processing: {event['type']}")
                next_state_type = self.current_state.handle_event(event, self.context)
                
                if next_state_type:
                    self.transition_to(next_state_type, event)
                
            except Empty:
                # No events, continue
                pass
            
            time.sleep(0.01)
    
    def start(self):
        """Start state machine in background thread"""
        self.thread = threading.Thread(target=self.run, daemon=True)
        self.thread.start()
    
    def stop(self):
        """Stop state machine"""
        self.running = False
        if self.thread:
            self.thread.join(timeout=2.0)

def main():
    sm = EventDrivenStateMachine()
    sm.start()
    
    # Simulate events from different sources
    time.sleep(0.5)
    sm.post_event_async({'type': 'start_command'})
    
    time.sleep(1.0)
    sm.post_event_async({'type': 'reached_home'})
    
    time.sleep(0.5)
    sm.post_event_async({'type': 'move_command', 'target': [100, 50, 0]})
    
    time.sleep(1.5)
    sm.post_event_async({'type': 'reached_target'})
    
    time.sleep(0.5)
    sm.post_event_async({'type': 'idle_command'})
    
    # Let it process
    time.sleep(2.0)
    sm.stop()
    
    print("\n" + "=" * 60)
    print(f"Total transitions: {len(sm.transition_log)}")

if __name__ == '__main__':
    main()
```

#### 2. Jalankan

```bash
python3 event_queue_sm.py
```

### 📝 Observasi:

**Tabel 3: Event Queue Behavior**

| Time (s) | Event Posted | Queue Size Before | Queue Size After | Processing Delay (ms) |
|----------|--------------|-------------------|------------------|-----------------------|
| 0.5 | start_command | 0 | 1 | - |
| 1.5 | reached_home | 0 | 1 | - |
| 2.0 | move_command | 0 | 1 | - |
| 3.5 | reached_target | 0 | 1 | - |
| 4.0 | idle_command | 0 | 1 | - |

### Analisis:

1. **Apa keuntungan async event posting vs direct processing?**
   
   Jawab: _______________________________________________

2. **Apa yang terjadi jika events posted lebih cepat dari processing?**
   
   Jawab: _______________________________________________

---

## 📚 PERCOBAAN 4: GUARD CONDITIONS

### Tujuan:
Implementasi guard conditions untuk safety checks

### Langkah Kerja:

#### 1. Buat File `guarded_state_machine.py`

```python
#!/usr/bin/env python3
"""
State Machine with Guard Conditions
"""

from oop_state_machine import StateMachineController, State, StateType as BaseStateType
from enum import Enum

class StateType(Enum):
    IDLE = 0
    READY = 1
    MOVING = 2
    ERROR = 3

class GuardedReadyState(State):
    def __init__(self):
        super().__init__("READY")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
    
    def handle_event(self, event, context):
        if event['type'] == 'move_command':
            # Multiple guard conditions
            guards = self.check_guards(event, context)
            
            print(f"  Guard Checks:")
            for guard_name, result in guards.items():
                status = "✓" if result else "✗"
                print(f"    {status} {guard_name}: {result}")
            
            # All guards must pass
            if all(guards.values()):
                context['target'] = event.get('target', [0, 0, 0])
                print("  ✅ All guards passed - transitioning to MOVING")
                return StateType.MOVING
            else:
                print("  ❌ Guard check failed - staying in READY")
                return None
        
        return None
    
    def check_guards(self, event, context):
        """Check all guard conditions"""
        return {
            'motors_enabled': context.get('motors_enabled', False),
            'path_clear': context.get('path_clear', True),
            'no_error': not context.get('has_error', False),
            'target_valid': self.validate_target(event.get('target', [])),
            'battery_ok': context.get('battery_level', 100) > 20
        }
    
    def validate_target(self, target):
        """Validate target position"""
        if not target or len(target) != 3:
            return False
        
        # Check workspace limits
        x, y, z = target
        return (-200 <= x <= 200 and 
                -200 <= y <= 200 and 
                0 <= z <= 300)

class GuardedStateMachine(StateMachineController):
    def __init__(self):
        self.states = {
            StateType.IDLE: IdleState(),
            StateType.READY: GuardedReadyState(),
            StateType.MOVING: MovingState(),
            StateType.ERROR: ErrorState()
        }
        self.current_state = self.states[StateType.READY]
        self.context = {
            'motors_enabled': True,
            'path_clear': True,
            'has_error': False,
            'battery_level': 80
        }
        self.transition_log = []

# Import other states from previous file
from oop_state_machine import IdleState, ErrorState

class MovingState(State):
    def __init__(self):
        super().__init__("MOVING")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
        print(f"  Target: {context.get('target', [0,0,0])}")
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
    
    def handle_event(self, event, context):
        if event['type'] == 'reached_target':
            return StateType.READY
        return None

def main():
    sm = GuardedStateMachine()
    
    print("=" * 60)
    print("Test 1: All guards pass")
    print("=" * 60)
    sm.post_event({'type': 'move_command', 'target': [100, 50, 20]})
    
    print("\n" + "=" * 60)
    print("Test 2: Invalid target (out of workspace)")
    print("=" * 60)
    sm.post_event({'type': 'reached_target'})  # Back to READY
    sm.post_event({'type': 'move_command', 'target': [500, 50, 20]})
    
    print("\n" + "=" * 60)
    print("Test 3: Path not clear")
    print("=" * 60)
    sm.context['path_clear'] = False
    sm.post_event({'type': 'move_command', 'target': [80, 80, 10]})
    
    print("\n" + "=" * 60)
    print("Test 4: Low battery")
    print("=" * 60)
    sm.context['path_clear'] = True
    sm.context['battery_level'] = 15
    sm.post_event({'type': 'move_command', 'target': [60, 60, 10]})

if __name__ == '__main__':
    main()
```

#### 2. Jalankan

```bash
python3 guarded_state_machine.py
```

### 📝 Observasi:

**Tabel 4: Guard Condition Results**

| Test | Target | motors_enabled | path_clear | battery_ok | target_valid | Transition? |
|------|--------|----------------|------------|------------|--------------|-------------|
| 1 | [100,50,20] | ✓ | ✓ | ✓ | ✓ | YES |
| 2 | [500,50,20] | ✓ | ✓ | ✓ | ✗ | NO |
| 3 | [80,80,10] | ✓ | ✗ | ✓ | ✓ | NO |
| 4 | [60,60,10] | ✓ | ✓ | ✗ | ✓ | NO |

### Analisis:

1. **Mengapa guard conditions penting untuk safety?**
   
   Jawab: _______________________________________________

2. **Bagaimana implementasi guard condition berbeda dengan IF statement biasa?**
   
   Jawab: _______________________________________________

---

## 📚 PERCOBAAN 5: HIERARCHICAL STATE MACHINE

### Tujuan:
Implementasi hierarchical states dengan sub-state machines

### Langkah Kerja:

#### 1. Buat File `hierarchical_sm.py`

```python
#!/usr/bin/env python3
"""
Hierarchical State Machine
OPERATIONAL state contains IDLE, HOMING, READY, MOVING sub-states
"""

from enum import Enum
from oop_state_machine import State
import time

class SuperStateType(Enum):
    OFFLINE = 0
    OPERATIONAL = 1
    MAINTENANCE = 2

class OperationalSubState(Enum):
    IDLE = 0
    HOMING = 1
    READY = 2
    MOVING = 3

class OperationalState(State):
    """Super-state containing sub-state machine"""
    
    def __init__(self):
        super().__init__("OPERATIONAL")
        self.sub_state = OperationalSubState.IDLE
    
    def on_enter(self, context):
        print(f"→ Entered SUPER-STATE: {self.name}")
        print(f"  Sub-state: {self.sub_state.name}")
        context['operational'] = True
    
    def on_exit(self, context):
        print(f"← Leaving SUPER-STATE: {self.name}")
        context['operational'] = False
    
    def handle_event(self, event, context):
        """Handle event at super-state level"""
        
        # Super-state level events (apply to ALL sub-states)
        if event['type'] == 'emergency_stop':
            print("  🚨 EMERGENCY STOP from any sub-state!")
            return SuperStateType.MAINTENANCE
        
        if event['type'] == 'shutdown':
            return SuperStateType.OFFLINE
        
        # Delegate to sub-state
        next_sub_state = self.handle_sub_state_event(event, context)
        if next_sub_state:
            self.transition_sub_state(next_sub_state, context)
        
        return None  # Stay in OPERATIONAL super-state
    
    def handle_sub_state_event(self, event, context):
        """Handle events at sub-state level"""
        
        if self.sub_state == OperationalSubState.IDLE:
            if event['type'] == 'start':
                return OperationalSubState.HOMING
        
        elif self.sub_state == OperationalSubState.HOMING:
            if event['type'] == 'homed':
                return OperationalSubState.READY
        
        elif self.sub_state == OperationalSubState.READY:
            if event['type'] == 'move':
                return OperationalSubState.MOVING
            elif event['type'] == 'idle':
                return OperationalSubState.IDLE
        
        elif self.sub_state == OperationalSubState.MOVING:
            if event['type'] == 'reached':
                return OperationalSubState.READY
        
        return None
    
    def transition_sub_state(self, new_sub_state, context):
        """Transition between sub-states"""
        old = self.sub_state
        self.sub_state = new_sub_state
        print(f"  Sub-transition: {old.name} → {new_sub_state.name}")

class HierarchicalStateMachine:
    def __init__(self):
        self.super_states = {
            SuperStateType.OFFLINE: State("OFFLINE"),
            SuperStateType.OPERATIONAL: OperationalState(),
            SuperStateType.MAINTENANCE: State("MAINTENANCE")
        }
        
        self.current_super_state = self.super_states[SuperStateType.OFFLINE]
        self.context = {}
    
    def post_event(self, event):
        """Post event to current super-state"""
        print(f"\n📨 Event: {event['type']}")
        
        next_super_state_type = self.current_super_state.handle_event(event, self.context)
        
        if next_super_state_type:
            self.transition_super_state(next_super_state_type)
    
    def transition_super_state(self, next_type):
        """Transition between super-states"""
        old = self.current_super_state
        new = self.super_states[next_type]
        
        print(f"\n🔄 SUPER-TRANSITION: {old.name} → {new.name}")
        old.on_exit(self.context)
        self.current_super_state = new
        new.on_enter(self.context)

# Simple state implementations for OFFLINE and MAINTENANCE
class OfflineState(State):
    def __init__(self):
        super().__init__("OFFLINE")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name}")
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
    
    def handle_event(self, event, context):
        if event['type'] == 'power_on':
            return SuperStateType.OPERATIONAL
        return None

class MaintenanceState(State):
    def __init__(self):
        super().__init__("MAINTENANCE")
    
    def on_enter(self, context):
        print(f"→ Entered {self.name} (all motors disabled)")
    
    def on_exit(self, context):
        print(f"← Leaving {self.name}")
    
    def handle_event(self, event, context):
        if event['type'] == 'maintenance_complete':
            return SuperStateType.OPERATIONAL
        elif event['type'] == 'power_off':
            return SuperStateType.OFFLINE
        return None

def main():
    # Replace simple states with full implementations
    sm = HierarchicalStateMachine()
    sm.super_states[SuperStateType.OFFLINE] = OfflineState()
    sm.super_states[SuperStateType.MAINTENANCE] = MaintenanceState()
    
    sm.current_super_state = sm.super_states[SuperStateType.OFFLINE]
    sm.current_super_state.on_enter(sm.context)
    
    print("=" * 60)
    print("Testing Hierarchical State Machine")
    print("=" * 60)
    
    # Test sequence
    sm.post_event({'type': 'power_on'})
    time.sleep(0.5)
    
    # Sub-state transitions
    sm.post_event({'type': 'start'})
    time.sleep(0.5)
    
    sm.post_event({'type': 'homed'})
    time.sleep(0.5)
    
    sm.post_event({'type': 'move'})
    time.sleep(0.5)
    
    # Emergency stop (from any sub-state to MAINTENANCE)
    sm.post_event({'type': 'emergency_stop'})
    time.sleep(0.5)
    
    # Resume
    sm.post_event({'type': 'maintenance_complete'})

if __name__ == '__main__':
    main()
```

#### 2. Jalankan

```bash
python3 hierarchical_sm.py
```

### 📝 Observasi:

**Tabel 5: Hierarchical State Behavior**

| Event | Super-State Before | Sub-State Before | Super-State After | Sub-State After |
|-------|-------------------|------------------|-------------------|-----------------|
| power_on | OFFLINE | - | OPERATIONAL | IDLE |
| start | OPERATIONAL | IDLE | OPERATIONAL | HOMING |
| homed | OPERATIONAL | HOMING | OPERATIONAL | READY |
| move | OPERATIONAL | READY | OPERATIONAL | MOVING |
| emergency_stop | OPERATIONAL | MOVING | MAINTENANCE | - |
| maintenance_complete | MAINTENANCE | - | OPERATIONAL | IDLE |

### Analisis:

1. **Apa keuntungan hierarchical state machine?**
   
   Jawab: _______________________________________________

2. **Event apa yang harus di-handle di super-state vs sub-state level?**
   
   Jawab: _______________________________________________

---

## 📚 PERCOBAAN 5: ROS2 STATE MACHINE NODE

### Tujuan:
Integrate state machine dengan ROS2 untuk robot control

### Langkah Kerja:

#### 1. Setup ROS2 Package

```bash
cd ~/ros2_ws/src
ros2 pkg create robot_state_machine --build-type ament_python --dependencies rclpy std_msgs
cd robot_state_machine
```

#### 2. Buat File `robot_state_machine/robot_state_node.py`

```python
#!/usr/bin/env python3
"""
ROS2 State Machine Node
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from event_queue_sm import EventDrivenStateMachine
import json
import threading

class RobotStateNode(Node):
    def __init__(self):
        super().__init__('robot_state_machine')
        
        # State machine
        self.state_machine = EventDrivenStateMachine()
        
        # ROS2 Publishers
        self.state_pub = self.create_publisher(String, 'robot/state', 10)
        self.event_pub = self.create_publisher(String, 'robot/events', 10)
        
        # ROS2 Subscribers
        self.command_sub = self.create_subscription(
            String,
            'robot/command',
            self.command_callback,
            10
        )
        
        # Timer untuk publish current state
        self.create_timer(0.5, self.publish_state)
        
        # Start state machine
        self.state_machine.start()
        
        self.get_logger().info("Robot State Machine Node started")
    
    def command_callback(self, msg):
        """Receive commands and convert to events"""
        try:
            command = json.loads(msg.data)
            
            # Map command to event
            event = {
                'type': command['command'],
                'timestamp': self.get_clock().now().to_msg()
            }
            
            # Add additional data
            if 'target' in command:
                event['target'] = command['target']
            
            # Post to state machine
            self.state_machine.post_event_async(event)
            
            self.get_logger().info(f"Command received: {command['command']}")
            
        except json.JSONDecodeError:
            self.get_logger().error(f"Invalid JSON: {msg.data}")
    
    def publish_state(self):
        """Publish current state"""
        state_msg = String()
        
        state_data = {
            'state': self.state_machine.current_state.name,
            'context': {
                k: v for k, v in self.state_machine.context.items()
                if isinstance(v, (int, float, str, bool))
            },
            'timestamp': self.get_clock().now().to_msg().sec
        }
        
        state_msg.data = json.dumps(state_data)
        self.state_pub.publish(state_msg)

def main(args=None):
    rclpy.init(args=args)
    node = RobotStateNode()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.state_machine.stop()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
```

#### 3. Build dan Run

```bash
cd ~/ros2_ws
colcon build --packages-select robot_state_machine
source install/setup.bash

ros2 run robot_state_machine robot_state_node
```

#### 4. Test dengan ROS2 CLI

```bash
# Terminal 2: Monitor state
ros2 topic echo /robot/state

# Terminal 3: Send commands
ros2 topic pub --once /robot/command std_msgs/String "{data: '{\"command\": \"start_command\"}'}"

ros2 topic pub --once /robot/command std_msgs/String "{data: '{\"command\": \"reached_home\"}'}"

ros2 topic pub --once /robot/command std_msgs/String "{data: '{\"command\": \"move_command\", \"target\": [100, 50, 20]}'}"

ros2 topic pub --once /robot/command std_msgs/String "{data: '{\"command\": \"reached_target\"}'}"
```

### 📝 Observasi:

**Tabel 6: ROS2 Integration**

| Command Sent | State Before | State After | Publish Latency (ms) |
|--------------|--------------|-------------|----------------------|
| start_command | IDLE | HOMING | |
| reached_home | HOMING | READY | |
| move_command | READY | MOVING | |
| reached_target | MOVING | READY | |

### Analisis:

1. **Apa keuntungan integrate state machine dengan ROS2?**
   
   Jawab: _______________________________________________

2. **Bagaimana cara handle multiple commands yang datang bersamaan?**
   
   Jawab: _______________________________________________

---

## 📚 PERCOBAAN 6: STATE MACHINE TESTING

### Tujuan:
Comprehensive testing dengan pytest

### Langkah Kerja:

#### 1. Buat File `test_state_machine.py`

```python
#!/usr/bin/env python3
"""
State Machine Unit Tests
"""

import pytest
import time
from oop_state_machine import StateMachineController, StateType

class TestStateMachine:
    @pytest.fixture
    def sm(self):
        """Create fresh state machine for each test"""
        sm = StateMachineController()
        sm.current_state = sm.states[StateType.IDLE]
        sm.current_state.on_enter(sm.context)
        return sm
    
    def test_initial_state(self, sm):
        """Test initial state is IDLE"""
        assert sm.current_state.name == "IDLE"
        assert sm.context.get('motors_enabled') == False
    
    def test_idle_to_homing(self, sm):
        """Test IDLE → HOMING transition"""
        sm.post_event({'type': 'start_command'})
        assert sm.current_state.name == "HOMING"
    
    def test_complete_sequence(self, sm):
        """Test complete operation sequence"""
        # IDLE → HOMING
        sm.post_event({'type': 'start_command'})
        assert sm.current_state.name == "HOMING"
        
        # HOMING → READY
        time.sleep(0.1)  # Simulate homing time
        sm.post_event({'type': 'reached_home'})
        assert sm.current_state.name == "READY"
        
        # READY → MOVING
        sm.context['path_clear'] = True
        sm.post_event({'type': 'move_command', 'target': [100, 50, 20]})
        assert sm.current_state.name == "MOVING"
        
        # MOVING → READY
        sm.post_event({'type': 'reached_target'})
        assert sm.current_state.name == "READY"
    
    def test_invalid_event(self, sm):
        """Test that invalid events don't change state"""
        initial_state = sm.current_state.name
        sm.post_event({'type': 'invalid_event'})
        assert sm.current_state.name == initial_state
    
    def test_guard_condition_fail(self, sm):
        """Test transition blocked by guard"""
        # Get to READY state
        sm.post_event({'type': 'start_command'})
        sm.post_event({'type': 'reached_home'})
        assert sm.current_state.name == "READY"
        
        # Try to move with path blocked
        sm.context['path_clear'] = False
        sm.post_event({'type': 'move_command'})
        assert sm.current_state.name == "READY"  # Should NOT transition
    
    def test_error_transition(self, sm):
        """Test error handling"""
        # Get to MOVING state
        sm.post_event({'type': 'start_command'})
        sm.post_event({'type': 'reached_home'})
        sm.context['path_clear'] = True
        sm.post_event({'type': 'move_command'})
        
        # Trigger error
        sm.post_event({'type': 'path_blocked'})
        assert sm.current_state.name == "ERROR"
    
    def test_transition_log(self, sm):
        """Test that transitions are logged"""
        sm.post_event({'type': 'start_command'})
        sm.post_event({'type': 'reached_home'})
        
        assert len(sm.transition_log) == 2
        assert sm.transition_log[0]['from'] == "IDLE"
        assert sm.transition_log[0]['to'] == "HOMING"
        assert sm.transition_log[1]['from'] == "HOMING"
        assert sm.transition_log[1]['to'] == "READY"

class TestEventQueue:
    def test_event_ordering(self):
        """Test events processed in order"""
        from event_queue_sm import EventDrivenStateMachine
        
        sm = EventDrivenStateMachine()
        sm.start()
        
        # Post multiple events
        sm.post_event_async({'type': 'start_command'})
        sm.post_event_async({'type': 'reached_home'})
        sm.post_event_async({'type': 'move_command'})
        
        # Give time to process
        time.sleep(1.0)
        sm.stop()
        
        # Check transitions happened in order
        assert len(sm.transition_log) >= 2

if __name__ == '__main__':
    pytest.main([__file__, '-v'])
```

#### 2. Jalankan Tests

```bash
pytest test_state_machine.py -v
```

### 📝 Observasi:

**Tabel 7: Test Results**

| Test Name | Result | Duration (ms) | Notes |
|-----------|--------|---------------|-------|
| test_initial_state | PASS | | |
| test_idle_to_homing | PASS | | |
| test_complete_sequence | PASS | | |
| test_invalid_event | PASS | | |
| test_guard_condition_fail | PASS | | |
| test_error_transition | PASS | | |
| test_transition_log | PASS | | |
| test_event_ordering | PASS | | |

**Test Coverage**: _____ %

### Analisis:

1. **Mengapa testing state machines berbeda dari testing function biasa?**
   
   Jawab: _______________________________________________

2. **Test case apa yang masih missing dari test suite ini?**
   
   Jawab: _______________________________________________

---

## 🔍 TROUBLESHOOTING

### Problem 1: State Stuck (Tidak Transition)

**Symptoms**:
- Event processed tapi state tidak berubah
- No error messages

**Root Causes**:
- Guard condition tidak terpenuhi
- Event type typo
- Handle_event() return None instead of next state

**Solution**:
```python
# Add debug logging
def handle_event(self, event, context):
    print(f"DEBUG: Handling {event['type']} in {self.name}")
    
    if event['type'] == 'start_command':
        print(f"DEBUG: Guard check - motors_enabled: {context.get('motors_enabled')}")
        if context.get('motors_enabled', False):
            return StateType.HOMING
        else:
            print("DEBUG: Guard failed!")
    
    return None
```

### Problem 2: Event Queue Overflow

**Symptoms**:
- Memory usage terus naik
- Events not processed cukup cepat

**Root Causes**:
- Events posted faster than processed
- Blocking operation dalam state

**Solution**:
```python
# Set max queue size
event_queue = Queue(maxsize=100)

# Check queue size
if event_queue.qsize() > 80:
    print("WARNING: Event queue nearly full!")
```

### Problem 3: Race Condition

**Symptoms**:
- Inconsistent behavior
- Context corruption

**Root Causes**:
- Multiple threads modify context tanpa lock
- Event processing race

**Solution**:
```python
import threading

class ThreadSafeStateMachine(StateMachineController):
    def __init__(self):
        super().__init__()
        self.lock = threading.Lock()
    
    def post_event(self, event):
        with self.lock:
            super().post_event(event)
```

---

## 📊 EVALUASI PRAKTIKUM

### Checklist:

- [ ] Percobaan 1: Simple state machine works
- [ ] Percobaan 2: OOP state pattern implemented
- [ ] Percobaan 3: Event queue functional
- [ ] Percobaan 4: Guard conditions working
- [ ] Percobaan 5: Hierarchical states implemented
- [ ] Percobaan 6: All tests passing

### Questions:

1. Apa perbedaan utama antara Moore dan Mealy state machine?

   Jawab: _______________________________________________

2. Kapan menggunakan hierarchical state machine vs flat state machine?

   Jawab: _______________________________________________

3. Bagaimana cara memastikan state machine thread-safe?

   Jawab: _______________________________________________

4. Apa keuntungan event queue dibanding direct method call?

   Jawab: _______________________________________________

5. Jelaskan perbedaan entry action, exit action, dan transition action!

   Jawab: _______________________________________________

---

## 🎓 TUGAS TAMBAHAN

### Tugas 1: Extend State Machine
Tambahkan state **CALIBRATING** antara HOMING dan READY:
- Entry action: Start sensor calibration
- Timeout: 5 seconds
- Success → READY, Timeout → ERROR

### Tugas 2: Logging System
Implementasikan logging semua state transitions ke file CSV:
- Columns: timestamp, from_state, to_state, event_type, duration_ms
- Auto-rotate file setiap 1000 lines

### Tugas 3: State Persistence
Implementasikan save/restore state ke file JSON:
- Save context setiap state change
- Restore pada startup
- Test dengan kill process dan restart

### Tugas 4: Visualization
Generate state diagram dari transition log:
- Use Graphviz
- Show transition counts (edge labels)
- Highlight most frequent path

---

## 📝 LAPORAN PRAKTIKUM

Laporan harus include:

1. **Pendahuluan**
   - Tujuan praktikum
   - Teori singkat state machine

2. **Metodologi**
   - Setup environment
   - State definitions dan diagram
   - Transition table

3. **Hasil**
   - Output dari 6 percobaan
   - Filled observation tables
   - Test results

4. **Analisis**
   - Jawaban pertanyaan analisis
   - Perbandingan implementation methods
   - Performance analysis

5. **Kesimpulan**
   - Summary key findings
   - Lessons learned
   - Aplikasi di robot nyata

6. **Appendix**
   - Complete source code
   - State diagrams
   - Test coverage report

---

## 🔗 REFERENSI

### Internal:
- MODUL 11: MQTT events → state machine events
- MODUL 13: REST API → trigger state transitions
- MODUL 14: State logs → Grafana monitoring

### External:
- Python `transitions` library: https://github.com/pytransitions/transitions
- ROS2 Lifecycle Nodes: https://design.ros2.org/articles/node_lifecycle.html
- PlantUML State Diagrams: https://plantuml.com/state-diagram

---

**Deadline Laporan**: 1 minggu setelah praktikum  
**Format**: PDF, max 20 halaman  
**Submit**: Email ke asisten praktikum

---

**Dosen Pengampu**: _________________  
**Asisten Praktikum**: _________________  
**Paraf**: _________________

