# MATERI PEMBELAJARAN
## Modul 3: Power & EMC Praktis untuk Robot/IIoT

**Tujuan Pembelajaran:**
- Memahami desain power distribution untuk sistem robotika
- Menguasai teknik grounding dan shielding
- Mampu menganalisis dan mengatasi noise elektromagnetik
- Memahami layout kabel yang proper untuk sistem mekatronika

---

## 1. POWER SYSTEM DESIGN

### 1.1 Power Tree Architecture

Sistem robotika memerlukan berbagai level tegangan:

```
        ┌─────────────────────────────────────────────────────────────┐
        │                    POWER TREE DIAGRAM                       │
        └─────────────────────────────────────────────────────────────┘
        
                              AC MAINS
                             (220V AC)
                                 │
                    ┌────────────┴────────────┐
                    │      Main PSU           │
                    │    (SMPS 24V/10A)       │
                    └────────────┬────────────┘
                                 │
                              24V DC
                                 │
         ┌───────────────────────┼───────────────────────┐
         │                       │                       │
    ┌────┴────┐            ┌────┴────┐            ┌────┴────┐
    │ DC-DC   │            │ DC-DC   │            │ Direct  │
    │ 24V→12V │            │ 24V→5V  │            │  24V    │
    │ (Motor) │            │ (Logic) │            │(Relay)  │
    └────┬────┘            └────┬────┘            └────┬────┘
         │                      │                      │
      12V DC                  5V DC                  24V DC
         │                      │                      │
    ┌────┴────┐           ┌────┴────┐           ┌────┴────┐
    │Motor    │           │  ESP32  │           │ Solenoid│
    │Driver   │           │  MCU    │           │ Relay   │
    └─────────┘           │         │           └─────────┘
                          │  3.3V   │
                          │ (LDO)   │
                          └────┬────┘
                               │
                          ┌────┴────┐
                          │ Sensors │
                          │ (3.3V)  │
                          └─────────┘
```

### 1.2 Voltage Selection Guidelines

| Application | Voltage | Reason |
|-------------|---------|--------|
| Industrial sensors | 24V | Noise immunity, standard |
| Motor drivers | 12V-48V | Power requirement |
| Logic/MCU | 5V/3.3V | IC compatibility |
| High-power motors | 48V-72V | Efficiency |
| Safety circuits | 24V | Industry standard |

### 1.3 Power Budget Calculation

**Contoh Perhitungan:**

| Component | Qty | Voltage | Current (max) | Power |
|-----------|-----|---------|---------------|-------|
| ESP32 | 1 | 5V | 500mA | 2.5W |
| Motor (stall) | 2 | 12V | 2A each | 48W |
| Encoder | 2 | 5V | 50mA each | 0.5W |
| Limit switch | 4 | 24V | 20mA each | 1.9W |
| Relay | 2 | 24V | 100mA each | 4.8W |
| LED indicators | 5 | 5V | 20mA each | 0.5W |
| **Total** | | | | **58.2W** |

**Safety Factor:** Minimum 1.5x untuk design margin

$$P_{PSU} = P_{total} \times SF = 58.2W \times 1.5 = 87.3W$$

Pilih PSU: **24V/5A (120W)** ✓

### 1.4 Voltage Regulator Selection

**Linear Regulator (LDO):**
- Simple, low noise
- Efisiensi rendah jika dropout besar
- Cocok untuk low current, small dropout

**Switching Regulator (Buck/Boost):**
- Efisiensi tinggi (85-95%)
- Noise output lebih tinggi
- Cocok untuk high current, large voltage difference

**Kapan Pakai Apa:**
```
                           ┌─────────────────┐
                           │ Voltage Drop    │
                           │ > 3V?           │
                           └────────┬────────┘
                                    │
                    ┌───────────────┴───────────────┐
                    │ YES                           │ NO
                    ▼                               ▼
            ┌───────────────┐               ┌───────────────┐
            │ Current       │               │ Current       │
            │ > 500mA?      │               │ > 1A?         │
            └───────┬───────┘               └───────┬───────┘
                    │                               │
         ┌──────────┴──────────┐         ┌─────────┴─────────┐
         │ YES                 │ NO      │ YES               │ NO
         ▼                     ▼         ▼                   ▼
    ┌─────────┐          ┌─────────┐ ┌─────────┐       ┌─────────┐
    │ SMPS    │          │ SMPS    │ │ SMPS    │       │  LDO    │
    │ (Buck)  │          │ atau    │ │ (Buck)  │       │         │
    └─────────┘          │  LDO    │ └─────────┘       └─────────┘
                         └─────────┘
```

---

## 2. GROUNDING & SHIELDING

### 2.1 Grounding Philosophy

