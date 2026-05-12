# Materi Modul 08: ROS Robot ARM Dobot Magician

## Identitas

- Program studi: Sarjana Terapan Teknologi Rekayasa Otomasi
- Mata kuliah: Praktikum Mekatronika, 2 SKS, 5 jam 40 menit praktikum
- Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.
- Modul: ROS Robot ARM berbasis Dobot Magician
- Platform: Ubuntu 22.04, ROS 2 Humble, Python, RViz, RQT, Dobot Magician

## Capaian Pembelajaran

Setelah menyelesaikan modul ini, praktikan mampu:

1. Menjelaskan konsep robot manipulator, derajat kebebasan, frame koordinat, dan transformasi spasial.
2. Menurunkan model kinematika maju dan kinematika balik manipulator lengan robot.
3. Menjelaskan arsitektur ROS 2 untuk kendali robot arm: node, topic, service, action, launch, parameter, dan TF.
4. Menjalankan stack ROS 2 Dobot Magician dari instalasi sampai gerak point-to-point.
5. Mengendalikan end effector berupa gripper atau suction cup.
6. Membaca joint state, pose TCP, diagnostik alarm, dan validasi trajectory.
7. Merancang program pick and place sederhana sampai project robotika tingkat lanjut.
8. Menganalisis keselamatan, akurasi, repeatability, workspace, dan performa lintasan.

## 1. Pengantar Robot Manipulator

Robot manipulator adalah sistem mekanik berantai yang terdiri dari link, joint, aktuator, sensor, pengendali, dan end effector. Robot arm digunakan untuk pemindahan objek, perakitan, inspeksi, pengelasan, pelukisan, sorting, dispensing, dan edukasi robotika.

Komponen utama:

- Link: batang atau struktur mekanik yang menghubungkan joint.
- Joint: sambungan gerak, umumnya revolute atau prismatic.
- Actuator: motor stepper, servo, DC, pneumatic, atau hydraulic.
- Sensor: encoder, limit switch, current sensor, force sensor, kamera.
- Controller: komputer, microcontroller, PLC, atau embedded controller.
- End effector: gripper, suction cup, pen, nozzle, camera mount.
- Workspace: ruang yang dapat dijangkau TCP.
- TCP: Tool Center Point, titik kerja utama di ujung tool.

## 2. Dobot Magician dan Konfigurasi Dasar

Dobot Magician adalah robot edukasi 4-DOF dengan gerak utama pada basis, lengan atas, lengan bawah, dan rotasi tool. Dalam stack ROS 2, Dobot dikendalikan melalui serial USB, state updater, action server PTP, service end effector, homing service, trajectory validator, RViz, dan RQT.

Paket penting pada workspace:

| Paket | Fungsi |
|---|---|
| `dobot_bringup` | Launch utama dan konfigurasi startup |
| `dobot_driver` | Komunikasi low-level serial ke Dobot |
| `dobot_state_updater` | Membaca joint angle dan TCP pose |
| `dobot_motion` | Action server PointToPoint |
| `dobot_kinematics` | Validasi workspace dan inverse kinematics |
| `dobot_homing` | Prosedur homing |
| `dobot_end_effector` | Service gripper dan suction cup |
| `dobot_description` | URDF, mesh, visualisasi RViz |
| `dobot_control_panel` | Plugin RQT kontrol manual |
| `dobot_demos` | Contoh program Python |
| `dobot_msgs` | Definisi message, service, action |

## 3. Derajat Kebebasan

Derajat kebebasan atau degree of freedom adalah jumlah variabel independen untuk menentukan konfigurasi robot.

Rumus umum mobilitas mekanisme spasial Gruebler-Kutzbach:

```text
M = 6(n - 1 - j) + Σ f_i
```

Keterangan:

- `M`: mobilitas atau DOF.
- `n`: jumlah link termasuk ground.
- `j`: jumlah joint.
- `f_i`: DOF joint ke-i.

