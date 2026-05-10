# Praktikum Mekatronika dan Robotika

[![GitHub](https://img.shields.io/badge/GitHub-rofiqcp-black?logo=github)](https://github.com/rofiqcp/Praktikum-Mekatronika-dan-Robotika)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ROS 2 Humble](https://img.shields.io/badge/ROS%202-Humble-blue)](https://docs.ros.org/en/humble/)
[![Ubuntu 22.04](https://img.shields.io/badge/Ubuntu-22.04-orange)](https://ubuntu.com/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange?logo=platformio)](https://platformio.org/)

| Info | Detail |
|------|--------|
| **Mata Kuliah** | Praktikum Mekatronika dan Robotika |
| **Program Studi** | Sarjana Terapan Teknologi Rekayasa Otomasi |
| **Beban Studi** | 2 SKS — 5 jam 40 menit per pertemuan |
| **Dosen Pengampu** | Rofiq Cahyo Prayogo, S.T., M.T. |
| **Jumlah Modul** | 10 Modul Praktikum |
| **Deadline Tugas** | 2 minggu setelah praktikum berlangsung |

---

## Deskripsi

Repository ini berisi materi praktikum lengkap untuk mata kuliah **Praktikum Mekatronika dan Robotika**. Setiap modul dilengkapi dengan Jobsheet, Materi teori, spesifikasi Project, dan panduan Tugas Video.

Cakupan materi: desain PCB elektronik, pemodelan mekanik 3D, pemrograman ROS 2 Humble, simulasi Gazebo, pengembangan web server full-stack, integrasi IoT dengan ESP32 dan MQTT, computer vision dengan YOLO, robot manipulator dengan MoveIt, kontrol PLC berbasis AI dan Modbus, serta robot **Line Follower** dengan kontroler PID pada ESP32 S2 Mini.

---

## Tujuan Pembelajaran

Setelah menyelesaikan seluruh modul, mahasiswa mampu:

1. Merancang skematik dan layout PCB menggunakan EasyEDA
2. Menginstal dan mengkonfigurasi environment ROS 2 Humble di Ubuntu 22.04
3. Membuat model mekanik 3D robot menggunakan Autodesk Fusion 360
4. Mengembangkan simulasi robot lengkap (sensor, aktuator, kontroler) di Gazebo
5. Membangun web server full-stack untuk monitoring dan kontrol robot
6. Mengintegrasikan ESP32 dengan server IoT melalui protokol MQTT
7. Mengimplementasikan deteksi objek real-time dengan YOLO pada ROS 2
8. Memprogram robot manipulator dengan motion planning MoveIt
9. Mengintegrasikan PLC dengan AI (MediaPipe) melalui protokol Modbus
10. Merancang, membangun, dan men-tuning robot line follower dengan kontroler PID

---

## Struktur Modul

### Modul 01 — Desain PCB (EasyEDA)

**Tujuan:** Mahasiswa mampu membuat PCB fungsional untuk sistem robotika.

**Topik:**
- Pengenalan EasyEDA (skematik, simulasi, layout PCB)
- Komponen elektronik: resistor, kapasitor, IC, konektor
- Routing PCB dan design rule check (DRC)
- Gerber file generation untuk fabrikasi
- Workflow produksi PCB (JLCPCB / PCBWay)

**File:**
- `Materi/MD/Jobsheet.md` — Panduan praktikum step-by-step
- `Materi/MD/Materi.md` — Teori dasar desain PCB
- `Materi/MD/Project.md` — Spesifikasi project PCB
- `Materi/MD/Tugas Video.md` — Panduan video presentasi
- `Referensi EasyEDA-Tutorial_v6.4.32.pdf` — Referensi resmi EasyEDA

**Output:** PCB siap produksi untuk sistem robotika

---

### Modul 02 — Setup ROS 2 Humble

**Tujuan:** Mahasiswa memiliki environment ROS 2 yang lengkap dan siap digunakan.

**Topik:**
- Instalasi Ubuntu 22.04 LTS (dual-boot dengan Ventoy)
- Instalasi ROS 2 Humble Hawksbill
- Konfigurasi workspace dan environment variables
- Instalasi library pendukung:
  - Machine Learning: TensorFlow, PyTorch, MediaPipe
  - Computer Vision: OpenCV, Ultralytics YOLO
  - Kontroler: `control_msgs`, `ros2_control`, MoveIt 2
  - Navigasi: Nav2, slam_toolbox
  - Simulasi: Gazebo Classic / Gazebo Fortress
- Verifikasi instalasi: node talker-listener, RViz, Gazebo

**Output:** Environment ROS 2 Humble yang lengkap dan terverifikasi

---

### Modul 03 — Desain Mekanik dengan Fusion 360

**Tujuan:** Mahasiswa mampu membuat model 3D robot line follower yang siap diproduksi.

**Topik:**
- Pengenalan interface Autodesk Fusion 360
- Sketching 2D dan extrude 3D
- Assembly dan constraint antar komponen
- Desain chassis robot line follower
- Penempatan motor, sensor, dan baterai
- Export STL untuk 3D printing
- Rendering presentasi desain

**File:**
- `Materi/MD/Jobsheet.md`, `Materi.md`, `Project.md`, `Tugas Video.md`
- `Fusion 360 Desain Mekanik Robot Line Follower.pdf`

**Output:** Model 3D robot line follower lengkap, siap 3D print

---

### Modul 04 — ROS 2 Dasar dan Simulasi Gazebo

**Tujuan:** Mahasiswa mampu membuat dan menjalankan simulasi robot di Gazebo.

**Topik (20 percobaan):**
- Konsep dasar: node, topic, service, action, parameter
- Publisher dan subscriber Python/C++
- URDF dan robot description (xacro)
- Gazebo world creation dan model spawning
- Sensor simulation: lidar, kamera, IMU
- Teleoperation (keyboard, joystick)
- SLAM dengan `slam_toolbox` dan peta 2D
- Navigasi otonom dengan Nav2
- Robot manipulator: forward/inverse kinematics, MoveIt
- Robot omni-directional (4WD) dan mecanum wheel
- Multi-robot simulation

**Source Code:** `src/gazebo_praktikum/` — 20 launch files (`percobaan1` s/d `percobaan20`)

**Output:** Simulasi robot lengkap dengan sensor, kontroler, SLAM, dan navigasi

---

### Modul 05 — Web Server Full-Stack JavaScript

**Tujuan:** Mahasiswa mampu membangun web application untuk monitoring dan kontrol robot.

**Topik:**
- Node.js, npm, dan ekosistem JavaScript modern
- Backend: Express.js/Fastify, RESTful API, WebSocket, GraphQL
- Database: MongoDB, PostgreSQL, SQLite, Redis
- Frontend: React, Vue 3, Svelte, Angular
- Real-time dashboard dengan Chart.js / D3.js
- Autentikasi: JWT, OAuth, session management
- Deployment: Cloudflare Tunnel, PM2, Docker
- Integrasi ROS 2 via rosbridge_suite

**Project Templates (20 proyek):**
01. Cloudflare Domain Setup  02. Todo List App  03. Weather App  04. Movie Database
05. Expense Tracker  06. Markdown Notes  07. IoT Dashboard  08. Chat App  09. Blog CMS
10. Kanban Board  11. Social Media Feed  12. Quiz Platform  13. AI Chatbot  14. Image Recognition
15. E-Commerce  16. Smart Home  17. Microservices  18. Smart Manufacturing
19. Supply Chain  20. Energy Management

**Output:** Web application full-stack untuk monitoring robot IoT

---

### Modul 06 — Web Server IoT dengan MQTT dan ESP32

**Tujuan:** Mahasiswa mampu membangun sistem IoT end-to-end dengan ESP32 dan MQTT.

**Topik:**
- Protokol MQTT: broker, topic, QoS, retained messages
- Setup Mosquitto broker (local + cloud)
- Pemrograman ESP32: WiFi, MQTT client, sensor, aktuator
- Backend IoT: Node.js/FastAPI + MQTT.js/paho-mqtt
- Database time-series: InfluxDB, TimescaleDB, SQLite
- Real-time dashboard: WebSocket + Vue 3 / React
- Keamanan: TLS/SSL, autentikasi MQTT
- Integrasi ESP32 dengan ROS 2 via MQTT bridge

**Project Templates IoT (10 proyek):**
01. ESP32 WebServer Monitor  02. DataLogger dengan SQLite  03. Real-time Sensor Dashboard (SSE + Vue 3 + FastAPI)
04. Authenticated IoT Dashboard (JWT + WebSocket + Angular)  05. Real-time WebSocket Dashboard
06. IoT Dashboard via MQTT  07. IoT Dashboard WebServer + MQTT + ESP32  08. IoT WebServer MQTT (Express + PostgreSQL + React)
09. IoT Smart Monitoring dengan ML Anomaly Detection  10. Complete Greenhouse Monitoring System

**Output:** Sistem IoT real-time dengan ESP32, MQTT, web dashboard, dan database

---

### Modul 07 — Computer Vision dengan ROS 2 dan YOLO

**Tujuan:** Mahasiswa mampu mengintegrasikan YOLO dengan ROS 2 untuk deteksi objek real-time.

**Topik:**
- Pengenalan deep learning dan CNN untuk computer vision
- Arsitektur YOLO (YOLOv8 / YOLOv11)
- Dataset preparation dan labeling (Roboflow)
- Training custom model YOLO
- Integrasi YOLO dengan ROS 2:
  - Image subscriber dari kamera/Gazebo
  - Object detection node
  - Bounding box publisher ke topic ROS 2
- Object tracking (ByteTrack, SORT)
- Depth estimation dengan kamera stereo/RGBD
- Aplikasi: obstacle avoidance, pick-and-place dengan vision

**ROS Workspace:** `ROS_YOLO/src/`

**Output:** Node ROS 2 deteksi objek real-time dari kamera

---

### Modul 08 — Robot Manipulator dengan ROS 2 dan MoveIt

**Tujuan:** Mahasiswa mampu memprogram robot manipulator dengan motion planning.

**Topik:**
- Kinematika robot manipulator:
  - Forward kinematics (FK) dan Denavit-Hartenberg
  - Inverse kinematics (IK) — analitik dan numerik
  - Workspace analysis
- MoveIt 2 setup:
  - URDF/XACRO configuration
  - SRDF generation dengan MoveIt Setup Assistant
  - Planning scene dan collision objects
  - Move group interface (Python/C++)
- Trajectory planning: Cartesian path, joint space
- Pick and place dengan vision (integrasi Modul 07)
- Collision avoidance real-time
- Hardware integration: ros2_control, joint_state_publisher

**ROS Workspace:** `ROS_DOBOT/src/`

**Output:** Robot manipulator dengan motion planning dan pick-and-place

---

### Modul 09 — PLC, AI (MediaPipe), dan Modbus

**Tujuan:** Mahasiswa mampu mengintegrasikan PLC industri dengan AI dan ROS 2 via Modbus.

**Topik:**
- Pengenalan PLC (Schneider Electric / Siemens)
- Protokol Modbus TCP/RTU — register, coil, function code
- Komunikasi Modbus dengan Python (`pymodbus`)
- Computer vision dengan MediaPipe:
  - Hand landmarks (21 titik)
  - Gesture recognition
  - Pose estimation
- Konversi gesture → perintah PLC via Modbus
- ROS 2 nodes untuk Modbus bridge
- GUI monitoring real-time (PyQt5 / Tkinter)
- Keamanan sistem kontrol industri

**ROS Workspace:** `ROS_PLC_AI/src/`

**Output:** Sistem kontrol PLC via gesture recognition dengan AI

---

### Modul 10 — Line Follower dengan ESP32 S2 Mini dan PID

**Tujuan:** Mahasiswa mampu merancang, membangun, dan men-tuning robot line follower menggunakan kontroler PID pada ESP32 S2 Mini.

**Platform Hardware:**
- **MCU:** ESP32 S2 Mini (Wemos LOLIN S2 Mini) — single-core Xtensa LX7 @ 240 MHz
- **Memory:** 4 MB Flash, 320 KB SRAM; ADC 13-bit (0–8191)
- **Display:** OLED 128×64 px via I2C (SDA=GPIO33, SCL=GPIO35, addr=0x3C)
- **Input:** 4 push button (GPIO0/12/13/15, INPUT_PULLUP, debounce 50 ms)
- **Motor:** 2× DC motor via H-Bridge (Left: EN=GPIO11, IN1=GPIO10, IN2=GPIO9; Right: EN=GPIO8, IN1=GPIO7, IN2=GPIO6)
- **Sensor:** 8-channel IR sensor array (GPIO1–5, 16–18), threshold default 2000
- **PWM:** LEDC 8-bit (0–255), 5 kHz, 2 channel

**Software Stack:**
- IDE: VS Code + PlatformIO IDE Extension
- Board target: `lolin_s2_mini`, framework `arduino`
- Library: Adafruit SSD1306, Adafruit GFX Library, Adafruit BusIO
- Serial monitor: 115200 baud

**Parameter PID Default:**
```
Kp=0.08  Ki=0.0  Kd=0.4
Base Speed=150  Max Speed=255  Min Speed=-255
Error range: -3500 s/d +3500  (setpoint=3500, weighted average 0–7000)
```

**Sub-Program (6 percobaan bertahap):**

| Folder | Percobaan | Deskripsi |
|--------|-----------|-----------|
| `01LcdI2c/` | Percobaan 1 | Test komunikasi I2C OLED — tampilkan teks |
| `02PushButton/` | Percobaan 2 | Test 4 push button dengan debouncing |
| `03Motor/` | Percobaan 3 | Test kontrol motor DC kiri/kanan dengan PWM |
| `04Sensor/` | Percobaan 4 | Kalibrasi sensor IR 8 channel, uji threshold |
| `05PID/` | Percobaan 5 | Implementasi full line follower dengan PID |
| `06PathPlanning/` | Percobaan 6 | Path planning — deteksi persimpangan dengan sensor mask |

**File Konfigurasi:**
- `Program/config.h` — Semua pin assignment, PID default, motor mode
- `Program/eeprom_config.h` — Penyimpanan konfigurasi persisten ke EEPROM

**Materi Teori (Jobsheet.md 1170 baris, Materi.md 1303 baris, 17 BAB):**
- BAB 1–9: Pengantar, listrik, ESP32, OLED, button, motor, sensor IR, estimasi posisi, teori PID
- BAB 10: Tuning PID sistematis (3 level referensi, 2 case study, perhitungan manual)
- BAB 11–15: Lost line handling, path planning, EEPROM, analisis performa, topik lanjutan
- BAB 16: Troubleshooting lengkap (hardware, software, performa, power, emergency)
- BAB 17: Referensi dan buku

**Pilihan Project (10 variasi ⭐–⭐⭐⭐⭐):**
- ⭐ P1: Optimalkan PID — 3 lap tanpa exit, min 10 percobaan tuning
- ⭐⭐ P2: Adaptive Speed — lambat di tikungan, cepat di lurus (berdasarkan `abs(error)`)
- ⭐⭐ P3: 3 Mode Operasi — Slow/Normal/Fast via push button, tampil di OLED
- ⭐⭐ P4: Logging CSV — via Serial, grafik Excel (Error vs Waktu, Speed vs Waktu)
- ⭐⭐ P5: EEPROM Tuning Interface — ubah Kp/Kd via tombol tanpa laptop
- ⭐⭐⭐ P6: Path Planning 4 persimpangan — rute Kanan → Kiri → Lurus → Stop
- ⭐⭐⭐ P7: Anti-Lost Line System — recovery otomatis 3 skenario, waktu < 3 detik
- ⭐⭐⭐ P8: Dashboard OLED 5 layar — posisi, PID, kecepatan, sensor raw, statistik lap
- ⭐⭐⭐ P9: Auto-Kalibrasi Sensor — gerak maju-mundur 3 detik, simpan threshold EEPROM
- ⭐⭐⭐⭐ P10: Competition Ready — semua fitur terintegrasi, lintasan 5 m+ dengan 2 tikungan tajam

**Output:** Robot line follower tertuning yang dapat mengikuti lintasan kompleks

---

## Teknologi yang Digunakan

### Software & Platform

| Kategori | Tools |
|----------|-------|
| OS | Ubuntu 22.04 LTS |
| Robot OS | ROS 2 Humble Hawksbill |
| Simulator | Gazebo Classic, Gazebo Fortress |
| CAD 3D | Autodesk Fusion 360 |
| PCB Design | EasyEDA (online) |
| Embedded IDE | VS Code + PlatformIO |
| General IDE | Visual Studio Code |
| Version Control | Git & GitHub |

### Bahasa Pemrograman

- **Python 3.10+** — ROS 2 nodes, AI/ML, Modbus, backend IoT
- **C++ 17** — ROS 2 nodes, simulasi Gazebo
- **C / Arduino** — Pemrograman ESP32 (PlatformIO)
- **JavaScript / TypeScript** — Web server, dashboard IoT

### Framework & Library

| Domain | Library |
|--------|---------|
| ROS 2 | rclpy, rclcpp, tf2, nav2, MoveIt 2, ros2_control |
| Computer Vision | OpenCV, Ultralytics YOLO, MediaPipe |
| Machine Learning | TensorFlow, PyTorch |
| Web Backend | Express.js, FastAPI, Socket.io |
| Web Frontend | React, Vue 3, Angular, Svelte |
| IoT | MQTT (Mosquitto), ESP-IDF, Arduino |
| ESP32 Display | Adafruit SSD1306, Adafruit GFX |
| Komunikasi | pymodbus, rosbridge_suite |

### Hardware

| Komponen | Spesifikasi |
|----------|-------------|
| MCU Utama | ESP32 S2 Mini (Xtensa LX7 @ 240 MHz) |
| Mikrokontroler | Arduino Uno/Mega (lab awal) |
| Sensor | Kamera, Lidar 2D, IMU, Ultrasonic, IR array 8 ch |
| Aktuator | DC Motor, Servo, Stepper |
| Display | OLED 128×64 px (SSD1306, I2C) |
| PLC | Schneider Electric / Siemens |
| IoT Node | ESP32 DevKit v1 / Wemos LOLIN S2 Mini |

---

## Instalasi

### Prasyarat Sistem

```
OS      : Ubuntu 22.04 LTS
RAM     : Minimal 8 GB (disarankan 16 GB)
Storage : Minimal 50 GB free space
GPU     : Opsional (untuk training YOLO)
```

### 1. Install ROS 2 Humble

```bash
# Set locale
sudo apt update && sudo apt install locales
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8

# Tambah ROS 2 repository
sudo apt install software-properties-common curl -y
sudo add-apt-repository universe
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
  -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] \
  http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" \
  | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Install ROS 2 Humble Desktop + dev tools
sudo apt update && sudo apt upgrade
sudo apt install ros-humble-desktop ros-dev-tools

# Source otomatis
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

### 2. Clone Repository

```bash
cd ~
git clone -b v1 https://github.com/rofiqcp/Praktikum-Mekatronika-dan-Robotika.git
cd Praktikum-Mekatronika-dan-Robotika
```

### 3. Install ROS 2 Dependencies (Modul 04, 07, 08, 09)

```bash
sudo apt install \
  ros-humble-gazebo-ros-pkgs \
  ros-humble-navigation2 ros-humble-nav2-bringup \
  ros-humble-slam-toolbox \
  ros-humble-moveit \
  ros-humble-ros2-control ros-humble-ros2-controllers \
  ros-humble-joint-state-publisher-gui \
  ros-humble-xacro
```

### 4. Install PlatformIO (Modul 10 — ESP32)

```bash
# Install PlatformIO Core
pip install platformio

# Atau via VS Code Extension: cari "PlatformIO IDE"
# Board: lolin_s2_mini sudah dikonfigurasi di platformio.ini
```

### 5. Install Python Packages

```bash
pip install pymodbus mediapipe opencv-python ultralytics \
            pyqt5 paho-mqtt fastapi uvicorn
```

---

## Quick Start

### Test ROS 2

```bash
# Terminal 1
ros2 run demo_nodes_cpp talker

# Terminal 2
ros2 run demo_nodes_cpp listener
```

### Jalankan Simulasi Gazebo (Modul 04)

```bash
cd "Modul04 ROS Dasar dan Gazebo"
colcon build --symlink-install
source install/setup.bash
ros2 launch gazebo_praktikum percobaan1_empty_world.launch.py
```

### Upload Program Line Follower ke ESP32 (Modul 10)

```bash
cd "Modul10 LineFollower/Program/05PID"

# Build
pio run

# Upload (pastikan kabel USB data, bukan charge)
pio run -t upload

# Monitor serial
pio device monitor -b 115200
```

### Jalankan Web Server (Modul 05)

```bash
cd "Modul05 WebServer FullStack JavaScript/Project Server/03-weather-app"
npm install
npm run dev
```

---

## Dokumentasi Per Modul

Setiap modul dilengkapi file dokumentasi berikut:

| File | Isi |
|------|-----|
| `Jobsheet.md` | Panduan praktikum step-by-step lengkap |
| `Materi.md` | Teori, rumus, dan konsep dasar |
| `Project.md` | Spesifikasi project akhir beserta rubrik |
| `Tugas Video.md` | Format, struktur, dan rubrik penilaian video |
| `notebookllm.txt` | Prompt 45 slide untuk NotebookLLM (Modul 10) |

---

## Penilaian

### Komponen Nilai Per Modul

| Komponen | Bobot |
|----------|-------|
| Kehadiran & Partisipasi | 10% |
| Jobsheet / Laporan Praktikum | 30% |
| Implementasi Project | 40% |
| Video Presentasi | 20% |

### Format Laporan Jobsheet

1. Cover (Nama, NIM, Program Studi, Tanggal)
2. Tujuan Praktikum
3. Alat dan Bahan
4. Langkah Kerja (dengan screenshot tiap langkah)
5. Hasil Pengamatan dan Analisis
6. Kesimpulan dan Saran
7. Referensi

### Format Video Presentasi (Modul 10)

- **Durasi:** 30–60 menit (disarankan 40–50 menit)
- **Resolusi:** Minimal 720p (disarankan 1080p)
- **Bahasa:** Indonesia
- **Judul YouTube:** `[Modul10] Line Follower — NamaMahasiswa — NIM — PraktikumMekatronika`
- **Visibilitas:** Publik atau Unlisted (bukan Privat)
- **Submit:** Link YouTube + foto tabel kalibrasi + foto tabel tuning

**Rubrik Video (100 poin):**

| Aspek | Poin |
|-------|------|
| Kelengkapan konten (identitas, teori, upload, kalibrasi, tuning, demo) | 40 |
| Kualitas penjelasan (rumus PID, analisis tuning, kendala & solusi) | 30 |
| Teknis video (resolusi, audio, screen record, deskripsi YouTube) | 15 |
| Ketepatan waktu pengumpulan | 10 |
| Kreativitas | 5 |

---

## Troubleshooting

### ESP32 / PlatformIO

**Upload gagal (Failed to connect):**
```bash
# 1. Gunakan kabel USB data (bukan charge-only)
# 2. Tahan tombol BOOT di ESP32, klik Upload, lepas BOOT setelah "Connecting..."
# 3. Tutup Serial Monitor sebelum upload
# 4. Linux: tambah user ke grup dialout
sudo usermod -a -G dialout $USER
# Logout dan login kembali
```

**Library tidak ditemukan:**
```bash
# Cek platformio.ini sudah ada lib_deps yang benar:
# lib_deps = adafruit/Adafruit SSD1306, adafruit/Adafruit GFX Library
pio lib install
```

### ROS 2

**Command not found:**
```bash
source /opt/ros/humble/setup.bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
```

**Gazebo tidak muncul:**
```bash
sudo apt install --reinstall ros-humble-gazebo-ros-pkgs
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/usr/share/gazebo/models
```

**colcon build error:**
```bash
cd <workspace>
rm -rf build/ install/ log/
colcon build --symlink-install
source install/setup.bash
```

### Robot Line Follower

**Robot zig-zag:** Turunkan Kp atau naikkan Kd

**Robot keluar tikungan:** Turunkan base speed, naikkan Kd

**Robot offset satu sisi:** Kalibrasi ulang threshold sensor, tambah Ki=0.001

**Robot lost line / stuck:** Cek threshold sensor, aktifkan recovery handler

**Motor terbalik:** Set `MOTOR_LEFT_INVERT=true` atau `MOTOR_RIGHT_INVERT=true` di `config.h`

### Node.js

**Version mismatch:**
```bash
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash
source ~/.bashrc
nvm install 18 && nvm use 18
```

---

## Progress Tracking

Checklist penyelesaian modul:

- [x] Modul 01: Desain PCB (EasyEDA)
- [x] Modul 02: Setup ROS 2 Humble
- [x] Modul 03: Desain Mekanik (Fusion 360)
- [x] Modul 04: ROS Dasar dan Simulasi Gazebo
- [x] Modul 05: Web Server Full-Stack JavaScript
- [x] Modul 06: Web Server IoT MQTT + ESP32
- [x] Modul 07: Computer Vision ROS 2 + YOLO
- [x] Modul 08: Robot Manipulator ROS 2 + MoveIt
- [x] Modul 09: PLC + AI (MediaPipe) + Modbus
- [x] Modul 10: Line Follower ESP32 S2 Mini + PID

---

## Referensi

### Dokumentasi Resmi

- [ROS 2 Humble — Dokumentasi Resmi](https://docs.ros.org/en/humble/)
- [Gazebo Sim — Dokumentasi](https://gazebosim.org/docs)
- [MoveIt 2 — Humble](https://moveit.picknik.ai/humble/index.html)
- [Nav2 — Navigation2](https://navigation.ros.org/)
- [Ultralytics YOLO](https://docs.ultralytics.com/)
- [PlatformIO Docs](https://docs.platformio.org/)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [EasyEDA Tutorial](https://docs.easyeda.com/)

### Buku Referensi

- *Programming Robots with ROS* — Morgan Quigley, Brian Gerkey, William Smart
- *Robotics, Vision and Control* — Peter Corke (2nd ed.)
- *Modern Robotics: Mechanics, Planning, and Control* — Kevin Lynch, Frank Park
- *Embedded Systems: Introduction to ARM Cortex-M Microcontrollers* — Jonathan Valvano
- *Feedback Control of Dynamic Systems* — Franklin, Powell, Emami-Naeini

### Online Courses & Tutorials

- [The Construct — ROS 2 Courses](https://www.theconstructsim.com/)
- [ROS 2 Tutorials Official](https://docs.ros.org/en/humble/Tutorials.html)
- [OpenCV Python Tutorials](https://docs.opencv.org/4.x/d6/d00/tutorial_py_root.html)
- [PlatformIO ESP32 Guide](https://docs.platformio.org/en/latest/boards/espressif32/lolin_s2_mini.html)

---

## Update Log

### v1 — 2026-05-10
- Restrukturisasi lengkap: 10 modul final (Modul 01–10)
- Modul 10 LineFollower: materi lengkap 1303 baris (17 BAB), jobsheet 1170 baris
- Penambahan 6 sub-program PlatformIO ESP32 S2 Mini
- Tuning PID sistematis: 3 level referensi, case study, perhitungan manual
- Troubleshooting komprehensif: hardware, software, performa, power, emergency
- 10 variasi project (⭐ hingga ⭐⭐⭐⭐)
- notebookllm.txt: 45 slide × tepat 280 karakter
- README dan .gitignore diperbarui

### v0.1 — 2026-05-09
- Initial release: 10 modul, dokumentasi dasar

---

## Lisensi

Repository ini dilisensikan di bawah [MIT License](LICENSE).

---

## Tim Pengajar

**Dosen Pengampu:**
- **Rofiq Cahyo Prayogo, S.T., M.T.**

**Program Studi:** Sarjana Terapan Teknologi Rekayasa Otomasi

---

**Selamat belajar! 🤖**

*Repository ini dibuat untuk keperluan pendidikan di Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi.*
