# Materi Modul 10: Robot Line Follower

## Identitas Modul

| Keterangan | Detail |
|---|---|
| Program Studi | Sarjana Terapan Teknologi Rekayasa Otomasi |
| Mata Kuliah | Praktikum Mekatronika |
| Beban SKS | 2 SKS, 5 jam 40 menit praktikum |
| Dosen Pengampu | Rofiq Cahyo Prayogo, S.T., M.T. |
| Topik | Robot Line Follower – ESP32 S2 Mini, Sensor IR 8ch, H-Bridge, PID, Path Planning |

---

## Capaian Pembelajaran

Setelah mempelajari modul ini, praktikan mampu:

1. Menjelaskan prinsip kerja robot line follower secara menyeluruh (sensor → kontrol → aktuator).
2. Merangkai ESP32 S2 Mini, OLED I2C, push button, driver motor H-bridge, motor DC, sensor IR 8 channel.
3. Membaca nilai ADC sensor garis, melakukan kalibrasi, dan mengonversi ke status digital.
4. Menghitung posisi garis menggunakan weighted average.
5. Mengendalikan motor DC dengan PWM dan H-bridge.
6. Menerapkan kontrol on-off, proportional (P), proportional-derivative (PD), dan PID penuh.
7. Melakukan tuning parameter Kp, Ki, Kd secara sistematis.
8. Menangani kondisi lost line, belokan tajam, persimpangan, dan finish line.
9. Memahami dan mengimplementasikan path planning berbasis sensor mask dan state machine.
10. Menyimpan dan memuat konfigurasi menggunakan EEPROM.

---

# BAB 1 — Pengantar Robot Line Follower

## 1.1 Definisi dan Konsep Dasar

Robot line follower adalah robot mobile otonom yang mengikuti lintasan berupa garis, biasanya garis hitam di atas permukaan putih (atau kebalikannya). Robot membaca perbedaan reflektansi permukaan menggunakan sensor infrared array, memperkirakan posisi garis relatif terhadap badan robot, lalu mengatur kecepatan motor kiri dan kanan secara diferensial agar robot tetap di jalur.

Sistem ini merupakan contoh klasik sistem mekatronika terpadu karena menggabungkan:

- **Mekanik**: chassis, roda penggerak, roda bebas, dudukan sensor, gearbox.
- **Elektronik**: mikrokontroler, sensor, driver motor, catu daya, display.
- **Kendali**: loop umpan balik (feedback control) berbasis error posisi garis.
- **Perangkat Lunak**: akuisisi data, filtering, pengambilan keputusan, pembangkitan sinyal PWM.

## 1.2 Diagram Blok Sistem

```
╔══════════════════════════════════════════════════════════════════╗
║                  DIAGRAM BLOK LINE FOLLOWER                     ║
╠══════════════════════════════════════════════════════════════════╣
║                                                                  ║
║   Lintasan ──► Sensor IR ──► ADC ESP32 ──► Estimasi Posisi      ║
║                                                  │               ║
║                                           Kontrol PID           ║
║                                                  │               ║
║                                             PWM Signal          ║
║                                                  │               ║
║                                        Driver Motor H-Bridge    ║
║                                                  │               ║
║                                         Motor DC L  Motor DC R  ║
║                                                  │               ║
║                                          Gerak Robot ◄──────────║
║                                                  │               ║
║   Sensor IR ◄─────────────────── Posisi Fisik Robot             ║
║   (umpan balik posisi garis)                                    ║
╚══════════════════════════════════════════════════════════════════╝
```

## 1.3 Prinsip Deteksi Garis

Sensor infrared reflektif terdiri dari:
- **LED IR** (transmitter): memancarkan cahaya infrared (λ ≈ 940 nm) ke permukaan.
- **Photodiode/phototransistor** (receiver): mendeteksi cahaya pantul.

Prinsip kerja:
- **Permukaan putih**: memantulkan ~80–90% cahaya → tegangan output **tinggi** → nilai ADC **besar**.
- **Permukaan hitam**: menyerap ~85–95% cahaya → tegangan output **rendah** → nilai ADC **kecil**.

---

# BAB 2 — Dasar Listrik dan Elektronika

## 2.1 Hukum Ohm

$$V = I \cdot R$$

$$I = \frac{V}{R}$$

$$R = \frac{V}{I}$$

Dimana:
- $V$ = tegangan [Volt, V]
- $I$ = arus listrik [Ampere, A]
- $R$ = hambatan/resistansi [Ohm, Ω]

## 2.2 Daya Listrik

$$P = V \cdot I = I^2 \cdot R = \frac{V^2}{R}$$

Dimana $P$ = daya [Watt, W].

## 2.3 Tegangan Pembagi (Voltage Divider)

$$V_{out} = V_{in} \cdot \frac{R_2}{R_1 + R_2}$$

Digunakan dalam rangkaian sensor untuk membagi tegangan sinyal.

## 2.4 Kapasitor — Rumus Dasar

$$Q = C \cdot V$$

$$I = C \cdot \frac{dV}{dt}$$

$$E = \frac{1}{2} C V^2$$

Dimana:
- $Q$ = muatan [Coulomb, C]
- $C$ = kapasitansi [Farad, F]
- $E$ = energi tersimpan [Joule, J]

## 2.5 Induktor — Rumus Dasar (relevan untuk motor)

$$V_L = L \cdot \frac{dI}{dt}$$

$$E = \frac{1}{2} L I^2$$

Dimana $L$ = induktansi [Henry, H]. Motor DC memiliki induktansi kumparan yang perlu diperhatikan saat switching PWM.

## 2.6 Hukum Kirchhoff

**KVL (Kirchhoff Voltage Law):**
$$\sum V = 0 \quad \text{(dalam satu loop tertutup)}$$

**KCL (Kirchhoff Current Law):**
$$\sum I_{masuk} = \sum I_{keluar} \quad \text{(pada satu simpul/node)}$$

---

# BAB 3 — ESP32 S2 Mini (Mikrokontroler)

## 3.1 Spesifikasi Utama

| Parameter | Nilai |
|---|---|
| CPU | Xtensa LX7 Single-core, 240 MHz |
| Flash | 4 MB |
| RAM | 320 KB SRAM |
| ADC | 2 × 13-bit SAR ADC, max 20 channel |
| PWM | LEDC 8 channel, frekuensi fleksibel |
| I2C | 2 × I2C, kecepatan hingga 1 MHz |
| Tegangan Operasi | 3.3 V |
| GPIO | 43 pin (beberapa dengan fungsi khusus) |
| USB | Tipe-C, USB 2.0 OTG |

## 3.2 ADC (Analog-to-Digital Converter)

Rumus konversi tegangan ke nilai ADC:

$$ADC_{value} = \frac{V_{in}}{V_{ref}} \cdot (2^n - 1)$$

Untuk ESP32 S2 dengan resolusi 13-bit ($n=13$) dan $V_{ref} = 3.3$ V:

$$ADC_{value} = \frac{V_{in}}{3.3} \cdot 8191$$

Resolusi ADC (tegangan per bit):

$$\Delta V = \frac{V_{ref}}{2^n} = \frac{3.3}{8192} \approx 0.403 \text{ mV/bit}$$

Konversi balik (nilai ADC ke tegangan):

$$V_{in} = \frac{ADC_{value}}{8191} \cdot 3.3 \text{ V}$$

> **Catatan**: ESP32 ADC memiliki non-linearitas, terutama di range tegangan rendah (< 0.15V) dan tinggi (> 3.1V). Untuk akurasi lebih baik, gunakan `analogSetAttenuation()` atau kalibrasi dengan lookup table.

## 3.3 PWM (Pulse Width Modulation)

Duty cycle:

$$D = \frac{t_{on}}{T} \times 100\%$$

Tegangan rata-rata output PWM:

$$V_{avg} = D \cdot V_{supply} = \frac{t_{on}}{T} \cdot V_{supply}$$

Periode sinyal PWM:

$$T = \frac{1}{f_{PWM}}$$

Resolusi PWM ESP32 LEDC (n-bit):

$$\text{Nilai PWM} \in [0, \; 2^n - 1]$$

Untuk 8-bit: nilai 0–255, dimana 255 = 100% duty cycle.

**Contoh**: `ledcWrite(channel, 128)` → duty cycle = $\frac{128}{255} \times 100\% \approx 50\%$

Frekuensi PWM yang digunakan: 5000 Hz (menghindari bunyi audio, efisien untuk motor).

## 3.4 I2C (Inter-Integrated Circuit)

$$f_{SCL} \leq 400 \text{ kHz (Fast Mode)}$$

Frame data I2C:
1. **START condition**: SDA turun saat SCL tinggi.
2. **Address frame** (7 bit): alamat perangkat + bit R/W.
3. **ACK/NACK**: konfirmasi dari slave.
4. **Data frames** (8 bit each): data yang dikirim/diterima.
5. **STOP condition**: SDA naik saat SCL tinggi.

Pin I2C pada praktikum ini: **SDA = GPIO 33**, **SCL = GPIO 35** (custom, menggunakan `Wire.begin(33, 35)`).

---

# BAB 4 — OLED I2C SSD1306

## 4.1 Spesifikasi

| Parameter | Nilai |
|---|---|
| Controller | SSD1306 |
| Resolusi | 128 × 64 piksel |
| Tipe Display | OLED monokrom |
| Interface | I2C (400 kHz) |
| Alamat I2C | 0x3C (default) atau 0x3D |
| Tegangan | 3.3 V / 5 V |

## 4.2 Prinsip Kerja OLED

OLED (Organic Light-Emitting Diode) memancarkan cahaya sendiri (tidak butuh backlight). Setiap piksel dikontrol secara individual melalui buffer RAM internal SSD1306. Alur kerja:

```
clearDisplay() → set buffer RAM → display() → layer fisik OLED
```

Fungsi penting library Adafruit SSD1306:

```cpp
lcd.clearDisplay();           // Bersihkan buffer (belum tampil)
lcd.setTextSize(n);           // Ukuran font (1=kecil, 2=sedang, 3=besar)
lcd.setTextColor(WHITE);      // Warna teks
lcd.setCursor(x, y);          // Posisi kursor (pixel)
lcd.print("teks");            // Tulis teks ke buffer
lcd.fillCircle(x, y, r, c);  // Gambar lingkaran penuh
lcd.drawLine(x1,y1,x2,y2,c); // Gambar garis
lcd.display();                // *** WAJIB dipanggil agar tampil ***
```

---

# BAB 5 — Push Button dan Debouncing

## 5.1 Cara Kerja Push Button

Button dihubungkan dengan resistor pull-up internal ESP32 (`INPUT_PULLUP`):
- Tidak ditekan: pin = HIGH (1)
- Ditekan: pin = LOW (0) → **active LOW**

## 5.2 Masalah Bouncing

Saat tombol ditekan/dilepas, sinyal mekanis mengalami *bouncing* (getaran cepat) selama 5–50 ms. Tanpa debouncing, satu penekanan bisa terbaca sebagai banyak penekanan.

## 5.3 Algoritma Debouncing Software

```
Deteksi edge (perubahan state) → catat timestamp
Jika (millis() - timestamp) > DEBOUNCE_TIME (50ms) → state valid
```

```cpp
bool readButton(int pin, bool &lastState, unsigned long &lastTime) {
  bool current = digitalRead(pin);
  if (current != lastState) {
    if (millis() - lastTime > 50) {
      lastState = current;
      lastTime = millis();
      return (current == LOW); // Aktif saat LOW
    }
  }
  return false;
}
```

## 5.4 Edge Detection (Rising/Falling)

```
Rising edge (LOW→HIGH): tombol dilepas
Falling edge (HIGH→LOW): tombol ditekan
```

---

# BAB 6 — Motor DC dan Driver H-Bridge

## 6.1 Prinsip Kerja Motor DC

Motor DC mengubah energi listrik menjadi energi mekanik berdasarkan gaya Lorentz:

$$F = B \cdot I \cdot L$$

Torka motor:

$$\tau = k_t \cdot I$$

Kecepatan angular (tanpa beban):

$$\omega = \frac{V - I \cdot R_a}{k_e}$$

Dimana:
- $F$ = gaya [Newton, N]
- $B$ = kerapatan fluks magnet [Tesla, T]
- $I$ = arus [Ampere, A]
- $L$ = panjang konduktor efektif [meter, m]
- $\tau$ = torka [Newton·meter, N·m]
- $k_t$ = konstanta torka motor
- $\omega$ = kecepatan angular [rad/s]
- $V$ = tegangan supply [V]
- $R_a$ = hambatan armature [Ω]
- $k_e$ = konstanta back-EMF

## 6.2 Gearbox

Robot line follower menggunakan motor DC dengan gearbox untuk meningkatkan torka:

$$\tau_{output} = \tau_{motor} \times \text{gear ratio}$$

$$\omega_{output} = \frac{\omega_{motor}}{\text{gear ratio}}$$

**Kecepatan linier roda**:
$$v = \omega_{output} \times r_{roda}$$

## 6.3 H-Bridge — Prinsip Kerja

H-Bridge adalah rangkaian 4 saklar (transistor/MOSFET) yang memungkinkan arus mengalir dalam dua arah melalui motor:

```
     +Vcc
      |
   S1   S2
   |     |
   +--M--+    (Motor DC)
   |     |
   S3   S4
      |
     GND
```

| S1 | S2 | S3 | S4 | Kondisi Motor |
|---|---|---|---|---|
| ON | OFF | OFF | ON | Maju |
| OFF | ON | ON | OFF | Mundur |
| OFF | OFF | OFF | OFF | Float (bebas) |
| ON | OFF | ON | OFF | Rem aktif (brake) |

## 6.4 Mode Kontrol Motor (dalam kode praktikum)

**Mode 2-Pin (MOTOR_MODE_2PIN)**:
- EN (Enable): sinyal PWM untuk kecepatan
- IN1, IN2: sinyal digital untuk arah

| IN1 | IN2 | EN | Kondisi |
|---|---|---|---|
| HIGH | LOW | PWM | Maju dengan kecepatan PWM% |
| LOW | HIGH | PWM | Mundur dengan kecepatan PWM% |
| HIGH | HIGH | X | Rem/Brake |
| LOW | LOW | X | Coast (bebas) |

## 6.5 Kontrol Kecepatan dengan PWM

```cpp
// LEDC ESP32 setup
ledcSetup(channel, freq, resolution);
ledcAttachPin(pin, channel);
ledcWrite(channel, duty); // 0-255 untuk 8-bit
```

Hubungan kecepatan motor dengan duty cycle:

$$v_{motor} \approx v_{max} \cdot \frac{duty}{255}$$

> Hubungan ini tidak linear sempurna karena adanya tegangan minimum untuk mengatasi inersia dan gesekan (dead zone). Biasanya motor mulai bergerak pada duty ≥ 30–80 (dari 255).

---

# BAB 7 — Sensor Infrared 8 Channel

## 7.1 Komponen Sensor

Sensor yang digunakan: **TCRT5000** atau **QTR-8A** (array 8 sensor).

Setiap sensor terdiri dari:
- LED IR emitter (λ ≈ 940 nm)
- Phototransistor receiver
- Resistor pembatas arus

## 7.2 Karakteristik Output

Output sensor adalah tegangan analog yang berbanding dengan intensitas cahaya pantul:

$$V_{out} = f(\text{reflektansi permukaan}, \text{jarak sensor-permukaan})$$

Nilai ADC tipikal (ESP32 S2 Mini, 13-bit):
- Di atas garis **hitam**: 0 – 1500
- Di atas permukaan **putih**: 2500 – 8191

Jarak optimal sensor ke permukaan: **3–5 mm**

## 7.3 Kalibrasi Sensor

Tujuan kalibrasi: menemukan nilai minimum (hitam) dan maksimum (putih) untuk setiap sensor, lalu menentukan threshold.

**Rumus threshold**:

$$T_i = \frac{V_{min,i} + V_{max,i}}{2}$$

Dimana:
- $T_i$ = threshold sensor ke-$i$
- $V_{min,i}$ = nilai ADC minimum (di atas garis hitam)
- $V_{max,i}$ = nilai ADC maksimum (di atas permukaan putih)

**Prosedur kalibrasi otomatis** (Program 04):
1. Robot bergerak maju pelan selama 2 detik → catat $V_{max}$ setiap sensor
2. Robot bergerak mundur pelan selama 2 detik → catat $V_{min}$ setiap sensor
3. Hitung threshold: $T_i = (V_{min,i} + V_{max,i}) / 2$
4. Simpan threshold ke EEPROM

## 7.4 Konversi Analog ke Digital

$$d_i = \begin{cases} 1 & \text{jika } ADC_i < T_i \quad \text{(garis hitam terdeteksi)} \\ 0 & \text{jika } ADC_i \geq T_i \quad \text{(permukaan putih)} \end{cases}$$

Hasilnya adalah array binary 8-bit: `[d7 d6 d5 d4 d3 d2 d1 d0]`

Contoh: `[0 0 0 1 1 0 0 0]` → garis di tengah (sensor 3 dan 4 aktif).

## 7.5 Pin Sensor pada Praktikum

```
Sensor 0 (kiri ekstrem) → GPIO 1
Sensor 1                 → GPIO 2
Sensor 2                 → GPIO 3
Sensor 3                 → GPIO 4
Sensor 4                 → GPIO 5
Sensor 5                 → GPIO 16
Sensor 6                 → GPIO 17
Sensor 7 (kanan ekstrem) → GPIO 18
```

---

# BAB 8 — Estimasi Posisi Garis

## 8.1 Metode Weighted Average (Rata-Rata Tertimbang)

Metode paling umum untuk mengestimasi posisi garis dari array sensor:

$$\text{pos} = \frac{\sum_{i=0}^{N-1} w_i \cdot d_i}{\sum_{i=0}^{N-1} d_i}$$

Dimana:
- $N$ = jumlah sensor (8)
- $d_i$ = nilai digital sensor ke-$i$ (0 atau 1)
- $w_i$ = bobot posisi sensor ke-$i$

Bobot sensor (untuk 8 sensor, posisi garis 0–7000):

$$w = [0, \; 1000, \; 2000, \; 3000, \; 4000, \; 5000, \; 6000, \; 7000]$$

Nilai tengah = 3500 (garis tepat di tengah antara sensor 3 dan 4).

**Error posisi** (setpoint = 3500):

$$\text{error} = 3500 - \text{pos}$$

- Error = 0: garis di tengah
- Error > 0: garis di sebelah kiri robot
- Error < 0: garis di sebelah kanan robot

## 8.2 Metode Normalized Position

Posisi dinormalisasi ke rentang -1.0 hingga +1.0:

$$\text{pos}_{norm} = \frac{\text{pos} - \frac{(N-1) \times 1000}{2}}{(N-1) \times 1000 / 2}$$

## 8.3 Metode Switch-Case (Digunakan dalam Kode)

Untuk penanganan kasus spesifik (persimpangan, garis hilang), digunakan pemetaan langsung dari pola sensor biner 8-bit ke nilai posisi integer:

```cpp
int calculatePosition(bool sensors[8]) {
  uint8_t pattern = 0;
  for (int i = 0; i < 8; i++) {
    if (sensors[i]) pattern |= (1 << i);
  }
  switch (pattern) {
    case 0b00011000: return 0;    // Tengah
    case 0b00001000: return -1;   // Sedikit kiri
    case 0b00110000: return 1;    // Sedikit kanan
    // ... dst
    case 0b11111111: return 99;   // Semua aktif (finish/intersection)
    default: return lastPos;      // Pertahankan posisi terakhir
  }
}
```

---

# BAB 9 — Teori Kontrol

## 9.1 Sistem Kendali Loop Tertutup

