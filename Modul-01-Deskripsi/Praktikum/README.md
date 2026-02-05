# PRAKTIKUM MODUL 1: ARSITEKTUR SISTEM
## Cara Menjalankan Semua Program

### STRUKTUR FOLDER

```
Praktikum/
├── Desain_CAD/          # File Fusion 360, STL, STEP
├── Desain_PCB/          # File EasyEDA, Gerber
├── Code_Embedded/       # PlatformIO project
│   └── esp32/
│       ├── platformio.ini
│       └── src/
│           ├── main.cpp
│           └── config.h
├── Code_PC/             # Python scripts, ROS2 (optional)
│   └── python_scripts/
├── output/              # Hasil test, plot, data
└── README.md            # File ini
```

---

## PREREQUISITES

### Software:
- **VS Code** dengan extension **PlatformIO IDE**
- **Python 3.8+** (untuk script analysis)
- **Git** (untuk version control)
- **Fusion 360** (untuk CAD)
- **EasyEDA** (online, untuk PCB)

### Hardware:
- ESP32 DevKit (atau Arduino Mega 2560)
- Motor DC 2 unit dengan encoder (opsional)
- Driver motor L298N (atau sejenisnya)
- Sensor: IR sensor, bumper switch
- Push button untuk E-Stop
- LED & Buzzer
- Breadboard & jumper wire
- Power supply 12V, 5V

---

## LANGKAH 1: INSTALL SOFTWARE

### 1.1 Install VS Code & PlatformIO

```bash
# Download VS Code
# https://code.visualstudio.com/

# Install PlatformIO extension:
# 1. Buka VS Code
# 2. Click Extensions (Ctrl+Shift+X)
# 3. Search "PlatformIO IDE"
# 4. Click Install
# 5. Reload VS Code
```

### 1.2 Install Python Dependencies

```bash
cd Praktikum/Code_PC/python_scripts
pip install -r requirements.txt
```

---

## LANGKAH 2: SETUP HARDWARE

### 2.1 Wiring Diagram

Ikuti wiring diagram di folder `Desain_PCB/` atau referensi table berikut:

| Component | Pin ESP32 | Notes |
|-----------|-----------|-------|
| Motor Left PWM | GPIO 25 | Connect to L298N ENA |
| Motor Left DIR | GPIO 26 | Connect to L298N IN1 |
| Motor Right PWM | GPIO 27 | Connect to L298N ENB |
| Motor Right DIR | GPIO 14 | Connect to L298N IN2 |
| Encoder Left A | GPIO 34 | Pull-up enabled |
| Encoder Left B | GPIO 35 | Pull-up enabled |
| IR Sensor Front | GPIO 36 (ADC) | Analog input |
| Bumper Switch | GPIO 23 | Active LOW, pull-up |
| E-Stop Button | GPIO 22 | Active LOW, pull-up |
| LED Status | GPIO 2 | Onboard LED |
| Buzzer | GPIO 19 | Active HIGH |

**⚠ IMPORTANT:**
- Verify voltage levels (ESP32 = 3.3V logic, motors might need 5V/12V)
- Add flyback diode di motor coil
- Add fuse di input power
- Double-check polarity sebelum power on

### 2.2 Power Supply

- **ESP32:** 5V via USB atau VIN pin
- **Motor:** 12V (sesuai spec motor)
- **Logic:** 5V untuk driver motor
- **Ground:** Common ground antara ESP32, driver, dan power supply

---

## LANGKAH 3: COMPILE & UPLOAD CODE

### 3.1 Open Project di PlatformIO

```bash
# Option 1: Via terminal
cd Praktikum/Code_Embedded/esp32
code .  # Opens VS Code in current directory

# Option 2: Via VS Code
# File → Open Folder → Navigate to Praktikum/Code_Embedded/esp32
```

### 3.2 Configure Settings

Edit file `src/config.h`:

```cpp
// Sesuaikan pin assignment jika berbeda
#define MOTOR_LEFT_PWM    25
#define MOTOR_LEFT_DIR    26
// ... dst

// Sesuaikan WiFi credentials (jika pakai WiFi)
#define WIFI_SSID         "YourWiFiSSID"
#define WIFI_PASSWORD     "YourPassword"

// Enable/disable debug output
#define DEBUG_SERIAL      true
#define DEBUG_MOTOR       true
```

### 3.3 Build & Upload

**Via PlatformIO GUI:**
1. Click PlatformIO icon di sidebar
2. Expand "esp32dev" environment
3. Click "Build" (checkmark icon)
4. Setelah build success, click "Upload" (arrow icon)

**Via Terminal:**
```bash
cd Praktikum/Code_Embedded/esp32

# Build only
pio run

# Build & Upload
pio run --target upload

# Open serial monitor after upload
pio device monitor
```

**Via VS Code Shortcuts:**
- Build: `Ctrl+Alt+B`
- Upload: `Ctrl+Alt+U`
- Serial Monitor: `Ctrl+Alt+S`

---

## LANGKAH 4: TEST PROGRAM

### 4.1 Serial Monitor

Setelah upload, buka serial monitor (115200 baud):

```
=== ESP32 System Architecture Demo ===
Initializing system...
✓ Pins configured
✓ System initialized
System state: IDLE
Press E-Stop button or send command via Serial
Commands: START, STOP, STATUS
```

### 4.2 Test Commands

Kirim command via serial monitor:

```
START      → System masuk ke state RUNNING, motor bergerak
STOP       → System kembali ke IDLE, motor stop
STATUS     → Tampilkan status current state & E-stop
```

