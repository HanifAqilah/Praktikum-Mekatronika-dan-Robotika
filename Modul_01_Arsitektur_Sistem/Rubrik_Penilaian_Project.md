# RUBRIK PENILAIAN PROJECT MODUL 1
## DESIGN & ANALYZE END-TO-END ROBOTICS SYSTEM

**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 01 - Arsitektur Sistem Mekatronika & Robotika  
**Total Bobot:** 100% (+ Bonus maksimal 10%)

---

## ASPEK PENILAIAN

### 1. DESAIN MEKANIK (15%)

| Kriteria | Excellent (13-15%) | Good (10-12%) | Satisfactory (7-9%) | Poor (< 7%) |
|----------|-------------------|---------------|---------------------|-------------|
| **CAD Model (5%)** | Assembly lengkap dengan semua komponen, constraint properly defined, motion study menunjukkan workspace accurate | Assembly lengkap, constraint ada tapi ada yang kurang tepat, motion study basic | Assembly ada tapi simplified, constraint minimal, no motion study | Model tidak lengkap atau tidak ada |
| **Engineering Drawing (4%)** | Drawing lengkap dengan dimensi, toleransi, material specification, view projection correct, follows standard (ISO/ANSI) | Drawing ada dengan dimensi, toleransi basic, projection correct, minor error | Drawing simplified, dimensi tidak lengkap, projection ada error | Drawing tidak ada atau tidak memadai |
| **Design Justification (3%)** | Component selection well-justified (torque calculation, stress analysis, material property), alternative considered | Component selection justified dengan calculation basic, some reasoning | Component selection dengan minimal justification, no calculation | No justification atau salah |
| **Physical Assembly (3%)** | Assembled hardware profesional, alignment perfect, cable management rapi, foto multiple angles high quality | Assembled functional, alignment good, cable ok, foto memadai | Assembled dengan minor issue (wobble, cable messy), foto kurang jelas | Assembly tidak selesai atau tidak functional |

**Sub-total: ______ / 15%**

---

### 2. DESAIN ELEKTRONIK (15%)

| Kriteria | Excellent (13-15%) | Good (10-12%) | Satisfactory (7-9%) | Poor (< 7%) |
|----------|-------------------|---------------|---------------------|-------------|
| **Schematic Quality (5%)** | Schematic lengkap, jelas, semua komponen properly rated (voltage, current, power), net labeling consistent, easy to follow | Schematic lengkap, komponen rated, minor labeling issue | Schematic simplified, some rating missing, confusing layout | Schematic tidak lengkap atau salah |
| **PCB Layout (4%)** | PCB layout optimal: power/signal separated, ground plane continuous, trace width sesuai current, via placement strategic, thermal consideration | Layout good: separation ok, ground plane ada, trace width ok, minor issue | Layout basic: power/signal mixed, no ground plane, trace width uniform | Layout poor atau tidak ada PCB |
| **Protection & Safety (3%)** | Protection lengkap: fuse di input, flyback diode di motor, TVS/varistor untuk spike, current limiting, polarity protection | Protection ada: fuse, flyback diode, basic protection | Protection minimal: hanya fuse atau flyback | No protection device |
| **BOM & Implementation (3%)** | BOM lengkap dengan part number, datasheet link, price, supplier, lead time; foto implementasi profesional dengan annotation | BOM lengkap part number & price; foto implementasi jelas | BOM basic (part name & qty); foto ada tapi kurang jelas | BOM tidak lengkap atau tidak ada foto |

**Sub-total: ______ / 15%**

---

### 3. SOFTWARE EMBEDDED (20%)

