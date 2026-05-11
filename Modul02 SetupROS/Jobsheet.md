# JOBSHEET MODUL 02: SETUP UBUNTU 22.04, ROS 2 HUMBLE, DAN LIBRARY PENDUKUNG

**Program Studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS  
**Dosen Pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.  
**Pertemuan:** 3–4 (2 × 2 SKS, Total 5 jam 40 menit)  
**Nama:** ___________________  
**NIM:** ___________________  
**Kelompok:** ___________________

---

## A. TUJUAN PRAKTIKUM

Setelah menyelesaikan jobsheet ini, mahasiswa mampu:
1. Menyiapkan partisi dual-boot untuk Ubuntu 22.04
2. Menginstal Ubuntu 22.04 menggunakan Ventoy dengan konfigurasi partisi manual
3. Menginstal ROS 2 Humble dan dependensi dasar
4. Menginstal library machine learning, OpenCV, YOLO, filter, kinematika, kontroler, MoveIt, robot arm, Gazebo, dan SLAM navigasi
5. Memverifikasi komunikasi ROS dengan node `talker` dan `listener`
6. Memahami dasar-dasar kinematika dan kontrol robot
7. Mengimplementasikan program sederhana dengan ROS 2

---

## B. ALAT DAN BAHAN

| No | Kebutuhan | Keterangan |
|----|-----------|------------|
| 1 | Laptop/PC | Minimal RAM 8 GB, Storage 50 GB free |
| 2 | Flashdisk bootable Ventoy | Berisi ISO Ubuntu 22.04 |
| 3 | Koneksi internet | Untuk apt, pip, VS Code, Copilot |
| 4 | Akun GitHub Education | Untuk aktivasi fitur mahasiswa |
| 5 | VS Code | Editor utama |
| 6 | Kertas dan alat tulis | Untuk sketsa diagram |

---

## C. KESELAMATAN DAN PENCEGAHAN RISIKO

1. **Wajib backup data** sebelum resize partisi.
2. Pastikan laptop terhubung charger selama instalasi.
3. Jangan menghapus partisi Windows saat tahap manual partition.
4. Dokumentasikan setiap langkah (screenshot/foto) untuk bukti praktikum.
5. Pastikan ventilasi laptop baik (hindari penutup ventilasi).
6. Jika terjadi kendala instalasi, jangan panik, catat error message.

---

## D. DASAR TEORI SINGKAT

### D.1 Konsep ROS 2

ROS 2 menggunakan sistem **graph** dengan nodes yang berkomunikasi melalui:
- **Topics:** Pub/sub (asynchronous)
- **Services:** Request/response (synchronous)
- **Actions:** Long-running tasks dengan feedback

### D.2 Kinematika Dasar

**Transformasi Koordinat (2D):**
```
[x'] = [cosθ  -sinθ] [x] + [t_x]
[y']   [sinθ   cosθ] [y]   [t_y]
```

**Kecepatan Angular:**
```
ω = Δθ / Δt  (rad/s)
```

### D.3 Kontrol Dasar

**Persamaan Gerak Lurus Berubah Beraturan (GLBB):**
```
s = v₀t + 1/2 at²
v = v₀ + at
```

Dimana:
- `s` = jarak (m)
- `v` = kecepatan (m/s)
- `a` = percepatan (m/s²)
- `t` = waktu (s)

---

## E. LANGKAH KERJA PRAKTIKUM

### TAHAP 1: PERSIAPAN DAN BACKUP DATA (45 Menit)

#### E.1.1 Backup Data Penting

1. Kumpulkan file penting:
   - Folder `Documents`, `Desktop`, `Downloads`, `Pictures`
   - File proyek kuliah
   - Bookmark browser
   - Credential penting (simpan di media terpisah)

2. Media backup yang disarankan:
   - HDD/SSD eksternal
   - Cloud storage (Google Drive/OneDrive/Dropbox)

3. Verifikasi backup:
   - Pastikan file dapat dibuka
   - Cek integritas data

**Checkpoint:**
- [ ] Backup data selesai
- [ ] Verifikasi backup berhasil

