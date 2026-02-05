# REFERENSI MODUL 4: SENSOR FUSION & CONDITIONING

---

## BUKU TEKS

### 1. Signal Processing & Filtering

**Steven W. Smith** — *The Scientist and Engineer's Guide to Digital Signal Processing*  
Publisher: California Technical Publishing (1997)  
ISBN: 978-0966017632  
**Available online**: http://www.dspguide.com/ (free)

**Coverage**:
- Chapter 15: Moving Average Filters
- Chapter 19: Recursive Filters (IIR, low-pass)
- Chapter 24: Linear Systems (frequency response)
- **Relevant untuk**: Percobaan 2, 3 (moving average, low-pass filter)

---

**David G. Alciatore, Michael B. Histand** — *Introduction to Mechatronics and Measurement Systems* (5th Edition)  
Publisher: McGraw-Hill Education (2018)  
ISBN: 978-1259892349

**Coverage**:
- Chapter 9: Analog Signal Processing (filtering, noise)
- Chapter 10: Digital Signal Processing (ADC, sampling theorem)
- Chapter 11: Sensor Characteristics (noise, drift, calibration)
- **Relevant untuk**: Percobaan 1 (noise analysis), Percobaan 2-3 (filtering)

---

### 2. Sensor Fusion & Kalman Filter

**Dan Simon** — *Optimal State Estimation: Kalman, H∞, and Nonlinear Approaches*  
Publisher: Wiley-Interscience (2006)  
ISBN: 978-0471708582

**Coverage**:
- Chapter 1-2: Introduction to State Estimation
- Chapter 3-4: Linear Optimal Filters (Kalman)
- Chapter 5: Nonlinear Filters (EKF, UKF)
- **Relevant untuk**: Percobaan 5 (Kalman filter)

---

**Greg Welch, Gary Bishop** — *An Introduction to the Kalman Filter*  
Publisher: University of North Carolina at Chapel Hill, Technical Report (2006)  
**Available online**: https://www.cs.unc.edu/~welch/media/pdf/kalman_intro.pdf (free)

**Coverage**:
- Kalman filter basics (discrete time)
- Example: tracking constant velocity object
- Q & R tuning guidelines
- **Relevant untuk**: Percobaan 5 (Kalman filter implementation)

---

**Sebastian Madgwick** — *An Efficient Orientation Filter for IMUs*  
Internal Report, University of Bristol (2010)  
**Available online**: https://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/ (free)

**Coverage**:
- Complementary filter untuk attitude estimation
- Gyroscope + accelerometer fusion
- **Relevant untuk**: Percobaan 4 (complementary filter)

---

### 3. Sensor & Measurement

**Jon S. Wilson** — *Sensor Technology Handbook*  
Publisher: Newnes (2004)  
ISBN: 978-0750677295

**Coverage**:
- Chapter 1: Sensor characteristics (accuracy, precision, noise)
- Chapter 2: Signal conditioning
- Chapter 12: Position & motion sensors (encoder, IMU)
- **Relevant untuk**: Percobaan 1 (sensor noise), Percobaan 6 (redundancy)

---

**Walter Boyes** — *Instrumentation Reference Book* (4th Edition)  
Publisher: Butterworth-Heinemann (2009)  
ISBN: 978-0750683081

**Coverage**:
- Chapter 3: Sensor signal conditioning
- Chapter 5: Data acquisition systems
- Chapter 15: Fault detection & isolation
- **Relevant untuk**: Percobaan 6 (sensor voting, fault detection)

---

## PAPER & JURNAL

### 1. Complementary Filter

**Robert Mahony, Tarek Hamel, Jean-Michel Pflimlin** (2008)  
*"Nonlinear Complementary Filters on the Special Orthogonal Group"*  
IEEE Transactions on Automatic Control, Vol. 53, No. 5, pp. 1203-1218  
DOI: 10.1109/TAC.2008.923738

**Summary**: Advanced complementary filter untuk 3D orientation estimation (quaternion-based).

---

**Shane Colton** (2007)  
*"The Balance Filter: A Simple Solution for Integrating Accelerometer and Gyroscope Measurements for a Balancing Platform"*  
MIT Bachelor Thesis

**Available**: http://web.mit.edu/~scolton/www/filter.pdf  
**Summary**: Simple complementary filter explanation, practical implementation.

---

### 2. Kalman Filter Applications

