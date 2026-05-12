from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder

def generate_launch_description():
    moveit_config = (
        MoveItConfigsBuilder("magician", package_name="dobot_moveit_config")
        .robot_description(file_path="config/magician.urdf.xacro")
        .robot_description_semantic(file_path="config/magician.srdf")
        .planning_pipelines(default_planning_pipeline="ompl", pipelines=["ompl", "pilz_industrial_motion_planner"], load_all=False)
        .trajectory_execution(file_path="config/moveit_controllers.yaml")
        .to_moveit_configs()
    )

    # Required so TF for magician_root_link/base/joints exists from /joint_states.
    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="screen",
        parameters=[moveit_config.robot_description],
    )

    # MoveGroup node
    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[moveit_config.to_dict()],
    )

    # RViz node
    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="screen",
        arguments=["-d", str(moveit_config.package_path / "config/moveit.rviz")],
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
        ],
    )

    # the bridge between MoveIt Action and Dobot PointToPoint
    moveit_bridge_node = Node(
        package="dobot_demos",
        executable="moveit_bridge",
        name="moveit_bridge",
        output="screen"
    )

    return LaunchDescription([
        robot_state_publisher_node,
        move_group_node,
        rviz_node,
        moveit_bridge_node
    ])
