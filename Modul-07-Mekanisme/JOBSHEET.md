# JOBSHEET MODUL 7: MEKANISME ROBOT & TRANSMISI

## INFORMASI PRAKTIKUM
- **Modul:** 7
- **Topik:** Mekanisme Robot & Transmisi
- **Durasi:** 4 jam (2 pertemuan @ 2 jam)
- **Prasyarat:** Modul 1-6 (Hardware platform, motor control)

---

## 1. TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:
1. Mendesain mekanisme transmisi (gear/belt/screw) sesuai requirement
2. Menganalisis gear ratio dan efficiency
3. Membuat model 3D CAD (Fusion 360) dengan assembly constraint
4. Melakukan workspace & singularity analysis
5. Fabrikasi dan assembly mechanism
6. Mengukur dan menganalisis performance (backlash, compliance, load capacity)

---

## 2. ALAT & BAHAN

### 2.1 Hardware
- [ ] Motor DC/BLDC dari Modul 5 (dengan encoder)
- [ ] Gearbox / belt & pulley / lead screw (sesuai design)
- [ ] Bearing (2-4 pcs, tergantung design)
- [ ] Frame / mounting bracket
- [ ] Coupler / shaft connector
- [ ] Precision measuring tool: caliper, micrometer, dial indicator
- [ ] Load test equipment: hanging mass (0-10 kg) atau friction brake

### 2.2 Software & Tools
- [ ] Fusion 360 (CAD software)
- [ ] 3D printer (untuk fabricate custom part) **atau** CNC machining
- [ ] Python 3.10+ (untuk workspace visualization)
- [ ] Libraries: numpy, matplotlib
- [ ] Spreadsheet (untuk data collection)

---

## 3. LANGKAH KERJA

### PERCOBAAN 1: MECHANISM DESIGN (FUSION 360)

#### 3.1.1 Tujuan
Membuat 3D CAD model mekanisme transmisi dengan assembly constraint dan motion study.

#### 3.1.2 Prosedur
1. **Buka Fusion 360**, create new design
2. **Design gear/pulley/screw** sesuai requirement:
   - Tentukan gear ratio target (contoh: 10:1)
   - Calculate teeth: input 10 teeth → output 100 teeth
   - Module gear: 1 mm atau 2 mm (standard)
   - Pressure angle: 20° (standard)

3. **Create 3D model component:**
   - Input gear/pulley
   - Output gear/pulley
   - Shaft (diameter sesuai torque)
   - Bearing housing
   - Frame/bracket

4. **Assembly constraint:**
   - **Revolute joint:** Untuk shaft rotation
   - **Rigid joint:** Untuk gear ke shaft
   - **Contact set:** Untuk gear meshing
   
5. **Motion study:**
   - Apply rotational input ke input shaft (360°)
   - Simulate motion
   - Verify output rotation = input / gear_ratio
   - Trace end-effector path (untuk robot arm)

6. **Workspace analysis:**
   - Vary joint angle (0-180° atau sesuai limit)
   - Record end-effector position
   - Generate workspace plot

#### 3.1.3 Data Pengamatan

**Tabel 1.1: Design Parameter**

| Parameter | Value | Unit | Keterangan |
|-----------|-------|------|------------|
| Input gear teeth | _____ | teeth | |
| Output gear teeth | _____ | teeth | |
| Gear ratio (theory) | _____ | - | N_output / N_input |
| Module | _____ | mm | Gear size parameter |
| Pressure angle | 20 | deg | Standard |
| Link length (if arm) | _____ | mm | |

**Tabel 1.2: Workspace Analysis**

| Joint Angle (deg) | End-Effector X (mm) | End-Effector Y (mm) | Reachable? |
|-------------------|---------------------|---------------------|------------|
| [0, 0] | _____ | _____ | ✓ / ✗ |
| [45, 45] | _____ | _____ | ✓ / ✗ |
| [90, 0] | _____ | _____ | ✓ / ✗ |
| [180, 0] | _____ | _____ | ✓ / ✗ |

#### 3.1.4 Deliverable
- [ ] CAD model file (.f3d)
- [ ] Assembly drawing (screenshot/PDF)
- [ ] Workspace plot image
- [ ] Singularity position identified

---

### PERCOBAAN 2: GEAR RATIO & EFFICIENCY ANALYSIS

