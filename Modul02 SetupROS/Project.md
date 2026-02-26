# PROJECT MODUL 02: SETUP LINGKUNGAN ROS 2 UNTUK PRAKTIKUM LANJUTAN

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS  
**Pengerjaan:** Per Kelompok (maks. 4 orang)  
**Platform:** Ubuntu 22.04 + ROS 2 Humble

---

## A. DESKRIPSI PROJECT

Setiap kelompok menyiapkan **1 lingkungan development standar** yang siap dipakai untuk modul 03–12, meliputi:
- Dual boot Ubuntu 22.04
- ROS 2 Humble
- Library robotika lanjutan (ML, visi komputer, simulasi, kontrol, perencanaan gerak, SLAM navigasi)
- Verifikasi node dasar ROS (`talker` dan `listener`)

Project ini menjadi fondasi agar modul Fusion360, IoT, Line Follower, Wall Follower, ROS Kinematika, OpenCV/YOLO, SLAM, dan Robot ARM berjalan tanpa kendala environment.

---

## B. TARGET TEKNIS WAJIB

### B.1 Sistem Operasi

| Item | Target |
|------|--------|
| Ubuntu | 22.04 LTS |
| Metode instalasi | Dual boot via Ventoy |
| Alokasi disk | Minimal 50 GB |
| Partisi swap | 8 GB |
| Partisi root | Sisa ruang (`/`) |
| Secure Boot | Disable |

### B.2 Software Utama

| Item | Target |
|------|--------|
| VS Code | Terinstal dan berjalan |
| GitHub Education | Login berhasil |
| GitHub Copilot | Extension aktif |
| ROS 2 | Humble Desktop |

### B.3 Library Tambahan

Kelompok wajib memasang library berikut:
1. Machine Learning
2. OpenCV
3. YOLO
4. Filter
5. Kinematika
6. Kontroler
7. MoveIt
8. Robot Arm
9. Gazebo
10. SLAM Navigasi

---

## C. PEMBAGIAN TUGAS ANGGOTA

| No | Anggota | NIM | Tanggung Jawab |
|----|---------|-----|----------------|
| 1 | | | Backup data, partisi disk, BIOS & Ventoy |
| 2 | | | Instal Ubuntu + konfigurasi awal |
| 3 | | | Instal VS Code, GitHub, Copilot, ROS Humble |
| 4 | | | Instal library lanjutan + verifikasi talker/listener |

> Semua anggota tetap wajib memahami seluruh alur, walau ada pembagian tugas.

---

## D. ALUR KERJA PROJECT

### Minggu 1
- Backup data semua laptop anggota
- Shrink disk dan siapkan 50 GB unallocated
- Disable Secure Boot, boot Ventoy

### Minggu 2
- Instal Ubuntu 22.04 (Minimal Installation)
- Partisi manual: swap 8 GB + root `/`
- Update sistem dan install VS Code

### Minggu 3
- Login GitHub Education di VS Code
- Install Copilot
- Install ROS 2 Humble + tools pengembangan

### Minggu 4
- Install seluruh library pendukung
- Verifikasi `talker` dan `listener`
- Lengkapi dokumentasi dan bukti screenshot

---

## E. PERINTAH INSTALASI STANDAR PROJECT

### E.1 ROS 2 Humble

```bash
sudo apt update && sudo apt install -y software-properties-common curl
sudo add-apt-repository universe -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
sudo apt install -y ros-humble-desktop python3-colcon-common-extensions python3-rosdep python3-vcstool
sudo rosdep init
rosdep update
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

### E.2 Library Tambahan

```bash
sudo apt install -y python3-pip python3-numpy python3-scipy python3-pandas python3-sklearn python3-matplotlib python3-seaborn
pip3 install --user jupyter ultralytics
sudo apt install -y python3-opencv libopencv-dev ros-humble-cv-bridge ros-humble-image-transport
sudo apt install -y ros-humble-filters ros-humble-laser-filters ros-humble-robot-localization
sudo apt install -y ros-humble-orocos-kdl ros-humble-kdl-parser ros-humble-tf2-kdl
sudo apt install -y ros-humble-ros2-control ros-humble-ros2-controllers ros-humble-control-toolbox
sudo apt install -y ros-humble-moveit
sudo apt install -y ros-humble-urdf ros-humble-xacro ros-humble-joint-state-publisher ros-humble-joint-state-publisher-gui ros-humble-rviz2
sudo apt install -y ros-humble-gazebo-ros-pkgs ros-humble-gazebo-plugins ros-humble-gazebo-ros2-control
sudo apt install -y ros-humble-navigation2 ros-humble-nav2-bringup ros-humble-slam-toolbox
```

### E.3 Verifikasi Komunikasi ROS

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_py listener
```

---

## F. DELIVERABLES KELOMPOK

| No | Deliverable | Format |
|----|-------------|--------|
| 1 | Laporan setup environment | PDF |
| 2 | Daftar paket/library terinstal | TXT/PDF |
| 3 | Screenshot partisi dan BIOS | PNG/JPG |
| 4 | Screenshot VS Code + Copilot + GitHub login | PNG/JPG |
| 5 | Screenshot verifikasi talker/listener | PNG/JPG |
| 6 | Video ringkas proses setup kelompok | MP4 |

---

## G. RUBRIK PENILAIAN PROJECT KELOMPOK (100 POIN)

| No | Kriteria | Bobot |
|----|----------|-------|
| 1 | Backup, partisi, dan instalasi Ubuntu sesuai prosedur | 20 |
| 2 | Instalasi ROS 2 Humble berhasil dan environment benar | 20 |
| 3 | Kelengkapan instalasi 10 kategori library | 20 |
| 4 | Konfigurasi VS Code + GitHub Education + Copilot | 15 |
| 5 | Verifikasi talker-listener valid | 15 |
| 6 | Dokumentasi dan kerapian laporan | 10 |
| **TOTAL** |  | **100** |

---

## H. REFERENSI

1. https://ubuntu.com/download/desktop
2. https://docs.ros.org/en/humble/
3. https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debians.html
4. https://code.visualstudio.com/
5. https://github.com/features/copilot
6. https://navigation.ros.org/
7. https://moveit.picknik.ai/

---

Dokumen project ini menjadi acuan eksekusi teknis kelompok untuk memastikan lingkungan praktikum standar dan seragam.
