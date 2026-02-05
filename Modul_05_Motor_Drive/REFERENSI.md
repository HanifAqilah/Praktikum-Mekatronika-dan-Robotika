# REFERENSI
## Modul 5: Motor & Drive Systems

---

## BUKU TEKS

### 1. Motor Control & Drives
- **Hughes, A., & Drury, B. (2019).** *Electric Motors and Drives: Fundamentals, Types and Applications* (5th ed.). Newnes.
  - Chapter 3: Conventional DC Motors
  - Chapter 9: Brushless DC Motors
  - ISBN: 978-0081028155

- **Mohan, N., Undeland, T., & Robbins, W. (2003).** *Power Electronics: Converters, Applications, and Design* (3rd ed.). Wiley.
  - Chapter 6: DC-DC Converters (PWM)
  - Chapter 8: Motor Drives
  - ISBN: 978-0471226932

- **Krishnan, R. (2017).** *Permanent Magnet Synchronous and Brushless DC Motor Drives*. CRC Press.
  - Chapter 4: BLDC Motor Fundamentals
  - Chapter 7: Control Techniques
  - ISBN: 978-0824753849

---

### 2. Control Systems
- **Åström, K. J., & Murray, R. M. (2021).** *Feedback Systems: An Introduction for Scientists and Engineers* (2nd ed.). Princeton University Press.
  - Chapter 10: PID Control
  - Chapter 11: Frequency Domain Design
  - **FREE PDF:** https://fbswiki.org/

- **Ogata, K. (2010).** *Modern Control Engineering* (5th ed.). Prentice Hall.
  - Chapter 5: Transient Response Analysis
  - Chapter 10: PID Controllers and Modified PID Controllers
  - ISBN: 978-0136156734

---

## PAPER & ARTIKEL ILMIAH

### 3. Motor Characterization
- **Hanselman, D. C. (2003).** "Brushless Permanent Magnet Motor Design" (2nd ed.). *The Writers' Collective*.
  - Appendix B: Motor Testing and Characterization Procedures

- **Zhang, Y., & Zhu, J. (2011).** "Direct Torque Control of Permanent Magnet Synchronous Motor." *IEEE Transactions on Power Electronics*, 26(5), 1561-1570.
  - DOI: 10.1109/TPEL.2010.2087773

---

### 4. PWM & H-Bridge Design
- **Texas Instruments (2016).** "PWM Control of DC Motors" (Application Report SLVA898).
  - **Download:** https://www.ti.com/lit/an/slva898/slva898.pdf

- **Microchip (2004).** "Brushed DC Motor Fundamentals" (Application Note AN905).
  - **Download:** https://ww1.microchip.com/downloads/en/AppNotes/00905a.pdf

- **Infineon (2017).** "H-Bridge Design Guide" (Application Note).
  - Dead-time calculation
  - Shoot-through protection
  - **Download:** https://www.infineon.com

---

### 5. Current Sensing
- **Ziegler, S., Woodward, R., & Iu, H. (2009).** "Current Sensing Techniques: A Review." *IEEE Sensors Journal*, 9(4), 354-376.
  - DOI: 10.1109/JSEN.2009.2013914
  - Comprehensive comparison: Shunt vs Hall-effect vs Rogowski coil

- **Texas Instruments (2018).** "A Basic Guide to Current Sensing" (Application Report SBOA274).
  - **Download:** https://www.ti.com/lit/an/sboa274/sboa274.pdf

---

## ONLINE RESOURCES & TUTORIAL

### 6. Video Tutorial
- **"How Brushless Motors Work - Part 1"** - Horizon Hobby (YouTube)
  - URL: https://www.youtube.com/watch?v=uOQk8SJso6Q
  - Durasi: 8 menit, visualisasi excellent untuk komutasi BLDC

- **"Understanding Motor Drivers"** - Adafruit Industries (YouTube)
  - URL: https://www.youtube.com/watch?v=WWXXzqXd_zk
  - Durasi: 12 menit, demo L298N & DRV8833

- **"PWM Explained: Arduino PWM Example"** - GreatScott! (YouTube)
  - URL: https://www.youtube.com/watch?v=YfV-vYT3yfQ
  - Durasi: 10 menit, oscilloscope analysis of PWM

---

### 7. Interactive Tools
- **Motor Sizing Tool** - Oriental Motor
  - URL: https://www.orientalmotor.com/motor-sizing/
  - Interactive calculator untuk memilih motor berdasarkan load, speed, torque

- **PWM Calculator** - Daycounter Engineering
  - URL: https://daycounter.com/Calculators/PWM-Calculator.phtml
  - Hitung duty cycle, frequency, resolution

