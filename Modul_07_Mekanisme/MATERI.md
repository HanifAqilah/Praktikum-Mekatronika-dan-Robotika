# MODUL 7: MEKANISME ROBOT & TRANSMISI

## 1. DEFINISI

### 1.1 Mekanisme
**Mekanisme** adalah kombinasi komponen mekanik yang berfungsi untuk mentransmisikan gerakan dan gaya dari satu titik ke titik lain dalam sistem robot. Mekanisme memungkinkan robot untuk mengubah energi rotasi motor menjadi gerakan yang diinginkan.

### 1.2 Transmisi
**Transmisi** adalah sistem yang mengubah kecepatan, torsi, atau arah gerakan menggunakan komponen seperti gear, belt, atau screw. Transmisi memungkinkan motor berkecepatan tinggi untuk menggerakkan beban dengan torsi yang lebih besar.

### 1.3 Backlash
**Backlash** adalah celah atau gap yang tidak diinginkan dalam mekanisme yang menyebabkan error posisi, terutama saat terjadi perubahan arah gerakan. Backlash adalah musuh utama dalam sistem kontrol presisi.

---

## 2. KONSEP INTI

### 2.1 Perhitungan Gear Ratio

**Gear Ratio** adalah rasio antara kecepatan output dan input:

$$\text{Gear Ratio} = \frac{N_{\text{output}}}{N_{\text{input}}} = \frac{\omega_{\text{output}}}{\omega_{\text{input}}} = \frac{T_{\text{output}}}{T_{\text{input}}}$$

Dimana:
- $N$ = jumlah teeth (gigi)
- $\omega$ = kecepatan angular (RPM atau rad/s)
- $T$ = torsi (Nm)

**Contoh:**
- Input gear: 10 teeth, 1000 RPM
- Output gear: 100 teeth
- Gear ratio = 100/10 = 10:1
- Output speed = 1000/10 = 100 RPM
- Output torque = Input torque × 10

### 2.2 Tipe Gear

#### a) Spur Gear
- **Karakteristik:** Gigi parallel dengan axis, transmisi langsung
- **Kelebihan:** Sederhana, murah, mudah manufacture
- **Kekurangan:** Noise tinggi pada kecepatan tinggi, efficiency ~90-95%
- **Aplikasi:** AGV wheel drive, simple robot joint

#### b) Planetary Gear
- **Karakteristik:** Sun gear di tengah, planet gear mengelilingi, ring gear di luar
- **Kelebihan:** Compact, high reduction ratio (10:1 hingga 100:1), smooth, load distribution merata
- **Kekurangan:** Kompleks, mahal, assembly presisi tinggi
- **Aplikasi:** Robot arm joint, servo motor gearbox

#### c) Bevel Gear
- **Karakteristik:** Transmisi antar axis yang berpotongan (90°)
- **Kelebihan:** Change direction, compact
- **Kekurangan:** Assembly kompleks, noise
- **Aplikasi:** Right-angle drive, differential gear

#### d) Worm Gear
- **Karakteristik:** Worm screw drive gear, reduction ratio sangat tinggi
- **Kelebihan:** Self-locking (tidak bisa back-drive), quiet operation
- **Kekurangan:** Efficiency rendah (40-60%), friction tinggi
- **Aplikasi:** Safety mechanism, lift system

### 2.3 Belt & Pulley

**Karakteristik:**
- Smooth operation (no teeth noise)
- Timing belt: no slip, precise positioning
- Flat belt: slip possible, need tensioner

**Gear Ratio:**
$$\text{Ratio} = \frac{D_{\text{output}}}{D_{\text{input}}}$$

**Kelebihan:**
- Low noise, smooth
- Compliance (absorb shock)
- Timing belt: zero backlash dengan proper tension

**Kekurangan:**
- Belt wear over time
- Slip possible (flat belt)
- Tensioner required

### 2.4 Lead Screw

**Fungsi:** Convert rotational motion → linear motion

**Formula:**
$$\text{Linear Advance} = \text{Pitch} \times \text{Rotation}$$

**Karakteristik:**
- High friction (need lubrication)
- Backlash possible (thread clearance)
- Self-locking (high friction angle)

**Aplikasi:** CNC axis, linear actuator, lift mechanism