#### E.1.2 Persiapan Partisi di Windows

1. Buka `Disk Management` di Windows (Klik kanan Start → Disk Management)
2. Cari partisi utama (biasanya `C:`)
3. Klik kanan partisi → **Shrink Volume**
4. Masukkan jumlah ruang yang akan dikecilkan: **51200 MB** (50 GB)
5. Tunggu proses selesai
6. Pastikan muncul **Unallocated Space** sebesar 50 GB

> **PENTING:** Jangan membuat partisi baru dari Windows. Biarkan dalam status **Unallocated**.

**Checkpoint:**
- [ ] Ruang kosong 50 GB tersedia (unallocated)
- [ ] Screenshot Disk Management diambil

---

### TAHAP 2: INSTALASI UBUNTU 22.04 (90 Menit)

#### E.2.1 Boot dari Ventoy

1. Masuk ke BIOS/UEFI:
   - Matikan laptop
   - Nyalakan dan tekan tombol BIOS (F2, F12, Del, atau Esc - tergantung merk laptop)

2. Pengaturan BIOS:
   - **Disable Secure Boot** (wajib)
   - Set boot priority ke USB/Ventoy
   - Save & Exit

3. Boot ke Ventoy:
   - Masukkan flashdisk Ventoy
   - Pilih ISO Ubuntu 22.04 dari menu Ventoy
   - Pilih "Try or Install Ubuntu"

#### E.2.2 Instalasi Ubuntu

1. Pilih **Install Ubuntu**
2. Keyboard layout: **English (US)** atau sesuai preferensi
3. Pilih **Minimal Installation** (untuk mempercepat proses)
4. Pada pilihan "What apps would you like to install?":
   - Uncheck "Install third-party software" (opsional)
5. Pada tipe instalasi, pilih **Something else** (Manual partition)

#### E.2.3 Skema Partisi Manual

Pada ruang kosong 50 GB (unallocated), buat partisi:

**Partisi 1: Swap**
- Size: **8192 MB** (8 GB)
- Type: swap area
- Mount point: (tidak perlu)

**Partisi 2: Root**
- Size: **Sisa ruang** (sekitar 42 GB)
- Type: ext4 journaling file system
- Mount point: `/` (root)

6. Pilih disk untuk boot loader: pilih disk utama (biasanya `/dev/sda`)
7. Klik **Install Now** → Confirm
8. Isi lokasi, username, password
9. Tunggu proses instalasi selesai (30-45 menit)
10. Restart ketika diminta
11. Keluarkan flashdisk Ventoy saat restart

**Checkpoint:**
- [ ] Ubuntu berhasil boot
- [ ] Partisi swap 8 GB terdeteksi (`swapon --show`)
- [ ] Partisi root `/` terdeteksi (`df -h /`)
- [ ] Screenshot Ubuntu desktop diambil

---

### TAHAP 3: KONFIGURASI DASAR DAN VS CODE (45 Menit)

#### E.3.1 Update Sistem

Buka terminal (Ctrl+Alt+T) dan jalankan:

```bash
sudo apt update
sudo apt upgrade -y
```

#### E.3.2 Instal VS Code

```bash
# Install dependencies
sudo apt update
sudo apt install -y wget gpg

# Download Microsoft GPG key
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg

# Install the key
sudo install -D -o root -g root -m 644 packages.microsoft.gpg /etc/apt/keyrings/packages.microsoft.gpg

# Add VS Code repository
echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" | sudo tee /etc/apt/sources.list.d/vscode.list > /dev/null

# Clean up
rm -f packages.microsoft.gpg

# Install VS Code
sudo apt update
sudo apt install -y code
```

#### E.3.3 Login GitHub Education di VS Code

1. Buka VS Code
2. Klik ikon akun (pojok kiri bawah) → **Sign in with GitHub**
3. Browser akan terbuka, login dengan akun GitHub Education
4. Authorize VS Code
5. Kembali ke VS Code, pastikan sudah login

#### E.3.4 Instal GitHub Copilot Extension

