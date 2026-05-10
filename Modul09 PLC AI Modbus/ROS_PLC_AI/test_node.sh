#!/bin/bash
# Test script for MediaPipe Modbus ROS2 Node

echo "=== Testing MediaPipe Modbus ROS2 Node ==="
echo ""

# Source ROS2 workspace
cd /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09\ PLC\ AI\ Modbus/ROS_PLC_AI
source install/setup.bash

echo "1. Checking ROS2 package..."
ros2 pkg list | grep mediapipe_modbus_pkg
if [ $? -eq 0 ]; then
    echo "   ✓ Package found"
else
    echo "   ✗ Package not found"
    exit 1
fi

echo ""
echo "2. Checking Python dependencies..."
python3 -c "import mediapipe; print('   ✓ MediaPipe:', mediapipe.__version__)"
python3 -c "import pymodbus; print('   ✓ pymodbus:', pymodbus.__version__)"
python3 -c "import cv2; print('   ✓ OpenCV:', cv2.__version__)"
python3 -c "import PyQt5; print('   ✓ PyQt5 installed')"
python3 -c "import yaml; print('   ✓ PyYAML installed')"

echo ""
echo "3. Listing available nodes..."
ros2 pkg executables mediapipe_modbus_pkg

echo ""
echo "=== Ready to run ==="
echo "To run the node:"
echo "  cd /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09\ PLC\ AI\ Modbus/ROS_PLC_AI"
echo "  source install/setup.bash"
echo "  ros2 run mediapipe_modbus_pkg mediapipe_modbus_node"
echo ""
echo "Or run directly:"
echo "  ./run_node.sh"
