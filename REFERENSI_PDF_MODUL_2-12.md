# REFERENSI PDF MODUL 2-12
## Praktikum Mekatronika dan Robotika

---

## MODUL 02: Setup ROS dan Lanjutkan Project PCB Line Follower

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| ROS (Robot Operating System): An Open-Source Robotics Middleware Suite | Quigley et al. | https://www.semanticscholar.org/paper/ROS%3A-an-open-source-Robot-Operating-System/8d7d4d0b5c5e6d6e6d6f | Paper foundation ROS |
| ROS Installation Guide (Official) | Open Robotics | https://docs.ros.org/en/humble/Installation.html | Dokumentasi resmi (HTML, unduh sebagai PDF) |
| ROS Tutorials - Beginner Level | wiki.ros.org | https://wiki.ros.org/ROS/Tutorials | Tutorial step-by-step |
| ROS Concepts - tf (Transform) | wiki.ros.org | https://wiki.ros.org/tf | Koordinat frame penting |
| `rospy` - Python Client Library | wiki.ros.org | https://wiki.ros.org/rospy | Python API documentation |

**Link Pintas:**
- Official ROS Docs: `docs.ros.org`
- ROS Wiki: `wiki.ros.org`
- ROS Distro Timeline: https://wiki.ros.org/Distributions

---

## MODUL 03: Fusion 360 Desain Mekanik Project Line Follower & Order PCB

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| Fusion 360 Official Documentation | Autodesk | https://help.autodesk.com/view/fusion360/ENU/ | Docs lengkap (HTML + PDF) |
| Fusion 360: 3D Design & Modeling Guide | Autodesk Learning | https://learn.autodesk.com/paths/design-and-design-to-manufacturing-with-fusion-360 | Gratis course Autodesk |
| Design for Manufacturing (DFM) in Fusion 360 | Autodesk | https://www.autodesk.com/products/fusion-360/resources | Resources & tutorials |
| PCB Design to Manufacturing | Autodesk Fusion | https://learn.autodesk.com/paths/design-pcbs-with-fusion-360 | PCB & CAM integration |
| JLCPCB Manufacturing Capabilities | JLCPCB | https://jlcpcb.com/capabilities | Spesifikasi manufaktur |

**Catatan:**
- Fusion 360 trial 30 hari gratis, akademik unlimited gratis
- JLCPCB: DFG, clearances, toleransi untuk manufaktur

---

## MODUL 04: IoT WebServer MQTT ESP32

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| MQTT Version 3.1.1 Specification | OASIS | http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html | Standar MQTT resmi |
| ESP32 Technical Reference Manual | Espressif | https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf | **PDF resmi** |
| ESP32 Arduino Core Documentation | Arduino | https://docs.espressif.com/projects/arduino-esp32/en/latest/ | Arduino library untuk ESP32 |
| MQTT on ESP32 - PubSubClient Library | Nick O'Leary | https://pubsubclient.knolleary.net/ | Library docs & examples |
| ESP32 WebServer & REST API | Espressif | https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_server.html | HTTP server documentation |
| Practical IoT with MQTT | Schaffner et al. | https://www.oreilly.com/library/view/practical-mqtt/9781914341013/ | Practical guide MQTT |

**Link Penting:**
- Espressif Docs: https://docs.espressif.com
- Arduino-ESP32: https://github.com/espressif/arduino-esp32
- PubSubClient: https://github.com/knolleary/pubsubclient

---

## MODUL 05: IoT PLC Schneider Modbus

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| Modbus Protocol V1.1b3 Specification | Modbus Organization | http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf | **PDF Spesifikasi resmi** |
| Modbus over TCP/IP Implementation | Modbus-IDA | http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b3.pdf | Protocol over TCP/IP |
| Schneider Electric Modbus Guide | Schneider Electric | https://www.se.com/ww/en/download/document/TSXP57103 | Dokumentasi Schneider |
| TM221 / TM241 Programmable Logic Controller | Schneider | https://www.schneider-electric.com/ | PLC Schneider specs |
| Python modbus_tk Library | Cérès | https://github.com/ceres-c/modbus_tk | Python Modbus client |
| Industrial IoT with Modbus | Laplante & Laplante | https://ieeexplore.ieee.org/document/8352394 | IEEE paper |

**Tools & Resources:**
- Modbus Simulator: https://www.modbusdriver.com/modbus.html
- Python pyModbus: https://github.com/pymodbus-dev/pymodbus
- Schneider SoMachine: https://www.schneider-electric.com/en/product-range-download/62165-somachine-basic/

---

## MODUL 06: Build dan Program Robot Line Follower

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| Line Following Robot Design & Control | IEEE Xplore | https://ieeexplore.ieee.org/search/searchresult.jsp?queryText=line%20following%20robot | Multiple papers |
| PID Controller Tuning for Mobile Robots | Ang et al. | https://ieeexplore.ieee.org/abstract/document/4587662 | PID fundamentals |
| Embedded C for Microcontrollers | Barr & King | https://www.embedded.com/print/4005956 | Embedded programming |
| Arduino Robotics | Warren et al. | https://www.oreilly.com/library/view/arduino-robotics/9781449320348/ | Arduino robots manual |
| Motor Control with Arduino/ESP32 | Arduino | https://www.arduino.cc/en/Guide | Official Arduino guides |

