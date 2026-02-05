# Panduan Implementasi Program
**Praktikum Mekatronika dan Robotika**

---

## 📋 Panduan Umum untuk Instruktur

### 1. Persiapan Awal (Pre-Program)

#### Hardware Audit
- [ ] Verifikasi ketersediaan semua komponen (lihat BOM di Modul 1-14)
- [ ] Test semua unit berfungsi (power supply, motor, sensor, microcontroller)
- [ ] Service/maintenance alat yang sudah lama tidak digunakan
- [ ] Stock consumables (solder, flux, jumper wire, kabel)
- [ ] Dokumentasikan serial number & kondisi awal setiap unit

#### Software Setup
- [ ] Install tools di lab PC:
  - PlatformIO IDE (VS Code extension)
  - Fusion 360 (CAD)
  - EasyEDA (PCB design)
  - ROS2 Humble (Ubuntu 22.04)
  - Python 3.10+, Node.js 18+
  - MQTT Broker (Mosquitto)
  - Database (InfluxDB/Timescale)
  - Grafana/ThingsBoard
  - Git & GitHub

- [ ] Setup lab environment:
  - GitHub Classroom atau private Git server
  - Project repository template
  - CI/CD untuk code check (optional)

#### Documentation
- [ ] Siapkan troubleshooting guide untuk setiap modul
- [ ] Buat demo video referensi (untuk modul 1-7)
- [ ] Dokumentasikan lab setup (network, equipment location, borrow policy)

---

### 2. Persiapan per Modul

#### Sebelum Praktikum (1-2 minggu sebelumnya)
1. **Review materi & jobsheet**
   - Validasi instruksi jelas & executable
   - Verifikasi semua code compile & run tanpa error
   - Test hardware dengan code yang diberikan
   - Identifikasi potential issue

2. **Siapkan hardware**
   - Reset/clean semua perangkat
   - Organize komponen sesuai jobsheet
   - Siapkan workstation (breadboard, jumper, tools)
   - Label & organize sesuai kelompok (jika ada)

3. **Siapkan rubrik & penilaian**
   - Review rubrik sesuai project/video
   - Siapkan template feedback
   - Setup grading sheet

#### Saat Praktikum
1. **Briefing (15-20 menit)**
   - Review tujuan praktikum
   - Tunjukkan expected result
   - Tunjukkan common mistake (dari pengalaman)
   - Tunjukkan safety procedure

2. **Hands-on Support**
   - Monitor progress setiap group
   - Jangan langsung kasi answer, tanya guiding question
   - Document issue yang muncul (untuk improvement)
   - Ambil foto/video progress

3. **Reflection (10-15 menit)**
   - Diskusi hasil & observation
   - Tanya ke students: "apa yang learned? apa yang challenging?"
   - Preview untuk modul berikutnya

---

### 3. Sistem Penilaian

#### Grading Policy
- **Kehadiran & participation:** 10%
- **Praktikum (jobsheet):** 30%
  - Data & observation: 10%
  - Analysis & conclusion: 10%
  - Code quality & documentation: 10%
- **Project (jika ada):** 30%
  - Design & fabrication: 15%
  - Code & functionality: 15%
- **Video presentation (per modul):** 20%
  - Teori explanation: 8%
  - Demo & analysis: 8%
  - Teknis video quality: 4%
- **Bonus:** +5% max (creativity, extra features)

#### Feedback Loop
- Return graded work dalam 1 minggu
- Highlight strength & area for improvement
- Suggest specific action untuk next submission
- One-on-one meeting jika grade kurang

---

### 4. Komunikasi & Support

#### Setup Channel Komunikasi
- **Synchronous:** Lab session (weekly atau per 2 minggu)
- **Asynchronous:** 
  - Email/LMS untuk announcement
  - Discord/Slack untuk Q&A (set response time: dalam 24 jam)
  - GitHub issue untuk technical question

#### Office Hour
- Set regular time (e.g., Rabu 14:00-16:00)
- Encourage students datang dengan specific question
- Document FAQ dari office hour

#### Community Support
- Encourage peer learning (study group, buddy system)
- Nominate top students sebagai "lab assistant"
- Share knowledge (presentation dari students untuk students)

---

## 📚 Panduan Umum untuk Mahasiswa

### 1. Sikap Belajar

#### Pre-Class Preparation
- ✅ **Baca materi** SEBELUM praktikum (jangan on-the-spot learning)
  - Baca Materi.md: pahami konsep utama
  - Baca Jobsheet.md: apa yang akan dilakukan
  - Identify bagian yang kurang mengerti
- ✅ **Siapkan pertanyaan** untuk asistensi
- ✅ **Verifikasi software** sudah install & working
- ✅ **Diskusi dalam kelompok** tentang approach & plan