1. Di VS Code, buka Extensions (Ctrl+Shift+X)
2. Cari "GitHub Copilot"
3. Klik **Install**
4. Jika diminta, authorize GitHub Copilot
5. Verifikasi Copilot aktif (icon Copilot di sidebar)

**Checkpoint:**
- [ ] VS Code terinstal (`code --version`)
- [ ] Login GitHub Education berhasil
- [ ] Copilot extension aktif
- [ ] Screenshot VS Code + Copilot diambil

---

### TAHAP 4: INSTALASI ROS 2 HUMBLE (60 Menit)

#### E.4.1 Setup Repository ROS

```bash
# Install dependencies
sudo apt update && sudo apt install -y software-properties-common curl

# Add universe repository
sudo add-apt-repository universe -y

# Add ROS 2 GPG key
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg

# Add ROS 2 repository
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Update packages
sudo apt update
```

#### E.4.2 Install ROS 2 Humble Desktop

```bash
# Install ROS 2 Humble Desktop (full installation)
sudo apt install -y ros-humble-desktop python3-colcon-common-extensions python3-rosdep python3-vcstool

# Initialize rosdep
sudo rosdep init
rosdep update

# Add ROS 2 to bashrc
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

#### E.4.3 Verifikasi Instalasi ROS

```bash
# Check ROS version
ros2 --version

# Check environment
printenv | grep ROS

# Expected output:
# ROS_DISTRO=humble
# ROS_LOCALHOST_ONLY=0
# ROS_PYTHON_VERSION=3
```

**Checkpoint:**
- [ ] `ros2 --version` menunjukkan versi Humble
- [ ] Environment ROS sudah ter-set
- [ ] Screenshot `ros2 --version` diambil

---

### TAHAP 5: INSTALASI LIBRARY PENDUKUNG (90 Menit)

#### E.5.1 Machine Learning Libraries

```bash
# Install Python ML libraries
sudo apt install -y python3-pip python3-numpy python3-scipy python3-pandas python3-sklearn python3-matplotlib python3-seaborn

# Install Jupyter Notebook
pip3 install --user jupyter

# Verify NumPy
python3 -c "import numpy as np; print(np.__version__)"
```

#### E.5.2 OpenCV

```bash
# Install OpenCV
sudo apt install -y python3-opencv libopencv-dev ros-humble-cv-bridge ros-humble-image-transport

# Verify OpenCV
python3 -c "import cv2; print(cv2.__version__)"
```

#### E.5.3 YOLO (Ultralytics)

```bash
# Install YOLO
pip3 install --user ultralytics

# Verify YOLO
python3 -c "from ultralytics import YOLO; print('YOLO installed')"
```

#### E.5.4 Filter Libraries

```bash
# Install filter packages
sudo apt install -y ros-humble-filters ros-humble-laser-filters ros-humble-robot-localization
```

#### E.5.5 Kinematika Libraries

```bash
# Install kinematics packages
sudo apt install -y ros-humble-orocos-kdl ros-humble-kdl-parser ros-humble-tf2-kdl