Untuk mekanisme planar:

```text
M = 3(n - 1 - j) + Σ f_i
```

Robot arm serial revolute 4 joint biasanya memiliki:

```text
q = [q1, q2, q3, q4]^T
DOF = 4
```

Konfigurasi robot ditentukan oleh vektor joint:

```text
q(t) = [θ1(t), θ2(t), θ3(t), θ4(t)]^T
```

Kecepatan joint:

```text
q_dot(t) = dq(t)/dt
```

Percepatan joint:

```text
q_ddot(t) = d²q(t)/dt²
```

## 4. Sistem Koordinat dan Frame

Robot arm bekerja memakai beberapa frame:

- World frame: kerangka global lingkungan.
- Base frame: kerangka pada dasar robot.
- Joint frame: kerangka pada setiap joint.
- Tool frame: kerangka pada tool.
- TCP frame: titik pusat kerja tool.
- Camera frame: kerangka kamera jika digunakan vision.

Vektor posisi titik dalam 3D:

```text
p = [x, y, z]^T
```

Orientasi dapat ditulis memakai Euler angle, rotation matrix, axis-angle, atau quaternion.

## 5. Rotasi 3D

Rotasi terhadap sumbu X:

```text
R_x(α) = [[1, 0, 0],
          [0, cosα, -sinα],
          [0, sinα, cosα]]
```

Rotasi terhadap sumbu Y:

```text
R_y(β) = [[cosβ, 0, sinβ],
          [0, 1, 0],
          [-sinβ, 0, cosβ]]
```

Rotasi terhadap sumbu Z:

```text
R_z(γ) = [[cosγ, -sinγ, 0],
          [sinγ, cosγ, 0],
          [0, 0, 1]]
```

Komposisi rotasi ZYX:

```text
R = R_z(ψ) R_y(θ) R_x(φ)
```

Sifat rotation matrix:

```text
R^T R = I
R^-1 = R^T
det(R) = 1
```

## 6. Transformasi Homogen

Transformasi homogen menggabungkan rotasi dan translasi:

```text
T = [[R, p],
     [0 0 0, 1]]
```

Dengan:

```text
R ∈ R^{3x3}
p = [x, y, z]^T
T ∈ SE(3)
```

Transformasi titik:

```text
p_B = R_AB p_A + t_AB
```

Dalam koordinat homogen:

```text
[p_B; 1] = T_AB [p_A; 1]
```

Invers transformasi:

```text
T^-1 = [[R^T, -R^T p],
        [0 0 0, 1]]
```

Komposisi transformasi serial:

```text
T_0n = T_01 T_12 T_23 ... T_(n-1)n
```

## 7. Denavit-Hartenberg

Metode Denavit-Hartenberg digunakan untuk membangun model kinematika robot serial. Empat parameter DH:

| Parameter | Makna |
|---|---|
| `a_i` | panjang link, jarak sepanjang x_i |
| `α_i` | twist link, rotasi terhadap x_i |
| `d_i` | offset link, jarak sepanjang z_{i-1} |
| `θ_i` | sudut joint, rotasi terhadap z_{i-1} |

Matriks DH standar:

```text
A_i = Rot_z(θ_i) Trans_z(d_i) Trans_x(a_i) Rot_x(α_i)
```

```text
A_i = [[cosθ_i, -sinθ_i cosα_i,  sinθ_i sinα_i, a_i cosθ_i],
       [sinθ_i,  cosθ_i cosα_i, -cosθ_i sinα_i, a_i sinθ_i],
       [0,       sinα_i,          cosα_i,         d_i],
       [0,       0,               0,              1]]
```

Kinematika maju:

```text
T_0n(q) = A_1(q_1) A_2(q_2) ... A_n(q_n)
```

Pose TCP:

```text
x = f(q)
```

## 8. Kinematika Maju Dobot 4-DOF Konseptual