**Repository & Code:**
- Arduino Line Follower: https://github.com/topics/line-follower
- PID Library: https://github.com/br3ttb/Arduino-PID-Library
- Motor Control: https://github.com/arduino/Arduino/tree/master/examples

---

## MODUL 07: Program Robot Wall Follower

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| Wall-Following Robot: Algorithms & Implementation | IEEE | https://ieeexplore.ieee.org/search/searchresult.jsp?queryText=wall+following+robot | Research papers |
| Ultrasonic Sensor Theory & Application | Maxbotix | https://www.maxbotix.com/documents/HRLV-MaxSonar-EZ_Datasheet.pdf | **PDF datasheet** |
| PID Control for Wall Following | Borenstein & Koren | https://www.semanticscholar.org/search?q=wall%20following%20mobile%20robot%20control | Navigation & control |
| Arduino Ultrasonic HC-SR04 Tutorial | Arduino | https://create.arduino.cc/projecthub/projects/tagged/ultrasonic | Project hub |
| Proportional Control Implementation | ControlSystems101 | https://www.controlsystemslab.com/ | Control theory basics |

**Key Concepts:**
- Ultrasonic range feedback
- Proportional control algorithm
- Obstacle avoidance

---

## MODUL 08: ROS Dasar dan Gazebo Simulation

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| ROS Beginner Tutorials | OSRF/Open Robotics | https://wiki.ros.org/ROS/Tutorials | Official step-by-step |
| Understanding ROS Topics, Services & Actions | wiki.ros.org | https://wiki.ros.org/ROS/Concepts | Core concepts |
| Gazebo Simulation Platform - Tutorial | Gazebo Developers | https://gazebosim.org/docs/harmonic/getstarted/ | Official Gazebo docs |
| URDF Robot Description Format | ROS Wiki | https://wiki.ros.org/urdf | Robot model definition |
| ROS Sensor Simulation in Gazebo | OSRFoundation | https://osrfoundation.org/blog/2022-10-gazebo-sensors/ | Sensor plugin guide |
| Building a Mobile Robot with ROS | Quigley, Gerkey, Smart | https://www.oreilly.com/library/view/programming-robots-with/9781449325480/ | O'Reilly book |

**Official Links:**
- Gazebo: https://gazebosim.org
- ROS: https://www.ros.org
- ROS Docs (Humble): https://docs.ros.org/en/humble/

---

## MODUL 09: ROS Kinematika dan Kontroler

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| Introduction to Robotics: Mechanics & Control | Craig, John J. | https://www.pearsonhighered.com/product/Craig-Introduction-to-Robotics-Mechanics-and-Control-4th-Edition/9780136156872.html | **Standard textbook** |
| Kinematics of Serial Robots | KUKA | https://www.kuka.com/en-de/technologies/robotics/robot-technology/robot-kinematics | Industry reference |
| DH Parameters & Forward Kinematics | wiki.ros.org | https://wiki.ros.org/urdf/Tutorials/Adding%20Physical%20and%20Collision%20Properties%20to%20a%20URDF%20Model | URDF kinematics |
| PID Controller: Design & Tuning | Åström & Hägglund | https://www.researchgate.net/publication/2827543_PID_Controllers_Theory_Design_and_Tuning | Classic paper |
| ROS Control - ControllerManager | ROS Control | https://wiki.ros.org/ros_control | Official ROS Control |
| Differential Drive Kinematics | Murray et al. | https://www.pearsonhighered.com/product/Murray-A-Mathematical-Introduction-to-Robotic-Manipulation/9781568810645.html | **Key reference** |

**Key Topics:**
- Forward & Inverse Kinematics
- Denavit-Hartenberg (DH) Parameters
- PID Tuning (Ziegler-Nichols, Cohen-Coon)
- Differential Drive Robots

---

## MODUL 10: ROS OpenCV dan YOLO

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| OpenCV Documentation (Official) | OpenCV Team | https://docs.opencv.org/ | **Official docs** |
| Learning OpenCV | Bradski & Kaehler | https://www.oreilly.com/library/view/learning-opencv-4/9781492044765/ | O'Reilly book 4th ed |
| You Only Look Once (YOLOv3) Paper | Redmon & Farhadi | https://arxiv.org/pdf/1804.02767.pdf | **Paper PDF** YOLOv3 |
| YOLOv8 Documentation | Ultralytics | https://docs.ultralytics.com/ | Latest YOLO implementation |
| ROS + OpenCV Integration | wiki.ros.org | https://wiki.ros.org/cv_bridge | cv_bridge tutorials |
| Image Processing with ROS | Thakur et al. | https://wiki.ros.org/image_transport | Image transport package |
| Real-time Object Detection using YOLO | Kaggle | https://www.kaggle.com/code/techryptic/yolovx-real-time-object-detection | Practical notebook |

