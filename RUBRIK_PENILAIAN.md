# Rubrik Penilaian
**Praktikum Mekatronika dan Robotika**

---

## 📊 Rubrik Penilaian Project

Rubrik ini digunakan untuk menilai project/capstone dari mahasiswa.

### Komponen Penilaian

#### 1. DESAIN MEKANIK (Fusion 360) — 15%

| Skor | Deskripsi |
|------|-----------|
| **91-100** | ✅ Assembly lengkap dengan constraint yang tepat, motion study menunjukkan mekanisme berfungsi, engineering drawing lengkap (dimensi, toleransi, GD&T), analisis workspace/DOF/singularity (jika applicable), foto hasil assembly fisik profesional, material selection justified |
| **81-90** | ✅ Assembly lengkap dengan constraint, motion study ada, drawing dengan dimensi dasar, analisis singkat workspace, foto assembly tersedia, material selection reasonable |
| **71-80** | ⚠️ Assembly ada tapi beberapa constraint kurang, motion study basic, drawing tidak lengkap (toleransi kurang), analisis minimal, foto assembly ada |
| **61-70** | ⚠️ Assembly ada tapi banyak constraint yang salah, motion study tidak akurat, drawing sederhana, tidak ada analisis, foto kurang jelas |
| **< 60** | ❌ Assembly incomplete atau tidak akurat, motion study tidak ada, drawing minimal, foto tidak ada |

**Kriteria Detail:**
- ✓ Constraint type yang tepat (coincident, parallel, perpendicular, distance, dll)
- ✓ Assembly bebas redundant constraint
- ✓ Motion study smooth dan sesuai expected behavior
- ✓ Drawing mengikuti ISO/ANSI standard
- ✓ Toleransi realistic (bukan 0.001mm untuk part plastic)
- ✓ BOM (Bill of Materials) lengkap dengan part number
- ✓ Foto assembly real: jelas, dari multiple angle, pencahayaan baik

---

#### 2. DESAIN ELEKTRONIK (EasyEDA) — 15%

| Skor | Deskripsi |
|------|-----------|
| **91-100** | ✅ Schematic lengkap & jelas, semua komponen properly rated, decoupling capacitor ada, protection device lengkap (fuse, TVS, dioda), PCB layout optimal (power/signal separation, trace width sesuai arus, via placement yang tepat), grounding strategy yang baik, Gerber file ready untuk manufacture, foto implementasi PCB & wiring profesional |
| **81-90** | ✅ Schematic lengkap, komponen mostly proper rated, decoupling ada, protection ada, PCB layout reasonable, grounding baik, foto implementasi tersedia |
| **71-80** | ⚠️ Schematic ada tapi beberapa connection tidak optimal, komponen rating marginal, protection kurang, PCB layout basic, grounding ada, foto implementasi sederhana |
| **61-70** | ⚠️ Schematic incomplete, komponen not properly rated, protection minimal, PCB layout poor, grounding tidak jelas, foto implementasi kurang clear |
| **< 60** | ❌ Schematic tidak jelas, komponen tidak rated, tidak ada protection, PCB layout tidak ada/poor, foto tidak ada |

**Kriteria Detail:**
- ✓ Semua pin terkoneksi dengan tepat (check dengan datasheet)
- ✓ Tegangan supply dekoupling setiap IC (0.1µF dekat IC, bulk kapasitor dekat supply)
- ✓ Protection: fuse untuk power input, TVS/diode untuk reverse polarity, motor flyback diode
- ✓ Trace width dihitung berdasarkan arus (online tool: 1A ~ 0.4mm copper)
- ✓ Via size appropriate untuk layer
- ✓ Power plane & ground plane (jika multilayer)
- ✓ Impedance matching untuk high-speed signal (jika applicable)
- ✓ EMI shielding (ground plane, ferrite bead untuk noisy signal)
- ✓ Foto: close-up schematic screenshot, PCB layout screenshot, foto soldered PCB dari multiple angle
- ✓ BOM dengan part number, value, footprint, quantity

---

#### 3. SOFTWARE EMBEDDED (PlatformIO) — 20%