#### 3.2.1 Tujuan
Mengukur gear ratio aktual dan efficiency transmisi.

#### 3.2.2 Prosedur
1. **Setup measurement:**
   - Mount motor dengan encoder di input shaft
   - Mount encoder di output shaft (atau count rotation manual)
   - Connect motor driver ke power supply

2. **Theoretical calculation:**
   ```
   Gear Ratio (theory) = N_output / N_input
   Expected output RPM = Input RPM / Gear Ratio
   ```

3. **Measurement:**
   - Set motor speed constant (contoh: 1000 RPM)
   - Measure output speed dengan encoder atau tachometer
   - Record data untuk 5 different input speed

4. **Efficiency calculation:**
   ```
   Input Power = Input Torque × Input Speed (Nm × rad/s)
   Output Power = Output Torque × Output Speed
   Efficiency = (Output Power / Input Power) × 100%
   ```
   
   - Apply load ke output (hanging mass atau brake)
   - Measure input current → estimate input torque
   - Calculate output torque = Load × radius
   - Calculate efficiency

#### 3.2.3 Data Pengamatan

**Tabel 2.1: Gear Ratio Measurement**

| Test | Input RPM | Output RPM | Ratio (theory) | Ratio (actual) | Error (%) |
|------|-----------|------------|----------------|----------------|-----------|
| 1 | 500 | _____ | 10.0 | _____ | _____ |
| 2 | 1000 | _____ | 10.0 | _____ | _____ |
| 3 | 1500 | _____ | 10.0 | _____ | _____ |
| 4 | 2000 | _____ | 10.0 | _____ | _____ |
| 5 | 2500 | _____ | 10.0 | _____ | _____ |

**Tabel 2.2: Efficiency Measurement**

| Test | Input Power (W) | Output Power (W) | Efficiency (%) | Loss Source |
|------|-----------------|------------------|----------------|-------------|
| No Load | _____ | 0 | 0 | Friction only |
| Load 1kg | _____ | _____ | _____ | Friction + mesh |
| Load 2kg | _____ | _____ | _____ | Friction + mesh |
| Load 5kg | _____ | _____ | _____ | Friction + mesh |

#### 3.2.4 Analisis
- Apakah ratio aktual mendekati theoretical? (error < 5% acceptable)
- Efficiency mencapai target? (> 85% untuk planetary, > 90% untuk spur)
- Apa penyebab loss terbesar? (bearing friction, gear mesh, seal friction)

#### 3.2.5 Deliverable
- [ ] Measurement data (table filled)
- [ ] Efficiency analysis report
- [ ] Comparison theory vs practice

---

### PERCOBAAN 3: BACKLASH MEASUREMENT

#### 3.3.1 Tujuan
Mengukur backlash mekanisme menggunakan hysteresis test.

#### 3.3.2 Prosedur
1. **Setup:**
   - Fix output shaft (clamp atau brake)
   - Mount dial indicator ke output untuk measure displacement
   - Zero dial indicator

2. **Hysteresis test:**
   - Rotate input shaft CW slowly (+1°, +2°, +3°, ...)
   - Record output displacement (should be 0, but will have error)
   - Rotate input shaft CCW slowly (-1°, -2°, -3°, ...)
   - Record output displacement
   
3. **Backlash calculation:**
   ```
   Backlash = |Output_CW - Output_CCW| (at same input angle)
   ```

4. **Plot hysteresis loop:**
   - X-axis: Input angle
   - Y-axis: Output position
   - Loop width = Backlash

#### 3.3.3 Data Pengamatan

**Tabel 3.1: Hysteresis Measurement**

| Input Angle (deg) | Output Pos CW (mm) | Output Pos CCW (mm) | Backlash (mm) |
|-------------------|--------------------|---------------------|---------------|
| 1 | _____ | _____ | _____ |
| 2 | _____ | _____ | _____ |
| 3 | _____ | _____ | _____ |
| 4 | _____ | _____ | _____ |
| 5 | _____ | _____ | _____ |

**Backlash (average):** _____ mm atau _____ degree

#### 3.3.4 Plot

```
Output Position (mm)
    ▲
    │      CW motion
    │     ╱
    │    ╱│
    │   │ │  ← Backlash width
    │   │╱
    │    CCW motion
    └────────────────► Input Angle (deg)
```