Dobot Magician dapat dimodelkan sebagai manipulator 4-DOF dengan basis yaw dan mekanisme lengan planar untuk menentukan radius dan tinggi TCP.

Misalkan:

```text
q = [θ1, θ2, θ3, θ4]^T
L1 = offset tinggi basis
L2 = panjang lengan atas
L3 = panjang lengan bawah
L4 = offset tool
```

Model planar sederhana:

```text
r = L2 cosθ2 + L3 cos(θ2 + θ3) + L4 cos(θ2 + θ3 + θ4)
z = L1 + L2 sinθ2 + L3 sin(θ2 + θ3) + L4 sin(θ2 + θ3 + θ4)
x = r cosθ1
y = r sinθ1
```

Orientasi tool terhadap bidang vertikal:

```text
φ = θ2 + θ3 + θ4
```

Untuk robot yang menjaga tool tetap horizontal:

```text
θ4 = φ_target - θ2 - θ3
```

Jika `φ_target = 0`:

```text
θ4 = -θ2 - θ3
```

## 9. Kinematika Balik Dasar

Kinematika balik mencari joint dari pose target:

```text
q = f^-1(x)
```

Untuk target Cartesian:

```text
p = [x, y, z]^T
```

Sudut basis:

```text
θ1 = atan2(y, x)
```

Radius horizontal:

```text
r = sqrt(x² + y²)
```

Jika offset tool diperhitungkan:

```text
r_w = r - L4 cosφ
z_w = z - L1 - L4 sinφ
```

Jarak wrist:

```text
D = (r_w² + z_w² - L2² - L3²) / (2 L2 L3)
```

Syarat reachable:

```text
-1 ≤ D ≤ 1
```

Solusi elbow:

```text
θ3 = atan2(±sqrt(1 - D²), D)
```

Sudut shoulder:

```text
θ2 = atan2(z_w, r_w) - atan2(L3 sinθ3, L2 + L3 cosθ3)
```

Sudut tool:

```text
θ4 = φ - θ2 - θ3
```

Dua solusi utama:

- Elbow up: tanda positif.
- Elbow down: tanda negatif.

## 10. Workspace

Workspace adalah himpunan posisi yang dapat dijangkau TCP:

```text
W = {p ∈ R³ | ∃q, p = f(q), q_min ≤ q ≤ q_max}
```

Untuk lengan planar dua link tanpa offset:

```text
|L2 - L3| ≤ ρ ≤ L2 + L3
```

Dengan:

```text
ρ = sqrt(r² + (z - L1)²)
```

Batas joint:

```text
q_min ≤ q ≤ q_max
```

Batas kecepatan:

```text
|q_dot_i| ≤ q_dot_i,max
```

Batas percepatan:

```text
|q_ddot_i| ≤ q_ddot_i,max
```

Target tidak boleh menyebabkan self-collision, melanggar limit joint, atau keluar workspace.

## 11. Jacobian

Jacobian menghubungkan kecepatan joint dengan kecepatan TCP:

```text
x_dot = J(q) q_dot
```

Untuk robot 3D:

```text
v = J_v(q) q_dot
ω = J_ω(q) q_dot
```

Jacobian penuh:

```text
[V] = [v; ω] = J(q) q_dot
```

Kolom Jacobian untuk joint revolute:

```text
J_v_i = z_{i-1} × (p_n - p_{i-1})
J_ω_i = z_{i-1}
```

Kolom Jacobian untuk joint prismatic:

```text
J_v_i = z_{i-1}
J_ω_i = [0, 0, 0]^T
```

Singularitas terjadi saat rank Jacobian turun:

```text
rank(J) < min(m, n)
```

Indikator manipulability Yoshikawa:

```text
w(q) = sqrt(det(J J^T))
```

Jika `w(q) ≈ 0`, robot dekat singularitas.

## 12. Statika dan Torsi Joint

Hubungan gaya TCP dengan torsi joint:

```text
τ = J(q)^T F
```

Dengan:

```text
τ = [τ1, τ2, ..., τn]^T
F = [Fx, Fy, Fz, Mx, My, Mz]^T
```

Energi virtual:

```text
δW = F^T δx = τ^T δq
```

Karena:

```text
δx = J δq
```

Maka:

```text
τ = J^T F
```

Torsi beban sederhana pada link:

```text
τ = m g l sinθ
```

Untuk beberapa massa:

```text
τ_i = Σ_k m_k g r_{k,i} sinθ_{k,i}
```

## 13. Dinamika Robot

Model dinamika manipulator:

```text
M(q) q_ddot + C(q, q_dot) q_dot + g(q) + τ_f = τ
```

Keterangan:

- `M(q)`: matriks inersia.
- `C(q, q_dot)`: gaya Coriolis dan sentrifugal.
- `g(q)`: gravitasi.
- `τ_f`: gesekan.
- `τ`: torsi aktuator.

Energi kinetik:

```text
K = 1/2 q_dot^T M(q) q_dot
```

Energi potensial:

```text
P = Σ m_i g h_i(q)
```

Lagrangian:

```text
L = K - P
```

Persamaan Euler-Lagrange:

```text
d/dt(∂L/∂q_dot_i) - ∂L/∂q_i = τ_i
```

## 14. Perencanaan Lintasan

Trajectory planning menentukan posisi, kecepatan, dan percepatan terhadap waktu.

Interpolasi linear posisi:

```text
p(t) = p0 + s(t)(pf - p0)
```

Parameter normalisasi:

```text
s = (t - t0) / (tf - t0), 0 ≤ s ≤ 1
```

Profil cubic polynomial:

```text
q(t) = a0 + a1 t + a2 t² + a3 t³
```

Dengan syarat:

```text
q(0)=q0, q(T)=qf, q_dot(0)=0, q_dot(T)=0
```

Koefisien:

```text
a0 = q0
a1 = 0
a2 = 3(qf - q0)/T²
a3 = -2(qf - q0)/T³
```

Profil quintic polynomial:

```text
q(t) = a0 + a1t + a2t² + a3t³ + a4t⁴ + a5t⁵
```

Untuk kecepatan dan percepatan awal-akhir nol:

```text
s(t)=10(t/T)^3 - 15(t/T)^4 + 6(t/T)^5
```

```text
q(t)=q0 + s(t)(qf-q0)
```

Kecepatan:

```text
q_dot(t)=s_dot(t)(qf-q0)
```

Percepatan:

```text
q_ddot(t)=s_ddot(t)(qf-q0)
```

Profil trapezoidal velocity:

```text
T = T_acc + T_const + T_dec
v_max = Δq / (T - T_acc)
```

Jika segitiga tanpa fase konstan:

```text
T = 2 sqrt(Δq/a_max)
v_peak = sqrt(Δq a_max)
```

## 15. Kendali Robot

Kendali posisi joint sederhana:

```text
e(t) = q_d(t) - q(t)
```

Kontrol P:

```text
u(t) = K_p e(t)
```

Kontrol PD:

```text
u(t) = K_p e(t) + K_d e_dot(t)
```

Kontrol PID:

```text
u(t) = K_p e(t) + K_i ∫e(t)dt + K_d de(t)/dt
```

Model closed-loop orde dua:

```text
s² + 2ζω_n s + ω_n² = 0
```

Hubungan gain sederhana:

```text
K_p = ω_n²
K_d = 2ζω_n
```

Error RMS tracking:

```text
RMSE = sqrt((1/N) Σ_i ||p_i - p̂_i||²)
```

Error posisi akhir:

```text
e_f = ||p_target - p_actual||
```

## 16. ROS 2 untuk Robot Arm

ROS 2 adalah middleware robotika yang menyediakan komunikasi antar proses secara modular.

Konsep utama:

- Node: proses komputasi mandiri.
- Topic: komunikasi publish-subscribe.
- Service: request-response.
- Action: goal-feedback-result untuk proses lama.
- Parameter: konfigurasi runtime node.
- Launch: menjalankan banyak node.
- Workspace: direktori kerja ROS berbasis colcon.
- Package: unit kode, pesan, launch, resource.
- TF: transformasi antar frame.

Komunikasi topic:

```text
publisher -> topic -> subscriber
```

Komunikasi service:

```text
client -> request -> server -> response -> client
```

Komunikasi action:

```text
client -> goal -> server
server -> feedback -> client
server -> result -> client
```

## 17. Topic, Service, dan Action Dobot

Topic penting:

```text
/joint_states
/dobot_TCP
/dobot_pose_raw
```

Service penting:

```text
/dobot_homing_service
/dobot_gripper_service
/dobot_suction_cup_service
```

Action penting:

```text
/PTP_action
```

Struktur goal PointToPoint:

```text
motion_type: 1, 2, 4, atau 5
target_pose: [x, y, z, r] atau [j1, j2, j3, j4]
velocity_ratio: 0.0 sampai 1.0
acceleration_ratio: 0.0 sampai 1.0
```

Makna motion type:

| Nilai | Makna |
|---|---|
| 1 | Joint interpolated, target Cartesian |
| 2 | Linear motion, target Cartesian |
| 4 | Joint interpolated, target joint |
| 5 | Linear motion, target joint |

Contoh goal:

```bash
ros2 action send_goal /PTP_action dobot_msgs/action/PointToPoint "{motion_type: 1, target_pose: [200.0, 0.0, 100.0, 0.0], velocity_ratio: 0.5, acceleration_ratio: 0.3}" --feedback
```

## 18. Homing

Homing adalah prosedur untuk menentukan referensi awal posisi robot. Dobot memakai encoder incremental, sehingga setelah power on robot perlu homing agar posisi aktual diketahui.

Command:

```bash
ros2 service call /dobot_homing_service dobot_msgs/srv/ExecuteHomingProcedure
```

Parameter homing:

```bash
ros2 param get /dobot_homing_srv homing_position
ros2 param set /dobot_homing_srv homing_position [150.0,0.0,100.0,0.0]
```

Aturan homing:

1. Area kerja kosong.
2. Tidak ada program lain yang mengendalikan robot.
3. Kecepatan rendah.
4. Emergency stop siap.
5. Operator berada di luar area gerak.

## 19. End Effector

Gripper digunakan untuk menjepit objek. Suction cup digunakan untuk mengangkat objek datar atau ringan memakai tekanan vakum.

Gaya cengkeram minimum:

```text
F_grip ≥ S_f m(g + a)
```

Dengan:

- `S_f`: faktor keamanan.
- `m`: massa objek.
- `g`: gravitasi.
- `a`: percepatan gerak.

Gaya hisap suction cup:

```text
F_suction = ΔP A
```

Dengan:

```text
A = πr²
```

Syarat angkat aman:

```text
ΔP A ≥ S_f m(g + a)
```

Contoh gripper:

```bash
ros2 service call /dobot_gripper_service dobot_msgs/srv/GripperControl "{gripper_state: 'open', keep_compressor_running: true}"
```

Contoh suction cup:

```bash
ros2 service call /dobot_suction_cup_service dobot_msgs/srv/SuctionCupControl "{enable_suction: true, keep_compressor_running: true}"
```

## 20. Akurasi, Presisi, dan Repeatability

Akurasi adalah kedekatan posisi aktual terhadap target. Presisi adalah konsistensi hasil pengukuran. Repeatability adalah kemampuan robot kembali ke titik yang sama berulang-ulang.

Error posisi:

```text
e_i = sqrt((x_i-x_t)² + (y_i-y_t)² + (z_i-z_t)²)
```

Mean error:

```text
ē = (1/N) Σ e_i
```

Standar deviasi:

```text
σ = sqrt((1/(N-1)) Σ(e_i - ē)²)
```