| Skor | Deskripsi |
|------|-----------|
| **91-100** | ✅ Code modular (sensor, motor, control, comm separasi jelas), readable (meaningful variable name, good comment), algoritma kontrol tepat (PID/fuzzy implementasi correct), safety system robust (E-Stop, watchdog, fault handling), unit test present, code compile tanpa warning, performance efficient |
| **81-90** | ✅ Code modular & readable, algoritma kontrol correct, safety system ada, testing ada, compile clean, performance baik |
| **71-80** | ⚠️ Code mostly modular, readable tapi ada dead code, algoritma basic tapi working, safety system partial, testing minimal, compile ada minor warning |
| **61-70** | ⚠️ Code kurang modular, readability poor, algoritma ada masalah, safety system incomplete, testing tidak ada, compile ada warning significant |
| **< 60** | ❌ Code tidak modular, tidak readable, algoritma salah, safety tidak ada, compile error/warning banyak |

**Kriteria Detail:**
- ✓ Folder structure: src/ (main.cpp, config.h, sensor.h/cpp, motor.h/cpp, pid.h/cpp, comm.h/cpp, utils.h/cpp)
- ✓ config.h: semua pin definition, PID parameter, constant - mudah diubah
- ✓ Variable naming: camelCase (motor_speed_rpm) atau snake_case (motor_speed_rpm), jangan single letter (i, x)
- ✓ Comment: function comment (input, output), logic comment (jangan obvious), no excessive comment
- ✓ PID: proportional/integral/derivative correct, anti-windup implementation, output saturation
- ✓ Sensor reading: filtering (moving average atau low-pass), debouncing
- ✓ Motor control: PWM frequency appropriate untuk motor type, direction control, current limiting
- ✓ Communication: error handling (timeout, retry), correct message format, proper baudrate/CAN bitrate
- ✓ Safety: E-Stop monitoring continuous, watchdog enabled, fault state handling (safe state), recovery procedure
- ✓ Unit test: test sensor reading, test motor control, test PID dengan mock
- ✓ Compile: `pio run` tanpa error (warning acceptable jika necessary)
- ✓ Code review: logic trace step-by-step correct

---

#### 4. SOFTWARE PC (ROS2, Web IoT) — 15%

| Skor | Deskripsi |
|------|-----------|
| **91-100** | ✅ ROS2 node architecture well-designed (separate node untuk tiap subsystem), topic & service design semantic, URDF model lengkap & accurate, TF hierarchy correct, web dashboard functional & responsive (mobile-friendly), backend API RESTful, database query efficient, data logging comprehensive |
| **81-90** | ✅ ROS2 nodes functional, topic/service reasonable design, URDF ada, TF mostly correct, dashboard functional, API working, database basic, logging ada |
| **71-80** | ⚠️ ROS2 nodes ada tapi architecture kurang optimal, URDF basic, TF ada error minor, dashboard functional tapi UI basic, API functional, database simple, logging partial |
| **61-70** | ⚠️ ROS2 nodes minimal, URDF incomplete, TF ada issue, dashboard kurang functional, API incomplete, database not used, logging kurang |
| **< 60** | ❌ ROS2 struktur tidak jelas, URDF tidak ada/salah, TF broken, dashboard tidak functional, API missing, database tidak ada |

**Kriteria Detail:**

**ROS2:**
- ✓ Node per subsystem: sensor_node, motor_node, control_node, bridge_node (jika MQTT)
- ✓ Topic naming: /robot_name/subsystem/signal (e.g., /robot/motor/speed_rpm)
- ✓ Message type appropriate (use sensor_msgs, geometry_msgs dari official)
- ✓ Service untuk command (SetMotorSpeed, EnableMotor, etc)
- ✓ Parameter server untuk configuration
- ✓ Launch file: multiple launch untuk different scenario (sim, real, test)
- ✓ Logging: rclpy.logging atau rclcpp logging
- ✓ Node lifecycle management (startup, shutdown sequence)

**URDF:**
- ✓ Base link & end link definition
- ✓ Joint type correct (revolute, prismatic, fixed)
- ✓ Link mass & inertia approximate (bukan semua 1kg)
- ✓ Visual & collision geometry defined
- ✓ Transmission definition (jika ada actuator)
- ✓ Spawn di gazebo (optional simulation)

**Web Dashboard:**
- ✓ Responsive design (works di mobile)
- ✓ Real-time data update (WebSocket tidak polling)
- ✓ Command buttons (motor start/stop, setpoint change)
- ✓ Graph display untuk telemetry data
- ✓ User-friendly (jelas input, output, status)
- ✓ Error handling (show error message jika command fail)

**Backend API:**
- ✓ RESTful endpoint: GET /api/robot/status, POST /api/motor/speed, etc
- ✓ Authentication basic (username/password atau token)
- ✓ Request/response documentation (atau use Swagger)
- ✓ Error response proper (HTTP status code correct)

