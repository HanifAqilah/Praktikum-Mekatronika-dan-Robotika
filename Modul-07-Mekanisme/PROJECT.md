# PROJECT MODUL 7: PRECISION GEARBOX DESIGN & OPTIMIZATION

## INFORMASI PROJECT
- **Judul:** Precision Gearbox Design & Optimization
- **Durasi:** 2-3 minggu
- **Tim:** 2-3 orang
- **Deliverable:** Design documentation, prototype, performance report, video demo

---

## 1. DESKRIPSI PROJECT

### 1.1 Latar Belakang
Dalam aplikasi robotika industri, pemilihan dan design gearbox yang tepat sangat critical untuk mencapai performance yang diinginkan. Gearbox harus balance antara:
- **Speed reduction** (untuk high-torque application)
- **Precision** (low backlash untuk accuracy)
- **Efficiency** (minimize energy loss)
- **Cost** (affordable untuk production)
- **Reliability** (long lifetime, low maintenance)

Project ini menantang mahasiswa untuk design, fabricate, test, dan optimize gearbox untuk specific robot application.

### 1.2 Tujuan Project
1. Develop systematic approach untuk gearbox selection dan design
2. Implement CAD design dengan consideration untuk manufacturability
3. Prototype fabrication dan assembly
4. Comprehensive performance testing dan optimization
5. Create reusable gearbox selection guide

---

## 2. SPESIFIKASI REQUIREMENTS

### 2.1 Application Scenario (Pilih Salah Satu)

#### Option A: Robot Arm Joint
- **Input:** BLDC motor 3000 RPM, 0.5 Nm
- **Output requirement:** 30 RPM, 50 Nm
- **Precision:** Backlash < 0.1° (< 2 arcmin)
- **Workspace:** Joint rotation 0-180°
- **Payload:** 5 kg at 300 mm arm length

#### Option B: AGV Wheel Drive
- **Input:** DC motor 1500 RPM, 0.3 Nm
- **Output requirement:** 60 RPM, 7.5 Nm
- **Precision:** Backlash < 1° (navigation acceptable)
- **Workspace:** Continuous rotation (wheel)
- **Payload:** 50 kg robot weight

#### Option C: Linear Actuator (Lead Screw)
- **Input:** Stepper motor 200 steps/rev, 1500 RPM
- **Output requirement:** 10 mm/s linear speed, 500 N force
- **Precision:** < 0.1 mm positioning error
- **Workspace:** 500 mm linear travel
- **Payload:** 20 kg vertical load

### 2.2 Performance Target

| Kriteria | Target | Measurement Method |
|----------|--------|--------------------|
| Gear ratio accuracy | Error < 5% | RPM measurement |
| Efficiency | > 85% | Power measurement |
| Backlash | < 0.1° (robot arm) atau < 1° (AGV) | Hysteresis test |
| Load capacity | Meet payload requirement dengan safety factor 2 | Load test |
| Noise | < 70 dB at 1 meter | Sound level meter |
| Lifetime | > 10,000 cycles | Endurance test (optional) |

---

## 3. DELIVERABLE

### 3.1 Requirement Specification Document
**Format:** PDF, 2-3 pages

**Isi:**
- Application scenario description
- Input/output specification (speed, torque, precision)
- Constraint (size, cost, availability)
- Performance target (dengan justification)
- Success criteria

### 3.2 Gear Selection Analysis
**Format:** PDF atau spreadsheet, 3-5 pages

**Isi:**
- Candidate comparison table (minimum 3 option):
  - Spur gear
  - Planetary gear
  - Belt & pulley
  - Lead screw (if applicable)
- Comparison criteria:
  - Gear ratio achievable
  - Backlash typical
  - Efficiency
  - Cost (estimated)
  - Complexity (assembly, maintenance)
  - Availability (commercial atau custom)
- **Selection decision** dengan justification

**Tabel Template:**

