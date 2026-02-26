# TUGAS VIDEO MODUL 02: SETUP UBUNTU 22.04, ROS 2 HUMBLE, DAN VERIFIKASI DASAR

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 02 – Setup ROS  
**Sifat Tugas:** Individual  
**Durasi Video:** 10–20 menit  
**Format:** MP4 (minimal 720p)  
**Platform Upload:** LMS / Google Drive / YouTube Unlisted

---

## A. KETENTUAN UMUM

1. Video dikerjakan **per orang**.
2. Wajib menampilkan **nama dan NIM** di awal video.
3. Gunakan **screen recording** yang jelas (bukan slideshow).
4. Narasi menggunakan Bahasa Indonesia yang runtut.
5. Semua langkah wajib dijelaskan sambil demonstrasi nyata.

---

## B. URUTAN WAJIB ISI VIDEO

Urutan ini harus diikuti sesuai instruksi praktikum:

### BAGIAN 1 — BACKUP DATA AWAL LAPTOP

- [ ] Tunjukkan folder/data penting yang akan dibackup
- [ ] Jelaskan media backup yang dipakai (eksternal/cloud)
- [ ] Tunjukkan bukti backup selesai

### BAGIAN 2 — DISK MANAGEMENT (SIAPKAN 50 GB)

- [ ] Buka `Disk Management` di Windows
- [ ] Lakukan `Shrink Volume`
- [ ] Sisakan **50 GB unallocated** untuk Ubuntu
- [ ] Jelaskan kenapa partisi ini disiapkan sebelum instalasi

### BAGIAN 3 — BOOT VENTOY DAN BIOS

- [ ] Tunjukkan media Ventoy sudah siap
- [ ] Masuk BIOS/UEFI
- [ ] Pastikan **Secure Boot = Disable**
- [ ] Boot ke Ventoy dan pilih ISO Ubuntu 22.04

### BAGIAN 4 — INSTAL UBUNTU 22.04

- [ ] Pilih mode **Minimal Installation**
- [ ] Pilih tipe instalasi **Something else (Other)**
- [ ] Buat partisi **swap 8 GB**
- [ ] Set **sisa partisi untuk root `/`**
- [ ] Lanjut hingga Ubuntu berhasil masuk desktop

### BAGIAN 5 — INSTAL VS CODE + GITHUB EDUCATION + COPILOT

- [ ] Install VS Code di Ubuntu
- [ ] Login akun GitHub Education di VS Code
- [ ] Install extension `GitHub Copilot`
- [ ] Tunjukkan Copilot aktif di editor

### BAGIAN 6 — INSTAL ROS HUMBLE

- [ ] Tambahkan repository ROS 2
- [ ] Install `ros-humble-desktop`
- [ ] Setup environment (`source /opt/ros/humble/setup.bash`)
- [ ] Tunjukkan ROS terpasang (`ros2 --version`)

### BAGIAN 7 — INSTAL LIBRARY WAJIB

Minimal tampilkan proses instalasi untuk kategori berikut:

- [ ] Machine Learning
- [ ] OpenCV
- [ ] YOLO
- [ ] Filter
- [ ] Kinematika
- [ ] Kontroler
- [ ] MoveIt
- [ ] Robot Arm
- [ ] Gazebo
- [ ] SLAM Navigasi

### BAGIAN 8 — VERIFIKASI ROS TALKER DAN LISTENER

- [ ] Jalankan `talker` di terminal 1
- [ ] Jalankan `listener` di terminal 2
- [ ] Tunjukkan data diterima listener
- [ ] Jelaskan arti hasil verifikasi

### BAGIAN 9 — PENUTUP

- [ ] Ringkas hasil instalasi
- [ ] Sebutkan kendala yang ditemui dan solusi singkat

---

## C. CONTOH PERINTAH YANG BOLEH DITAMPILKAN

### Instal ROS 2 Humble

```bash
sudo apt update && sudo apt install -y software-properties-common curl
sudo add-apt-repository universe -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
sudo apt install -y ros-humble-desktop
```

### Verifikasi Talker-Listener

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_py listener
```

---

## D. RUBRIK PENILAIAN VIDEO (100 POIN)

| No | Komponen Penilaian | Bobot |
|----|--------------------|-------|
| 1 | Kelengkapan urutan proses (backup → partisi → BIOS → install) | 25 |
| 2 | Ketepatan instalasi Ubuntu (minimal + partisi swap 8 GB dan `/`) | 15 |
| 3 | Ketepatan instalasi ROS Humble + library wajib | 25 |
| 4 | VS Code + GitHub Education + Copilot | 10 |
| 5 | Verifikasi talker-listener valid | 15 |
| 6 | Kualitas penjelasan dan rekaman | 10 |
| **TOTAL** |  | **100** |

---

## E. CHECKLIST SEBELUM SUBMIT

**Konten wajib:**

- [ ] Nama dan NIM tampil di awal
- [ ] Ada bukti backup data
- [ ] Ada bukti shrink disk 50 GB
- [ ] Ada bukti Secure Boot disable
- [ ] Ada bukti partisi swap 8 GB dan root `/`
- [ ] Ada instalasi VS Code + login GitHub Education + Copilot
- [ ] Ada instalasi ROS Humble
- [ ] Ada instalasi seluruh library kategori
- [ ] Ada verifikasi talker dan listener

**Kualitas video:**

- [ ] Resolusi minimal 720p
- [ ] Audio jelas
- [ ] Durasi 10–20 menit

**Penamaan file:**

- [ ] `Video_Modul02_[NIM]_[NamaLengkap].mp4`

---

Video yang tidak menampilkan verifikasi `talker-listener` atau tidak mengikuti urutan wajib dapat dinyatakan belum memenuhi syarat penilaian.
