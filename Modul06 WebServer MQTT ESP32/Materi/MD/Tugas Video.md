# Tugas Video — Modul 6: WebServer MQTT ESP32
**Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi**  
**Dosen: Rofiq Cahyo Prayogo, S.T., M.T.**  
**Praktikum Mekatronika — 2 SKS**

---

## RINGKASAN TUGAS

| Aspek | Ketentuan |
|-------|-----------|
| Jenis Tugas | Rekaman Video + Upload YouTube |
| Jumlah Video | 2 video per mahasiswa |
| Platform | YouTube (masing-masing akun) |
| Deadline | **2 minggu setelah pelaksanaan praktikum** |
| Pengumpulan | Link YouTube dikumpulkan via LMS/Google Form |
| Format Pengumpulan | NIM – Nama – Link Video 1 – Link Video 2 |

---

## DESKRIPSI TUGAS

Setiap mahasiswa wajib membuat dan mengunggah **dua video** ke YouTube pribadi masing-masing yang merekam dan menjelaskan seluruh proses belajar Modul 6: WebServer MQTT ESP32.

### Filosofi Tugas Ini
Tugas video dirancang untuk memastikan bahwa:
1. Praktikan **benar-benar memahami** materi, bukan hanya menyalin kode
2. Kemampuan **mengkomunikasikan** pengetahuan teknis secara verbal
3. **Dokumentasi proses belajar** yang dapat ditinjau kembali
4. Membangun **portofolio digital** di bidang IoT

---

## VIDEO 1 — Screen Recording Penjelasan Teori & Program

### Tujuan Video 1
Membuktikan pemahaman konseptual tentang WebServer, MQTT, ESP32, dan kode program yang dibuat.

### Durasi
- **Minimum: 10 menit**
- **Maksimum: 25 menit**

### Konten Wajib (Checklist)

**Bagian 1 — Perkenalan (1-2 menit):**
- [ ] Sebutkan nama lengkap dan NIM
- [ ] Sebutkan judul modul yang dipraktikumkan
- [ ] Tampilkan outline singkat apa yang akan dijelaskan

**Bagian 2 — Penjelasan Teori (3-5 menit):**
- [ ] Jelaskan apa itu IoT dan arsitektur sistemnya
- [ ] Jelaskan perbedaan HTTP dan MQTT dengan kata-kata sendiri
- [ ] Jelaskan konsep Publish-Subscribe pada MQTT
- [ ] Jelaskan minimal **2 rumus** yang relevan (pilih dari daftar di bawah)
- [ ] Jelaskan QoS MQTT (Level 0, 1, 2) dan kapan masing-masing digunakan

**Bagian 3 — Penjelasan Kode Program (5-10 menit):**
- [ ] Buka Arduino IDE, tampilkan kode yang dibuat
- [ ] Jelaskan fungsi setiap bagian kode (setup, loop, callback)
- [ ] Jelaskan cara kerja AsyncWebServer routes
- [ ] Jelaskan cara kerja MQTT publish dan subscribe
- [ ] Jelaskan penggunaan ArduinoJson
- [ ] Jelaskan implementasi FreeRTOS (jika ada)
- [ ] Jelaskan struktur JSON yang digunakan

**Bagian 4 — Kesimpulan (1 menit):**
- [ ] Sebutkan 3 hal yang dipelajari dari praktikum ini
- [ ] Sebutkan 1 kesulitan yang ditemui dan cara mengatasinya

### Daftar Rumus yang Harus Dijelaskan (Pilih Minimal 2)

1. **FSPL (Free Space Path Loss):** $L_{FSPL}(dB) = 20\log_{10}(d) + 20\log_{10}(f) - 147.55$
2. **MQTT Latency QoS 1:** $L_{QoS1} = RTT_{pub-broker} + RTT_{broker-sub} + 2L_{broker}$
3. **Efisiensi Protokol:** $\eta = \frac{D_{payload}}{D_{total}} \times 100\%$
4. **Throughput TCP:** $T_{TCP} = \frac{W_{window}}{RTT}$
5. **Battery Life:** $t = \frac{C_{battery}}{I_{avg}} = \frac{C_{battery}}{I_{active} \times D + I_{sleep} \times (1-D)}$
6. **Nyquist Sampling:** $f_s \geq 2 \times f_{max}$
7. **RMS ADC:** $V_{rms} = \sqrt{\frac{1}{N}\sum v_i^2}$
8. **Daya Listrik:** $P = V_{rms} \times I_{rms} \times \cos\phi$

