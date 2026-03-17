# PROJECT MODUL 07: MAZE SOLVER ROBOT

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 07 – Wall Follower  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Tim rescue robotics kampus mendapat misi membangun **robot maze solver** yang bisa menyelesaikan labirin 1 × 1 meter otonom. Labirin dibuat dari papan triplek setinggi 15 cm, dengan jalur selebar 25 cm, memiliki 4 belokan, 1 dead-end, dan zona garis di lantai (transisi line → wall mode).
>
> Tantangan utama: robot harus **menyelesaikan labirin dari START ke FINISH** dengan waktu secepat mungkin. Pada run ke-2, robot diharapkan bisa menyelesaikan **lebih cepat** karena sudah "belajar" dari run pertama (path optimization).
>
> Panitia memberikan bonus jika robot bisa menampilkan **peta labirin** (grid map) di Serial Monitor setelah selesai misi.

---

## B. SOAL PROJECT

### Soal 1 — Maze Solving dengan Left-Wall Following

> Implementasikan left-wall following untuk menyelesaikan labirin. Robot harus:
> - Membaca 3 sensor ultrasonik secara kontinu
> - Menggunakan FSM minimal 4 state
> - Menyelesaikan labirin dari START ke FINISH tanpa bantuan manusia
>
> **Output:** Video run pertama full, log Serial Monitor (jarak sensor + state).

### Soal 2 — PD Tuning untuk Labirin

> Tuning parameter PD agar robot mengikuti dinding dengan halus di labirin (bukan dinding lurus saja). Catat minimal 8 set parameter dengan hasil di arena.
>
> **Output:** Tabel tuning 8+ percobaan, video perbandingan parameter jelek vs optimal.

### Soal 3 — Dead-End Recovery

> Labirin memiliki 1 dead-end (jalan buntu). Implementasikan:
> - Deteksi dead-end (depan dan kiri/kanan < threshold)
> - U-turn (putar 180°) secara stabil
> - Kembali ke wall following setelah U-turn
>
> **Output:** Kode program, video robot masuk dan keluar dead-end.

### Soal 4 — Dual-Mode: Line + Wall Following

> Arena memiliki zona garis di lantai dan zona dinding. Robot harus otomatis berpindah mode. Implementasikan switching sesuai sensor yang mendeteksi.
>
> **Output:** Video robot berpindah line → wall → line seamlessly.

### Soal 5 — Speed Run (Bonus: Path Learning)

> Lakukan 3 run resmi. Pada run ke-2 dan ke-3, optimasi kecepatan.
>
> **Bonus:** Simpan path (urutan belokan L/R/S/U-turn) ke array. Di run selanjutnya, robot langsung ambil jalur terpendek (skip dead-end).
>
> **Output:** 3 video run + tabel waktu, kode path optimization (bonus).

---

## C. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | Maze solving (left-wall) berhasil | 25 |
| 2 | PD tuning systematic (8+ percobaan) | 15 |
| 3 | Dead-end recovery | 15 |
| 4 | Dual-mode line + wall | 15 |
| 5 | Speed run performance | 15 |
| 6 | Path learning bonus | 5 |
| 7 | Dokumentasi dan analisis | 10 |
| **TOTAL** | | **100** |

---

## D. REFERENSI

1. Choset, H. et al. *Principles of Robot Motion*. MIT Press.
2. Siegwart, R. et al. *Introduction to Autonomous Mobile Robots*. MIT Press.
3. Arduino PID Library: https://github.com/br3ttb/Arduino-PID-Library