### 2.5 Backlash Analysis

#### Sumber Backlash:
1. **Gear tooth gap:** Clearance between meshing teeth
2. **Bearing play:** Radial/axial clearance di bearing
3. **Bolt preload:** Insufficient clamping force
4. **Shaft deflection:** Bending under load

#### Efek Backlash:
- **Hysteresis:** Position berbeda untuk CW vs CCW motion
- **Dead-zone:** Range dimana input motion tidak produce output motion
- **Control instability:** Oscillation near target position

#### Backlash Compensation:

**Metode 1: Software Compensation**
```python
def backlash_compensation(target_position, current_position, backlash_offset):
    # Detect direction change
    direction_change = (target_position - current_position) * (last_target - current_position) < 0
    
    if direction_change:
        # Add offset saat direction change
        compensated_target = target_position + backlash_offset * sign(target_position - current_position)
        return compensated_target
    else:
        return target_position
```

**Metode 2: Hardware Compensation**
- Preloaded bearing (eliminate bearing play)
- Zero-backlash gear (split gear dengan spring)
- Anti-backlash nut (preloaded thread)

### 2.6 Workspace Analysis

**Workspace** adalah himpunan semua posisi yang dapat dicapai oleh end-effector robot.

**Faktor yang Mempengaruhi Workspace:**
1. **Link length:** Panjang arm segment
2. **Joint limit:** Range motion (0-180° typical)
3. **Singularity:** Posisi dimana robot lose DOF
4. **Collision:** Self-collision atau collision dengan obstacle

**Workspace Types:**
- **Reachable workspace:** Semua posisi yang dapat dicapai
- **Dexterous workspace:** Posisi yang dapat dicapai dengan semua orientasi
- **Planar workspace:** Untuk 2D robot (circular atau annular)
- **Spatial workspace:** Untuk 3D robot (spherical atau toroidal)

### 2.7 Dexterity & Manipulability

**Dexterity** mengukur kualitas workspace (kemampuan robot bergerak di semua arah).

**Manipulability Index:**
$$w(q) = \sqrt{\det(J(q) \cdot J(q)^T)}$$

Dimana:
- $J(q)$ = Jacobian matrix
- $w(q)$ = manipulability measure

**Interpretasi:**
- $w \rightarrow 0$: Singularity (poor dexterity)
- $w$ maksimum: Optimal configuration (best dexterity)

---

## 3. DIAGRAM & ILUSTRASI

### 3.1 Gear Type Comparison

```
SPUR GEAR                 PLANETARY GEAR              BEVEL GEAR
┌────────┐               ┌──────────────┐            ┌────────┐
│  ┌──┐  │               │    ┌───┐     │            │   ╱    │
│ ││  ││ │               │   ╱     ╲    │            │  ╱     │
│ ││  ││ │               │  │ SUN  │    │            │ ╱  90° │
│  └──┘  │               │   ╲     ╱    │            │╱       │
└────────┘               │    └───┘     │            └────────┘
Parallel axis            Compact, high ratio         Right-angle
Simple, cheap            Smooth, expensive           Direction change
```

### 3.2 Gear Meshing Diagram

```
     Input Gear                Output Gear
        (10T)                     (100T)
      ┌─────┐                   ┌─────┐
      │  ●  │────────┬──────────│  ●  │
      └─────┘        │          └─────┘
                 Backlash Gap
                  (0.1-0.5mm)
                  
Pitch Circle ─────────┴──────────────
```

### 3.3 Backlash Effect on Position

```
Input Motion (Back & Forth)
     ▲
     │    ┌─────┐
     │    │     │
     │────┘     └────
     └───────────────► Time

Output Position
     ▲      Hysteresis Loop
     │    ┌──────┐
     │   ╱│      │╲
     │  │ │      │ │  ← Backlash width
     │  │ │      │ │
     │   ╲│      │╱
     │    └──────┘
     └───────────────► Time

Without Compensation: Position error
With Compensation: Reduced error
```

### 3.4 Lead Screw Pitch & Advance

```
      Rotation Direction
           →
    ┌─────────────────┐
    │ ╱╱╱╱╱╱╱╱╱╱╱╱╱╱ │ ← Thread
    │╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱ │
    └─────────────────┘
    
    Pitch (P) = 2mm
    1 rotation = 2mm linear advance
    10 rotation = 20mm linear advance
```