---

## VIDEO 2 — Rekaman Praktikum & Demo Hardware

### Tujuan Video 2
Membuktikan bahwa sistem benar-benar berjalan dan praktikan mampu mengoperasikannya secara langsung.

### Durasi
- **Minimum: 10 menit**
- **Maksimum: 30 menit**

### Konten Wajib (Checklist)

**Bagian 1 — Perkenalan & Setup (1-2 menit):**
- [ ] Sebutkan nama dan NIM di awal video
- [ ] Tampilkan hardware (ESP32, sensor, rangkaian) secara jelas
- [ ] Tampilkan skema rangkaian atau diagram koneksi

**Bagian 2 — Demo Instalasi & Konfigurasi (3-5 menit):**
- [ ] Tampilkan Arduino IDE dengan library yang terinstal
- [ ] Tampilkan Board Manager ESP32 terinstal
- [ ] Tampilkan Mosquitto broker berjalan (status active)
- [ ] Tampilkan konfigurasi WiFi dan MQTT di kode

**Bagian 3 — Demo Upload & Running (5-10 menit):**
- [ ] Rekam proses compile dan upload ke ESP32 (tampilkan tidak ada error)
- [ ] Tampilkan Serial Monitor dengan output IP Address
- [ ] Buka browser dan akses WebServer, tampilkan dashboard
- [ ] Demonstrasikan data sensor update real-time di dashboard
- [ ] Demonstrasikan tombol LED ON/OFF berfungsi mengubah LED fisik
- [ ] Buka MQTT Explorer atau terminal, tampilkan data MQTT masuk
- [ ] Kirim perintah via `mosquitto_pub`, tampilkan LED merespons
- [ ] Tampilkan Node-RED dashboard (jika mengerjakan Praktikum 5)

**Bagian 4 — Demo Project (jika ada, 5-10 menit):**
- [ ] Demonstrasikan project tambahan yang dipilih
- [ ] Jelaskan fitur-fitur yang ditambahkan
- [ ] Tampilkan kode tambahan yang dibuat

**Bagian 5 — Penutup (1 menit):**
- [ ] Refleksi singkat tentang apa yang berhasil dan tidak
- [ ] Ucapan terima kasih

---

## TEKNIS PEMBUATAN VIDEO

### Peralatan yang Dibutuhkan

| Kebutuhan | Rekomendasi |
|-----------|-------------|
| Screen Recorder | OBS Studio (gratis), Loom, ShareX |
| Kamera | Kamera HP (720p minimum) untuk Video 2 |
| Mikrofon | Earphone HP (cukup untuk suara jelas) |
| Pencahayaan | Pastikan cukup cahaya, tidak backlit |
| Editor Video | DaVinci Resolve (gratis), CapCut, iMovie |

### Standar Teknis Minimum

| Parameter | Requirement |
|-----------|-------------|
| Resolusi | Minimum **720p** (1280×720) |
| Frame Rate | Minimum 24 fps |
| Audio | Suara jelas, tidak ada noise berlebihan |
| Format File | MP4 (sebelum upload YouTube) |
| Bahasa | Indonesia |
| Subtitle | Opsional (nilai plus) |

### Tips Membuat Video yang Baik

**Video 1 (Screen Recording):**
- Gunakan OBS Studio: Settings → Output → Recording Quality: High
- Pastikan font terminal/editor cukup besar agar terbaca
- Tampilkan wajah di Picture-in-Picture (pojok layar)
- Gunakan laser pointer software untuk highlight kode
- Bicara dengan ritme yang tidak terlalu cepat

**Video 2 (Demo Hardware):**
- Posisikan kamera stabil (gunakan tripod atau sandar)
- Pastikan LED dan tampilan layar laptop terlihat di satu frame
- Saat rekam terminal, pastikan huruf besar dan terbaca
- Rekam dalam satu take jika memungkinkan (lebih natural)
- Pastikan lingkungan tidak terlalu berisik

### Format Judul Video YouTube

