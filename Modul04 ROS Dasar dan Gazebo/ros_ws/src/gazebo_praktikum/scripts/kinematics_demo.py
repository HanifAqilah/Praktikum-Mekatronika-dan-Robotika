#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Demo Kinematika Manipulator (Percobaan 11) - ROS 2 Humble
============================================================
Mendemonstrasikan:
  FASE 1 - Forward Kinematics (FK):
    Arm bergerak ke beberapa konfigurasi sudut joint dengan Python,
    lalu menghitung dan menampilkan posisi end-effector dari FK.

  FASE 2 - Inverse Kinematics (IK) Halus:
    Diberikan target posisi Cartesian, hitung IK secara analitik,
    lalu gerakkan arm secara halus (interpolasi linear 50 Hz).

Topik:
  /arm_position_controller/commands - perintah joint (publish)
  /kinematics/end_effector          - Marker posisi EE (publish)
  /kinematics/target                - Marker posisi target IK (publish)

Geometri manipulator (dari robot_manipulator.urdf.xacro):
  BASE_H = 0.10 m  (tinggi base)
  L1     = 0.30 m  (link_1 vertikal)
  L2     = 0.25 m  (link_2 horizontal)
  L3     = 0.20 m  (link_3 horizontal)
  Bahu ada di z = BASE_H + L1 = 0.40 m
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
from visualization_msgs.msg import Marker
import math
import time

# ── Konstanta lengan ──────────────────────────────────────────────────────────
BASE_H = 0.10
L1     = 0.30
L2     = 0.25
L3     = 0.20
SHOULDER_Z = BASE_H + L1          # 0.40 m - tinggi bahu dari dunia


# ── Kinematika ────────────────────────────────────────────────────────────────
def forward_kinematics(q1: float, q2: float, q3: float):
    """Hitung posisi (x, y, z) end-effector dari sudut joint (rad)."""
    r = L2 * math.cos(q2) + L3 * math.cos(q2 + q3)
    z = SHOULDER_Z + L2 * math.sin(q2) + L3 * math.sin(q2 + q3)
    x = r * math.cos(q1)
    y = r * math.sin(q1)
    return x, y, z


def inverse_kinematics(tx: float, ty: float, tz: float):
    """
    Hitung sudut joint dari target posisi end-effector (meters).
    Returns (q1, q2, q3) atau None jika di luar jangkauan.
    """
    q1 = math.atan2(ty, tx)

    r      = math.sqrt(tx**2 + ty**2)
    z_eff  = tz - SHOULDER_Z          # jarak vertikal dari bahu

    d_sq   = r**2 + z_eff**2
    d      = math.sqrt(d_sq)

    # Cek jangkauan
    if d > L2 + L3 - 1e-6 or d < abs(L2 - L3) + 1e-6:
        return None

    # Sudut siku (elbow down: q3 <= 0)
    cos_q3 = (d_sq - L2**2 - L3**2) / (2.0 * L2 * L3)
    cos_q3 = max(-1.0, min(1.0, cos_q3))
    q3     = -math.acos(cos_q3)      # negatif = elbow ke bawah

    # Sudut bahu
    alpha  = math.atan2(z_eff, r)
    beta   = math.atan2(L3 * math.sin(-q3), L2 + L3 * math.cos(-q3))
    q2     = alpha + beta

    # Cek batas joint  (joint_2: -pi/2..pi/2, joint_3: -2.0944..0)
    if not (-math.pi / 2 <= q2 <= math.pi / 2):
        return None
    if not (-2.0944 <= q3 <= 0.0):
        return None

    return q1, q2, q3


