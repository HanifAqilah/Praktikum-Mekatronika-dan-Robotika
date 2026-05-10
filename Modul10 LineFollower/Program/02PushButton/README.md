# Program 02: Push Button Test

## Tujuan

- Memahami cara membaca input digital dari push button
- Implementasi debouncing untuk menghindari false trigger
- Menggunakan INPUT_PULLUP untuk button
- Menampilkan status button di OLED dan Serial Monitor

## Hardware yang Digunakan

| Komponen | Pin ESP32 | Keterangan |
|----------|-----------|------------|
| Button 1 | GPIO 0 | Button kiri atas |
| Button 2 | GPIO 12 | Button kanan atas |
| Button 3 | GPIO 13 | Button kiri bawah |
| Button 4 | GPIO 15 | Button kanan bawah |
| OLED SDA | GPIO 33 | Display |
| OLED SCL | GPIO 35 | Display |

## Wiring Diagram

```
Button: Satu pin ke GPIO, pin lainnya ke GND
Tidak perlu resistor pull-up eksternal (menggunakan internal)
```

## Cara Menjalankan

```bash
cd 02PushButton
pio run -t upload
pio device monitor -b 115200
```

## Output yang Diharapkan

### Serial Monitor
```
Button 1 pressed!
Button 2 pressed!
```

### OLED Display
```
Button Test
B1: [X]  B2: [ ]
B3: [ ]  B4: [X]
```

## Konsep Penting

### INPUT_PULLUP
- Button tidak ditekan: HIGH (pulled up ke 3.3V)
- Button ditekan: LOW (terhubung ke GND)
- Active LOW logic

### Debouncing
Button mekanis "bounce" saat ditekan (5-50ms). Solusi:
```cpp
if (millis() - lastPress > 50) { // Debounce 50ms
  // Process button
}
```

## Troubleshooting

### Button tidak terdeteksi
- Cek wiring (GPIO ke button, button ke GND)
- Pastikan pinMode(pin, INPUT_PULLUP)
- Test dengan Serial.println(digitalRead(pin))

### Button terdeteksi berkali-kali
- Tambahkan debounce delay (50-100ms)
- Gunakan state tracking (lastState)

## Modifikasi

### Counter Button Press
```cpp
int counter = 0;
if (button1Pressed) {
  counter++;
  lcd.print("Count: ");
  lcd.print(counter);
}
```

### Toggle LED
```cpp
static bool ledState = false;
if (button1Pressed) {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}
```

## Langkah Selanjutnya

Setelah berhasil:
- ✅ Button berfungsi dengan baik
- ✅ Debouncing bekerja
- ✅ Status tampil di OLED

**Lanjut ke**: Program 03 - Motor Test

---

**Dibuat oleh**: Tim Praktikum Mekatronika dan Robotika  
**Versi**: 1.0
