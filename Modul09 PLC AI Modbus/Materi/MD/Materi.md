# Materi Praktikum Modul 09: PLC, AI Vision, ROS 2, dan Modbus TCP

**Program studi:** Sarjana Terapan Teknologi Rekayasa Otomasi  
**Mata kuliah:** Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum  
**Dosen pengampu:** Rofiq Cahyo Prayogo, S.T., M.T.  
**Topik:** Integrasi deteksi gestur tangan berbasis AI dengan PLC melalui ROS 2 dan Modbus TCP.

---

## 1. Capaian Pembelajaran

Setelah mengikuti modul ini, praktikan mampu:

1. Menjelaskan prinsip kerja PLC, scan cycle, I/O digital, memori coil, register, dan komunikasi industri.
2. Menjelaskan Modbus TCP, struktur frame, addressing, function code, serta proses read/write data PLC.
3. Menjelaskan dasar AI vision, computer vision, landmark tangan, dan klasifikasi gestur sederhana.
4. Menjelaskan konsep ROS 2 node, package, topic, parameter, workspace, dan eksekusi node Python.
5. Menghubungkan kamera, aplikasi ROS 2, AI MediaPipe, GUI, jaringan Ethernet, dan PLC/V-BOX.
6. Melakukan konfigurasi IP, port, unit ID, mapping jari ke coil/register, dan uji kontrol output.
7. Menganalisis delay, sampling, error komunikasi, akurasi deteksi, dan keandalan sistem otomasi.

---

## 2. Gambaran Sistem

Sistem praktikum mengubah gerakan jari menjadi sinyal kontrol PLC. Kamera menangkap citra tangan, MediaPipe mendeteksi landmark tangan, program Python menentukan status setiap jari, GUI menampilkan status, lalu ROS 2 node mengirim status ke PLC melalui Modbus TCP.

Alur sistem:

```text
Tangan praktikan -> Kamera -> OpenCV -> MediaPipe Hands -> Logika status jari
-> GUI PyQt5 -> Modbus TCP Client -> PLC/V-BOX -> Coil/Register -> Output/LED/aktuator
```

Contoh pemetaan:

| Jari | Status | Coil PLC | Fungsi contoh |
|---|---:|---:|---|
| Jempol | 0/1 | M0 atau coil 0 | Start lampu 1 |
| Telunjuk | 0/1 | M1 atau coil 1 | Start lampu 2 |
| Tengah | 0/1 | M2 atau coil 2 | Start lampu 3 |
| Manis | 0/1 | M3 atau coil 3 | Start lampu 4 |
| Kelingking | 0/1 | M4 atau coil 4 | Start lampu 5 |

---

## 3. Dasar PLC

### 3.1 Definisi PLC

PLC atau Programmable Logic Controller adalah komputer industri yang dirancang untuk membaca input, menjalankan logika kontrol, dan menghasilkan output secara deterministik. PLC digunakan pada mesin produksi, conveyor, robotika, proses industri, panel kontrol, dan sistem keselamatan.

### 3.2 Komponen PLC

1. **CPU:** menjalankan program kontrol.
2. **Power supply:** menyediakan tegangan kerja.
3. **Input module:** membaca sensor, tombol, limit switch, proximity, encoder.
4. **Output module:** mengendalikan relay, lampu, solenoid, motor starter, inverter.
5. **Memory:** menyimpan program, bit internal, timer, counter, data register.
6. **Communication port:** Ethernet, RS-485, USB, Modbus, Profinet, EtherNet/IP, CANopen.

### 3.3 Scan Cycle PLC

PLC bekerja berulang dalam siklus:

1. Baca input fisik.
2. Eksekusi program ladder/ST/FBD.
3. Update output fisik.
4. Komunikasi dan diagnosis.

Rumus waktu respon sederhana:

\[
T_{response} \approx T_{input_filter}+T_{scan}+T_{output_delay}+T_{network}
\]

Jika input berubah tepat setelah proses baca input, output baru aktif pada scan berikutnya sehingga worst-case:

\[
T_{response,max} \approx 2T_{scan}+T_{filter}+T_{output}+T_{network}
\]

Frekuensi scan:

\[
f_{scan}=\frac{1}{T_{scan}}
\]

