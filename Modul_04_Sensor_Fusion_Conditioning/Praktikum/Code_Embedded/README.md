# Sensor Fusion & Conditioning - Code Implementation

This directory contains embedded firmware for sensor fusion experiments (Modul 4).

## Structure

```
Code_Embedded/
├── src/
│   ├── main.cpp           # Main program entry point
│   ├── config.h           # All parameters & pin definitions
│   ├── sensor.h/cpp       # Sensor reading functions
│   ├── filter.h           # Filter implementations (MA, Low-pass)
│   ├── kalman.h           # Kalman filter class
│   └── fusion.cpp/.h      # Sensor fusion & voting logic
├── platformio.ini         # PlatformIO configuration
└── README.md              # This file
```

## Hardware Requirements

- ESP32 DevKit or Arduino Mega 2560
- Incremental encoder (500+ PPR recommended)
- IMU module (MPU6050, BMI088, or similar)
- Motor with encoder (for testing)
- Oscilloscope (for signal analysis)

## Pin Configuration

See `config.h` for pin mapping:
- Encoder A/B pins
- IMU I2C (SDA, SCL)
- Motor PWM & direction pins

## Building & Uploading

### Using PlatformIO

```bash
# Build
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

### Using Arduino IDE

1. Open `main.cpp` in Arduino IDE
2. Select board: ESP32 Dev Module (or Arduino Mega 2560)
3. Select port: /dev/ttyUSB0 (or appropriate port)
4. Click Upload

## Running Experiments

### Percobaan 1: Sensor Noise Analysis
```bash
# Upload firmware
pio run -t upload

# Collect data (10 seconds)
pio device monitor > ../output/sensor_data.csv

# Analyze with Python
cd ../output
python noise_analysis.py
```

### Percobaan 2-3: Filter Comparison
```bash
# Modify config.h to select filter
#define FILTER_TYPE MOVING_AVERAGE  // or LOWPASS

# Upload & test
pio run -t upload
pio device monitor
```

### Percobaan 4: Complementary Filter
```bash
# Enable complementary filter
#define FILTER_TYPE COMPLEMENTARY

# Tune weights in config.h
#define W_ENCODER 0.7
#define W_ACCEL 0.3
```

### Percobaan 5: Kalman Filter
```bash
# Enable Kalman filter
#define FILTER_TYPE KALMAN

# Tune Q/R in config.h
#define KALMAN_Q 0.001
#define KALMAN_R 10.0
```

### Percobaan 6: Sensor Voting
```bash
# Enable voting logic
#define ENABLE_VOTING true
#define NUM_SENSORS 3
```

## Serial Output Format

CSV format for easy Python analysis:

```
timestamp, encoder_raw, accel_x, encoder_filtered, position_fused
0, 0, 0.05, 0.0, 0.0
10, 5, 0.12, 1.2, 1.1
20, 12, 0.08, 3.5, 3.2
...
```

## Dependencies

Listed in `platformio.ini`:
- Adafruit MPU6050 library (for IMU)
- Encoder library (for robust encoder reading)

## Troubleshooting

### Encoder not counting
- Check wiring (A, B, GND)
- Verify interrupt pins (ESP32: any pin, Mega: pins 2, 3, 18-21)
- Check pull-up resistors (enable internal or external)

### IMU not responding
- Check I2C connection (SDA, SCL, VCC, GND)
- Scan I2C address: `i2c_scanner.ino` (Arduino example)
- Default address MPU6050: 0x68 or 0x69

### Kalman filter diverge
- Reduce Q (trust model more)
- Increase R (distrust sensor more)
- Check measurement noise estimate (run noise analysis first)

### Serial monitor garbled
- Check baud rate: 115200 (match code & monitor setting)
- Ensure correct port selected

## Performance Benchmarks

Measured on ESP32 @ 240MHz:

| Filter Type | CPU Usage | Memory (bytes) | Latency (μs) |
|-------------|-----------|----------------|--------------|
| Moving Average (W=10) | 2% | 40 | 15 |
| Low-Pass Filter | 1% | 8 | 8 |
| Complementary Filter | 5% | 50 | 25 |
| Kalman Filter (2-state) | 12% | 200 | 80 |

## Author

[Student Name]  
[Student ID]  
Praktikum Mekatronika dan Robotika - Batch 2 - Modul 4

## License

MIT License (for educational purposes)
