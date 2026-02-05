# PROJECT MODUL 3: ROBUST POWER SYSTEM DESIGN

## INFORMASI PROJECT
- **Judul:** Robust Power Distribution System for Mobile Robot
- **Durasi:** 2 minggu
- **Tim:** 2-3 orang
- **Deliverable:** Power system design, PCB layout, noise analysis report, prototype

---

## 1. DESKRIPSI PROJECT

### 1.1 Latar Belakang
Power system yang reliable adalah fondasi dari sistem robotika yang handal. Noise dari motor, switching regulator, dan sumber EMI lainnya dapat menyebabkan:
- ADC reading error → sensor inaccurate
- Communication glitch → lost data
- MCU reset → system failure
- Servo jitter → inaccurate positioning

Project ini menantang mahasiswa untuk mendesain power system yang robust untuk mobile robot dengan multiple voltage rails dan comprehensive noise mitigation.

### 1.2 Tujuan Project
1. Design complete power distribution system
2. Implement effective noise filtering
3. Create proper PCB layout untuk EMC
4. Validate design dengan measurement
5. Document design decisions

---

## 2. SPESIFIKASI REQUIREMENTS

### 2.1 Robot Specification

```
┌─────────────────────────────────────────────────────────────┐
│                  MOBILE ROBOT PLATFORM                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│    ┌─────────────────────────────────────────────────┐     │
│    │                 ROBOT TOP VIEW                   │     │
│    │                                                  │     │
│    │    [IMU]    [ESP32]    [Sensor Array]           │     │
│    │      │         │            │                    │     │
│    │   ┌──┴─────────┴────────────┴──┐                │     │
│    │   │                            │                │     │
│    │   │       POWER PCB            │                │     │
│    │   │                            │                │     │
│    │   └────────────────────────────┘                │     │
│    │          │              │                        │     │
│    │   ┌──────┴──┐    ┌──────┴──┐                   │     │
│    │   │ Motor L │    │ Motor R │                    │     │
│    │   └─────────┘    └─────────┘                    │     │
│    │                                                  │     │
│    │   [Battery: LiPo 3S 11.1V 2200mAh]             │     │
│    └──────────────────────────────────────────────────┘     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 Power Budget

| Component | Qty | Voltage | Current (typ) | Current (max) | Power (max) |
|-----------|-----|---------|---------------|---------------|-------------|
| Motor + Encoder | 2 | 12V | 300mA | 2A (stall) | 48W |
| Motor Driver | 1 | 12V | 50mA | 100mA | 1.2W |
| ESP32 | 1 | 5V | 100mA | 500mA | 2.5W |
| IMU (MPU6050) | 1 | 3.3V | 4mA | 10mA | 0.03W |
| Ultrasonic | 3 | 5V | 15mA | 30mA | 0.45W |
| OLED Display | 1 | 3.3V | 20mA | 30mA | 0.1W |
| LED + Buzzer | - | 5V | 50mA | 100mA | 0.5W |
| **Total** | | | | | **~53W** |

### 2.3 Voltage Rails Required

| Rail | Voltage | Current (max) | Ripple Spec | Application |
|------|---------|---------------|-------------|-------------|
| Motor | 12V | 4A | < 200mV | Motor driver |
| Logic | 5V | 1A | < 50mV | ESP32, sensors |
| Analog | 3.3V | 200mA | < 10mV | IMU, ADC ref |

### 2.4 EMC Requirements

| Parameter | Specification |
|-----------|---------------|
| ADC noise floor | < 5 LSB (12-bit) |
| Power rail ripple | As per table above |
| Ground bounce | < 100mV |
| ESD protection | ± 2kV contact |

---

## 3. DELIVERABLE

### 3.1 Design Document
**Format:** PDF, 10-15 pages

**Isi:**
1. **Power Architecture**
   - Block diagram
   - Voltage rail definition
   - Component selection justification

2. **Schematic Design**
   - Main power path
   - Protection circuits
   - Filter design with calculations

3. **Grounding Strategy**
   - Ground topology
   - Star point location
   - Motor ground isolation

4. **EMC Design**
   - Filter specifications
   - Shielding requirements
   - Cable routing plan

### 3.2 PCB Design (Optional - dapat diganti dengan perfboard)
**Format:** Gerber files atau foto prototype

**Requirements:**
- 2-layer minimum (4-layer recommended)
- Ground plane on layer 2
- Proper trace width for current
- Component placement zoning

### 3.3 Bill of Materials
**Format:** Excel atau CSV

| Item | Description | Part Number | Qty | Unit Price | Total |
|------|-------------|-------------|-----|------------|-------|
| 1 | DC-DC Buck 12V/5A | LM2596-12V | 1 | | |
| 2 | DC-DC Buck 5V/3A | LM2596-5V | 1 | | |
| ... | ... | ... | ... | ... | ... |

### 3.4 Noise Analysis Report
**Format:** PDF, 5-10 pages

**Isi:**
1. **Measurement Setup**
   - Equipment used
   - Test points
   - Test conditions

2. **Baseline Measurements**
   - No load
   - Static load
   - Dynamic load (motor running)

3. **Filter Performance**
   - Before/after comparison
   - Frequency response
   - Noise reduction achieved

4. **Validation Results**
   - ADC accuracy test
   - Communication reliability
   - Overall system stability

### 3.5 Video Demo
**Durasi:** 3-5 menit

**Konten:**
- Power system overview
- Noise measurement demo
- Filter effect demonstration
- Robot operation showing stability

---

## 4. MILESTONE & TIMELINE

### Week 1: Design & Prototyping
**Tasks:**
- Power budget calculation
- Component selection
- Schematic design
- Prototype on breadboard/perfboard
- Initial testing

**Checkpoint:** Working prototype, initial measurements

### Week 2: Optimization & Documentation
**Tasks:**
- Noise analysis
- Filter optimization
- PCB layout (optional)
- Full system integration
- Documentation
- Video recording

**Checkpoint:** All deliverables complete

---

## 5. RUBRIK PENILAIAN

### 5.1 Design Quality (40%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Power Budget | Complete, accurate, with margin | Complete, accurate | Basic calculation | Incomplete |
| Component Selection | Optimal, justified | Appropriate | Acceptable | Poor choices |
| Filter Design | Calculated, optimal | Well designed | Basic filter | Inadequate |
| Grounding | Star ground, isolated motor | Proper separation | Basic | Messy |

### 5.2 Implementation (30%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Build Quality | Clean, professional | Good | Acceptable | Messy |
| Noise Performance | Exceeds spec | Meets spec | Close to spec | Fails |
| Reliability | 100% stable | Very stable | Occasional glitch | Unstable |

### 5.3 Documentation (20%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Design Doc | Professional, complete | Good coverage | Basic | Incomplete |
| Noise Report | Detailed, analyzed | Good analysis | Basic data | Missing |
| BOM | Complete, sourced | Complete | Partial | Incomplete |

### 5.4 Demo (10%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Presentation | Clear, professional | Good | Adequate | Poor |
| System Working | Perfect operation | Minor issues | Works with issues | Fails |

---

## 6. BONUS CHALLENGES

### Bonus 1: Battery Management (+10%)
- Implement low voltage cutoff
- Battery level indicator
- Charging circuit integration

### Bonus 2: Hot-Swap Capability (+5%)
- Design for battery swap tanpa power loss
- Supercapacitor backup

### Bonus 3: Power Monitoring Dashboard (+5%)
- Real-time voltage/current monitoring
- Web-based display
- Fault logging

### Bonus 4: Professional PCB (+10%)
- 4-layer PCB design
- Manufactured and assembled
- Full EMC compliance

---

## 7. REFERENSI

### Books
1. Henry Ott - "Electromagnetic Compatibility Engineering"
2. Keith Billings - "Switchmode Power Supply Handbook"

### Application Notes
1. TI - "Power Supply Layout and EMI"
2. Analog Devices - "Power Supply Noise Reduction"
3. Würth - "Application Manual for EMC"

### Online
1. TI Power Stage Designer Tool
2. DigiKey PCB Design Resources
3. EEVblog Power Supply Design Videos

---

**Selamat mengerjakan project!**

*"A robot is only as reliable as its power system."*
