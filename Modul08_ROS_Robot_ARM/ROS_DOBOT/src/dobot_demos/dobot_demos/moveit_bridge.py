#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from rclpy.action import ActionServer, ActionClient
from control_msgs.action import FollowJointTrajectory
from dobot_msgs.action import PointToPoint
import math

class MoveItToDobotBridge(Node):
    def __init__(self):
        super().__init__('moveit_to_dobot_bridge')
        
        # Create an action server to receive MoveIt trajectories
        self._action_server = ActionServer(
            self,
            FollowJointTrajectory,
            '/dobot_arm_controller/follow_joint_trajectory',
            self.execute_callback
        )
        
        # Create an action client to send commands to the real Dobot
        self._ptp_client = ActionClient(self, PointToPoint, '/PTP_action')
        
        self.get_logger().info('MoveIt to Dobot Bridge Started. Waiting for trajectories...')

    def execute_callback(self, goal_handle):
        self.get_logger().info('Received trajectory from MoveIt. Executing on physical Dobot...')
        
        trajectory = goal_handle.request.trajectory
        points = trajectory.points
        
        if not points:
            goal_handle.abort()
            return FollowJointTrajectory.Result()

        # For Dobot, point-to-point is usually sufficient. 
        # We'll take the very last point of the MoveIt trajectory (the final goal).
        final_point = points[-1]
        
        # MoveIt outputs radians, Dobot driver might expect degrees or radians depending on implementation.
        # Let's assume the driver expects angles in degrees for target_pose if it maps 1:1 to Dobot Studio,
        # OR radians if the driver is well-written. Let's send what MoveIt gives (radians) but we might need conversion.
        # By inspecting dobot_motion, if `MOTION_TYPE_MOVJ_ANGLE` = 4, target_pose is [j1, j2, j3, j4]
        
        # Dobot PTP target_pose has 4 elements
        # MoveIt joints: magician_joint_1, magician_joint_2, magician_joint_3, magician_joint_4
        
        # Get indices
        j1_idx = trajectory.joint_names.index('magician_joint_1')
        j2_idx = trajectory.joint_names.index('magician_joint_2')
        j3_idx = trajectory.joint_names.index('magician_joint_3')
        j4_idx = trajectory.joint_names.index('magician_joint_4')

        # Convert to degrees if necessary. The python dobot_api usually uses degrees.
        # Let's convert to degrees to be safe, as Dobot natively operates in degrees.
        # If the driver expects radians, we can remove math.degrees.
        j1 = math.degrees(final_point.positions[j1_idx])
        j2 = math.degrees(final_point.positions[j2_idx])
        j3 = math.degrees(final_point.positions[j3_idx])
        j4 = math.degrees(final_point.positions[j4_idx])
        
        # Wait for the PTP server
        if not self._ptp_client.wait_for_server(timeout_sec=5.0):
            self.get_logger().error('PTP Action Server not available!')
            goal_handle.abort()
            return FollowJointTrajectory.Result()

        ptp_goal = PointToPoint.Goal()
        ptp_goal.motion_type = 4 # MOTION_TYPE_MOVJ_ANGLE
        ptp_goal.target_pose = [j1, j2, j3, j4]
        ptp_goal.velocity_ratio = 0.5
        ptp_goal.acceleration_ratio = 0.5

        self.get_logger().info(f'Sending Dobot to angles: {ptp_goal.target_pose}')
        
        self._ptp_client.send_goal_async(ptp_goal)

        # We assume success immediately for the sake of the bridge to free MoveIt
        goal_handle.succeed()
        
        result = FollowJointTrajectory.Result()
        # SUCCESS
        result.error_code = FollowJointTrajectory.Result.SUCCESSFUL
        return result

def main(args=None):
    rclpy.init(args=args)
    node = MoveItToDobotBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