| Kriteria | Excellent (17-20%) | Good (14-16%) | Satisfactory (10-13%) | Poor (< 10%) |
|----------|-------------------|---------------|---------------------|-------------|
| **Code Structure (5%)** | Modular design (sensor.h, motor.h, control.h separate), consistent naming, proper header guards, makefile/platformio.ini configured | Modular dengan minor issue, naming mostly consistent, config ok | Some modularity, naming inconsistent, config basic | Monolithic code, poor structure |
| **Code Quality (5%)** | Clean code: meaningful variable names, appropriate comments, no magic numbers (uses #define), efficient algorithm, follows style guide | Code readable, good naming, comments present, minor style issue | Code works but hard to read, minimal comments, some magic numbers | Code messy, no comments, hard to understand |
| **Control Algorithm (5%)** | Algorithm correct & optimal (e.g., PID tuned well, state machine robust, sensor fusion accurate), handles edge cases | Algorithm correct, basic tuning, handles normal cases | Algorithm works for simple case, no tuning, limited functionality | Algorithm incorrect atau tidak ada |
| **Safety Implementation (3%)** | Robust safety: E-stop responsive (<100ms), watchdog enabled, timeout detection, fault logging, recovery mechanism | Safety functional: E-stop ok, timeout detection, basic fault handling | Minimal safety: E-stop only, no recovery | No safety features |
| **Testing & Debugging (2%)** | Unit test present, compile clean (no warning), serial debug output structured, test coverage good | Compile clean, debug output ada, basic test | Compile dengan warning, minimal debug output | Compile error atau no debug capability |

**Sub-total: ______ / 20%**

---

### 4. SOFTWARE PC (ROS2, Web Dashboard) — 15%

| Kriteria | Excellent (13-15%) | Good (10-12%) | Satisfactory (7-9%) | Poor (< 7%) |
|----------|-------------------|---------------|---------------------|-------------|
| **Architecture Design (5%)** | ROS2 node well-designed (separation of concern), topic/service appropriate, clean architecture, URDF model accurate | ROS2 architecture ok, topic ok, URDF basic | ROS2 setup basic, functionality limited, URDF simplified | No ROS2 atau architecture poor |
| **Web Dashboard (5%)** | Dashboard functional & responsive, real-time data update, user-friendly UI, multiple views (graph, map, status), mobile-compatible | Dashboard functional, real-time update, UI decent, basic views | Dashboard basic, delayed update, UI simple | No dashboard atau tidak functional |
| **Data Logging (3%)** | Comprehensive logging: timestamped, multiple data source, CSV/JSON format, plot generated, statistics calculated | Logging functional: timestamp, CSV, basic plot | Minimal logging: text file, no plot | No logging |
| **Database (2%)** | Database (SQL/NoSQL) untuk persistent storage, query efficient, data integrity maintained | Database setup, basic query, some persistence | Minimal database (file-based) | No database |

**Sub-total: ______ / 15%**

**Note:** Jika tidak menggunakan ROS2/Web (embedded only), bobot dipindah ke Software Embedded (total 35%) atau nilai disesuaikan.

---

### 5. INTEGRASI SISTEM (15%)

| Kriteria | Excellent (13-15%) | Good (10-12%) | Satisfactory (7-9%) | Poor (< 7%) |
|----------|-------------------|---------------|---------------------|-------------|
| **End-to-End Integration (6%)** | Semua layer terintegrasi seamless (mechanical, electrical, embedded, PC), command flow smooth (web → ROS2 → ESP32 → motor), no manual intervention needed | Integration works, minor manual step, mostly seamless | Integration dengan banyak manual step, some disconnection | Integration gagal atau sangat terbatas |
| **Communication Reliability (4%)** | Komunikasi robust dengan error handling (retry, timeout, checksum), no packet loss, consistent latency (<100ms for critical) | Komunikasi functional, basic error handling, occasional packet loss, latency acceptable | Komunikasi works tapi unreliable, high latency, frequent disconnect | Komunikasi sering gagal |
| **Fault Recovery (3%)** | System dapat auto-recover dari fault common (sensor disconnect, motor stall), graceful degradation, log error for analysis | Basic recovery (e.g., restart after timeout), manual intervention sometimes needed | Minimal recovery, mostly manual reset | No recovery, system hang/crash |
| **System Robustness (2%)** | System stable untuk prolonged operation (>30 menit continuous), handle load variation, no memory leak, thermal management ok | System stable untuk moderate operation (10-20 menit), handle normal load | System unstable after short operation, crash occasionally | System crash frequently |

**Sub-total: ______ / 15%**

---

### 6. PERFORMA SISTEM (10%)

| Kriteria | Excellent (9-10%) | Good (7-8%) | Satisfactory (5-6%) | Poor (< 5%) |
|----------|-------------------|---------------|---------------------|-------------|
| **Specification Compliance (5%)** | Memenuhi atau exceed specification (accuracy, speed, repeatability, success rate) dengan data kuantitatif lengkap (min 20 test runs) | Memenuhi spec dengan minor deviation, data kuantitatif memadai (10-15 runs) | Partly meet spec, data terbatas (5-10 runs) | Tidak memenuhi spec atau data tidak cukup |
| **Performance Analysis (3%)** | Analisis mendalam: compare spec vs actual dengan statistical analysis (mean, std dev, confidence interval), identify limiting factor, propose optimization | Analisis ok: compare spec vs actual, basic statistics, some analysis | Analisis superficial: hanya show data, minimal interpretation | No analysis |
| **Consistency & Repeatability (2%)** | Hasil consistent across multiple trials, low variance, repeatability good (std dev < 5% of mean) | Hasil mostly consistent, moderate variance (std dev 5-10%) | Hasil inconsistent, high variance (std dev > 10%) | Hasil random, tidak repeatable |

**Sub-total: ______ / 10%**

---

### 7. LAPORAN & DOKUMENTASI (10%)

| Kriteria | Excellent (9-10%) | Good (7-8%) | Satisfactory (5-6%) | Poor (< 5%) |
|----------|-------------------|---------------|---------------------|-------------|
| **Report Structure (3%)** | Terstruktur profesional dengan executive summary, clear section, proper figure/table numbering, citation correct (IEEE style), grammar excellent | Struktur ok, section clear, citation present, minor grammar error | Struktur basic, some section missing, citation incomplete, grammar issue | Struktur poor, section messy, no citation |
| **Technical Content (4%)** | Content jelas & mendalam, technical depth appropriate, diagram/graph informative, derivation/calculation correct, trade-off discussed | Content jelas, technical ok, diagram ok, calculation correct, some depth | Content basic, technical shallow, diagram minimal, calculation ada error | Content tidak jelas, technical wrong, diagram confusing |
| **Code Documentation (2%)** | GitHub repo organized (folder structure, README.md comprehensive, commit history clean, meaningful commit message, API documentation) | Repo ok, README present, commit ok, some documentation | Repo messy, README minimal, commit message poor | No GitHub atau repo sangat buruk |
| **Presentation Material (1%)** | Slide profesional, visual appealing, content concise, demo video high quality (15-20 menit, 720p+, audio clear, editing smooth) | Slide ok, visual decent, video ok (15-20 min, 720p, audio ok) | Slide basic, visual simple, video kurang (durasi/quality) | Slide poor, no video atau video sangat buruk |

**Sub-total: ______ / 10%**

---

## TOTAL SCORE (Base): ______ / 100%

---

## BONUS POINTS (Maksimal +10%)

| Kriteria | Bobot | Deskripsi | Score |
|----------|-------|-----------|-------|
| **Innovative Design** | +5% | Fitur tambahan beyond requirement yang meaningful (e.g., computer vision, advanced path planning, adaptive control, multi-robot coordination) | ______ |
| **Advanced Analysis** | +3% | Cost-benefit analysis, Monte Carlo simulation untuk reliability, FEA untuk mechanical stress, thermal analysis | ______ |
| **Full Integration** | +5% | ROS2 + Web dashboard fully functional dengan semua fitur advanced (e.g., SLAM, autonomous navigation, database analytics) | ______ |
| **Exceptional Performance** | +5% | Performance jauh exceed specification (e.g., accuracy 2× better, speed 1.5× faster, success rate 98%+) dengan data supporting | ______ |
| **Professional Presentation** | +2% | Video demo sangat profesional (editing, narration, animation), poster quality publication-level | ______ |

**Bonus Total: ______ / 10% (max)**

---

## PENALTY

| Kriteria | Deduction |
|----------|-----------|
| **Late Submission** | -5% per hari (max -25%) |
| **Plagiarism** | -100% (zero score) + academic disciplinary action |
| **No Video Demo** | -15% |
| **No Hardware Implementation** | -20% (simulation only) |
| **Incomplete Deliverable** | -5% per missing major deliverable |
| **Safety Violation** | -10% (e.g., no fuse, no E-stop pada hardware berbahaya) |

**Penalty Total: ______ %**

---

## FINAL SCORE

**Final Score = Base Score + Bonus - Penalty**

**Final Score: ______ / 100%**

---

## GRADE CONVERSION

| Range | Grade | Keterangan |
|-------|-------|------------|
| 91-110 | **A** | Excellent — Memenuhi semua requirement dengan kualitas tinggi, ada inovasi |
| 81-90 | **AB** | Very Good — Memenuhi semua requirement dengan baik |
| 71-80 | **B** | Good — Memenuhi requirement dengan beberapa kekurangan minor |
| 61-70 | **BC** | Satisfactory — Memenuhi requirement minimum, banyak area untuk improvement |
| 51-60 | **C** | Pass — Barely meet requirement, significant improvement needed |
| < 50 | **D/E** | Fail — Tidak memenuhi requirement minimum |

**Grade Akhir: ______**

---

## FEEDBACK & COMMENTS

### Strengths (Apa yang sudah baik):
1. 
2. 
3. 

### Areas for Improvement (Apa yang perlu diperbaiki):
1. 
2. 
3. 

### Specific Technical Feedback:
- **Mechanical Design:**  

- **Electrical Design:**  

- **Software (Embedded):**  

- **Software (PC):**  

- **Integration:**  

- **Documentation:**  

### Recommendations untuk Modul Berikutnya:
1. 
2. 
3. 

---

**Penilai:** __________________  
**Tanggal:** __________________  
**Tanda Tangan:** __________________

---

## CATATAN PENGGUNAAN RUBRIK

**Untuk Mahasiswa:**
- Gunakan rubrik ini sebagai **checklist** saat mengerjakan project
- Pastikan semua aspek terpenuhi sebelum submission
- Self-assessment: nilai diri sendiri dulu, identify gap, improve
- Tanyakan ke instruktur jika ada kriteria yang tidak jelas

**Untuk Instruktur:**
- Nilai setiap aspek secara **independent** (jangan terpengaruh overall impression)
- Berikan **feedback specific** untuk setiap section, bukan hanya score
- Diskusikan hasil dengan mahasiswa untuk **learning opportunity**
- Konsisten dengan rubrik, tapi boleh ada **discretion** untuk kasus khusus (jelaskan di comments)

---

**Version:** 1.0  
**Last Updated:** 05-Feb-2026  
**Next Review:** End of semester (feedback from students & instructors)
