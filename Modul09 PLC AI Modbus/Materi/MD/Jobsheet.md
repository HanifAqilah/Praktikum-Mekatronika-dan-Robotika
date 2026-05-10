# Jobsheet Praktikum Modul 09: PLC AI Modbus dengan ROS 2

**Program studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata kuliah:** Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum  
**Dosen pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.  
**Durasi:** 1 pertemuan praktikum 5 jam 40 menit  
**Topik:** Menjalankan sistem deteksi gestur tangan berbasis MediaPipe dan mengirim status ke PLC/V-BOX melalui Modbus TCP.

---

## 1. Tujuan Praktikum

Praktikan mampu:

1. Menyiapkan lingkungan ROS 2 Python untuk node AI vision dan Modbus TCP.
2. Menguji kamera, dependencies Python, dan workspace ROS 2.
3. Menghubungkan komputer ke PLC/V-BOX melalui jaringan Ethernet/Wi-Fi lab.
4. Menjalankan GUI MediaPipe Modbus ROS 2 Node.
5. Melakukan scan IP, koneksi Modbus TCP, dan write coil/register.
6. Memetakan status 5 jari ke alamat PLC.
7. Menguji hasil pada LED/output PLC.
8. Menganalisis error, delay, akurasi gestur, dan keberhasilan komunikasi.

---

## 2. Alat dan Bahan

### 2.1 Perangkat keras

1. Laptop/PC Ubuntu Linux dengan ROS 2.
2. Webcam internal atau USB camera.
3. PLC/V-BOX yang mendukung Modbus TCP.
4. Kabel Ethernet atau koneksi Wi-Fi lab.
5. Power supply PLC.
6. LED/output trainer PLC atau indikator internal PLC.
7. Switch Ethernet jika diperlukan.

### 2.2 Perangkat lunak

1. Ubuntu Linux.
2. ROS 2.
3. Python 3 dan pip.
4. OpenCV Python.
5. MediaPipe.
6. PyModbus.
7. PyQt5.
8. PyYAML.
9. Colcon build tools.
10. Workspace `ROS_PLC_AI`.

---

## 3. Keselamatan Kerja

1. Pastikan tegangan PLC sesuai spesifikasi.
2. Jangan menyentuh terminal listrik saat perangkat menyala.
3. Gunakan beban LED/trainer, bukan aktuator berdaya tinggi.
4. Jangan menulis register PLC yang tidak diketahui fungsinya.
5. Jangan mengubah konfigurasi jaringan kelompok lain.
6. Jangan melakukan scan jaringan di luar jaringan praktikum.
7. Tekan emergency stop atau matikan power jika output tidak terkendali.

---

## 4. Struktur Folder Praktikum

Folder utama:

```bash
/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09 PLC AI Modbus/ROS_PLC_AI
```

Isi penting:

```text
ROS_PLC_AI/
├── README.md
├── requirements.txt
├── run_node.sh
├── test_node.sh
├── src/
│   └── mediapipe_modbus_pkg/
├── build/
├── install/
└── log/
```

---

## 5. Pembagian Waktu Praktikum

| Tahap | Aktivitas | Waktu |
|---|---|---:|
| 1 | Briefing teori dan keselamatan | 30 menit |
| 2 | Cek perangkat, kamera, jaringan | 40 menit |
| 3 | Instalasi dependencies | 45 menit |
| 4 | Build dan test ROS 2 package | 45 menit |
| 5 | Koneksi Modbus TCP ke PLC | 50 menit |
| 6 | Uji gestur dan mapping coil | 70 menit |
| 7 | Analisis data dan troubleshooting | 40 menit |
| 8 | Dokumentasi video | 40 menit |
| Total | 5 jam 40 menit | 340 menit |

---

## 6. Persiapan Awal

### Langkah 1: Buka terminal

Gunakan terminal Linux. Pastikan berada pada user yang memiliki akses kamera dan workspace.