Contoh: jika `Tscan = 10 ms`, maka `fscan = 100 Hz`.

### 3.4 Jenis Data PLC

| Data | Makna | Contoh |
|---|---|---|
| X/I | input digital | tombol start |
| Y/Q | output digital | lampu, relay |
| M | internal relay/coil | bit bantu |
| T | timer | delay ON/OFF |
| C | counter | penghitung objek |
| D/R | data register | angka 16-bit/32-bit |
| AI/AQ | analog input/output | sensor 0-10 V, 4-20 mA |

### 3.5 Logika Boolean PLC

Operasi dasar:

\[
Y=A
\]

\[
Y=\overline{A}
\]

\[
Y=A \cdot B
\]

\[
Y=A+B
\]

\[
Y=(A \cdot B)+C
\]

Tabel kebenaran AND:

| A | B | Y=A·B |
|---:|---:|---:|
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

Tabel kebenaran OR:

| A | B | Y=A+B |
|---:|---:|---:|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 1 |

### 3.6 Latching Start-Stop

Persamaan latch motor:

\[
M=(Start+M)\cdot \overline{Stop}\cdot Safety
\]

Output:

\[
Y=M
\]

Artinya motor tetap menyala setelah tombol start dilepas, selama stop tidak aktif dan safety terpenuhi.

### 3.7 Timer

Timer ON delay aktif setelah input ON selama waktu preset.

\[
Q(t)=1 \quad jika \quad IN=1 \text{ dan } ET \ge PT
\]

Konversi waktu:

\[
PT_{ms}=PT_s \times 1000
\]

Jika basis timer 10 ms:

\[
Preset=\frac{PT_{ms}}{10}
\]

Contoh: delay 3 s dengan basis 10 ms menghasilkan `Preset = 300`.

### 3.8 Counter

Counter menghitung pulsa naik:

\[
CV(k)=CV(k-1)+1 \quad jika \quad rising\_edge(CU)=1
\]

Output counter aktif jika:

\[
Q=1 \quad jika \quad CV \ge PV
\]

### 3.9 Scaling Analog

Sensor analog sering menghasilkan 0-10 V atau 4-20 mA yang dikonversi ADC ke nilai digital.

Rumus scaling umum:

\[
Y=Y_{min}+\frac{X-X_{min}}{X_{max}-X_{min}}(Y_{max}-Y_{min})
\]

Contoh ADC 0-4095 untuk tekanan 0-10 bar:

\[
P=\frac{ADC}{4095}\times10
\]

Untuk sensor 4-20 mA dengan ADC yang sudah dikonversi ke arus:

\[
P=P_{min}+\frac{I-4}{20-4}(P_{max}-P_{min})
\]

### 3.10 Keamanan PLC

1. Output berbahaya harus menggunakan interlock fisik.
2. AI vision tidak boleh menjadi satu-satunya safety device.
3. Gunakan emergency stop hardwired.
4. Pisahkan jaringan praktikum dari jaringan produksi.
5. Validasi alamat coil sebelum write.
6. Hindari write register acak ke PLC produksi.

---

## 4. Dasar Jaringan Industri

### 4.1 IP Address

IP address IPv4 terdiri dari 32 bit, contoh `192.168.1.10`. Subnet mask menentukan bagian network dan host.

Rumus jumlah host:

\[
N_{host}=2^{(32-prefix)}-2
\]

Contoh `/24`:

\[
N_{host}=2^8-2=254
\]

### 4.2 Syarat Komunikasi TCP/IP

Perangkat dapat berkomunikasi jika:

1. IP berada pada subnet yang sama atau ada gateway.
2. Port layanan terbuka.
3. Firewall tidak memblokir.
4. Kabel/switch benar.
5. Tidak ada konflik IP.

### 4.3 Latency dan Throughput

Latency total kontrol:

\[
T_{total}=T_{camera}+T_{processing}+T_{ros}+T_{tcp}+T_{plc}
\]

Throughput data:

\[
Throughput=\frac{Data\;bits}{Time\;seconds}
\]

Frame rate kamera:

\[
FPS=\frac{N_{frame}}{T}
\]

Waktu antar-frame:

\[
T_{frame}=\frac{1}{FPS}
\]

Contoh 30 FPS:

\[
T_{frame}=33.3\;ms
\]

---

