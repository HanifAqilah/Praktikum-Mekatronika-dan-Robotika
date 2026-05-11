# TUGAS VIDEO MODUL 02: SETUP DAN DEMONSTRASI ROS 2

**Program Studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS dan Dasar Robotika  
**Dosen Pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.  
**Sifat Tugas:** Individual  
**Durasi Video:** 10–20 menit  
**Format:** MP4 (minimal 720p)  
**Platform Upload:** YouTube (Unlisted)  
**Deadline:** 2 Minggu Setelah Praktikum

---

## A. KETENTUAN UMUM

1. Video dikerjakan **per orang** (tidak boleh kelompok).
2. Wajib menampilkan **nama lengkap dan NIM** di awal video (bisa teks overlay atau narasi).
3. Gunakan **screen recording** yang jelas dan berkualitas tinggi (bukan slideshow/presentasi).
4. Narasi menggunakan Bahasa Indonesia yang runtut, jelas, dan dapat dipahami.
5. Semua langkah praktikum wajib dijelaskan sambil demonstrasi nyata (live demonstration).
6. Tampilkan wajah/suara praktikan (kamera depan laptop boleh menyala atau hanya audio).
7. Video diupload ke YouTube sebagai **Unlisted** (tidak publik), lalu kirimkan link-nya ke LMS.

---

## B. URUTAN WAJIB ISI VIDEO

Urutan ini harus diikuti sesuai instruksi praktikum. Jangan melewatkan satu pun bagian.

### BAGIAN 1 — PENDAHULUAN (1 Menit)

Tampilkan:
- [ ] Nama lengkap praktikan
- [ ] NIM
- [ ] Program studi (Sarjana Terapan Teknologi Rekayasa Otomasi)
- [ ] Modul (Modul 02 – Setup ROS)
- [ ] Pernyataan bahwa ini adalah rekaman praktikum individu

**Narasi Contoh:**
> "Halo, nama saya [Nama], NIM [NIM], dari Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi. Ini adalah video tugas Modul 02 Praktikum Mekatronika dan Robotika tentang Setup ROS 2 Humble."

---

### BAGIAN 2 — BACKUP DATA AWAL LAPTOP (1-2 Menit)

- [ ] Tunjukkan folder/data penting yang akan dibackup
- [ ] Jelaskan media backup yang dipakai (eksternal/cloud)
- [ ] Tunjukkan proses backup (copy-paste file atau sinkronisasi cloud)
- [ ] Tunjukkan bukti backup selesai (file ada di media backup)

**Penjelasan Teori Singkat:**
Jelaskan mengapa backup penting sebelum mengubah partisi disk (risiko data loss).

---

### BAGIAN 3 — DISK MANAGEMENT (SIAPKAN 50 GB) (2-3 Menit)

- [ ] Buka `Disk Management` di Windows
- [ ] Tunjukkan kondisi disk sebelum di-shrink
- [ ] Lakukan `Shrink Volume` pada partisi utama
- [ ] Masukkan ukuran: **51200 MB** (50 GB)
- [ ] Tunjukkan munculnya **Unallocated Space** 50 GB
- [ ] Jelaskan kenapa partisi ini disiapkan sebelum instalasi

**Penjelasan Teori:**
Jelaskan konsep partisi disk, apa itu unallocated space, dan mengapa tidak membuat partisi dari Windows.

---

### BAGIAN 4 — BOOT VENTOY DAN PENGATURAN BIOS (2-3 Menit)

- [ ] Tunjukkan media Ventoy sudah siap (flashdisk terbaca)
- [ ] Restart laptop dan masuk BIOS/UEFI
- [ ] Tunjukkan proses masuk BIOS (tampilkan tombol yang ditekan)
- [ ] Cari pengaturan **Secure Boot** dan pastikan **Disable**
- [ ] Set boot priority ke USB/Ventoy
- [ ] Save & Exit, boot ke Ventoy
- [ ] Pilih ISO Ubuntu 22.04 dari menu Ventoy

**Penjelasan Teori:**
Jelaskan apa itu Secure Boot, mengapa harus dinonaktifkan untuk instalasi Ubuntu dan ROS.