```
[Video 1] NIM_NAMA_Modul6_MQTT_Teori
Contoh: 2151234_AhmadFauzi_Modul6_MQTT_Teori

[Video 2] NIM_NAMA_Modul6_MQTT_Demo
Contoh: 2151234_AhmadFauzi_Modul6_MQTT_Demo
```

### Deskripsi YouTube (Isi minimal):
```
Praktikum Mekatronika - Modul 6: WebServer MQTT ESP32
Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi
Dosen: Rofiq Cahyo Prayogo, S.T., M.T.

Nama: [Nama Mahasiswa]
NIM: [NIM]
Tanggal Praktikum: [Tanggal]

#ESP32 #MQTT #IoT #WebServer #Arduino #Mekatronika
```

---

## CARA PENGUMPULAN

### Format Pengumpulan via LMS

Isi form dengan format:
```
NIM         : [NIM Anda]
Nama        : [Nama Lengkap]
Link Video 1: https://youtu.be/xxxxx (Teori & Program)
Link Video 2: https://youtu.be/xxxxx (Demo Hardware)
Catatan     : [Opsional — hal yang ingin disampaikan ke dosen]
```

### Deadline
**Tepat 2 minggu (14 hari) setelah tanggal pelaksanaan praktikum**

| Waktu Pengumpulan | Konsekuensi |
|-------------------|-------------|
| Tepat waktu | Nilai penuh |
| Terlambat 1-3 hari | Pengurangan 20 poin |
| Terlambat 4-7 hari | Pengurangan 40 poin |
| Terlambat > 7 hari | Tidak diterima (nilai 0) |
| Tidak mengumpulkan | Nilai E untuk tugas |

---

## RUBRIK PENILAIAN

**Total Nilai Tugas: 100 poin**  
*(Bobot dalam nilai akhir praktikum: sesuai kontrak perkuliahan)*

---

### KOMPONEN A — KONTEN TEORI (Video 1) — 30 Poin

#### A1. Kedalaman Penjelasan Konsep (15 poin)

| Skor | Indikator |
|------|-----------|
| 13-15 | Menjelaskan IoT, HTTP, MQTT, dan ESP32 dengan benar, komprehensif, menggunakan analogi yang tepat, mampu membedakan use-case setiap protokol |
| 9-12  | Menjelaskan sebagian besar konsep dengan benar, ada sedikit ketidakakuratan, pemahaman dasar terpenuhi |
| 5-8   | Penjelasan dangkal atau terlalu bergantung membaca slide/catatan, beberapa konsep salah |
| 1-4   | Sebagian besar konsep salah atau tidak dijelaskan |
| 0     | Tidak ada penjelasan teori sama sekali |

#### A2. Penjelasan Rumus (15 poin)

| Skor | Indikator |
|------|-----------|
| 13-15 | Menjelaskan ≥ 3 rumus dengan benar, menyebutkan variabel dan satuannya, memberikan contoh perhitungan numerik |
| 9-12  | Menjelaskan 2 rumus dengan benar, contoh perhitungan ada namun mungkin tidak lengkap |
| 5-8   | Menjelaskan 1-2 rumus, ada beberapa kesalahan dalam penjelasan |
| 1-4   | Menyebutkan rumus tapi tidak mampu menjelaskan artinya |
| 0     | Tidak ada penjelasan rumus |

---

### KOMPONEN B — PENJELASAN KODE PROGRAM (Video 1) — 25 Poin

#### B1. Penjelasan Kode WebServer (10 poin)

| Skor | Indikator |
|------|-----------|
| 9-10 | Menjelaskan semua route handler, cara kerja async, method GET/POST, JSON response |
| 7-8  | Menjelaskan sebagian besar kode dengan benar, ada penjelasan yang kurang lengkap |
| 4-6  | Penjelasan kode sangat umum, tidak menjelaskan detail penting |
| 1-3  | Hanya membacakan kode tanpa penjelasan bermakna |
| 0    | Tidak menjelaskan kode sama sekali |

#### B2. Penjelasan Kode MQTT (10 poin)

| Skor | Indikator |
|------|-----------|
| 9-10 | Menjelaskan setup PubSubClient, connect, publish, subscribe, callback, LWT dengan benar |
| 7-8  | Menjelaskan sebagian besar fungsi MQTT dengan benar |
| 4-6  | Hanya menjelaskan fungsi dasar (connect + publish) |
| 1-3  | Penjelasan tidak akurat atau sangat dangkal |
| 0    | Tidak menjelaskan kode MQTT |

