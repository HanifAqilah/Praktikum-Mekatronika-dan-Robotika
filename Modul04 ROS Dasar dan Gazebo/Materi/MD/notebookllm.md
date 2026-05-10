# NotebookLM Prompts — Modul 04: ROS2 Dasar dan Simulasi Gazebo

---

## PROMPT 1 — Slide 1-15 (Fondasi ROS2 + Gazebo + Percobaan 1-5)

Buat 15 slide presentasi akademik Modul 04: ROS2 Dasar dan Simulasi Gazebo menggunakan ROS2 Humble Hawksbill dan Gazebo Classic 11. Tiap slide informatif, sertakan perintah terminal, kode Python/XML, dan penjelasan konsep lengkap.

**Slide 1** — Judul Modul 04: ROS2 (Robot Operating System 2) adalah framework open-source untuk pengembangan perangkat lunak robot. Perbedaan ROS1 vs ROS2: ROS2 menggunakan DDS (Data Distribution Service) sebagai middleware, mendukung real-time, multi-robot, keamanan, dan tidak bergantung pada ROS Master. Arsitektur: Node → Topic/Service/Action → DDS middleware → aplikasi robot.

**Slide 2** — Konsep Dasar ROS2: (1) Node: unit komputasi tunggal, satu proses. (2) Topic: komunikasi publisher-subscriber async. (3) Service: request-response sync. (4) Action: goal-feedback-result untuk task panjang. (5) Parameter: konfigurasi runtime. Perintah dasar: "ros2 node list", "ros2 topic list", "ros2 topic echo /topik", "ros2 service list", "ros2 param list".

**Slide 3** — Package ROS2 dan Launch File Python: Struktur package ament_cmake: CMakeLists.txt, package.xml, folder launch/, urdf/, worlds/, config/, scripts/. Launch file Python: fungsi generate_launch_description() mengembalikan LaunchDescription([...]). TimerAction untuk delay. Build: "colcon build --packages-select gazebo_praktikum" dari root workspace, lalu "source install/setup.bash".

**Slide 4** — URDF dan XACRO: URDF (Unified Robot Description Format) XML mendefinisikan link (geometry, inertia, collision, visual) dan joint (revolute, prismatic, fixed, continuous). XACRO (XML Macros) ekstensi URDF dengan macro dan property. Konversi: "xacro robot.xacro > robot.urdf". robot_state_publisher memproses robot_description dan publish TF frames sesuai JointState.

**Slide 5** — SDF World Format dan Gazebo Classic: SDF mendefinisikan lingkungan simulasi: physics (gravity, max_step_size), model (geometry, collision, visual, plugin), light. File .world berformat SDF. Gazebo Classic 11 stabil untuk ROS2 Humble. Perbedaan SDF vs URDF: SDF lebih lengkap untuk simulasi fisika dan sensor, URDF untuk deskripsi robot di ekosistem ROS.

**Slide 6** — Gazebo Plugin Roda Diferensial: libgazebo_ros_diff_drive.so subscribe /cmd_vel (geometry_msgs/Twist) untuk perintah kecepatan robot. Publish /odom (nav_msgs/Odometry) dan TF odom→base_footprint. Parameter XACRO: wheel_separation (jarak antar roda), wheel_radius (jari-jari), publish_odom_tf true/false. Plugin disisipkan dalam tag <gazebo> pada XACRO robot.

**Slide 7** — Gazebo Plugin Sensor Kamera dan LIDAR: libgazebo_ros_camera.so publish /camera/image_raw (sensor_msgs/Image) dan /camera/camera_info. libgazebo_ros_ray_sensor.so publish /scan (sensor_msgs/LaserScan) dengan parameter: min_range, max_range, samples (360 untuk 360 derajat), update_rate Hz. Sensor dikonfigurasi dalam tag <sensor> di XACRO. Keduanya terlihat di RViz2.

**Slide 8** — TF2 Frame Tree: TF2 mengelola transformasi koordinat antar frame waktu nyata. Frame tipikal: map → odom → base_footprint → base_link → sensor_frame. robot_state_publisher publish berdasarkan URDF + JointState. Perintah debug: "ros2 run tf2_tools view_frames" (buat PDF tree), "ros2 run tf2_ros tf2_echo odom base_link" (cek transform dua frame). Masalah TF paling umum: frame not found.

