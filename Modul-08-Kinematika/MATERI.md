# MODUL 8: KINEMATIKA ROBOT (APPLIED)

## 1. DEFINISI

### 1.1 Kinematics
**Kinematics** adalah studi tentang gerak tanpa mempertimbangkan gaya. Dalam robotika, kinematics memetakan hubungan antara sudut joint dan posisi/ orientasi end-effector.

### 1.2 Forward Kinematics (FK)
**Forward Kinematics** menghitung posisi dan orientasi end-effector dari sudut joint yang diketahui.

### 1.3 Inverse Kinematics (IK)
**Inverse Kinematics** menghitung sudut joint yang diperlukan untuk mencapai posisi end-effector yang diinginkan.

### 1.4 Denavit–Hartenberg (DH)
**DH parameter** adalah notasi standar untuk mendeskripsikan geometri link dan joint secara sistematis.

---

## 2. KONSEP INTI

### 2.1 DH Parameter
Untuk setiap link, DH parameter terdiri dari:
- $a_i$ : link length
- $\alpha_i$ : link twist
- $d_i$ : joint offset
- $\theta_i$ : joint angle

Tabel DH umum:

| Joint | $a_i$ | $\alpha_i$ | $d_i$ | $\theta_i$ |
|------|------|-----------|------|-----------|
| 1 | $a_1$ | $\alpha_1$ | $d_1$ | $\theta_1$ |
| 2 | $a_2$ | $\alpha_2$ | $d_2$ | $\theta_2$ |
| 3 | $a_3$ | $\alpha_3$ | $d_3$ | $\theta_3$ |

### 2.2 Homogeneous Transformation Matrix
Transformasi antar joint menggunakan matriks homogen 4×4:

$$
T_i^{i-1}=
\begin{bmatrix}
\cos\theta_i & -\sin\theta_i\cos\alpha_i & \sin\theta_i\sin\alpha_i & a_i\cos\theta_i \\
\sin\theta_i & \cos\theta_i\cos\alpha_i & -\cos\theta_i\sin\alpha_i & a_i\sin\theta_i \\
0 & \sin\alpha_i & \cos\alpha_i & d_i \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

### 2.3 Forward Kinematics (FK)
Transformasi total:

$$T_0^n=T_0^1\,T_1^2\,\cdots\,T_{n-1}^n$$

Posisi end-effector:

$$\mathbf{p}=\begin{bmatrix}x\\y\\z\end{bmatrix}=T_0^n[0:3,3]$$

### 2.4 Inverse Kinematics (IK) 2-DOF Planar
Untuk 2-link planar arm ($L_1$, $L_2$):

$$r=\sqrt{x^2+y^2}$$

$$\cos\theta_2=\frac{r^2-L_1^2-L_2^2}{2L_1L_2}$$

$$\theta_2=\arccos(\cos\theta_2)$$

$$\theta_1=\operatorname{atan2}(y,x)-\operatorname{atan2}(L_2\sin\theta_2,\,L_1+L_2\cos\theta_2)$$

Solusi IK memiliki **elbow-up** dan **elbow-down**.

### 2.5 Jacobian & Singularity
Jacobian memetakan kecepatan joint ke kecepatan end-effector:

$$\dot{\mathbf{x}}=J(\mathbf{q})\dot{\mathbf{q}}$$

Singularity terjadi saat rank Jacobian turun:

$$\det(J)\approx 0$$

Manipulability:

$$w(\mathbf{q})=\sqrt{\det(JJ^T)}$$

---

## 3. WORKFLOW IMPLEMENTASI

1. **Ekstrak DH** dari CAD atau pengukuran.
2. **Implement FK** dan validasi dengan posisi known.
3. **Implement IK** (analytical untuk 2-DOF, numerical untuk 3-DOF+).
4. **Hitung Jacobian** dan deteksi singularity.
5. **Plot workspace** dan analisis coverage.
6. **Validasi hardware** dengan target posisi.

---

## 4. CONTOH SEDERHANA 2-DOF

- Home: $\theta_1=0,\theta_2=0$ → posisi $(L_1+L_2, 0)$
- $\theta_1=90^\circ, \theta_2=0$ → posisi $(0, L_1+L_2)$

---

## 5. RINGKASAN

- FK: langsung dari sudut → posisi.
- IK: posisi → sudut, bisa multi-solution.
- Jacobian penting untuk kecepatan dan singularity.
- Workspace & singularity harus dipetakan sebelum aplikasi nyata.