| Gear Type | Ratio | Backlash | Efficiency | Cost | Complexity | Score |
|-----------|-------|----------|------------|------|------------|-------|
| Spur | 10:1 | 0.2° | 92% | Low | Low | _____ |
| Planetary | 10:1 | 0.05° | 88% | High | High | _____ |
| Timing Belt | 10:1 | 0.01° | 95% | Medium | Medium | _____ |

**Scoring:** Weight setiap criteria sesuai application priority.

### 3.3 CAD Design (Fusion 360)
**Format:** .f3d file + PDF drawing

**Isi:**
- Complete 3D assembly model
- Individual part models
- Assembly constraint documentation
- Motion study result (animation)
- Engineering drawing:
  - Assembly drawing (overall view)
  - Part drawing (dengan dimension, tolerance)
  - Bill of Materials (BOM)

**Requirements:**
- Proper constraint (revolute, rigid, contact)
- Realistic dimension (based on commercial component atau machineable)
- Material assignment (steel, aluminum, plastic)
- Mass & inertia calculation (Fusion 360 automatic)

### 3.4 Backlash Analysis & Compensation Strategy
**Format:** PDF, 2-3 pages + code

**Isi:**
- Backlash source identification (gear, bearing, shaft)
- Measurement procedure (hysteresis test)
- Measurement result (backlash value)
- Compensation strategy:
  - Software compensation (direction change detection + offset)
  - Hardware solution (preloaded bearing, zero-backlash gear)
- Implementation code (Python atau firmware)
- Test result (before/after accuracy comparison)

**Code Deliverable:**
```python
# backlash_compensator.py
class BacklashCompensator:
    """Complete implementation dengan test"""
    pass
```

### 3.5 Performance Test Report
**Format:** PDF, 5-7 pages

**Isi:**
1. **Test Setup:**
   - Hardware configuration
   - Measurement equipment
   - Test procedure

2. **Test Result:**
   - Gear ratio measurement (table + graph)
   - Efficiency measurement (multiple load condition)
   - Backlash measurement (hysteresis plot)
   - Load capacity test (compliance curve)
   - Temperature rise test (thermal profile)

3. **Analysis:**
   - Compare result dengan target
   - Identify performance bottleneck
   - Root cause analysis untuk any deviation

4. **Optimization:**
   - Improvement implemented (lubrication, alignment, tuning)
   - Before/after comparison

### 3.6 Fabrication Documentation
**Format:** PDF, 3-5 pages dengan photo

**Isi:**
- Part list (BOM) dengan supplier info
- Manufacturing method (3D print, CNC machining, order)
- Assembly procedure (step-by-step dengan photo):
  1. Bearing installation
  2. Shaft alignment
  3. Gear meshing check
  4. Frame assembly
  5. Final check (backlash, rotation smoothness)
- Tools required
- Assembly time estimate
- Cost breakdown

### 3.7 Cost Analysis & Trade-off Study
**Format:** Spreadsheet + PDF summary, 2-3 pages

**Isi:**
- Cost breakdown:
  - Component cost (gear, bearing, frame, etc)
  - Manufacturing cost (3D print, machining)
  - Assembly time cost
  - Total cost
- **Trade-off analysis:**
  - Cost vs Performance curve
  - Identify optimization point (best value)
- **Alternative analysis:**
  - If budget 50% → what to change? (impact on performance?)
  - If precision requirement 2× stricter → what component upgrade? (cost increase?)

### 3.8 Gearbox Selection Guide Document
**Format:** PDF, 5-10 pages (reference document)

**Isi:**
- Decision tree untuk gear type selection
- Quick reference table (gear type vs application)
- Design checklist
- Common pitfall & troubleshooting
- Vendor recommendation (local supplier, online)
- Cost estimation guideline

**Target audience:** Junior engineer, future student

### 3.9 Video Demo
**Format:** MP4, 10-15 menit

