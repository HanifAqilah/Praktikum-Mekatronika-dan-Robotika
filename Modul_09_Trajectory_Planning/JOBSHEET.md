# JOBSHEET MODUL 9: TRAJECTORY PLANNING & MOTION CONTROL

## 1. TUJUAN PRAKTIKUM
- Implementasi trapezoidal dan S-curve velocity profile
- Trajectory calculation (posisi vs waktu)
- Interpolasi linear dan circular
- Multi-point trajectory
- Verifikasi motion smoothness

---

## 2. ALAT & BAHAN
- Robot arm dengan kinematics (Modul 8)
- Python + NumPy/Matplotlib
- Encoder/ sensor posisi

---

## 3. LANGKAH KERJA

### PERCOBAAN 1: TRAPEZOIDAL PROFILE
1. Input: $s_0$, $s_{target}$, $v_{max}$, $a_{max}$.
2. Hitung $t_a$, $s_a$, $t_v$.
3. Plot posisi, kecepatan, percepatan.

**Data:**
| Parameter | Nilai |
|---|---|
| $v_{max}$ | ___ |
| $a_{max}$ | ___ |
| $t_a$ | ___ |
| $t_v$ | ___ |

**Deliverable:** Grafik profil dan tabel parameter.

---

### PERCOBAAN 2: S-CURVE PROFILE
1. Tentukan $j_{max}$.
2. Implementasi kurva S.
3. Bandingkan jerk dengan trapezoidal.

**Deliverable:** Grafik jerk + perbandingan smoothness.

---

### PERCOBAAN 3: LINEAR INTERPOLATION
1. Tentukan titik A dan B.
2. Gunakan profil kecepatan untuk timing.
3. Plot jalur XY.

**Deliverable:** Plot lintasan linear + kurva waktu.

---

### PERCOBAAN 4: CIRCULAR ARC
1. Tentukan pusat, radius, sudut awal-akhir.
2. Interpolasi posisi pada arc.
3. Terapkan profil kecepatan.

**Deliverable:** Plot lintasan circular.

---

### PERCOBAAN 5: MULTI-POINT TRAJECTORY
1. Definisikan waypoint.
2. Gabungkan segmen dengan blending.
3. Evaluasi waktu eksekusi.

**Data:**
| Waypoint | Posisi | Velocity Reduction |
|---|---|---|
| Start | ___ | - |
| A | ___ | ___ |
| B | ___ | ___ |
| End | ___ | - |

**Deliverable:** Plot lintasan multi-point + estimasi waktu.

---

## 4. ANALISIS
- Profil memenuhi constraint $v_{max}$, $a_{max}$, $j_{max}$?
- S-curve menurunkan jerk?
- Jalur aktual sesuai jalur rencana?

---

## 5. KESIMPULAN
Tuliskan ringkasan performa trajectory planning dan rekomendasi parameter.