---

### BAGIAN 5 — INSTALASI UBUNTU 22.04 (2-3 Menit)

*Catatan: Jika proses instalasi memakan waktu lama, bisa dipercepat dengan time-lapse atau skip bagian tunggu, tapi tunjukkan langkah kuncinya.*

- [ ] Pilih mode **Minimal Installation**
- [ ] Pilih tipe instalasi **Something else (Other)**
- [ ] Tunjukkan pembuatan partisi **swap 8 GB**
- [ ] Tunjukkan pembuatan partisi **root `/`** dengan sisa ruang (ext4)
- [ ] Tunjukkan proses instalasi berjalan
- [ ] Setelah selesai, restart dan tunjukkan Ubuntu berhasil masuk desktop

**Penjelasan Teori:**
Jelaskan fungsi swap memory, mengapa root `/` menggunakan ext4, dan perbedaan dengan Windows filesystems.

---

### BAGIAN 6 — INSTALASI VS CODE + GITHUB EDUCATION + COPILOT (2 Menit)

- [ ] Buka terminal dan jalankan perintah instalasi VS Code
- [ ] Tunjukkan proses instalasi VS Code selesai
- [ ] Buka aplikasi VS Code
- [ ] Klik ikon akun → **Sign in with GitHub**
- [ ] Login dengan akun GitHub Education (tunjukkan prosesnya)
- [ ] Di Extensions, cari dan install `GitHub Copilot`
- [ ] Tunjukkan Copilot aktif di editor (bisa ketik kode sederhana dan Copilot memberi saran)

**Penjelasan Teori:**
Jelaskan manfaat GitHub Copilot untuk bantuan coding dalam robotika dan mengapa VS Code cocok untuk ROS development.

---

### BAGIAN 7 — INSTALASI ROS 2 HUMBLE (2-3 Menit)

- [ ] Tambahkan repository ROS 2:
```bash
sudo apt update && sudo apt install -y software-properties-common curl
sudo add-apt-repository universe -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
```
- [ ] Install `ros-humble-desktop`:
```bash
sudo apt install -y ros-humble-desktop python3-colcon-common-extensions python3-rosdep python3-vcstool
sudo rosdep init
rosdep update
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```
- [ ] Tunjukkan ROS terpasang (`ros2 --version`)
- [ ] Tunjukkan environment ROS (`printenv | grep ROS`)

**Penjelasan Teori:**
Jelaskan apa itu ROS 2, arsitekturnya (DDS), dan perbedaan dengan ROS 1.

---

### BAGIAN 8 — INSTALASI LIBRARY WAJIB (2-3 Menit)

Tampilkan proses instalasi untuk kategori berikut (bisa dipercepat dengan editing, tapi tunjukkan perintah dan hasilnya):

- [ ] Machine Learning:
```bash
sudo apt install -y python3-pip python3-numpy python3-scipy python3-pandas python3-sklearn python3-matplotlib python3-seaborn
pip3 install --user jupyter
```
- [ ] OpenCV:
```bash
sudo apt install -y python3-opencv libopencv-dev ros-humble-cv-bridge ros-humble-image-transport
```
- [ ] YOLO:
```bash
pip3 install --user ultralytics
```
- [ ] Filter:
```bash
sudo apt install -y ros-humble-filters ros-humble-laser-filters ros-humble-robot-localization
```
- [ ] Kinematika:
```bash
sudo apt install -y ros-humble-orocos-kdl ros-humble-kdl-parser ros-humble-tf2-kdl
```
- [ ] Kontroler:
```bash
sudo apt install -y ros-humble-ros2-control ros-humble-ros2-controllers ros-humble-control-toolbox
```
- [ ] MoveIt:
```bash
sudo apt install -y ros-humble-moveit
```
- [ ] Robot Arm:
```bash
sudo apt install -y ros-humble-urdf ros-humble-xacro ros-humble-joint-state-publisher ros-humble-joint-state-publisher-gui ros-humble-rviz2
```
- [ ] Gazebo:
```bash
sudo apt install -y ros-humble-gazebo-ros-pkgs ros-humble-gazebo-plugins ros-humble-gazebo-ros2-control
```
- [ ] SLAM Navigasi:
```bash
sudo apt install -y ros-humble-navigation2 ros-humble-nav2-bringup ros-humble-slam-toolbox
```

