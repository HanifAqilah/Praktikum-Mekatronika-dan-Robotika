# RUBRIK PENILAIAN VIDEO MODUL 1
## ARSITEKTUR SISTEM MEKATRONIKA & ROBOTIKA

**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 01 - Arsitektur Sistem Mekatronika & Robotika  
**Durasi Video:** 15-20 menit  
**Total Bobot:** 100%

---

## KOMPONEN PENILAIAN

### 1. PEMBUKAAN (5%)

| Kriteria | Excellent (4.5-5%) | Good (3.5-4%) | Satisfactory (2.5-3%) | Poor (< 2.5%) |
|----------|-------------------|---------------|---------------------|---------------|
| **Perkenalan** | Perkenalan jelas (nama, kelompok, institusi), confident, profesional | Perkenalan jelas, cukup confident | Perkenalan basic, kurang confident | Perkenalan tidak jelas atau tidak ada |
| **Judul & Tujuan** | Judul disebutkan jelas, tujuan pembelajaran specific & measurable, context setting excellent | Judul & tujuan jelas, context ok | Judul/tujuan disebutkan, minimal context | Tidak jelas atau skip |
| **Outline** | Outline lengkap & terstruktur (5-7 poin), audience tahu expect apa | Outline ok (3-5 poin), cukup jelas | Outline minimal (1-2 poin) | No outline |

**Sub-total: ______ / 5%**

---

### 2. PENJELASAN TEORI (25%)

| Kriteria | Excellent (22-25%) | Good (18-21%) | Satisfactory (13-17%) | Poor (< 13%) |
|----------|-------------------|---------------|---------------------|---------------|
| **Konsep Sensor-Controller-Actuator (5%)** | Dijelaskan jelas dengan diagram, feedback loop clear, contoh konkret, analogy membantu pemahaman | Konsep dijelaskan, diagram ada, contoh ok | Konsep dijelaskan basic, diagram minimal | Tidak jelas atau salah |
| **5-Layer Architecture (5%)** | Setiap layer explained (plant, embedded, edge, industrial, IoT), interface antar layer jelas, contoh each layer | Layer disebutkan & dijelaskan, interface ok, some contoh | Layer disebutkan, penjelasan basic | Layer tidak jelas atau skip |
| **Centralized vs Distributed (4%)** | Perbandingan jelas (table/diagram), advantage-disadvantage discussed, use case when to use which, trade-off explained | Perbandingan ok, advantage-disadvantage mentioned, use case basic | Perbandingan minimal, advantage-disadvantage disebutkan | Tidak jelas atau skip |
| **Communication Protocol (4%)** | MQTT, Modbus, CAN dijelaskan (topology, latency, reliability), comparison table shown, selection criteria discussed | Protocol dijelaskan, comparison ok, criteria mentioned | Protocol disebutkan, comparison minimal | Tidak jelas atau skip |
| **Real-time System (3%)** | Dijelaskan why important (determinism, latency, jitter), contoh real-time vs non-real-time, impact to system | Dijelaskan importance, contoh ok | Explained basic, minimal contoh | Skip atau salah |
| **Contoh Industri (2%)** | Min 2 contoh nyata (AGV, robot arm, smart building) dengan detail, relate to theory | 1-2 contoh, relate to theory | 1 contoh, brief mention | No contoh |
| **Formula/Metrik (2%)** | Formula/metrik disebutkan (latency ms, throughput msg/sec, reliability %), contoh calculation shown | Formula disebutkan, no calculation | Formula mentioned briefly | No formula |

**Sub-total: ______ / 25%**

---

### 3. SETUP & HARDWARE (15%)

| Kriteria | Excellent (13-15%) | Good (10-12%) | Satisfactory (7-9%) | Poor (< 7%) |
|----------|-------------------|---------------|---------------------|---------------|
| **Komponen Ditunjukkan (5%)** | Semua komponen ditunjukkan jelas (motor, sensor, PCB, mikrokontroler, PC), close-up shot, annotation on screen, part number disebutkan | Komponen ditunjukkan, cukup jelas, some annotation | Komponen ditunjukkan, kurang jelas, no annotation | Komponen tidak jelas atau skip |
| **Koneksi Dijelaskan (5%)** | Signal flow dijelaskan step-by-step (motor ↔ driver ↔ ESP32 ↔ WiFi ↔ Ubuntu ↔ Web), wiring shown, protocol disebutkan | Koneksi dijelaskan, signal flow ok, wiring shown | Koneksi disebutkan, penjelasan basic | Tidak jelas atau skip |
| **Software Stack (3%)** | Software stack explained (PlatformIO, ROS2, Node.js), version disebutkan, dependency management discussed | Software disebutkan, version ok, basic explanation | Software mentioned briefly | Skip atau tidak jelas |
| **Block Diagram (2%)** | Block diagram ditunjukkan on screen, annotation clear, relate to hardware shown | Block diagram shown, annotation ok | Block diagram shown, minimal annotation | No block diagram |

