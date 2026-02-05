# REFERENSI MODUL 1: ARSITEKTUR SISTEM MEKATRONIKA & ROBOTIKA

## BUKU TEKS

### Mekatronika (Fundamental)

1. **Bolton, W.** (2015). *Mechatronics: Electronic Control Systems in Mechanical and Electrical Engineering* (7th Edition). Pearson Education.
   - **Coverage:** Sensor technology, actuator systems, microcontroller interfacing, system integration
   - **Relevant untuk:** Konsep dasar mekatronika, sensor-controller-actuator loop
   - **Chapter yang relevan:** 
     - Chapter 1: Introducing Mechatronics
     - Chapter 2: Sensors and Transducers
     - Chapter 3: Actuation Systems
     - Chapter 4: System Models
     - Chapter 10: Microprocessor Systems

2. **Alciatore, D. G., & Histand, M. B.** (2018). *Introduction to Mechatronics and Measurement Systems* (5th Edition). McGraw-Hill Education.
   - **Coverage:** Measurement systems, signal conditioning, data acquisition, control systems
   - **Relevant untuk:** Sensor conditioning, measurement accuracy, system design
   - **Chapter yang relevan:**
     - Chapter 1: Introduction to Mechatronics and Measurement Systems
     - Chapter 3: Semiconductor Electronics
     - Chapter 7: Sensors
     - Chapter 9: Actuators
     - Chapter 12: Microcontroller Programming and Interfacing

3. **Craig, J. J.** (2017). *Introduction to Robotics: Mechanics and Control* (4th Edition). Pearson.
   - **Coverage:** Robot kinematics, dynamics, control, trajectory planning
   - **Relevant untuk:** Robot arm design, coordinate frames, workspace analysis
   - **Chapter yang relevan:**
     - Chapter 1: Introduction
     - Chapter 2: Spatial Descriptions and Transformations
     - Chapter 3: Forward Kinematics
     - Chapter 4: Inverse Kinematics

4. **Siciliano, B., Sciavicco, L., Villani, L., & Oriolo, G.** (2010). *Robotics: Modelling, Planning and Control*. Springer.
   - **Coverage:** Robot modeling, motion planning, control architectures
   - **Relevant untuk:** System architecture, distributed control, path planning
   - **Chapter yang relevan:**
     - Chapter 1: Introduction
     - Chapter 7: Motion Control
     - Chapter 8: Interaction Control

---

## STANDARDS & TECHNICAL DOCUMENTS

### International Standards

1. **ISO 8373:2021** — *Robotics — Vocabulary*
   - Definisi terminologi robotika standard internasional
   - Available: ISO website atau university library
   - **Relevant untuk:** Terminology, classification of robots

2. **IEC 61131-3** — *Programmable Logic Controllers - Part 3: Programming Languages*
   - Standard untuk PLC programming (relevant untuk control logic design)
   - **Relevant untuk:** Sequential control, state machine implementation

3. **IEEE 1588** — *Precision Time Protocol (PTP)*
   - Time synchronization untuk distributed systems
   - **Relevant untuk:** Multi-controller synchronization

---

## ARTIKEL JURNAL & CONFERENCE PAPERS

### System Architecture

1. **Bruyninckx, H.** (2001). "Open robot control software: the OROCOS project." *Proceedings 2001 ICRA. IEEE International Conference on Robotics and Automation*, Vol. 3, pp. 2523-2528.
   - **Topic:** Open-source robot control architecture
   - **Relevant untuk:** Modular control architecture design
   - **DOI:** 10.1109/ROBOT.2001.933002

2. **Quigley, M., et al.** (2009). "ROS: an open-source Robot Operating System." *ICRA Workshop on Open Source Software*, Vol. 3, No. 3.2, p. 5.
   - **Topic:** ROS architecture, publish-subscribe communication
   - **Relevant untuk:** Distributed system architecture, communication protocol
   - **Available:** https://www.willowgarage.com/papers/ros-open-source-robot-operating-system

### Failure Mode Analysis

3. **Carlson, C. S.** (2012). *Effective FMEAs: Achieving Safe, Reliable, and Economical Products and Processes using Failure Mode and Effects Analysis*. John Wiley & Sons.
   - **Topic:** FMEA methodology, risk assessment
   - **Relevant untuk:** System failure analysis, mitigation strategy

---

## ONLINE RESOURCES & DOCUMENTATION

### ROS (Robot Operating System)

1. **ROS2 Humble Documentation**
   - URL: https://docs.ros.org/en/humble/
   - **Coverage:** Installation, tutorials, concepts (nodes, topics, services)
   - **Relevant untuk:** High-level control architecture, distributed systems

2. **ROS2 Design Documentation**
   - URL: https://design.ros2.org/
   - **Coverage:** ROS2 architectural decisions, DDS middleware, QoS
   - **Relevant untuk:** Understanding communication architecture

### Embedded Systems

3. **ESP32 Technical Reference Manual**
   - URL: https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf
   - **Coverage:** GPIO, PWM, ADC, UART, I2C, SPI, WiFi
   - **Relevant untuk:** Hardware interfacing, peripheral configuration

