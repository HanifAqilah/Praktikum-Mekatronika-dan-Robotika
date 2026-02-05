#!/usr/bin/env python3
"""
WORKING State Implementation
Robot performing a task (pick, place, weld, etc.)
"""

from .base_state import State
from ..state_machine_controller import StateType
import logging

logger = logging.getLogger(__name__)


class WorkingState(State):
    """WORKING state - performing task"""
    
    def __init__(self):
        super().__init__("WORKING")
    
    def on_enter(self, context):
        """Entry action: start task execution"""
        super().on_enter(context)
        
        logger.info("→ Entered WORKING state")
        
        task_type = context.get('task_type', 'unknown')
        task_params = context.get('task_params', {})
        
        logger.info(f"  Starting task: {task_type}")
        logger.debug(f"  Task parameters: {task_params}")
        
        # Initialize task status
        context['task_in_progress'] = True
        context['task_start_time'] = self.enter_time
        
        # In real implementation, execute task sequence here
        # For example:
        # - pick: close gripper, lift
        # - place: lower, open gripper
        # - weld: activate torch, move pattern
    
    def on_exit(self, context):
        """Exit action: log task completion"""
        logger.info("← Leaving WORKING state")
        
        import time
        task_duration = time.time() - context.get('task_start_time', time.time())
        
        logger.info(f"  Task completed in {task_duration:.2f}s")
        
        # Clear task info
        context['task_in_progress'] = False
        context.pop('task_type', None)
        context.pop('task_params', None)
    
    def handle_event(self, event, context):
        """
        Handle events in WORKING state
        
        Accepted events:
        - task_complete: Task finished successfully → IDLE or READY
        - task_failed: Task failed → ERROR
        - emergency_stop: Emergency → ERROR
        """
        event_type = event.get('type')
        
        if event_type == 'task_complete':
            logger.info("  Task completed successfully")
            
            # Log result
            result = event.get('data', {}).get('result', 'success')
            context['last_task_result'] = result
            
            logger.info(f"  Task result: {result}")
            
            # Check if there's a next move or return to idle
            if context.get('next_target'):
                logger.info("  Moving to next target")
                return StateType.MOVING
            else:
                logger.info("  Returning to IDLE")
                return StateType.IDLE
        
        elif event_type == 'task_failed':
            logger.error("  Task failed!")
            
            error_msg = event.get('data', {}).get('error', 'Task execution failed')
            context['has_error'] = True
            context['error_message'] = error_msg
            
            logger.error(f"  Error: {error_msg}")
            
            return StateType.ERROR
        
        elif event_type == 'emergency_stop':
            logger.error("  Emergency stop during task")
            context['has_error'] = True
            context['error_message'] = "Emergency stop during task execution"
            return StateType.ERROR
        
        else:
            logger.debug(f"  Event '{event_type}' ignored in WORKING state")
            return None
