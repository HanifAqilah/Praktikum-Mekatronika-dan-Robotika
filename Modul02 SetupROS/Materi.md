# MATERI MODUL 02: SETUP UBUNTU 22.04, ROS 2 HUMBLE, DAN LIBRARY ROBOTIKA

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS  
**OS Utama:** Ubuntu 22.04 LTS (Jammy)  
**Middleware:** ROS 2 Humble Hawksbill  
**Estimasi Waktu Belajar:** 6–10 Jam

---

## DAFTAR ISI

1. Pendahuluan
2. Prasyarat Sebelum Instalasi
3. Backup Data dan Persiapan Partisi
4. Instalasi Ubuntu 22.04 (Dual Boot)
5. Instalasi VS Code, GitHub Education, dan Copilot
6. Instalasi ROS 2 Humble
7. Instalasi Library Pendukung
8. Verifikasi ROS Talker dan Listener
9. Troubleshooting Umum

---

## 1. PENDAHULUAN

Pada modul ini mahasiswa menyiapkan lingkungan pengembangan robotika berbasis:
- **Ubuntu 22.04 LTS** sebagai sistem operasi
- **ROS 2 Humble** sebagai framework robotika
- Library tambahan untuk **machine learning, OpenCV, YOLO, filter, kinematika, kontroler, MoveIt, robot arm, Gazebo, dan SLAM navigasi**

Target akhir modul:
1. Sistem Ubuntu terpasang dual boot dengan alokasi minimal 50 GB
2. ROS 2 Humble berjalan normal
3. Library inti berhasil terpasang
4. Demo `talker` dan `listener` berjalan sukses

---

## 2. PRASYARAT SEBELUM INSTALASI

### 2.1 Spesifikasi Minimum Laptop

| Komponen | Minimum | Rekomendasi |
|----------|---------|-------------|
| CPU | Intel i5 / Ryzen 5 | Intel i7 / Ryzen 7 |
| RAM | 8 GB | 16 GB |
| Storage kosong | 50 GB | 100+ GB |
| Internet | Stabil | Stabil + kuota cukup |

### 2.2 File yang Harus Disiapkan

1. ISO Ubuntu 22.04 LTS Desktop
2. Ventoy (sudah disiapkan)
3. Flashdisk minimal 16 GB
4. Akun GitHub (disarankan sudah terverifikasi GitHub Education)

### 2.3 Catatan Penting

- **Secure Boot** sebaiknya dinonaktifkan dari BIOS/UEFI sebelum instalasi Ubuntu.
- Selalu lakukan backup data penting sebelum mengubah partisi.
- Gunakan mode instalasi **Minimal Installation** saat setup Ubuntu.

---

## 3. BACKUP DATA DAN PERSIAPAN PARTISI

### 3.1 Backup Data Awal

Backup minimal:
- Folder `Documents`, `Desktop`, `Downloads`, `Pictures`
- File proyek kuliah
- Bookmark browser dan credential penting

Media backup:
- HDD/SSD eksternal
- Cloud storage (Google Drive/OneDrive)

### 3.2 Mengecilkan Partisi di Windows

1. Di Windows, buka **Disk Management**
2. Klik kanan partisi utama (biasanya `C:`) → **Shrink Volume**
3. Sisakan ruang kosong **50 GB** (minimum) untuk Ubuntu
4. Pastikan muncul **Unallocated Space**

> Disarankan tidak membuat partisi baru dari Windows. Biarkan dalam status **Unallocated** agar dipakai installer Ubuntu.

---

## 4. INSTALASI UBUNTU 22.04 (DUAL BOOT)

### 4.1 Boot dari Ventoy

1. Masuk BIOS/UEFI
2. **Disable Secure Boot**
3. Set boot priority ke USB/Ventoy
4. Pilih ISO Ubuntu 22.04 dari menu Ventoy

### 4.2 Langkah Instalasi Ubuntu

1. Pilih **Install Ubuntu**
2. Keyboard layout sesuai kebutuhan
3. Pilih **Minimal Installation**
4. Pada tipe instalasi pilih **Something else (Other)**

### 4.3 Skema Partisi yang Wajib

Pada ruang kosong 50 GB:
- Buat partisi **swap 8 GB**
- Sisanya jadikan partisi root **`/`** dengan filesystem ext4