## 5. Dasar Modbus

### 5.1 Definisi Modbus

Modbus adalah protokol komunikasi industri master-slave/client-server. Modbus TCP berjalan di Ethernet menggunakan port standar 502. Client mengirim request, server/PLC membalas response.

### 5.2 Model Data Modbus

| Area | Nama | Akses | Ukuran | Contoh fungsi |
|---|---|---|---|---|
| 0xxxx | Coil | Read/Write | 1 bit | output/internal bit |
| 1xxxx | Discrete Input | Read only | 1 bit | input digital |
| 3xxxx | Input Register | Read only | 16 bit | sensor analog |
| 4xxxx | Holding Register | Read/Write | 16 bit | parameter/setpoint |

### 5.3 Function Code Penting

| Function code | Nama | Keterangan |
|---:|---|---|
| 01 | Read Coils | membaca coil bit |
| 02 | Read Discrete Inputs | membaca input bit |
| 03 | Read Holding Registers | membaca register R/W |
| 04 | Read Input Registers | membaca register read-only |
| 05 | Write Single Coil | menulis satu coil |
| 06 | Write Single Register | menulis satu register |
| 15 | Write Multiple Coils | menulis banyak coil |
| 16 | Write Multiple Registers | menulis banyak register |

### 5.4 Struktur Modbus TCP

Modbus TCP memiliki MBAP header dan PDU.

| Field | Ukuran | Fungsi |
|---|---:|---|
| Transaction ID | 2 byte | pasangan request-response |
| Protocol ID | 2 byte | selalu 0 untuk Modbus |
| Length | 2 byte | panjang unit ID + PDU |
| Unit ID | 1 byte | alamat slave/gateway |
| Function Code | 1 byte | jenis operasi |
| Data | n byte | alamat, nilai, jumlah data |

Panjang frame total:

\[
L_{total}=7+L_{PDU}
\]

### 5.5 Addressing

Banyak library Python menggunakan alamat berbasis 0. Jika dokumentasi PLC menulis coil 00001, maka alamat library sering menjadi 0.

Rumus konversi umum:

\[
Address_{library}=Address_{manual}-1
\]

Contoh coil 00005 pada manual menjadi address 4 pada program.

### 5.6 Representasi Coil

Coil hanya memiliki dua nilai:

\[
Coil \in \{0,1\}
\]

Write coil Modbus biasanya memakai nilai:

\[
ON=0xFF00
\]

\[
OFF=0x0000
\]

### 5.7 Register 16-bit

Holding register menyimpan angka 16-bit:

\[
0 \le Register \le 65535
\]

Signed 16-bit:

\[
-32768 \le x \le 32767
\]

Konversi unsigned ke signed:

\[
Signed=x-65536 \quad jika \quad x>32767
\]

### 5.8 Register 32-bit

Dua register 16-bit dapat membentuk 32-bit.

Big-endian word:

\[
Value_{32}=HighWord\times65536+LowWord
\]

Little-endian word:

\[
Value_{32}=LowWord\times65536+HighWord
\]

### 5.9 Float IEEE 754

Float 32-bit menggunakan 2 register. Nilai float:

\[
(-1)^s \times (1.fraction)_2 \times 2^{exponent-127}
\]

Byte order harus sesuai dengan PLC. Kesalahan endian menyebabkan nilai tidak masuk akal.

### 5.10 Exception Response

Jika terjadi error, function code response menjadi:

\[
FC_{error}=FC_{request}+0x80
\]

Contoh FC 03 error menjadi 0x83.

Kode exception umum:

| Kode | Nama | Makna |
|---:|---|---|
| 01 | Illegal Function | fungsi tidak didukung |
| 02 | Illegal Data Address | alamat salah |
| 03 | Illegal Data Value | nilai salah |
| 04 | Slave Device Failure | perangkat gagal |

---

## 6. Dasar Computer Vision

### 6.1 Citra Digital

Citra digital adalah matriks piksel.

Resolusi:

\[
N_{pixel}=Width\times Height
\]

Contoh 640x480:

\[
N_{pixel}=307200
\]

Citra RGB memiliki tiga kanal:

\[
Image(x,y)=[R,G,B]
\]

Ukuran data mentah RGB 8-bit:

\[
Size=Width\times Height\times3\;byte
\]

