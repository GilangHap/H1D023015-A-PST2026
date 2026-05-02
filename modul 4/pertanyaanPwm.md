## Pertanyaan Praktikum Pulse Width Modulation (PWM)

1) Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!
2) Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
3) Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file README.md.
---

## Jawaban

### 1. Mengapa LED Dapat Diatur Kecerahannya dengan analogWrite()

analogWrite() menghasilkan sinyal **PWM (Pulse Width Modulation)** pada pin digital yang mendukung PWM (ditandai simbol `~`, misalnya pin 9). PWM mengatur kecerahan LED dengan mengubah duty cycle (perbandingan waktu sinyal HIGH terhadap satu periode sinyal). Semakin besar nilai yang diberikan ke analogWrite() (0–255), semakin lama LED dalam kondisi ON, sehingga manusia melihatnya sebagai peningkatan kecerahan rata-rata.


---

### 2. Hubungan Nilai ADC (0–1023) dan PWM (0–255)

ADC Arduino memiliki resolusi **10-bit** (2^10 = 1024 nilai, 0–1023), sedangkan PWM memiliki resolusi **8-bit** (2^8 = 256 nilai, 0–255). Agar nilai pembacaan sensor dapat langsung digunakan untuk mengontrol kecerahan LED, perlu dilakukan penyekalaan (scaling) dari rentang 10-bit ke rentang 8-bit menggunakan fungsi map().

---

### 3. Modifikasi LED Hanya Menyala pada Rentang PWM 50–200

```cpp
#include <Arduino.h> // library dasar Arduino

const int potPin = A0;   // Pin analog A0 untuk potensiometer
const int ledPin = 9;    // Pin digital 9 (mendukung PWM)

int nilaiADC = 0;  // Variabel menampung input 0-1023
int pwm = 0;       // Variabel menampung output 0-255

void setup() {
  pinMode(ledPin, OUTPUT);  // Atur pin LED sebagai output
  Serial.begin(9600);       // Mulai komunikasi serial 9600 bps
}

void loop() {
  nilaiADC = analogRead(potPin);  // Baca nilai analog dari potensiometer (0–1023)

  // Konversi nilai ADC ke rentang PWM 0–255
  pwm = map(nilaiADC, 0, 1023, 0, 255);

  // Modifikasi: LED hanya menyala jika nilai PWM berada dalam rentang 50–200
  if (pwm >= 50 && pwm <= 200) {
    analogWrite(ledPin, pwm);  // Nyalakan LED dengan nilai PWM saat ini
  } else {
    analogWrite(ledPin, 0);   // Matikan LED jika di luar rentang 50–200
  }

  // Tampilkan data ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | PWM: ");
  Serial.println(pwm);

  delay(50);  // Delay agar Serial Monitor tidak bergerak terlalu cepat
}
```