# ── Node utama ────────────────────────────────────────────────────────────────
class KinematicsDemo(Node):
    def __init__(self):
        super().__init__('kinematics_demo')

        self.pub_arm    = self.create_publisher(
            Float64MultiArray, '/arm_position_controller/commands', 10)
        self.pub_ee     = self.create_publisher(
            Marker, '/kinematics/end_effector', 10)
        self.pub_target = self.create_publisher(
            Marker, '/kinematics/target', 10)

        # Joint saat ini (dilacak secara internal)
        self._joints = [0.0, 0.0, 0.0]

        self.get_logger().info('=== Demo Kinematika Manipulator ===')
        self.get_logger().info('Tunggu 3 detik sebelum mulai...')
        time.sleep(3.0)

        self._run_demo()

    # ── helpers ────────────────────────────────────────────────────────────────
    def _send(self, q1: float, q2: float, q3: float):
        m = Float64MultiArray()
        m.data = [q1, q2, q3]
        self.pub_arm.publish(m)

    def _smooth_move(self, target: list, duration: float = 2.5, hz: float = 50.0):
        """Interpolasi linear dari posisi saat ini ke target."""
        start = self._joints[:]
        steps = max(1, int(duration * hz))
        dt    = 1.0 / hz
        for i in range(1, steps + 1):
            if not rclpy.ok():
                return
            t      = i / steps
            joints = [start[k] + t * (target[k] - start[k]) for k in range(3)]
            self._send(*joints)
            time.sleep(dt)
        self._joints = target[:]

    def _marker(self, pub, x, y, z, r, g, b, mid=0, scale=0.05):
        mk = Marker()
        mk.header.frame_id = 'world'
        mk.header.stamp    = self.get_clock().now().to_msg()
        mk.id              = mid
        mk.type            = Marker.SPHERE
        mk.action          = Marker.ADD
        mk.pose.position.x = x
        mk.pose.position.y = y
        mk.pose.position.z = z
        mk.pose.orientation.w = 1.0
        mk.scale.x = mk.scale.y = mk.scale.z = scale
        mk.color.a = 1.0
        mk.color.r = r
        mk.color.g = g
        mk.color.b = b
        pub.publish(mk)

    # ── FASE 1: Forward Kinematics ─────────────────────────────────────────────
    def _phase_fk(self):
        self.get_logger().info('\n' + '='*50)
        self.get_logger().info('FASE 1: Demo Forward Kinematics')
        self.get_logger().info('='*50)

        # Konfigurasi sudut joint yang akan didemonstrasikan
        configs = [
            ([0.0,   0.0,   0.0],   'Home'),
            ([0.0,   0.5,  -1.0],   'Reach depan bawah'),
            ([1.57,  0.5,  -1.0],   'Rotasi kiri - reach'),
            ([0.0,   0.8,  -1.5],   'Lean depan jauh'),
            ([-1.57, 0.3,  -0.6],   'Rotasi kanan - atas'),
            ([0.0,   0.0,   0.0],   'Kembali Home'),
        ]

        for target_q, label in configs:
            if not rclpy.ok():
                return
            # Gerak ke konfigurasi ini (langkah per langkah, tapi tidak perlu halus
            # karena tujuannya menunjukkan posisi berbeda)
            self._smooth_move(target_q, duration=2.0)

            # Hitung dan tampilkan FK
            x, y, z = forward_kinematics(*target_q)
            self.get_logger().info(
                f'  [{label}]'
                f'\n    Joint : q1={target_q[0]:.3f}  q2={target_q[1]:.3f}  q3={target_q[2]:.3f} rad'
                f'\n    EE    : x={x:.3f}  y={y:.3f}  z={z:.3f} m')

            # Marker hijau di posisi end-effector
            self._marker(self.pub_ee, x, y, z, 0.0, 1.0, 0.0, mid=0)
            time.sleep(2.0)  # Tahan supaya dapat dilihat di RViz2

    # ── FASE 2: Inverse Kinematics (halus) ─────────────────────────────────────
    def _phase_ik(self):
        self.get_logger().info('\n' + '='*50)
        self.get_logger().info('FASE 2: Demo Inverse Kinematics (Gerakan Halus)')
        self.get_logger().info('='*50)

        # Target Cartesian yang ingin dicapai arm
        targets = [
            (0.30,  0.00,  0.50, 'Depan atas'),
            (0.00,  0.30,  0.45, 'Kiri tengah'),
            (0.30,  0.00,  0.30, 'Depan bawah'),
            (0.00, -0.30,  0.45, 'Kanan tengah'),
            (0.35,  0.00,  0.42, 'Depan jauh'),
        ]

        for tx, ty, tz, label in targets:
            if not rclpy.ok():
                return

            # Marker merah di posisi target
            self._marker(self.pub_target, tx, ty, tz, 1.0, 0.0, 0.0, mid=1, scale=0.07)

            result = inverse_kinematics(tx, ty, tz)
            if result is None:
                self.get_logger().warn(
                    f'  [{label}] ({tx:.2f},{ty:.2f},{tz:.2f}) - DI LUAR JANGKAUAN, skip.')
                time.sleep(1.0)
                continue

            q1, q2, q3 = result
            x_fk, y_fk, z_fk = forward_kinematics(q1, q2, q3)
            err = math.sqrt((x_fk-tx)**2 + (y_fk-ty)**2 + (z_fk-tz)**2)

            self.get_logger().info(
                f'  [{label}] Target=({tx:.2f},{ty:.2f},{tz:.2f})'
                f'\n    IK    : q1={q1:.3f}  q2={q2:.3f}  q3={q3:.3f} rad'
                f'\n    FK    : ({x_fk:.3f},{y_fk:.3f},{z_fk:.3f})  err={err*1000:.2f} mm')

            # Gerak halus ke posisi IK
            self._smooth_move([q1, q2, q3], duration=3.0)

            # Marker hijau di posisi EE setelah sampai
            self._marker(self.pub_ee, x_fk, y_fk, z_fk, 0.0, 1.0, 0.0, mid=0)
            time.sleep(1.5)  # Tahan sebentar

    # ── Main demo ──────────────────────────────────────────────────────────────
    def _run_demo(self):
        self._phase_fk()

        if rclpy.ok():
            self.get_logger().info('\nBeralih ke Fase 2 dalam 2 detik...')
            time.sleep(2.0)
            self._phase_ik()

        # Kembali ke Home
        if rclpy.ok():
            self.get_logger().info('\nDemo selesai. Kembali ke Home...')
            self._smooth_move([0.0, 0.0, 0.0], duration=2.0)
            self.get_logger().info('Selesai.')


def main(args=None):
    rclpy.init(args=args)
    node = KinematicsDemo()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