**Tipe Ground:**
1. **Safety Ground (PE):** Proteksi dari shock, connected ke earth
2. **Signal Ground (SGND):** Reference untuk sinyal analog/digital
3. **Power Ground (PGND):** Return path untuk power supply
4. **Chassis Ground:** Enclosure, shielding

**Single Point Ground (Star Ground):**
```
                    ┌──────────────────────────────────┐
                    │           STAR GROUND            │
                    └──────────────────────────────────┘
                    
                                   ★ Central Point
                                   │
                ┌──────────────────┼──────────────────┐
                │                  │                  │
           ┌────┴────┐        ┌───┴────┐        ┌───┴────┐
           │  Power  │        │ Signal │        │ Motor  │
           │ Section │        │Section │        │Section │
           └─────────┘        └────────┘        └────────┘
           
    ✓ Prevents ground loops
    ✓ Clean signal reference
    ✗ Not suitable for high frequency
```

**Multi-Point Ground (untuk High Frequency):**
```
                    ┌──────────────────────────────────┐
                    │        GROUND PLANE              │
                    └──────────────────────────────────┘
                    
        ┌─────────────────────────────────────────────────┐
        │  ════════════════════════════════════════════  │ Ground Plane
        │    │         │         │         │         │   │
        │  ┌─┴─┐     ┌─┴─┐     ┌─┴─┐     ┌─┴─┐     ┌─┴─┐ │
        │  │IC1│     │IC2│     │IC3│     │IC4│     │IC5│ │
        │  └───┘     └───┘     └───┘     └───┘     └───┘ │
        └─────────────────────────────────────────────────┘
        
    ✓ Low impedance at high frequency
    ✓ Better for digital circuits
    ✗ May create ground loops in mixed systems
```

### 2.2 Ground Loop Prevention

**Problem:**
```
        ┌─────────┐         ┌─────────┐
        │ Device  │ Signal  │ Device  │
        │    A    ├────────►│    B    │
        └────┬────┘         └────┬────┘
             │     Vn (noise)    │
        ─────┴───────≋≋≋≋≋──────┴───── GND
             ◄───────────────────►
                Ground Loop
```

**Solution 1: Isolation**
```
        ┌─────────┐  ┌──────────┐  ┌─────────┐
        │ Device  │  │ Opto-    │  │ Device  │
        │    A    ├──┤ isolator ├──┤    B    │
        └────┬────┘  └──────────┘  └────┬────┘
             │                          │
        ─────┴────────              ────┴───── GND
         GND A                       GND B
                (Isolated)
```

**Solution 2: Differential Signaling**
```
        ┌─────────┐  Signal+   ┌─────────┐
        │ Device  ├───────────►│ Device  │
        │    A    │  Signal-   │    B    │
        │         ├───────────►│         │
        └────┬────┘            └────┬────┘
             │      Noise affects │
        ─────┴──────≋≋≋≋≋≋──────┴───── GND
                    BOTH lines equally
                    → Cancelled by differential receiver!
```

### 2.3 Shielding Techniques

**Cable Shielding:**
```
    UNSHIELDED CABLE:              SHIELDED CABLE:
    
    ════════════════               ╔══════════════╗
         Signal                    ║    Signal    ║ Shield
    ════════════════               ╚══════════════╝
                                          │
    EMI can couple                    GND ┴ (one end)
    directly to signal
```

**Shielding Connection:**
- Shield grounded at ONE end (low frequency): Prevents ground loop
- Shield grounded at BOTH ends (high frequency): Better HF shielding
- Shield grounded via capacitor: Best of both worlds

```
    ┌─────────────────────────────────────────────────┐
    │         SHIELD GROUNDING OPTIONS                │
    └─────────────────────────────────────────────────┘
    
    Option A: Single-end ground (LF signals)
    ╔════════════════════════════════════════╗
    ║                                        ║
    ╚════════════════════════════════════════╝
    │                                        ×
    GND                                    Float
    
    Option B: Both-end ground (HF signals)
    ╔════════════════════════════════════════╗
    ║                                        ║
    ╚════════════════════════════════════════╝
    │                                        │
    GND                                     GND
    
    Option C: Capacitive coupling (mixed)
    ╔════════════════════════════════════════╗
    ║                                        ║
    ╚════════════════════════════════════════╝
    │                                      ──┴──
    GND                                   ──┬── 10nF
                                           │
                                          GND
```

---

## 3. EMC (ELECTROMAGNETIC COMPATIBILITY)

### 3.1 EMI Sources in Robotics

| Source | Frequency | Coupling Mode | Victim |
|--------|-----------|---------------|--------|
| Motor PWM | 1-50 kHz | Conducted, Radiated | ADC, Communication |
| Switching PSU | 50-500 kHz | Conducted | Analog sensors |
| Motor brushes | Broadband | Radiated | Everything |
| Relay switching | Impulse | Conducted | MCU reset |
| ESD | ns pulse | Radiated, Conducted | ICs |