Contoh skema:

| Mount Point | Tipe | Ukuran |
|-------------|------|--------|
| swap | swap area | 8 GB |
| `/` | ext4 | sisa ruang |

5. Lanjutkan instalasi sampai selesai
6. Restart dan pastikan Ubuntu bisa boot normal

---

## 5. INSTALASI VS CODE, GITHUB EDUCATION, DAN COPILOT

### 5.1 Instal VS Code

```bash
sudo apt update
sudo apt install -y wget gpg
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
sudo install -D -o root -g root -m 644 packages.microsoft.gpg /etc/apt/keyrings/packages.microsoft.gpg
echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" | sudo tee /etc/apt/sources.list.d/vscode.list > /dev/null
rm -f packages.microsoft.gpg
sudo apt update
sudo apt install -y code
```

### 5.2 Login GitHub Education

1. Buka VS Code
2. Klik ikon akun kanan bawah → **Sign in with GitHub**
3. Login dengan akun GitHub Education

### 5.3 Instal Ekstensi Copilot

Di VS Code Extensions, install:
- `GitHub Copilot`
- `GitHub Copilot Chat` (opsional, disarankan)

---

## 6. INSTALASI ROS 2 HUMBLE

### 6.1 Setup Repository ROS

```bash
sudo apt update && sudo apt install -y software-properties-common curl
sudo add-apt-repository universe -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
```

### 6.2 Install ROS Humble Desktop

```bash
sudo apt install -y ros-humble-desktop python3-colcon-common-extensions python3-rosdep python3-vcstool
sudo rosdep init
rosdep update
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

### 6.3 Cek Instalasi

```bash
printenv | grep ROS
ros2 --version
```

---

## 7. INSTALASI LIBRARY PENDUKUNG

Jalankan per kategori berikut.

### 7.1 Machine Learning

```bash
sudo apt install -y python3-pip python3-numpy python3-scipy python3-pandas python3-sklearn python3-matplotlib python3-seaborn
pip3 install --user jupyter
```

### 7.2 OpenCV

```bash
sudo apt install -y python3-opencv libopencv-dev ros-humble-cv-bridge ros-humble-image-transport
```

### 7.3 YOLO

```bash
pip3 install --user ultralytics
```

### 7.4 Filter

```bash
sudo apt install -y ros-humble-filters ros-humble-laser-filters ros-humble-robot-localization
```

### 7.5 Kinematika

```bash
sudo apt install -y ros-humble-orocos-kdl ros-humble-kdl-parser ros-humble-tf2-kdl
```

### 7.6 Kontroler

```bash
sudo apt install -y ros-humble-ros2-control ros-humble-ros2-controllers ros-humble-control-toolbox
```

### 7.7 MoveIt

```bash
sudo apt install -y ros-humble-moveit
```

### 7.8 Robot Arm

```bash
sudo apt install -y ros-humble-urdf ros-humble-xacro ros-humble-joint-state-publisher ros-humble-joint-state-publisher-gui ros-humble-rviz2
```

### 7.9 Gazebo

```bash
sudo apt install -y ros-humble-gazebo-ros-pkgs ros-humble-gazebo-plugins ros-humble-gazebo-ros2-control
```

### 7.10 SLAM Navigasi

```bash
sudo apt install -y ros-humble-navigation2 ros-humble-nav2-bringup ros-humble-slam-toolbox
```

---

## 8. VERIFIKASI ROS TALKER DAN LISTENER

Buka 2 terminal:

### Terminal 1

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```

### Terminal 2

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_py listener
```

Jika berhasil, listener akan menerima pesan berkala dari talker.

---

## 9. TROUBLESHOOTING UMUM

| Masalah | Penyebab Umum | Solusi |
|--------|----------------|--------|
| `ros2: command not found` | Environment belum di-source | Jalankan `source /opt/ros/humble/setup.bash` |
| Gagal apt install ROS | Repo/key belum benar | Ulangi setup repo ROS |
| Copilot tidak aktif | Belum login GitHub / lisensi | Login ulang akun GitHub Education |
| Ubuntu tidak muncul di boot menu | Boot order berubah | Atur ulang boot priority di BIOS |

---

Dokumen ini menjadi acuan utama untuk praktikum Modul 02 dan tugas video instalasi.