Repeatability ISO sederhana:

```text
R = ē + 3σ
```

Persen keberhasilan pick and place:

```text
Success Rate = (jumlah berhasil / jumlah percobaan) × 100%
```

Throughput:

```text
Throughput = jumlah objek / waktu operasi
```

Cycle time:

```text
T_cycle = t_place - t_pick_start
```

## 21. Keselamatan Robot Arm

Bahaya utama robot arm:

- Tertabrak link atau end effector.
- Terjepit gripper.
- Objek terlempar atau jatuh.
- Kabel tertarik.
- Robot bergerak di luar perkiraan.
- Overcurrent, alarm, atau komunikasi serial gagal.

Prinsip keselamatan:

1. Cek area kerja sebelum power on.
2. Gunakan kecepatan dan percepatan rendah saat belajar.
3. Jangan menyentuh robot saat program berjalan.
4. Siapkan emergency stop atau tombol power.
5. Hindari target di luar workspace.
6. Pastikan objek tidak melebihi payload.
7. Jalankan homing sebelum eksperimen.
8. Jangan menjalankan RQT manual dan script otomatis bersamaan.
9. Dokumentasikan alarm dan kegagalan.
10. Matikan suction/gripper sebelum melepas objek.

## 22. Pemrograman Praktik ROS 2 Dobot

Alur program robot arm:

1. Inisialisasi node ROS 2.
2. Buat action client ke `/PTP_action`.
3. Tunggu action server tersedia.
4. Kirim target pose.
5. Pantau feedback posisi.
6. Tunggu result.
7. Aktifkan atau nonaktifkan end effector melalui service.
8. Ulangi sequence sesuai proses.
9. Tangani error, timeout, dan cancel.
10. Shutdown node.

Pseudocode pick and place:

```text
home()
move(above_pick)
move(pick)
suction_on()
move(above_pick)
move(above_place)
move(place)
suction_off()
move(above_place)
move(home_pose)
```

## 23. Tingkat Lanjut

Topik advanced yang relevan:

- Multi-point trajectory dengan blending.
- Workspace mapping memakai grid sampling.
- Integrasi kamera untuk visual servoing.
- Transformasi pixel ke robot coordinate.
- Kalibrasi hand-eye.
- Collision avoidance.
- Digital twin di RViz atau simulator.
- State machine untuk otomasi proses.
- Behavior tree untuk task planning.
- Logging data joint dan TCP untuk analisis.
- Estimasi cycle time dan optimasi lintasan.
- Multi-robot coordination.

Transformasi kamera ke robot:

```text
p_base = T_base_camera p_camera
```

Model pinhole camera:

```text
u = fx X/Z + cx
v = fy Y/Z + cy
```

Dari pixel dan depth:

```text
X = (u - cx)Z/fx
Y = (v - cy)Z/fy
Z = depth
```

Hand-eye calibration:

```text
A_i X = X B_i
```

Dengan `X` adalah transformasi kamera terhadap tool atau base.

## 24. Troubleshooting

| Gejala | Penyebab | Solusi |
|---|---|---|
| Dobot disconnected | USB/power tidak terhubung | Cek kabel, power, `lsusb` |
| Permission denied serial | User belum masuk dialout | `sudo usermod -a -G dialout $USER`, logout/login |
| Launch gagal MAGICIAN_TOOL | Env var belum diset | `export MAGICIAN_TOOL=suction_cup` |
| Action rejected | Target di luar workspace | Ubah target, cek limit joint |
| RQT plugin tidak muncul | Plugin belum ditemukan | `rqt --force-discover` |
| Robot tidak presisi | Belum homing atau beban berlebih | Homing, kurangi beban, ulangi pengukuran |
| Service tidak ada | Stack belum running | Cek `ros2 node list` dan launch ulang |
| Gerak berbahaya | Kecepatan terlalu tinggi | Turunkan `velocity_ratio` dan `acceleration_ratio` |