### 3.2 Coupling Mechanisms

**1. Conducted Coupling:**
```
    Noise source ───────┬─────── Victim
                        │
                    Shared
                   Impedance
                        │
                       GND
```

**2. Capacitive Coupling (E-field):**
```
    Noise Source ═══════════════
                        ┊ Capacitive
                        ┊ coupling
    Victim      ═══════════════
```

**3. Inductive Coupling (H-field):**
```
         ╭──────╮
    ┌────┤      ├────┐
    │    ╰──────╯    │   Magnetic
    │    Noise       │   field
    └────────────────┘
                ↕
         ╭──────╮
    ┌────┤      ├────┐
    │    ╰──────╯    │
    │    Victim      │
    └────────────────┘
```

### 3.3 EMI Mitigation Techniques

**Filtering:**
```
    ┌─────────────────────────────────────────────────┐
    │              LC LOW-PASS FILTER                 │
    └─────────────────────────────────────────────────┘
    
           L (Ferrite bead)
    In ────ΩΩΩΩΩ────┬──── Out
                    │
                   ═╧═ C
                    │
                   GND
    
    Cutoff frequency: fc = 1/(2π√LC)
```

**Decoupling Capacitors:**
```
    ┌─────────────────────────────────────────────────┐
    │          DECOUPLING PLACEMENT                   │
    └─────────────────────────────────────────────────┘
    
              VCC
               │
    ┌──────────┼──────────┐
    │          │          │
    │   C1    ═╧═   C2    │
    │  100µF   │  100nF   │  Place C2 closest to IC
    │          │          │
    └──────────┼──────────┘
               │
          ┌────┴────┐
          │   IC    │
          │   VCC   │
          └────┬────┘
               │
              GND
```

**Snubber Circuit (untuk relay/motor):**
```
    ┌─────────────────────────────────────────────────┐
    │              RC SNUBBER                         │
    └─────────────────────────────────────────────────┘
    
    Inductive        ┌────────────┐
    Load        ─────┤   RELAY    ├─────
    (Motor,          │   COIL     │
     Solenoid)       └────┬───────┘
                          │
                     ┌────┴────┐
                     │   R     │
                     │  100Ω   │
                     └────┬────┘
                          │
                     ┌────┴────┐
                     │   C     │
                     │  100nF  │
                     └────┬────┘
                          │
                         GND
    
    Snubber absorbs energy spike when
    inductive load is switched off
```

**TVS Diode Protection:**
```
    ┌─────────────────────────────────────────────────┐
    │          TVS PROTECTION                         │
    └─────────────────────────────────────────────────┘
    
                    Signal In
                        │
                   ┌────┴────┐
                   │  Series │
                   │Resistor │
                   └────┬────┘
                        │
                        ├────────── To MCU
                        │
                    ┌───┴───┐
                    │  TVS  │  Clamps voltage
                    │ Diode │  to safe level
                    └───┬───┘
                        │
                       GND
```

---

## 4. CABLE ROUTING & LAYOUT

### 4.1 Cable Separation Rules

```
┌─────────────────────────────────────────────────────────┐
│              CABLE SEPARATION MATRIX                    │
├─────────────────────────────────────────────────────────┤
│                                                         │
│   HIGH POWER (Motor, Heater, AC)                       │
│   ════════════════════════════                          │
│        │                                                │
│        │ > 30 cm or shield                             │
│        │                                                │
│   CONTROL (24V I/O, Relay)                             │
│   ─────────────────────────────                         │
│        │                                                │
│        │ > 15 cm                                        │
│        │                                                │
│   SIGNAL (Analog sensors, Encoders)                    │
│   - - - - - - - - - - - - - - - -                      │
│        │                                                │
│        │ > 10 cm                                        │
│        │                                                │
│   DATA (Ethernet, USB, RS485)                          │
│   ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙                         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### 4.2 Crossing Rules

```
    WRONG:                      CORRECT:
    
    Power ═══════════════       Power ═══════════════
               ║                          │
    Signal ════╬════════       Signal ════┼════════
               ║                          │
               ║                     90° crossing
    Parallel run                   minimizes coupling
    maximizes coupling
