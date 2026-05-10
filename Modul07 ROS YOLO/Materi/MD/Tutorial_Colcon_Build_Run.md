# TUTORIAL COLCON BUILD DAN RUN YOLO ROS 2

## PENDAHULUAN
Tutorial ini menjelaskan cara build package ROS 2 dan menjalankan semua node YOLO yang telah dibuat.

---

## LANGKAH 1: STRUKTUR WORKSPACE

Pastikan struktur workspace sudah benar:
```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"

tree -L 3 src/
```

Struktur yang benar:
```
src/
└── yolo_ros/
    ├── package.xml
    ├── setup.py
    ├── resource/
    │   └── yolo_ros
    ├── launch/
    │   └── yolo.launch.py
    ├── config/
    │   └── yolo_params.yaml
    ├── models/
    │   └── yolov8n.pt
    ├── yolo_ros/
    │   └── scripts/
    │       ├── yolo_node.py
    │       ├── camera_node.py
    │       ├── yolo_camera_node.py
    │       ├── training_node.py
    │       └── rviz_display.py
    ├── training_data/  (empty, untuk hasil roboflow/cvat)
    └── roboflow_data/  (empty, untuk hasil roboflow)
```

---

## LANGKAH 2: INSTALL DEPENDENSI

```bash
# Masuk ke workspace
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"

# Update rosdep
sudo rosdep init
rosdep update

# Install dependensi dari package.xml
rosdep install -i --from-path src --rosdistro humble -y

# Install Python dependencies
pip3 install ultralytics opencv-python
```

---

## LANGKAH 3: COLCON BUILD

### 3.1 Build Workspace
```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"

# Build semua package
colcon build

# Atau build spesifik package saja
colcon build --packages-select yolo_ros
```

**Output yang diharapkan**:
```
Starting >>> yolo_ros
Finished <<< yolo_ros [5.21s]

Summary: 1 package finished [5.50s]
```

### 3.2 Jika Build Error
```bash
# Clean build
rm -rf build/ install/ log/

# Rebuild
colcon build --symlink-install

# Cek error di log
cat log/latest_build/build.log
```

---

## LANGKAH 4: SOURCE WORKSPACE

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"

# Source workspace
source install/setup.bash

# Atau tambahkan ke .bashrc agar otomatis
echo "source /home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07\ ROS\ YOLO/ROS_YOLO/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

---

## LANGKAH 5: CEK NODES TERDAFTAR

```bash
# Source dulu
source install/setup.bash

# Cek daftar executable
ros2 pkg executables yolo_ros
```

**Output**:
```
yolo_ros camera_node
yolo_ros rviz_display
yolo_ros training_node
yolo_ros yolo_camera_node
yolo_ros yolo_node
```

---

## LANGKAH 6: RUNNING NODES

### 6.1 Menjalankan YOLO Detection Node
```bash
# Terminal 1: Jalankan yolo_detector node
source install/setup.bash
ros2 run yolo_ros yolo_node
```

**Node ini**:
- Subscribe: `/camera/image_raw`
- Publish: `/yolo/annotated` (gambar dengan bounding box)
- Publish: `/yolo/detections` (JSON detection info)

### 6.2 Menjalankan Camera Display Node (Raw Camera)
```bash
# Terminal 2: Tampilkan kamera raw
source install/setup.bash
ros2 run yolo_ros camera_node
```

### 6.3 Menjalankan YOLO Camera Node (Kamera + YOLO)
```bash
# Terminal 3: Kamera dengan YOLO detection
source install/setup.bash
ros2 run yolo_ros yolo_camera_node
```

### 6.4 Menjalankan RViz Display (Raw + YOLO Side by Side)
```bash
# Terminal 4: Tampilkan 2 window (Raw & YOLO)
source install/setup.bash
ros2 run yolo_ros rviz_display
```

### 6.5 Menjalankan Training Node
```bash
# Terminal 5: Training node (untuk trigger training)
source install/setup.bash
ros2 run yolo_ros training_node
```

---

## LANGKAH 7: MENGGUNAKAN LAUNCH FILE

### 7.1 Jalankan Launch File (YOLO + Display)
```bash
source install/setup.bash

ros2 launch yolo_ros yolo.launch.py
```

