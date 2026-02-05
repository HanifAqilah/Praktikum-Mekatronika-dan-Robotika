# JOBSHEET MODUL 8: KINEMATIKA ROBOT (APPLIED)

## 1. TUJUAN PRAKTIKUM
- Menentukan DH parameter dari robot
- Mengimplementasikan forward kinematics (FK)
- Mengimplementasikan inverse kinematics (IK)
- Menghitung Jacobian & singularity
- Membuat peta workspace
- Validasi pada hardware

---

## 2. ALAT & BAHAN
- Robot arm 2-DOF atau 3-DOF
- CAD model / hasil pengukuran link
- Python + NumPy/Matplotlib
- Encoder untuk feedback joint

---

## 3. LANGKAH KERJA

### PERCOBAAN 1: DH PARAMETER EXTRACTION
1. Definisikan frame 0 hingga frame N.
2. Ukur $a_i$, $\alpha_i$, $d_i$, $\theta_i$.
3. Buat tabel DH.

**Data:**
| Joint | $a_i$ (mm) | $\alpha_i$ (deg) | $d_i$ (mm) | Tipe |
|---|---|---|---|---|
| 1 | ___ | ___ | ___ | Revolute |
| 2 | ___ | ___ | ___ | Revolute |
| 3 | ___ | ___ | ___ | Revolute |

**Deliverable:** Tabel DH + diagram frame.

---

### PERCOBAAN 2: FORWARD KINEMATICS
1. Implementasi fungsi FK.
2. Uji posisi home dan 90° pada joint 1.
3. Bandingkan hasil dengan perhitungan manual.

**Data:**
| Sudut (deg) | Posisi (mm) | Validasi |
|---|---|---|
| [0,0,0] | ___ | ✓/✗ |
| [90,0,0] | ___ | ✓/✗ |

**Deliverable:** Kode FK + tabel validasi.

---

### PERCOBAAN 3: INVERSE KINEMATICS
1. Implementasi IK 2-DOF (analytical).
2. Jika 3-DOF, gunakan metode numerik.
3. Verifikasi dengan FK (FK(IK(target))).

**Data:**
| Target (mm) | IK (deg) | FK result (mm) | Error (mm) |
|---|---|---|---|
| [100,50] | ___ | ___ | ___ |
| [150,0] | ___ | ___ | ___ |

**Deliverable:** Kode IK + tabel error.

---

### PERCOBAAN 4: WORKSPACE VISUALIZATION
1. Sampling joint (0–360° atau limit joint).
2. Hitung FK setiap sampel.
3. Plot scatter workspace.

**Deliverable:** Gambar workspace + analisis coverage.

---

### PERCOBAAN 5: JACOBIAN & SINGULARITY
1. Hitung Jacobian numerik.
2. Hitung $\det(J)$ atau manipulability.
3. Tandai konfigurasi singular.

**Data:**
| Sudut (deg) | det(J) | Singularity? |
|---|---|---|
| [45,45] | ___ | ✓/✗ |
| [0,0] | ___ | ✓/✗ |

**Deliverable:** Plot manipulability + daftar singularity.

---

### PERCOBAAN 6: HARDWARE VALIDATION
1. Kirim target posisi.
2. Hitung IK → sudut joint.
3. Bandingkan posisi aktual dengan target.

**Data:**
| Target (mm) | Aktual (mm) | Error (mm) |
|---|---|---|
| [100,50] | ___ | ___ |
| [150,0] | ___ | ___ |

**Deliverable:** Grafik error & analisis.

---

## 4. ANALISIS
- Apakah FK sesuai parameter?
- IK memiliki multi-solution?
- Singularitas berpengaruh ke gerak?
- Error hardware disebabkan backlash/sensor?

---

## 5. KESIMPULAN
Tuliskan ringkasan hasil dan evaluasi performa kinematika robot.