#### B3. Penjelasan JSON & FreeRTOS (5 poin)

| Skor | Indikator |
|------|-----------|
| 5   | Menjelaskan ArduinoJson serialize/deserialize DAN FreeRTOS task/mutex dengan benar |
| 4   | Menjelaskan salah satu (JSON atau FreeRTOS) dengan baik |
| 2-3 | Penjelasan ada namun kurang akurat |
| 1   | Disebutkan tapi tidak dijelaskan |
| 0   | Tidak ada penjelasan |

---

### KOMPONEN C — DEMO HARDWARE (Video 2) — 25 Poin

#### C1. Kelengkapan Demo Sistem (15 poin)

| Skor | Indikator |
|------|-----------|
| 13-15 | Semua fitur terdemonstrasi: dashboard update real-time, toggle LED dari browser, MQTT Explorer menampilkan data, perintah mqtt_pub merespons LED, Node-RED dashboard |
| 9-12  | Sebagian besar fitur terdemonstrasi (minimal: dashboard berjalan + LED dapat dikontrol + MQTT data tampil) |
| 5-8   | Demo parsial: WebServer berjalan tapi MQTT tidak atau sebaliknya |
| 1-4   | Demo gagal di sebagian besar bagian, hanya upload berhasil |
| 0     | Tidak ada demo hardware yang berhasil |

#### C2. Kualitas Rekaman Hardware (5 poin)

| Skor | Indikator |
|------|-----------|
| 5   | Rangkaian, LED, dan layar laptop terlihat jelas dalam satu frame, pencahayaan baik |
| 4   | Sebagian besar terlihat jelas, ada satu aspek yang kurang |
| 2-3 | Gambar buram atau sulit melihat detail hardware/layar |
| 1   | Sangat sulit melihat apa yang sedang terjadi |
| 0   | Hardware tidak terlihat sama sekali |

#### C3. Kelancaran dan Kepercayaan Diri Presentasi (5 poin)

| Skor | Indikator |
|------|-----------|
| 5   | Presentasi mengalir lancar, suara jelas, tidak terbaca dari catatan, mampu menjelaskan impromptu saat ada masalah |
| 4   | Sebagian besar lancar, ada jeda wajar |
| 2-3 | Terlihat membaca catatan, beberapa kali berhenti lama |
| 1   | Sangat tidak lancar, banyak jeda panjang |
| 0   | Tidak berbicara atau audio tidak jelas sama sekali |

---

### KOMPONEN D — KUALITAS VIDEO (Teknis) — 10 Poin

#### D1. Kualitas Audio-Visual (5 poin)

| Skor | Indikator |
|------|-----------|
| 5   | Resolusi ≥ 1080p, suara jernih, pencahayaan sempurna, konten terbaca jelas |
| 4   | Resolusi 720p, suara cukup jelas, pencahayaan baik |
| 2-3 | Kualitas minimum, ada noise audio atau gambar kurang jelas |
| 1   | Kualitas buruk tapi masih dapat dipahami |
| 0   | Kualitas sangat buruk, tidak dapat dipahami |

#### D2. Struktur dan Kelengkapan Video (5 poin)

| Skor | Indikator |
|------|-----------|
| 5   | Kedua video memenuhi durasi minimum, ada intro + isi + penutup, judul YouTube sesuai format |
| 4   | Kedua video ada, durasi sedikit kurang atau judul tidak sesuai format |
| 2-3 | Hanya satu video yang lengkap |
| 1   | Video ada tapi jauh di bawah requirement |
| 0   | Video tidak diupload atau tidak dapat diakses |

---

### KOMPONEN E — KREATIVITAS & INOVASI — 10 Poin

| Skor | Indikator |
|------|-----------|
| 9-10 | Menambahkan elemen kreatif signifikan: demo project tambahan yang bekerja, visualisasi materi kreatif (animasi/diagram), koneksi ke platform cloud tambahan, fitur inovatif di luar requirement |
| 7-8  | Ada tambahan kreatif seperti editing video yang baik, diagram sendiri, atau satu fitur ekstra pada program |
| 4-6  | Ada upaya kreativitas kecil (thumbnail menarik, background musik, atau penjelasan dengan papan tulis) |
| 1-3  | Sedikit usaha di luar minimum requirement |
| 0    | Tidak ada elemen kreativitas, hanya memenuhi minimum requirement |