**Konten:**
1. **Introduction (1 min):**
   - Team member introduction
   - Application scenario

2. **CAD Design (3 min):**
   - Show 3D model di Fusion 360
   - Explain assembly constraint
   - Motion study demo
   - Workspace visualization

3. **Fabrication (2 min):**
   - Show manufacturing process (3D print atau machining)
   - Assembly time-lapse
   - Final assembled gearbox

4. **Performance Test (4 min):**
   - Gear ratio measurement demo
   - Efficiency test setup & result
   - Backlash hysteresis demo
   - Load test demo (apply load, show deformation)

5. **Result & Analysis (3 min):**
   - Performance summary (table)
   - Comparison theory vs actual
   - Lesson learned

6. **Conclusion (1 min):**
   - Achievement summary
   - Recommendation untuk future improvement

**Video Quality:**
- Resolution: Minimum 1080p
- Audio: Clear narration (Indonesia atau English)
- Subtitle: Recommended untuk technical term
- Editing: Cut boring waiting time, highlight key moment

---

## 4. TIMELINE & MILESTONES

### Week 1: Design & Selection
- [ ] Day 1-2: Requirement analysis, gear selection
- [ ] Day 3-5: CAD design (Fusion 360)
- [ ] Day 6-7: Design review, finalize design

**Milestone 1:** CAD design complete, BOM finalized

### Week 2: Fabrication & Assembly
- [ ] Day 8-10: Part fabrication (3D print atau order)
- [ ] Day 11-12: Assembly
- [ ] Day 13-14: Initial testing, troubleshooting

**Milestone 2:** Physical prototype assembled, basic function tested

### Week 3: Testing & Optimization
- [ ] Day 15-16: Performance test (ratio, efficiency, backlash, load)
- [ ] Day 17-18: Optimization (lubrication, alignment, compensation)
- [ ] Day 19-20: Re-test, documentation
- [ ] Day 21: Video recording & editing

**Milestone 3:** Complete test report, video demo, all deliverable ready

---

## 5. GRADING RUBRIC

### 5.1 Technical Quality (50%)

| Aspek | Poin | Kriteria |
|-------|------|----------|
| Design completeness | 10 | CAD model complete, assembly constraint correct |
| Analysis depth | 10 | Gear ratio, efficiency, backlash, workspace analyzed |
| Measurement accuracy | 10 | Data collection systematic, proper tool usage |
| Optimization effort | 10 | Improvement implemented, quantified |
| Performance achievement | 10 | Meet target specification (ratio, efficiency, backlash) |

### 5.2 Documentation (25%)

| Aspek | Poin | Kriteria |
|-------|------|----------|
| Report clarity | 10 | Well-structured, clear explanation |
| Data visualization | 5 | Graph, plot, table informative |
| Code quality | 5 | Clean code, commented, tested |
| Fabrication doc | 5 | Assembly procedure clear, reproducible |

### 5.3 Innovation & Bonus (15%)

| Aspek | Poin | Kriteria |
|-------|------|----------|
| Zero-backlash design | 5 | Implement backlash-free mechanism |
| Efficiency optimization | 5 | Lubrication test, material selection |
| Compliance reduction | 5 | Stiffness analysis, material optimization |

### 5.4 Presentation (10%)

| Aspek | Poin | Kriteria |
|-------|------|----------|
| Video quality | 5 | Clear, informative, professional editing |
| Demo effectiveness | 5 | Working prototype, test result clear |

**Total:** 100%

**Grade:**
- A: 85-100 (Excellent, meet all target + bonus)
- B: 70-84 (Good, meet most target)
- C: 60-69 (Satisfactory, meet minimum requirement)
- D: 50-59 (Need improvement)
- E: < 50 (Incomplete atau major issue)

---

## 6. BONUS CHALLENGES

### 6.1 Backlash-Free Design (5 point)
**Challenge:** Design mechanism dengan backlash < 0.01°

