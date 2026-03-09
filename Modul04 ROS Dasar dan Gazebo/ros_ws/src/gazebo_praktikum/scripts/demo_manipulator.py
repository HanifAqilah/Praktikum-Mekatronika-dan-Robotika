#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Demo Gerakan Manipulator (Percobaan 8) – ROS 2 Humble
=======================================================
Script ini mengirimkan perintah posisi joint secara berurutan
untuk mendemonstrasikan gerakan robot lengan 3-DOF di Gazebo.

Topik yang digunakan (ros2_control JointGroupPositionController):
  /manipulator/joint_1_position_controller/commands  (std_msgs/Float64MultiArray)
  /manipulator/joint_2_position_controller/commands  (std_msgs/Float64MultiArray)
  /manipulator/joint_3_position_controller/commands  (std_msgs/Float64MultiArray)
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
import math
import time


class DemoManipulator(Node):
    def __init__(self):
        super().__init__('demo_gerak_manipulator')

        ns = '/manipulator'
        self.pub_j1 = self.create_publisher(
            Float64MultiArray, f'{ns}/joint_1_position_controller/commands', 10)
        self.pub_j2 = self.create_publisher(
            Float64MultiArray, f'{ns}/joint_2_position_controller/commands', 10)
        self.pub_j3 = self.create_publisher(
            Float64MultiArray, f'{ns}/joint_3_position_controller/commands', 10)

        self.get_logger().info('Demo Manipulator dimulai. Tunggu 3 detik...')
        time.sleep(3.0)

        self._run_demo()

    def _publish(self, j1: float, j2: float, j3: float):
        def _msg(val):
            m = Float64MultiArray()
            m.data = [val]
            return m
        self.pub_j1.publish(_msg(j1))
        self.pub_j2.publish(_msg(j2))
        self.pub_j3.publish(_msg(j3))

    def _run_demo(self):
        # Urutan gerakan demonstrasi: (joint1, joint2, joint3, tahan_detik, keterangan)
        posisi_demo = [
            (0.0,   0.0,   0.0,   2.0, 'Posisi Home'),
            (0.0,   0.5,  -1.0,   2.0, 'Reach depan'),
            (1.57,  0.5,  -1.0,   2.0, 'Rotate kiri'),
            (1.57,  0.8,  -1.5,   2.0, 'Turunkan lengan'),
            (-1.57, 0.5,  -1.0,   2.0, 'Rotate kanan'),
            (-1.57, 0.8,  -1.5,   2.0, 'Turunkan lengan kanan'),
            (0.0,   0.0,   0.0,   2.0, 'Kembali Home'),
        ]

        self.get_logger().info('Memulai urutan gerakan demo...')
        for idx, (j1, j2, j3, tahan, ket) in enumerate(posisi_demo):
            if not rclpy.ok():
                return
            self.get_logger().info(
                f'Langkah {idx+1}: {ket}  '
                f'J1={j1:.2f}  J2={j2:.2f}  J3={j3:.2f} rad')
            self._publish(j1, j2, j3)
            time.sleep(tahan)

        # Gerakan sinusoidal berkelanjutan
        self.get_logger().info('Memulai gerakan sinusoidal...')
        t_start = time.monotonic()
        rate_ns = 100_000_000  # 10 Hz dalam nanosecond

        while rclpy.ok():
            t = time.monotonic() - t_start
            j1 = math.sin(0.5 * t) * math.pi / 2.0
            j2 = math.sin(0.3 * t + 1.0) * 0.5
            j3 = -abs(math.sin(0.4 * t)) * 1.5
            self._publish(j1, j2, j3)
            time.sleep(0.1)  # 10 Hz


def main(args=None):
    rclpy.init(args=args)
    node = DemoManipulator()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
