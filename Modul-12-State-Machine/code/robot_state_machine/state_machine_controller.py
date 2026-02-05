#!/usr/bin/env python3
"""
State Machine Controller
Main controller that manages states, transitions, and events
"""

from enum import Enum
import time
import threading
from queue import PriorityQueue, Empty
import logging

# Setup logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class StateType(Enum):
    """Enumeration of all possible states"""
    IDLE = 0
    HOMING = 1
    READY = 2
    MOVING = 3
    WORKING = 4
    ERROR = 5


class StateMachineController:
    """
    Main state machine controller
    Manages states, processes events, handles transitions
    """
    
    def __init__(self):
        self.states = {}
        self.current_state = None
        self.context = {
            'motors_enabled': False,
            'path_clear': True,
            'has_error': False,
            'battery_level': 100,
            'home_position': None
        }
        
        # Event queue (priority, timestamp, event)
        self.event_queue = PriorityQueue(maxsize=200)
        
        # Logging
        self.transition_log = []
        self.event_log = []
        
        # Threading
        self.running = False
        self.thread = None
        self.lock = threading.Lock()
        
        # Metrics
        self.metrics = {
            'total_transitions': 0,
            'total_events': 0,
            'events_dropped': 0,
            'state_duration': {},
            'transition_count': {}
        }
    
    def add_state(self, state_type, state_obj):
        """
        Register a state
        
        Args:
            state_type: StateType enum
            state_obj: State instance
        """
        self.states[state_type] = state_obj
        
        # Initialize metrics for this state
        self.metrics['state_duration'][state_type.name] = []
        logger.info(f"Registered state: {state_obj.name}")
    
    def set_initial_state(self, state_type):
        """
        Set the starting state
        
        Args:
            state_type: StateType enum
        """
        with self.lock:
            self.current_state = self.states[state_type]
            self.current_state.on_enter(self.context)
            logger.info(f"Initial state set to: {self.current_state.name}")
    
    def post_event(self, event, priority=5):
        """
        Post event to queue (thread-safe)
        
        Args:
            event: Event dictionary {'type': str, 'data': dict}
            priority: Priority 0-5 (0=highest)
        
        Returns:
            bool: True if queued, False if dropped
        """
        try:
            timestamp = time.time()
            self.event_queue.put((priority, timestamp, event), block=False)
            
            self.metrics['total_events'] += 1
            
            logger.debug(f"Event queued: {event.get('type')} (priority={priority}, queue_size={self.event_queue.qsize()})")
            return True
            
        except:
            # Queue full - drop event
            self.metrics['events_dropped'] += 1
            logger.warning(f"Event dropped (queue full): {event.get('type')}")
            return False
    
    def process_next_event(self):
        """
        Process one event from queue
        This method is called by the main event loop
        """
        try:
            # Get event with timeout
            priority, timestamp, event = self.event_queue.get(timeout=0.1)
            
            # Log event
            self.event_log.append({
                'timestamp': timestamp,
                'event': event,
                'priority': priority,
                'state': self.current_state.name
            })
            
            # Process event
            with self.lock:
                start_time = time.time()
                
                next_state_type = self.current_state.handle_event(event, self.context)
                
                processing_time = (time.time() - start_time) * 1000  # ms
                
                # Transition if needed
                if next_state_type:
                    self.transition_to(next_state_type, event)
                
                logger.debug(f"Event processed: {event.get('type')} in {processing_time:.2f}ms")
        
        except Empty:
            # No events available
            pass
    
    def transition_to(self, next_state_type, event=None):
        """
        Execute state transition
        
        Args:
            next_state_type: StateType enum for target state
            event: Event that triggered transition (optional)
        """
        if next_state_type not in self.states:
            logger.error(f"Invalid state type: {next_state_type}")
            return
        
        old_state = self.current_state
        new_state = self.states[next_state_type]
        
        # Record duration in old state
        duration = old_state.get_duration()
        self.metrics['state_duration'][old_state.name].append(duration)
        
        # Log transition
        transition_record = {
            'timestamp': time.time(),
            'from': old_state.name,
            'to': new_state.name,
            'event': event.get('type') if event else None,
            'duration_ms': duration * 1000
        }
        self.transition_log.append(transition_record)
        
        # Update metrics
        transition_key = f"{old_state.name}→{new_state.name}"
        self.metrics['transition_count'][transition_key] = \
            self.metrics['transition_count'].get(transition_key, 0) + 1
        self.metrics['total_transitions'] += 1
        
        # Execute transition
        logger.info(f"TRANSITION: {old_state.name} → {new_state.name} (event: {event.get('type') if event else 'None'})")
        
        old_state.on_exit(self.context)
        self.current_state = new_state
        new_state.on_enter(self.context)
    
    def run(self):
        """
        Main event processing loop
        Run in separate thread
        """
        logger.info("State machine started")
        self.running = True
        
        while self.running:
            self.process_next_event()
            time.sleep(0.001)  # 1ms sleep to prevent CPU spinning
        
        logger.info("State machine stopped")
    
    def start(self):
        """Start state machine in background thread"""
        if not self.thread or not self.thread.is_alive():
            self.thread = threading.Thread(target=self.run, daemon=True)
            self.thread.start()
            logger.info("State machine thread started")
    
    def stop(self):
        """Stop state machine gracefully"""
        self.running = False
        if self.thread:
            self.thread.join(timeout=2.0)
            logger.info("State machine thread stopped")
    
    def get_current_state(self):
        """Get current state name (thread-safe)"""
        with self.lock:
            return self.current_state.name if self.current_state else None
    
    def get_metrics(self):
        """Get current metrics"""
        with self.lock:
            return {
                'current_state': self.current_state.name if self.current_state else None,
                'queue_size': self.event_queue.qsize(),
                'total_transitions': self.metrics['total_transitions'],
                'total_events': self.metrics['total_events'],
                'events_dropped': self.metrics['events_dropped'],
                'uptime': time.time() - self.transition_log[0]['timestamp'] if self.transition_log else 0
            }
    
    def save_log(self, filename='state_machine_log.json'):
        """Save transition log to file"""
        import json
        
        log_data = {
            'transitions': self.transition_log,
            'events': self.event_log,
            'metrics': self.metrics
        }
        
        with open(filename, 'w') as f:
            json.dump(log_data, f, indent=2)
        
        logger.info(f"Log saved to {filename}")
