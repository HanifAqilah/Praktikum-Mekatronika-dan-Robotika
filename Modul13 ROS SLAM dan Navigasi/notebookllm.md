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

## PROMPT 2 — Slide 16-30 (Teleop + SLAM + Nav2 + Manipulator + Multi-Robot)

Lanjutkan presentasi Modul 04: ROS2 Dasar dan Simulasi Gazebo, slide 16-30. Fokus pada percobaan 6-9: kontrol keyboard dengan ros2_control, pemetaan SLAM, navigasi otonom Nav2, manipulator 3-DOF, dan simulasi multi-robot dengan namespace.

**Slide 16** — Percobaan 6 – Teleoperasi ros2_control: Perintah: "ros2 launch gazebo_praktikum percobaan6_teleop.launch.py". Terminal: 2. Terminal 1 launch (Gazebo + RSP + spawn + controller_manager + joint_state_broadcaster + diff_drive_controller + RViz2). Terminal 2: "ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap cmd_vel:=/diff_drive_controller/cmd_vel_unstamped". Tombol: i=maju, spasi=berhenti, j=kiri, l=kanan, k=stop penuh.

**Slide 17** — diff_drive_controller Detail: Controller menerima geometry_msgs/Twist dari /diff_drive_controller/cmd_vel_unstamped, konversi linear.x dan angular.z ke kecepatan tiap roda via hardware interface. Publish odometri ke /diff_drive_controller/odom (nav_msgs/Odometry) dan TF odom→base_footprint ke /tf. Monitor: "ros2 topic echo /diff_drive_controller/odom". Amati posisi robot berubah saat bergerak.

**Slide 18** — Percobaan 7A – SLAM dengan slam_toolbox: SLAM = robot membangun peta dan melokalisasi diri secara simultan. Perintah: "ros2 launch gazebo_praktikum percobaan7a_slam.launch.py". Terminal: 2 (Terminal 2: teleop_twist_keyboard). Node async_slam_toolbox_node subscribe /scan dan /tf, publish /map (nav_msgs/OccupancyGrid). Gerakkan robot di labirin, peta terbentuk di RViz2 panel Map. Simpan: "ros2 run nav2_map_server map_saver_cli -f ~/maps/my_map".

**Slide 19** — Occupancy Grid dan slam_params.yaml: Peta /map berupa grid 2D: 0=bebas (putih), 100=terisi (hitam), -1=tidak diketahui (abu-abu). Resolution 0.05 m/sel. slam_params.yaml: mode: mapping, resolution: 0.05, max_laser_range: 20.0, use_sim_time: true. Frame /map dipublish tiap ada scan baru. Setelah coverage mencukupi, simpan peta. File .yaml berisi metadata peta, .pgm berisi gambar occupancy grid.

**Slide 20** — Percobaan 7B – Navigasi Otonom Nav2: Perintah: "ros2 launch gazebo_praktikum percobaan7b_navigasi.launch.py map:=/path/my_map.yaml". Terminal: 1 (goal via klik RViz2). Prasyarat: peta .yaml dari 7A. Nav2 bringup_launch.py menjalankan: map_server (saji peta), AMCL (lokalisasi particle filter), planner_server (path global NavFn), controller_server (path lokal DWB), recovery_server, bt_navigator (orkestrasi behavior tree navigasi).

**Slide 21** — AMCL Lokalisasi dan Cara Pakai di RViz2: AMCL: particle filter dengan ratusan partikel hipotesa posisi robot. Tiap scan LIDAR dibandingkan peta untuk update bobot partikel. Prosedur di RViz2: (1) Klik "2D Pose Estimate" lalu klik posisi awal robot di peta (partikel konvergen). (2) Klik "2D Nav Goal" lalu klik tujuan (robot plan path dan bergerak otonom). Monitor: "ros2 topic echo /amcl_pose" untuk posisi estimasi.

**Slide 22** — Nav2 Parameter Kunci (nav2_params.yaml): amcl: max_particles 2000, min_particles 500. planner_server plugin GridBased (NavfnPlanner). controller_server plugin FollowPath (DWBLocalPlanner), max_vel_x 0.5 m/s. local_costmap resolution 0.05, inflation_radius 0.55 m. global_costmap resolution 0.05. Tuning: naikkan inflation_radius robot terlalu dekat dinding, turunkan max_vel_x robot goyang saat belok.