**Slide 9** — Percobaan 1 – Empty World: Menjalankan Gazebo dengan dunia kosong. Perintah: "ros2 launch gazebo_praktikum percobaan1_empty_world.launch.py". Argumen: gui:=true, verbose:=false, paused:=false. Terminal dibutuhkan: 1. Tujuan: kenali antarmuka Gazebo (Play/Pause, Insert, Properties), tambahkan objek primitif manual lewat Insert, amati simulasi fisika dasar gravitasi dan tumbukan.

**Slide 10** — Percobaan 2 – Shapes World: Dunia berisi box, cylinder, sphere dengan fisika berbeda. Perintah: "ros2 launch gazebo_praktikum percobaan2_shapes.launch.py". Terminal: 1. Objek dikonfigurasi dengan mass, inertia, friction, dan damping berbeda dalam SDF. Amati perbedaan perilaku jatuh dan tumbukan. Pantau: "ros2 topic list" untuk melihat topik aktif Gazebo.

**Slide 11** — Percobaan 3 – URDF dan RViz2: Visualisasi robot tanpa Gazebo. Perintah: "ros2 launch gazebo_praktikum percobaan3_urdf_rviz.launch.py". Terminal: 1. Tiga node: robot_state_publisher (proses URDF, publish /robot_description dan /tf), joint_state_publisher_gui (slider GUI interaktif menggerakkan joint), rviz2 (tampil model 3D). Gerakkan slider GUI dan amati model robot serta TF tree berubah real-time.

**Slide 12** — Percobaan 4 – Spawn Robot ke Gazebo: Robot URDF muncul di dalam simulasi Gazebo. Perintah: "ros2 launch gazebo_praktikum percobaan4_spawn_robot.launch.py". Terminal: 1. spawn_entity.py dijalankan dengan TimerAction delay 2 detik agar Gazebo siap lebih dahulu. Argumen: -entity robot_sederhana, -topic /robot_description, posisi awal (0,0,0.05). Visualisasi robot tampil bersamaan di Gazebo dan RViz2.

**Slide 13** — Percobaan 5 – Sensor Kamera dan LIDAR: Robot dilengkapi dua sensor aktif. Perintah: "ros2 launch gazebo_praktikum percobaan5_sensor.launch.py". Terminal: 2 (Terminal 2: "ros2 run teleop_twist_keyboard teleop_twist_keyboard"). Topik aktif: /robot/camera/image_raw, /robot/camera/camera_info, /scan, /odom. RViz2: tampilkan panel Camera dan LaserScan. Cek frekuensi sensor: "ros2 topic hz /scan".

**Slide 14** — ros2_control Arsitektur dan Komponen: Framework standar kontrol robot ROS2. Komponen: (1) controller_manager: satu node memuat dan mengelola semua controller. (2) hardware_interface: abstraksi fisik atau simulasi (GazeboSystem plugin). (3) Controller tersedia: diff_drive_controller untuk robot roda, joint_state_broadcaster wajib publish JointState, joint_trajectory_controller untuk manipulator. Konfigurasi via file YAML.

**Slide 15** — ros2_control Konfigurasi YAML: File ros2_control_diff_drive.yaml: controller_manager dengan update_rate 100 Hz. joint_state_broadcaster type: joint_state_broadcaster/JointStateBroadcaster. diff_drive_controller type: diff_drive_controller/DiffDriveController, parameter: wheel_separation 0.2, wheel_radius 0.05, cmd_vel_timeout 0.5. Spawner node: "ros2 run controller_manager spawner diff_drive_controller". Semua spawner menggunakan TimerAction delay bertingkat.

---

## PROMPT 2 — Slide 16-30 (Teleop + SLAM + Nav2 + Multi-Robot + Manipulator)

Lanjutkan presentasi Modul 04: ROS2 Dasar dan Simulasi Gazebo, slide 16-30. Fokus pada percobaan 6-11: kontrol keyboard dengan ros2_control, pemetaan SLAM, navigasi otonom Nav2, multi-robot dengan namespace, manipulator 3-DOF dengan ros2_control, dan kinematika FK/IK.