Sketch atau insert image dari actual measurement.

#### 3.3.5 Deliverable
- [ ] Hysteresis plot image
- [ ] Backlash measurement value
- [ ] Analysis: Apakah backlash acceptable untuk aplikasi?

---

### PERCOBAAN 4: BACKLASH COMPENSATION

#### 3.4.1 Tujuan
Implementasi software compensation untuk reduce backlash error.

#### 3.4.2 Prosedur
1. **Direction change detection:**
   ```python
   last_target = 0
   last_direction = 1  # 1 = CW, -1 = CCW
   
   def detect_direction_change(target, current):
       global last_target, last_direction
       
       new_direction = 1 if (target - current) > 0 else -1
       
       direction_changed = (new_direction != last_direction)
       last_direction = new_direction
       last_target = target
       
       return direction_changed
   ```

2. **Add compensation offset:**
   ```python
   BACKLASH_OFFSET = 0.2  # degree (from measurement)
   
   def compensate_backlash(target, current):
       if detect_direction_change(target, current):
           # Add offset in direction of motion
           compensated = target + BACKLASH_OFFSET * sign(target - current)
           return compensated
       else:
           return target
   ```

3. **Test compensation:**
   - Command position: 0° → 90° → 0° → 90° (multiple cycle)
   - **Without compensation:** Measure error saat each reversal
   - **With compensation:** Measure error after compensation
   - Compare accuracy improvement

#### 3.4.3 Data Pengamatan

**Tabel 4.1: Position Accuracy Before/After Compensation**

| Target (deg) | Actual (No Comp) | Error (No Comp) | Actual (Comp) | Error (Comp) | Improvement |
|--------------|------------------|-----------------|---------------|--------------|-------------|
| 0 → 90 | _____ | _____ | _____ | _____ | _____ % |
| 90 → 0 | _____ | _____ | _____ | _____ | _____ % |
| 0 → 45 | _____ | _____ | _____ | _____ | _____ % |
| 45 → 90 | _____ | _____ | _____ | _____ | _____ % |

**Average improvement:** _____ %

#### 3.4.4 Deliverable
- [ ] Compensation code (Python atau firmware)
- [ ] Position accuracy plot (before/after)
- [ ] Improvement quantified (percentage)

---

### PERCOBAAN 5: LOAD CAPACITY TEST

#### 3.5.1 Tujuan
Determine maximum load capacity dengan measure deformation, efficiency drop, dan temperature rise.

#### 3.5.2 Prosedur
1. **Load test setup:**
   - Attach load ke output (hanging mass atau friction brake)
   - Mount displacement sensor untuk measure deformation
   - Monitor motor current & voltage
   - Monitor temperature (infrared thermometer atau thermocouple)

2. **Incremental load test:**
   - Start: No load (0 kg)
   - Increment: 1 kg setiap step
   - Maximum: 10 kg atau sampai failure/excessive deformation

3. **Measurement setiap load:**
   - Deformation (mm): displacement dari zero-load position
   - Efficiency (%): calculate dari power measurement
   - Temperature (°C): motor housing temperature
   - Run duration: 2 minute per load untuk thermal stabilization

4. **Acceptance criteria:**
   - Deformation < 1 mm (untuk precision application)
   - Efficiency > 70% (untuk energy consideration)
   - Temperature < 80°C (untuk motor safety)

#### 3.5.3 Data Pengamatan

**Tabel 5.1: Load Capacity Test**

| Load (kg) | Deformation (mm) | Efficiency (%) | Temperature (°C) | Note |
|-----------|------------------|----------------|------------------|------|
| 0 | 0.00 | - | _____ | Baseline |
| 1 | _____ | _____ | _____ | |
| 2 | _____ | _____ | _____ | |
| 3 | _____ | _____ | _____ | |
| 4 | _____ | _____ | _____ | |
| 5 | _____ | _____ | _____ | |
| 6 | _____ | _____ | _____ | |
| 7 | _____ | _____ | _____ | |
| 8 | _____ | _____ | _____ | |
| 9 | _____ | _____ | _____ | |
| 10 | _____ | _____ | _____ | |

**Maximum acceptable load:** _____ kg (based on deformation < 1mm criteria)

#### 3.5.4 Plot