**Sub-total: ______ / 15%**

---

### 4. DEMO PRAKTIKUM (30%)

| Kriteria | Excellent (26-30%) | Good (21-25%) | Satisfactory (16-20%) | Poor (< 16%) |
|----------|-------------------|---------------|---------------------|---------------|
| **CAD Design (5%)** | Fusion 360 screen recording jelas, navigate assembly, motion study shown, explain design choice, constraint explained | Screen recording ok, assembly shown, explain design, motion study ada | Screen recording basic, assembly shown, minimal explanation | Tidak jelas atau skip |
| **Schematic (4%)** | EasyEDA screen recording, explain schematic section (power, control, interface), component rating disebutkan, protection circuit highlighted | Schematic shown, explain basic, component ok | Schematic shown briefly | Skip |
| **Code Structure (4%)** | Code structure explained (sensor.h, motor.h, control.h), modular design jelas, open file & show, explain interface | Code structure explained, modular ok, show code | Code shown, minimal explanation | Skip |
| **Command Flow Demo (7%)** | Live demo: user command via web → ROS2 → ESP32 → motor bergerak, screen recording + hardware video side-by-side, latency mentioned | Demo jelas, command flow ok, screen + hardware shown, some latency info | Demo ok, command flow shown, kurang smooth | Demo gagal atau tidak jelas |
| **Signal Routing Explained (4%)** | Signal type explained (digital, PWM, analog, serial), oscilloscope shown (optional tapi bonus jika ada), data format discussed | Signal routing explained, type mentioned, format ok | Signal routing mentioned briefly | Skip |
| **Latency Analysis (3%)** | Latency dari user command sampai motor response measured (e.g., 150ms), breakdown per stage (web 20ms, ROS 30ms, ESP 10ms, motor 90ms) | Latency measured, overall value mentioned | Latency mentioned, no measurement | Skip |
| **Parameter Variation (3%)** | Min 2-3 parameter diubah (e.g., speed, trajectory, sensor threshold), effect shown, analyze impact | 1-2 parameter diubah, effect shown | 1 parameter diubah briefly | No variation |

**Sub-total: ______ / 30%**

---

### 5. FAILURE ANALYSIS (15%)

| Kriteria | Excellent (13-15%) | Good (10-12%) | Satisfactory (7-9%) | Poor (< 7%) |
|----------|-------------------|---------------|---------------------|---------------|
| **FMEA Table (5%)** | FMEA table ditunjukkan on screen, min 5 failure mode explained, severity/occurrence/detection discussed, RPN calculated | FMEA table shown, 3-5 failure mode, severity discussed, RPN mentioned | FMEA shown, minimal explanation | No FMEA |
| **Contoh Failure (4%)** | Min 1 contoh failure konkret (e.g., motor tidak bergerak), cause analysis (power loss, driver fail, firmware crash), systematic debugging shown | Contoh failure, cause analysis ok, some debugging | Contoh failure mentioned briefly | No contoh |
| **Mitigation Demo (3%)** | Mitigation demonstrated (e.g., trigger watchdog → observe safe stop, disconnect sensor → timeout → fallback), effect shown | Mitigation explained, demo basic | Mitigation mentioned, no demo | Skip |
| **Safety Perspective (3%)** | Safety discussed ("jika motor gagal, system harus safe stop, bukan continue"), fail-safe design explained | Safety mentioned, fail-safe concept ok | Safety mentioned briefly | Skip |

**Sub-total: ______ / 15%**

---

### 6. TROUBLESHOOTING & INSIGHT (10%)

| Kriteria | Excellent (9-10%) | Good (7-8%) | Satisfactory (5-6%) | Poor (< 5%) |
|----------|-------------------|---------------|---------------------|---------------|
| **Problem Encountered (3%)** | Min 2 masalah dijelaskan konkret (e.g., "motor tidak jalan karena driver overheating"), real situation, not generic | 1-2 masalah explained, cukup konkret | 1 masalah mentioned briefly | No problem discussed |
| **Debugging Process (3%)** | Debugging systematic (check power → check kabel → check firmware → check parameter), tools used (multimeter, serial monitor), step-by-step shown | Debugging explained, some tools, systematic approach | Debugging mentioned, minimal detail | Skip |
| **Lesson Learned (2%)** | Insight valuable ("penting verifikasi setiap layer independent sebelum integrate"), actionable takeaway, relatable | Lesson mentioned, some insight | Lesson mentioned briefly | No lesson learned |
| **Tips (2%)** | Min 2-3 tips practical ("dokumentasikan pin mapping, jangan hardcode, gunakan config file"), helpful untuk audience | 1-2 tips, cukup helpful | 1 tip mentioned | No tips |

**Sub-total: ______ / 10%**

---

## TOTAL SCORE: ______ / 100%

---

## BONUS POINTS (Maksimal +5%)

