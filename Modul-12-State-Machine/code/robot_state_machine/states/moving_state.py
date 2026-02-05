#!/usr/bin/env python3
"""
MOVING State Implementation
Robot executing trajectory to target position
"""

from .base_state import State
from ..state_machine_controller import StateType
import logging

logger = logging.getLogger(__name__)


class MovingState(State):
    """MOVING state - executing trajectory"""
    
    def __init__(self):
        super().__init__("MOVING")
    
    def on_enter(self, context):
        """Entry action: start trajectory execution"""
        super().on_enter(context)
        
        logger.info("→ Entered MOVING state")
        
        target = context.get('target_position', [0, 0, 0])
        speed = context.get('move_speed', 1.0)
        
        logger.info(f"  Moving to: {target} at speed {speed}")
        
        # Initialize movement status
        context['movement_started'] = True
        context['current_position'] = context.get('home_position', [0, 0, 0])
        
        # In real implementation, send movement commands to motors here
    
    def on_exit(self, context):
        """Exit action: stop motors"""
        logger.info("← Leaving MOVING state")
        
        # Stop motors
        logger.debug("  Stopping motors")
        context['movement_started'] = False
    
    def handle_event(self, event, context):
        """
        Handle events in MOVING state
        
        Accepted events:
        - reached_target: Movement complete → READY or WORKING
        - path_blocked: Obstacle detected → ERROR
        - abort_command: User abort → READY
        - emergency_stop: Emergency → ERROR
        """
        event_type = event.get('type')
        
        if event_type == 'reached_target':
            target = context.get('target_position', [0, 0, 0])
            logger.info(f"  Target reached: {target}")
            
            # Update current position
            context['current_position'] = target
            
            # Check if there's a task to perform
            if context.get('task_type'):
                logger.info("  Transitioning to WORKING to perform task")
                return StateType.WORKING
            else:
                logger.info("  Movement complete, returning to READY")
                return StateType.READY
        
        elif event_type == 'path_blocked':
            logger.error("  Path blocked detected!")
            context['has_error'] = True
            context['error_message'] = "Path blocked during movement"
            
            # Emergency stop
            return StateType.ERROR
        
        elif event_type == 'abort_command':
            logger.warning("  Movement aborted by user")
            
            # Stop at current position
            # In real implementation, stop motors gracefully
            
            return StateType.READY
        
        elif event_type == 'emergency_stop':
            logger.error("  Emergency stop during movement")
            context['has_error'] = True
            context['error_message'] = "Emergency stop during movement"
            return StateType.ERROR
        
        else:
            logger.debug(f"  Event '{event_type}' ignored in MOVING state")
            return None