**Slide 16** — Percobaan 6 – Teleoperasi ros2_control: Perintah: "ros2 launch gazebo_praktikum percobaan6_teleop.launch.py". Terminal: 2. Terminal 1 launch (Gazebo + RSP + spawn + controller_manager + joint_state_broadcaster + diff_drive_controller + RViz2). Terminal 2: "ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap cmd_vel:=/diff_drive_controller/cmd_vel_unstamped". Tombol: i=maju, spasi=berhenti, j=kiri, l=kanan, k=stop penuh.

**Slide 17** — diff_drive_controller Detail: Controller menerima geometry_msgs/Twist dari /diff_drive_controller/cmd_vel_unstamped, konversi linear.x dan angular.z ke kecepatan tiap roda via hardware interface. Publish odometri ke /diff_drive_controller/odom (nav_msgs/Odometry) dan TF odom→base_footprint ke /tf. Monitor: "ros2 topic echo /diff_drive_controller/odom". Amati posisi robot berubah saat bergerak.

**Slide 18** — Percobaan 7A – SLAM dengan slam_toolbox: SLAM = robot membangun peta dan melokalisasi diri secara simultan. Perintah: "ros2 launch gazebo_praktikum percobaan7a_slam.launch.py". Terminal: 2 (Terminal 2: teleop_twist_keyboard). Node async_slam_toolbox_node subscribe /scan dan /tf, publish /map (nav_msgs/OccupancyGrid). Gerakkan robot di labirin, peta terbentuk di RViz2 panel Map. Simpan: "ros2 run nav2_map_server map_saver_cli -f ~/maps/my_map".

**Slide 19** — Occupancy Grid dan slam_params.yaml: Peta /map berupa grid 2D: 0=bebas (putih), 100=terisi (hitam), -1=tidak diketahui (abu-abu). Resolution 0.05 m/sel. slam_params.yaml: mode: mapping, resolution: 0.05, max_laser_range: 20.0, use_sim_time: true. Frame /map dipublish tiap ada scan baru. Setelah coverage mencukupi, simpan peta. File .yaml berisi metadata peta, .pgm berisi gambar occupancy grid.

**Slide 20** — Percobaan 7B – Navigasi Otonom Nav2: Perintah: "ros2 launch gazebo_praktikum percobaan7b_navigasi.launch.py map:=/path/my_map.yaml". Terminal: 1 (goal via klik RViz2). Prasyarat: peta .yaml dari 7A. Nav2 bringup_launch.py menjalankan: map_server (saji peta), AMCL (lokalisasi particle filter), planner_server (path global NavFn), controller_server (path lokal DWB), recovery_server, bt_navigator (orkestrasi behavior tree navigasi).

**Slide 21** — AMCL Lokalisasi dan Cara Pakai di RViz2: AMCL: particle filter dengan ratusan partikel hipotesa posisi robot. Tiap scan LIDAR dibandingkan peta untuk update bobot partikel. Prosedur di RViz2: (1) Klik "2D Pose Estimate" lalu klik posisi awal robot di peta (partikel konvergen). (2) Klik "2D Nav Goal" lalu klik tujuan (robot plan path dan bergerak otonom). Monitor: "ros2 topic echo /amcl_pose" untuk posisi estimasi.

**Slide 22** — Nav2 Parameter Kunci (nav2_params.yaml): amcl: max_particles 2000, min_particles 500. planner_server plugin GridBased (NavfnPlanner). controller_server plugin FollowPath (DWBLocalPlanner), max_vel_x 0.5 m/s. local_costmap resolution 0.05, inflation_radius 0.55 m. global_costmap resolution 0.05. Tuning: naikkan inflation_radius robot terlalu dekat dinding, turunkan max_vel_x robot goyang saat belok.

**Slide 23** — Percobaan 8 – Multi-Robot dengan Namespace: Perintah: "ros2 launch gazebo_praktikum percobaan8_multi_robot.launch.py". Terminal: 3 (Terminal 2: teleop robot1, Terminal 3: teleop robot2). Konstruksi launch: PushRosNamespace dan GroupAction mengisolasi topik tiap robot. Argumen xacro namespace:=robot1/robot2. frame_prefix: robot1/ pisahkan TF. Dua robot spawn di posisi berbeda: robot1 di (-3,0), robot2 di (3,0) dengan yaw 180 derajat.