**Slide 23** — Percobaan 8 – Manipulator 3-DOF: Perintah: "ros2 launch gazebo_praktikum percobaan8_manipulator.launch.py". Terminal: 1 (demo_manipulator.py otomatis berjalan setelah 8 detik). paused:=true default agar controller siap sebelum simulasi jalan. Namespace /manipulator/. Controller: joint_state_broadcaster (4s), joint_1_position_controller (5s), joint_2_position_controller (5.5s), joint_3_position_controller (6s). Delay bertingkat cegah race condition.

**Slide 24** — Kontrol Manual Manipulator (Terminal 2 Opsional): Setelah launch, Terminal 2: "ros2 topic pub /manipulator/joint_1_position_controller/command std_msgs/msg/Float64 "data: 1.57" --once". Joint 2: "data: 0.5". Joint 3: "data: -1.0". Nilai dalam radian. Baca posisi aktual: "ros2 topic echo /manipulator/joint_state_broadcaster/joint_states". Amati robot arm bergerak ke posisi target di Gazebo dan RViz2.

**Slide 25** — manipulator_controllers.yaml Konfigurasi: Setiap joint menggunakan type: position_controllers/JointGroupPositionController. field joints: [joint_1, joint_2, joint_3]. command_interfaces: [position]. state_interfaces: [position, velocity]. Controller di-spawn berurutan dengan delay: joint_state_broadcaster 4s, joint_1 5s, joint_2 5.5s, joint_3 6s untuk memastikan controller_manager sudah siap sebelum spawner dipanggil.

**Slide 26** — Percobaan 9 – Multi-Robot dengan Namespace: Perintah: "ros2 launch gazebo_praktikum percobaan9_multi_robot.launch.py". Terminal: 3 (Terminal 2: teleop robot1, Terminal 3: teleop robot2). Konstruksi launch: PushRosNamespace dan GroupAction mengisolasi topik tiap robot. Argumen xacro namespace:=robot1/robot2. frame_prefix: robot1/ pisahkan TF. Dua robot spawn di posisi berbeda: robot1 di (-3,0), robot2 di (3,0) dengan yaw 180 derajat.

**Slide 27** — Teleop Dua Robot Secara Independen: Terminal 2: "ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap /cmd_vel:=/robot1/cmd_vel" (kendalikan robot1). Terminal 3: "ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap /cmd_vel:=/robot2/cmd_vel" (kendalikan robot2). Keduanya berjalan bersamaan di terminal berbeda tanpa konflik. "ros2 topic list" tampilkan prefix namespace masing-masing robot.

**Slide 28** — Monitoring Topik dan Data Sensor: LaserScan: "ros2 topic echo /scan --no-arr" (info tanpa array 360 nilai). Odometri: "ros2 topic echo /odom". Frekuensi: "ros2 topic hz /scan". Latensi: "ros2 topic delay /scan". Kamera: "ros2 run image_tools showimage --ros-args --remap /image:=/robot/camera/image_raw". Node info: "ros2 node info /robot_state_publisher" untuk lihat semua publisher dan subscriber.

**Slide 29** — TF2 Debug dan Troubleshooting Umum: TF debug: "ros2 run tf2_ros tf2_echo odom base_footprint" (cetak transform). "ros2 run tf2_tools view_frames" (PDF tree). Masalah umum dan solusi: (1) Robot tidak muncul Gazebo → spawn_entity belum selesai, naikkan delay TimerAction. (2) Joint tidak bergerak → cek joint_state_publisher berjalan. (3) Controller gagal spawn → naikkan delay spawner. (4) TF not found → cek URDF tidak ada link terputus.

**Slide 30** — Ringkasan Percobaan 1-9 Terminal dan Perintah: P1 Empty World: 1T, "ros2 launch ... percobaan1". P2 Shapes: 1T. P3 URDF+RViz2: 1T. P4 Spawn Robot: 1T. P5 Sensor: 2T (T2: teleop). P6 ros2_control: 2T (T2: teleop remap diff_drive). P7A SLAM: 2T (T2: teleop) + perintah simpan peta. P7B Nav2: 1T + klik RViz2. P8 Manipulator: 1T + opsional T2 manual. P9 Multi-Robot: 3T (T2: teleop robot1, T3: teleop robot2).

---

## PROMPT 3 — Slide 31-45 (Plugin C++ + Advanced + Project + Tugas Video)

