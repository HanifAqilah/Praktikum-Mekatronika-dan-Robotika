# JOBSHEET MODUL 05: IoT PLC SCHNEIDER MODBUS

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 05 – IoT PLC AI Modbus  
**Pertemuan:** 9–10 (2 × 2 SKS)  
**Tanggal:** ___________________  
**Nama Kelompok:** ___________________  
**Anggota:**

| No | Nama | NIM |
|----|------|-----|
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

Setelah menyelesaikan praktikum ini, mahasiswa mampu:

1. Memahami arsitektur PLC dan perbedaannya dengan mikrokontroler
2. Memprogram PLC Schneider menggunakan Ladder Diagram dan Structured Text
3. Memahami protokol Modbus (RTU dan TCP/IP) beserta register dan function code
4. Mengintegrasikan PLC Schneider dengan ESP32 via Modbus
5. Membuat Python client untuk membaca/menulis data PLC via Modbus TCP
6. Mengimplementasikan anomaly detection sederhana pada data PLC

---

## B. ALAT DAN BAHAN

| No | Komponen | Jumlah | Keterangan |
|----|---------|--------|-----------|
| 1 | PLC Schneider TM221CE16R | 1 | PLC utama |
| 2 | Power Supply 24V DC | 1 | Suplai PLC |
| 3 | ESP32 DevKit | 1 | Gateway IoT |
| 4 | MAX485 Module | 1 | Konverter TTL ke RS485 |
| 5 | Push Button NO | 3 | Input DI |
| 6 | LED Indikator 24V | 3 | Output DO (atau relay) |
| 7 | Potensiometer 10kΩ | 1 | Simulasi analog input |
| 8 | Kabel jumper + terminal | secukupnya | Wiring |
| 9 | Kabel Ethernet | 1 | Koneksi PLC ke jaringan |
| 10 | Laptop + Software | 1 | EcoStruxure + Arduino IDE + Python |

### Software

| No | Software | Fungsi |
|----|---------|--------|
| 1 | EcoStruxure Machine Expert Basic | Pemrograman PLC |
| 2 | Arduino IDE | Pemrograman ESP32 Modbus |
| 3 | Python 3 + pyModbus | Client Modbus TCP |
| 4 | MQTT Explorer | Debug MQTT (opsional) |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: PEMROGRAMAN PLC – LADDER DIAGRAM START/STOP MOTOR

**Estimasi waktu: 30 menit**

1. Buka EcoStruxure Machine Expert Basic
2. Buat project baru → pilih PLC TM221CE16R
3. Buat Ladder Diagram:
   - **Rung 1:** Start/Stop motor dengan self-holding
     ```
     I0.0 (Start)  I0.1 (Stop)     Q0.0 (Motor)
     ──┤ ├──────────┤/├────────────( )──
     Q0.0
     ──┤ ├─────────────────────────( )──
     ```
   - **Rung 2:** Lampu indikator mengikuti motor
     ```
     Q0.0                           Q0.1 (Lamp)
     ──┤ ├─────────────────────────( )──
     ```
4. Download ke PLC → Test dengan push button fisik
5. Verifikasi motor (LED) ON saat Start ditekan, OFF saat Stop ditekan

**Checkpoint ✅:** Start/Stop berfungsi dengan self-holding.

---

### PERCOBAAN 2: PEMROGRAMAN PLC – STRUCTURED TEXT COUNTER DAN TIMER

**Estimasi waktu: 30 menit**

1. Tambahkan program Structured Text baru
2. Implementasi counter dan timer:

```
(* Counter: hitung berapa kali I0.2 ditekan *)
IF I0.2 AND NOT prev_I02 THEN
    counter := counter + 1;
END_IF;
prev_I02 := I0.2;

(* Simpan counter ke Holding Register %MW0 *)
%MW0 := counter;

(* Timer: Q0.2 blink setiap 1 detik *)
timer := timer + 1;
IF timer >= 500 THEN  (* 500 × 2ms scan = 1 detik *)
    timer := 0;
    Q0.2 := NOT Q0.2;
END_IF;
%MW1 := timer;
```

3. Download dan verifikasi:
   - %MW0 bertambah setiap kali push button ditekan
   - LED Q0.2 berkedip setiap 1 detik

**Checkpoint ✅:** Counter dan timer berfungsi, nilai tersimpan di memory word.

---

### PERCOBAAN 3: MODBUS RTU – ESP32 MEMBACA REGISTER PLC

**Estimasi waktu: 45 menit**

**Wiring RS485:**
```
PLC RS485 A ── MAX485 A
PLC RS485 B ── MAX485 B
PLC GND     ── MAX485 GND
MAX485 DI   ── ESP32 GPIO17
MAX485 RO   ── ESP32 GPIO16
MAX485 DE+RE── ESP32 GPIO4
MAX485 VCC  ── ESP32 3.3V
```

