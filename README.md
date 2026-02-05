# Program Praktikum Mekatronika dan Robotika
**End-to-End Robotics/IIoT System**

## 📋 Daftar Isi
- [Ringkasan Program](#ringkasan-program)
- [Arsitektur Sistem](#arsitektur-sistem)
- [Tools dan Platform](#tools-dan-platform)
- [Struktur Kurikulum](#struktur-kurikulum)
- [Format Deliverable](#format-deliverable)
- [Panduan Umum](#panduan-umum)

---

## 🎯 Ringkasan Program

Program praktikum ini dirancang untuk mengajarkan sistem mekatronika dan robotika secara **end-to-end**, dari desain mekanik, elektronik, embedded control, hingga IoT monitoring. Peserta akan belajar merancang dan mengimplementasikan sistem robotika terintegrasi yang layak industri.

**Target Peserta:** Mahasiswa teknik, robotika, otomasi industri (level sarjana/diploma akhir)

**Durasi:** 14 modul + 2-4 minggu capstone project (total ~16-18 minggu)

**Prinsip Pembelajaran:**
- ✅ Hands-on praktikum dengan hardware real
- ✅ Teori → Implementasi → Troubleshooting
- ✅ Integration across multiple domains (mekanik, elektronik, software)
- ✅ Industry-ready practices

---

## 🏗️ Arsitektur Sistem

Program menggunakan **5-layer architecture** untuk sistem robotika/IIoT:

### Layer A — Plant/Robot (Mekanik + Elektronik)
- **Mekanik:** Chassis AGV, arm 3-DOF, conveyor, transmisi (belt/gear/screw)
- **Elektronik:** Power tree (24V/12V/5V), motor driver, proteksi (fuse/TVS), E-Stop & interlock

### Layer B — Real-time Control (Embedded)
- **Mikrokontroler:** ESP32 untuk loop kontrol cepat
- **Fungsi:** Pembacaan sensor, PWM/pulse, PID kontrol, watchdog, fault handling
- **Komunikasi:** MQTT / Modbus RTU / CAN

### Layer C — Edge Compute (Ubuntu + ROS2 Humble)
- **Framework:** ROS2 sebagai "otak sistem"
- **Fungsi:** State machine, path planning, sensor fusion, TF/URDF
- **Bridge:** micro-ROS atau MQTT bridge ke perangkat

### Layer D — Otomasi Industri (PLC + SCADA) - Optional
- **PLC:** Sequencing, safety logic, interlock
- **SCADA:** Monitoring, alarm, trend, historian

### Layer E — Webserver IoT (Monitoring Jarak Jauh)
- **API:** REST + WebSocket
- **Database:** InfluxDB/Timescale + Grafana/ThingsBoard
- **Frontend:** HTML/CSS/JavaScript

---

## 🛠️ Tools dan Platform

| Domain | Tools | Purpose |
|--------|-------|---------|
| **Mekanik** | Fusion 360 | Desain 3D, simulasi mekanisme, analisis FEA |
| **Elektronik** | EasyEDA | Schematic & PCB design |
| **Embedded (PlatformIO)** | Arduino Mega 2560, ESP32, STM32F4 | Real-time control, sensor/actuator interfacing |
| **PC - Robotika** | ROS2 Humble (Ubuntu 22.04) | Framework robotika, node communication, TF |
| **PC - Data** | Python, Node.js | Scripting, data processing, AI/ML, OpenCV |
| **PC - Komunikasi** | MQTT Mosquitto | IoT message broker |
| **PC - Database** | InfluxDB, PostgreSQL/Timescale | Time-series dan relational data |
| **PC - Monitoring** | Grafana, ThingsBoard | Dashboard dan visualization |
| **Kontrol** | PID, Fuzzy Logic, State Machine | Algoritma kontrol |

---

## 📚 Struktur Kurikulum

Program terdiri dari **14 modul + 1 capstone project** yang tersusun progresif:

### Modul Fundamentals (1-4)
- **Modul 1:** Arsitektur Sistem Mekatronika & Robotika
- **Modul 2:** Safety & Commissioning Sistem
- **Modul 3:** Power & EMC Praktis
- **Modul 4:** Sensor Fusion & Conditioning

### Modul Hardware Control (5-7)
- **Modul 5:** Motor & Drive System
- **Modul 6:** Motion Control di ESP32 (PID)
- **Modul 7:** Mekanisme Robot & Transmisi

### Modul Robotika (8-9)
- **Modul 8:** Kinematika Robot (FK/IK)
- **Modul 9:** Trajectory Planning & Motion Control

### Modul Software Integration (10-12)
- **Modul 10:** ROS2 Fundamental untuk Robot
- **Modul 11:** ESP32 ↔ ROS2 Bridge (Komunikasi IoT)
- **Modul 12:** State Machine & Sequencing (ROS2)

### Modul IoT & Advanced (13-14)
- **Modul 13:** Webserver IoT & Remote Monitoring
- **Modul 14:** Observability & Data Engineering

### Capstone Project (2-4 Minggu)
Pilihan tema:
- **A. AGV Mini:** Navigasi antar station + docking + web monitoring
- **B. Pick & Place 3-DOF:** Robot arm + conveyor + sorting + sequencing
- **C. Mini Production Cell:** Inspection + reject gate + historian + fault recovery

**Persyaratan Capstone:**
✓ Desain mekanik (Fusion 360) + PCB (EasyEDA)  
✓ Embedded control (ESP32/STM32) dengan PID/fuzzy  
✓ Edge computing (ROS2 di Ubuntu)  
✓ Web server IoT untuk monitoring & control  
✓ Kinematika (untuk arm/AGV)  
✓ Safety system & fault handling  
✓ Data logging & visualization

---

## 📦 Format Deliverable

Setiap modul menghasilkan deliverable berikut:

```
Modul_XX_NamaModul/
├── MATERI.md                    # Materi pembelajaran teori
├── JOBSHEET.md                  # Panduan percobaan praktis
├── PROJECT.md                   # Proyek aplikatif (open-ended)
├── REFERENSI.md                 # Daftar pustaka (optional)
├── Rubrik_Penilaian_Project.md  # Rubrik scoring project (optional)
├── Rubrik_Penilaian_Video.md    # Rubrik scoring video (optional)
│
├── Praktikum/
│   ├── Desain_CAD/              # File Fusion 360, STL, STEP
│   ├── Desain_PCB/              # File EasyEDA, Gerber
│   ├── Code_Embedded/           # PlatformIO project
│   │   ├── arduino_mega/
│   │   ├── esp32/
│   │   └── stm32/
│   ├── Code_PC/                 # ROS2, Python, Node.js
│   │   ├── ros2_ws/
│   │   ├── python_scripts/
│   │   └── webserver_iot/
│   ├── output/                  # Hasil test, plot, data, video
│   └── README.md                # Cara menjalankan semua program
│
├── Data/
│   ├── images/                  # Foto komponen, hasil percobaan
│   ├── videos/                  # Demo sistem, tutorial
│   ├── sensor_data/             # Log sensor (CSV, JSON)
│   └── test_results/            # Grafik, analisis
│
├── requirements.txt             # Python packages
├── platformio.ini               # Embedded config
└── package.json                 # Node.js dependencies
```

---

## 🎓 Panduan Umum

### Untuk Instruktur
- ✅ Test semua hardware & code sebelum praktikum
- ✅ Siapkan troubleshooting guide untuk masalah umum
- ✅ Buat demo video sebagai referensi
- ✅ Setup GitHub Classroom untuk submission
- ✅ Siapkan feedback template untuk review

### Untuk Mahasiswa
- ✅ Baca materi **SEBELUM** praktikum
- ✅ Siapkan pertanyaan untuk asistensi
- ✅ Dokumentasikan progress (foto, video, log) sejak awal
- ✅ Backup code & data dengan Git
- ✅ Kolaborasi tim: bagi tugas sesuai expertise

### Prinsip Coding
- **Modular:** Pisahkan fungsi (sensor, motor, control, communication)
- **Readable:** Variable naming jelas, comment cukup
- **Testable:** Masing-masing fungsi bisa ditest independent
- **Portable:** Bisa jalan di Windows/Linux/Mac
- **Configurable:** Parameter di file config, jangan hardcode

### Version Control (Git)
- Commit message jelas: `[Modul-XX] Add PID controller implementation`
- Branch strategy: `main` (stable), `dev` (development), `feature/*` (features)
- Pull request untuk merge ke main (code review)
- Tag untuk milestone: `v1.0-module06-completed`

---

## 📖 Lihat Juga

- [Index Modul Lengkap](INDEX_MODUL.md) — Deskripsi detail setiap modul
- [Referensi Buku](REFERENSI.md) — Kurasi buku untuk setiap topik
- [Rubrik Penilaian](RUBRIK_PENILAIAN.md) — Kriteria scoring project & video

---

**Last Updated:** February 2026  
**Version:** 1.0