Lanjutkan presentasi Modul 04: ROS2 Dasar dan Simulasi Gazebo, slide 31-45. Slide 31-35 percobaan 10 plugin Custom C++ Gazebo dan spawn dinamis, slide 36-41 project akhir simulasi robot otonom di lingkungan kustom, slide 42-45 panduan lengkap tugas video demonstrasi.

**Slide 31** — Percobaan 10 – World Plugin C++ (Konsep): Perintah: "ros2 launch gazebo_praktikum percobaan10_advanced.launch.py". Terminal: 1 (opsional T2: "ros2 topic echo /plugin/sim_time"). Plugin libmy_world_plugin.so dimuat SDF via tag plugin dengan filename. SetEnvironmentVariable di launch file set GAZEBO_PLUGIN_PATH ke folder lib install package menggunakan ament_index agar path selalu benar.

**Slide 32** — World Plugin C++ (Implementasi): Struktur my_world_plugin.cpp: class MyWorldPlugin inherits gazebo::WorldPlugin. Method Load(WorldPtr, sdf::ElementPtr) dipanggil saat Gazebo load world. node_ = gazebo_ros::Node::Get(sdf) mendapat rclcpp node. Publisher std_msgs::msg::Float64 ke /plugin/sim_time. update_connection_ = Events::ConnectWorldUpdateBegin(callback) dipanggil tiap physics step publish waktu simulasi.

**Slide 33** — World Plugin C++ (CMakeLists.txt): Konfigurasi CMakeLists untuk plugin: find_package(gazebo REQUIRED) dan find_package(gazebo_ros REQUIRED). add_library(my_world_plugin SHARED src/my_world_plugin.cpp). ament_target_dependencies: rclcpp, gazebo_ros, std_msgs. target_include_directories tambahkan GAZEBO_INCLUDE_DIRS. target_link_libraries tambahkan GAZEBO_LIBRARIES. install(TARGETS my_world_plugin DESTINATION lib) agar terinstall dan dapat dimuat Gazebo.

**Slide 34** — Dynamic Spawn via Gazebo Service: Script advanced_demo.py (rclpy node) spawn objek dinamis menggunakan service /spawn_entity (gazebo_msgs/srv/SpawnEntity). Field: name (string unik), xml (SDF string), initial_pose (geometry_msgs/Pose). Contoh: spawn box 0.3m di koordinat (2, 0, 0.5). Service /delete_entity untuk hapus objek. Berguna: skenario dinamis obstacle muncul/menghilang, target berpindah, multi-stage experiment.

**Slide 35** — Monitor Plugin dan Output Advanced: "ros2 topic echo /plugin/sim_time" (Float64: waktu simulasi detik). "ros2 topic echo /plugin/status" (String: status world). "ros2 topic hz /plugin/sim_time" ukur frekuensi publish sinkron physics update sekitar 1000 Hz. Plugin dunia berguna: logging data eksperimen otomatis, trigger event berdasarkan kondisi simulasi, integrasi sensor virtual kustom, kontrol parameter fisika saat runtime.

**Slide 36** — Project Akhir Overview: Project Modul 04: Simulasi Robot Otonom di Lingkungan Kustom. Kelompok 2-3 mahasiswa. Lima deliverable: (1) ROS2 package lengkap (world kustom + URDF/XACRO + launch files), (2) peta SLAM lingkungan kustom (.yaml + .pgm), (3) demo navigasi otonom ke 2+ waypoint, (4) laporan PDF 8+ halaman, (5) video demo 5-8 menit. Kriteria: Teknis 40%, Hasil 25%, Kreativitas 15%, Laporan 20%.

**Slide 37** — Project Fase 1 – Desain World Kustom: Buat world SDF kustom minimal 1 ruangan 10x10 meter, 4 dinding, 3 rintangan berbeda bentuk. Gunakan Gazebo Building Editor (Edit menu) atau tulis SDF manual. Tambahkan pencahayaan directional dan material lantai. Simpan ke worlds/my_world.world. Test dengan launch file sederhana menggunakan percobaan1 sebagai template ganti world file. Screenshot world masuk laporan.

**Slide 38** — Project Fase 2 – URDF Robot Kustom: Buat robot_kustom.urdf.xacro: base_link (box geometry 0.3x0.2x0.1m), 2 roda continuous joint (radius 0.05m, separation 0.2m), caster wheel fixed joint (sphere 0.025m), lidar_link (cylinder kecil), diff_drive plugin, ray sensor plugin. Validasi: "xacro robot.xacro | check_urdf". Visualisasi: gunakan template percobaan3. Diagram link-joint masuk laporan.