#### During Practical
- ✅ **Teliti saat setup** hardware (jangan ambil shortcut)
- ✅ **Follow procedure** di jobsheet step-by-step
- ✅ **Dokumentasikan progress** (foto, video, note)
- ✅ **Troubleshoot methodically:**
  - Identify symptom (apa yang tidak normal?)
  - Check obvious things dulu (power, kabel, config)
  - Test subsystem independent (isolate problem)
  - Consult reference (datasheet, documentation)
  - Ask instructor jika stuck > 30 menit
- ✅ **Collaborate efficiently** di kelompok (bagi task, communication jelas)

#### Post-Class
- ✅ **Analisis data** menggunakan rubrik di jobsheet
- ✅ **Dokumentasikan learning** dalam laporan
- ✅ **Backup code & data** dengan Git
- ✅ **Prepare untuk next modul** (read materi, install software)

### 2. Troubleshooting Mindset

**Methodology:**
1. **Understand the system** → Blok diagram, datasheet, code logic
2. **Identify the problem** → Symptom, scope (hardware/software/interface)
3. **Generate hypothesis** → Apa yang mungkin cause issue?
4. **Test hypothesis** → Change one variable, observe result
5. **Validate solution** → Confirm fix work, understand root cause

**Tools & Resources:**
- Serial monitor / oscilloscope untuk inspect signal
- Datasheet untuk reference spec & pin
- Documentation untuk understand interface
- Git history untuk understand code change
- Community (Stack Overflow, GitHub issue, forum)

### 3. Collaboration Best Practice

#### Team Organization
- **Define role:** Hardware lead, software lead, documentation lead
- **Regular standup:** Daily 5-10 min sync (progress, blocker, plan)
- **Clear ownership:** Setiap task jelas siapa responsible
- **Knowledge sharing:** Share learning dengan tim, jangan silo knowledge

#### Code Collaboration
- **Use Git:** Bukan share file via WhatsApp/email
- **Branch per feature:** Jangan semua orang di main branch
- **Pull request:** Review code sebelum merge (bahkan dalam team)
- **Commit message:** Jelas dan descriptive

### 4. Documentation Best Practice

#### During Development
- **Note-taking:** Capture design decision, issue encountered, solution
- **Screenshot/video:** Visual documentation saat develop
- **Code comment:** Inline comment untuk logic kompleks

#### At Project End
- **README.md:** Quick start, dependency, how to run
- **API doc:** Endpoint, parameter, response (jika ada API)
- **Design doc:** Architecture, design decision, trade-off
- **Test report:** Test procedure, result, analysis

### 5. Version Control (Git) Best Practice

#### Workflow
```bash
# Clone repo
git clone <repo>

# Create feature branch
git checkout -b feature/add-pid-controller

# Make change, commit frequently
git add .
git commit -m "[Feature] Implement PID tuning"

# Push & create pull request
git push origin feature/add-pid-controller

# (On GitHub) Create PR, request review
# (After review) Merge ke develop branch

# (Before submission) Merge develop → main
git checkout main
git merge develop
git tag v1.0-final
git push --tags
```

#### Commit Message Convention
- `[Feature] Add new feature`
- `[Fix] Fix bug description`
- `[Docs] Update documentation`
- `[Refactor] Code cleanup`
- `[Test] Add test case`

---

## 🚀 Setup Guide untuk Setiap Tool

### PlatformIO (Embedded Development)

#### Install
```bash
# VS Code → Extensions → search "PlatformIO" → Install

# Verify installation
pio --version
```

#### New Project
```bash
# Via VS Code: PlatformIO → New Project → pilih board & framework
# Atau via CLI:
pio project init --board esp32 --framework arduino
```

#### Compile & Upload
```bash
# Compile
pio run

# Upload ke board
pio run -t upload

# Monitor serial output
pio device monitor --baud 115200
```

#### Troubleshoot
- **Board not detected:** Check USB driver, device manager (Windows)
- **Compile error:** Check library version di platformio.ini
- **Upload fail:** Check board selection, COM port, baud rate

---

### ROS2 Humble (Ubuntu 22.04)

#### Install
```bash
# Add ROS repository
curl -sSL https://raw.githubusercontent.com/ros/rosdep/master/rosdep.sh | sudo bash /dev/stdin

# Install ROS2
sudo apt update
sudo apt install ros-humble-desktop-full

# Setup environment
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc

# Verify
ros2 --version
```

#### Create ROS2 Package
```bash
# Create workspace
mkdir -p ~/robot_ws/src
cd ~/robot_ws/src

# Create package (Python)
ros2 pkg create my_robot_pkg --build-type ament_python

# Or (C++)
ros2 pkg create my_robot_pkg --build-type ament_cmake
```

