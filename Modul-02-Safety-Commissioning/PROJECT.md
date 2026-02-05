# PROJECT MODUL 2: INTEGRATED SAFETY SYSTEM

## INFORMASI PROJECT
- **Judul:** Integrated Safety System for Robotic Cell
- **Durasi:** 2 minggu
- **Tim:** 2-3 orang
- **Deliverable:** Safety system implementation, documentation, commissioning report, video demo

---

## 1. DESKRIPSI PROJECT

### 1.1 Latar Belakang
Dalam industri modern, safety system bukan hanya requirement regulasi, tetapi juga critical untuk produktivitas. Sistem safety yang well-designed memungkinkan:
- Operasi yang aman bagi operator dan equipment
- Quick recovery dari fault conditions
- Compliance dengan standar internasional
- Reduced downtime due to incidents

### 1.2 Tujuan Project
1. Design dan implement comprehensive safety system
2. Integrate multiple safety functions (E-Stop, interlocks, homing)
3. Create proper documentation sesuai standar industri
4. Execute professional commissioning procedure
5. Demonstrate safe operation modes

---

## 2. SPESIFIKASI REQUIREMENTS

### 2.1 System Configuration

```
┌─────────────────────────────────────────────────────────────┐
│                    ROBOTIC CELL LAYOUT                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│    ┌─────────┐         ┌─────────────────┐                 │
│    │ E-STOP  │         │                 │    ┌─────────┐  │
│    │    1    │         │   ROBOT ARM     │    │ E-STOP  │  │
│    └─────────┘         │   (Simulated    │    │    2    │  │
│                        │    by motor)    │    └─────────┘  │
│    [Door Sensor]       │                 │                 │
│          │             └────────┬────────┘                 │
│          │                      │                          │
│    ┌─────┴─────┐          ┌────┴─────┐                    │
│    │   DOOR    │          │  LIMIT   │                    │
│    │  SWITCH   │          │ SWITCHES │                    │
│    └───────────┘          └──────────┘                    │
│                                                             │
│    ┌─────────────────────────────────────┐                 │
│    │         CONTROL PANEL               │                 │
│    │  [START] [STOP] [RESET] [MODE]     │                 │
│    │  [STATUS LEDs] [7-SEG DISPLAY]     │                 │
│    └─────────────────────────────────────┘                 │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 Safety Requirements

| ID | Requirement | Priority | Implementation |
|----|-------------|----------|----------------|
| SR-01 | E-Stop shall stop all motion within 100ms | Critical | Hardware + Software |
| SR-02 | Door open shall prevent automatic operation | Critical | Interlock |
| SR-03 | Homing required before automatic mode | High | Software |
| SR-04 | Limit switch violation shall trigger fault | High | Hardware + Software |
| SR-05 | Communication loss shall trigger safe stop | Medium | Watchdog |
| SR-06 | Manual mode override with reduced speed | Medium | Software |
| SR-07 | All faults shall be logged with timestamp | Medium | Software |
| SR-08 | Visual/audio indication for all states | Low | LEDs + Buzzer |

### 2.3 Operating Modes

```
┌──────────────────────────────────────────────────────────┐
│                    OPERATING MODES                        │
├──────────────────────────────────────────────────────────┤
│                                                          │
│  ┌────────────┐                                         │
│  │    OFF     │◄──── Power Off / E-Stop Active          │
│  └─────┬──────┘                                         │
│        │ Power ON + E-Stop Released                     │
│        ▼                                                │
│  ┌────────────┐                                         │
│  │   INIT     │◄──── Self-test, I/O check               │
│  └─────┬──────┘                                         │
│        │ Init OK                                        │
│        ▼                                                │
│  ┌────────────┐         ┌────────────┐                  │
│  │   MANUAL   │◄───────►│   HOMING   │                  │
│  │   MODE     │         │   MODE     │                  │
│  └─────┬──────┘         └─────┬──────┘                  │
│        │                      │                          │
│        │ Homing Complete      │                          │
│        │         ┌────────────┘                          │
│        │         ▼                                       │
│        │   ┌────────────┐                                │
│        └──►│   AUTO     │◄──── Door Closed + All OK     │
│            │   MODE     │                                │
│            └────────────┘                                │
│                                                          │
│  ANY STATE ───► E-Stop ───► EMERGENCY STOP              │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

---

## 3. DELIVERABLE

### 3.1 Safety Circuit Diagram
**Format:** PDF atau image file

**Isi:**
- Complete wiring diagram
- E-Stop circuit (dual channel jika applicable)
- Interlock connections
- Motor enable circuit
- Indicator connections

### 3.2 Software Implementation
**Format:** PlatformIO project

**Required Modules:**
```cpp
// File structure
├── src/
│   ├── main.cpp
│   ├── safety_system.h
│   ├── safety_system.cpp
│   ├── state_machine.h
│   ├── state_machine.cpp
│   ├── homing.h
│   ├── homing.cpp
│   ├── interlock.h
│   ├── interlock.cpp
│   ├── fault_handler.h
│   ├── fault_handler.cpp
│   ├── logger.h
│   └── logger.cpp
├── include/
│   └── config.h
└── platformio.ini
```