**Slide 24** — Teleop Dua Robot Secara Independen: Terminal 2: "ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap /cmd_vel:=/robot1/cmd_vel" (kendalikan robot1). Terminal 3: "ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap /cmd_vel:=/robot2/cmd_vel" (kendalikan robot2). Keduanya berjalan bersamaan di terminal berbeda tanpa konflik. "ros2 topic list" tampilkan prefix namespace masing-masing robot.

**Slide 25** — Percobaan 9 – Manipulator Basic (Visualisasi): Perintah: "ros2 launch gazebo_praktikum percobaan9_manipulator.launch.py". Terminal: 1 (joint_state_publisher_gui muncul otomatis). Robot lengan 3-DOF dengan joint_1 (revolute Z), joint_2 (revolute Y), joint_3 (revolute Y). Gerakkan slider di GUI dan amati model arm bergerak di RViz2. Fixed Frame: world. Amati TF tree: world → base_link → link1 → link2 → link3 → end_effector.

**Slide 26** — Percobaan 10 – Manipulator ros2_control: Perintah: "ros2 launch gazebo_praktikum percobaan10_manipulator_control.launch.py". Terminal: 2 (Terminal 2: "ros2 run gazebo_praktikum demo_manipulator.py"). ros2_control dengan mock_components/GenericSystem. Controller: arm_position_controller (JointGroupPositionController) mengendalikan 3 joint sekaligus. Kontrol manual: "ros2 topic pub /arm_position_controller/commands std_msgs/msg/Float64MultiArray 'data: [1.57, 0.5, -1.0]'".

**Slide 27** — Percobaan 11 – Kinematika FK dan IK: Perintah: "ros2 launch gazebo_praktikum percobaan11_kinematics.launch.py". Terminal: 1 (kinematics_demo.py otomatis berjalan). Forward Kinematics (FK): menghitung posisi end-effector dari sudut joint menggunakan parameter link (L1=0.30m, L2=0.25m, L3=0.20m). Inverse Kinematics (IK): menghitung sudut joint yang dibutuhkan untuk mencapai posisi target. Marker hijau = posisi FK, Marker merah = target IK. Visualisasi di RViz2 dengan display Marker.

**Slide 28** — Monitoring Topik dan Data Sensor: LaserScan: "ros2 topic echo /scan --no-arr" (info tanpa array 360 nilai). Odometri: "ros2 topic echo /odom". Frekuensi: "ros2 topic hz /scan". Latensi: "ros2 topic delay /scan". Kamera: "ros2 run image_tools showimage --ros-args --remap /image:=/robot/camera/image_raw". Node info: "ros2 node info /robot_state_publisher" untuk lihat semua publisher dan subscriber.

**Slide 29** — TF2 Debug dan Troubleshooting Umum: TF debug: "ros2 run tf2_ros tf2_echo odom base_footprint" (cetak transform). "ros2 run tf2_tools view_frames" (PDF tree). Masalah umum dan solusi: (1) Robot tidak muncul Gazebo → spawn_entity belum selesai, naikkan delay TimerAction. (2) Joint tidak bergerak → cek joint_state_publisher berjalan. (3) Controller gagal spawn → naikkan delay spawner. (4) TF not found → cek URDF tidak ada link terputus.

**Slide 30** — Ringkasan Percobaan 1-11 Terminal dan Perintah: P1 Empty World: 1T, "ros2 launch ... percobaan1". P2 Shapes: 1T. P3 URDF+RViz2: 1T. P4 Spawn Robot: 1T. P5 Sensor: 2T (T2: teleop). P6 ros2_control: 2T (T2: teleop remap diff_drive). P7A SLAM: 2T (T2: teleop) + perintah simpan peta. P7B Nav2: 1T + klik RViz2. P8 Multi-Robot: 3T (T2: teleop robot1, T3: teleop robot2). P9 Manipulator Visual: 1T (slider GUI). P10 Manipulator ros2_control: 2T (T2: demo_manipulator.py). P11 Kinematika: 1T (FK/IK otomatis).

---

## PROMPT 3 — Slide 31-45 (TurtleBot3 + Omni + Mecanum + Project + Tugas Video)

Lanjutkan presentasi Modul 04: ROS2 Dasar dan Simulasi Gazebo, slide 31-45. Slide 31-33 percobaan 12-14 TurtleBot3, slide 34-36 percobaan 15-17 robot omni wheel, slide 37-39 percobaan 18-20 robot mecanum, slide 40-42 project akhir, slide 43-45 panduan tugas video.