- **Torque-Speed Curve Plotter** - MotorWizard
  - URL: https://motorwizard.com (commercial, free trial available)

---

### 8. Arduino/ESP32 Libraries & Examples
- **ESP32 LEDC (PWM)** - Espressif Official Documentation
  - URL: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html
  - Code examples untuk 16-bit PWM

- **Encoder Library** - Paul Stoffregen (PJRC)
  - GitHub: https://github.com/PaulStoffregen/Encoder
  - Quadrature encoder reading dengan interrupt

- **PID_v1 Library** - Brett Beauregard
  - GitHub: https://github.com/br3ttb/Arduino-PID-Library
  - Industry-standard PID implementation untuk Arduino

- **Adafruit_INA219** - Adafruit Industries
  - GitHub: https://github.com/adafruit/Adafruit_INA219
  - Current & voltage sensor library

---

## DATASHEET & TECHNICAL DOCS

### 9. Motor Driver ICs
- **L298N Dual H-Bridge Driver**
  - Manufacturer: STMicroelectronics
  - Max current: 2A per channel (4A peak)
  - Datasheet: https://www.st.com/resource/en/datasheet/l298.pdf

- **DRV8833 Dual Motor Driver**
  - Manufacturer: Texas Instruments
  - Max current: 1.5A RMS per channel
  - Efficiency: >90%
  - Datasheet: https://www.ti.com/lit/ds/symlink/drv8833.pdf

- **TB6612FNG Dual Motor Driver**
  - Manufacturer: Toshiba
  - Max current: 1.2A per channel (3.2A peak)
  - Low voltage drop: 0.5V typical
  - Datasheet: https://www.sparkfun.com/datasheets/Robotics/TB6612FNG.pdf

- **VNH5019 Single H-Bridge**
  - Manufacturer: STMicroelectronics
  - Max current: 12A continuous, 30A peak
  - Thermal shutdown, current limiting built-in
  - Datasheet: https://www.pololu.com/file/0J504/vnh5019a-e.pdf

---

### 10. Motor Specifications
- **Pololu 37D Metal Gearmotor** (contoh motor untuk praktikum)
  - Datasheet: https://www.pololu.com/product/4751
  - Specs: 12V, 150 RPM, 5.5 kg·cm stall torque, 64 CPR encoder

- **Maxon DCX Motor Series**
  - Datasheet: https://www.maxongroup.com/maxon/view/content/DCX-Program
  - High-performance brushed DC motors
  - Complete motor constant data

---

### 11. Sensor Modules
- **INA219 High Side Current Sensor**
  - Manufacturer: Texas Instruments
  - Range: ±3.2A
  - Interface: I2C
  - Datasheet: https://www.ti.com/lit/ds/symlink/ina219.pdf

- **ACS712 Hall-Effect Current Sensor**
  - Manufacturer: Allegro MicroSystems
  - Range: ±5A, ±20A, ±30A versions
  - Output: Analog voltage (Vcc/2 ± sensitivity × current)
  - Datasheet: https://www.allegromicro.com/en/products/sense/current-sensor-ics/zero-to-fifty-amp-integrated-conductor-sensor-ics/acs712

---

## SOFTWARE TOOLS

### 12. Simulation & Modeling
- **LTspice** (Circuit Simulator)
  - URL: https://www.analog.com/en/design-center/design-tools-and-calculators/ltspice-simulator.html
  - FREE, untuk simulasi H-bridge circuit, PWM analysis

- **MATLAB/Simulink** - Motor Control Toolbox
  - Simulink model untuk DC motor + PID controller
  - Tutorial: https://www.mathworks.com/help/physmod/sps/ug/dc-motor-speed-control.html

- **Python Control Systems Library**
  - GitHub: https://github.com/python-control/python-control
  - Simulasi PID, Bode plot, root locus

---

### 13. Data Analysis
- **Python Libraries:**
  - `numpy` - Numerical computation
  - `scipy` - Signal processing (FFT, filter design)
  - `matplotlib` - Plotting
  - `pandas` - Data handling
  - `control` - Control system analysis

- **Installation:**
  ```bash
  pip install numpy scipy matplotlib pandas control-systems
  ```

---

## FORUM & COMMUNITY

### 14. Discussion Boards
- **ESP32 Forum** - Espressif Official
  - URL: https://esp32.com/
  - Kategori: Hardware & Programming

- **Arduino Forum** - Motor Control Section
  - URL: https://forum.arduino.cc/c/using-arduino/motors-mechanics-power-and-cnc/11

- **Reddit - r/robotics**
  - URL: https://www.reddit.com/r/robotics/
  - Active community, good for troubleshooting