### 3.5 Belt & Pulley System

```
         Motor                        Load
         Pulley                       Pulley
         (D1)                         (D2)
        ┌───┐                        ┌───┐
        │ ● │────────────────────────│ ● │
        └───┘         Belt           └───┘
                    Tension
                       │
                       ▼
                   Tensioner
                     ┌───┐
                     │ ● │
                     └───┘

Gear Ratio = D2 / D1
Proper tension prevent slip
```

### 3.6 Robot Arm Workspace (2-DOF Planar)

```
        Y
        ▲
        │
        │    Workspace (Annular)
        │     ┌─────────────┐
        │    ╱               ╲
        │   │    Reachable   │
        │   │      Area      │
        │   │                │
        │    ╲               ╱
        │     └─────────────┘
        │          ● Base
        └────────────────────► X

Maximum reach = L1 + L2
Minimum reach = |L1 - L2|
```

### 3.7 Singularity Position

```
SINGULARITY: Arm Fully Extended
        
        ┌─────────Link2─────────┐
        │                       │
    Joint1                   End-Effector
        ●─────Link1──────●──────●
        
    Det(Jacobian) ≈ 0
    Loss of DOF in perpendicular direction
    Cannot move perpendicular to arm
    

SINGULARITY: Elbow = Wrist (Folded)

        End-Effector
             ●
             │Link2
        Joint2 ●
             │Link1
        Joint1 ●
        
    Multiple configuration for same position
    Numerical instability
```

### 3.8 Compliance Diagram

```
Force vs Deformation

Force (N)
    ▲
    │      ╱
    │     ╱ Stiffness = ΔF/Δx
    │    ╱
    │   ╱
    │  ╱
    │ ╱
    └─────────────► Deformation (mm)

High stiffness = Low compliance (good for precision)
Low stiffness = High compliance (good for shock absorption)
```

---

## 4. CONTOH KASUS INDUSTRI

### 4.1 Robot Arm: Planetary Gearbox
**Aplikasi:** ABB IRB 6700, KUKA KR series

**Spesifikasi:**
- Gear ratio: 50:1 hingga 160:1
- Backlash: < 1 arcmin (< 0.0167°)
- Efficiency: > 85%
- Kompak dan ringan

**Mengapa Planetary?**
- Joint robot butuh reduction ratio tinggi (motor 3000 RPM → joint 20 RPM)
- Compact untuk fit di joint housing
- Smooth operation untuk precision control
- Load distribution merata (multiple planet gear)

### 4.2 AGV: Spur Gear Wheel Drive
**Aplikasi:** Mobile robot, warehouse AGV

**Spesifikasi:**
- Gear ratio: 10:1 hingga 20:1
- Simple, low cost
- Easy maintenance

**Mengapa Spur?**
- Cost-effective untuk aplikasi tidak butuh presisi ekstrem
- Parallel axis cocok untuk wheel drive layout
- Easy replacement saat wear

### 4.3 CNC Spindle: Belt Drive
**Aplikasi:** CNC milling machine spindle

**Spesifikasi:**
- Belt ratio: 1:1 hingga 3:1
- Variable speed dengan adjust pulley diameter
- Smooth, low vibration

**Mengapa Belt?**
- Absorb vibration dari motor
- Quiet operation critical untuk machining precision
- Easy adjust speed dengan change pulley

### 4.4 Servo Robot: Zero-Backlash Requirement
**Aplikasi:** Electronics assembly robot, precision pick & place

**Spesifikasi:**
- Backlash requirement: < 1 arcmin (0.0167°)
- Repeatability: ± 0.01 mm
- Solution: Harmonic drive atau timing belt dengan preload

**Challenge:**
- Standard gear backlash ~0.1° (too large)
- Custom zero-backlash gear expensive
- Timing belt dengan tensioner: compromise cost vs performance

---

## 5. RINGKASAN

### Key Takeaways:

1. **Mechanism Design Tradeoff**
   - Cost vs Performance: Spur gear murah tapi noisy, planetary expensive tapi smooth
   - Simplicity vs Precision: Belt simple tapi slip possible, gear precise tapi kompleks
   - Compact vs Efficiency: Worm gear compact tapi efficiency rendah