**Slide 31** — Percobaan 12 – TurtleBot3 di Gazebo (Konsep): TurtleBot3 adalah robot standar ROS2 oleh ROBOTIS. Tipe: Burger (compact, LDS-01 LIDAR), Waffle (lebih besar, LIDAR + kamera Intel RealSense). Perintah: "ros2 launch gazebo_praktikum percobaan12_turtlebot3.launch.py". Environment variable TURTLEBOT3_MODEL=waffle diset otomatis di launch file. Package turtlebot3_gazebo menyediakan world dan model siap pakai.

**Slide 32** — Percobaan 13 – TurtleBot3 SLAM: Perintah: "ros2 launch gazebo_praktikum percobaan13_turtlebot3_slam.launch.py". Terminal 2: teleop_twist_keyboard. TurtleBot3 menggunakan sensor LDS-01 (360-degree LIDAR) yang ideal untuk SLAM. Keliling TurtleBot3 World, peta terbentuk di RViz2 panel Map. Simpan: "ros2 run nav2_map_server map_saver_cli -f ~/map_turtlebot3". Bandingkan akurasi peta TB3 vs robot custom (sensor berbeda).

**Slide 33** — Percobaan 14 – TurtleBot3 Navigasi Nav2: Perintah: "ros2 launch gazebo_praktikum percobaan14_turtlebot3_nav.launch.py map:=/path/map_turtlebot3.yaml". Prasyarat: peta dari percobaan 13. Di RViz2: 2D Pose Estimate (lokalisasi awal) lalu 2D Nav Goal (kirim tujuan). TurtleBot3 bergerak otonom menghindari rintangan. Nav2 stack: map_server, AMCL, planner_server, controller_server, bt_navigator.

**Slide 34** — Robot Omnidirectional (Konsep): Robot omni menggunakan roda dengan roller perpendicular terhadap sumbu roda, memungkinkan gerak ke segala arah tanpa perlu berputar. Kinematika holonomic: 3 DOF di bidang (x, y, yaw) semuanya terkontrol langsung. Kelebihan: manuver di ruang sempit, gerakan lateral tanpa rotasi. Plugin Gazebo: libgazebo_ros_planar_move.so untuk simulasi gerak omnidirectional.

**Slide 35** — Percobaan 15-17 – Robot Omni (Teleop, SLAM, Nav): Percobaan 15: "ros2 launch gazebo_praktikum percobaan15_omni_teleop.launch.py" + teleop. Gerak lateral: linear.y di cmd_vel. URDF: robot_omni.urdf.xacro dengan 4 roda omni, LIDAR 360. Percobaan 16: omni SLAM menggunakan slam_toolbox, simpan peta. Percobaan 17: omni navigation dengan Nav2 menggunakan peta dari percobaan 16. Perbandingan: omni lebih cepat bermanuver di lorong sempit.

**Slide 36** — Robot Mecanum Wheel (Konsep): Roda mecanum memiliki roller miring 45 derajat terhadap sumbu roda. Dengan 4 roda mecanum, robot dapat bergerak holonomic seperti omni. Perbedaan vs omni: roda mecanum lebih tahan terhadap beban berat, roller 45 membuat gerak lateral sedikit lebih kasar. URDF: robot_mecanum.urdf.xacro dengan camera + LIDAR. Plugin: libgazebo_ros_planar_move.so.

**Slide 37** — Percobaan 18-20 – Robot Mecanum (Teleop, SLAM, Nav): Percobaan 18: "ros2 launch gazebo_praktikum percobaan18_mecanum_teleop.launch.py" + teleop. Percobaan 19: mecanum SLAM + simpan peta. Percobaan 20: mecanum navigation dengan Nav2. Robot mecanum juga memiliki kamera RGB selain LIDAR. Tugas analisa: bandingkan performa mapping dan navigation antara diff-drive (percobaan 7), omni (percobaan 16-17), dan mecanum (percobaan 19-20).

**Slide 38** — Perbandingan Tipe Robot: Tabel perbandingan 4 tipe robot: (1) Differential Drive: 2 roda + caster, sederhana, hanya maju/mundur/putar. (2) TurtleBot3: diff-drive standar, sensor lengkap, banyak library. (3) Omni 4-roda: holonomic, gerakan lateral halus, roller perpendicular. (4) Mecanum 4-roda: holonomic, tahan beban, roller 45 derajat. Pemilihan robot tergantung aplikasi: warehouse (omni/mecanum), outdoor (diff-drive), research (TurtleBot3).

