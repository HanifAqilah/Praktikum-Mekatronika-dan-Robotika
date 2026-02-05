#!/usr/bin/env python3
"""
ERROR State Implementation
Robot in fault condition, requires reset
"""

from .base_state import State
from ..state_machine_controller import StateType
import logging

logger = logging.getLogger(__name__)


class ErrorState(State):
    """ERROR state - fault condition"""
    
    def __init__(self):
        super().__init__("ERROR")
    
    def on_enter(self, context):
        """Entry action: emergency stop and trigger alarm"""
        super().on_enter(context)
        
        logger.error("→ Entered ERROR state")
        logger.error("  🚨 EMERGENCY STOP ACTIVATED 🚨")
        
        # Emergency stop all motors
        context['motors_enabled'] = False
        context['emergency_stop_active'] = True
        
        # Log error
        error_msg = context.get('error_message', 'Unknown error')
        logger.error(f"  Error: {error_msg}")
        
        # Trigger alarm (in real implementation)
        # - Sound buzzer
        # - Flash warning lights
        # - Send notifications
        
        # Save state for diagnostics
        context['error_timestamp'] = self.enter_time
        context['error_state_before'] = context.get('last_state', 'UNKNOWN')
    
    def on_exit(self, context):
        """Exit action: clear error flags"""
        logger.info("← Leaving ERROR state")
        
        # Clear error flags
        context['has_error'] = False
        context['emergency_stop_active'] = False
        context.pop('error_message', None)
        
        logger.info("  Error flags cleared")
    
    def handle_event(self, event, context):
        """
        Handle events in ERROR state
        
        Only accepted event:
        - reset_command: Reset to IDLE (requires fault_cleared guard)
        
        All other events ignored while in ERROR state
        """
        event_type = event.get('type')
        
        if event_type == 'reset_command':
            # Check if fault has been cleared
            fault_cleared = context.get('fault_cleared', False)
            
            if fault_cleared:
                logger.info("  Reset command accepted - fault cleared")
                logger.info("  Transitioning to IDLE")
                
                # Additional safety checks could be added here
                return StateType.IDLE
            else:
                logger.warning("  Reset command rejected - fault not cleared")
                logger.warning("  Please clear fault condition before reset")
                return None
        
        elif event_type == 'acknowledge_error':
            # Operator acknowledged error
            context['error_acknowledged'] = True
            logger.info("  Error acknowledged by operator")
            return None
        
        else:
            # All other events ignored in ERROR state
            logger.debug(f"  Event '{event_type}' ignored in ERROR state (safety locked)")
            return None