**Konfigurasi PLC:** Pastikan RS485 diaktifkan: Modbus RTU Slave, Baud=9600, ID=1

**Kode ESP32:**

```cpp
#include <ModbusMaster.h>

ModbusMaster node;
#define DE_RE_PIN 4

void preTransmission() { digitalWrite(DE_RE_PIN, HIGH); }
void postTransmission() { digitalWrite(DE_RE_PIN, LOW); }

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);
  
  node.begin(1, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  uint8_t result = node.readHoldingRegisters(0, 4);
  if (result == node.ku8MBSuccess) {
    Serial.println("=== PLC Registers ===");
    for (int i = 0; i < 4; i++) {
      Serial.print("MW" + String(i) + ": ");
      Serial.println(node.getResponseBuffer(i));
    }
  } else {
    Serial.println("Modbus Error: " + String(result));
  }
  delay(1000);
}
```

**Yang harus diamati:**
1. Nilai %MW0 (counter) terbaca di Serial Monitor ESP32
2. Nilai %MW1 (timer) berubah-ubah
3. Tidak ada error komunikasi

**Checkpoint ✅:** ESP32 berhasil membaca register PLC via Modbus RTU.

---

### PERCOBAAN 4: MODBUS RTU – ESP32 MENULIS REGISTER PLC

**Estimasi waktu: 30 menit**

Tambahkan kode untuk menulis register:

```cpp
void loop() {
  // Baca sensor analog dari ESP32
  int sensorValue = analogRead(34);
  
  // Tulis ke PLC %MW10
  node.writeSingleRegister(10, sensorValue);
  Serial.println("Written to MW10: " + String(sensorValue));
  
  // Tulis coil (set Q0.0 ON)
  node.writeSingleCoil(8, true); // Coil 8 = Q0.0
  
  delay(1000);
}
```

Di PLC, buat program yang membaca %MW10:
```
(* Di Structured Text PLC *)
IF %MW10 > 2000 THEN
    Q0.1 := TRUE;   (* Alarm lamp ON jika sensor tinggi *)
ELSE
    Q0.1 := FALSE;
END_IF;
```

**Yang harus diamati:**
1. Nilai sensor ESP32 tersimpan di register PLC
2. LED PLC menyala ketika sensor melewati threshold
3. Komunikasi bidirectional berjalan

**Checkpoint ✅:** ESP32 berhasil menulis register dan coil PLC, PLC bereaksi.

---

### PERCOBAAN 5: MODBUS TCP/IP – PYTHON CLIENT

**Estimasi waktu: 30 menit**

**Prasyarat:** PLC terhubung ke jaringan via Ethernet, IP PLC diketahui.

```python
from pymodbus.client import ModbusTcpClient
import time

# Koneksi ke PLC
client = ModbusTcpClient('192.168.1.10', port=502)
connected = client.connect()
print(f"Connected: {connected}")

# Read Holding Registers (MW0 - MW9)
result = client.read_holding_registers(0, 10, slave=1)
if not result.isError():
    print("Registers MW0-MW9:", result.registers)
else:
    print("Error:", result)

# Write Holding Register MW10
client.write_register(10, 5000, slave=1)
print("Written 5000 to MW10")

# Read Coils (Input status I0.0 - I0.7)
coils = client.read_discrete_inputs(0, 8, slave=1)
if not coils.isError():
    print("Inputs I0.0-I0.7:", coils.bits[:8])

# Write Coil (Output Q0.0)
client.write_coil(8, True, slave=1)
print("Q0.0 set to ON")

# Continuous monitoring
for i in range(30):
    result = client.read_holding_registers(0, 5, slave=1)
    if not result.isError():
        print(f"[{i}] MW0={result.registers[0]}, MW1={result.registers[1]}")
    time.sleep(1)

client.close()
```

**Yang harus diamati:**
1. Python berhasil terhubung ke PLC via Ethernet
2. Register terbaca dengan benar
3. Output PLC bisa dikontrol dari Python

**Checkpoint ✅:** Python client berhasil R/W register dan coil PLC via Modbus TCP.

---

### PERCOBAAN 6: BRIDGE ESP32 – MODBUS RTU KE MQTT

**Estimasi waktu: 30 menit**

ESP32 berfungsi sebagai **gateway**: membaca PLC via Modbus RTU, mengirim ke MQTT.

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>

ModbusMaster node;
WiFiClient espClient;
PubSubClient mqtt(espClient);

void setup() {
  // Setup WiFi, MQTT, Modbus (seperti sebelumnya)
}

