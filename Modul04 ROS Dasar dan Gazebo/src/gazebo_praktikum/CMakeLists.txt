cmake_minimum_required(VERSION 3.8)
project(gazebo_praktikum)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── Dependensi ────────────────────────────────────────────────────────────────
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(urdf REQUIRED)
find_package(xacro REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(gazebo_dev REQUIRED)
find_package(gazebo_msgs REQUIRED)

# ── Gazebo Classic ────────────────────────────────────────────────────────────
find_package(gazebo REQUIRED)

include_directories(
  include
  ${GAZEBO_INCLUDE_DIRS}
)
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# ── Plugin C++ (Percobaan 10) ─────────────────────────────────────────────────
add_library(my_world_plugin SHARED
  src/my_world_plugin.cpp
)
ament_target_dependencies(my_world_plugin
  rclcpp
  std_msgs
  gazebo_ros
  gazebo_dev
)
target_link_libraries(my_world_plugin
  ${GAZEBO_LIBRARIES}
)

# ── Install ───────────────────────────────────────────────────────────────────
install(TARGETS my_world_plugin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY
  launch worlds urdf config rviz maps meshes
  DESTINATION share/${PROJECT_NAME}/
)

install(PROGRAMS
  scripts/demo_manipulator.py
  scripts/advanced_demo.py
  scripts/kinematics_demo.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