# Verify KDL
python3 -c "import PyKDL; print('KDL available')"
```

#### E.5.6 Controller Libraries

```bash
# Install ros2_control
sudo apt install -y ros-humble-ros2-control ros-humble-ros2-controllers ros-humble-control-toolbox
```

#### E.5.7 MoveIt (Motion Planning)

```bash
# Install MoveIt
sudo apt install -y ros-humble-moveit
```

#### E.5.8 Robot Arm Libraries

```bash
# Install robot description packages
sudo apt install -y ros-humble-urdf ros-humble-xacro ros-humble-joint-state-publisher ros-humble-joint-state-publisher-gui ros-humble-rviz2
```

#### E.5.9 Gazebo Simulation

```bash
# Install Gazebo
sudo apt install -y ros-humble-gazebo-ros-pkgs ros-humble-gazebo-plugins ros-humble-gazebo-ros2-control
```

#### E.5.10 SLAM Navigasi

```bash
# Install Navigation2 and SLAM Toolbox
sudo apt install -y ros-humble-navigation2 ros-humble-nav2-bringup ros-humble-slam-toolbox
```

**Checkpoint:**
- [ ] Semua library terpasang tanpa error fatal
- [ ] Verifikasi masing-masing library berhasil
- [ ] Screenshot instalasi diambil

---

### TAHAP 6: VERIFIKASI ROS 2 DASAR (30 Menit)

#### E.6.1 Talker-Listener Test

Buka **dua terminal** terpisah.

**Terminal 1 (Talker):**
```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```

Output yang diharapkan:
```
[INFO 1234567890.123456789] Publishing: 'Hello World: 0'
[INFO 1234567890.223456789] Publishing: 'Hello World: 1'
[INFO 1234567890.323456789] Publishing: 'Hello World: 2'
...
```

**Terminal 2 (Listener):**
```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_py listener
```

Output yang diharapkan:
```
[INFO 1234567890.123456789] I heard: 'Hello World: 0'
[INFO 1234567890.223456789] I heard: 'Hello World: 1'
[INFO 1234567890.323456789] I heard: 'Hello World: 2'
...
```

#### E.6.2 Inspect ROS Graph

Di terminal baru:
```bash
# List nodes
ros2 node list
# Expected: /talker, /listener

# List topics
ros2 topic list
# Expected: /chatter, /parameter_events, /rosout

# Echo topic
ros2 topic echo /chatter
# Should show the messages from talker

# Check topic info
ros2 topic info /chatter
# Should show publisher (talker) and subscriber (listener)
```

**Checkpoint:**
- [ ] Talker berjalan dan mempublish pesan
- [ ] Listener menerima pesan dari talker
- [ ] `ros2 node list` menampilkan node yang aktif
- [ ] `ros2 topic list` menampilkan topic `/chatter`
- [ ] Screenshot verifikasi diambil

---

### TAHAP 7: PRAKTIK SEDERHANA ROS 2 (35 Menit)

#### E.7.1 Membuat Workspace ROS 2

```bash
# Create workspace
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/

# Build workspace
colcon build

# Source workspace
echo "source ~/ros2_ws/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

#### E.7.2 Membuat Package Sederhana

```bash
cd ~/ros2_ws/src

# Create package
ros2 pkg create --build-type ament_python my_first_package --dependencies rclpy

# Navigate to package
cd my_first_package
```

#### E.7.3 Membuat Node Python Sederhana

Buat file `my_first_node.py` di `~/ros2_ws/src/my_first_package/my_first_package/`:

```python
import rclpy
from rclpy.node import Node

class MyFirstNode(Node):
    def __init__(self):
        super().__init__('my_first_node')
        self.get_logger().info('Hello from my first node!')
        self.counter_ = 0
        self.timer_ = self.create_timer(1.0, self.timer_callback)

    def timer_callback(self):
        self.counter_ += 1
        self.get_logger().info(f'Counter: {self.counter_}')

def main(args=None):
    rclpy.init(args=args)
    node = MyFirstNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

#### E.7.4 Edit setup.py

Edit `setup.py` di `~/ros2_ws/src/my_first_package/`:

```python
from setuptools import setup