```
         ┌──────────┐    e(t)   ┌──────────┐    u(t)   ┌────────┐
r(t) ─+─►│ Kontroler├──────────►│  Plant   ├──────────►│ Output │
      ↑   └──────────┘          └────┬─────┘           └───┬────┘
      │                              │                      │
      │   ┌──────────┐               │                      │
      └───┤  Sensor  │◄──────────────┴──────────────────────┘
          └──────────┘  y(t)
```

Dimana:
- $r(t)$ = setpoint (nilai yang diinginkan)
- $y(t)$ = output aktual
- $e(t) = r(t) - y(t)$ = error
- $u(t)$ = sinyal kontrol (output kontroler)

## 9.2 Kontrol On-Off (Bang-Bang)

Kontrol paling sederhana:

$$u(t) = \begin{cases} u_{max} & \text{jika } e(t) > 0 \\ u_{min} & \text{jika } e(t) \leq 0 \end{cases}$$

Untuk line follower:
```
jika posisi di kiri garis → belok kanan penuh
jika posisi di kanan garis → belok kiri penuh
```

**Kekurangan**: osilasi tinggi, tidak halus.

## 9.3 Kontrol Proportional (P)

$$u(t) = K_p \cdot e(t)$$

Untuk line follower:

$$\text{correction} = K_p \times \text{error}$$

$$v_L = v_{base} + \text{correction}$$

$$v_R = v_{base} - \text{correction}$$

**Efek $K_p$**:
- $K_p$ terlalu kecil → respon lambat, tidak bisa ikut tikungan tajam.
- $K_p$ terlalu besar → osilasi (goyang kiri-kanan), tidak stabil.

## 9.4 Kontrol Proportional-Integral (PI)

$$u(t) = K_p \cdot e(t) + K_i \int_0^t e(\tau) d\tau$$

Diskritisasi (implementasi digital):

$$\text{integral}_{k} = \text{integral}_{k-1} + e_k \cdot \Delta t$$

$$u_k = K_p \cdot e_k + K_i \cdot \text{integral}_k$$

**Efek $K_i$**:
- Menghilangkan *steady-state error* (offset permanen).
- $K_i$ terlalu besar → *integral windup*, overshoot, tidak stabil.

**Anti-Windup** (penting!):
```cpp
if (abs(integral) > INTEGRAL_LIMIT) {
  integral = INTEGRAL_LIMIT * sign(integral);
}
```

## 9.5 Kontrol Proportional-Derivative (PD)

$$u(t) = K_p \cdot e(t) + K_d \cdot \frac{de(t)}{dt}$$

Diskritisasi:

$$\frac{de}{dt} \approx \frac{e_k - e_{k-1}}{\Delta t}$$

$$u_k = K_p \cdot e_k + K_d \cdot \frac{e_k - e_{k-1}}{\Delta t}$$

**Efek $K_d$**:
- Memprediksi tren error → mengurangi overshoot dan osilasi.
- $K_d$ terlalu kecil → overshoot besar.
- $K_d$ terlalu besar → sensitif terhadap noise, respon lambat.

## 9.6 Kontrol PID Penuh

$$u(t) = K_p \cdot e(t) + K_i \int_0^t e(\tau) d\tau + K_d \cdot \frac{de(t)}{dt}$$

**Implementasi digital (diskrit)**:

$$\boxed{u_k = K_p \cdot e_k + K_i \cdot \sum_{j=0}^{k} e_j \cdot \Delta t + K_d \cdot \frac{e_k - e_{k-1}}{\Delta t}}$$

**Implementasi dalam kode (Program 05)**:

```cpp
// Hitung komponen PID
float P = config.pid_kp * error;
float I_val = config.pid_ki * integral;
float D = config.pid_kd * (error - lastError);

float correction = P + I_val + D;

// Aplikasikan ke motor
int leftSpeed  = config.base_speed + (int)correction;
int rightSpeed = config.base_speed - (int)correction;

// Batasi kecepatan
leftSpeed  = constrain(leftSpeed,  -255, 255);
rightSpeed = constrain(rightSpeed, -255, 255);

// Update state
integral  += error;
lastError  = error;
```

## 9.7 Diagram Respons PID

```
Respon Sistem terhadap Step Input
                               Setpoint ─────────────────────
                             ╱
Kp kecil:          ─────────╱
Kp optimal: ────────────────────────────────────── (sedikit overshoot)
                   ╱────────╲──────────────────
Kp besar:  ───────╱  osilasi ╲╱╲╱──────────────
```

## 9.8 Analisis Stabilitas — Ziegler-Nichols

Metode tuning Ziegler-Nichols:

1. Set $K_i = 0$, $K_d = 0$.
2. Naikkan $K_p$ secara bertahap sampai sistem mulai osilasi berkelanjutan → catat $K_u$ (ultimate gain) dan $T_u$ (ultimate period).
3. Gunakan tabel:

| Mode Kontrol | $K_p$ | $K_i$ | $K_d$ |
|---|---|---|---|
| P | $0.5 \cdot K_u$ | — | — |
| PI | $0.45 \cdot K_u$ | $\frac{1.2 \cdot K_u}{T_u}$ | — |
| PID | $0.6 \cdot K_u$ | $\frac{2 \cdot K_u}{T_u}$ | $\frac{K_u \cdot T_u}{8}$ |

> **Catatan untuk line follower**: Metode Z-N sering menghasilkan respon agresif. Gunakan sebagai titik awal, lalu fine-tune manual.

## 9.9 Differential Drive Kinematics

Robot dengan dua roda penggerak (differential drive):

**Kecepatan linier dan angular robot**:

$$v = \frac{v_R + v_L}{2}$$

$$\omega = \frac{v_R - v_L}{d}$$

**Radius belokan**:

$$R = \frac{d}{2} \cdot \frac{v_R + v_L}{v_R - v_L}$$

**Perubahan posisi dalam interval waktu $\Delta t$**:

$$\Delta x = v \cdot \cos(\theta) \cdot \Delta t$$

$$\Delta y = v \cdot \sin(\theta) \cdot \Delta t$$

$$\Delta \theta = \omega \cdot \Delta t$$

Dimana:
- $v_R$, $v_L$ = kecepatan linier roda kanan/kiri [m/s]
- $d$ = jarak antar roda (wheelbase) [m]
- $\omega$ = kecepatan angular robot [rad/s]
- $v$ = kecepatan linier robot [m/s]
- $\theta$ = orientasi robot [rad]

---

# BAB 10 — Tuning PID untuk Line Follower

## 10.1 Panduan Tuning Manual (Sistematis)

### Nilai Awal (Starting Point)

```cpp
float Kp = 0.05;      // Mulai dari sini
float Ki = 0.0;       // Tambahkan terakhir (optional)
float Kd = 0.0;       // Tambahkan setelah Kp stabil
int baseSpeed = 120;  // Kecepatan dasar (aman untuk pemula)
```