| Kriteria | Bobot | Deskripsi | Score |
|----------|-------|-----------|-------|
| **Visual Effects & Editing** | +2% | Professional editing (transition smooth, text overlay, highlight penting, background music subtle, color grading) | ______ |
| **Oscilloscope/Advanced Tool** | +2% | Show oscilloscope untuk verify signal, logic analyzer, atau tool advanced lain dengan proper explanation | ______ |
| **Animation/Diagram** | +1% | Custom animation untuk explain concept (e.g., signal flow animation, 3D CAD animation), not just static slide | ______ |
| **English Narration** | +2% | Full English narration (jika course dalam Bahasa Indonesia, ini bonus; vice versa) | ______ |

**Bonus Total: ______ / 5% (max)**

---

## PENALTY

| Kriteria | Deduction |
|----------|-----------|
| **Durasi < 15 menit** | -5% |
| **Durasi > 25 menit** | -3% (too long, audience fatigue) |
| **Audio Tidak Jelas** | -5% (noise, echo, volume terlalu rendah) |
| **Video Quality < 720p** | -3% |
| **No Screen Recording** | -10% (critical untuk demo code & software) |
| **No Hardware Shown** | -15% (must show real hardware, not just simulation) |
| **Reading from Script** | -5% (video terlihat tidak natural, kurang engaging) |
| **Plagiarism (copy video orang)** | -100% + disciplinary action |

**Penalty Total: ______ %**

---

## TECHNICAL QUALITY (Termasuk dalam penilaian keseluruhan)

| Aspek | Checklist | Score Impact |
|-------|-----------|--------------|
| **Video Resolution** | ☐ 1080p (ideal) ☐ 720p (min acceptable) ☐ < 720p (penalty) | Termasuk dalam penalty jika < 720p |
| **Audio Quality** | ☐ Clear voice ☐ No background noise ☐ Volume consistent | Penalty jika tidak jelas |
| **Lighting** | ☐ Face & hardware well-lit ☐ No glare on screen | Affect clarity, indirect impact |
| **Camera Stability** | ☐ Stable (tripod/stand) ☐ No shaky footage | Affect professionalism |
| **Screen Recording Quality** | ☐ High resolution ☐ Cursor visible ☐ Text readable | Critical untuk demo, penalty jika tidak ada |

---

## FINAL SCORE

**Final Score = Base Score + Bonus - Penalty**

**Final Score: ______ / 100%**

---

## GRADE CONVERSION

| Range | Grade | Keterangan |
|-------|-------|------------|
| 91-105 | **A** | Excellent — Video sangat jelas, profesional, insightful |
| 81-90 | **AB** | Very Good — Video jelas, semua requirement terpenuhi dengan baik |
| 71-80 | **B** | Good — Video ok, requirement terpenuhi, ada area improvement |
| 61-70 | **BC** | Satisfactory — Video memenuhi minimum, banyak yang bisa ditingkatkan |
| 51-60 | **C** | Pass — Video kurang jelas, barely meet requirement |
| < 50 | **D/E** | Fail — Video tidak memenuhi requirement minimum |

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

### Specific Feedback per Section:

**Pembukaan:**  


**Teori:**  


**Setup & Hardware:**  


**Demo:**  


**Failure Analysis:**  


**Troubleshooting:**  


**Technical Quality:**  


### Recommendations untuk Video Berikutnya:
1. 
2. 
3. 

---

**Penilai:** __________________  
**Tanggal:** __________________  
**Tanda Tangan:** __________________

---

## TIPS UNTUK MAHASISWA

### Pre-Production:
✓ **Script outline** — Buat outline, tapi jangan baca verbatim (terlihat tidak natural)  
✓ **Practice** — Latihan 2-3× sebelum recording final  
✓ **Equipment check** — Test camera, mic, lighting sebelum mulai  

### During Recording:
✓ **Eye contact** — Lihat ke kamera sesekali, engage dengan audience  
✓ **Pace** — Jangan terlalu cepat (audience tidak paham) atau terlalu lambat (boring)  
✓ **Energy** — Enthusiasm terlihat di video, jangan monoton  
✓ **Show, don't just tell** — Demo hardware & software, jangan cuma explain  

### Post-Production:
✓ **Edit out mistakes** — Cut bagian yang salah, stutter, long pause  
✓ **Add captions/annotations** — Text overlay untuk highlight poin penting  
✓ **Background music** — Subtle, jangan terlalu loud, fade out saat narration  
✓ **Check before submit** — Watch full video, check audio sync, export quality  

### Common Mistakes to Avoid:
✗ Reading script word-by-word (tidak natural)  
✗ Too much theory, too little demo (boring)  
✗ Shaky camera, poor lighting (unprofessional)  
✗ No hardware shown (simulation only)  
✗ Too long (> 25 menit, audience fatigue)  
✗ No conclusion (ending tiba-tiba)  

---

**Version:** 1.0  
**Last Updated:** 05-Feb-2026  
**Reference:** Berdasarkan best practice video tutorial teknik & presentasi akademik
