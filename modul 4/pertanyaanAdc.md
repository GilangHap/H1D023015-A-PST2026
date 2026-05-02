## Pertanyaan Praktikum Analog to Digital Converter (ADC)

1) Apa fungsi perintah analogRead() pada rangkaian praktikum ini?
2) Mengapa diperlukan fungsi map() dalam program tersebut?
3) Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file README.md
---

## Jawaban

### 1. Fungsi analogRead()

analogRead() berfungsi membaca tegangan analog dari pin potensiometer dan mengubahnya menjadi nilai digital 10-bit (0–1023). Pada rangkaian ini, potensiometer memberikan tegangan variabel 0–5 V yang dibaca oleh pin A0.

---

### 2. Fungsi map()

Fungsi map() diperlukan untuk menykalakan (scaling) nilai dari satu rentang ke rentang lain. ADC Arduino menghasilkan nilai 10-bit (0–1023), sedangkan servo membutuhkan sudut 0–180° dan PWM membutuhkan nilai 8-bit (0–255). Tanpa map(), nilai ADC tidak bisa langsung digunakan untuk mengontrol servo atau kecerahan LED.

---

### 3. Modifikasi Servo Rentang 30°–150°

```cpp
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

const int potensioPin = A0;   // Pin analog A0 untuk potensiometer
const int servoPin = 9;       // Pin digital 9 (PWM) untuk servo

int pos = 0; // Variabel menyimpan hasil pemetaan sudut
int val = 0; // Variabel menyimpan nilai mentah ADC

void setup() {
  myservo.attach(servoPin);   // Hubungkan servo ke pin 9
  Serial.begin(9600);         // Mulai komunikasi serial 9600 bps
}

void loop() {
  val = analogRead(potensioPin);  // Baca nilai ADC dari potensiometer (0–1023)

  // Modifikasi: ubah rentang output servo dari 0–180 menjadi 30–150 derajat
  // ADC 0 tetap dipetakan ke 30 (posisi minimum baru)
  // ADC 1023 tetap dipetakan ke 150 (posisi maksimum baru)
  pos = map(val, 0, 1023, 30, 150);

  myservo.write(pos);  // Gerakkan servo sesuai hasil mapping

  // Tampilkan data ke Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(val);
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  delay(15);  // Delay agar servo punya waktu mencapai posisi
}
```