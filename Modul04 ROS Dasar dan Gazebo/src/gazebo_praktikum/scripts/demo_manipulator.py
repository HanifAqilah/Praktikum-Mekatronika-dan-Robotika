#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Demo Gerakan Manipulator (Percobaan 10) - ROS 2 Humble
========================================================
Script ini mengirimkan perintah posisi joint secara berurutan
dan dengan interpolasi halus untuk mendemonstrasikan gerakan robot lengan 3-DOF.

Topik yang digunakan (ros2_control JointGroupPositionController):
  /arm_position_controller/commands  (std_msgs/Float64MultiArray)
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
import math
import time


class DemoManipulator(Node):
    def __init__(self):
        super().__init__("demo_gerak_manipulator")

        self.pub_arm = self.create_publisher(
            Float64MultiArray, "/arm_position_controller/commands", 10)

        # Posisi saat ini (dilacak secara internal)
        self.current_joints = [0.0, 0.0, 0.0]

        self.get_logger().info("Demo Manipulator dimulai. Tunggu 3 detik...")
        time.sleep(3.0)

        self._run_demo()

    def _publish(self, j1: float, j2: float, j3: float):
        m = Float64MultiArray()
        m.data = [j1, j2, j3]
        self.pub_arm.publish(m)

    def _smooth_move(self, target: list, duration: float = 2.0, hz: float = 50.0):
        """Gerak halus dari posisi saat ini ke target dengan interpolasi linear."""
        start = self.current_joints[:]
        steps = max(1, int(duration * hz))
        dt = 1.0 / hz
        for i in range(1, steps + 1):
            if not rclpy.ok():
                return
            t = i / steps
            interp = [start[k] + t * (target[k] - start[k]) for k in range(3)]
            self._publish(*interp)
            time.sleep(dt)
        self.current_joints = target[:]

    def _run_demo(self):
        # Urutan gerakan demonstrasi: (joint1, joint2, joint3, durasi_detik, keterangan)
        posisi_demo = [
            ([0.0,   0.0,   0.0],   2.0, "Posisi Home"),
            ([0.0,   0.5,  -1.0],   2.5, "Reach depan"),
            ([1.57,  0.5,  -1.0],   2.5, "Rotate kiri"),
            ([1.57,  0.8,  -1.5],   2.5, "Turunkan lengan kiri"),
            ([-1.57, 0.5,  -1.0],   3.0, "Rotate kanan"),
            ([-1.57, 0.8,  -1.5],   2.5, "Turunkan lengan kanan"),
            ([0.0,   0.0,   0.0],   2.5, "Kembali Home"),
        ]

        self.get_logger().info("Memulai urutan gerakan demo (gerakan halus)...")
        for idx, (target, durasi, ket) in enumerate(posisi_demo):
            if not rclpy.ok():
                return
            self.get_logger().info(
                f"Langkah {idx+1}: {ket}  "
                f"J=[{target[0]:.2f}, {target[1]:.2f}, {target[2]:.2f}] rad")
            self._smooth_move(target, duration=durasi)
            # Tahan 0.5 detik sebelum langkah berikutnya
            time.sleep(0.5)

        # Gerakan sinusoidal berkelanjutan
        self.get_logger().info("Memulai gerakan sinusoidal...")
        t_start = time.monotonic()
        dt = 0.02  # 50 Hz

        while rclpy.ok():
            t = time.monotonic() - t_start
            j1 = math.sin(0.5 * t) * math.pi / 2.0
            j2 = math.sin(0.3 * t + 1.0) * 0.5
            j3 = -abs(math.sin(0.4 * t)) * 1.5
            self._publish(j1, j2, j3)
            self.current_joints = [j1, j2, j3]
            time.sleep(dt)


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


if __name__ == "__main__":
    main()