## 25. Rekomendasi Buku

1. John J. Craig, *Introduction to Robotics: Mechanics and Control*, Pearson.
2. Bruno Siciliano, Lorenzo Sciavicco, Luigi Villani, Giuseppe Oriolo, *Robotics: Modelling, Planning and Control*, Springer.
3. Mark W. Spong, Seth Hutchinson, M. Vidyasagar, *Robot Modeling and Control*, Wiley.
4. Kevin M. Lynch, Frank C. Park, *Modern Robotics: Mechanics, Planning, and Control*, Cambridge University Press.
5. Roland Siegwart, Illah Nourbakhsh, Davide Scaramuzza, *Introduction to Autonomous Mobile Robots*, MIT Press.
6. Sebastian Thrun, Wolfram Burgard, Dieter Fox, *Probabilistic Robotics*, MIT Press.
7. Peter Corke, *Robotics, Vision and Control*, Springer.
8. Richard Szeliski, *Computer Vision: Algorithms and Applications*, Springer.
9. Steven M. LaValle, *Planning Algorithms*, Cambridge University Press.
10. Morgan Quigley, Brian Gerkey, William D. Smart, *Programming Robots with ROS*, O'Reilly.
11. Francisco Martín Rico, *A Concise Introduction to Robot Programming with ROS2*, CRC Press.
12. Lentin Joseph, *ROS Robotics Projects*, Packt.
13. Wyatt Newman, *A Systematic Approach to Learning Robot Programming with ROS*, CRC Press.
14. Thomas Bräunl, *Embedded Robotics*, Springer.
15. Karl Johan Åström, Richard M. Murray, *Feedback Systems*, Princeton University Press.

## 26. Rekomendasi Paper dan Standar

1. Denavit, J. and Hartenberg, R. S., 1955, *A Kinematic Notation for Lower-Pair Mechanisms Based on Matrices*.
2. Whitney, D. E., 1969, *Resolved Motion Rate Control of Manipulators and Human Prostheses*.
3. Paul, R. P., 1981, *Robot Manipulators: Mathematics, Programming, and Control*.
4. Khatib, O., 1986, *Real-Time Obstacle Avoidance for Manipulators and Mobile Robots*.
5. Yoshikawa, T., 1985, *Manipulability of Robotic Mechanisms*.
6. Craig, J. J. and Raibert, M. H., 1979, *A Systematic Method of Hybrid Position/Force Control of a Manipulator*.
7. Siciliano, B. and Khatib, O., 2016, *Springer Handbook of Robotics*.
8. Quigley, M. et al., 2009, *ROS: an Open-Source Robot Operating System*.
9. Macenski, S. et al., 2022, *Robot Operating System 2: Design, Architecture, and Uses in the Wild*.
10. Coleman, D. et al., 2014, *Reducing the Barrier to Entry of Complex Robotic Software: a MoveIt! Case Study*.
11. Tsai, R. Y. and Lenz, R. K., 1989, *A New Technique for Fully Autonomous and Efficient 3D Robotics Hand/Eye Calibration*.
12. ISO 9283, *Manipulating Industrial Robots - Performance Criteria and Related Test Methods*.
13. ISO 10218-1/2, *Robots and Robotic Devices - Safety Requirements for Industrial Robots*.
14. ISO/TS 15066, *Collaborative Robots Safety Requirements*.
15. Corke, P., 1996, *A Robotics Toolbox for MATLAB*.

## 27. Ringkasan Inti

Robot arm membutuhkan pemahaman mekanika, transformasi, kinematika, trajectory, kontrol, ROS 2, keselamatan, dan validasi eksperimen. Pada modul ini praktikan memulai dari teori frame dan DH, lalu menjalankan Dobot Magician melalui ROS 2, melakukan homing, membaca state, mengirim action PTP, mengendalikan end effector, membuat pick and place, dan mengevaluasi performa dengan metrik error, repeatability, success rate, serta cycle time.