> Nilai ini untuk sensor 8 channel, setpoint = 3500, error range ±3500.

### Langkah 1: Tuning Kp (Proportional Only)

1. Set $K_i = 0$, $K_d = 0$, mulai $K_p = 0.05$, baseSpeed = 120.
2. Jalankan robot di lintasan lurus.
3. Amati dan diagnosis berdasarkan tabel:

| Perilaku Robot | Diagnosis | Aksi |
|---|---|---|
| Robot tidak mengikuti garis, terlalu lambat berbelok | Kp terlalu kecil | Naikkan +0.01 |
| Robot zig-zag cepat, tidak stabil | Kp terlalu besar | Turunkan -0.01 |
| Robot mengikuti garis, osilasi kecil | Kp sudah bagus | Lanjut ke Langkah 2 |

**Target**: robot mengikuti garis di lintasan lurus dengan osilasi yang dapat diterima.

### Langkah 2: Tuning Kd (Tambah Derivative)

1. Pertahankan $K_p$ dari Langkah 1. Mulai $K_d = 0.5$.
2. Test di lintasan dengan tikungan.
3. Amati dan diagnosis:

| Perilaku Robot | Diagnosis | Aksi |
|---|---|---|
| Masih zig-zag di tikungan | Kd terlalu kecil | Naikkan +0.5 |
| Robot sangat lambat bereaksi, keluar di tikungan tajam | Kd terlalu besar | Turunkan -0.2 |
| Robot stabil di lurus dan tikungan | Kd optimal | Selesai (atau lanjut Ki) |

**Target**: robot mengikuti garis smooth, stabil di tikungan. **Kd target ≈ 1.5.**

### Langkah 3: Tuning Ki (Opsional — hanya jika offset persisten)

Kapan perlu $K_i$:
- Robot selalu offset ke satu sisi (kiri atau kanan) secara konsisten.
- Robot tidak bisa kembali ke center setelah tikungan (steady-state error).

Jika tidak ada masalah di atas, **$K_i = 0$ sudah optimal.**

1. Mulai $K_i = 0.001$. Naikkan +0.001 jika masih offset. Turunkan jika robot mulai overshoot.
2. **Selalu sertakan anti-windup**:

```cpp
integral += error * dt;
integral = constrain(integral, -ANTI_WINDUP, ANTI_WINDUP);
#define ANTI_WINDUP 10000  // batas integral
```

> **PERINGATAN**: $K_i$ terlalu besar menyebabkan integral windup → robot tidak stabil dan beroskilasi semakin parah.

### Langkah 4: Tuning Base Speed

1. Mulai dari 120. Naikkan +10 sampai robot mulai tidak stabil.
2. Turunkan -5 sebagai safety margin.
3. Dengan naiknya kecepatan, mungkin perlu tuning ulang $K_p$ dan $K_d$.

## 10.2 Parameter Awal (dalam config.h)

```cpp
#define PID_KP         0.08
#define PID_KI         0.0
#define PID_KD         0.4
#define PID_BASE_SPEED 150
#define PID_MAX_SPEED  255
```

## 10.3 Tabel Referensi Parameter PID

| Level | $K_p$ | $K_i$ | $K_d$ | Speed |
|---|---:|---:|---:|---:|
| Pemula (lintasan mudah) | 0.05–0.08 | 0 | 0.5–1.5 | 100–130 |
| Menengah (lintasan sedang) | 0.08–0.12 | 0 | 1.0–2.0 | 130–160 |
| Advanced (kompetisi) | 0.10–0.15 | 0–0.002 | 1.5–3.0 | 150–200 |

## 10.4 Contoh Kasus Tuning Nyata

### Kasus 1 — Pemula (Lintasan Sederhana)

| Iterasi | Kp | Ki | Kd | Speed | Hasil |
|---:|---:|---:|---:|---:|---|
| 1 | 0.05 | 0 | 0 | 120 | Lambat, keluar di tikungan |
| 2 | 0.08 | 0 | 0 | 120 | Zig-zag cepat |
| 3 | 0.08 | 0 | 1.0 | 120 | Stabil di lurus |
| 4 | 0.08 | 0 | 1.5 | 120 | Stabil, tikungan OK |
| 5 | 0.08 | 0 | 1.5 | 130 | Final |

**Hasil final**: $K_p=0.08$, $K_i=0$, $K_d=1.5$, Speed=130.

### Kasus 2 — Menengah (Lintasan dengan Tikungan Tajam)

| Iterasi | Kp | Ki | Kd | Speed | Hasil |
|---:|---:|---:|---:|---:|---|
| 1 | 0.08 | 0 | 1.5 | 130 | Bagus di lurus, keluar di tikungan tajam |
| 2 | 0.10 | 0 | 1.5 | 130 | Lebih responsif |
| 3 | 0.10 | 0 | 1.8 | 130 | Stabil di tikungan tajam |
| 4 | 0.10 | 0 | 1.8 | 135 | Final |

**Hasil final**: $K_p=0.10$, $K_i=0$, $K_d=1.8$, Speed=135.

## 10.5 Perhitungan Manual PID

Contoh kondisi nyata:
- $K_p = 0.08$, $K_d = 1.5$, $K_i = 0$, baseSpeed = 130
- position = 1000, lastError = −2000

$$\text{error} = 1000 - 3500 = -2500$$

$$P = 0.08 \times (-2500) = -200$$

$$D = 1.5 \times (-2500 - (-2000)) = 1.5 \times (-500) = -750$$

$$\text{correction} = -200 + (-750) = -950$$

$$v_L = 130 + (-950) = -820 \xrightarrow{\text{constrain}} 0$$

$$v_R = 130 - (-950) = 1080 \xrightarrow{\text{constrain}} 255$$

**Interpretasi**: Robot berada di kanan garis (position=1000, setpoint=3500). Motor kiri berhenti, motor kanan full → robot berbelok kiri untuk kembali ke tengah.

## 10.6 Pengaruh Kecepatan pada Parameter PID

Semakin tinggi kecepatan, semakin agresif koreksi yang diperlukan:

$$K_p^{high} \approx K_p^{low} \times \frac{v_{low}}{v_{high}}$$

## 10.7 Multi-Mode PID

Untuk performa optimal, gunakan parameter PID berbeda per kondisi:

| Kondisi | $K_p$ | $K_d$ | $v_{base}$ |
|---|---|---|---|
| Track lurus | Kecil | Besar | Tinggi |
| Tikungan moderat | Sedang | Sedang | Sedang |
| Tikungan tajam | Besar | Kecil | Rendah |


# BAB 11 — Lost Line Handling