- **Stack Exchange - Electrical Engineering**
  - URL: https://electronics.stackexchange.com/
  - Tag: [motor-control], [pwm], [h-bridge]

---

## STANDARDS & SAFETY

### 15. Industry Standards
- **IEC 60034-1:2017** - Rotating electrical machines - Part 1: Rating and performance
  - Standar internasional untuk rating motor

- **NEMA MG 1-2021** - Motors and Generators
  - North American standard untuk motor specification

- **ISO 9283:1998** - Manipulating industrial robots - Performance criteria and related test methods
  - Untuk robotics application dengan motor positioning

---

### 16. Safety Guidelines
- **OSHA 1910.212** - General requirements for all machines
  - Machinery guarding, emergency stop

- **IEC 61508** - Functional Safety of Electrical/Electronic/Programmable Electronic Safety-related Systems
  - Safety Integrity Level (SIL) requirements

- **UL 508C** - Power Conversion Equipment
  - Safety standard untuk motor drivers & inverters

---

## VENDOR RESOURCES

### 17. Motor & Driver Suppliers
- **Pololu Robotics & Electronics**
  - URL: https://www.pololu.com/category/22/motors-and-gearboxes
  - Excellent datasheets, application notes, video guides

- **SparkFun Electronics**
  - URL: https://www.sparkfun.com/categories/178
  - Tutorials, hookup guides untuk motor drivers

- **Adafruit Industries**
  - URL: https://www.adafruit.com/category/63
  - Learn system dengan step-by-step guides

- **Maxon Motor**
  - URL: https://www.maxongroup.com/
  - Premium motors dengan comprehensive technical data

---

## PROJECT EXAMPLES

### 18. Open-Source Projects
- **ODrive High Performance Motor Controller**
  - GitHub: https://github.com/odriverobotics/ODrive
  - Advanced BLDC control dengan FOC (Field-Oriented Control)

- **SimpleFOC - Field Oriented Control Library**
  - GitHub: https://github.com/simplefoc/Arduino-FOC
  - Arduino library untuk BLDC/stepper FOC

- **ROS Motor Controller Examples**
  - GitHub: https://github.com/ros-controls/ros_controllers
  - Integration dengan ROS (preview untuk Modul 11)

---

## TOOLS & EQUIPMENT GUIDES

### 19. Oscilloscope Usage
- **Keysight (Agilent) - "Oscilloscope Fundamentals"**
  - PDF: https://www.keysight.com/us/en/assets/7018-06894/primers/5990-6618.pdf
  - Trigger setup, FFT analysis

- **Tektronix - "XYZ's of Oscilloscopes"**
  - Interactive guide: https://www.tek.com/en/documents/primer/xyz-s-oscilloscopes-primer

---

### 20. Multimeter & Current Measurement
- **Fluke - "How to Use a Multimeter"**
  - Video: https://www.fluke.com/en-us/learn/blog/digital-multimeters/how-to-use-a-multimeter

- **EEVblog - "Multimeter Tutorial"**
  - YouTube: https://www.youtube.com/watch?v=bF3OyQ3HwfU
  - Detailed 30-min tutorial

---

## TROUBLESHOOTING RESOURCES

### 21. Common Issues & Solutions
- **"Motor Driver Troubleshooting Guide"** - Pololu
  - URL: https://www.pololu.com/docs/0J55
  - Diagnostic flowchart, common failure modes

- **"Why is my motor not working?"** - Adafruit Forum
  - Checklist: Power supply, wiring, code, driver enable pin

- **"Encoder Noise Filtering"** - PJRC Forum
  - Hardware: RC filter, Schmitt trigger
  - Software: Debouncing, median filter

---

## ADDITIONAL READING

### 22. Advanced Topics (untuk eksplorasi lebih lanjut)
- **Field-Oriented Control (FOC) untuk BLDC**
  - "Vector Control of AC Drives" - Peter Vas (buku)
  - Lebih efisien dibanding six-step commutation

- **Sensorless Motor Control**
  - "Sensorless BLDC Motor Control Using Back-EMF" - Microchip AN1160
  - Eliminasi Hall sensor dengan observasi back-EMF

- **Regenerative Braking**
  - "Energy Recovery in Electric Vehicles" - IEEE paper
  - Implementasi untuk efficiency boost

---

## KONTAK SUPPORT

**Technical Support:**
- Lab Teknisi: [Email]
- Teaching Assistant: [Email]
- Instructor: [Email]

**Operating Hours:**
- Lab: Senin-Jumat, 08:00-17:00
- Online support: 24/7 via forum

---

*Referensi ini akan di-update berkala. Cek repository untuk versi terbaru.*

---

**Last Update:** Februari 2026
