# TUGAS VIDEO MODUL 04: IoT WebServer MQTT ESP32

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 04 – IoT WebServer MQTT ESP32  
**Sifat Tugas:** Individual  
**Durasi Video:** 15–25 menit  
**Format:** MP4 (minimal 720p)  
**Platform Upload:** LMS / Google Drive / YouTube Unlisted

---

## A. KETENTUAN UMUM

1. Video dikerjakan **per orang**.
2. Wajib menampilkan **nama lengkap dan NIM** di awal video.
3. Gunakan **screen recording** untuk bagian coding dan dashboard.
4. **Webcam** wajib terlihat di sudut layar saat menjelaskan.
5. Sertakan **video hardware** — tunjukkan rangkaian ESP32 + sensor + LED yang bekerja nyata.
6. Narasi Bahasa Indonesia yang runtut.

---

## B. URUTAN WAJIB ISI VIDEO

### BAGIAN 1 — PENJELASAN MATERI (Maks. 3 menit)

- [ ] Jelaskan konsep IoT dan arsitektur publish-subscribe
- [ ] Jelaskan perbedaan HTTP Web Server vs MQTT
- [ ] Jelaskan komponen MQTT: publisher, subscriber, broker, topic, QoS
- [ ] Jelaskan peran ESP32 sebagai edge device IoT

### BAGIAN 2 — DEMONSTRASI SELURUH PERCOBAAN (Maks. 10 menit)

Tunjukkan dengan **screen recording** dan **video hardware**:

- [ ] **Percobaan 1:** WiFi Station — Serial Monitor menampilkan IP dan RSSI
- [ ] **Percobaan 2:** Access Point — smartphone terkoneksi ke AP ESP32
- [ ] **Percobaan 3:** Web Server Sensor — browser menampilkan data sensor real-time
- [ ] **Percobaan 4:** Web Server LED — kontrol 3 LED dari browser (tunjukkan LED fisik menyala)
- [ ] **Percobaan 5:** MQTT Publisher — data sensor muncul di terminal subscriber Ubuntu
- [ ] **Percobaan 6:** MQTT Subscriber — LED dikontrol dari mosquitto_pub (tunjukkan LED fisik)
- [ ] **Percobaan 7:** Integrasi — dashboard HTML lengkap dengan sensor + kontrol + MQTT

### BAGIAN 3 — DEMONSTRASI PROJECT (Maks. 7 menit)

- [ ] Tunjukkan dashboard greenhouse dengan grafik real-time
- [ ] Demonstrasikan mode otomatis (sensor trigger aktuator)
- [ ] Demonstrasikan mode manual (override dari dashboard)
- [ ] Tunjukkan alert/notifikasi saat threshold terlampaui
- [ ] Tunjukkan MQTT Explorer menerima semua data

### BAGIAN 4 — VIDEO HARDWARE PRAKTIKUM DAN PROJECT (Maks. 3 menit)

- [ ] Tunjukkan rangkaian fisik ESP32 + sensor + LED + buzzer di breadboard
- [ ] Tunjukkan LED menyala/mati sesuai kontrol
- [ ] Tunjukkan sensor DHT22 membaca data nyata
- [ ] Tunjukkan buzzer berbunyi saat alert

### BAGIAN 5 — PENUTUP (Maks. 2 menit)

- [ ] Ringkas seluruh materi dan percobaan
- [ ] Sebutkan kendala dan solusi
- [ ] Kesimpulan tentang penerapan IoT dalam robotika

---

## C. RUBRIK PENILAIAN VIDEO (100 POIN)

| No | Komponen Penilaian | Bobot |
|----|--------------------|-------|
| 1 | Penjelasan materi IoT, MQTT, WebServer | 15 |
| 2 | Demonstrasi seluruh 7 percobaan | 25 |
| 3 | Demonstrasi project (dashboard + mode + alert) | 25 |
| 4 | Video hardware (rangkaian fisik bekerja) | 20 |
| 5 | Kualitas rekaman, penjelasan, webcam | 15 |
| **TOTAL** | | **100** |

---

## D. CHECKLIST SEBELUM SUBMIT

- [ ] Nama dan NIM tampil di awal
- [ ] Webcam terlihat saat menjelaskan
- [ ] Penjelasan materi IoT dan MQTT
- [ ] Demonstrasi 7 percobaan dengan screen record + hardware
- [ ] Demonstrasi project (dashboard, mode, alert)
- [ ] Video hardware rangkaian fisik
- [ ] Penutup dengan ringkasan
- [ ] Resolusi minimal 720p, audio jelas
- [ ] Durasi 15–25 menit

**Penamaan file:** `Video_Modul04_[NIM]_[NamaLengkap].mp4`
