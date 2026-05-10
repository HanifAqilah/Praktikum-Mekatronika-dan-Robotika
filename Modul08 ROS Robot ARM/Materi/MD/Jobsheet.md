# Jobsheet Modul 08: Praktikum ROS Robot ARM Dobot Magician

## Identitas Praktikum

- Program studi: Sarjana Terapan Teknologi Rekayasa Otomasi
- Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum
- Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.
- Modul: ROS Robot ARM Dobot Magician
- Workspace: `/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul08 ROS Robot ARM/ROS_DOBOT`
- Sistem: Ubuntu 22.04, ROS 2 Humble

## 1. Tujuan Praktikum

Praktikan mampu melakukan instalasi, setup, build, source workspace, koneksi Dobot, homing, visualisasi RViz, monitoring RQT, pengiriman goal PTP, kontrol gripper/suction cup, pembuatan sequence pick and place, serta troubleshooting dasar sampai robot berhasil berjalan.

## 2. Alokasi Waktu

Total: 5 jam 40 menit.

| Tahap | Durasi |
|---|---:|
| Briefing, keselamatan, teori singkat | 40 menit |
| Instalasi dan pengecekan workspace | 60 menit |
| Build, source, dan konfigurasi serial | 40 menit |
| Launch stack, homing, monitoring | 60 menit |
| Gerak PTP dan end effector | 80 menit |
| Program pick and place | 60 menit |
| Pengujian, dokumentasi, evaluasi | 40 menit |

## 3. Peralatan

1. Laptop/PC Ubuntu 22.04.
2. ROS 2 Humble Desktop.
3. Dobot Magician.
4. Kabel USB.
5. Power adapter Dobot.
6. Gripper atau suction cup.
7. Objek ringan untuk pick and place.
8. Alas kerja datar.
9. Kamera HP atau screen recorder untuk dokumentasi.
10. Koneksi internet saat instalasi awal.

## 4. Keselamatan Kerja

1. Pastikan area kerja bebas tangan, kabel, dan benda tak perlu.
2. Jangan menyentuh link robot ketika program berjalan.
3. Gunakan kecepatan rendah saat percobaan awal.
4. Siapkan tombol power atau emergency stop.
5. Jangan menjalankan program otomatis dan RQT manual bersamaan.
6. Jangan memberi target ekstrem di luar workspace.
7. Jangan mengangkat objek melebihi payload.
8. Setelah error, hentikan program dan baca alarm sebelum mengulang.
9. Saat memakai suction, pastikan objek tidak mudah jatuh.
10. Matikan robot setelah praktikum selesai.

## 5. Struktur Workspace

Folder utama:

```bash
/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul08 ROS Robot ARM/ROS_DOBOT
```

Struktur penting:

```text
ROS_DOBOT/
├── build/
├── install/
├── log/
└── src/
    ├── dobot_bringup/
    ├── dobot_control_panel/
    ├── dobot_demos/
    ├── dobot_description/
    ├── dobot_diagnostics/
    ├── dobot_driver/
    ├── dobot_end_effector/
    ├── dobot_homing/
    ├── dobot_kinematics/
    ├── dobot_motion/
    ├── dobot_msgs/
    ├── dobot_state_updater/
    └── dobot_visualization_tools/
```

## 6. Tahap 1: Cek Sistem Operasi

Buka terminal.

Cek versi Ubuntu:

```bash
lsb_release -a
```

Target hasil:

```text
Ubuntu 22.04 LTS
```

Cek arsitektur:

```bash
uname -m
```

Umumnya:

```text
x86_64
```

## 7. Tahap 2: Instal ROS 2 Humble

Jika ROS 2 belum terinstal, jalankan:

```bash
sudo apt update
sudo apt install software-properties-common
sudo add-apt-repository universe
sudo apt update
sudo apt install curl gnupg lsb-release
```

Tambahkan key dan repository ROS 2:

```bash
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
```

```bash
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
```

Instal ROS 2 Desktop:

```bash
sudo apt update
sudo apt install ros-humble-desktop
```

Instal development tools:

```bash
sudo apt install python3-colcon-common-extensions python3-rosdep python3-pip python3-vcstool
```

Inisialisasi rosdep jika belum:

```bash
sudo rosdep init
rosdep update
```

Source ROS 2:

```bash
source /opt/ros/humble/setup.bash
```

Opsional permanen:

```bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
```

## 8. Tahap 3: Instal Dependensi Dobot

