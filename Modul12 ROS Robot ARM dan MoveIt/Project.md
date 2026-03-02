# PROJECT MODUL 12: AUTOMATED SORTING ROBOT ARM

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 12 – ROS Robot ARM dan MoveIt  
**Pengerjaan:** Per Kelompok (Maks. 4 Orang)

---

## A. SOAL CERITA

> Sebuah pabrik elektronik membutuhkan **robot arm sorting station** untuk memilah komponen dari conveyor belt ke 3 bin tujuan berdasarkan ukuran:
> - **Bin A** (kiri): komponen kecil (kubus 2 cm)
> - **Bin B** (tengah): komponen sedang (kubus 4 cm)
> - **Bin C** (kanan): komponen besar (kubus 6 cm)
>
> Robot arm harus mengambil komponen dari posisi pick, menentukan bin tujuan, dan menempatkan objek di bin yang benar — semua secara otonom.

---

## B. SOAL PROJECT

### Soal 1 — URDF Arm + Gripper

> Modifikasi URDF arm dari percobaan agar memiliki **gripper fungsional** (2-finger parallel gripper):
> - Gripper bisa buka/tutup via ros2_control
> - Buat planning group terpisah: `arm` dan `gripper`
>
> **Output:** File URDF, screenshot arm+gripper di RViz2.

### Soal 2 — Gazebo Sorting Scene

> Buat Gazebo world dengan:
> - Robot arm di tengah meja
> - 3 bin di posisi berbeda (kiri, tengah, kanan)
> - Minimal 5 objek kubus (campuran 3 ukuran)
> - Posisi pick di depan arm
>
> **Output:** File .world, screenshot scene.

### Soal 3 — Automated Pick-and-Place

> Buat node Python yang melakukan sorting satu objek:
> 1. Pindah ke pre-grasp
> 2. Turun ke grasp → close gripper → attach object
> 3. Angkat → pindah ke bin tujuan
> 4. Buka gripper → detach
>
> **Ukuran objek** ditentukan dari parameter (hardcoded bin assignment). 
>
> **Output:** Kode Python, video sorting 1 objek.

### Soal 4 — Multi-Object Sorting

> Extend Soal 3 untuk sorting 3+ objek secara berurutan:
> - Objek berikutnya di-spawn atau sudah diletakkan
> - Robot arm beralih ke objek berikutnya setelah selesai
>
> **Output:** Kode Python, video sorting 3 objek ke 3 bin.

### Soal 5 — Safety & Collision Avoidance

> Tambahkan obstacle (tiang) di workspace arm. Robot harus:
> - Planning menghindari tiang
> - Tidak menabrak bin lain saat sorting
> - Tampilkan collision objects di RViz2
>
> **Output:** Kode penambahan collision scene, video sorting yang menghindari obstacle.

---

## C. RUBRIK PENILAIAN (100 POIN)

| No | Kriteria | Bobot |
|----|---------|-------|
| 1 | URDF arm + gripper fungsional | 15 |
| 2 | Gazebo scene (meja, 3 bin, objek) | 15 |
| 3 | Automated pick-and-place 1 objek | 25 |
| 4 | Multi-object sorting (3+) | 25 |
| 5 | Collision avoidance + scene | 10 |
| 6 | Dokumentasi dan analisis | 10 |
| **TOTAL** | | **100** |

---

## D. REFERENSI

1. MoveIt 2 Tutorials: https://moveit.picknik.ai/humble/doc/tutorials/tutorials.html
2. OMPL: https://ompl.kavrakilab.org
3. Siciliano, B. et al. *Robotics: Modelling, Planning and Control*. Springer.
