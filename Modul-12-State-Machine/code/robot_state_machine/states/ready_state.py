#!/usr/bin/env python3
"""
READY State Implementation
Robot is homed and ready to receive commands
"""

from .base_state import State
from ..state_machine_controller import StateType
import logging

logger = logging.getLogger(__name__)


class ReadyState(State):
    """READY state - waiting for commands"""
    
    def __init__(self):
        super().__init__("READY")
    
    def on_enter(self, context):
        """Entry action: publish ready status"""
        super().on_enter(context)
        
        logger.info("→ Entered READY state")
        logger.info("  Robot ready for commands")
        
        # Publish ready status (in ROS2 implementation)
        context['robot_ready'] = True
    
    def on_exit(self, context):
        """Exit action: clear ready flag"""
        logger.info("← Leaving READY state")
        context['robot_ready'] = False
    
    def handle_event(self, event, context):
        """
        Handle events in READY state
        
        Accepted events:
        - move_command: Move to position → MOVING (with guards)
        - task_command: Execute task → WORKING (with guards)
        - idle_command: Return to IDLE
        - return_home: Re-home → HOMING
        - emergency_stop: Emergency → ERROR
        """
        event_type = event.get('type')
        
        if event_type == 'move_command':
            # Check guard conditions
            if not self._check_move_guards(event, context):
                logger.warning("  Move command rejected by guard conditions")
                return None
            
            # Save target
            context['target_position'] = event.get('data', {}).get('target', [0, 0, 0])
            context['move_speed'] = event.get('data', {}).get('speed', 1.0)
            
            logger.info(f"  Move command accepted: target={context['target_position']}")
            return StateType.MOVING
        
        elif event_type == 'task_command':
            # Check guard conditions
            if not self._check_task_guards(event, context):
                logger.warning("  Task command rejected by guard conditions")
                return None
            
            # Save task info
            context['task_type'] = event.get('data', {}).get('task_type', 'unknown')
            context['task_params'] = event.get('data', {}).get('params', {})
            
            logger.info(f"  Task command accepted: {context['task_type']}")
            return StateType.WORKING
        
        elif event_type == 'idle_command':
            logger.info("  Returning to IDLE")
            return StateType.IDLE
        
        elif event_type == 'return_home':
            logger.info("  Re-homing requested")
            return StateType.HOMING
        
        elif event_type == 'emergency_stop':
            logger.error("  Emergency stop in READY state")
            context['has_error'] = True
            return StateType.ERROR
        
        else:
            logger.debug(f"  Event '{event_type}' ignored in READY state")
            return None
    
    def _check_move_guards(self, event, context):
        """
        Check guard conditions for move command
        
        Guards:
        - motors_enabled
        - path_clear
        - no_error
        - target_valid
        - battery_sufficient
        """
        guards = {}
        
        # Motors enabled
        guards['motors_enabled'] = context.get('motors_enabled', False)
        
        # Path clear
        guards['path_clear'] = context.get('path_clear', True)
        
        # No errors
        guards['no_error'] = not context.get('has_error', False)
        
        # Target valid
        target = event.get('data', {}).get('target', [])
        guards['target_valid'] = self._validate_target(target)
        
        # Battery sufficient
        guards['battery_sufficient'] = context.get('battery_level', 100) > 20
        
        # Log guard results
        logger.debug("  Guard checks:")
        for guard_name, result in guards.items():
            status = "✓" if result else "✗"
            logger.debug(f"    {status} {guard_name}: {result}")
        
        # All guards must pass
        return all(guards.values())
    
    def _check_task_guards(self, event, context):
        """
        Check guard conditions for task command
        
        Guards:
        - motors_enabled
        - tool_ready
        - workspace_clear
        - no_error
        """
        guards = {}
        
        guards['motors_enabled'] = context.get('motors_enabled', False)
        guards['tool_ready'] = context.get('tool_ready', True)
        guards['workspace_clear'] = context.get('workspace_clear', True)
        guards['no_error'] = not context.get('has_error', False)
        
        logger.debug("  Task guard checks:")
        for guard_name, result in guards.items():
            status = "✓" if result else "✗"
            logger.debug(f"    {status} {guard_name}: {result}")
        
        return all(guards.values())
    
    def _validate_target(self, target):
        """
        Validate target position within workspace limits
        
        Workspace limits (example):
        x: [-300, 300] mm
        y: [-300, 300] mm
        z: [0, 400] mm
        """
        if not target or len(target) != 3:
            return False
        
        x, y, z = target
        
        # Check limits
        x_valid = -300 <= x <= 300
        y_valid = -300 <= y <= 300
        z_valid = 0 <= z <= 400
        
        return x_valid and y_valid and z_valid
