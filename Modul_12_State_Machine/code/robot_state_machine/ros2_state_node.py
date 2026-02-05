#!/usr/bin/env python3
"""
ROS2 State Machine Node
Integrates state machine with ROS2
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_srvs.srv import Trigger, SetBool
import json
import threading

from .state_machine_controller import StateMachineController, StateType
from .states.idle_state import IdleState
from .states.homing_state import HomingState
from .states.ready_state import ReadyState
from .states.moving_state import MovingState
from .states.working_state import WorkingState
from .states.error_state import ErrorState


class RobotStateNode(Node):
    """
    ROS2 node that wraps the state machine controller
    Provides topics and services for state control
    """
    
    def __init__(self):
        super().__init__('robot_state_machine')
        
        # Create state machine
        self.state_machine = StateMachineController()
        
        # Register all states
        self.state_machine.add_state(StateType.IDLE, IdleState())
        self.state_machine.add_state(StateType.HOMING, HomingState())
        self.state_machine.add_state(StateType.READY, ReadyState())
        self.state_machine.add_state(StateType.MOVING, MovingState())
        self.state_machine.add_state(StateType.WORKING, WorkingState())
        self.state_machine.add_state(StateType.ERROR, ErrorState())
        
        # Set initial state
        self.state_machine.set_initial_state(StateType.IDLE)
        
        # ROS2 Publishers
        self.state_pub = self.create_publisher(String, 'robot/state', 10)
        self.event_pub = self.create_publisher(String, 'robot/events', 10)
        self.metrics_pub = self.create_publisher(String, 'robot/metrics', 10)
        
        # ROS2 Subscribers
        self.command_sub = self.create_subscription(
            String,
            'robot/command',
            self.command_callback,
            10
        )
        
        # ROS2 Services
        self.reset_service = self.create_service(
            Trigger,
            'robot/reset',
            self.reset_callback
        )
        
        self.emergency_stop_service = self.create_service(
            Trigger,
            'robot/emergency_stop',
            self.emergency_stop_callback
        )
        
        # Timers
        self.create_timer(0.5, self.publish_state)  # 2 Hz
        self.create_timer(1.0, self.publish_metrics)  # 1 Hz
        
        # Start state machine
        self.state_machine.start()
        
        self.get_logger().info("Robot State Machine Node started")
        self.get_logger().info(f"Initial state: {self.state_machine.get_current_state()}")
    
    def command_callback(self, msg):
        """
        Receive commands via ROS2 topic
        
        Expected JSON format:
        {
            "command": "move_command",
            "priority": 2,
            "data": {
                "target": [100, 50, 20],
                "speed": 1.0
            }
        }
        """
        try:
            command = json.loads(msg.data)
            
            # Create event
            event = {
                'type': command.get('command'),
                'data': command.get('data', {}),
                'timestamp': self.get_clock().now().to_msg()
            }
            
            priority = command.get('priority', 5)
            
            # Post to state machine
            success = self.state_machine.post_event(event, priority)
            
            if success:
                self.get_logger().info(f"Command received: {command.get('command')}")
                
                # Publish event notification
                event_msg = String()
                event_msg.data = json.dumps({
                    'type': 'command_received',
                    'command': command.get('command'),
                    'queued': True
                })
                self.event_pub.publish(event_msg)
            else:
                self.get_logger().warn(f"Command dropped (queue full): {command.get('command')}")
        
        except json.JSONDecodeError as e:
            self.get_logger().error(f"Invalid JSON: {msg.data} - {e}")
        except Exception as e:
            self.get_logger().error(f"Error processing command: {e}")
    
    def reset_callback(self, request, response):
        """
        Service to reset state machine to IDLE
        """
        try:
            # Check if in ERROR state
            current_state = self.state_machine.get_current_state()
            
            if current_state == "ERROR":
                # Clear fault
                self.state_machine.context['has_error'] = False
                self.state_machine.context['fault_cleared'] = True
                
                # Post reset event
                event = {'type': 'reset_command'}
                self.state_machine.post_event(event, priority=1)
                
                response.success = True
                response.message = f"Reset initiated from {current_state}"
            else:
                response.success = False
                response.message = f"Cannot reset from {current_state} (must be in ERROR state)"
            
        except Exception as e:
            response.success = False
            response.message = f"Reset failed: {str(e)}"
            self.get_logger().error(f"Reset service error: {e}")
        
        return response
    
    def emergency_stop_callback(self, request, response):
        """
        Service to trigger emergency stop
        """
        try:
            # Post emergency stop event with highest priority
            event = {'type': 'emergency_stop'}
            self.state_machine.post_event(event, priority=0)
            
            response.success = True
            response.message = "Emergency stop triggered"
            
            self.get_logger().warn("EMERGENCY STOP triggered via service")
        
        except Exception as e:
            response.success = False
            response.message = f"Emergency stop failed: {str(e)}"
            self.get_logger().error(f"Emergency stop service error: {e}")
        
        return response
    
    def publish_state(self):
        """
        Publish current state periodically
        """
        try:
            state_msg = String()
            
            state_data = {
                'state': self.state_machine.get_current_state(),
                'context': {
                    k: v for k, v in self.state_machine.context.items()
                    if isinstance(v, (int, float, str, bool, list))
                },
                'timestamp': self.get_clock().now().to_msg().sec,
                'queue_size': self.state_machine.event_queue.qsize()
            }
            
            state_msg.data = json.dumps(state_data)
            self.state_pub.publish(state_msg)
        
        except Exception as e:
            self.get_logger().error(f"Error publishing state: {e}")
    
    def publish_metrics(self):
        """
        Publish metrics periodically
        """
        try:
            metrics_msg = String()
            
            metrics = self.state_machine.get_metrics()
            metrics['timestamp'] = self.get_clock().now().to_msg().sec
            
            metrics_msg.data = json.dumps(metrics)
            self.metrics_pub.publish(metrics_msg)
        
        except Exception as e:
            self.get_logger().error(f"Error publishing metrics: {e}")
    
    def destroy_node(self):
        """Clean shutdown"""
        self.get_logger().info("Shutting down state machine node")
        
        # Stop state machine
        self.state_machine.stop()
        
        # Save log
        self.state_machine.save_log('state_machine_log.json')
        
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    
    node = RobotStateNode()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
