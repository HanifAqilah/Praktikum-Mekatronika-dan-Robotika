#!/usr/bin/env python3
"""
IDLE State Implementation
Robot is powered on but motors disabled, waiting for start command
"""

from .base_state import State
from ..state_machine_controller import StateType
import logging

logger = logging.getLogger(__name__)


class IdleState(State):
    """IDLE state - standby mode"""
    
    def __init__(self):
        super().__init__("IDLE")
    
    def on_enter(self, context):
        """Entry action: disable motors and reset error flags"""
        super().on_enter(context)
        
        logger.info("→ Entered IDLE state")
        
        # Disable motors for safety
        context['motors_enabled'] = False
        
        # Reset error flags
        context['has_error'] = False
        context['fault_cleared'] = False
        
        # Clear target if any
        context.pop('target_position', None)
        
        logger.debug("  Motors disabled, errors cleared")
    
    def on_exit(self, context):
        """Exit action: log start timestamp"""
        logger.info("← Leaving IDLE state")
        
        import time
        context['operation_start_time'] = time.time()
    
    def handle_event(self, event, context):
        """
        Handle events in IDLE state
        
        Accepted events:
        - start_command: Transition to HOMING
        - shutdown: Stay in IDLE (handle graceful shutdown)
        """
        event_type = event.get('type')
        
        if event_type == 'start_command':
            # Can only start if motors can be enabled
            if not context.get('hardware_fault', False):
                logger.info("  Start command received - transitioning to HOMING")
                context['motors_enabled'] = True
                return StateType.HOMING
            else:
                logger.warning("  Cannot start: hardware fault present")
                return StateType.ERROR
        
        elif event_type == 'shutdown':
            logger.info("  Shutdown command received - staying in IDLE")
            # Could add shutdown logic here
            return None
        
        else:
            logger.debug(f"  Event '{event_type}' ignored in IDLE state")
            return None