## 11.1 Deteksi Garis Hilang

Kondisi lost line terjadi ketika tidak ada sensor yang mendeteksi garis (semua sensor membaca putih):

$$\text{lost\_line} = \begin{cases} \text{TRUE} & \text{jika } \sum_{i=0}^{7} d_i = 0 \\ \text{FALSE} & \text{sebaliknya} \end{cases}$$

## 11.2 Strategi Recovery

```cpp
if (lostLine) {
  if (lostTimer > LOST_LINE_TIMEOUT) { // misal 500ms
    // Strategi recovery berdasarkan error terakhir
    if (lastError > 0) {
      // Garis di kiri terakhir → putar kiri
      setMotors(-searchSpeed, searchSpeed);
    } else {
      // Garis di kanan terakhir → putar kanan
      setMotors(searchSpeed, -searchSpeed);
    }
  }
} else {
  lostTimer = 0; // Reset timer jika garis ditemukan
}
```

## 11.3 Deteksi Finish Line dan Intersection

- **Finish line** (semua sensor aktif): `pattern == 0xFF`
- **T-junction kanan**: sensor 7 dan 0 aktif bersamaan
- **T-junction kiri**: sensor 0 dan beberapa lainnya
- **Cross junction**: banyak sensor aktif sekaligus

---

# BAB 12 — Path Planning

## 12.1 Konsep Waypoint Navigation

Path planning untuk line follower berbasis *waypoint*: serangkaian titik penting di track (persimpangan, tikungan, finish) yang masing-masing memiliki:
- **Trigger**: kondisi sensor yang menandai waypoint tercapai.
- **Action**: aksi yang dilakukan (belok kiri/kanan, lurus, stop).
- **Speed profile**: kecepatan turbo, kecepatan base, dan pengereman.

## 12.2 State Machine Eksekusi Path

```
┌─────────────┐    timeout     ┌─────────────┐    sensor      ┌─────────────┐
│ STATE_TURBO ├───────────────►│ STATE_BASE  ├───────────────►│STATE_ACTION │
└─────────────┘                └─────────────┘   trigger      └──────┬──────┘
                                                                       │
                                                               ┌───────▼─────┐
                                                               │STATE_BRAKE  │
                                                               └───────┬─────┘
                                                                       │ timeout
                                                               ┌───────▼─────┐
                                                               │ STATE_NEXT  │
                                                               └─────────────┘
```

## 12.3 Sensor Mask

Sensor trigger dikodekan sebagai bitmask 8-bit:

$$\text{mask} = \sum_{i=0}^{7} d_i \cdot 2^i$$

Contoh:
- Sensor kanan aktif (S7): `mask = 0b10000000 = 0x80`
- Sensor kiri aktif (S0): `mask = 0b00000001 = 0x01`
- Kedua ujung aktif (T-junction): `mask = 0b10000001 = 0x81`
- Sensor tengah aktif: `mask = 0b00011000 = 0x18`

Deteksi trigger:
```cpp
bool triggered = (currentSensorPattern & triggerMask) == triggerMask;
```

## 12.4 Speed Profile

Setiap segmen path memiliki profil kecepatan:

| Fase | Kecepatan | Durasi | Tujuan |
|---|---|---|---|
| TURBO | v_turbo (tinggi) | timer_turbo ms | Momentum awal, melewati bagian lurus |
| BASE | v_base (normal) | sampai trigger | Approach waypoint dengan kontrol presisi |
| ACTION | variabel | tergantung aksi | Eksekusi belokan/stop |
| BRAKE | rem_speed (mundur) | timer_rem ms | Fine positioning, hilangkan momentum |

## 12.5 Contoh Konfigurasi Path

```cpp
PathPoint path[] = {
  // Path 0: Turbo start, lurus ke persimpangan kanan
  {SENSOR_RIGHT, 0, 0, ACTION_RIGHT, 200, 150, 300, 80, 100, "Awal→Kanan"},
  // Path 1: Setelah belok, lurus ke persimpangan kiri
  {SENSOR_LEFT,  0, 0, ACTION_LEFT,  180, 130, 200, 0,  0,   "Segmen→Kiri"},
  // Path 2: Stop di finish (semua sensor)
  {SENSOR_BOTH,  0, 0, ACTION_STOP,  150, 120, 0,   0,  0,   "Finish"}
};
```

---

# BAB 13 — EEPROM dan Penyimpanan Konfigurasi

## 13.1 Konsep EEPROM

EEPROM (Electrically Erasable Programmable Read-Only Memory) adalah memori non-volatile yang mempertahankan data setelah power dimatikan. ESP32 menggunakan simulasi EEPROM di flash memory.

## 13.2 Struktur Data Konfigurasi

```cpp
struct LineFollowerConfig {
  uint16_t magic;              // Validasi: 0xABCD
  uint8_t  version;            // Versi struktur
  uint16_t sensor_threshold;   // Threshold default sensor
  bool     line_type_black;    // true = garis hitam
  uint16_t sensor_min[8];      // Nilai minimum kalibrasi
  uint16_t sensor_max[8];      // Nilai maksimum kalibrasi
  float    pid_kp;             // Gain Proportional
  float    pid_ki;             // Gain Integral
  float    pid_kd;             // Gain Derivative
  uint8_t  base_speed;         // Kecepatan dasar
  uint8_t  max_speed;          // Kecepatan maksimum
  uint16_t checksum;           // Validasi integritas data
};
```

## 13.3 Checksum untuk Validasi

$$\text{checksum} = \sum_{i=0}^{sizeof(config)-2} \text{byte}_i$$

Jika checksum tidak cocok saat load, konfigurasi direset ke default untuk mencegah penggunaan data korup.

---

# BAB 14 — Analisis Performa Robot

## 14.1 Metrik Performa

| Metrik | Definisi | Rumus / Cara Ukur |
|---|---|---|
| Waktu tempuh | Waktu dari start ke finish | Stopwatch atau millis() |
| Rata-rata error | Error rata-rata selama perjalanan | $\bar{e} = \frac{1}{N}\sum\|e_i\|$ |
| Error RMS | Root Mean Square error | $e_{RMS} = \sqrt{\frac{1}{N}\sum e_i^2}$ |
| Overshoot | % lewat setpoint | $OS = \frac{y_{max} - r}{r} \times 100\%$ |
| Rise time | Waktu mencapai setpoint pertama kali | Dari plot respons |
| Settling time | Waktu stabil dalam ±5% setpoint | Dari plot respons |

## 14.2 Error RMS sebagai Metrik Kualitas

$$e_{RMS} = \sqrt{\frac{1}{N} \sum_{k=1}^{N} e_k^2}$$

Nilai $e_{RMS}$ yang lebih kecil berarti robot lebih presisi mengikuti garis. Digunakan untuk membandingkan hasil tuning PID berbeda.