**Plot 1: Load vs Deformation (Compliance Curve)**
```
Deformation (mm)
    ▲
    │        ╱
    │       ╱
    │      ╱
    │     ╱
    │    ╱
    │   ╱
    └────────────► Load (kg)
```

**Plot 2: Load vs Efficiency**
```
Efficiency (%)
    ▲
100 │────╲
    │     ╲
 80 │      ╲___
    │           ╲___
 60 │               ╲___
    └────────────────────► Load (kg)
```

#### 3.5.5 Deliverable
- [ ] Load test data table (complete)
- [ ] Compliance curve plot
- [ ] Load limit recommendation dengan justification

---

## 4. DATA PENGAMATAN LENGKAP

### 4.1 CAD Model
- [ ] Screenshot 3D model (isometric view)
- [ ] Screenshot assembly constraint setup
- [ ] Motion study animation (exported as GIF atau video)

### 4.2 Workspace Visualization
- [ ] Workspace plot image (2D atau 3D)
- [ ] Reachable area highlighted
- [ ] Singularity region identified

### 4.3 Performance Data
- [ ] Gear ratio measurement (theory vs actual)
- [ ] Efficiency data (all load condition)
- [ ] Backlash hysteresis plot
- [ ] Load vs deformation curve
- [ ] Position accuracy before/after compensation

---

## 5. ANALISIS

### 5.1 Pertanyaan Analisis

1. **Efficiency Analysis:**
   - Apakah efficiency mencapai target (> 85% untuk planetary, > 90% untuk spur)?
   - Jika tidak, apa penyebab utama loss? (friction bearing, gear mesh, seal)
   - Bagaimana cara meningkatkan efficiency? (lubrication, bearing quality, alignment)

2. **Backlash Analysis:**
   - Berapa backlash yang terukur (dalam degree dan mm)?
   - Apakah backlash acceptable untuk aplikasi (< 0.1° untuk precision robot)?
   - Apakah compensation effective? (improvement berapa %?)

3. **Load Capacity:**
   - Berapa maximum load untuk maintain deformation < 1 mm?
   - Apakah load capacity meet requirement?
   - Safety factor berapa yang digunakan? (recommended: 2-3×)

4. **Workspace Analysis:**
   - Apakah workspace sufficient untuk task?
   - Di posisi mana terjadi singularity?
   - Apakah singularity region perlu dihindari dalam operation?

5. **Design Trade-off:**
   - Trade-off antara cost, performance, dan complexity?
   - Jika budget terbatas, component mana yang dapat diganti dengan alternatif lebih murah?
   - Jika precision critical, component mana yang perlu upgrade?

### 5.2 Format Analisis

Tulis analisis dalam format:
```
ANALISIS [Nama Percobaan]

1. Hasil Pengamatan:
   - [Ringkasan data]
   
2. Perbandingan dengan Teori/Target:
   - [Theory vs practice comparison]
   
3. Penyebab Deviasi:
   - [Root cause analysis]
   
4. Rekomendasi Improvement:
   - [Actionable suggestion]
```

---

## 6. KESIMPULAN

Tulis kesimpulan yang mencakup:

1. **Pencapaian Tujuan:**
   - Apakah mechanism design berhasil sesuai requirement?
   - Apakah semua measurement complete dan valid?

2. **Lesson Learned:**
   - Insight penting tentang mechanism selection
   - Backlash impact pada control accuracy
   - Trade-off dalam design decision

3. **Aplikasi Praktis:**
   - Bagaimana knowledge ini applicable untuk robot application?
   - Best practice dalam mechanism design dan testing

---

## 7. TUGAS TAMBAHAN (OPTIONAL)

### 7.1 Advanced Analysis
- [ ] Finite Element Analysis (FEA) untuk stress analysis (Fusion 360 simulation)
- [ ] Dynamic simulation dengan load (motion study dengan force applied)
- [ ] Vibration analysis (measure vibration dengan accelerometer)

### 7.2 Optimization
- [ ] Test different lubrication type (dry, oil, grease)
- [ ] Test different gear material (plastic, steel, brass)
- [ ] Optimize gear ratio untuk specific task (trade-off speed vs torque)

### 7.3 Documentation
- [ ] Create mechanism selection guide (decision tree)
- [ ] Bill of materials (BOM) dengan cost estimation
- [ ] Assembly instruction document (step-by-step dengan photo)