**Approach:**
- Zero-backlash gear (split gear dengan spring preload)
- Preloaded bearing (eliminate bearing play)
- Anti-backlash nut (untuk lead screw)

**Deliverable:**
- Design documentation
- Backlash measurement < 0.01°
- Cost analysis (vs standard design)

### 6.2 Efficiency Optimization (5 point)
**Challenge:** Achieve efficiency > 90%

**Approach:**
- Lubrication type test (dry, oil, grease, PTFE)
- Bearing selection (ball bearing vs sleeve bearing)
- Gear material selection (steel, brass, nylon)

**Deliverable:**
- Test matrix (all combination tested)
- Efficiency comparison table
- Recommendation dengan cost-benefit analysis

### 6.3 Compliance Reduction (5 point)
**Challenge:** Reduce deformation < 0.5 mm under max load

**Approach:**
- Material selection (steel vs aluminum, stiffness comparison)
- FEA analysis (Fusion 360 simulation)
- Structure optimization (rib, thicker section)

**Deliverable:**
- FEA result (stress, deformation plot)
- Material comparison
- Optimized design dengan stiffness improved 50%

### 6.4 Multi-Stage Gearbox (5 point)
**Challenge:** Design 2-stage atau 3-stage gearbox untuk achieve high ratio (> 50:1)

**Approach:**
- Stage 1: Spur gear 5:1
- Stage 2: Planetary gear 10:1
- Total ratio: 50:1

**Deliverable:**
- Multi-stage CAD model
- Efficiency analysis setiap stage (compound efficiency)
- Compact design (compare dengan single-stage equivalent)

---

## 7. DELIVERABLE CHECKLIST

### Required Deliverable (Minimum untuk Pass)
- [ ] Requirement specification document
- [ ] Gear selection analysis (minimum 3 candidate)
- [ ] CAD design (Fusion 360, validated)
- [ ] Backlash analysis & compensation strategy
- [ ] Performance test report (ratio, efficiency, backlash, load)
- [ ] Fabrication documentation
- [ ] Video demo (10-15 min, semua test shown)

### Optional Deliverable (untuk Bonus Point)
- [ ] Cost analysis & trade-off study
- [ ] Gearbox selection guide document (reusable reference)
- [ ] Zero-backlash design implementation
- [ ] Efficiency optimization study
- [ ] FEA simulation
- [ ] Endurance test (1000+ cycle)

---

## 8. SUBMISSION GUIDELINES

### 8.1 Format
- **Report:** PDF, well-structured dengan table of content
- **Code:** Python, well-commented, include README
- **CAD:** Fusion 360 file + exported STEP file (for compatibility)
- **Video:** MP4, upload ke YouTube (unlisted) atau cloud storage
- **Submission:** ZIP file atau GitHub repository link

### 8.2 Folder Structure
```
Modul_07_Project_[NamaKelompok]/
├── 01_Requirement/
│   └── requirement_spec.pdf
├── 02_Selection/
│   └── gear_selection_analysis.pdf
├── 03_CAD/
│   ├── gearbox_assembly.f3d
│   ├── gearbox_assembly.step
│   └── drawings/
│       ├── assembly_drawing.pdf
│       └── part_drawings/
├── 04_Analysis/
│   ├── backlash_analysis.pdf
│   └── workspace_visualization.png
├── 05_Code/
│   ├── workspace_map.py
│   ├── backlash_compensator.py
│   └── README.md
├── 06_Test/
│   ├── performance_test_report.pdf
│   ├── data/
│   │   ├── gear_ratio_data.csv
│   │   ├── efficiency_data.csv
│   │   └── backlash_data.csv
│   └── plots/
│       ├── efficiency_curve.png
│       ├── hysteresis_plot.png
│       └── compliance_curve.png
├── 07_Fabrication/
│   ├── fabrication_doc.pdf
│   ├── bom.xlsx
│   └── assembly_photos/
├── 08_Cost_Analysis/
│   └── cost_tradeoff_study.pdf
├── 09_Guide/
│   └── gearbox_selection_guide.pdf (optional)
├── 10_Video/
│   └── video_link.txt (YouTube link)
└── README.md (project summary)
```