### 6.2 Grayscale

Konversi RGB ke grayscale:

\[
Y=0.299R+0.587G+0.114B
\]

### 6.3 Normalisasi Koordinat

MediaPipe menghasilkan koordinat landmark ternormalisasi 0 sampai 1.

Konversi ke piksel:

\[
x_{px}=x_{norm}\times Width
\]

\[
y_{px}=y_{norm}\times Height
\]

### 6.4 Jarak Euclidean

Jarak dua titik landmark:

\[
d=\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}
\]

Untuk 3D:

\[
d=\sqrt{(x_2-x_1)^2+(y_2-y_1)^2+(z_2-z_1)^2}
\]

### 6.5 Sudut Antar Vektor

Jika vektor:

\[
\vec{a}=P_1-P_0
\]

\[
\vec{b}=P_2-P_1
\]

Maka sudut:

\[
\theta=\cos^{-1}\left(\frac{\vec{a}\cdot\vec{b}}{|\vec{a}||\vec{b}|}\right)
\]

Dot product:

\[
\vec{a}\cdot\vec{b}=a_xb_x+a_yb_y+a_zb_z
\]

### 6.6 Akurasi Deteksi

Confusion matrix biner:

| | Prediksi 1 | Prediksi 0 |
|---|---:|---:|
| Aktual 1 | TP | FN |
| Aktual 0 | FP | TN |

Akurasi:

\[
Accuracy=\frac{TP+TN}{TP+TN+FP+FN}
\]

Presisi:

\[
Precision=\frac{TP}{TP+FP}
\]

Recall:

\[
Recall=\frac{TP}{TP+FN}
\]

F1-score:

\[
F1=2\times\frac{Precision\times Recall}{Precision+Recall}
\]

### 6.7 Smoothing Status Gestur

Untuk mengurangi noise, gunakan voting N frame:

\[
State=1 \quad jika \quad \sum_{i=1}^{N} state_i \ge Threshold
\]

Moving average:

\[
\bar{x}_k=\frac{1}{N}\sum_{i=0}^{N-1}x_{k-i}
\]

Exponential moving average:

\[
y_k=\alpha x_k+(1-\alpha)y_{k-1}
\]

Dengan `0 < alpha <= 1`.

---

## 7. MediaPipe Hands

MediaPipe Hands mendeteksi 21 landmark tangan.

| ID | Titik |
|---:|---|
| 0 | Wrist |
| 1-4 | Thumb CMC, MCP, IP, tip |
| 5-8 | Index MCP, PIP, DIP, tip |
| 9-12 | Middle MCP, PIP, DIP, tip |
| 13-16 | Ring MCP, PIP, DIP, tip |
| 17-20 | Pinky MCP, PIP, DIP, tip |

Deteksi jari terbuka sederhana:

1. Untuk telunjuk, tengah, manis, kelingking: tip lebih tinggi daripada PIP pada citra.
2. Untuk jempol: perbandingan posisi x tip dan IP tergantung tangan kiri/kanan.
3. Tambahkan threshold agar stabil.

Rumus threshold sederhana:

\[
FingerOpen=1 \quad jika \quad y_{tip}<y_{pip}-\epsilon
\]

Karena koordinat y citra semakin besar ke bawah.

---

## 8. Dasar ROS 2

### 8.1 Definisi ROS 2

ROS 2 adalah middleware robotika untuk membuat sistem berbasis node. Setiap node menjalankan fungsi tertentu dan dapat berkomunikasi melalui topic, service, action, parameter, dan transform.

### 8.2 Workspace dan Package

Struktur umum:

```text
workspace/
├── src/
│   └── package_name/
├── build/
├── install/
└── log/
```

Build package:

```bash
colcon build --packages-select mediapipe_modbus_pkg
```

Source environment:

```bash
source install/setup.bash
```

Run node:

```bash
ros2 run mediapipe_modbus_pkg mediapipe_modbus_node
```

### 8.3 Node

Node adalah proses komputasi. Pada modul ini node melakukan:

1. Membaca kamera.
2. Memproses citra dengan MediaPipe.
3. Menampilkan GUI.
4. Menulis data ke PLC via Modbus TCP.

### 8.4 Topic dan Rate

Jika node menerbitkan status jari 30 kali per detik:

\[
T=\frac{1}{30}=33.3\;ms
\]

Rate loop:

\[
Rate=\frac{1}{Period}
\]

### 8.5 QoS Dasar

QoS menentukan reliabilitas komunikasi:

1. **Reliable:** pesan diusahakan sampai.
2. **Best effort:** cepat, boleh hilang.
3. **Depth:** ukuran antrian pesan.
4. **Durability:** apakah pesan lama disimpan.

Untuk vision real-time biasanya best effort cukup, karena frame lama tidak berguna.

---

## 9. Integrasi AI-PLC

### 9.1 Prinsip Mapping

Status jari dinyatakan sebagai vektor:

\[
\mathbf{F}=[f_1,f_2,f_3,f_4,f_5]
\]

Dengan:

\[
f_i \in \{0,1\}
\]

Mapping ke coil:

\[
Coil_i=f_i
\]

Jumlah jari terbuka:

\[
N_{open}=\sum_{i=1}^{5} f_i
\]

Kode gestur 5-bit:

\[
G=f_1 2^0+f_2 2^1+f_3 2^2+f_4 2^3+f_5 2^4
\]

Rentang kode:

\[
0 \le G \le 31
\]

### 9.2 State Machine Gestur

Sistem dapat dibuat sebagai finite state machine:

```text
IDLE -> DETECT -> VALIDATE -> SEND_MODBUS -> PLC_ACTION -> MONITOR
```

Transisi validasi:

\[
Valid=1 \quad jika \quad Confidence \ge C_{min} \land StableFrames \ge N
\]

### 9.3 Latensi End-to-End

\[
T_{e2e}=T_{capture}+T_{preprocess}+T_{inference}+T_{gui}+T_{modbus}+T_{plc}
\]

Contoh:

| Komponen | Waktu |
|---|---:|
| Capture | 33 ms |
| Inference | 20 ms |
| GUI | 5 ms |
| Modbus | 10 ms |
| PLC scan | 10 ms |

\[
T_{e2e}=33+20+5+10+10=78\;ms
\]

### 9.4 Keandalan Sistem

Availability:

\[
A=\frac{MTBF}{MTBF+MTTR}
\]

Error rate komunikasi:

\[
ErrorRate=\frac{N_{error}}{N_{request}}\times100\%
\]

Packet success rate:

\[
SuccessRate=\frac{N_{success}}{N_{total}}\times100\%
\]

---

## 10. Praktik Analisis Sistem

### 10.1 Pengukuran FPS

Catat jumlah frame selama waktu tertentu:

\[
FPS=\frac{frame\_count}{elapsed\_time}
\]

### 10.2 Pengukuran Delay Manual

Gunakan rekaman video 60 FPS. Jika perubahan gestur terjadi pada frame 100 dan LED PLC menyala pada frame 108:

\[
Delay=\frac{108-100}{60}=0.133\;s
\]

### 10.3 Pengujian Akurasi Jari

Lakukan 20 percobaan per jari. Jika 18 benar:

\[
Accuracy=\frac{18}{20}\times100\%=90\%
\]

### 10.4 Pengujian Noise

Jika status jari salah 6 kali dari 300 frame:

\[
NoiseRate=\frac{6}{300}\times100\%=2\%
\]

---

## 11. Troubleshooting Konseptual

| Masalah | Penyebab | Solusi |
|---|---|---|
| Kamera tidak terbuka | device salah, permission, kamera dipakai aplikasi lain | cek `/dev/video*`, tutup aplikasi kamera lain |
| MediaPipe error | versi numpy/opencv tidak cocok | gunakan `numpy<2`, `opencv-python<4.10` |
| PLC tidak terkoneksi | IP salah, subnet beda, port tertutup | ping IP, cek port 502, cek kabel |
| Coil tidak berubah | alamat offset salah | cek base address 0/1 |
| LED salah urutan | mapping salah | atur mapping di GUI/YAML |
| Deteksi tangan tidak stabil | cahaya buruk, background kompleks | tambah pencahayaan dan threshold |
| Delay besar | FPS rendah, jaringan lambat | turunkan resolusi, optimasi loop |

---

## 12. Etika dan Keselamatan Praktikum