### Langkah 2: Masuk ke folder praktikum

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09 PLC AI Modbus/ROS_PLC_AI"
```

### Langkah 3: Cek isi folder

```bash
ls
```

Hasil minimal harus berisi:

```text
README.md requirements.txt run_node.sh test_node.sh src
```

### Langkah 4: Cek versi Python

```bash
python3 --version
pip3 --version
```

Catat hasil pada tabel:

| Item | Hasil |
|---|---|
| Python version | |
| pip version | |

---

## 7. Pemeriksaan ROS 2

### Langkah 1: Cek ROS 2 tersedia

```bash
ros2 --version
```

Jika command tidak ditemukan, source ROS 2 terlebih dahulu sesuai instalasi lab, contoh:

```bash
source /opt/ros/humble/setup.bash
```

### Langkah 2: Cek colcon

```bash
colcon --help
```

Jika belum tersedia:

```bash
sudo apt update
sudo apt install python3-colcon-common-extensions
```

### Langkah 3: Catat distro ROS 2

```bash
printenv ROS_DISTRO
```

| Item | Hasil |
|---|---|
| ROS_DISTRO | |

---

## 8. Instalasi Dependencies Python

### Langkah 1: Masuk folder workspace

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09 PLC AI Modbus/ROS_PLC_AI"
```

### Langkah 2: Install dependencies dari requirements

```bash
pip3 install -r requirements.txt
```

Jika terjadi konflik MediaPipe dan NumPy, jalankan:

```bash
pip3 install 'numpy<2' mediapipe 'opencv-python<4.10' pymodbus PyQt5 pyyaml
```

### Langkah 3: Verifikasi import library

```bash
python3 - <<'PY'
import cv2
import mediapipe
import pymodbus
import PyQt5
import yaml
print('Dependencies OK')
PY
```

Jika muncul `Dependencies OK`, lanjut ke tahap berikutnya.

---

## 9. Pemeriksaan Kamera

### Langkah 1: Cek device kamera

```bash
ls /dev/video*
```

Catat device yang muncul:

| Kamera | Device |
|---|---|
| Internal/USB | |

### Langkah 2: Uji kamera sederhana dengan Python

```bash
python3 - <<'PY'
import cv2
cap = cv2.VideoCapture(0)
print('Camera opened:', cap.isOpened())
ret, frame = cap.read()
print('Frame captured:', ret)
if ret:
    print('Resolution:', frame.shape)
cap.release()
PY
```

Jika `Camera opened: False`, coba:

1. Tutup aplikasi kamera lain.
2. Ganti index kamera dari 0 ke 1.
3. Cabut-pasang USB camera.
4. Cek permission user.

---

## 10. Build ROS 2 Package

### Langkah 1: Source ROS 2

```bash
source /opt/ros/humble/setup.bash
```

Sesuaikan `humble` dengan distro ROS 2 pada komputer lab.

### Langkah 2: Build package

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09 PLC AI Modbus/ROS_PLC_AI"
colcon build --packages-select mediapipe_modbus_pkg --symlink-install
```

### Langkah 3: Source workspace

```bash
source install/setup.bash
```

### Langkah 4: Cek package terdeteksi

```bash
ros2 pkg list | grep mediapipe_modbus_pkg
```

Hasil yang benar:

```text
mediapipe_modbus_pkg
```

---

## 11. Test Node

### Langkah 1: Beri permission script

```bash
chmod +x test_node.sh run_node.sh
```

### Langkah 2: Jalankan test

```bash
./test_node.sh
```

Catat hasil:

| Komponen | Status OK/Error | Catatan |
|---|---|---|
| Python | | |
| OpenCV | | |
| MediaPipe | | |
| PyModbus | | |
| PyQt5 | | |
| ROS 2 package | | |
| Kamera | | |

---

## 12. Konfigurasi Jaringan PLC/V-BOX

### Langkah 1: Identifikasi IP laptop

```bash
ip addr
```

Cari interface aktif, misalnya `eth0`, `enp3s0`, atau `wlan0`.

### Langkah 2: Pastikan satu subnet dengan PLC

Contoh:

| Perangkat | IP | Subnet |
|---|---|---|
| Laptop | 10.41.168.100 | /24 |
| PLC/V-BOX | 10.41.168.203 | /24 |

Satu subnet `/24` berarti tiga oktet pertama sama: `10.41.168.x`.

### Langkah 3: Ping PLC

```bash
ping 10.41.168.203
```

Ganti IP sesuai perangkat kelompok.

Kriteria berhasil:

```text
64 bytes from 10.41.168.203
```

### Langkah 4: Cek port Modbus TCP

```bash
nc -vz 10.41.168.203 502
```

Jika `succeeded`, port 502 terbuka.

Jika gagal:

1. Cek kabel/switch.
2. Cek IP PLC.
3. Cek firewall.
4. Cek Modbus TCP server PLC aktif.
5. Pastikan tidak ada konflik IP.

---

## 13. Menjalankan Aplikasi GUI

### Metode A: Script otomatis

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09 PLC AI Modbus/ROS_PLC_AI"
./run_node.sh
```

