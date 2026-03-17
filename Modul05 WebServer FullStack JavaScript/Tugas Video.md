# TUGAS VIDEO MODUL 05: WEB SERVER & FULL-STACK JAVASCRIPT

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – Web Server & Full-Stack JavaScript  
**Sifat Tugas:** Individual  
**Durasi Video:** 10–15 menit  
**Format:** MP4 minimal 1080p  
**Platform Upload:** LMS / Google Drive / YouTube Unlisted

---

## A. KETENTUAN UMUM

1. Video dikerjakan **per orang** (bukan kelompok).
2. Tampilkan **nama lengkap dan NIM** di awal video dengan teks overlay.
3. Gunakan **screen recording** untuk semua demo (terminal + browser wajib terlihat).
4. **Webcam** opsional tapi direkomendasikan di sudut layar.
5. Tidak ada hardware fisik — semua berbasis software.
6. Narasi Bahasa Indonesia yang runtut dan jelas.

---

## B. URUTAN WAJIB ISI VIDEO

### BAGIAN 1 — PENJELASAN MATERI (Maks. 3 menit)

- [ ] Jelaskan arsitektur full-stack: Browser ↔ Express.js ↔ Database
- [ ] Jelaskan perbedaan HTTP polling vs WebSocket (Socket.IO)
- [ ] Jelaskan alur autentikasi JWT: register → login → token → protected route
- [ ] Jelaskan kegunaan CloudFlare Tunnel untuk deployment

### BAGIAN 2 — DEMONSTRASI PERCOBAAN 1–7 (Maks. 6 menit)

Setiap percobaan: tampilkan **terminal running + browser/Postman hasil**

- [ ] **Percobaan 1:** Express Hello World — terminal server running, 3 endpoint di browser/Postman
- [ ] **Percobaan 2:** REST API CRUD — Postman 8 request (GET, POST baru, PUT update, DELETE, 404)
- [ ] **Percobaan 3:** SQLite — tambah data, restart server, data tetap ada (persistent)
- [ ] **Percobaan 4:** React Frontend — UI berjalan, add/toggle/delete todo via browser
- [ ] **Percobaan 5:** Socket.IO — **2 tab browser** update counter bersamaan realtime
- [ ] **Percobaan 6:** CloudFlare Tunnel — URL tunnel diakses dari HP (tunjukkan address bar)
- [ ] **Percobaan 7:** Full-Stack Integration — 2 tab tersinkronisasi via Socket.IO tanpa refresh

### BAGIAN 3 — DEMONSTRASI PROJECT (Maks. 4 menit)

Demonstrasikan project kelompok yang dipilih (Soal 1-5):

- [ ] Tunjukkan tampilan frontend (UI/UX) yang berjalan
- [ ] Demo login dengan JWT (register → login → akses protected route)
- [ ] Demo fitur utama CRUD (buat, edit, hapus minimal 1 item)
- [ ] Demo real-time (Socket.IO) — tampilkan 2 tab tersync
- [ ] Tunjukkan URL CloudFlare Tunnel accessible (bukan localhost)
- [ ] Tunjukkan di Postman: minimal 1 endpoint API dengan token JWT di header

### BAGIAN 4 — PENUTUP (Maks. 2 menit)

- [ ] Ringkas yang dipelajari dari modul ini (3 poin penting)
- [ ] Sebutkan kesulitan terbesar dan bagaimana mengatasinya
- [ ] Sebutkan potensi pengembangan aplikasi ke depan

---

## C. RUBRIK PENILAIAN VIDEO (100 POIN)

| No | Komponen Penilaian | Bobot |
|----|--------------------|-------|
| 1 | Penjelasan materi (arsitektur, JWT, Socket.IO) | 20 |
| 2 | Demo percobaan 1–7 (semua terlihat jelas) | 30 |
| 3 | Demo project (fitur lengkap, realtime, deploy) | 30 |
| 4 | Kualitas rekaman (resolusi, audio, narasi) | 20 |
| **TOTAL** | | **100** |