**Penjelasan Teori Singkat:**
Jelaskan fungsi dari masing-masing library (contoh: OpenCV untuk computer vision, MoveIt untuk motion planning, dll).

---

### BAGIAN 9 — VERIFIKASI ROS TALKER DAN LISTENER (2 Menit)

- [ ] Buka Terminal 1, jalankan:
```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```
- [ ] Buka Terminal 2, jalankan:
```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_py listener
```
- [ ] Tunjukkan `talker` mempublish pesan "Hello World"
- [ ] Tunjukkan `listener` menerima pesan tersebut
- [ ] Jelaskan arti verifikasi ini (komunikasi ROS berhasil)

**Penjelasan Teori:**
Jelaskan konsep Publisher-Subscriber, topics, dan mengapa verifikasi ini penting sebelum lanjut ke modul berikutnya.

---

### BAGIAN 10 — PENUTUP DAN REFLEKSI (1 Menit)

- [ ] Ringkas hasil instalasi (ROS 2 Humble + semua library berhasil)
- [ ] Sebutkan kendala yang ditemui selama proses (jika ada) dan solusinya
- [ ] Berikan kesan dan pesan setelah menyelesaikan modul ini
- [ ] Ucapkan terima kasih dan tutup video

---

## C. RUBRIK PENILAIAN VIDEO (100 POIN)

| No | Komponen Penilaian | Bobot | Skor Maksimal | Kriteria Penilaian |
|----|--------------------|-------|---------------|---------------------|
| 1 | **Kelengkapan Urutan Proses** (backup → partisi → BIOS → install Ubuntu → VS Code → ROS → library → verifikasi) | 25 | 25 | Sangat Lengkap (25), Lengkap (20), Kurang Lengkap (10), Tidak Lengkap (0) |
| 2 | **Ketepatan Instalasi Ubuntu** (minimal + partisi swap 8 GB dan `/`) | 15 | 15 | Sangat Tepat (15), Tepat (10), Kurang Tepat (5), Tidak Tepat (0) |
| 3 | **Ketepatan Instalasi ROS Humble + Library** (semua 10 kategori library terinstal) | 25 | 25 | Sangat Tepat (25), Tepat (20), Kurang Tepat (10), Tidak Tepat (0) |
| 4 | **VS Code + GitHub Education + Copilot** (login berhasil, extension aktif) | 10 | 10 | Sangat Baik (10), Baik (7), Cukup (4), Tidak Ada (0) |
| 5 | **Verifikasi Talker-Listener** (valid, data diterima listener) | 15 | 15 | Valid (15), Kurang Valid (8), Tidak Valid (0) |
| 6 | **Kualitas Video dan Penjelasan** (audio jelas, narasi runtut, visual berkualitas) | 10 | 10 | Sangat Baik (10), Baik (7), Cukup (4), Kurang (0) |
| **TOTAL** | | **100** | **100** | |

### Konversi Nilai

| Skor | Nilai Huruf | Nilai Angka |
|------|-------------|-------------|
| 90-100 | A | 4.0 |
| 80-89 | B | 3.0 |
| 70-79 | C | 2.0 |
| 60-69 | D | 1.0 |
| <60 | E | 0 |

---

## D. CHECKLIST SEBELUM SUBMIT

### Konten Wajib:

- [ ] Nama dan NIM tampil di awal video (teks atau narasi)
- [ ] Ada bukti backup data (folder/file yang dibackup)
- [ ] Ada bukti shrink disk 50 GB (Disk Management)
- [ ] Ada bukti BIOS (Secure Boot disable)
- [ ] Ada bukti partisi swap 8 GB dan root `/` (installer Ubuntu)
- [ ] Ada instalasi VS Code + login GitHub Education + Copilot aktif
- [ ] Ada instalasi ROS Humble (`ros2 --version` tampil)
- [ ] Ada instalasi seluruh library kategori (ML, OpenCV, YOLO, Filter, Kinematika, Kontroler, MoveIt, Robot Arm, Gazebo, SLAM)
- [ ] Ada verifikasi talker dan listener (terminal 1 dan 2)
- [ ] Ada penjelasan teori singkat di setiap tahapan
- [ ] Ada penutup dan refleksi

