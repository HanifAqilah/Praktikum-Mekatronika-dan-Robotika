#!/usr/bin/env python3
"""
HOMING State Implementation
Robot finding home position for all joints
"""

from .base_state import State
from ..state_machine_controller import StateType
import logging
import time

logger = logging.getLogger(__name__)


class HomingState(State):
    """HOMING state - finding home position"""
    
    def __init__(self, timeout=30.0):
        super().__init__("HOMING")
        self.timeout = timeout
        self.homing_start_time = None
    
    def on_enter(self, context):
        """Entry action: start homing sequence"""
        super().on_enter(context)
        
        logger.info("→ Entered HOMING state")
        
        # Enable motors
        context['motors_enabled'] = True
        
        # Start homing timer
        self.homing_start_time = time.time()
        
        # Initialize homing status
        context['homing_progress'] = {
            'joint_1': False,
            'joint_2': False,
            'joint_3': False,
            'all_homed': False
        }
        
        logger.info(f"  Starting homing sequence (timeout: {self.timeout}s)")
        
        # In real implementation, would send homing commands to motors here
        # For simulation, assume homing takes 2-5 seconds
    
    def on_exit(self, context):
        """Exit action: save home position"""
        logger.info("← Leaving HOMING state")
        
        # Save home position
        if context.get('homing_progress', {}).get('all_homed', False):
            context['home_position'] = [0.0, 0.0, 0.0]
            logger.info("  Home position saved: [0, 0, 0]")
        else:
            logger.warning("  Leaving HOMING without complete homing")
    
    def handle_event(self, event, context):
        """
        Handle events in HOMING state
        
        Accepted events:
        - reached_home: Homing successful → READY
        - homing_timeout: Timeout → ERROR
        - abort_command: User abort → IDLE
        - emergency_stop: Emergency → ERROR
        """
        event_type = event.get('type')
        
        # Check timeout
        if time.time() - self.homing_start_time > self.timeout:
            logger.error(f"  HOMING TIMEOUT after {self.timeout}s!")
            context['has_error'] = True
            context['error_message'] = "Homing timeout"
            return StateType.ERROR
        
        if event_type == 'reached_home':
            # Verify all joints homed
            homing_progress = context.get('homing_progress', {})
            homing_progress['all_homed'] = True
            
            if homing_progress.get('all_homed', False):
                logger.info("  All joints homed successfully")
                return StateType.READY
            else:
                logger.warning("  Not all joints homed yet")
                return None
        
        elif event_type == 'homing_timeout':
            logger.error("  Homing timeout event received")
            context['has_error'] = True
            context['error_message'] = "Homing timeout"
            return StateType.ERROR
        
        elif event_type == 'abort_command':
            logger.warning("  Homing aborted by user")
            return StateType.IDLE
        
        elif event_type == 'emergency_stop':
            logger.error("  Emergency stop during homing")
            context['has_error'] = True
            return StateType.ERROR
        
        else:
            logger.debug(f"  Event '{event_type}' ignored in HOMING state")
            return None