void loop() {
  if (!mqtt.connected()) mqtt.connect("PLC_Gateway");
  mqtt.loop();
  
  uint8_t result = node.readHoldingRegisters(0, 5);
  if (result == node.ku8MBSuccess) {
    for (int i = 0; i < 5; i++) {
      String topic = "plc/register/MW" + String(i);
      String value = String(node.getResponseBuffer(i));
      mqtt.publish(topic.c_str(), value.c_str());
    }
  }
  delay(2000);
}
```

**Checkpoint ✅:** Data PLC diteruskan ke MQTT broker via ESP32 gateway.

---

### PERCOBAAN 7: ANOMALY DETECTION DENGAN PYTHON

**Estimasi waktu: 40 menit**

```python
import numpy as np
from sklearn.ensemble import IsolationForest
from pymodbus.client import ModbusTcpClient
import time

client = ModbusTcpClient('192.168.1.10', port=502)
client.connect()

# Phase 1: Collect training data (normal operation, 60 samples)
print("Collecting training data... (60 seconds)")
training_data = []
for i in range(60):
    result = client.read_holding_registers(0, 3, slave=1)
    if not result.isError():
        training_data.append(result.registers)
        print(f"  Sample {i+1}/60: {result.registers}")
    time.sleep(1)

X_train = np.array(training_data)
print(f"Training data shape: {X_train.shape}")

# Phase 2: Train model
model = IsolationForest(contamination=0.1, random_state=42)
model.fit(X_train)
print("Model trained!")

# Phase 3: Real-time prediction
print("\nStarting real-time monitoring...")
anomaly_count = 0
total_count = 0
while total_count < 60:
    result = client.read_holding_registers(0, 3, slave=1)
    if not result.isError():
        total_count += 1
        prediction = model.predict([result.registers])
        status = "ANOMALY" if prediction[0] == -1 else "Normal"
        if prediction[0] == -1:
            anomaly_count += 1
        print(f"[{total_count}] {result.registers} -> {status}")
    time.sleep(1)

print(f"\nTotal: {total_count}, Anomalies: {anomaly_count}")
client.close()
```

**Yang harus diamati:**
1. Model berhasil training dari data normal PLC
2. Anomali terdeteksi saat operasi berubah signifikan
3. Akurasi deteksi (false positive/negative)

**Checkpoint ✅:** Sistem anomaly detection berjalan pada data PLC.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1 – Ladder Diagram

1. Jelaskan prinsip self-holding pada Ladder Diagram. Mengapa diperlukan?
2. Apa fungsi kontak NC (normally closed) pada tombol STOP?
3. Bandingkan scan cycle PLC dengan loop() Arduino. Apa perbedaan utamanya?

### Analisa Percobaan 2 – Structured Text

1. Mengapa pointer `prev_I02` digunakan untuk deteksi rising edge?
2. Apa keuntungan Structured Text dibanding Ladder untuk logika kompleks?

### Analisa Percobaan 3 – Modbus RTU Read

1. Jelaskan fungsi pin DE/RE pada MAX485. Mengapa harus dikontrol saat transmisi?
2. Apa yang terjadi jika baud rate PLC dan ESP32 tidak sama?
3. Berapa slave maksimal pada satu jaringan RS485?

### Analisa Percobaan 4 – Modbus RTU Write

1. Jelaskan perbedaan write coil (FC 05) dan write register (FC 06).
2. Bagaimana memastikan data yang ditulis ke PLC valid dan aman?

### Analisa Percobaan 5 – Modbus TCP Python

1. Bandingkan performa Modbus RTU vs TCP. Mana yang lebih cepat? Mengapa?
2. Apa keuntungan menggunakan Python sebagai Modbus client dibanding ESP32?

### Analisa Percobaan 6 – Bridge Modbus-MQTT

1. Jelaskan arsitektur sistem bridge. Mengapa ESP32 berperan sebagai gateway?
2. Apa tantangan jika PLC perlu diakses dari internet?

### Analisa Percobaan 7 – Anomaly Detection

1. Jelaskan cara kerja Isolation Forest untuk deteksi anomali.
2. Apa yang terjadi jika training data sudah mengandung anomali?
3. Bagaimana meningkatkan akurasi model? Sebutkan minimal 3 cara.

---

## E. KESIMPULAN

Tuliskan minimal 5 poin kesimpulan:

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] Screenshot program Ladder Diagram di EcoStruxure
- [ ] Screenshot program Structured Text
- [ ] Foto wiring PLC + ESP32 + MAX485
- [ ] Screenshot Serial Monitor ESP32 (Modbus RTU)
- [ ] Screenshot Python client (Modbus TCP)
- [ ] Screenshot MQTT data dari PLC gateway
- [ ] Screenshot output anomaly detection
- [ ] Kode program lengkap (PLC + ESP32 + Python)

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
