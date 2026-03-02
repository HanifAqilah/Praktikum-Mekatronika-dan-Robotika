# TUGAS VIDEO MODUL 05: IoT PLC SCHNEIDER MODBUS

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – IoT PLC AI Modbus  
**Sifat Tugas:** Individual  
**Durasi Video:** 15–25 menit  
**Format:** MP4 (minimal 720p)  
**Platform Upload:** LMS / Google Drive / YouTube Unlisted

---

## A. KETENTUAN UMUM

1. Video dikerjakan **per orang**.
2. Wajib menampilkan **nama lengkap dan NIM** di awal video.
3. **Screen recording** untuk software (EcoStruxure, Arduino IDE, Python, browser).
4. **Webcam** wajib terlihat di sudut layar saat menjelaskan.
5. **Video hardware** wajib menunjukkan PLC fisik, wiring, dan ESP32.

---

## B. URUTAN WAJIB ISI VIDEO

### BAGIAN 1 — PENJELASAN MATERI (Maks. 3 menit)

- [ ] Jelaskan apa itu PLC dan perbedaannya dengan mikrokontroler
- [ ] Jelaskan protokol Modbus (RTU dan TCP), konsep master-slave
- [ ] Jelaskan register Modbus: coil, discrete input, holding register, input register
- [ ] Jelaskan konsep AI predictive maintenance

### BAGIAN 2 — DEMONSTRASI SELURUH PERCOBAAN (Maks. 10 menit)

- [ ] **Percobaan 1:** Ladder Diagram Start/Stop — screen record EcoStruxure + video PLC LED
- [ ] **Percobaan 2:** Structured Text counter/timer — tampilkan register berubah
- [ ] **Percobaan 3:** Modbus RTU Read — ESP32 membaca register PLC
- [ ] **Percobaan 4:** Modbus RTU Write — ESP32 menulis ke PLC, PLC bereaksi
- [ ] **Percobaan 5:** Modbus TCP Python — Python membaca/menulis register
- [ ] **Percobaan 6:** Bridge Modbus-MQTT — data PLC muncul di MQTT
- [ ] **Percobaan 7:** Anomaly Detection — Python mendeteksi anomali dari data PLC

### BAGIAN 3 — DEMONSTRASI PROJECT (Maks. 7 menit)

- [ ] Tunjukkan program PLC lini produksi berjalan
- [ ] Tunjukkan dashboard SCADA web dari ESP32
- [ ] Tunjukkan data PLC di MQTT Explorer
- [ ] Tunjukkan AI predictive maintenance dengan grafik anomali
- [ ] Tunjukkan sistem terintegrasi berjalan

### BAGIAN 4 — VIDEO HARDWARE (Maks. 3 menit)

- [ ] Tunjukkan wiring PLC + power supply + push button + LED
- [ ] Tunjukkan koneksi RS485 (PLC ↔ MAX485 ↔ ESP32)
- [ ] Tunjukkan PLC fisik dengan program berjalan (LED output berubah)
- [ ] Tunjukkan rangkaian ESP32 sebagai gateway

### BAGIAN 5 — PENUTUP (Maks. 2 menit)

- [ ] Ringkas materi dan percobaan
- [ ] Sebutkan kendala dan solusi
- [ ] Kesimpulan penerapan PLC + IoT + AI

---

## C. RUBRIK PENILAIAN VIDEO (100 POIN)

| No | Komponen | Bobot |
|----|---------|-------|
| 1 | Penjelasan materi PLC, Modbus, AI | 15 |
| 2 | Demonstrasi 7 percobaan | 25 |
| 3 | Demonstrasi project SCADA mini | 25 |
| 4 | Video hardware PLC + ESP32 | 20 |
| 5 | Kualitas rekaman dan penjelasan | 15 |
| **TOTAL** | | **100** |

---

## D. CHECKLIST SEBELUM SUBMIT

- [ ] Nama dan NIM di awal video
- [ ] Webcam terlihat
- [ ] Penjelasan materi PLC + Modbus + AI
- [ ] Demonstrasi 7 percobaan (screen record + hardware)
- [ ] Demonstrasi project SCADA
- [ ] Video hardware PLC fisik
- [ ] Penutup dengan ringkasan
- [ ] Resolusi 720p+, audio jelas, durasi 15–25 menit

**Penamaan file:** `Video_Modul05_[NIM]_[NamaLengkap].mp4`