Masuk ke folder source:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul08 ROS Robot ARM/ROS_DOBOT/src"
```

Instal paket Python:

```bash
pip3 install -r requirements.txt
```

Instal paket apt tambahan:

```bash
sudo apt install ros-humble-diagnostic-aggregator ros-humble-rqt-robot-monitor python3-pykdl python3-pyqt5 ros-humble-joint-state-publisher-gui
```

Jika RQT plugin belum muncul:

```bash
rqt --force-discover
```

## 9. Tahap 4: Setup Akses Serial

Cek perangkat USB:

```bash
lsusb
```

Cari perangkat seperti:

```text
Silicon Labs CP210x UART Bridge
```

atau:

```text
QinHeng Electronics
```

Cek port serial:

```bash
ls /dev/ttyUSB*
```

Tambahkan user ke grup dialout:

```bash
sudo usermod -a -G dialout $USER
```

Logout/login atau reboot:

```bash
reboot
```

Setelah masuk lagi, cek grup:

```bash
groups
```

Pastikan ada:

```text
dialout
```

## 10. Tahap 5: Build Workspace

Masuk workspace:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul08 ROS Robot ARM/ROS_DOBOT"
```

Source ROS 2:

```bash
source /opt/ros/humble/setup.bash
```

Instal dependency dari rosdep:

```bash
rosdep install -i --from-path src --rosdistro humble -y
```

Build:

```bash
colcon build
```

Jika berhasil, source workspace:

```bash
source install/setup.bash
```

Opsional permanen:

```bash
echo "source '/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul08 ROS Robot ARM/ROS_DOBOT/install/setup.bash'" >> ~/.bashrc
```

## 11. Tahap 6: Cek Package ROS 2

Setelah source workspace:

```bash
ros2 pkg list | grep dobot
```

Target terlihat:

```text
dobot_bringup
dobot_control_panel
dobot_demos
dobot_description
dobot_diagnostics
dobot_driver
dobot_end_effector
dobot_homing
dobot_kinematics
dobot_motion
dobot_msgs
dobot_state_updater
dobot_visualization_tools
```

## 12. Tahap 7: Visualisasi Tanpa Robot

Jalankan RViz dengan model Dobot:

```bash
ros2 launch dobot_description display.launch.py DOF:=4 tool:=extended_gripper use_camera:=true gui:=true
```

Yang harus diamati:

1. Model robot muncul di RViz.
2. Joint State Publisher GUI muncul.
3. Saat slider joint digeser, model berubah.
4. Frame dan link terlihat benar.

Catat screenshot RViz untuk laporan.

## 13. Tahap 8: Menyiapkan Robot Fisik

1. Letakkan Dobot pada meja datar.
2. Pastikan area kerja kosong.
3. Pasang end effector sesuai tugas.
4. Sambungkan power adapter.
5. Sambungkan USB ke PC.
6. Nyalakan Dobot.
7. Cek `lsusb` dan `/dev/ttyUSB0`.
8. Jangan menyentuh robot setelah stack berjalan.

## 14. Tahap 9: Set Tool Environment

Pilih salah satu sesuai alat:

Tanpa tool:

```bash
export MAGICIAN_TOOL=none
```

Suction cup:

```bash
export MAGICIAN_TOOL=suction_cup
```

Gripper:

```bash
export MAGICIAN_TOOL=gripper
```

Extended gripper:

```bash
export MAGICIAN_TOOL=extended_gripper
```

Cek:

```bash
echo $MAGICIAN_TOOL
```

## 15. Tahap 10: Launch Control Stack