```

### 4.3 Enclosure Layout

```
┌─────────────────────────────────────────────────────────┐
│              CONTROL PANEL LAYOUT                       │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐     │
│  │    PSU      │  │  Terminal   │  │   Relay     │     │
│  │   24V/5A    │  │   Blocks    │  │   Module    │     │
│  └─────────────┘  └─────────────┘  └─────────────┘     │
│        │                                                │
│  ══════╧═══════════════════════════════════════════    │
│                    POWER BUS                            │
│  ─────────────────────────────────────────────────     │
│                    CABLE DUCT                           │
│  ─────────────────────────────────────────────────     │
│                                                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐     │
│  │    MCU      │  │   I/O       │  │  Motor      │     │
│  │   ESP32     │  │  Module     │  │  Driver     │     │
│  └─────────────┘  └─────────────┘  └─────────────┘     │
│                                                         │
│  ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙ │
│                    SIGNAL BUS                           │
│  ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙ │
│                                                         │
└─────────────────────────────────────────────────────────┘

KEY:
═══ Power cables (high current)
─── Control cables (24V digital)
∙∙∙ Signal cables (analog, communication)
```

---

## 5. NOISE ANALYSIS & FILTERING

### 5.1 Noise Measurement

**Tools:**
- Oscilloscope dengan probes yang proper
- Spectrum analyzer (atau FFT di oscilloscope)
- Current probe untuk conducted emission

**Measurement Points:**
1. Power supply output ripple
2. Motor driver input/output
3. Analog sensor output
4. MCU ADC reference

### 5.2 Filter Design

**First-Order Low-Pass:**
$$f_c = \frac{1}{2\pi RC}$$

**Second-Order Low-Pass (Butterworth):**
$$f_c = \frac{1}{2\pi\sqrt{LC}}$$

**Example: Anti-aliasing filter untuk ADC 1kHz sampling:**
- Nyquist: 500 Hz
- Target cutoff: 200 Hz (dengan margin)
- R = 10kΩ, C = 80nF → fc = 199 Hz ✓

### 5.3 Practical Filter Circuits

**RC Filter for Analog Sensor:**
```
    Sensor Output ────┬────R=10k────┬──── To ADC
                      │             │
                     ═╧═           ═╧═
                     │C=100n       │C=10n
                      │             │
                     GND           GND
                  
    2-stage filter for better rolloff
```

**Pi Filter for Power:**
```
    Noisy DC ──┬───ΩΩΩΩΩ───┬──── Clean DC
               │     L     │
              ═╧═         ═╧═
              │C1         │C2
               │           │
              GND         GND
              
    C1 = C2 = 100µF, L = 10µH (ferrite bead)
```

---

## 6. PCB DESIGN CONSIDERATIONS

### 6.1 Layer Stack-up

**4-Layer PCB (Recommended untuk mixed signal):**
```
    Layer 1: Signal (Top)      ═══════════════
    Layer 2: Ground Plane      ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
    Layer 3: Power Plane       ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    Layer 4: Signal (Bottom)   ═══════════════
```

### 6.2 Component Placement

```
┌─────────────────────────────────────────────────────────┐
│              PCB ZONING                                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────────┐    ┌─────────────────┐            │
│  │   POWER ZONE    │    │  DIGITAL ZONE   │            │
│  │                 │    │                 │            │
│  │  - PSU input    │    │  - MCU          │            │
│  │  - Regulators   │    │  - Crystal      │            │
│  │  - Bulk caps    │    │  - Decoupling   │            │
│  └─────────────────┘    └─────────────────┘            │
│           │                     │                       │
│           └──────┬──────────────┘                       │
│                  │                                      │
│  ┌─────────────────────────────────────┐               │
│  │           ANALOG ZONE               │               │
│  │                                     │               │
│  │  - Sensor interface                 │               │
│  │  - ADC                              │               │
│  │  - Reference voltage                │               │
│  │  - Keep away from switching noise   │               │
│  └─────────────────────────────────────┘               │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### 6.3 Trace Routing Rules

| Signal Type | Width | Spacing | Via |
|-------------|-------|---------|-----|
| Power (>1A) | 2mm+ | 0.5mm | Multiple |
| Signal digital | 0.3mm | 0.2mm | Standard |
| Signal analog | 0.5mm | 0.5mm | Minimize |
| High-speed | 0.3mm | 3x width | Avoid |

---

## 7. PRAKTIKUM PREVIEW

### 7.1 Eksperimen Noise Analysis
- Measure noise sebelum/sesudah filtering
- Bandingkan berbagai filter topology
- Dokumentasikan dengan oscilloscope

### 7.2 Eksperimen Grounding
- Compare star vs multi-point ground
- Observe ground loop effects
- Implement proper grounding

### 7.3 Eksperimen EMI Mitigation
- Measure EMI dari motor PWM
- Apply various mitigation techniques
- Validate dengan measurement

---

## REFERENSI

1. Henry Ott - "Electromagnetic Compatibility Engineering"
2. Tim Williams - "EMC for Product Designers"
3. Howard Johnson - "High-Speed Digital Design"
4. Würth Elektronik - "Trilogy of Magnetics"
5. Texas Instruments - "Power Supply Design Seminar"

---

*"A clean power system is the foundation of a reliable robot."*