**Dengan parameter**:
```bash
ros2 launch yolo_ros yolo.launch.py \
  model_path:=/path/to/custom_yolov8.pt \
  camera_topic:=/usb_cam/image_raw
```

### 7.2 Jika Launch File Error
```bash
# Install launch dependencies
sudo apt install ros-humble-launch-ros

# Cek syntax launch file
python3 src/yolo_ros/launch/yolo.launch.py
```

---

## LANGKAH 8: TESTING DENGAN FAKE CAMERA

Jika tidak ada kamera asli, gunakan ros2 video feed:

```bash
# Terminal 1: Install image publisher
sudo apt install ros-humble-image-publisher

# Publish video/image sebagai topic ROS
ros2 run image_publisher image_publisher_node \
  --ros-args -p filename:=/path/to/test_image.jpg \
  -p topic:=/camera/image_raw \
  -p width:=640 -p height:=480
```

Atau menggunakan `v4l2_camera` untuk webcam:
```bash
sudo apt install ros-humble-v4l2-camera

ros2 run v4l2_camera v4l2_camera_node \
  --ros-args -p video_device:=/dev/video0 \
  -p image_size:=[640,480]
```

---

## LANGKAH 9: CEK TOPIC DAN NODE

```bash
# Cek nodes yang running
ros2 node list

# Cek topics
ros2 topic list

# Cek topic info
ros2 topic info /yolo/annotated

# Lihat isi topic (detections)
ros2 topic echo /yolo/detections
```

---

## LANGKAH 10: MENGGANTI MODEL YOLO

### 10.1 Ganti Model Default
```bash
# Copy model baru ke folder models
cp /path/to/custom_yolov8.pt \
  "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/models/"

# Rebuild
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO"
colcon build
```

### 10.2 Pakai Parameter Waktu Run
```bash
ros2 run yolo_ros yolo_node --ros-args \
  -p model_path:="/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul07 ROS YOLO/ROS_YOLO/src/yolo_ros/models/custom_yolov8.pt"
```

---

## LANGKAH 11: TIPS DEBUGGING

### Cek Log Node
```bash
# Lihat log node
ros2 topic echo /rosout

# Atau lihat di terminal saat menjalankan node
```

### Cek Performance
```bash
# Cek CPU/GPU usage
htop

# Jika pakai GPU untuk YOLO
nvidia-smi -l 1  # Update setiap 1 detik
```

### Matikan Node
- Tekan `Ctrl + C` di terminal node
- Atau `ros2 node kill /node_name` (jika support)

---

## RINGKASAN PERINTAH UTAMA

| Perintah | Fungsi |
|----------|--------|
| `colcon build` | Build semua package |
| `colcon build --packages-select yolo_ros` | Build package tertentu |
| `source install/setup.bash` | Source workspace |
| `ros2 run yolo_ros yolo_node` | Jalankan yolo detector |
| `ros2 run yolo_ros camera_node` | Tampilkan kamera raw |
| `ros2 run yolo_ros yolo_camera_node` | Kamera + YOLO |
| `ros2 run yolo_ros rviz_display` | Display raw + YOLO |
| `ros2 run yolo_ros training_node` | Training node |
| `ros2 launch yolo_ros yolo.launch.py` | Jalankan launch file |
| `ros2 node list` | Cek node yang running |
| `ros2 topic list` | Cek topic yang ada |
| `ros2 topic echo /topic_name` | Lihat isi topic |

---

## TROUBLESHOOTING

### Error: "Package not found"
```bash
# Pastikan sudah source
source install/setup.bash

# Cek apakah package terinstall
ros2 pkg list | grep yolo_ros
```

### Error: "Module not found (ultralytics)"
```bash
# Install di Python environment
pip3 install ultralytics

# Atau jika pakai virtualenv, aktifkan dulu
```

### Error: "cv_bridge not found"
```bash
sudo apt install ros-humble-cv-bridge
```

### Error: "Cannot open camera"
```bash
# Cek device kamera
ls /dev/video*

# Jika tidak ada, cek kernel module
v4l2-ctl --list-devices
```

---

## REFERENSI
- ROS 2 Colcon: https://docs.ros.org/en/humble/Tutorials/Colcon-Tutorial.html
- ROS 2 Launch: https://docs.ros.org/en/humble/Tutorials/Launch-Files.html
- YOLOv8: https://docs.ultralytics.com