package_name = 'my_first_package'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='your_name',
    maintainer_email='your_email@example.com',
    description='My first ROS 2 package',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'my_first_node = my_first_package.my_first_node:main',
        ],
    },
)
```

#### E.7.5 Build dan Run

```bash
cd ~/ros2_ws
colcon build
source install/setup.bash
ros2 run my_first_package my_first_node
```

Output yang diharapkan:
```
[INFO] Hello from my first node!
[INFO] Counter: 1
[INFO] Counter: 2
...
```

**Checkpoint:**
- [ ] Workspace berhasil dibuat
- [ ] Package berhasil dibuild
- [ ] Node sederhana berjalan
- [ ] Screenshot output node diambil

---

## F. TABEL PENGAMATAN

### F.1 Ringkasan Instalasi

| Item | Status | Catatan |
|------|--------|---------|
| Backup data | | |
| Shrink disk 50 GB | | |
| Disable Secure Boot | | |
| Ubuntu Minimal + partisi manual | | |
| VS Code + GitHub Education + Copilot | | |
| ROS 2 Humble | | |
| Machine Learning Libraries | | |
| OpenCV | | |
| YOLO | | |
| Filter Libraries | | |
| Kinematika Libraries | | |
| Controller Libraries | | |
| MoveIt | | |
| Robot Arm Libraries | | |
| Gazebo | | |
| SLAM Navigasi | | |
| Talker-listener | | |
| Custom Node | | |

### F.2 Pengamatan Talker-Listener

| No | Parameter | Hasil |
|----|-----------|-------|
| 1 | Frekuensi pesan talker (Hz) | |
| 2 | Jumlah pesan diterima listener | |
| 3 | Delay komunikasi (ms) | |

### F.3 Kendala dan Solusi

| No | Kendala | Solusi |
|----|---------|--------|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |

---

## G. PERTANYAAN ANALISIS

1. Mengapa pada dual boot disarankan backup data sebelum resize partisi? Jelaskan risikonya!

2. Apa alasan swap 8 GB tetap dipakai walau RAM modern sudah besar? Kapan swap digunakan?

3. Jelaskan perbedaan fungsi ROS 2 package `navigation2` dan `slam_toolbox`!

4. Mengapa validasi `talker`–`listener` penting sebelum lanjut ke modul lanjutan?

5. Apa manfaat integrasi VS Code + GitHub Copilot untuk workflow robotika?

6. Hitunglah kecepatan linear robot differential drive jika:
   - Diameter roda: 10 cm
   - RPM roda kiri: 30
   - RPM roda kanan: 30
   
   Gunakan rumus: `v = π × D × RPM / 60`

7. Jika robot berputar di tempat (roda kiri 30 RPM maju, roda kanan 30 RPM mundur), berapakah kecepatan angularnya?
   
   Gunakan rumus: `ω = (v_right - v_left) / wheelbase`
   Asumsi wheelbase = 15 cm

---

## H. KESIMPULAN

Tuliskan minimal 5 poin kesimpulan hasil praktikum:

1. ____________________________________________

2. ____________________________________________

3. ____________________________________________

4. ____________________________________________

5. ____________________________________________

---

## I. LAMPIRAN WAJIB

- [ ] Screenshot Disk Management (ruang 50 GB unallocated)
- [ ] Screenshot BIOS (Secure Boot disable)
- [ ] Screenshot partisi installer Ubuntu (swap 8 GB + `/`)
- [ ] Screenshot Ubuntu berhasil boot
- [ ] Screenshot VS Code + login GitHub
- [ ] Screenshot extension Copilot aktif
- [ ] Screenshot instalasi ROS dan library
- [ ] Screenshot verifikasi `talker` dan `listener`
- [ ] Screenshot custom node berjalan
- [ ] Foto praktikan sedang mengerjakan

---

## J. RUMUS-RUMUS PENDUKUNG

### J.1 Kinematika Differential Drive

**Kecepatan Linear:**
```
v = (v_right + v_left) / 2
v = π × D × RPM / 60
```

**Kecepatan Angular:**
```
ω = (v_right - v_left) / L
ω = 2π × (RPM_right - RPM_left) / 60 / L
```

Dimana:
- `D` = diameter roda (m)
- `L` = wheelbase (m)
- `RPM` = rotation per minute

### J.2 Transformasi 2D

**Rotasi:**
```
x' = x × cosθ - y × sinθ
y' = x × sinθ + y × cosθ
```

**Translasi + Rotasi:**
```
[x'] = [cosθ  -sinθ] [x] + [t_x]
[y']   [sinθ   cosθ] [y]   [t_y]
```

### J.3 PID Controller

```
u(t) = Kp × e(t) + Ki × ∫e(t)dt + Kd × de(t)/dt
```

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal Praktikum:** ___________________  
**Nilai:** ___________________

---

Dokumen jobsheet ini wajib diisi lengkap dan dikumpulkan bersama video tugas.