#### Build & Run
```bash
# From workspace root
colcon build

# Source setup
source install/setup.bash

# Run node
ros2 run my_robot_pkg my_node

# Monitor topic
ros2 topic list
ros2 topic echo /topic_name

# Inspect service
ros2 service list
ros2 service call /service_name srv_type "{...}"
```

---

### Fusion 360 (CAD Design)

#### Install
- Download dari autodesk.com (free untuk student/startup)
- Cloud-based, tidak perlu install

#### Basic Workflow
1. **Sketch:** 2D design (rectangle, circle, line)
2. **Extrude:** 3D dari sketch (pad/pocket)
3. **Assembly:** Combine parts, add constraint
4. **Motion Study:** Simulate mekanik gerak
5. **Export:** STL (untuk 3D print) atau STEP (untuk manufaktur)

#### Modeling Best Practice
- Use constraint (jangan dimension random)
- Parametric design (change dimension, model update)
- Organize part: rename feature, use folder
- Use standard library (bearing, motor, dll)

---

### EasyEDA (PCB Design)

#### Install
- Web-based, buka eda.jlcpcb.com atau easyeda.com

#### Basic Workflow
1. **Schematic:** Connect component, net naming
2. **PCB layout:** Placement, routing, constraint
3. **Design rule check:** Verify aturan manufaktur
4. **Export:** Gerber untuk manufacture

#### Design Best Practice
- Check design rule dengan fab manufacturer
- DRC (Design Rule Check) no error sebelum export
- Copper width sesuai arus (1oz copper ≈ 0.5A/mm)
- Via: enough untuk thermal via, spacing adequate

---

### Git & GitHub

#### Setup
```bash
# Install git (Ubuntu)
sudo apt install git

# Configure
git config --global user.name "Your Name"
git config --global user.email "your@email.com"

# Generate SSH key (untuk secure push)
ssh-keygen -t ed25519 -C "your@email.com"
cat ~/.ssh/id_ed25519.pub
# Copy & paste di GitHub → Settings → SSH keys
```

#### Workflow
```bash
# Clone repo
git clone git@github.com:username/repo.git
cd repo

# Create & switch branch
git checkout -b feature/my-feature

# Make change
# ... edit file ...

# Stage & commit
git add .
git commit -m "[Feature] Description of change"

# Push to GitHub
git push origin feature/my-feature

# Create Pull Request di GitHub web
# ... request review, discuss, merge ...
```

---

## 🎓 Rekomendasi Timeline

### Semester (16 minggu)

| Minggu | Aktivitas |
|--------|-----------|
| 1-2 | Modul 1-2 (Arsitektur, Safety) |
| 3-4 | Modul 3-4 (Power, Sensor) |
| 5 | Modul 5 (Motor & Drive) |
| 6-7 | Modul 6 (Motion Control PID) |
| 8 | Midterm / Catch-up |
| 9 | Modul 7 (Mekanik) |
| 10 | Modul 8 (Kinematika) |
| 11 | Modul 9 (Trajectory Planning) |
| 12 | Modul 10-11 (ROS2 & Bridge) |
| 13 | Modul 12 (State Machine) |
| 14 | Modul 13-14 (IoT & Monitoring) |
| 15-16 | Capstone Project (intensive) |

### Workshop / Short Course (5 minggu)

| Minggu | Aktivitas |
|--------|-----------|
| 1 | Modul 1-2 (Overview & Safety) |
| 2 | Modul 5-6 (Motor Control) |
| 3 | Modul 8-9 (Kinematics & Trajectory) |
| 4 | Modul 10-11-12 (ROS2 Integration) |
| 5 | Capstone Project Demo |

---

## ✅ Checklist Implementasi Program

### Pre-Program
- [ ] Furniture & workspace (meja, kursi, lemari)
- [ ] Hardware stock & audit
- [ ] Software install & test
- [ ] Documentation siap
- [ ] Instruktur training / siap

### Per Modul
- [ ] Materi.md siap
- [ ] Jobsheet.md tested & executable
- [ ] Project.md jelas & challenging
- [ ] Referensi link verified
- [ ] Rubrik penilaian jelas
- [ ] Demo code compiled & running
- [ ] Hardware setup tested
- [ ] Troubleshooting guide prepared

### End of Program
- [ ] Semua deliverable terkumpul & dinilai
- [ ] Feedback diberikan ke students
- [ ] Repository archived
- [ ] Program evaluation (apa yang berhasil, apa yang perlu improvement)
- [ ] Documentation updated untuk next batch

---

**Last Updated:** February 2026  
**Version:** 1.0