**Hugh Durrant-Whyte, Tim Bailey** (2006)  
*"Simultaneous Localization and Mapping (SLAM): Part I"*  
IEEE Robotics & Automation Magazine, Vol. 13, No. 2, pp. 99-110  
DOI: 10.1109/MRA.2006.1638022

**Summary**: Kalman filter application untuk robot localization (SLAM).

---

**Welch, Bishop** (1995)  
*"An Introduction to the Kalman Filter"*  
SIGGRAPH 2001 Course Notes

**Available**: https://www.cs.unc.edu/~welch/kalman/  
**Summary**: Tutorial Kalman filter, include MATLAB code example.

---

### 3. Sensor Redundancy & Fault Tolerance

**Isermann, Rolf** (2006)  
*"Fault-Diagnosis Systems: An Introduction from Fault Detection to Fault Tolerance"*  
Springer-Verlag Berlin Heidelberg  
ISBN: 978-3540303688

**Coverage**: Chapter 5 - Sensor redundancy, voting logic, fault detection algorithms.

---

**NASA** (1996)  
*"Fault Tolerant Control System Design"*  
NASA Technical Memorandum 110226

**Available**: https://ntrs.nasa.gov/  
**Summary**: Triple redundancy, 2-out-of-3 voting, aerospace application.

---

## ONLINE RESOURCES

### 1. Tutorial & Documentation

**Espressif ESP32 ADC Documentation**  
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html  
**Content**: ADC resolution, sampling rate, noise consideration, calibration.

---

**Arduino Reference: Analog Input**  
https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/  
**Content**: ADC basics, analogRead() usage, noise mitigation.

---

**DSP Guide Online**  
http://www.dspguide.com/  
**Content**: Free online textbook, comprehensive DSP tutorial (filtering, FFT, convolution).

---

### 2. Tools & Calculators

**Filter Design Tool**  
http://t-filter.engineerjs.com/  
**Usage**: Design FIR/IIR filter, visualize frequency response, export C code.

---

**FFT Online Calculator**  
https://www.earlevel.com/main/category/digital-audio/  
**Usage**: FFT calculation, frequency analysis, filter design.

---

**Kalman Filter Interactive Demo**  
https://www.kalmanfilter.net/default.aspx  
**Usage**: Interactive Kalman filter tutorial dengan visualization.

---

### 3. Code Examples & Libraries

**Arduino PID Library**  
https://github.com/br3ttb/Arduino-PID-Library  
**Usage**: Reference implementation PID controller (will be used in Modul 6).

---

**SimpleKalmanFilter (Arduino)**  
https://github.com/denyssene/SimpleKalmanFilter  
**Usage**: Reference Kalman filter implementation, simple 1D example.

---

**MPU6050 Library (IMU)**  
https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050  
**Usage**: Read accelerometer & gyroscope data dari MPU6050.

---

**Encoder Library (Arduino)**  
https://www.pjrc.com/teensy/td_libs_Encoder.html  
**Usage**: Robust encoder reading dengan interrupt.

---

## VIDEO PEMBELAJARAN

### 1. Signal Processing Basics

**3Blue1Brown** — *"But what is the Fourier Transform? A visual introduction"*  
https://www.youtube.com/watch?v=spUNpyF58BY  
**Duration**: 20 min  
**Content**: Intuitive explanation FFT, frequency domain.

---

**Brian Douglas** — *"Understanding the FFT"*  
https://www.youtube.com/watch?v=fEHSGJJeAQM  
**Duration**: 15 min  
**Content**: FFT practical application, frequency analysis.

---

### 2. Kalman Filter

**Michel van Biezen** — *"Kalman Filter Explained"* (Playlist)  
https://www.youtube.com/playlist?list=PLX2gX-ftPVXU3oUFNATxGXY90AULiqnWT  
**Duration**: ~2 hours (multiple video)  
**Content**: Step-by-step Kalman filter derivation, example.

---

**Udacity** — *"Artificial Intelligence for Robotics"* (CS373)  
https://www.udacity.com/course/artificial-intelligence-for-robotics--cs373  
**Duration**: Self-paced  
**Content**: Kalman filter, particle filter, SLAM (free course).

---

### 3. Complementary Filter

**Philip Salmony** — *"Complementary Filter Design"*  
https://www.youtube.com/watch?v=whSw42XddsU  
**Duration**: 15 min  
**Content**: Complementary filter for IMU, practical implementation.

---