**Database:**
- ✓ Schema design normalized
- ✓ Index pada frequently queried field
- ✓ Query efficient (not N+1 query)
- ✓ Backup strategy (jika production)

**Data Logging:**
- ✓ Sensor data logged dengan timestamp
- ✓ Command & response logged
- ✓ Error/fault logged dengan severity level
- ✓ Log format structured (CSV, JSON, atau database row)

---

#### 5. INTEGRASI SISTEM — 15%

| Skor | Deskripsi |
|------|-----------|
| **91-100** | ✅ Semua layer terintegrasi seamless (embedded-ROS2-web), komunikasi reliable (error handling, retry logic), latency acceptable (< 100ms untuk critical command), fault recovery robust (graceful degradation, automatic restart), system stability tested (stress test, long-run test) |
| **81-90** | ✅ Integrasi mostly seamless, komunikasi reliable, latency acceptable, fault recovery ada, stability tested |
| **71-80** | ⚠️ Integrasi functional tapi ada issue minor, komunikasi mostly reliable, latency marginal, fault recovery partial, stability tested sederhana |
| **61-70** | ⚠️ Integrasi ada issue, komunikasi tidak always reliable, latency tinggi, fault recovery minimal, stability testing tidak comprehensive |
| **< 60** | ❌ Integrasi tidak berfungsi baik, komunikasi tidak reliable, latency tinggi, fault recovery tidak ada, stability testing tidak ada |

**Kriteria Detail:**
- ✓ End-to-end communication tested (command di web → ROS2 → embedded → aktuator response)
- ✓ Latency measured: < 50ms ideal, < 100ms acceptable untuk most robot, < 500ms untuk non-critical
- ✓ Komunikasi error handling: timeout, retry dengan exponential backoff, fallback mode
- ✓ QoS (Quality of Service): message delivery guaranteed untuk critical command
- ✓ Fault tolerance: jika embedded disconnected, ROS2 notify, web show offline status
- ✓ Automatic recovery: reconnection logic, state synchronization setelah reconnect
- ✓ Graceful degradation: sistem dapat operate di degraded mode (e.g., manual control jika auto disabled)
- ✓ Stress test: high frequency command (100Hz), multiple client connect, long-run (1+ jam) stable
- ✓ Log integration: all layer log ke single source (untuk debugging)

---

#### 6. PERFORMA SISTEM — 10%

| Skor | Deskripsi |
|------|-----------|
| **91-100** | ✅ Memenuhi semua spesifikasi (akurasi ±target, speed, repeatability), data kuantitatif lengkap (error, cycle time, uptime >95%), performa analysis mendalam (bandingkan actual vs target, identify bottleneck, suggest improvement) |
| **81-90** | ✅ Memenuhi spesifikasi, data kuantitatif ada, analysis reasonable |
| **71-80** | ⚠️ Memenuhi sebagian spesifikasi, data kuantitatif ada tapi incomplete, analysis basic |
| **61-70** | ⚠️ Spesifikasi tidak semua tercapai, data kuantitatif minimal, analysis superficial |
| **< 60** | ❌ Spesifikasi tidak tercapai, data kuantitatif tidak ada, analysis tidak ada |

**Kriteria Detail:**
- ✓ **Spesifikasi performa:** position accuracy, speed (m/s atau RPM), repeatability (σ, coefficient of variation)
- ✓ **Measurement method:** jelas & reproducible (sensor type, sampling rate, environment condition)
- ✓ **Data kuantitatif:**
  - Position error: max error, RMS error, steady-state error (jika tracking)
  - Speed: max speed, regulation (response time saat beban berubah)
  - Torque/Force: max output, consistency
  - Cycle time: average, min-max range
  - Uptime: MTBF (Mean Time Between Failure), availability %
- ✓ **Analisis mendalam:**
  - Compare actual vs target (chart overlay)
  - Error source analysis (mechanical, electrical, software)
  - Trade-off analysis (speed vs accuracy, power vs performance)
  - Improvement suggestion (untuk next iteration)
- ✓ **Graphical presentation:** plot clear, axis labeled, legend present, unit specified

---

#### 7. LAPORAN & DOKUMENTASI — 10%