Terminal 1:

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul08 ROS Robot ARM/ROS_DOBOT"
source /opt/ros/humble/setup.bash
source install/setup.bash
export MAGICIAN_TOOL=suction_cup
ros2 launch dobot_bringup dobot_magician_control_system.launch.py
```

Jika memakai gripper, ganti:

```bash
export MAGICIAN_TOOL=gripper
```

Jika launch berhasil, node utama aktif: driver, diagnostics, homing, trajectory validator, PTP action server, state updater, dan end effector service.

## 16. Tahap 11: Cek Node, Topic, Service, Action

Terminal 2:

```bash
source /opt/ros/humble/setup.bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul08 ROS Robot ARM/ROS_DOBOT"
source install/setup.bash
```

Cek node:

```bash
ros2 node list
```

Cek topic:

```bash
ros2 topic list
```

Cek service:

```bash
ros2 service list
```

Cek action:

```bash
ros2 action list
```

Cek tipe action:

```bash
ros2 action info /PTP_action
```

## 17. Tahap 12: Homing

Homing wajib dilakukan setelah robot menyala.

Pastikan area aman, lalu jalankan:

```bash
ros2 service call /dobot_homing_service dobot_msgs/srv/ExecuteHomingProcedure
```

Tunggu hingga selesai.

Cek pose TCP:

```bash
ros2 topic echo /dobot_pose_raw
```

Tekan `Ctrl+C` setelah data terbaca.

## 18. Tahap 13: Monitoring Joint dan TCP

Cek joint:

```bash
ros2 topic echo /joint_states
```

Cek TCP pose:

```bash
ros2 topic echo /dobot_TCP
```

Cek pose raw:

```bash
ros2 topic echo /dobot_pose_raw
```

Data yang dicatat:

1. Nama joint.
2. Sudut joint.
3. Posisi x, y, z.
4. Orientasi tool.
5. Perubahan data saat robot bergerak.

## 19. Tahap 14: Gerak PTP Pertama

Gunakan target aman dan kecepatan rendah.

```bash
ros2 action send_goal /PTP_action dobot_msgs/action/PointToPoint "{motion_type: 1, target_pose: [200.0, 0.0, 100.0, 0.0], velocity_ratio: 0.3, acceleration_ratio: 0.3}" --feedback
```

Uji target kedua:

```bash
ros2 action send_goal /PTP_action dobot_msgs/action/PointToPoint "{motion_type: 1, target_pose: [220.0, 50.0, 120.0, 0.0], velocity_ratio: 0.3, acceleration_ratio: 0.3}" --feedback
```

Uji target ketiga:

```bash
ros2 action send_goal /PTP_action dobot_msgs/action/PointToPoint "{motion_type: 1, target_pose: [220.0, -50.0, 120.0, 0.0], velocity_ratio: 0.3, acceleration_ratio: 0.3}" --feedback
```

Jika robot bergerak benar, catat feedback dan pose akhir.

## 20. Tahap 15: Membatalkan Gerak

Jika target salah atau perlu dihentikan:

```bash
ros2 service call /PTP_action/_action/cancel_goal action_msgs/srv/CancelGoal
```

Jika gerakan berbahaya, gunakan tombol power/emergency stop.

## 21. Tahap 16: Kontrol Gripper

Jika memakai gripper, buka gripper:

```bash
ros2 service call /dobot_gripper_service dobot_msgs/srv/GripperControl "{gripper_state: 'open', keep_compressor_running: true}"
```

Tutup gripper:

```bash
ros2 service call /dobot_gripper_service dobot_msgs/srv/GripperControl "{gripper_state: 'close', keep_compressor_running: true}"
```

Matikan kompresor:

```bash
ros2 service call /dobot_gripper_service dobot_msgs/srv/GripperControl "{gripper_state: 'open', keep_compressor_running: false}"
```

## 22. Tahap 17: Kontrol Suction Cup

Jika memakai suction cup, aktifkan suction:

```bash
ros2 service call /dobot_suction_cup_service dobot_msgs/srv/SuctionCupControl "{enable_suction: true, keep_compressor_running: true}"
```

Nonaktifkan suction:

```bash
ros2 service call /dobot_suction_cup_service dobot_msgs/srv/SuctionCupControl "{enable_suction: false, keep_compressor_running: false}"
```

Pastikan objek ringan dan permukaan cukup rata.

## 23. Tahap 18: RQT Control Panel

Jalankan RQT:

```bash
rqt -s dobot_control_panel
```

Gunakan panel untuk:

1. Melihat pose robot.
2. Mengirim posisi manual.
3. Mengatur velocity dan acceleration.
4. Mengaktifkan gripper atau suction.
5. Mengecek status.

Catatan: jangan menjalankan script otomatis ketika RQT sedang mengontrol robot secara manual.

## 24. Tahap 19: Diagnostics Viewer

Jalankan:

```bash
rqt -s rqt_robot_monitor
```

Amati:

1. Alarm aktif.
2. Warning komunikasi.
3. Error motion.
4. Status node.

Jika alarm muncul, catat teks alarm, penyebab, dan solusi.

## 25. Tahap 20: Menjalankan Demo Bawaan

Cek executable:

```bash
ros2 pkg executables dobot_demos
```

Jalankan homing client jika tersedia:

```bash
ros2 run dobot_demos homing_client
```

Jalankan PTP client:

```bash
ros2 run dobot_demos PTP_client
```

Jalankan pick and place:

```bash
ros2 run dobot_demos pick_and_place
```

Jika target demo tidak sesuai meja praktikum, hentikan dan edit target pada file demo sebelum mengulang.

## 26. Tahap 21: Membuat Program Praktikum Sendiri

Buat file Python di package demo atau jalankan dari script terpisah yang source workspace.

Contoh alur minimal:

```text
1. Import rclpy.
2. Buat node.
3. Buat ActionClient ke /PTP_action.
4. Buat client service suction/gripper.
5. Kirim target above_pick.
6. Kirim target pick.
7. Aktifkan end effector.
8. Kirim target above_pick.
9. Kirim target above_place.
10. Kirim target place.
11. Nonaktifkan end effector.
12. Kembali home.
```

Target awal yang relatif aman untuk diuji:

```text
home        = [200.0,   0.0, 120.0, 0.0]
above_pick  = [220.0,  60.0, 120.0, 0.0]
pick        = [220.0,  60.0,  45.0, 0.0]
above_place = [220.0, -60.0, 120.0, 0.0]
place       = [220.0, -60.0,  45.0, 0.0]
```

Sesuaikan `z` terhadap tinggi objek dan meja.

## 27. Tahap 22: Pengujian Pick and Place

Lakukan 10 percobaan.

Tabel data:

| Percobaan | Pick berhasil | Place berhasil | Waktu siklus | Error posisi | Catatan |
|---:|---|---|---:|---:|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |
| 5 | | | | | |
| 6 | | | | | |
| 7 | | | | | |
| 8 | | | | | |
| 9 | | | | | |
| 10 | | | | | |

Hitung:

```text
Success Rate = jumlah berhasil / 10 × 100%
```

```text
T_avg = ΣT_i / 10
```

```text
Error rata-rata = Σe_i / 10
```

## 28. Tahap 23: Analisis Data

Jawab pertanyaan berikut:

1. Apa fungsi homing pada Dobot?
2. Apa perbedaan topic, service, dan action?
3. Mengapa PTP action lebih cocok untuk gerak robot dibanding service biasa?
4. Apa penyebab target ditolak trajectory validator?
5. Apa pengaruh `velocity_ratio` terhadap keselamatan dan cycle time?
6. Apa pengaruh tinggi `z` terhadap keberhasilan pick?
7. Apa perbedaan gripper dan suction cup dari sisi objek yang cocok?
8. Apa error terbesar selama percobaan dan penyebabnya?
9. Bagaimana cara meningkatkan success rate?
10. Bagaimana cara membuat program lebih aman?

## 29. Checklist Keberhasilan

| Item | Status |
|---|---|
| ROS 2 Humble aktif | |
| Workspace berhasil build | |
| Package dobot terbaca | |
| Dobot terdeteksi `lsusb` | |
| User masuk grup dialout | |
| `MAGICIAN_TOOL` diset | |
| Launch stack berhasil | |
| Homing berhasil | |
| `/joint_states` terbaca | |
| `/dobot_TCP` terbaca | |
| Gerak PTP berhasil | |
| End effector berhasil | |
| Pick and place berhasil | |
| Data 10 percobaan lengkap | |
| Video proses dan screen record lengkap | |

## 30. Troubleshooting Cepat

| Masalah | Solusi |
|---|---|
| `Dobot is disconnected` | Cek USB, power, `lsusb`, port `/dev/ttyUSB0` |
| `Permission denied` | Tambahkan dialout dan reboot |
| `MAGICIAN_TOOL env var is not set` | Jalankan `export MAGICIAN_TOOL=suction_cup` atau `gripper` |
| `Goal rejected` | Target keluar workspace atau joint limit |
| Robot tidak bergerak | Cek action server, homing, alarm, kabel |
| End effector tidak aktif | Cek service list dan tool environment |
| RQT plugin hilang | Jalankan `rqt --force-discover` |
| Build gagal dependency | Jalankan `rosdep install` dan cek pip requirements |
| Pose tidak stabil | Homing ulang, turunkan speed, cek beban |
| Program bertabrakan dengan RQT | Tutup RQT manual control sebelum script otomatis |

## 31. Format Laporan Praktikum

Laporan minimal berisi:

1. Judul dan identitas kelompok.
2. Tujuan praktikum.
3. Dasar teori singkat robot arm dan ROS 2.
4. Alat dan bahan.
5. Langkah kerja.
6. Screenshot instalasi, build, launch, RViz/RQT.
7. Program yang digunakan.
8. Data 10 percobaan.
9. Perhitungan success rate, cycle time, error.
10. Analisis kegagalan dan solusi.
11. Kesimpulan.
12. Link video dan screen record.

## 32. Kriteria Minimal Lulus Praktikum

Praktikan dinyatakan memenuhi kompetensi minimal jika:

1. Dapat menjalankan workspace ROS 2 Dobot.
2. Dapat melakukan homing.
3. Dapat mengirim minimal 3 target PTP aman.
4. Dapat mengaktifkan end effector.
5. Dapat menjalankan sequence pick and place minimal 5 kali.
6. Dapat menjelaskan topic, service, action, dan fungsi package utama.
7. Menyerahkan video praktik dan screen record penjelasan.
