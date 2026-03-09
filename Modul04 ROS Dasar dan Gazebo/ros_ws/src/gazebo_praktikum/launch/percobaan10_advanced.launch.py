"""
PERCOBAAN 10 – Plugin Custom C++ & Advanced Environment
=========================================================
Tujuan:
  - Membuat dan mengintegrasikan World Plugin Gazebo dengan C++
  - Plugin mempublikasikan waktu simulasi ke topik ROS 2
  - Spawn objek secara dinamis menggunakan Gazebo service

Prasyarat – build plugin dulu:
  cd "$HOME/Documents/Praktikum-Mekatronika-dan-Robotika/Modul04 ROS Dasar dan Gazebo/ros_ws"
  source /opt/ros/humble/setup.bash
  colcon build --packages-select gazebo_praktikum
  source install/setup.bash

Cara menjalankan:
  ros2 launch gazebo_praktikum percobaan10_advanced.launch.py

Topik plugin:
  /plugin/sim_time   (std_msgs/Float64) - waktu simulasi
  /plugin/status     (std_msgs/String)  - status world

Monitor plugin:
  ros2 topic echo /plugin/sim_time
  ros2 topic echo /plugin/status
"""

import os
import xacro
from ament_index_python.packages import get_package_prefix
from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, ExecuteProcess,
                             IncludeLaunchDescription, RegisterEventHandler,
                             SetEnvironmentVariable, TimerAction)
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    pkg_share    = FindPackageShare('gazebo_praktikum').find('gazebo_praktikum')
    gazebo_share = FindPackageShare('gazebo_ros').find('gazebo_ros')

    xacro_file = os.path.join(pkg_share, 'urdf',   'robot_lengkap.urdf.xacro')
    world_file = os.path.join(pkg_share, 'worlds', 'percobaan10_advanced_world.world')
    rviz_file  = os.path.join(pkg_share, 'rviz',   'percobaan10_advanced.rviz')

    # Tambahkan path plugin ke GAZEBO_PLUGIN_PATH (dari install directory)
    plugin_install_lib = os.path.join(get_package_prefix('gazebo_praktikum'), 'lib')
    plugin_path = SetEnvironmentVariable(
        name='GAZEBO_PLUGIN_PATH',
        value=[plugin_install_lib]
    )

    arg_gui     = DeclareLaunchArgument('gui',     default_value='true')
    arg_verbose = DeclareLaunchArgument('verbose', default_value='true')

    robot_description = ParameterValue(
        xacro.process_file(xacro_file).toxml(),
        value_type=str
    )

    kill_stale = ExecuteProcess(
        cmd=['bash', '-c',
             'pkill -9 -x gzserver 2>/dev/null; pkill -9 -x gzclient 2>/dev/null; true'],
        output='screen',
    )

    # ── Gazebo ────────────────────────────────────────────────────────────────
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_share, 'launch', 'gazebo.launch.py')
        ),
        launch_arguments={
            'world':   world_file,
            'gui':     LaunchConfiguration('gui'),
            'verbose': LaunchConfiguration('verbose'),
        }.items(),
    )

    # ── Robot State Publisher ─────────────────────────────────────────────────
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description,
            'publish_frequency': 50.0,
            'use_sim_time': True,
        }],
    )

    joint_state_publisher = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
    )

    # ── Spawn robot ───────────────────────────────────────────────────────────
    spawn_entity = TimerAction(
        period=3.0,
        actions=[Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=[
                '-entity', 'robot_advanced',
                '-topic',  '/robot_description',
                '-x', '0.0', '-y', '0.0', '-z', '0.05',
            ],
            output='screen',
        )]
    )

    # ── Script Python: spawn objek dinamis ───────────────────────────────────
    advanced_demo = TimerAction(
        period=5.0,
        actions=[Node(
            package='gazebo_praktikum',
            executable='advanced_demo.py',
            name='advanced_demo',
            output='screen',
        )]
    )

    # ── RViz2 ─────────────────────────────────────────────────────────────────
    rviz2 = TimerAction(
        period=4.0,
        actions=[Node(
            package='rviz2',
            executable='rviz2',
            arguments=['-d', rviz_file],
            output='screen',
        )]
    )

    return LaunchDescription([
        plugin_path,
        arg_gui, arg_verbose,
        kill_stale,
        RegisterEventHandler(OnProcessExit(target_action=kill_stale, on_exit=[gazebo])),
        robot_state_publisher,
        joint_state_publisher,
        spawn_entity,
        advanced_demo,
        rviz2,
    ])