## DATASHEET & TECHNICAL DOCUMENTS

### Encoder

**Example**: Incremental Rotary Encoder E6B2-CWZ1X (Omron)  
**Datasheet**: https://www.ia.omron.com/  
**Key specs**: Resolution (PPR), output type (TTL, HTL), max RPM.

---

### IMU (Accelerometer + Gyroscope)

**MPU6050** (InvenSense/TDK)  
**Datasheet**: https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/  
**Key specs**:
- Accelerometer range: ±2g, ±4g, ±8g, ±16g
- Gyroscope range: ±250°/s, ±500°/s, ±1000°/s, ±2000°/s
- ADC resolution: 16-bit
- Noise density: 400 μg/√Hz (accel), 0.05 °/s/√Hz (gyro)

**Application note**: MPU-6050 Register Map and Descriptions (Rev. 4.2)

---

**BMI088** (Bosch) — High-performance 6-axis IMU  
**Datasheet**: https://www.bosch-sensortec.com/products/motion-sensors/imus/bmi088/  
**Advantage**: Lower noise vs MPU6050, separate accel & gyro die (less interference).

---

### Current Sensor (for Modul 5 integration)

**ACS712** (Allegro) — Hall-effect current sensor  
**Datasheet**: https://www.allegromicro.com/en/products/sense/current-sensor-ics/zero-to-fifty-amp-integrated-conductor-sensor-ics/acs712  
**Variants**: 5A, 20A, 30A  
**Output**: Analog voltage proportional to current.

---

## SOFTWARE & TOOLS

### Python Libraries

**NumPy** (Numerical Python)  
https://numpy.org/  
**Usage**: Array operations, mathematical functions.

```bash
pip install numpy
```

---

**SciPy** (Scientific Python)  
https://scipy.org/  
**Usage**: Signal processing (scipy.signal), FFT (scipy.fft), filtering.

```bash
pip install scipy
```

---

**Matplotlib** (Plotting)  
https://matplotlib.org/  
**Usage**: 2D plot, real-time animation.

```bash
pip install matplotlib
```

---

**Pandas** (Data Analysis)  
https://pandas.org/  
**Usage**: CSV handling, data manipulation.

```bash
pip install pandas
```

---

### Development Environment

**PlatformIO**  
https://platformio.org/  
**Usage**: Embedded development (Arduino, ESP32), library management.

**Install**:
- VS Code extension: search "PlatformIO IDE"
- Or CLI: `pip install platformio`

---

**Arduino IDE** (alternative to PlatformIO)  
https://www.arduino.cc/en/software  
**Usage**: Simple development environment, good for beginners.

---

### Oscilloscope Software (if using USB oscilloscope)

**Digilent WaveForms**  
https://digilent.com/shop/software/digilent-waveforms/  
**Compatibility**: Analog Discovery 2, compatible scope.

---

**PicoScope**  
https://www.picotech.com/downloads  
**Compatibility**: PicoScope USB oscilloscope.

---

## REFERENSI TAMBAHAN

### Application Notes

**Texas Instruments** — *"A Beginner's Guide to Accelerometers"* (SLAA508)  
https://www.ti.com/lit/an/slaa508/slaa508.pdf  
**Content**: Accelerometer basics, noise, filtering.

---

**Analog Devices** — *"A Practical Approach to Kalman Filter and How to Implement It"* (AN-1345)  
https://www.analog.com/  
**Content**: Kalman filter implementation, practical tips.

---

**Maxim Integrated** — *"Understanding Noise in Linear Regulators"* (Tutorial 4066)  
https://www.maximintegrated.com/  
**Content**: Noise source, filtering, power supply noise mitigation.

---

### Open-Source Projects

**ArduPilot** (Drone autopilot)  
https://github.com/ArduPilot/ardupilot  
**Relevant code**: `libraries/AP_AHRS/` (Attitude & Heading Reference System)  
**Usage**: Production-grade complementary filter & EKF implementation.

---

**BetaFlight** (Quadcopter flight controller)  
https://github.com/betaflight/betaflight  
**Relevant code**: `src/main/flight/` (gyro filter, complementary filter)  
**Usage**: Real-time filter implementation, optimized for speed.

---

**ROS2 robot_localization**  
https://github.com/cra-ros-pkg/robot_localization  
**Content**: EKF & UKF implementation untuk sensor fusion (encoder, IMU, GPS).

---

## COURSE & MOOC