| Skor | Deskripsi |
|------|-----------|
| **91-100** | ✅ Laporan terstruktur dengan executive summary, technical content jelas & mendalam, analisis & insight beyond requirement, documentation code lengkap (README, API doc, deployment guide), GitHub organized dengan README tiap folder, version control history clean, video demo jelas profesional |
| **81-90** | ✅ Laporan terstruktur, content jelas, documentation adequate, GitHub organized, video demo functional |
| **71-80** | ⚠️ Laporan ada struktur tapi ada bagian kurang clear, documentation basic, GitHub somewhat organized, video demo ada tapi quality sedang |
| **61-70** | ⚠️ Laporan kurang terstruktur, documentation minimal, GitHub kurang organized, video demo kurang clear |
| **< 60** | ❌ Laporan not structured, documentation missing, GitHub tidak terorganisir, video demo tidak ada/poor quality |

**Kriteria Detail:**

**Laporan:**
- ✓ Executive summary (1-2 halaman ringkasan project)
- ✓ Table of contents
- ✓ Introduction (problem statement, project objective, scope)
- ✓ System architecture (block diagram, layer explanation)
- ✓ Design & implementation (detail setiap subsystem, design decision & justification)
- ✓ Testing & validation (test procedure, result, analysis)
- ✓ Performance analysis (vs specification, trade-off, limitation)
- ✓ Conclusion & future work
- ✓ Appendix (code snippet, datasheet link, calibration data)
- ✓ Format: professional (header/footer, consistent font, page number, proper citation)

**Code Documentation:**
- ✓ README.md (di root): quick start, dependency, build instruction, hardware setup
- ✓ README.md (per folder): apa isi folder, cara menggunakan
- ✓ API documentation: endpoint list, parameter, response format (use Swagger atau simple table)
- ✓ Deployment guide: step untuk production deployment (software install, configuration, network setup)
- ✓ Troubleshooting guide: common issue & solution
- ✓ Code comment: function header, complex logic explanation

**GitHub:**
- ✓ Repository public (atau private dengan access granted ke instructor)
- ✓ Meaningful README.md di root (project description, key features, quick start)
- ✓ Folder structure clear & organized
- ✓ .gitignore proper (exclude build/, output/, .vscode/, node_modules/)
- ✓ Commit message semantic: `[Feature] Add PID tuning`, `[Fix] Fix motor direction`, `[Docs] Update README`
- ✓ Branch management: main (stable), develop (latest development)
- ✓ Tag untuk release/milestone: `v1.0`, `capstone-final`

**Video Demo:**
- ✓ Duration 5-10 menit (capstone), 2-3 menit (modul practical)
- ✓ Content: hardware overview → control demo → result explanation
- ✓ Screen recording: clear & readable (font size, mouse visible)
- ✓ Audio: clear, no background noise (atau subtitle jika audio problem)
- ✓ Video quality: min 720p, smooth frame rate
- ✓ Editing: professional cut, transition smooth, text overlay jika perlu
- ✓ Show multiple scenario (normal operation, boundary case, failure & recovery jika applicable)

---

## 📹 Rubrik Penilaian Tugas Video (Modul)

Mahasiswa diminta membuat video presentasi teori + praktikum setiap modul.

### Komponen Penilaian

#### 1. PEMBUKAAN — 5%

| Skor | Kriteria |
|------|----------|
| **91-100** | Perkenalan jelas & engaging, judul & tujuan disampaikan dengan menarik, outline video jelas, setter untuk audience |
| **81-90** | Perkenalan jelas, judul & tujuan ada, outline ada |
| **71-80** | Perkenalan ada, judul ada, outline kurang detail |
| **< 70** | Perkenalan kurang jelas atau tidak ada, outline tidak ada |

---

#### 2. PENJELASAN TEORI — 25%

| Skor | Kriteria |
|------|----------|
| **91-100** | Konsep utama dijelaskan correct & comprehensive, menggunakan diagram/visual yang mendukung, contoh aplikasi dunia nyata relevan & engaging, bahasa clear tanpa terlalu teknis, mengaitkan konsep dengan praktikum |
| **81-90** | Konsep utama correct, diagram ada, contoh ada, bahasa clear |
| **71-80** | Konsep mostly correct, diagram ada tapi sederhana, contoh ada, bahasa acceptable |
| **61-70** | Konsep ada error atau incomplete, diagram minimal, contoh kurang, bahasa kurang jelas |
| **< 60** | Konsep salah atau tidak jelas, diagram tidak ada, tidak ada contoh |

**Detail:**
- ✓ Penjelasan per sub-topik jelas & terstruktur
- ✓ Menggunakan visual: slide, diagram, whiteboard, animasi
- ✓ Formula/equation: jelaskan notation, unit, contoh nilai
- ✓ Contoh nyata: dari industri atau kehidupan sehari-hari
- ✓ Connection: "ini akan digunakan di praktikum..." atau "ini aplikasi dari modul sebelumnya..."
- ✓ Pace: tidak terlalu cepat, beri waktu audience untuk absorb