---

## REKAP NILAI

| Komponen | Sub-Komponen | Poin Maks |
|----------|-------------|-----------|
| **A. Konten Teori** | A1. Penjelasan Konsep | 15 |
|                     | A2. Penjelasan Rumus | 15 |
| **B. Kode Program** | B1. WebServer | 10 |
|                     | B2. MQTT | 10 |
|                     | B3. JSON + FreeRTOS | 5 |
| **C. Demo Hardware** | C1. Kelengkapan Demo | 15 |
|                      | C2. Kualitas Rekaman | 5 |
|                      | C3. Presentasi | 5 |
| **D. Kualitas Video** | D1. Audio-Visual | 5 |
|                       | D2. Struktur & Kelengkapan | 5 |
| **E. Kreativitas** | Inovasi & Kreativitas | 10 |
| | **TOTAL** | **100** |

---

## CONTOH HASIL YANG BAIK

### Indikator Video 1 yang Baik:
```
✓ Membuka screen recording dengan menunjukkan kode sambil menjelaskan
✓ "Di sini saya mendefinisikan route GET /api/sensor yang akan 
   mengembalikan data JSON dari sensor DHT22..."
✓ "Rumus MQTT Latency untuk QoS 1 adalah RTT_pub-broker ditambah 
   RTT_broker-sub ditambah 2 kali latency broker. Jika RTT saya 
   30ms dan broker latency 1ms, maka latency QoS 1 = 30 + 30 + 2 = 62ms"
✓ Menjelaskan mengapa FreeRTOS digunakan: "Karena AsyncWebServer perlu 
   merespons request kapan saja, sementara kita juga perlu publish MQTT 
   secara periodik..."
```

### Indikator Video 2 yang Baik:
```
✓ Memulai dengan menampilkan breadboard + ESP32 + sensor
✓ "Saya akan upload kode sekarang, perhatikan tidak ada error compile"
✓ "IP Address yang muncul di Serial Monitor adalah 192.168.1.105"
✓ "Sekarang saya buka browser dan ketik 192.168.1.105..."
✓ "Bisa dilihat suhu berubah dari 28.2 menjadi 28.4 secara otomatis"
✓ "Sekarang saya kirim perintah LED ON dari terminal..."
✓ [LED fisik menyala di video]
```

---

## FAQ (Pertanyaan yang Sering Ditanyakan)

**Q: Apakah video harus diunggah sebagai publik di YouTube?**  
A: Tidak harus publik. Anda boleh set sebagai **Unlisted** (tidak terindeks, tapi bisa diakses via link). Jangan private karena dosen tidak bisa mengakses.

**Q: Apakah wajah harus terlihat di video?**  
A: Video 1 (screen recording) wajib ada PiP wajah di pojok layar. Video 2 (hardware demo) wajah tidak harus terlihat jelas, yang penting suara jelas.

**Q: Bolehkah demo dilakukan dalam kondisi ada error, lalu debugging?**  
A: Sangat dianjurkan! Proses debugging yang jujur lebih dihargai daripada demo yang dipotong. Nilai tambah untuk troubleshooting yang baik.

**Q: Apakah saya harus mengerjakan project tambahan?**  
A: Tidak wajib, tapi nilai Kreativitas (10 poin) lebih mudah penuh jika ada demo project.

**Q: Apakah harus dalam bahasa Indonesia?**  
A: Ya, gunakan Bahasa Indonesia. Istilah teknis (HTTP, MQTT, publish, subscribe, dll) boleh tetap dalam bahasa Inggris.

**Q: Bagaimana jika ESP32 saya rusak atau komponen bermasalah?**  
A: Hubungi asisten dosen secepatnya. Sistem yang tidak dapat berjalan secara fisik dapat diganti dengan simulasi di Wokwi Online (wokwi.com) untuk Video 2, dengan catatan.

---

*Tugas ini merupakan komponen penting dari nilai praktikum. Kerjakan dengan sungguh-sungguh sebagai bukti pemahaman dan kompetensi Anda di bidang IoT.*

*Selamat mengerjakan! 💪*