**Coursera** — *"Robotics: Estimation and Learning"* (University of Pennsylvania)  
https://www.coursera.org/learn/robotics-learning  
**Content**: Kalman filter, particle filter, sensor fusion.  
**Duration**: 4 weeks, self-paced.

---

**edX** — *"Autonomous Navigation for Flying Robots"* (TU München)  
https://www.edx.org/course/autonomous-navigation-for-flying-robots  
**Content**: IMU sensor fusion, complementary filter, visual odometry.

---

**Udacity** — *"Sensor Fusion Engineer Nanodegree"*  
https://www.udacity.com/course/sensor-fusion-engineer-nanodegree--nd313  
**Content**: Kalman filter, LiDAR, radar, camera fusion (paid course).

---

## TOOLS & SIMULATORS

**MATLAB/Simulink** — Control System Toolbox  
https://www.mathworks.com/products/control.html  
**Usage**: Filter design, Kalman filter simulation, Bode plot.

**Example code** (MATLAB):
```matlab
% Design low-pass filter
fc = 5;  % cutoff frequency (Hz)
fs = 100;  % sampling rate (Hz)
[b, a] = butter(1, fc/(fs/2));  % 1st order Butterworth

% Apply filter
filtered = filter(b, a, raw_signal);

% Plot
plot(time, raw_signal, time, filtered);
legend('Raw', 'Filtered');
```

---

**Python Control Systems Library** (python-control)  
https://python-control.readthedocs.io/  
**Install**: `pip install control`

**Usage**: Transfer function, Bode plot, filter design (free alternative to MATLAB).

---

**GNU Octave** (MATLAB alternative, free)  
https://www.gnu.org/software/octave/  
**Usage**: Same syntax as MATLAB, free & open-source.

---

## CHEAT SHEET

### Filter Selection Matrix

| Scenario | Recommended Filter | Reason |
|----------|-------------------|--------|
| White noise, low latency requirement | Moving Average (small window) | Simple, predictable delay |
| White noise, high accuracy requirement | Low-Pass Filter (low cutoff) | Better smoothing than MA |
| Drift + noise (2 sensor) | Complementary Filter | Combine sensor strength |
| Optimal estimation (linear system) | Kalman Filter | Mathematically optimal |
| Nonlinear system | Extended Kalman Filter (EKF) | Handle nonlinearity |
| Non-Gaussian noise | Particle Filter | Robust to outlier |

---

### Kalman Filter Q/R Tuning Guide

| Scenario | Q (process noise) | R (measurement noise) | Effect |
|----------|-------------------|----------------------|--------|
| Trust model, distrust sensor | Small (0.001) | Large (100) | Smooth estimate, slow adapt |
| Distrust model, trust sensor | Large (1.0) | Small (1) | Follow measurement closely |
| Balanced | Medium (0.01) | Medium (10) | Balanced (start here) |

**Rule of thumb**:
- R ≈ sensor noise variance (measure dari data)
- Q ≈ 1% of R (starting point), then adjust

---

### Complementary Filter Weight Guide

| Application | W_Encoder | W_Accel | Reason |
|-------------|-----------|---------|--------|
| Encoder low noise | 0.9 | 0.1 | Trust encoder more |
| Encoder high noise, accel accurate | 0.5 | 0.5 | Equal weight |
| Encoder drift, accel stable | 0.3 | 0.7 | Trust accel more |
| **Default start point** | **0.7** | **0.3** | **Typical industrial** |

---

## CONTACT & SUPPORT

### Instructor
- **Name**: [Instructor Name]
- **Email**: [instructor@university.edu]
- **Office Hour**: [Day, Time]
- **Location**: [Room number]

### Lab Assistant
- **Name**: [Assistant Name]
- **Email**: [assistant@university.edu]
- **Availability**: Lab session only

### Discussion Forum
- **Platform**: [Discord/Slack/Forum link]
- **Channel**: #modul-04-sensor-fusion

### Repository
- **GitHub Classroom**: [classroom link]
- **Template code**: [repo template link]

---

## CHANGELOG

### Version 1.0 (February 2026)
- Initial release
- Added: Kalman filter reference, complementary filter paper
- Added: Python code example, MATLAB alternative

### Version 0.9 (January 2026)
- Draft version
- Basic reference list

---

**Compiled by**: [Instructor Name]  
**Last updated**: February 5, 2026  
**Module**: Batch 2 - Modul 4  