4. **Arduino Reference**
   - URL: https://www.arduino.cc/reference/en/
   - **Coverage:** Arduino language API, libraries, examples
   - **Relevant untuk:** Quick prototyping, basic embedded programming

5. **PlatformIO Documentation**
   - URL: https://docs.platformio.org/
   - **Coverage:** IDE setup, library management, board configuration
   - **Relevant untuk:** Embedded development workflow

### CAD & Electronics Design

6. **Fusion 360 Learning Center**
   - URL: https://help.autodesk.com/view/fusion360/ENU/
   - **Coverage:** CAD modeling, assembly, simulation, CAM
   - **Relevant untuk:** Mechanical design, motion study

7. **EasyEDA Documentation**
   - URL: https://docs.easyeda.com/
   - **Coverage:** Schematic capture, PCB layout, design rules
   - **Relevant untuk:** Electrical design, PCB prototyping

---

## TUTORIALS & COURSES (FREE)

### System Architecture & Design

1. **MIT OpenCourseWare: Introduction to Robotics**
   - URL: https://ocw.mit.edu/courses/mechanical-engineering/2-12-introduction-to-robotics-fall-2005/
   - **Coverage:** Robot kinematics, control, sensors, actuators
   - **Format:** Lecture notes, assignments, exams

2. **Coursera: Robotics Specialization (University of Pennsylvania)**
   - URL: https://www.coursera.org/specializations/robotics
   - **Coverage:** Aerial robotics, computational motion planning, mobility
   - **Format:** Video lectures, quizzes, programming assignments
   - **Note:** Free to audit (certificate berbayar)

### Embedded Systems

3. **EdX: Embedded Systems - Shape The World (UT Austin)**
   - URL: https://www.edx.org/course/embedded-systems-shape-the-world-microcontroller-i
   - **Coverage:** Microcontroller programming, I/O, interfacing
   - **Format:** Video lectures, labs (with TI LaunchPad)

4. **YouTube: Phil's Lab (PCB Design & Embedded)**
   - URL: https://www.youtube.com/@PhilsLab
   - **Coverage:** PCB design tutorials, STM32 programming, sensor integration
   - **Format:** Hands-on video tutorials
   - **Highlight:** Professional-grade PCB layout tips

### ROS & Robotics Software

5. **The Construct (ROS2 Tutorials)**
   - URL: https://www.theconstruct.ai/
   - **Coverage:** ROS2 basics, navigation, manipulation
   - **Format:** Interactive browser-based ROS environment
   - **Note:** Free tier available

6. **Articulated Robotics (YouTube)**
   - URL: https://www.youtube.com/@ArticulatedRobotics
   - **Coverage:** ROS2 from scratch, URDF, Gazebo, hardware integration
   - **Format:** Step-by-step video tutorials
   - **Highlight:** Practical projects with real hardware

---

## TOOLS & SOFTWARE

### Design Tools (Free/Student Version)

1. **Fusion 360**
   - URL: https://www.autodesk.com/products/fusion-360/personal
   - **License:** Free for students/educators (1 year renewable)
   - **Platform:** Windows, macOS
   - **Use case:** Mechanical CAD, assembly, simulation

2. **EasyEDA (Online)**
   - URL: https://easyeda.com/
   - **License:** Free (cloud-based)
   - **Platform:** Web browser
   - **Use case:** Schematic, PCB layout, simulation

3. **KiCad**
   - URL: https://www.kicad.org/
   - **License:** Open source (GPL)
   - **Platform:** Windows, macOS, Linux
   - **Use case:** Alternative untuk EasyEDA, lebih powerful

4. **Draw.io (Diagrams.net)**
   - URL: https://app.diagrams.net/
   - **License:** Free, open source
   - **Platform:** Web browser, desktop app
   - **Use case:** Block diagrams, flowcharts, architecture diagrams

### Development Tools

5. **Visual Studio Code**
   - URL: https://code.visualstudio.com/
   - **License:** Free (MIT)
   - **Platform:** Windows, macOS, Linux
   - **Extensions:** PlatformIO IDE, Python, C/C++

6. **Git & GitHub**
   - URL: https://git-scm.com/ , https://github.com/
   - **License:** Free (open source)
   - **Use case:** Version control, collaboration
   - **GitHub Student Pack:** https://education.github.com/pack

### Simulation Tools

7. **Gazebo**
   - URL: https://gazebosim.org/
   - **License:** Open source (Apache 2.0)
   - **Platform:** Linux (recommended), Windows (via WSL)
   - **Use case:** Robot simulation, sensor simulation, physics engine

8. **Webots**
   - URL: https://cyberbotics.com/
   - **License:** Open source (Apache 2.0)
   - **Platform:** Windows, macOS, Linux
   - **Use case:** Alternative Gazebo, built-in robot models

---

## COMMUNITY & FORUMS

### Discussion Forums

1. **ROS Discourse**
   - URL: https://discourse.ros.org/
   - **Use case:** Ask ROS-related questions, announcements, discussions