---

#### 3. SETUP & HARDWARE — 15%

| Skor | Kriteria |
|------|----------|
| **91-100** | Semua komponen ditunjukkan dengan clear, wiring/connection dijelaskan detail (close-up, trace satu-satu), software & konfigurasi dijelaskan step, environment setup clear |
| **81-90** | Komponen ditunjukkan, wiring explained, software setup ada, environment clear |
| **71-80** | Komponen ditunjukkan basic, wiring shown tapi kurang detail, software setup partial |
| **61-70** | Komponen ditunjukkan kurang jelas, wiring tidak detail, software setup kurang clear |
| **< 60** | Komponen tidak ditunjukkan atau tidak clear, wiring tidak ada, software setup tidak ada |

**Detail:**
- ✓ Show & identify setiap komponen (nama, fungsi, spesifikasi singkat)
- ✓ Wiring: close-up video, trace setiap koneksi (dari pin A ke pin B), jelaskan why (power ke Vcc, data ke GPIO)
- ✓ Software setup: install, konfigurasi (screenshot atau demo), verifikasi install success
- ✓ Hardware test: sederhana test (e.g., "power on, LED nyala, sensor reading ada")

---

#### 4. DEMO PRAKTIKUM — 30%

| Skor | Kriteria |
|------|----------|
| **91-100** | Screen recording coding/build jelas & readable, hasil output ditampilkan (serial monitor, plot, dashboard, hardware moving), penjelasan code line-by-line untuk bagian penting, demo parameter change (min 3 variasi) dengan visible effect, analisis hasil vs teori, troubleshooting issue ditampilkan & dijelaskan solusi |
| **81-90** | Screen recording clear, output ditampilkan, code explanation ada, parameter change ada (2 variasi), analisis reasonable, troubleshooting partial |
| **71-80** | Screen recording somewhat clear, output ada, code explanation basic, parameter change 1, analisis surface-level |
| **61-70** | Screen recording kurang clear, output kurang visible, code explanation minimal, parameter tidak diubah |
| **< 60** | Screen recording tidak clear, output tidak ditampilkan, code explanation tidak ada |

**Detail:**

**Screen Recording:**
- ✓ Code editor terlihat jelas (font size besar, tidak zoom out terlalu)
- ✓ Compilation process: mouse click compile, wait, show "compile success"
- ✓ Upload to hardware: show serial port selection, upload progress, "Upload successful"
- ✓ Rata-rata 2-3 menit untuk bagian ini

**Output Display:**
- ✓ **Sensor/embedded:** serial monitor showing data real-time
- ✓ **ROS2:** topic list, message content, rqt graph jika ada
- ✓ **Control:** setpoint vs actual plot, error plot
- ✓ **Hardware:** video robot/mechanism moving (jika applicable)
- ✓ **Dashboard:** web interface showing live data

**Code Explanation:**
- ✓ Penjelasan per section: setup(), loop(), function important
- ✓ Penjelasan khusus: algorithm kritis (PID implementation, sensor fusion, state machine)
- ✓ Jangan explain syntax basics (apa itu for loop), focus pada logic
- ✓ 3-5 menit untuk code walkthrough

**Parameter Change:**
- ✓ Identifikasi 3-4 parameter penting yang bisa diubah
- ✓ Setiap kali ubah, compile → upload → tampilkan result
- ✓ Jelaskan expected effect, bandingkan dengan actual
- ✓ Contoh (untuk modul PID): ubah Kp → lihat rise time berubah, ubah Ki → lihat steady error hilang, ubah Kd → lihat overshoot berkurang

**Analisis:**
- ✓ Result match dengan teori?
- ✓ Jika ada deviation, analysis mengapa
- ✓ Error source identification (measurement noise, hardware limitation, tuning suboptimal)
- ✓ Comparison: "algoritma A vs B, lebih baik mana?"

---

#### 5. TROUBLESHOOTING & INSIGHT — 15%

| Skor | Kriteria |
|------|----------|
| **91-100** | Masalah yang dihadapi dijelaskan dengan jelas, debugging process ditampilkan (step-by-step approach), solusi explained & validated, lesson learned dari issue, tips useful untuk audience, insight beyond expectation |
| **81-90** | Masalah & solusi jelas, debugging shown, lesson learned ada, tips ada |
| **71-80** | Masalah dijelaskan, solusi ada, debugging partial, tips basic |
| **61-70** | Masalah kurang jelas, solusi surface, troubleshooting minimal |
| **< 60** | Tidak ada troubleshooting content |