**Slide 39** — Project Fase 3 – SLAM Pemetaan Lingkungan Kustom: Launch SLAM di world kustom menggunakan percobaan7A sebagai template, ganti world file dan XACRO. Jalankan teleop terminal 2, keliling seluruh ruangan sampai peta coverage mencukupi. Monitor di RViz2 panel Map (Durability: Transient Local). Simpan peta saat coverage baik: "ros2 run nav2_map_server map_saver_cli -f ~/ros2_ws/maps/nama_peta". Verifikasi file .yaml dan .pgm terbuat. Screenshot peta masuk laporan.

**Slide 40** — Project Fase 4 – Navigasi Otonom ke Dua Waypoint: Gunakan peta dari fase 3 dengan percobaan7B template. Launch dengan argumen map:=/path/my_map.yaml. Set initial pose di RViz2 sesuai posisi spawn robot (partikel AMCL konvergen). Kirim goal waypoint 1 klik 2D Nav Goal, robot bergerak otonom. Kirim goal waypoint 2 setelah robot tiba. Rekam video robot menghindari rintangan. Eksperimen parameter Nav2 untuk tuning performa.

**Slide 41** — Project Laporan Struktur dan Penilaian: Laporan PDF 8+ halaman: Bab 1 Pendahuluan dan tujuan. Bab 2 Desain lingkungan (gambar world). Bab 3 Desain robot URDF (diagram link-joint). Bab 4 SLAM konfigurasi dan hasil peta. Bab 5 Nav2 konfigurasi parameter dan analisis. Bab 6 Hasil dan diskusi (screenshot, path plan, kendala dan solusi). Bab 7 Kesimpulan. Bab 8 Referensi ROS2/Nav2/slam_toolbox. Penilaian: Teknis 40%, Hasil 25%, Kreativitas 15%, Laporan 20%.

**Slide 42** — Tugas Video Struktur dan Alat: Video demonstrasi 10-15 menit meliput seluruh 10 percobaan dan project. Struktur: (1) Intro konsep ROS2 1 menit. (2) Percobaan 1-5 dalam 3 menit. (3) Percobaan 6-7A SLAM 3 menit. (4) Percobaan 7B-10 navigasi+manipulator+multi-robot+plugin 4 menit. (5) Demo project kustom 3 menit. (6) Kesimpulan 1 menit. Gunakan OBS Studio atau Kazam screen recorder. Audio narasi bahasa Indonesia jelas.

**Slide 43** — Tugas Video Bagian 1 dan 2 (Percobaan 1-5): Bagian 1 (P1-P3): tampilkan Gazebo terbuka P1, objek jatuh gravitasi P2, gerakkan slider joint_state_publisher_gui P3 dan robot bergerak RViz2. Bagian 2 (P4-P5): robot muncul Gazebo setelah 2 detik spawn, tampilkan panel Camera dan LaserScan di RViz2 aktif, gerakkan robot dengan teleop keyboard, "ros2 topic echo /scan --no-arr" terlihat data mengalir di terminal.

**Slide 44** — Tugas Video Bagian 3 dan 4 (Percobaan 6-10): Bagian 3 (P6-7A): dual terminal launch + teleop, tampilkan odometri saat robot bergerak, peta SLAM terbentuk bertahap di RViz2, simpan peta .pgm. Bagian 4 (P7B-10): Nav2 robot gerak otonom ke dua goal hindari rintangan, manipulator 3 joint gerak otomatis dan manual, dua terminal teleop multi-robot bersamaan, "ros2 topic echo /plugin/sim_time" aktif plugin output terlihat.

**Slide 45** — Penutup dan Referensi Lengkap: Referensi: ROS2 Humble Documentation (docs.ros.org/en/humble), Gazebo Classic Documentation (classic.gazebosim.org), Nav2 Documentation (navigation.ros.org), slam_toolbox GitHub (SteveMacenski/slam_toolbox), ros2_control Documentation (control.ros.org). Selanjutnya: Modul 05 Computer Vision dan Robot Perception. Semua 10 percobaan Modul 04 tercakup: empty world, shapes, URDF+RViz2, spawn robot, sensor kamera+LIDAR, ros2_control+teleop, SLAM, navigasi Nav2, manipulator 3-DOF, multi-robot namespace, plugin C++ Gazebo custom.

---