**Slide 39** — Project Akhir Overview: Project Modul 04: Simulasi Robot Otonom di Lingkungan Kustom. Kelompok 2-4 mahasiswa. Lima deliverable: (1) ROS2 package lengkap (world kustom + URDF/XACRO + launch files), (2) peta SLAM lingkungan kustom (.yaml + .pgm), (3) demo navigasi otonom ke 3+ waypoint, (4) laporan PDF 8+ halaman, (5) video demo 5-8 menit. Referensi: seluruh 20 percobaan sebagai template. Gunakan robot custom dengan sensor LIDAR + camera.

**Slide 40** — Project Fase 1-3 Desain dan SLAM: Fase 1: Buat world SDF kustom (rumah sakit/gudang/kantor) minimal 3 ruangan, 6 objek statis, pencahayaan realistis. Fase 2: Buat robot URDF/XACRO kustom: chassis 2 primitif, 2 roda + caster, diff_drive, LIDAR + kamera. Fase 3: Launch file terintegrasi satu perintah. Fase 4: Jalankan SLAM di world kustom, simpan peta.

**Slide 41** — Project Fase 4-5 Navigasi dan Bonus: Fase 4: Navigasi Nav2 menggunakan peta dari SLAM. Buat Python script kirim 3 goal pose berurutan via Nav2 action server. Fase 5 (bonus): Node obstacle avoidance reaktif membaca /scan, jika jarak < 0.4m: berhenti, mundur, putar 90 derajat. Rubrik: World 20%, URDF 25%, Launch 15%, SLAM 20%, Nav 20%, Bonus 15%.

**Slide 42** — Tugas Video Struktur dan Alat: Video demonstrasi 18-30 menit meliput seluruh 20 percobaan dan project. Struktur: (1) Intro konsep ROS2 3 menit. (2) Percobaan 1-7B dalam 6 menit. (3) Percobaan 8-11 multi-robot+manipulator+kinematika 4 menit. (4) Percobaan 12-14 TurtleBot3 3 menit. (5) Percobaan 15-20 omni+mecanum 4 menit. (6) Demo project kustom 6 menit. (7) Penjelasan kode 3 menit. (8) Penutup 1 menit. Gunakan OBS Studio, audio narasi bahasa Indonesia jelas.

**Slide 43** — Tugas Video Bagian Demo Percobaan: Demo percobaan 1-7B: world kosong, shapes fisika, URDF slider, spawn Gazebo, sensor LIDAR+kamera RViz2, teleop keyboard odometri, SLAM peta bertahap, Nav2 otonom. Demo percobaan 8-11: dua robot namespace terpisah, manipulator slider GUI, ros2_control arm, FK/IK marker RViz2. Demo percobaan 12-20: TurtleBot3 teleop+SLAM+nav, omni lateral teleop+SLAM+nav, mecanum teleop+SLAM+nav.

**Slide 44** — Tugas Video Demo Project dan Kode: Demo project: world custom, URDF custom LIDAR + kamera, launch satu perintah, SLAM peta lengkap, navigasi ke 2+ goal. Penjelasan kode: (1) robot_lengkap.urdf.xacro (plugin diff_drive, sensor), (2) robot_omni.urdf.xacro (plugin planar_move), (3) nav2_params.yaml (AMCL + planner), (4) perbedaan kinematika diff-drive vs omni vs mecanum.

**Slide 45** — Penutup dan Referensi Lengkap: Referensi: ROS2 Humble Documentation (docs.ros.org/en/humble), Gazebo Classic Documentation (classic.gazebosim.org), Nav2 Documentation (navigation.ros.org), slam_toolbox GitHub (SteveMacenski/slam_toolbox), ros2_control Documentation (control.ros.org), TurtleBot3 e-Manual (emanual.robotis.com). Selanjutnya: Modul 05. Semua 20 percobaan Modul 04 tercakup: world dasar, URDF, sensor, teleop, SLAM, Nav2, multi-robot, manipulator, kinematika, TurtleBot3, robot omni wheel, robot mecanum wheel.

---