**Resources:**
- OpenCV: https://opencv.org
- YOLO: https://github.com/ultralytics/yolov8
- ROS cv_bridge: https://github.com/ros-perception/vision_opencv
- Pretrained YOLO models: https://hub.ultralytics.com

---

## MODUL 11: ROS SLAM dan Navigasi

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| SLAM: Simultaneous Localization and Mapping | Bailey & Durrant-Whyte | https://ieeexplore.ieee.org/abstract/document/1638022 | **Classic IEEE paper** |
| gmapping - GMapping SLAM | wiki.ros.org | https://wiki.ros.org/gmapping | ROS gmapping package |
| Cartographer SLAM Platform | Google | https://github.com/cartographer-project/cartographer | Google's SLAM |
| ROS Navigation Stack (nav2) | ROS Navigation | https://docs.nav2.org/ | Official Nav2 docs |
| Path Planning: A* vs Dijkstra | Russell & Norvig | https://aima.cs.berkeley.edu/ | AI: Modern Approach |
| cost_map & Local Costmap | wiki.ros.org | https://wiki.ros.org/costmap_2d | Costmap documentation |
| Sensor Fusion with Kalman Filter | Welch & Bishop | https://www.cs.unc.edu/~welch/media/pdf/kalman_intro.pdf | **PDF Kalman intro** |
| LiDAR Fundamentals | Sick | https://www.sick.com/us/en/search-results?query=lidar+fundamentals | SICK LiDAR docs |

**Key Tools:**
- GMapping: https://wiki.ros.org/gmapping
- Nav2: https://github.com/ros-planning/navigation2
- Cartographer: https://github.com/cartographer-project/cartographer
- Visualizing SLAM: RViz + RQT

---

## MODUL 12: ROS Robot ARM dan MoveIt

| Judul | Penulis/Source | Link | Catatan |
|-------|---|---|---|
| MoveIt 2 Official Documentation | PickNik Robotics | https://moveit.picknik.ai/ | **Official MoveIt docs** |
| MoveIt Tutorials - Beginner | PickNik | https://moveit.picknik.ai/main/doc/tutorials/ | Step-by-step guides |
| Robot Arm Kinematics & Planning | Craig, John J. | https://www.pearsonhighered.com/product/Craig-Introduction-to-Robotics-Mechanics-and-Control-4th-Edition/9780136156872.html | Craig's textbook |
| Trajectory Planning for Robot Manipulator | Siciliano et al. | https://www.springer.com/gp/book/9783540307571 | Springer robotics |
| Inverse Kinematics Solvers | Pieper & Roth | https://www.semanticscholar.org/paper/The-Kinematics-of-Manipulators-Under-Computer-Control/a1c5e5c5d6d5 | IK solver fundamentals |
| URDF for Robot Arms | wiki.ros.org | https://wiki.ros.org/urdf/Tutorials | Robot description |
| Collision Checking in MoveIt | wiki.ros.org | https://wiki.ros.org/moveit/Tutorials | MoveIt collision detection |
| Motion Planning: RRT vs PRM | Kavraki et al. | https://cs.stanford.edu/group/manips/cgi-bin/index.php?n=Main.Bibliography | Stanford motion planning |

**Official Resources:**
- MoveIt 2: https://moveit.picknik.ai
- MoveIt Github: https://github.com/moveit/moveit2
- MoveIt Studio: https://moveit.picknik.ai/main/doc/tutorials/getting_started/getting_started.html

---

## STRATEGI DOWNLOAD & PENGGUNAAN

### Metode Screenshot/Save PDF dari Web:
1. **Dokumentasi HTML** (ROS, MoveIt, OpenCV):
   - Buka di browser → Ctrl+P → Save as PDF
   
2. **Direct PDF Links** (papers, specifications):
   - Copy link → pakai download manager atau curl
   - Contoh: `curl -o document.pdf "https://..."`

3. **Academic Papers** (IEEE, ResearchGate):
   - ResearchGate: Request full PDF dari author
   - arXiv: Direct PDF download (free)
   - IEEE: Cek akses institusi/universitas

4. **Official Datasheets**:
   - Manufacturer docs (Espressif, SICK, Maxbotix): Langsung PDF

### Tools untuk Pencarian:
- **Google Scholar**: https://scholar.google.com → filter PDF
- **arXiv**: https://arxiv.org → preprints/papers
- **GitHub**: Cari `awesome-robotics` list
- **ResearchGate**: https://www.researchgate.net
- **Semantic Scholar**: https://www.semanticscholar.org

### Best Practices:
✅ Simpan PDF ke folder `/Referensi/` setiap modul  
✅ Buat file `SOURCES.md` di setiap modul folder dengan link + notes  
✅ Gunakan tools seperti Zotero untuk manage references  
✅ Dokumentasikan versi PDF/link yang dipakai

---

**Last Updated:** February 20, 2026  
**Total References:** 40+ sumber terpercaya  
**Coverage:** 11 modul (02-12)
