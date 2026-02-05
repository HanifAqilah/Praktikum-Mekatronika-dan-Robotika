# MODUL 9: TRAJECTORY PLANNING & MOTION CONTROL

## 1. DEFINISI

- **Trajectory**: posisi, kecepatan, percepatan sebagai fungsi waktu.
- **Path**: geometri lintasan (tanpa waktu).
- **Velocity profile**: aturan perubahan kecepatan terhadap waktu.
- **Jerk**: laju perubahan percepatan.

---

## 2. KONSEP INTI

### 2.1 Trapezoidal Velocity Profile
Tiga fase: akselerasi → kecepatan konstan → deselerasi.

Waktu akselerasi:

$$t_a=\frac{v_{max}}{a_{max}}$$

Jarak akselerasi:

$$s_a=\frac{1}{2}a_{max}t_a^2$$

Jika $2s_a > d$, maka profil menjadi **triangular**:

$$t_a=\sqrt{\frac{d}{a_{max}}}$$

### 2.2 S-Curve Profile
Profil dengan **jerk terbatas**, menghasilkan transisi percepatan halus.

Keunggulan:
- Getaran lebih kecil
- Presisi lebih baik

### 2.3 Path vs Trajectory
- **Path**: urutan titik posisi.
- **Trajectory**: path + timing (kecepatan, percepatan, jerk).

### 2.4 Interpolasi Jalur
**Linear**:

$$\mathbf{p}(t)=\mathbf{p}_0+\frac{t}{T}(\mathbf{p}_1-\mathbf{p}_0)$$

**Circular**:

$$x(\theta)=c_x+r\cos\theta,\quad y(\theta)=c_y+r\sin\theta$$

### 2.5 Constraint
- $v \le v_{max}$
- $a \le a_{max}$
- $j \le j_{max}$

---

## 3. PRAKTIKUM KUNCI
- Implementasi trapezoidal profile
- Implementasi S-curve profile
- Linear & circular interpolation
- Multi-point trajectory dengan blending

---

## 4. RINGKASAN
- Trapezoidal sederhana dan cepat.
- S-curve lebih halus untuk hardware nyata.
- Trajectory planning menggabungkan path dan timing.
- Constraint wajib dipenuhi untuk menghindari overshoot dan kerusakan mekanik.