### Detail Rubrik Per Bagian

**Penjelasan Materi (20 poin):**
| Kriteria | Poin |
|---------|------|
| Arsitektur full-stack dijelaskan dengan diagram | 5 |
| Perbedaan polling vs WebSocket dipahami | 5 |
| Alur JWT benar (hash, sign, verify) | 5 |
| CloudFlare tunnel dijelaskan tujuan dan caranya | 5 |

**Demo Percobaan (30 poin):**
| Kriteria | Poin |
|---------|------|
| P1 Express Hello World — 3 endpoint terlihat | 3 |
| P2 CRUD API — 5+ test Postman dengan status code | 5 |
| P3 SQLite — persistensi setelah restart dibuktikan | 4 |
| P4 React — UI operasional (add/toggle/delete) | 4 |
| P5 Socket.IO — 2 tab sync realtime terlihat | 6 |
| P6 CloudFlare — URL tunnel dari HP | 4 |
| P7 Full-Stack — kombinasi semua berjalan | 4 |

**Demo Project (30 poin):**
| Kriteria | Poin |
|---------|------|
| Login JWT berfungsi | 5 |
| CRUD fitur utama berjalan | 8 |
| Realtime Socket.IO dibuktikan | 8 |
| CloudFlare URL accessible | 5 |
| Postman JWT protected route | 4 |

**Kualitas Produksi (20 poin):**
| Kriteria | Poin |
|---------|------|
| Resolusi ≥ 1080p, tidak blur | 5 |
| Audio jelas, tidak ada noise berlebihan | 5 |
| Narasi runtut dan dapat dipahami | 5 |
| Durasi tepat (10-15 menit) | 5 |

---

## D. TIPS RECORDING

### Tool Rekomendasi

| Tool | Platform | Keterangan |
|------|---------|-----------|
| **OBS Studio** | Windows/macOS/Linux | Free, professional, bisa overlay webcam |
| **ShareX** | Windows | Free, simple, screenshot + screen record |
| **Loom** | All | Upload otomatis ke cloud, share link |
| **CapCut** | Windows/macOS | Editing mudah, add text overlay |

### Setup Recording OBS

1. Download OBS: https://obsproject.com
2. Sources: `Display Capture` (layar penuh) + `Audio Input Capture` (mic)
3. Webcam (opsional): `Video Capture Device`
4. Output: Settings → Output → Recording Format: `mp4`
5. Resolution: 1920×1080, 30fps

### Tips Audio

- Rekam di ruangan tenang, jauh dari kipas/AC
- Gunakan headset dengan mic (lebih bersih dari mic laptop)
- Pastikan volume normal — tidak terlalu keras/pelan

### Tips Editing

- Potong bagian setup/instalasi yang lama (skip atau time-lapse)
- Tambah teks overlay untuk label setiap percobaan
- Tambah penanda waktu atau nama percobaan di pojok video

---

## E. CHECKLIST SEBELUM SUBMIT

- [ ] Durasi video 10–15 menit
- [ ] Nama + NIM terlihat di awal video
- [ ] Semua 7 percobaan ter-demo
- [ ] Project kelompok ter-demo (login, CRUD, realtime, tunnel)
- [ ] CloudFlare URL terlihat di address bar (bukan localhost)
- [ ] Resolusi ≥ 1080p
- [ ] Audio jelas
- [ ] File format MP4
- [ ] Upload ke Google Drive / YouTube dan share link

---

## F. LARANGAN

- ✗ Tidak boleh menggunakan video orang lain atau merekam ulang video grup
- ✗ Tidak boleh mengubah kecepatan video untuk menyembunyikan error
- ✗ Tidak boleh memperlihatkan credential asli (API key, password DB)
- ✗ Tidak boleh submit video kurang dari 10 menit

---

**Deadline: Minggu ke-4, Jumat 23:59 WIB**

Submit: Upload video + paste link ke kolom LMS