---

# BAB 15 — Topik Lanjutan

## 15.1 Adaptive PID

Parameter PID diubah secara dinamis berdasarkan kondisi:

```cpp
if (abs(error) > THRESHOLD_HIGH) {
  kp = kp_aggressive;
  kd = kd_aggressive;
} else {
  kp = kp_normal;
  kd = kd_normal;
}
```

## 15.2 Fuzzy Logic Control

Menggunakan aturan linguistik alih-alih rumus matematis murni:
- Input: error dan delta error
- Output: koreksi kecepatan

Contoh aturan:
- IF error = Large Positive AND delta_error = Positive THEN correction = Very Large Negative
- IF error = Zero AND delta_error = Zero THEN correction = Zero

## 15.3 Feedforward Control

Menggabungkan PID (feedback) dengan prediksi berbasis pengetahuan track:

$$u(t) = u_{PID}(t) + u_{FF}(t)$$

$$u_{FF}(t) = K_{FF} \cdot \text{curvature}(t)$$

## 15.4 Encoder-Based Speed Control

Dengan encoder pada motor, kecepatan dapat dikontrol secara presisi:

$$\omega_{measured} = \frac{N_{pulses}}{\Delta t \cdot PPR} \times 2\pi$$

$$e_{speed} = \omega_{target} - \omega_{measured}$$

$$v_{PWM} = K_p \cdot e_{speed} + \text{feedforward}$$

---

# BAB 16 — Troubleshooting dan Pemecahan Masalah

## 16.1 Hardware: OLED Tidak Muncul / Blank

**Gejala**: LCD OLED tidak menampilkan apa-apa atau hanya putih/hitam.

Checklist:
- Cek wiring: SDA → GPIO 33, SCL → GPIO 35, VCC → **3.3 V** (bukan 5 V), GND → GND.
- Cek alamat I2C: biasanya `0x3C` atau `0x3D`. Test dengan I2C Scanner:

```cpp
#include <Wire.h>
void setup() {
  Serial.begin(115200);
  Wire.begin(33, 35);
}
void loop() {
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found 0x"); Serial.println(addr, HEX);
    }
  }
  delay(5000);
}
```

- Turunkan clock I2C jika perlu: `Wire.setClock(100000);`
- Jika alamat berubah: pastikan pull-up resistor 4.7kΩ di SDA dan SCL.

## 16.2 Hardware: Motor Tidak Bergerak

Checklist:
- Cek supply baterai motor (min 7.4V dengan multimeter).
- Cek wiring EN, IN1, IN2 sesuai `config.h`.
- Cek GND bersama: GND ESP32, GND driver motor, GND baterai harus terhubung.
- Cek LED indikator driver motor.
- Test motor bypass driver: hubungkan motor langsung ke baterai sebentar.

Solusi kode — test motor individual:

```cpp
ledcWrite(PWM_CHANNEL_L, 150);
digitalWrite(MOTOR_LEFT_IN1, HIGH);
digitalWrite(MOTOR_LEFT_IN2, LOW);
delay(2000);
ledcWrite(PWM_CHANNEL_L, 0);
```

## 16.3 Hardware: Sensor Tidak Stabil / Nilai Acak

Checklist:
- Kalibrasi ulang sensor (Program 04).
- Jarak sensor ke lantai: optimal **5–10 mm**.
- Hindari cahaya matahari langsung pada sensor.
- Tambahkan moving average filter:

```cpp
#define FILTER_SIZE 5
uint16_t hist[8][FILTER_SIZE];
int histIdx = 0;
void readSensorsFiltered(uint16_t* out) {
  for (int i = 0; i < 8; i++)
    hist[i][histIdx] = analogRead(sensorPins[i]);
  histIdx = (histIdx + 1) % FILTER_SIZE;
  for (int i = 0; i < 8; i++) {
    uint32_t s = 0;
    for (int j = 0; j < FILTER_SIZE; j++) s += hist[i][j];
    out[i] = s / FILTER_SIZE;
  }
}
```

## 16.4 Hardware: Push Button Tidak Responsif

Pastikan wiring: satu kaki tombol ke GPIO, kaki lain ke GND. Gunakan `INPUT_PULLUP`.

```cpp
bool readButton(int pin) {
  static unsigned long t = 0;
  static bool last = HIGH;
  bool cur = digitalRead(pin);
  if (cur == LOW && last == HIGH && millis() - t > 50) {
    t = millis(); last = cur; return true;
  }
  last = cur; return false;
}
```

## 16.5 Software: Upload Gagal

Error: "Failed to connect", "Timed out", "Serial port not found".

Langkah perbaikan:
1. Gunakan kabel USB **data** (bukan charge only).
2. Pilih port yang benar di PlatformIO.
3. Tutup Serial Monitor sebelum upload.
4. Tekan dan tahan BOOT → klik Upload → lepas BOOT setelah "Connecting...".
5. Linux: `sudo usermod -a -G dialout $USER` lalu logout-login ulang.
6. Cek `platformio.ini`: board harus `lolin_s2_mini`.
7. Manual boot mode: tahan BOOT → tekan RESET → lepas RESET → lepas BOOT → upload.

## 16.6 Software: Library Not Found

Error: `fatal error: Adafruit_SSD1306.h: No such file or directory`

```bash
pio lib install "Adafruit SSD1306"
pio lib install "Adafruit GFX Library"
pio run -t clean && pio run
```

Atau di `platformio.ini`:

```ini
lib_deps =
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit GFX Library@^1.11.3
```

## 16.7 Performa: Robot Keluar Lintasan di Tikungan

| Penyebab | Solusi |
|---|---|
| Base speed terlalu tinggi | Turunkan ke 120 |
| Kd terlalu kecil | Naikkan Kd +0.5 |
| Threshold sensor tidak tepat | Kalibrasi ulang (Program 04) |
| Sensor terlalu jauh dari lantai | Adjust ke 5–10 mm |
| Center of gravity tidak balance | Pindahkan baterai ke tengah |

## 16.8 Performa: Robot Zig-Zag Terus Menerus

| Penyebab | Solusi |
|---|---|
| Kp terlalu besar | Turunkan Kp -0.01 |
| Kd terlalu kecil | Naikkan Kd +0.5 |
| Sensor noise | Tambah smoothing filter |
| Sensor tidak terkalibrasi | Kalibrasi ulang |

Tambahkan smoothing sederhana:

```cpp
float smoothedError = 0.7f * error + 0.3f * lastError;
```

## 16.9 Performa: Robot Selalu Offset ke Satu Sisi