### Kualitas Video:

- [ ] Resolusi minimal 720p (HD)
- [ ] Audio jelas (tidak ada noise berlebih)
- [ ] Durasi 10–20 menit (tidak kurang, tidak lebih)
- [ ] Video menampilkan proses nyata (bukan rekaman slide)
- [ ] Video diupload ke YouTube sebagai **Unlisted**
- [ ] Link video dikirimkan ke LMS dengan format benar

### Penamaan File/Link:

- [ ] Format nama: `Video_Modul02_[NIM]_[NamaLengkap].mp4`
- [ ] Contoh: `Video_Modul02_12345678_JohnDoe.mp4`
- [ ] Di deskripsi YouTube, tulis: Nama, NIM, Modul 02 Setup ROS

---

## E. SANKSI DAN KETENTUAN KHUSUS

1. **Plagiarisme:** Jika terbukti mencontoh/sama persis dengan video orang lain, nilai langsung **E (0)**.
2. **Tidak Upload:** Jika video tidak diupload ke YouTube (Unlisted) dan link tidak dikirim, nilai langsung **E (0)**.
3. **Terlambat:** Keterlambatan pengumpulan akan mendapat pengurangan nilai **10 poin per hari**.
4. **Tidak Lengkap:** Jika ada bagian yang dilewatkan (misal: tidak ada verifikasi talker-listener), poin untuk bagian tersebut **0**.
5. **Kualitas Buruk:** Video tidak jelas (blur, audio tidak kedengaran) akan mendapat pengurangan poin signifikan.

---

## F. CONTOH SCRIPT VIDEO (OPSIONAL)

Berikut adalah contoh script singkat yang bisa diikuti (bukan harus dihafal, hanya panduan):

```
[Halaman Judul: Nama, NIM, Modul 02]

Halo semua, saya [Nama], NIM [NIM]. Hari ini saya akan mendemonstrasikan proses setup Ubuntu 22.04, ROS 2 Humble, dan library pendukung untuk Praktikum Mekatronika dan Robotika.

Langkah pertama, saya melakukan backup data penting ke harddisk eksternal. Backup sangat penting karena kita akan mengubah partisi disk, dan ada risiko data hilang.

[Tunjukkan backup]

Selanjutnya, saya membuka Disk Management di Windows dan melakukan shrink volume sebesar 50 GB untuk persiapan instalasi Ubuntu.

[Tunjukkan shrink disk]

Kemudian, saya masuk ke BIOS dan menonaktifkan Secure Boot, lalu boot dari Ventoy...

[Dan seterusnya mengikuti urutan yang sudah ditentukan]
```

---

## G. LINK PENGUMPULAN

**Kirim link video YouTube (Unlisted) ke:**
- LMS (Learning Management System) Program Studi
- Atau via Email ke dosen pengampu dengan subject: `[Tugas Video Modul 02] NIM - Nama`

**Format Email/ LMS:**
```
Subject: Tugas Video Modul 02 - [NIM] - [Nama Lengkap]

Nama: [Nama Lengkap]
NIM: [NIM]
Prodi: Sarjana Terapan Teknologi Rekayasa Otomasi
Modul: 02 - Setup ROS

Link Video YouTube (Unlisted): [paste link here]

Terima kasih.
```

---

**Dosen Pengampu:**  
Rofiq Cahyo Prayogo, S.T., M.T.

**Deadline:** 2 Minggu Setelah Praktikum  
**Duration:** 10-20 Menit  
**Format:** MP4 720p, Upload YouTube Unlisted

---

*Pastikan semua checklist terpenuhi sebelum mengumpulkan tugas. Video yang tidak sesuai ketentuan akan dikembalikan/dinilai kurang.*