### Metode B: Manual

```bash
cd "/home/otomasi/Praktikum-Mekatronika-dan-Robotika/Modul09 PLC AI Modbus/ROS_PLC_AI"
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 run mediapipe_modbus_pkg mediapipe_modbus_node
```

Aplikasi berhasil jika GUI terbuka dan tampilan kamera muncul.

---

## 14. Scan dan Koneksi PLC

### Langkah 1: Buka GUI

Pastikan jendela aplikasi tampil.

### Langkah 2: Scan IP

Klik tombol **Scan**. Tunggu daftar perangkat muncul.

### Langkah 3: Pilih IP PLC/V-BOX

Pilih IP dari dropdown atau masukkan manual.

### Langkah 4: Set parameter Modbus

| Parameter | Nilai umum |
|---|---:|
| Port | 502 |
| Unit ID | 1 |
| Timeout | 1-3 s |

### Langkah 5: Connect

Klik **CONNECT**.

Kriteria berhasil:

1. Status berubah menjadi `CONNECTED`.
2. Tidak ada error timeout.
3. PLC menerima request Modbus.

---

## 15. Konfigurasi Mapping Jari

Buka menu settings lalu tab mapping jari.

Contoh mapping:

| Jari | Register type | Address | Enabled |
|---|---|---:|---|
| Jempol | coil | 0 | true |
| Telunjuk | coil | 1 | true |
| Tengah | coil | 2 | true |
| Manis | coil | 3 | true |
| Kelingking | coil | 4 | true |

Simpan konfigurasi. File konfigurasi tersimpan di:

```text
~/.ros/mediapipe_modbus_config.yaml
```

---

## 16. Uji Deteksi Gestur

### Langkah 1: Aktifkan kontrol gestur

Centang opsi kontrol gestur pada GUI.

### Langkah 2: Tampilkan tangan ke kamera

Gunakan pencahayaan cukup. Jarak ideal 30-80 cm dari kamera.

### Langkah 3: Uji tiap jari

Lakukan satu per satu:

| Percobaan | Gestur | Status GUI | Coil PLC | LED/output | Berhasil/Tidak |
|---:|---|---|---|---|---|
| 1 | Jempol | | | | |
| 2 | Telunjuk | | | | |
| 3 | Tengah | | | | |
| 4 | Manis | | | | |
| 5 | Kelingking | | | | |
| 6 | Semua jari | | | | |
| 7 | Kepal | | | | |

### Langkah 4: Uji kombinasi

Contoh kombinasi:

| Kombinasi | Kode biner | Nilai desimal |
|---|---:|---:|
| Tidak ada jari | 00000 | 0 |
| Jempol saja | 00001 | 1 |
| Telunjuk saja | 00010 | 2 |
| Jempol + telunjuk | 00011 | 3 |
| Semua jari | 11111 | 31 |

---

## 17. Uji Readback PLC

Jika GUI menyediakan indikator LED PLC, amati apakah status yang dibaca dari PLC sama dengan perintah.

Tabel readback:

| Coil | Command dari AI | Readback PLC | Sama? |
|---:|---:|---:|---|
| 0 | | | |
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |

Jika berbeda:

1. Cek alamat offset.
2. Cek register type.
3. Cek permission write PLC.
4. Cek mapping LED GUI.

---

## 18. Pengukuran Delay

### Metode video

1. Rekam tangan dan LED/output PLC dalam satu frame video.
2. Lakukan gestur ON.
3. Hitung selisih frame antara jari terbuka dan LED menyala.
4. Gunakan rumus:

\[
Delay=\frac{N_{selisih\_frame}}{FPS_{video}}
\]

Tabel pengukuran:

| Percobaan | FPS video | Frame gestur | Frame output | Selisih | Delay (s) |
|---:|---:|---:|---:|---:|---:|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| Rata-rata | | | | | |