| Penyebab | Solusi |
|---|---|
| Sensor tidak center | Adjust posisi mounting sensor |
| Motor tidak balance | Tambahkan offset kalibrasi |
| Threshold tidak tepat | Kalibrasi ulang sensor |
| Perlu integral | Tambahkan Ki = 0.001 |

```cpp
#define MOTOR_RIGHT_OFFSET 0.95f  // Koreksi jika kanan lebih kuat
void setMotors(int l, int r) {
  r = (int)(r * MOTOR_RIGHT_OFFSET);
  // ... apply to PWM
}
```

## 16.10 Performa: Robot Berhenti / Stuck (Lost Line)

```cpp
bool allWhite() {
  for (int i = 0; i < 8; i++)
    if (sensorDigital[i]) return false;
  return true;
}
if (allWhite()) {
  lostCtr++;
  if (lostCtr > 10) {
    setMotors(lastError < 0 ? 50 : 150,
              lastError < 0 ? 150 : 50);
  }
} else { lostCtr = 0; }
```

Pastikan speed tidak negatif:

```cpp
leftSpeed  = constrain(leftSpeed,  0, 255);
rightSpeed = constrain(rightSpeed, 0, 255);
```

## 16.11 Power: Baterai Cepat Habis / Robot Restart Sendiri

- Gunakan baterai minimal 1000 mAh (lebih baik 2000 mAh).
- Tambahkan kapasitor **1000 µF** di VCC driver motor untuk reduksi voltage drop.
- Pisahkan supply ESP32 dan motor untuk menghindari brownout.
- Soft start untuk kurangi inrush current:

```cpp
void softStart(int targetL, int targetR) {
  for (int s = 0; s <= max(targetL, targetR); s += 10) {
    setMotors(min(s, targetL), min(s, targetR));
    delay(30);
  }
}
```

## 16.12 Quick Reference: Checklist Sebelum Run

- [ ] Baterai ≥ 7.4 V.
- [ ] Semua wiring dicek.
- [ ] Sensor dikalibrasi (Program 04).
- [ ] Motor test OK (Program 03).
- [ ] OLED dan button berfungsi.
- [ ] Lintasan test siap.
- [ ] Kabel tidak menghalangi roda.
- [ ] Tabel tuning sudah disiapkan.

## 16.13 Emergency Procedures

1. **Robot tidak terkontrol**: tekan emergency stop, angkat robot, atau cabut baterai.
2. **Smoke atau bau terbakar**: matikan power segera, cabut baterai, cek short circuit.
3. **Upload error berulang**: restart ESP32, restart laptop, ganti kabel USB, ganti port USB.

---

# BAB 17 — Rekomendasi Buku dan Referensi

## 17.1 Buku Teks Wajib

| No | Judul | Pengarang | Penerbit | Relevansi |
|---|---|---|---|---|
| 1 | *Modern Control Engineering* (5th ed.) | Ogata, K. | Prentice Hall | Teori kontrol PID, sistem dinamik |
| 2 | *Control Systems Engineering* (7th ed.) | Nise, N. S. | Wiley | Analisis sistem loop tertutup |
| 3 | *Programming Robots with ROS* | Quigley, M. et al. | O'Reilly | Arsitektur robot mobile |
| 4 | *Mobile Robots: Inspiration to Implementation* | Flynn, A. & Jones, J. | A K Peters | Robot mobile dari dasar |
| 5 | *Introduction to Embedded Systems* (2nd ed.) | Lee, E. A. & Seshia, S. A. | MIT Press | Pemrograman embedded dan kontrol |

## 17.2 Buku Elektronika dan Mikrokontroler

| No | Judul | Pengarang | Relevansi |
|---|---|---|---|
| 1 | *The Art of Electronics* (3rd ed.) | Horowitz & Hill | Dasar elektronika analog/digital |
| 2 | *Make: Electronics* | Platt, C. | Elektronika praktis untuk pemula |
| 3 | *ESP32 for IoT Applications* | Manoj R. Thakur | Pemrograman ESP32 mendalam |
| 4 | *Programming Arduino* | Monk, S. | Dasar pemrograman Arduino/ESP32 |
| 5 | *Getting Started with Arduino* (3rd ed.) | Banzi, M. & Shiloh, M. | Arduino ecosystem |

## 17.3 Paper Ilmiah Terpilih

| No | Judul Paper | Penulis | Jurnal/Konferensi | Tahun |
|---|---|---|---|---|
| 1 | A Line Follower Robot with PID Controller for Different Curvatures of Path | Priyanka, E. B. & Thangavel, S. | J. Electrical Engineering | 2021 |
| 2 | Optimization of PID Parameters for Line Follower Robot Using Genetic Algorithm | Hamamci, S. E. | IFAC Proceedings | 2020 |
| 3 | Fuzzy Logic Based Line Following Robot | Abiyev, R. H. & Abizade, S. | Procedia Computer Science | 2014 |
| 4 | Autonomous Line Following Robot Using PID Controller | Susnea, I. & Minzu, V. | IFAC Papersonline | 2020 |
| 5 | Design and Implementation of a Fast Line Follower Robot | de Moraes, J. S. & Ferreira, J. V. | IEEE ICIT | 2019 |
| 6 | Real-Time Line Following Robot with Adaptive Speed Control | Wahyunggoro, O. et al. | J. Robotics | 2022 |
| 7 | Performance Analysis of PID, Fuzzy and Neuro-Fuzzy Controllers for Line Tracking | Mahi, H. & Hirzel, A. | International J. Computer Science | 2015 |
| 8 | Path Planning and Motion Control for Autonomous Mobile Robots | LaValle, S. M. | Cambridge University Press | 2006 |

## 17.4 Sumber Daring (Online Resources)

| No | Sumber | URL | Konten |
|---|---|---|---|
| 1 | PlatformIO Documentation | platformio.org/docs | IDE dan build system |
| 2 | ESP32 Technical Reference Manual | espressif.com | Datasheet ESP32 S2 |
| 3 | Adafruit SSD1306 Library | github.com/adafruit/Adafruit_SSD1306 | Library OLED |
| 4 | Arduino PID Library (Brett Beauregard) | github.com/br3ttb/Arduino-PID-Library | Referensi PID |
| 5 | Pololu QTR Sensor Library | github.com/pololu/qtr-sensors-arduino | Sensor array |
| 6 | Control Tutorials for MATLAB (Univ. Michigan) | ctms.engin.umich.edu | Tutorial kontrol interaktif |
| 7 | Hackaday Line Follower Projects | hackaday.io/project | Contoh implementasi nyata |

---

*Materi ini disusun untuk Modul 10 Praktikum Mekatronika, Program Studi Sarjana Terapan Teknologi Rekayasa Otomasi. Dosen pengampu: Rofiq Cahyo Prayogo, S.T., M.T.*
