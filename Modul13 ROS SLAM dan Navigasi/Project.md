# PROJECT MODUL 11: AUTONOMOUS EXPLORATION AND NAVIGATION

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 11 – ROS SLAM dan Navigasi  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Tim robotika dipercaya membangun **robot inspeksi gedung** pasca bencana. Robot harus:
> 1. **Memetakan** gedung yang tidak diketahui layout-nya (exploration)
> 2. **Menyimpan** peta sebagai referensi tim rescue
> 3. **Bernavigasi** otonom ke 5 titik evakuasi (waypoint) yang ditentukan setelah peta jadi
> 4. Memberikan **laporan** posisi saat ini, progress navigasi, dan peta final
>
> Arena: Gazebo world kompleks dengan minimal 4 ruangan, 2 koridor, dan rintangan statis.

---

## B. SOAL PROJECT

### Soal 1 — World Building

> Buat Gazebo world yang merepresentasikan gedung:
> - Minimal 4 ruangan terpisah
> - 2 koridor penghubung
> - 5 rintangan statis (meja, lemari, kursi)
> - 1 area outdoor (opsional)
>
> **Output:** File .world, screenshot top-view.

### Soal 2 — Full SLAM Exploration

> Lakukan SLAM dan petakan seluruh gedung:
> - Gunakan teleop (manual) atau frontier exploration (otonom, bonus)
> - Pastikan seluruh area termapping (tidak ada area abu-abu besar)
> - Simpan peta final
>
> **Output:** Peta (PGM+YAML), video proses SLAM (fast-forward), screenshot peta final.

### Soal 3 — Multi-Waypoint Navigation

> Setelah peta jadi, navigasi otonom ke 5 titik evakuasi:
> - Gunakan `nav2_simple_commander`
> - Tampilkan progress (waypoint ke-n, distance remaining)
> - Jika navigasi gagal (stuck), lakukan recovery + retry
>
> **Output:** Kode Python, video navigasi 5 waypoint.

### Soal 4 — Dynamic Obstacle Handling

> Saat robot bernavigasi, tambahkan rintangan baru di Gazebo (drag objek ke path). Robot harus:
> - Mendeteksi rintangan baru via local costmap
> - Replan path secara otomatis
>
> **Output:** Video robot replanning saat ada rintangan baru.

### Soal 5 — Mission Report

> Buat node `mission_reporter.py` yang publish laporan ke topic `/mission_report`:
> - Posisi robot saat ini (x, y, θ)
> - Waypoint saat ini / total (misal 3/5)
> - Status: navigating / reached / failed / all_complete
> - Total jarak tempuh (hitung dari odometry)
>
> **Output:** Kode, screenshot terminal report.

---

## C. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | Gazebo world (4 ruangan + koridor) | 15 |
| 2 | Full SLAM (peta lengkap) | 20 |
| 3 | Multi-waypoint navigation (5 waypoints) | 25 |
| 4 | Dynamic obstacle handling | 15 |
| 5 | Mission report logger | 10 |
| 6 | Dokumentasi dan analisis | 15 |
| **TOTAL** | | **100** |

---

## D. REFERENSI

1. Thrun, S. et al. *Probabilistic Robotics*. MIT Press.
2. Nav2 Docs: https://docs.nav2.org
3. SLAM Toolbox: https://github.com/SteveMacenski/slam_toolbox
