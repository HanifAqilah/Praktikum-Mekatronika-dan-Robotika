# MATERI MODUL 05: IoT PLC SCHNEIDER MODBUS

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – IoT PLC AI Modbus  
**Hardware:** PLC Schneider TM221/TM241, ESP32  
**Software:** SoMachine Basic / EcoStruxure Machine Expert, Python  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan PLC dan Otomasi Industri](#1-pendahuluan-plc-dan-otomasi-industri)
2. [PLC Schneider Electric](#2-plc-schneider-electric)
3. [Bahasa Pemrograman PLC (IEC 61131-3)](#3-bahasa-pemrograman-plc-iec-61131-3)
4. [Protokol Modbus](#4-protokol-modbus)
5. [Modbus RTU vs Modbus TCP/IP](#5-modbus-rtu-vs-modbus-tcpip)
6. [Register dan Function Code Modbus](#6-register-dan-function-code-modbus)
7. [Integrasi PLC dengan ESP32 via Modbus](#7-integrasi-plc-dengan-esp32-via-modbus)
8. [Python Modbus Client](#8-python-modbus-client)
9. [AI untuk Predictive Maintenance](#9-ai-untuk-predictive-maintenance)
10. [Keamanan Sistem SCADA/IoT Industri](#10-keamanan-sistem-scadaiot-industri)
11. [Referensi](#11-referensi)

---

## 1. PENDAHULUAN PLC DAN OTOMASI INDUSTRI

### 1.1 Apa itu PLC?

**PLC (Programmable Logic Controller)** adalah komputer industri khusus yang dirancang untuk mengontrol proses manufaktur dan otomasi. PLC menggantikan panel relay konvensional dengan keunggulan:

- **Programmable** — logika kontrol bisa diubah via software
- **Reliable** — dirancang untuk lingkungan industri (debu, suhu, getaran)
- **Real-time** — scan cycle dalam milidetik
- **Modular** — bisa diperluas dengan modul I/O tambahan
- **Komunikasi** — mendukung protokol industri (Modbus, Profibus, EtherNet/IP)

### 1.2 Arsitektur PLC

```
┌─────────────────────────────────────────────┐
│                    PLC                       │
│  ┌──────┐  ┌────────────┐  ┌─────────────┐ │
│  │INPUT │→ │   CPU      │→ │  OUTPUT     │ │
│  │Module│  │(Processor) │  │  Module     │ │
│  │DI/AI │  │ Program    │  │  DO/AO      │ │
│  └──────┘  │ Memory     │  └─────────────┘ │
│            │ Comm Port  │                   │
│            └────────────┘                   │
│  ┌──────────────────────────────┐           │
│  │  Communication Module        │           │
│  │  RS485 / Ethernet / WiFi     │           │
│  └──────────────────────────────┘           │
└─────────────────────────────────────────────┘
```

### 1.3 Scan Cycle PLC

```
[1. Read Inputs] → [2. Execute Program] → [3. Update Outputs] → [4. Communication] → [Ulangi]
```

Waktu scan tipikal: 1–10 ms, tergantung kompleksitas program.

### 1.4 PLC vs Mikrokontroler

| Aspek | PLC | Mikrokontroler (ESP32) |
|-------|-----|----------------------|
| Reliability | Industrial-grade | Consumer-grade |
| I/O | Modul standar, plug & play | Perlu rangkaian tambahan |
| Programming | Ladder, FBD, ST | C/C++, Python |
| Real-time | Deterministik | Soft real-time |
| Harga | Jutaan rupiah | Puluhan ribu |
| Sertifikasi | CE, UL, IEC 61131 | Tidak standar |
| Penggunaan | Pabrik, SCADA | IoT, prototyping, robot kecil |

---

## 2. PLC SCHNEIDER ELECTRIC

### 2.1 Schneider TM221 Series

| Parameter | Spesifikasi |
|-----------|-----------|
| Seri | TM221CE16R / TM221CE24R |
| CPU | ARM Cortex |
| Digital Input | 9–14 DI (24V DC) |
| Digital Output | 7–10 DO (Relay) |
| Analog Input | 2 AI (0–10V) |
| Communication | 1× Serial RS485, 1× Ethernet |
| Protocol | Modbus RTU (RS485), Modbus TCP (Ethernet) |
| Programming | SoMachine Basic / EcoStruxure Machine Expert Basic |
| Power | 24V DC |

### 2.2 Software Pemrograman

**EcoStruxure Machine Expert Basic** (sebelumnya SoMachine Basic):
- Download gratis dari website Schneider
- Mendukung Ladder Diagram dan Function Block Diagram
- Simulator built-in untuk testing tanpa PLC fisik
- Monitoring online real-time

### 2.3 Wiring PLC

```
Power Supply 24V DC ──→ L+(24V), M(0V) PLC
Sensor Proximity  ──→ DI0 (I0.0)
Push Button Start ──→ DI1 (I0.1)
Push Button Stop  ──→ DI2 (I0.2)
Motor Contactor   ←── DO0 (Q0.0)
Indicator Lamp    ←── DO1 (Q0.1)
RS485 (A, B, GND) ──→ ESP32 (via MAX485)
Ethernet          ──→ Switch/Router
```

---

## 3. BAHASA PEMROGRAMAN PLC (IEC 61131-3)

### 3.1 Lima Bahasa Standar

| Bahasa | Tipe | Keterangan |
|--------|------|-----------|
| **Ladder Diagram (LD)** | Grafis | Mirip diagram relay, paling populer |
| **Function Block Diagram (FBD)** | Grafis | Blok fungsi dihubungkan |
| **Structured Text (ST)** | Teks | Mirip Pascal/C |
| **Instruction List (IL)** | Teks | Mirip assembly (sudah deprecated) |
| **Sequential Function Chart (SFC)** | Grafis | Flowchart step-transition |

### 3.2 Ladder Diagram (LD)

```
     I0.0     I0.1          Q0.0
  ──┤ ├──────┤/├──────────( )──
  (Start)   (Stop)      (Motor)
  
     Q0.0
  ──┤ ├──────────────────( )──
  (Self-holding / Latch)
```

### 3.3 Structured Text (ST)

```
IF I0.0 AND NOT I0.1 THEN
    Q0.0 := TRUE;    (* Motor ON *)
    Q0.1 := TRUE;    (* Lamp ON *)
ELSIF I0.1 THEN
    Q0.0 := FALSE;   (* Motor OFF *)
    Q0.1 := FALSE;   (* Lamp OFF *)
END_IF;
```

### 3.4 Tipe Data PLC

| Tipe | Ukuran | Range | Contoh |
|------|--------|-------|--------|
| BOOL | 1 bit | TRUE/FALSE | I0.0, Q0.0 |
| INT | 16 bit | -32768 to 32767 | %MW0 |
| DINT | 32 bit | -2^31 to 2^31-1 | %MD0 |
| REAL | 32 bit | Float | %MF0 |
| WORD | 16 bit | 0 to 65535 | %MW0 |

---

## 4. PROTOKOL MODBUS

### 4.1 Sejarah dan Konsep

**Modbus** dikembangkan oleh Modicon (sekarang Schneider Electric) pada 1979 sebagai protokol komunikasi serial untuk PLC. Modbus menjadi standar de facto industri karena:
- **Terbuka** — tidak ada lisensi
- **Sederhana** — mudah diimplementasikan
- **Universal** — didukung hampir semua perangkat industri

### 4.2 Model Master-Slave

```
[Master]  ──Request──>  [Slave 1 (ID=1)]
          <──Response──
          ──Request──>  [Slave 2 (ID=2)]
          <──Response──
          ──Request──>  [Slave N (ID=N)]
          <──Response──
```

- **Master** menginisiasi komunikasi (selalu bertanya)
- **Slave** merespons (tidak pernah memulai komunikasi)
- Maksimal 247 slave per jaringan RS485

### 4.3 Frame Modbus RTU

```
[Slave ID (1 byte)] [Function Code (1 byte)] [Data (N bytes)] [CRC (2 bytes)]
```

Contoh: Read Holding Register (FC=03) dari slave ID=1, register 0, jumlah 2
```
Request:  01 03 00 00 00 02 C4 0B
Response: 01 03 04 00 64 00 C8 XX XX
                    ^    ^
                    100  200 (data register)
```

---

## 5. MODBUS RTU vs MODBUS TCP/IP

| Aspek | Modbus RTU | Modbus TCP/IP |
|-------|-----------|---------------|
| Media | RS232/RS485 (serial) | Ethernet |
| Kecepatan | 9600–115200 baud | 10/100 Mbps |
| Jarak | 1200 m (RS485) | 100 m (per segment) |
| Addressing | Slave ID (1–247) | IP Address + Unit ID |
| Port | COM/Serial | TCP port 502 |
| Error check | CRC-16 | TCP checksum |
| Topology | Bus (daisy-chain) | Star (switch/hub) |
| Concurrency | 1 master | Multiple client |

### 5.1 Modbus TCP Frame

```
[MBAP Header (7 bytes)] [Function Code (1 byte)] [Data (N bytes)]

MBAP Header:
  Transaction ID (2) | Protocol ID (2) | Length (2) | Unit ID (1)
```

---

## 6. REGISTER DAN FUNCTION CODE MODBUS

### 6.1 Empat Tipe Data Modbus

| Tipe | Alamat | Akses | Ukuran | Contoh |
|------|--------|-------|--------|--------|
| **Coil** | 0xxxx | Read/Write | 1 bit | Output relay |
| **Discrete Input** | 1xxxx | Read only | 1 bit | Input sensor digital |
| **Input Register** | 3xxxx | Read only | 16 bit | Analog input |
| **Holding Register** | 4xxxx | Read/Write | 16 bit | Setpoint, parameter |

### 6.2 Function Code Utama

| FC | Nama | Fungsi |
|----|------|--------|
| 01 | Read Coils | Baca status coil (DO) |
| 02 | Read Discrete Inputs | Baca status input (DI) |
| 03 | Read Holding Registers | Baca register R/W |
| 04 | Read Input Registers | Baca register read-only |
| 05 | Write Single Coil | Tulis 1 coil |
| 06 | Write Single Register | Tulis 1 register |
| 15 | Write Multiple Coils | Tulis banyak coil |
| 16 | Write Multiple Registers | Tulis banyak register |

### 6.3 Mapping Register PLC Schneider

| Register PLC | Alamat Modbus | Tipe | Keterangan |
|-------------|--------------|------|-----------|
| %I0.0–%I0.7 | Coil 0–7 | Discrete Input | Input digital |
| %Q0.0–%Q0.7 | Coil 8–15 | Coil | Output digital |
| %MW0 | Register 0 | Holding Register | Memory word 0 |
| %MW1 | Register 1 | Holding Register | Memory word 1 |
| %IW0.0 | Input Reg 0 | Input Register | Analog input |

---

## 7. INTEGRASI PLC DENGAN ESP32 VIA MODBUS

### 7.1 Koneksi Hardware (Modbus RTU via RS485)

```
PLC RS485          MAX485 Module         ESP32
  A ────────────── A                     
  B ────────────── B                     
  GND ──────────── GND ──────────────── GND
                   DI  ──────────────── GPIO17 (TX2)
                   RO  ──────────────── GPIO16 (RX2)
                   DE + RE ──────────── GPIO4 (Direction)
                   VCC ──────────────── 3.3V
```

### 7.2 Library ESP32 Modbus Master

```cpp
#include <ModbusMaster.h>

ModbusMaster node;

#define MAX485_DE 4
#define MAX485_RE 4

void preTransmission() {
  digitalWrite(MAX485_DE, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_DE, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_DE, LOW);
  
  node.begin(1, Serial2); // Slave ID = 1
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  uint8_t result;
  
  // Read Holding Register 0 (1 register)
  result = node.readHoldingRegisters(0, 1);
  if (result == node.ku8MBSuccess) {
    Serial.print("Register 0: ");
    Serial.println(node.getResponseBuffer(0));
  }
  
  // Write Holding Register 1
  node.writeSingleRegister(1, 1234);
  
  delay(1000);
}
```

### 7.3 Koneksi Modbus TCP/IP

```cpp
#include <WiFi.h>
#include <ModbusTCPClient.h>

WiFiClient wifiClient;
ModbusTCPClient modbusTCP(wifiClient);

void setup() {
  WiFi.begin("SSID", "PASS");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  modbusTCP.begin("192.168.1.10", 502); // IP PLC
}

void loop() {
  if (modbusTCP.connected()) {
    int value = modbusTCP.holdingRegisterRead(0);
    Serial.println("Register 0: " + String(value));
    
    modbusTCP.coilWrite(8, HIGH); // Write coil Q0.0
  }
  delay(1000);
}
```

---

## 8. PYTHON MODBUS CLIENT

### 8.1 Instalasi pyModbus

```bash
pip3 install pymodbus
```

### 8.2 Python Modbus TCP Client

```python
from pymodbus.client import ModbusTcpClient

client = ModbusTcpClient('192.168.1.10', port=502)
client.connect()

# Read Holding Register
result = client.read_holding_registers(0, 10, slave=1)
if not result.isError():
    print("Registers:", result.registers)

# Write Holding Register
client.write_register(0, 1234, slave=1)

# Read Coil (output)
coils = client.read_coils(0, 8, slave=1)
print("Coils:", coils.bits[:8])

# Write Coil
client.write_coil(8, True, slave=1)

client.close()
```

### 8.3 Python Modbus RTU Client

```python
from pymodbus.client import ModbusSerialClient

client = ModbusSerialClient(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity='N',
    stopbits=1,
    bytesize=8,
    timeout=1
)

client.connect()
result = client.read_holding_registers(0, 5, slave=1)
print("Registers:", result.registers)
client.close()
```

---

## 9. AI UNTUK PREDICTIVE MAINTENANCE

### 9.1 Konsep Predictive Maintenance

Menggunakan data historis sensor dari PLC/IoT untuk **memprediksi kegagalan mesin** sebelum terjadi:

```
[Sensor Data] → [Data Collection] → [Feature Extraction] → [ML Model] → [Prediction]
  Vibration       Modbus/MQTT        Statistical features    Random Forest   Normal/Anomaly
  Temperature                        FFT, RMS, Peak          Neural Network
  Current                            Mean, Std, Kurtosis     SVM
```

### 9.2 Contoh Sederhana: Anomaly Detection

```python
import numpy as np
from sklearn.ensemble import IsolationForest
from pymodbus.client import ModbusTcpClient
import time

client = ModbusTcpClient('192.168.1.10', port=502)
client.connect()

# Collect training data (normal operation)
training_data = []
for i in range(100):
    result = client.read_holding_registers(0, 3, slave=1)
    if not result.isError():
        training_data.append(result.registers)
    time.sleep(1)

X_train = np.array(training_data)

# Train Isolation Forest
model = IsolationForest(contamination=0.1, random_state=42)
model.fit(X_train)

# Real-time prediction
while True:
    result = client.read_holding_registers(0, 3, slave=1)
    if not result.isError():
        prediction = model.predict([result.registers])
        if prediction[0] == -1:
            print("⚠️ ANOMALY DETECTED:", result.registers)
        else:
            print("✅ Normal:", result.registers)
    time.sleep(1)
```

### 9.3 Framework AI untuk IoT Industri

| Framework | Keterangan |
|-----------|-----------|
| scikit-learn | ML klasik (Random Forest, SVM, Isolation Forest) |
| TensorFlow Lite | Neural network di edge device |
| ONNX Runtime | Model inference cross-platform |
| Apache Kafka | Stream processing data besar |
| InfluxDB | Time-series database untuk data sensor |

---

## 10. KEAMANAN SISTEM SCADA/IoT INDUSTRI

### 10.1 Risiko Keamanan

| Risiko | Dampak |
|--------|--------|
| Modbus tanpa autentikasi | Siapa saja di jaringan bisa kontrol PLC |
| Firmware outdated | Exploitasi vulnerability |
| Flat network | Serangan lateral dari IT ke OT |
| Credential default | Password PLC tidak diubah |

### 10.2 Best Practice Keamanan

1. **Segmentasi jaringan** — pisahkan IT dan OT network
2. **Firewall industrial** — filter traffic Modbus
3. **VPN** untuk remote access
4. **Ganti password default** PLC
5. **Update firmware** secara berkala
6. **Monitoring anomali** traffic jaringan

---

## 11. REFERENSI

1. Modbus Organization. (2006). *Modbus Application Protocol V1.1b3*. http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf
2. Modbus-IDA. (2006). *Modbus Messaging Implementation Guide V1.0b3*. http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b3.pdf
3. Schneider Electric. (2024). *TM221/TM241 PLC Documentation*. https://www.schneider-electric.com/
4. pyModbus. (2024). *Python Modbus Library*. https://github.com/pymodbus-dev/pymodbus
5. Laplante, P. A. (2018). *Industrial IoT with Modbus*. IEEE Xplore. https://ieeexplore.ieee.org/document/8352394
6. IEC 61131-3. (2013). *Programmable Controllers – Programming Languages*.
7. Schneider Electric. (2024). *EcoStruxure Machine Expert*. https://www.schneider-electric.com/

---

Dokumen ini menjadi acuan utama materi Modul 05 IoT PLC Schneider Modbus.
