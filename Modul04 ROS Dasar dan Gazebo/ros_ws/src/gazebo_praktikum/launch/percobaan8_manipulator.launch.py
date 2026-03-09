"""
PERCOBAAN 8 – Robot Manipulator 3-DOF dengan ros2_control
===========================================================
Tujuan:
  - Mensimulasikan robot lengan 3 joint di Gazebo (model) + RViz2 (animasi)
  - Mengontrol posisi joint dengan JointGroupPositionController
  - Mengirim perintah lewat topik ROS 2
  - Hardware: mock_components/GenericSystem (tidak perlu gazebo_ros2_control)

Cara menjalankan:
  ros2 launch gazebo_praktikum percobaan8_manipulator.launch.py

Kontrol manual joint (terminal baru), contoh:
  ros2 topic pub /manipulator/joint_1_position_controller/commands \
    std_msgs/msg/Float64MultiArray "data: [1.57]" --once

  ros2 topic pub /manipulator/joint_2_position_controller/commands \
    std_msgs/msg/Float64MultiArray "data: [0.5]" --once

  ros2 topic pub /manipulator/joint_3_position_controller/commands \
    std_msgs/msg/Float64MultiArray "data: [-1.0]" --once

Demo otomatis (script bawaan):
  Script demo_manipulator.py sudah berjalan otomatis setelah launch.

Catatan:
  Gerakan joint terlihat di RViz2. Model Gazebo bersifat statis (tidak perlu
  ros-humble-gazebo-ros2-control untuk percobaan ini).
"""

import os
import xacro
from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, ExecuteProcess,
                             IncludeLaunchDescription, RegisterEventHandler,
                             TimerAction)
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    pkg_share    = FindPackageShare('gazebo_praktikum').find('gazebo_praktikum')
    gazebo_share = FindPackageShare('gazebo_ros').find('gazebo_ros')

    xacro_file  = os.path.join(pkg_share, 'urdf',   'robot_manipulator.urdf.xacro')
    ctrl_file   = os.path.join(pkg_share, 'config', 'manipulator_controllers.yaml')
    rviz_file   = os.path.join(pkg_share, 'rviz',   'percobaan8_manipulator.rviz')

    arg_gui    = DeclareLaunchArgument('gui', default_value='true')

    robot_description = ParameterValue(
        xacro.process_file(xacro_file).toxml(),
        value_type=str
    )

    # ── Bersihkan proses Gazebo lama ─────────────────────────────────────────
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
        launch_arguments={'gui': LaunchConfiguration('gui')}.items(),
    )
    gazebo_after_kill = RegisterEventHandler(
        OnProcessExit(target_action=kill_stale, on_exit=[gazebo])
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

    # ── Spawn manipulator ─────────────────────────────────────────────────────
    spawn_entity = TimerAction(
        period=2.0,
        actions=[Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            name='spawn_manipulator',
            arguments=[
                '-entity', 'robot_manipulator',
                '-topic',  '/robot_description',
                '-x', '0.0', '-y', '0.0', '-z', '0.0',
            ],
            output='screen',
        )]
    )

    # ── Controller Manager (standalone, mock_components tidak butuh Gazebo plugin) ──
    controller_manager = Node(
        package='controller_manager',
        executable='ros2_control_node',
        namespace='manipulator',
        parameters=[
            {'robot_description': robot_description},
            ctrl_file,
        ],
        output='screen',
    )

    # ── Spawn controllers ────────────────────────────────────────────────────
    joint_state_broadcaster = TimerAction(
        period=4.0,
        actions=[Node(
            package='controller_manager',
            executable='spawner',
            namespace='manipulator',
            arguments=['joint_state_broadcaster'],
            output='screen',
        )]
    )

    joint1_ctrl = TimerAction(
        period=5.0,
        actions=[Node(
            package='controller_manager',
            executable='spawner',
            namespace='manipulator',
            arguments=['joint_1_position_controller'],
        )]
    )
    joint2_ctrl = TimerAction(
        period=5.5,
        actions=[Node(
            package='controller_manager',
            executable='spawner',
            namespace='manipulator',
            arguments=['joint_2_position_controller'],
        )]
    )
    joint3_ctrl = TimerAction(
        period=6.0,
        actions=[Node(
            package='controller_manager',
            executable='spawner',
            namespace='manipulator',
            arguments=['joint_3_position_controller'],
        )]
    )

    # ── Script demo gerak ──────────────────────────────────────────────────────
    demo_script = TimerAction(
        period=8.0,
        actions=[Node(
            package='gazebo_praktikum',
            executable='demo_manipulator.py',
            name='demo_gerak_manipulator',
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
        arg_gui,
        kill_stale,
        gazebo_after_kill,
        robot_state_publisher,
        spawn_entity,
        controller_manager,
        joint_state_broadcaster,
        joint1_ctrl, joint2_ctrl, joint3_ctrl,
        demo_script,
        rviz2,
    ])