2. **Arduino Forum**
   - URL: https://forum.arduino.cc/
   - **Use case:** Embedded programming help, hardware interfacing

3. **Stack Overflow**
   - URL: https://stackoverflow.com/
   - **Tags:** `robotics`, `ros`, `embedded`, `arduino`, `esp32`
   - **Use case:** Specific coding questions

4. **Reddit Communities**
   - r/robotics: https://www.reddit.com/r/robotics/
   - r/ROS: https://www.reddit.com/r/ROS/
   - r/arduino: https://www.reddit.com/r/arduino/
   - r/embedded: https://www.reddit.com/r/embedded/

### GitHub Repositories (Examples & Inspiration)

5. **Awesome Robotics (Curated List)**
   - URL: https://github.com/kiloreux/awesome-robotics
   - **Content:** Curated list of robotics libraries, tools, frameworks

6. **ROS2 Examples**
   - URL: https://github.com/ros2/examples
   - **Content:** Official ROS2 example code (publisher, subscriber, service, etc.)

7. **PlatformIO Examples**
   - URL: https://github.com/platformio/platformio-examples
   - **Content:** Example projects untuk berbagai board dan framework

---

## SUPPLIER & COMPONENT SOURCES (Indonesia)

### Electronics Components

1. **Tokopedia / Shopee / Bukalapak**
   - **Search keywords:** "Arduino Mega", "ESP32 DevKit", "L298N motor driver"
   - **Tip:** Compare prices, check seller rating

2. **Toko Elektronik Lokal**
   - **Jakarta:** Glodok (ITC Glodok, Harco Glodok)
   - **Bandung:** Pasar Grosir Cikapundung
   - **Surabaya:** Hi-Tech Mall
   - **Yogyakarta:** Pasar Beringharjo elektronik section

### 3D Printing Services

3. **Lokal 3D Printing**
   - Search: "3D printing service [kota]"
   - **Typical price:** Rp 1.000 - Rp 3.000 per gram (PLA)

4. **University FabLab**
   - Cek apakah kampus memiliki FabLab atau Maker Space dengan 3D printer
   - Biasanya gratis atau harga subsidi untuk mahasiswa

---

## VIDEO TUTORIALS (BAHASA INDONESIA)

1. **Kelas Robot (YouTube)**
   - Channel tentang robotika, Arduino, IoT dalam bahasa Indonesia
   - URL: Search "Kelas Robot" di YouTube

2. **Innovative Electronics (YouTube)**
   - Tutorial Arduino, sensor, motor driver
   - URL: Search "Innovative Electronics" di YouTube

3. **Elektronika Mudah (YouTube)**
   - Basic electronics, circuit design
   - URL: Search "Elektronika Mudah" di YouTube

---

## RECOMMENDED READING ORDER

### Untuk Pemula:
1. Baca **Bolton - Mechatronics** Chapter 1-4 (overview, sensor, actuator, system model)
2. Tutorial **Arduino Reference** untuk basic programming
3. Follow **ROS2 Humble Tutorials** (jika pakai ROS2)

### Untuk Intermediate:
1. Baca **Alciatore - Measurement Systems** Chapter 7, 9 (sensor & actuator detail)
2. Study **ESP32 Technical Reference** bagian GPIO, PWM, ADC
3. Read **FMEA guide** untuk failure analysis

### Untuk Advanced:
1. Baca **Craig - Robotics** Chapter 2-4 (kinematics) jika project robot arm
2. Study **ROS2 Design Documentation** untuk distributed architecture
3. Explore **research papers** tentang specific topics (path planning, sensor fusion, dll)

---

## CITATION STYLE

Untuk laporan, gunakan **IEEE citation style**:

**Book:**
[1] W. Bolton, *Mechatronics: Electronic Control Systems in Mechanical and Electrical Engineering*, 7th ed. Pearson Education, 2015.

**Journal/Conference:**
[2] M. Quigley et al., "ROS: an open-source Robot Operating System," in *ICRA Workshop on Open Source Software*, vol. 3, no. 3.2, 2009, p. 5.

**Website:**
[3] "ROS2 Documentation," ROS.org. [Online]. Available: https://docs.ros.org/en/humble/. [Accessed: 05-Feb-2026].

**Standard:**
[4] ISO 8373:2021, *Robotics — Vocabulary*. International Organization for Standardization, 2021.

---

## CATATAN PENGGUNAAN REFERENSI

✓ **Selalu cite sumber** yang kamu gunakan di laporan  
✓ **Pahami, jangan copy-paste** — plagiarism akan terdeteksi  
✓ **Cross-reference** multiple sources untuk verify informasi  
✓ **Check publication date** — prefer recent sources (< 5 tahun) untuk teknologi  
✓ **Evaluate credibility** — prefer peer-reviewed journals, official documentation, reputable books  

---

## UPDATE LOG

| Date | Update |
|------|--------|
| 05-Feb-2026 | Initial version — Modul 1 references compiled |

---

**Untuk request tambahan referensi atau link yang broken, silakan contact instruktur.**