**Detail:**
- ✓ **Real issue:** bukan buatan, tapi genuine problem yang ditemui
- ✓ **Debugging approach:** show bagaimana identify issue (serial monitor, oscilloscope, sensor reading)
- ✓ **Solution:** explain fix yang diapply, why it works
- ✓ **Validation:** after fix, show result correct
- ✓ **Lesson learned:** takeaway untuk praktikum selanjutnya atau production system
- ✓ **Tips:** e.g., "jangan lupa decoupling capacitor", "tuning PID test di kondisi nyata jangan simulasi"

---

#### 6. PENUTUP — 5%

| Skor | Kriteria |
|------|----------|
| **91-100** | Kesimpulan singkat jelas, ringkas key point, link ke modul selanjutnya, call-to-action (tonton next modul, coba praktikum), closing profesional |
| **81-90** | Kesimpulan jelas, next steps ada, closing profesional |
| **71-80** | Kesimpulan ada, next steps partial, closing sederhana |
| **< 70** | Kesimpulan kurang jelas atau tidak ada |

---

#### 7. TEKNIS VIDEO — 5%

| Skor | Kriteria |
|------|----------|
| **91-100** | Audio crystal clear (no noise, volume consistent, dynamic range good), visual HD minimum 1080p (sharp, well-lit), editing professional (smooth cut, transition meaningful, text overlay readable), pacing appropriate (tidak terlalu cepat/lambat), subtitle available (jika non-native speaker) |
| **81-90** | Audio clear, video 720p minimum, editing professional, pacing good |
| **71-80** | Audio acceptable (minor noise), video 720p, editing simple, pacing reasonable |
| **61-70** | Audio have background noise, video 480p acceptable, editing basic, pacing jerky |
| **< 60** | Audio poor/noise, video low quality, editing rough, pacing inconsistent |

**Detail:**
- ✓ **Audio:** jelas & mudah dimengerti, no echo, background noise minimal (use mic atau noise reduction)
- ✓ **Video:** 720p min (1080p better), frame rate smooth (24/30fps min), not shaky (use tripod/stabilizer)
- ✓ **Lighting:** wajah terang, latar belakang not blown out, consistent throughout video
- ✓ **Editing:** cut out silence/mistake, smooth transition antar scene, text overlay readable (white/black dengan kontrast)
- ✓ **Pacing:** tidak terlalu cepat (audience dapat ikut), tidak terlalu lambat (boring), total 15-20 menit ideal
- ✓ **Caption/Subtitle:** recommended untuk clarity terutama technical term
- ✓ **Thumbnail & title:** menarik & professional untuk YouTube (jika upload)

---

### 📊 Skala Penilaian (Umum)

| Skala | Grade | Interpretasi |
|-------|-------|--------------|
| 91-100 | A | Excellent - Melebihi ekspektasi, innovative solution, industry-ready |
| 81-90 | AB | Very Good - Memenuhi semua requirement dengan baik, minor refinement |
| 71-80 | B | Good - Memenuhi requirement, ada beberapa issue minor |
| 61-70 | BC | Satisfactory - Memenuhi requirement minimal, beberapa issue significant |
| 51-60 | C | Acceptable - Banyak issue tapi sistem masih berfungsi, major revision needed |
| < 50 | D/E | Not Acceptable - Sistem tidak berfungsi atau tidak memenuhi requirement |

---

### 🎁 BONUS POINTS

**Project Bonus (+5 poin max):**
- ✅ Kreativitas desain (aesthetic, innovative mechanical solution)
- ✅ Extra features beyond requirement
- ✅ Optimization significant (energy efficient, cost reduction)
- ✅ Documentation outstanding (presentation quality, README clarity)
- ✅ Open-source contribution (library, tools published)

**Video Bonus (+5 poin max):**
- ✅ Animasi atau visual effect yang enhance explanation
- ✅ Kolaborasi tim terlihat jelas (bukan hanya satu orang narasi)
- ✅ Insight mendalam (teori advanced, experiment design yang sophisticated)
- ✅ Production quality tinggi (professional edit, sound design, cinematography)
- ✅ Engagement tinggi (penonton merasa tertarik, walau tidak saat praktikum)

---

**Last Updated:** February 2026  
**Version:** 1.0