1. Jangan menghubungkan output PLC langsung ke aktuator berbahaya tanpa pengaman.
2. Gunakan simulator, LED, atau beban rendah saat belajar.
3. Jangan mengubah IP perangkat kelompok lain.
4. Jangan melakukan scan jaringan di luar jaringan laboratorium tanpa izin.
5. Dokumentasikan semua perubahan konfigurasi.
6. Laporkan kegagalan perangkat kepada dosen/asisten.

---

## 13. Topik Advanced

### 13.1 Debounce Digital

Debounce memastikan sinyal stabil selama waktu tertentu.

\[
Stable=1 \quad jika \quad input(t)=input(t-\Delta t) \text{ selama } T_d
\]

### 13.2 Watchdog

Watchdog mematikan output jika tidak ada update dalam batas waktu.

\[
Fault=1 \quad jika \quad t_{now}-t_{last\_packet}>T_{watchdog}
\]

### 13.3 Heartbeat

Heartbeat toggling:

\[
HB_k=\overline{HB_{k-1}}
\]

PLC memantau perubahan heartbeat. Jika berhenti berubah, komunikasi dianggap gagal.

### 13.4 Batch Write Modbus

Daripada menulis 5 coil satu per satu, gunakan write multiple coils:

\[
N_{request}=1
\]

Untuk single write:

\[
N_{request}=5
\]

Efisiensi request:

\[
Efficiency=\frac{5-1}{5}\times100\%=80\%
\]

### 13.5 Gesture Command Encoding

Dengan 5 jari, jumlah kombinasi gestur:

\[
N=2^5=32
\]

Jika ditambah tangan kiri/kanan:

\[
N=2^5\times2=64
\]

Jika memakai dua tangan:

\[
N=2^{10}=1024
\]

### 13.6 Fail-Safe Design

Prinsip fail-safe:

\[
Output=Command \land CommunicationOK \land SafetyOK
\]

Jika komunikasi gagal:

\[
CommunicationOK=0 \Rightarrow Output=0
\]

---

## 14. Rekomendasi Buku

1. John W. Webb dan Ronald A. Reis, **Programmable Logic Controllers: Principles and Applications**.
2. Frank D. Petruzella, **Programmable Logic Controllers**.
3. Hugh Jack, **Automating Manufacturing Systems with PLCs**.
4. Gary Dunning, **Introduction to Programmable Logic Controllers**.
5. William Bolton, **Programmable Logic Controllers**.
6. Karl Johan Åström dan Richard M. Murray, **Feedback Systems: An Introduction for Scientists and Engineers**.
7. Richard Szeliski, **Computer Vision: Algorithms and Applications**.
8. Adrian Kaehler dan Gary Bradski, **Learning OpenCV**.
9. Morgan Quigley, Brian Gerkey, dan William D. Smart, **Programming Robots with ROS**.
10. Steve Heath, **Embedded Systems Design**.
11. Tanenbaum dan Wetherall, **Computer Networks**.
12. Modicon/Schneider Electric, **Modbus Protocol Reference Guide**.

---

## 15. Rekomendasi Paper dan Referensi Teknis

1. Modbus Organization, **MODBUS Application Protocol Specification V1.1b3**.
2. Modbus Organization, **MODBUS Messaging on TCP/IP Implementation Guide V1.0b**.
3. Lugaresi et al., **MediaPipe: A Framework for Building Perception Pipelines**, Google Research.
4. Zhang et al., **MediaPipe Hands: On-device Real-time Hand Tracking**.
5. Quigley et al., **ROS: an open-source Robot Operating System**.
6. Macenski et al., **Robot Operating System 2: Design, Architecture, and Uses in the Wild**.
7. IEEE papers on industrial Ethernet latency and reliability.
8. Papers on human-machine interaction using hand gesture recognition.
9. Papers on vision-based control for collaborative robotics.
10. Papers on PLC cybersecurity and industrial network segmentation.

---

## 16. Ringkasan

Modul ini menggabungkan PLC, Modbus TCP, AI vision, ROS 2, dan GUI Python. Inti praktikum adalah memahami bagaimana data visual diubah menjadi bit kontrol industri. Praktikan harus menguasai konsep scan cycle, addressing Modbus, landmark tangan, mapping data, validasi, latency, troubleshooting, dan keselamatan sistem otomasi.