### 8.3 Deadline
- **Week 1:** Requirement spec + gear selection analysis
- **Week 2:** CAD design + fabrication start
- **Week 3:** Testing complete, all deliverable submitted

---

## 9. EVALUATION CRITERIA

### 9.1 Technical Excellence (40 point)
- Design meet requirement specification? (10 pt)
- CAD model quality (constraint, motion study, drawing)? (10 pt)
- Performance test comprehensive (all metric measured)? (10 pt)
- Optimization implemented (compensation, tuning)? (10 pt)

### 9.2 Analysis & Documentation (30 point)
- Requirement analysis clear (selection justification)? (10 pt)
- Test report detailed (procedure, data, analysis)? (10 pt)
- Documentation complete (CAD, code, fabrication)? (10 pt)

### 9.3 Implementation (20 point)
- Prototype working (assembly correct, motion smooth)? (10 pt)
- Performance achieve target (ratio, efficiency, backlash)? (10 pt)

### 9.4 Presentation (10 point)
- Video demo clear (show design, fabrication, test)? (5 pt)
- Explanation accurate (technical concept correct)? (5 pt)

**Total:** 100 point

**Bonus:** Up to 20 point (zero-backlash, efficiency optimization, compliance reduction, multi-stage)

**Final Grade:** Total point (max 120) scaled to 0-100

---

## 10. TIPS & BEST PRACTICES

### 10.1 Design Phase
✅ **DO:**
- Start dengan requirement yang clear (speed, torque, precision)
- Consider manufacturability (can you make it atau order it?)
- Verify commercial gear availability (McMaster-Carr, SDP/SI, local supplier)
- Use standard module (1mm, 2mm) untuk easier sourcing

❌ **DON'T:**
- Design custom gear tanpa check manufacturability first
- Ignore backlash dalam design (address it early)
- Forget safety factor (use 2-3× untuk load calculation)

### 10.2 Fabrication Phase
✅ **DO:**
- Test fit component sebelum final assembly
- Use proper tools (bearing press, alignment jig)
- Document assembly process dengan photo
- Measure backlash immediately after assembly

❌ **DON'T:**
- Force fit component (can damage bearing atau gear)
- Over-tighten bolt (can cause deformation)
- Skip alignment check (cause efficiency loss, wear)

### 10.3 Testing Phase
✅ **DO:**
- Systematic test (one variable at a time)
- Multiple measurement (repeat 3× untuk average)
- Document unexpected behavior (vibration, noise, temperature)
- Plot data immediately (easier detect anomaly)

❌ **DON'T:**
- Test under extreme condition without ramp-up (can damage)
- Ignore safety (use guard, emergency stop)
- Skip calibration (verify measurement tool accuracy)

