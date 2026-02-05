#!/usr/bin/env python3
"""
Base State Abstract Class
All states inherit from this class
"""

from abc import ABC, abstractmethod
import time

class State(ABC):
    """Abstract base class for all states"""
    
    def __init__(self, name):
        self.name = name
        self.enter_time = None
    
    @abstractmethod
    def on_enter(self, context):
        """
        Called when entering this state
        
        Args:
            context: Dictionary containing shared state machine context
        """
        self.enter_time = time.time()
    
    @abstractmethod
    def on_exit(self, context):
        """
        Called when exiting this state
        
        Args:
            context: Dictionary containing shared state machine context
        """
        pass
    
    @abstractmethod
    def handle_event(self, event, context):
        """
        Handle event in this state
        
        Args:
            event: Event dictionary with 'type' and optional 'data'
            context: Dictionary containing shared state machine context
        
        Returns:
            StateType enum if transition needed, None to stay in current state
        """
        pass
    
    def get_duration(self):
        """Get time spent in this state (seconds)"""
        if self.enter_time:
            return time.time() - self.enter_time
        return 0
    
    def __repr__(self):
        return f"State({self.name})"