2. **Backlash Critical untuk Accuracy**
   - Measure backlash dengan hysteresis test
   - Compensate di software (direction change detection + offset)
   - Atau hardware solution (preloaded bearing, zero-backlash gear)

3. **Workspace & Singularity**
   - Workspace analysis penting untuk verify robot dapat reach target
   - Singularity position dangerous (loss of control authority)
   - Avoid singularity region dalam path planning

4. **Selection Criteria**
   - Application requirement: speed, torsi, precision, cost
   - Environment: dust, temperature, lubrication availability
   - Maintenance: lifetime, replacement frequency, spare part availability

---

## 6. DESKRIPSI TUGAS VIDEO (MODUL 7)

### Struktur Video (15-20 menit)

#### Bagian 1: Pendahuluan (2 menit)
- Tujuan: Design mekanisme untuk robot application
- Problem statement: Motor high-speed, robot butuh high-torque low-speed
- Solution: Gear/belt/screw transmission

#### Bagian 2: Mechanism Selection (3 menit)
**Point yang HARUS dijelaskan:**
- Untuk aplikasi ini (robot arm / AGV / pick&place), gear/belt/screw mana yang cocok?
- Trade-off analysis: cost, performance, complexity
- Selection justification: mengapa pilih planetary gear? (compact, high ratio, smooth)

#### Bagian 3: Gear Ratio Calculation (3 menit)
**Point yang HARUS dijelaskan:**
- Contoh: Motor 1000 RPM → requirement output 100 RPM
- Gear ratio = 1000/100 = 10:1
- Teeth calculation: input 10 teeth → output 100 teeth
- Torsi multiplication: input 0.5 Nm → output 5 Nm (ideal)
- Efficiency consideration: actual output torque = 5 Nm × 0.9 = 4.5 Nm

#### Bagian 4: Fusion 360 Design (4 menit)
**Point yang HARUS dijelaskan:**
- 3D model assembly: gear, shaft, bearing, frame
- Constraint: revolute joint, fixed joint
- Motion study: simulate mechanism rotation
- Workspace visualization: trace end-effector path
- Export drawing: assembly drawing, part list

#### Bagian 5: Backlash Measurement & Compensation (4 menit)
**Point yang HARUS dijelaskan:**
- Measurement procedure: fix output, rotate input back & forth
- Hysteresis loop plot: input angle vs output position
- Backlash value: width of hysteresis = 0.2° (example)
- Compensation strategy: detect direction change → add offset 0.2°
- Result: position accuracy improve 80% (before 0.2° error → after 0.04° error)

#### Bagian 6: Workspace & Singularity (2 menit)
**Point yang HARUS dijelaskan:**
- Workspace visualization: area reachable (green) vs not reachable (red)
- Singularity position: arm fully extended (Det(J) ≈ 0)
- Risk: saat singular, arm lose control dalam certain direction
- Solution: avoid singular configuration dalam path planning

#### Bagian 7: Fabrication & Testing (2 menit)
**Point yang HARUS dijelaskan:**
- Part manufacturing: 3D print / machining / order dari supplier
- Assembly procedure: bearing install, shaft alignment, gear meshing check
- Load test: apply 0-10 kg load, measure deformation
- Result: max load 5 kg untuk maintain accuracy < 1 mm

#### Kesimpulan (1 menit)
- Mechanism design balance cost, performance, complexity
- Backlash measurement & compensation critical
- Test & validation confirm specification

---

## Referensi
1. Norton, R. L. (2020). *Design of Machinery*. McGraw-Hill Education.
2. Budynas, R. G., & Nisbett, J. K. (2020). *Shigley's Mechanical Engineering Design*. McGraw-Hill.
3. Craig, J. J. (2017). *Introduction to Robotics: Mechanics and Control*. Pearson.
4. ABB Robotics. (2021). *Technical Reference Manual - Gearbox Selection*.

---

**Catatan:** Dokumen ini adalah materi pembelajaran untuk Modul 7. Untuk implementasi praktikum, lihat JOBSHEET.md. Untuk project akhir, lihat PROJECT.md.