Rata-rata:

\[
\bar{x}=\frac{x_1+x_2+...+x_n}{n}
\]

---

## 19. Pengukuran Akurasi Gestur

Lakukan 10 percobaan tiap gestur.

| Gestur | Jumlah benar | Total | Akurasi |
|---|---:|---:|---:|
| Jempol | | 10 | |
| Telunjuk | | 10 | |
| Tengah | | 10 | |
| Manis | | 10 | |
| Kelingking | | 10 | |
| Kepal | | 10 | |
| Semua jari | | 10 | |

Rumus:

\[
Accuracy=\frac{Jumlah\;benar}{Total\;percobaan}\times100\%
\]

---

## 20. Troubleshooting Praktikum

### Kasus 1: GUI tidak muncul

Solusi:

```bash
pip3 install PyQt5
source install/setup.bash
ros2 run mediapipe_modbus_pkg mediapipe_modbus_node
```

### Kasus 2: MediaPipe gagal import

Solusi:

```bash
pip3 install 'numpy<2' mediapipe
```

### Kasus 3: Kamera hitam

Solusi:

1. Cek kamera dipakai aplikasi lain.
2. Coba kamera index lain.
3. Uji dengan OpenCV.
4. Cek pencahayaan.

### Kasus 4: PLC timeout

Solusi:

```bash
ping IP_PLC
nc -vz IP_PLC 502
```

Lalu cek IP, subnet, port, kabel, dan konfigurasi Modbus server.

### Kasus 5: Alamat coil tidak sesuai

Solusi:

1. Cek apakah PLC memakai base 0 atau base 1.
2. Uji coil satu per satu.
3. Cocokkan dengan dokumentasi PLC.
4. Simpan mapping di settings.

---

## 21. Data yang Wajib Dikumpulkan

Semua data berikut harus ditampilkan dalam video tugas:

1. Screenshot GUI saat connected.
2. Screenshot mapping jari.
3. Foto/video rangkaian PLC dan laptop.
4. Tabel hasil uji setiap jari.
5. Tabel delay.
6. Tabel akurasi.
7. Catatan error dan solusi.

**Catatan:** Tidak ada laporan tertulis. Semua dokumentasi ada di video YouTube.

---

## 22. Pertanyaan Analisis

1. Jelaskan fungsi Modbus TCP pada sistem ini.
2. Mengapa alamat coil pada program dapat berbeda satu angka dari dokumentasi PLC?
3. Apa penyebab delay antara gestur dan output PLC?
4. Mengapa deteksi jari dapat salah saat pencahayaan buruk?
5. Mengapa sistem AI vision tidak boleh digunakan sebagai satu-satunya safety device?
6. Bagaimana cara membuat sistem lebih stabil saat status jari bergetar?
7. Bagaimana desain watchdog jika komunikasi Modbus putus?

---

## 23. Kriteria Keberhasilan Praktikum

Praktikum berhasil jika:

1. Aplikasi GUI dapat berjalan.
2. Kamera menampilkan citra real-time.
3. MediaPipe mendeteksi tangan.
4. Komputer dapat terkoneksi ke PLC/V-BOX.
5. Minimal 5 coil dapat dikendalikan dari gestur jari.
6. Readback atau indikator output sesuai mapping.
7. Praktikan dapat menjelaskan alur data dari kamera sampai PLC.
8. Praktikan mengumpulkan data delay dan akurasi.

---

## 24. Konten Video Tugas

Video YouTube minimal 15 menit harus berisi:

1. Identitas kelompok.
2. Tujuan praktikum.
3. Alat dan bahan.
4. Diagram sistem.
5. Langkah instalasi dan setup.
6. Konfigurasi IP dan Modbus.
7. Mapping jari ke coil/register.
8. Hasil uji gestur.
9. Pengukuran delay dan akurasi.
10. Troubleshooting.
11. Kesimpulan.

**Tidak ada laporan tertulis.** Lihat detail di file `Tugas Video.md`.

---

## 25. Penutup

Jobsheet ini memandu praktikan dari instalasi awal hingga sistem berhasil running. Fokus utama bukan hanya membuat program berjalan, tetapi memahami hubungan antara AI vision, ROS 2, komunikasi Modbus TCP, dan kontrol PLC secara aman dan terukur.
