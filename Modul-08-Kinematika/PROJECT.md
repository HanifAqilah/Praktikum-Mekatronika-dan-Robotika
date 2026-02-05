# PROJECT MODUL 8: ROBOT ARM KINEMATICS & MOTION PLANNING

## 1. DESKRIPSI
Bangun sistem kinematika lengkap untuk robot arm 2-DOF atau 3-DOF, termasuk FK, IK, Jacobian, dan analisis workspace. Implementasi harus dapat menggerakkan robot ke target koordinat dengan error minimal.

---

## 2. SPESIFIKASI
- Ekstrak DH parameter dari CAD atau pengukuran.
- Implementasi FK dan IK (analytical untuk 2-DOF, numerical untuk 3-DOF).
- Hitung Jacobian dan deteksi singularity.
- Visualisasi workspace (2D/3D).
- Validasi hardware dengan set target.

---

## 3. DELIVERABLE
1. Dokumentasi DH parameter + diagram frame.
2. Kode FK & IK (modular, terdokumentasi).
3. Jacobian calculation + peta singularity.
4. Workspace map (gambar/plot).
5. Hasil validasi hardware (tabel error).
6. Laporan analisis (accuracy, singularity, limitation).
7. Demo video (FK/IK → motor move).

---

## 4. KRITERIA PENILAIAN
- Akurasi FK/IK (error posisi).
- Kejelasan dokumentasi DH.
- Kualitas visualisasi workspace.
- Robustness IK (unreachable handling).
- Presentasi demo & laporan.

---

## 5. MILESTONE
- Minggu 1: DH parameter + FK.
- Minggu 2: IK + validasi.
- Minggu 3: Jacobian + workspace.
- Minggu 4: Integrasi hardware + demo.