### 4.3 Test E-Stop

1. Jalankan system dengan command `START`
2. Tekan tombol E-Stop (atau ground pin GPIO 22)
3. Observe:
   - Motor langsung stop
   - Buzzer bunyi
   - Serial output: "🛑 E-STOP ACTIVATED!"
   - LED berkedip cepat

### 4.4 Test Sensor

1. Jalankan system dengan `START`
2. Dekatkan objek di depan IR sensor
3. Observe: Motor stop jika jarak < threshold
4. Tekan bumper switch
5. Observe: Motor stop immediately

### 4.5 Expected Output

```
--- Telemetry ---
State: 2 (RUNNING)
E-Stop: Inactive
Uptime: 15 sec
----------------
→ Moving forward
--- Telemetry ---
State: 2
E-Stop: Inactive
Uptime: 16 sec
----------------
⚠ Obstacle detected, stopping
```

---

## LANGKAH 5: ANALISIS DATA

### 5.1 Log Data ke File

Redirect serial output ke file:

```bash
# Linux/macOS
pio device monitor > output/test_log_$(date +%Y%m%d_%H%M%S).txt

# Windows (PowerShell)
pio device monitor > output/test_log_$(Get-Date -Format "yyyyMMdd_HHmmss").txt
```

### 5.2 Analyze dengan Python

```bash
cd Praktikum/Code_PC/python_scripts
python analyze_telemetry.py ../../../output/test_log_20260205_143000.txt
```

Output:
- Grafik state transition vs time
- Statistik: average uptime per state
- Event timeline (E-stop, fault, dll)

---

## TROUBLESHOOTING

### Problem: "Upload failed" atau "Device not found"

**Solution:**
```bash
# Check connected devices
pio device list

# Specify upload port manually
pio run --target upload --upload-port /dev/ttyUSB0  # Linux
pio run --target upload --upload-port COM3          # Windows

# Install/update USB driver
# ESP32: CP210x atau CH340 driver
```

### Problem: Motor tidak bergerak

**Checklist:**
1. ✓ Check power supply (12V on motor power input?)
2. ✓ Check wiring (PWM & DIR pin correct?)
3. ✓ Check driver enable pin (L298N: ENA/ENB high?)
4. ✓ Check motor (connect directly to power, rotate?)
5. ✓ Check code (PWM value > MOTOR_MIN_SPEED?)

### Problem: Sensor tidak terbaca

**Checklist:**
1. ✓ Check wiring (VCC, GND, Signal correct?)
2. ✓ Check sensor power (LED on sensor menyala?)
3. ✓ Check analog read (Serial print raw value)
4. ✓ Check threshold (OBSTACLE_THRESHOLD terlalu tinggi/rendah?)

### Problem: E-Stop tidak response

**Checklist:**
1. ✓ Check button wiring (normally open, connect to GND saat pressed)
2. ✓ Check pin configuration (INPUT_PULLUP enabled?)
3. ✓ Check logic (active LOW, read == LOW saat pressed?)
4. ✓ Test button dengan multimeter (continuity test)

### Problem: Serial monitor no output

**Checklist:**
1. ✓ Check baud rate (115200 match dengan Serial.begin?)
2. ✓ Check USB cable (data cable, bukan charging only)
3. ✓ Check driver (CP210x/CH340 installed?)
4. ✓ Restart ESP32 (press reset button setelah open monitor)

---

## MODIFIKASI & EXPERIMENT

### Experiment 1: Ubah Control Frequency

Edit `config.h`:
```cpp
#define CONTROL_LOOP_HZ   50    // UBAH nilai ini (10, 20, 50, 100)
```

**Observe:** Responsiveness system berubah? Latency menurun?

### Experiment 2: Tune PID Parameters

Edit `config.h`:
```cpp
#define PID_KP            1.0   // UBAH nilai ini
#define PID_KI            0.5   // UBAH nilai ini
#define PID_KD            0.1   // UBAH nilai ini
```

**Observe:** Motor speed stability? Overshoot? Settling time?

### Experiment 3: Add New State

Edit `main.cpp`, tambahkan state baru:
```cpp
enum SystemState {
    // ... existing states
    STATE_CALIBRATION,  // New state
};
```

Implementasikan logic di `updateStateMachine()`.

---

## FILE OUTPUT

Semua hasil test disimpan di folder `output/`:

```
output/
├── test_log_20260205_143000.txt    # Serial log
├── telemetry_plot.png              # Grafik telemetry
├── state_timeline.png              # Timeline state transition
└── fmea_analysis.xlsx              # FMEA table
```

---

## RESOURCES

### Documentation:
- ESP32 Datasheet: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
- PlatformIO Docs: https://docs.platformio.org/
- Arduino Reference: https://www.arduino.cc/reference/

### Libraries:
- PubSubClient (MQTT): https://github.com/knolleary/pubsubclient
- ArduinoJson: https://arduinojson.org/

### Community:
- PlatformIO Forum: https://community.platformio.org/
- ESP32 Forum: https://esp32.com/

---

## CONTACT & SUPPORT

**Instruktur:**
- Email: [instructor@example.com]
- Office Hour: [schedule]

**GitHub Repository:**
- Repo: [https://github.com/your-org/praktikum-modul-1]
- Issues: [https://github.com/your-org/praktikum-modul-1/issues]

---

**Version:** 1.0  
**Last Updated:** 05-Feb-2026  
**Author:** [Your Name / Team]