---

## ASSESSMENT RUBRIC

| Kriteria | Bobot | Keterangan |
|----------|-------|------------|
| CAD model quality | 15% | Design complete, constraint correct, motion study working |
| Measurement accuracy | 20% | Data collection systematic, precision tool used properly |
| Backlash analysis | 15% | Hysteresis plot clear, compensation implemented |
| Load test | 15% | Complete data, compliance curve, load limit identified |
| Analysis depth | 20% | Root cause identified, comparison theory vs practice |
| Documentation | 10% | Report clear, structured, data visualization good |
| Video presentation | 5% | Explain concept clear, demo working mechanism |

**Total:** 100%

---

## LAMPIRAN

### A. Code Template: Workspace Visualization (Python)

```python
import numpy as np
import matplotlib.pyplot as plt

# Robot parameter (2-DOF planar arm)
L1 = 200  # mm, link 1 length
L2 = 150  # mm, link 2 length

def forward_kinematics(theta1, theta2):
    """Calculate end-effector position"""
    x = L1 * np.cos(theta1) + L2 * np.cos(theta1 + theta2)
    y = L1 * np.sin(theta1) + L2 * np.sin(theta1 + theta2)
    return x, y

def workspace_map():
    """Generate workspace visualization"""
    positions = []
    
    # Sample joint angles
    for theta1 in np.linspace(0, 2*np.pi, 36):  # 10° increment
        for theta2 in np.linspace(-np.pi, np.pi, 36):  # 10° increment
            x, y = forward_kinematics(theta1, theta2)
            positions.append([x, y])
    
    positions = np.array(positions)
    
    # Plot
    plt.figure(figsize=(8, 8))
    plt.scatter(positions[:, 0], positions[:, 1], alpha=0.3, s=1)
    plt.xlabel('X (mm)')
    plt.ylabel('Y (mm)')
    plt.title(f'Robot Workspace (L1={L1}mm, L2={L2}mm)')
    plt.grid(True)
    plt.axis('equal')
    
    # Add robot base
    plt.plot(0, 0, 'ro', markersize=10, label='Base')
    plt.legend()
    
    plt.savefig('workspace.png', dpi=300)
    plt.show()

if __name__ == '__main__':
    workspace_map()
```

### B. Backlash Compensation Code

```python
import time

class BacklashCompensator:
    def __init__(self, backlash_offset=0.2):
        """
        Initialize backlash compensator
        
        Args:
            backlash_offset: Measured backlash in degrees
        """
        self.backlash_offset = backlash_offset
        self.last_target = 0
        self.last_direction = 1
    
    def compensate(self, target_position):
        """
        Apply backlash compensation
        
        Args:
            target_position: Desired position (degrees)
            
        Returns:
            compensated_position: Adjusted position accounting for backlash
        """
        # Determine direction
        direction = 1 if target_position > self.last_target else -1
        
        # Detect direction change
        direction_changed = (direction != self.last_direction)
        
        # Apply compensation if direction changed
        if direction_changed:
            compensated = target_position + self.backlash_offset * direction
            print(f"Direction change detected! Compensating: {target_position}° → {compensated}°")
        else:
            compensated = target_position
        
        # Update state
        self.last_target = target_position
        self.last_direction = direction
        
        return compensated

# Usage example
compensator = BacklashCompensator(backlash_offset=0.2)

# Test sequence
targets = [0, 90, 45, 135, 0]
for target in targets:
    compensated = compensator.compensate(target)
    # Send compensated position to motor
    print(f"Target: {target}° → Compensated: {compensated}°")
```

---

## REFERENSI
1. Norton, R. L. (2020). *Design of Machinery*. McGraw-Hill Education.
2. Budynas, R. G., & Nisbett, J. K. (2020). *Shigley's Mechanical Engineering Design*. McGraw-Hill.
3. Fusion 360 Help. *Assembly Constraints and Joints*. Autodesk.
4. ISO 6336. *Calculation of Load Capacity of Spur and Helical Gears*.

---

**Catatan Keselamatan:**
- Gunakan safety glasses saat operate rotating machinery
- Ensure proper guarding untuk prevent contact dengan moving parts
- Disconnect power sebelum assembly/disassembly
- Use proper tools untuk prevent damage ke precision components
