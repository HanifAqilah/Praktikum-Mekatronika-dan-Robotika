#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Advanced Demo – Spawn Objek Dinamis (Percobaan 10) – ROS 2 Humble
==================================================================
Mendemonstrasikan penggunaan Gazebo service untuk:
  1. Spawn model baru secara dinamis saat runtime
  2. Menghapus model
  3. Mengambil pose model (GetEntityState)
  4. Mengatur pose model (SetEntityState)

Service ROS 2 Humble (gazebo_ros_pkgs):
  /spawn_entity          – gazebo_msgs/srv/SpawnEntity
  /delete_entity         – gazebo_msgs/srv/DeleteEntity
  /gazebo/get_entity_state – gazebo_msgs/srv/GetEntityState
  /gazebo/set_entity_state – gazebo_msgs/srv/SetEntityState
"""

import rclpy
from rclpy.node import Node
from gazebo_msgs.srv import SpawnEntity, DeleteEntity, GetEntityState, SetEntityState
from geometry_msgs.msg import Pose, Point, Quaternion
from std_msgs.msg import String
import math
import time

# Template SDF untuk kotak sederhana
SDF_BOX_TEMPLATE = """<?xml version="1.0"?>
<sdf version="1.6">
  <model name="{name}">
    <static>false</static>
    <link name="link">
      <inertial>
        <mass>1.0</mass>
        <inertia>
          <ixx>0.0167</ixx><ixy>0</ixy><ixz>0</ixz>
          <iyy>0.0167</iyy><iyz>0</iyz>
          <izz>0.0167</izz>
        </inertia>
      </inertial>
      <collision name="col">
        <geometry><box><size>0.3 0.3 0.3</size></box></geometry>
      </collision>
      <visual name="vis">
        <geometry><box><size>0.3 0.3 0.3</size></box></geometry>
        <material>
          <ambient>{r} {g} {b} 1</ambient>
          <diffuse>{r} {g} {b} 1</diffuse>
          <specular>0.1 0.1 0.1 1</specular>
        </material>
      </visual>
    </link>
  </model>
</sdf>
"""


class AdvancedDemoNode(Node):
    def __init__(self):
        super().__init__('advanced_demo')

        self.pub_status = self.create_publisher(String, '/plugin/status', 10)

        # Buat service clients
        self.cli_spawn  = self.create_client(SpawnEntity,    '/spawn_entity')
        self.cli_delete = self.create_client(DeleteEntity,   '/delete_entity')
        self.cli_get    = self.create_client(GetEntityState, '/gazebo/get_entity_state')
        self.cli_set    = self.create_client(SetEntityState, '/gazebo/set_entity_state')

        self.get_logger().info('Advanced Demo: menunggu service Gazebo...')
        self._wait_for_services()
        self.get_logger().info('Semua service tersedia. Demo dimulai.')

        self.spawned_models: list[str] = []
        self.model_counter = 0
        self.t_start = time.monotonic()

        # Timer utama 1 Hz
        self.create_timer(1.0, self._loop)

    def _wait_for_services(self, timeout_sec: float = 30.0):
        for cli in [self.cli_spawn, self.cli_delete, self.cli_get, self.cli_set]:
            if not cli.wait_for_service(timeout_sec=timeout_sec):
                self.get_logger().warn(f'Service {cli.srv_name} tidak tersedia.')

    def _call_sync(self, client, request):
        """Memanggil service secara sinkron menggunakan spin_until_future_complete."""
        future = client.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=5.0)
        return future.result()

    def _loop(self):
        t = time.monotonic() - self.t_start

        # Spawn kotak baru setiap 5 detik (maks 5 kotak)
        if int(t) % 5 == 0 and self.model_counter < 5:
            self._spawn_kotak(self.model_counter)
            self.model_counter += 1

        # Animasi: gerakkan kotak secara sinusoidal
        self._animasi_kotak(t)

        pesan = String()
        pesan.data = f'[AdvancedDemo] t={t:.1f}s, model aktif={len(self.spawned_models)}'
        self.pub_status.publish(pesan)
        self.get_logger().info(pesan.data)

    def _spawn_kotak(self, idx: int):
        nama  = f'kotak_dinamis_{idx}'
        angle = idx * (2.0 * math.pi / 5.0)
        x     = 2.0 * math.cos(angle)
        y     = 2.0 * math.sin(angle)
        z     = 0.5

        r = ((idx * 51)  % 256) / 255.0
        g = ((idx * 133) % 256) / 255.0
        b = ((idx * 197) % 256) / 255.0

        sdf = SDF_BOX_TEMPLATE.format(name=nama, r=r, g=g, b=b)

        req = SpawnEntity.Request()
        req.name            = nama
        req.xml             = sdf
        req.robot_namespace = ''
        req.initial_pose    = Pose(
            position=Point(x=x, y=y, z=z),
            orientation=Quaternion(x=0.0, y=0.0, z=0.0, w=1.0))
        req.reference_frame = 'world'

        resp = self._call_sync(self.cli_spawn, req)
        if resp and resp.success:
            self.spawned_models.append(nama)
            self.get_logger().info(f'Spawned: {nama} di ({x:.1f}, {y:.1f})')
        else:
            msg = resp.status_message if resp else 'timeout'
            self.get_logger().warn(f'Gagal spawn {nama}: {msg}')

    def _animasi_kotak(self, t: float):
        for nama in list(self.spawned_models):
            req_get = GetEntityState.Request()
            req_get.name           = nama
            req_get.reference_frame = 'world'
            resp = self._call_sync(self.cli_get, req_get)
            if not resp or not resp.success:
                continue

            idx  = self.spawned_models.index(nama)
            fase = idx * (2.0 * math.pi / max(len(self.spawned_models), 1))

            new_pose = resp.state.pose
            new_pose.position.z = 0.5 + 0.3 * math.sin(t + fase)

            req_set = SetEntityState.Request()
            req_set.state.name           = nama
            req_set.state.pose           = new_pose
            req_set.state.reference_frame = 'world'
            self._call_sync(self.cli_set, req_set)


def main(args=None):
    rclpy.init(args=args)
    node = AdvancedDemoNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
