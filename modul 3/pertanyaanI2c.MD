## Pertanyaan Praktikum I2C

1) Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
2) Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
3) Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output) sehingga:
   - Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor.
   - Adapun data yang ditampilkan pada Serial Monitor sesuai dengan tabel berikut:

     | ADC: 0 | Volt: 0.00 V | Persen: 0% |
     | ------ | ------------ | ---------- |

     Tampilan jika potensiometer dalam kondisi diputar paling kiri.
   - Tampilan LCD: `ADC: 0  0%` pada `setCursor(0, 0)` dan Bar (level) pada `setCursor(0, 1)`.
   - Berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4) Lengkapi tabel berikut berdasarkan pengamatan pada Serial Monitor:

   | ADC | Volt (V) | Persen (%) |
   | --- | -------- | ---------- |
   | 1   |          |            |
   | 21  |          |            |
   | 49  |          |            |
   | 74  |          |            |
   | 96  |          |            |

---

## Jawaban

### 1. Cara Kerja Komunikasi I2C antara Arduino dan LCD

I2C menggunakan dua jalur, yaitu SDA (data) dan SCL (clock). Arduino berperan sebagai master dan modul LCD I2C sebagai slave dengan alamat 0x27. Master mengirim alamat slave dan data karakter melalui SDA, sinkron dengan clock di SCL. Modul I2C LCD memiliki chip PCF8574 yang mengubah data serial menjadi paralel untuk pin LCD, sehingga Arduino cukup memakai 2 pin (A4 = SDA, A5 = SCL).

---

### 2. Pin Potensiometer

Potensiometer punya 3 pin: kiri ke VCC, tengah (wiper) ke A0, dan kanan ke GND. Jika pin kiri dan kanan tertukar, arah pembacaan menjadi terbalik. Saat diputar paling kiri yang seharusnya bernilai 0 justru menghasilkan 1023, begitu pula sebaliknya. Rangkaian tetap aman karena hanya membalik referensi tegangan.

---

### 3. Modifikasi: Gabungan UART + I2C

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Inisialisasi LCD I2C alamat 0x27
const int pinPot = A0;                // Pin analog potensiometer

void setup() {
  Serial.begin(9600);                 // Mulai komunikasi serial 9600 bps
  lcd.init();                         // Inisialisasi LCD
  lcd.backlight();                    // Nyalakan backlight LCD
}

void loop() {
  int nilai = analogRead(pinPot);                 // Baca ADC (0-1023)
  float volt = nilai * (5.0 / 1023.0);            // Konversi ke tegangan (0-5 V)
  int persen = map(nilai, 0, 1023, 0, 100);       // Konversi ke persen (0-100)
  int panjangBar = map(nilai, 0, 1023, 0, 16);    // Panjang bar untuk LCD (0-16)

  // ===== Output ke Serial Monitor =====
  Serial.print("ADC: ");    Serial.print(nilai);
  Serial.print(" | Volt: "); Serial.print(volt, 2); Serial.print(" V");
  Serial.print(" | Persen: "); Serial.print(persen); Serial.println("%");

  // ===== Output ke LCD baris 1: ADC dan persen =====
  lcd.setCursor(0, 0);
  lcd.print("ADC:");
  lcd.print(nilai);
  lcd.print("  ");
  lcd.print(persen);
  lcd.print("%   ");                  // Spasi untuk hapus sisa karakter

  // ===== Output ke LCD baris 2: bar level =====
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) lcd.print((char)255); // Karakter blok penuh
    else                lcd.print(" ");
  }

  delay(200);
}
```

---

### 4. Tabel Hasil Pengamatan

| ADC | Volt (V) | Persen (%) |
| --- | -------- | ---------- |
| 1   | 0.00     | 0          |
| 21  | 0.10     | 2          |
| 49  | 0.24     | 5          |
| 74  | 0.36     | 7          |
| 96  | 0.47     | 9          |