### 10.4 Documentation Phase
✅ **DO:**
- Write as you go (don't wait until end)
- Include negative result (failed attempt is learning)
- Clear graph dengan label, unit, legend
- Code commented (explain non-obvious logic)

❌ **DON'T:**
- Copy-paste text tanpa understand
- Use low-resolution image (minimum 300 dpi untuk print)
- Skip proofreading (typo, unit error)

---

## 11. COMMON PITFALLS & TROUBLESHOOTING

### 11.1 High Backlash
**Symptom:** Hysteresis loop width > 0.5°

**Possible Cause:**
- Gear tooth clearance too large
- Bearing play excessive
- Shaft misalignment

**Solution:**
- Tighten bearing preload
- Replace worn bearing
- Realign shaft (use alignment jig)
- Implement software compensation

### 11.2 Low Efficiency
**Symptom:** Efficiency < 70%

**Possible Cause:**
- Insufficient lubrication
- Bearing friction high
- Gear misalignment
- Seal drag

**Solution:**
- Apply proper lubrication (grease untuk gear, oil untuk bearing)
- Check bearing quality (replace dengan low-friction bearing)
- Align gear mesh (proper backlash, no binding)

### 11.3 Noise & Vibration
**Symptom:** Noise > 80 dB, vibration visible

**Possible Cause:**
- Gear teeth wear atau damage
- Imbalance (shaft atau gear not concentric)
- Resonance (operating speed = natural frequency)

**Solution:**
- Inspect gear teeth (replace if damaged)
- Balance rotating component
- Change operating speed (avoid resonance frequency)

### 11.4 Excessive Deformation
**Symptom:** Compliance > 2 mm under load

**Possible Cause:**
- Frame not rigid enough
- Bearing clearance large
- Shaft bending

**Solution:**
- Stiffen frame (add rib, thicker material)
- Use bearing dengan tighter tolerance
- Increase shaft diameter (stiffness ∝ diameter⁴)

---

## 12. REFERENSI & RESOURCES

### 12.1 Gear Design Calculator
- **Online:** [geargenerator.com](https://geargenerator.com) (free gear profile generator)
- **Software:** Fusion 360 built-in spur gear generator

### 12.2 Component Supplier
- **International:** McMaster-Carr, SDP/SI (Stock Drive Products)
- **Local:** [List supplier lokal untuk gear, bearing, shaft]

### 12.3 Learning Resources
- YouTube: "Gear Design Tutorial" (search: mechanism design, gear assembly)
- Fusion 360 Tutorial: Assembly constraint, motion study
- Documentation: DIN 3960 (Gear standard)

---

## LAMPIRAN

### A. Design Calculation Template

```python
# gear_design_calculator.py

import math

def calculate_gear_parameters(teeth_input, teeth_output, module):
    """
    Calculate gear parameters
    
    Args:
        teeth_input: Number of teeth on input gear
        teeth_output: Number of teeth on output gear
        module: Gear module (mm)
        
    Returns:
        dict with gear parameters
    """
    gear_ratio = teeth_output / teeth_input
    
    # Pitch diameter
    pitch_dia_input = teeth_input * module
    pitch_dia_output = teeth_output * module
    
    # Center distance
    center_distance = (pitch_dia_input + pitch_dia_output) / 2
    
    # Theoretical output speed
    def output_speed(input_rpm):
        return input_rpm / gear_ratio
    
    # Theoretical output torque
    def output_torque(input_torque, efficiency=0.90):
        return input_torque * gear_ratio * efficiency
    
    return {
        'gear_ratio': gear_ratio,
        'pitch_dia_input': pitch_dia_input,
        'pitch_dia_output': pitch_dia_output,
        'center_distance': center_distance,
        'output_speed': output_speed,
        'output_torque': output_torque
    }

# Example usage
params = calculate_gear_parameters(teeth_input=10, teeth_output=100, module=2)
print(f"Gear Ratio: {params['gear_ratio']}")
print(f"Center Distance: {params['center_distance']} mm")
print(f"Output speed at 1000 RPM input: {params['output_speed'](1000)} RPM")
print(f"Output torque at 0.5 Nm input: {params['output_torque'](0.5)} Nm")
```

### B. Performance Test Data Template (CSV)

```csv
test_id,input_rpm,output_rpm,gear_ratio_actual,input_torque_nm,output_torque_nm,efficiency_percent,temperature_c,note
1,500,49.8,10.04,0.5,4.2,84,35,Baseline
2,1000,99.5,10.05,0.5,4.1,82,42,Slight efficiency drop
3,1500,149.0,10.07,0.5,4.0,80,48,Temperature rising
```

---

**Good Luck dengan Project! 🚀⚙️**

**Contact:** Asisten Lab untuk troubleshooting, component sourcing assistance.
