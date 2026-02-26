# JOBSHEET MODUL 02: SETUP UBUNTU 22.04, ROS 2 HUMBLE, DAN LIBRARY PENDUKUNG

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS  
**Pertemuan:** 3–4 (2 × 2 SKS)  
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

---

## B. ALAT DAN BAHAN

| No | Kebutuhan | Keterangan |
|----|-----------|------------|
| 1 | Laptop/PC | Minimal RAM 8 GB |
| 2 | Flashdisk bootable Ventoy | Berisi ISO Ubuntu 22.04 |
| 3 | Koneksi internet | Untuk apt, pip, VS Code, Copilot |
| 4 | Akun GitHub Education | Untuk aktivasi fitur mahasiswa |
| 5 | VS Code | Editor utama |

---

## C. KESELAMATAN DAN PENCEGAHAN RISIKO

1. **Wajib backup data** sebelum resize partisi.
2. Pastikan laptop terhubung charger selama instalasi.
3. Jangan menghapus partisi Windows saat tahap manual partition.
4. Dokumentasikan setiap langkah (screenshot/foto) untuk bukti praktikum.

---

## D. LANGKAH KERJA PRAKTIKUM

### D.1 Backup Data dan Persiapan Disk

1. Backup data penting ke storage eksternal/cloud.
2. Buka `Disk Management` di Windows.
3. Lakukan `Shrink Volume` pada partisi utama.
4. Sisakan **unallocated 50 GB** untuk Ubuntu.

**Checkpoint:**
- [ ] Backup selesai
- [ ] Ruang kosong 50 GB tersedia

---

### D.2 Boot Ventoy dan Pengaturan BIOS

1. Masuk BIOS/UEFI.
2. **Disable Secure Boot**.
3. Pilih USB/Ventoy sebagai boot device.
4. Jalankan installer Ubuntu 22.04.

**Checkpoint:**
- [ ] Berhasil boot installer Ubuntu
- [ ] Secure Boot sudah nonaktif

---

### D.3 Instal Ubuntu 22.04

1. Pilih `Install Ubuntu`.
2. Pilih `Minimal Installation`.
3. Pada tipe instalasi, pilih `Something else`.
4. Buat partisi:
   - Swap: **8 GB**
   - Root `/`: **sisa ruang** (ext4)
5. Lanjutkan hingga reboot.

**Checkpoint:**
- [ ] Ubuntu berhasil boot
- [ ] Partisi sesuai ketentuan

---

### D.4 Instal VS Code, Login GitHub Education, dan Copilot

Jalankan perintah:

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

Lanjutkan di aplikasi VS Code:
1. Sign in dengan akun GitHub Education
2. Install extension `GitHub Copilot`

**Checkpoint:**
- [ ] VS Code terinstal
- [ ] Login GitHub berhasil
- [ ] Copilot aktif

---

### D.5 Instal ROS 2 Humble

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

**Checkpoint:**
- [ ] `ros2 --version` sukses

---

### D.6 Instal Library Pendukung Modul Lanjut

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

**Checkpoint:**
- [ ] Semua paket terpasang tanpa error kritis

---

### D.7 Verifikasi Talker dan Listener

Terminal 1:

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```

Terminal 2:

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_py listener
```

**Checkpoint:**
- [ ] Listener menerima data dari talker
- [ ] Screenshot bukti verifikasi tersimpan

---

## E. TABEL PENGAMATAN

### E.1 Ringkasan Instalasi

| Item | Status | Catatan |
|------|--------|---------|
| Backup data | | |
| Shrink disk 50 GB | | |
| Disable Secure Boot | | |
| Ubuntu Minimal + partisi manual | | |
| VS Code + GitHub Education + Copilot | | |
| ROS 2 Humble | | |
| Library pendukung | | |
| Talker-listener | | |

### E.2 Kendala dan Solusi

| No | Kendala | Solusi |
|----|---------|--------|
| 1 | | |
| 2 | | |
| 3 | | |

---

## F. PERTANYAAN ANALISIS

1. Mengapa pada dual boot disarankan backup data sebelum resize partisi?
2. Apa alasan swap 8 GB tetap dipakai walau RAM modern sudah besar?
3. Jelaskan perbedaan fungsi ROS 2 package `navigation2` dan `slam_toolbox`.
4. Mengapa validasi `talker`–`listener` penting sebelum lanjut ke modul lanjutan?
5. Apa manfaat integrasi VS Code + GitHub Copilot untuk workflow robotika?

---

## G. KESIMPULAN

Tuliskan minimal 4 poin kesimpulan hasil praktikum:

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________

---

## H. LAMPIRAN WAJIB

- [ ] Screenshot Disk Management (ruang 50 GB unallocated)
- [ ] Screenshot BIOS (Secure Boot disable)
- [ ] Screenshot partisi installer Ubuntu (swap 8 GB + `/`)
- [ ] Screenshot Ubuntu berhasil boot
- [ ] Screenshot VS Code + login GitHub
- [ ] Screenshot extension Copilot aktif
- [ ] Screenshot instalasi ROS dan library
- [ ] Screenshot verifikasi `talker` dan `listener`

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