### 3.3 Documentation Package

**3.3.1 Safety Analysis Document (3-5 pages)**
- Hazard identification
- Risk assessment matrix
- Safety function allocation
- Residual risk evaluation

**3.3.2 Interlock Matrix (1-2 pages)**
| Operation | E-Stop | Door | Homing | Temp | Mode | Allowed |
|-----------|--------|------|--------|------|------|---------|
| Manual Jog | ✓ | - | - | ✓ | Manual | ✓ |
| Auto Cycle | ✓ | ✓ | ✓ | ✓ | Auto | ✓ |
| Homing | ✓ | - | - | ✓ | Any | ✓ |
| Tool Change | ✓ | ✓ | ✓ | - | Manual | ✓ |

**3.3.3 Fault Response Table (1-2 pages)**
| Fault Code | Description | Response | Recovery |
|------------|-------------|----------|----------|
| F001 | E-Stop Pressed | Immediate stop | Manual reset |
| F002 | Door Opened | Safe stop | Close + Reset |
| F003 | Limit Exceeded | Stop + Retract | Home + Reset |
| F004 | Comm Timeout | Safe stop | Auto retry |
| F005 | Overtemperature | Controlled stop | Cool + Reset |

**3.3.4 Recovery Procedure Document (2-3 pages)**
- Step-by-step recovery for each fault
- Flowchart for recovery decision
- Required tools/actions

### 3.4 Commissioning Report
**Format:** PDF, 5-10 pages

**Isi:**
- Completed commissioning checklist
- Test results with data
- Issues found and resolution
- Sign-off page
- Photo documentation

### 3.5 Video Demo
**Durasi:** 5-7 menit

**Konten:**
1. System overview (1 min)
2. E-Stop demonstration (1 min)
3. Homing sequence (1 min)
4. Interlock demonstration (1 min)
5. Fault and recovery demo (1-2 min)
6. Full automatic cycle (1 min)

---

## 4. MILESTONE & TIMELINE

### Week 1: Design & Basic Implementation
**Tasks:**
- Safety analysis and documentation
- Hardware wiring
- Basic E-Stop and interlock implementation
- State machine framework

**Checkpoint:** E-Stop working, basic interlocks functional

### Week 2: Integration & Commissioning
**Tasks:**
- Complete all safety functions
- Homing implementation
- Logging system
- Full commissioning procedure
- Documentation completion
- Video recording

**Checkpoint:** All deliverables complete

---

## 5. RUBRIK PENILAIAN

### 5.1 Safety Implementation (40%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| E-Stop | < 50ms response, reliable | < 100ms, reliable | < 200ms | Not working |
| Interlocks | Complete, all conditions | Most conditions | Basic conditions | Incomplete |
| Homing | Repeatable, robust | Working correctly | Basic function | Not working |
| Fault Handling | Comprehensive, logged | Good coverage | Basic handling | Missing |

### 5.2 Documentation (30%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Safety Analysis | Professional, complete | Good analysis | Basic attempt | Missing |
| Wiring Diagram | Clear, complete | Good diagram | Basic | Incomplete |
| Commissioning | Thorough, signed | Complete | Partial | Missing |
| Code Documentation | Excellent comments | Good comments | Some comments | None |

### 5.3 System Performance (20%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Response Time | Exceeds spec | Meets spec | Close to spec | Fails |
| Reliability | 100% test pass | > 95% pass | > 80% pass | < 80% |
| Recovery | Smooth, automatic | Works correctly | Manual OK | Fails |

### 5.4 Video Demo (10%)

| Aspek | Excellent (4) | Good (3) | Adequate (2) | Poor (1) |
|-------|--------------|----------|--------------|----------|
| Presentation | Professional | Good | Adequate | Poor |
| Coverage | All functions shown | Most functions | Some functions | Incomplete |

---

## 6. BONUS CHALLENGES

### Bonus 1: Dual-Channel E-Stop (+10%)
- Implement redundant E-Stop monitoring
- Cross-check between channels
- Detect discrepancy faults

### Bonus 2: Safety PLC Simulation (+10%)
- Implement safety logic as separate task
- Communication between safety and main controller
- Safety function block library

### Bonus 3: Remote Monitoring (+5%)
- Web-based status display
- Fault history viewer
- Real-time state monitoring

### Bonus 4: Data Logging to SD Card (+5%)
- Timestamped event logging
- Fault history with details
- Exportable CSV format

---

## 7. REFERENSI

### Standards
1. IEC 60204-1 - Safety of machinery - Electrical equipment
2. ISO 13849-1 - Safety-related parts of control systems
3. ISO 10218-1/2 - Industrial robot safety

### Technical Resources
1. Pilz - Safety Compendium
2. Rockwell - Safebook Series
3. SICK - Safe Robotics Guide
4. Omron - Safety Application Guide

### Online Resources
1. machinery-safety.co.uk
2. pilz.com/en/knowhow
3. rockwellautomation.com/safety

---

**Selamat mengerjakan project!**

*"The best safety device is a careful operator, but the best design assumes the operator might not be."*
