## Pertanyaan Praktikum Push Button 2.6.4

1. Gambarkan rangkaian schematic yang digunakan pada percobaan! 
2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno? 
Apa keuntungannya dibandingkan rangkaian biasa? 
3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari 
sisi hardware maupun software? 
4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai 
penambahan (increment) dan pengurangan (decrement) pada sistem counter dan 
berikan penjelasan disetiap baris kode nya dalam bentuk README.md! 
---

## Jawaban

### 1. Skematik Rangkaian

Berikut adalah gambar rangkaian schematic yang digunakan pada percobaan:

![Rangkaian Push Button](dokumentasi/pushbutton.jpg)

---

### 2. Mengapa Menggunakan `INPUT_PULLUP`?

Agar pin tidak *floating* yang membuat pembacaan menjadi acak. `INPUT_PULLUP` mengaktifkan resistor internal Arduino, sehingga pin stabil di `HIGH` saat tidak ditekan.
**Keuntungannya:** Rangkaian lebih simpel dan menghemat tempat serta biaya karena tidak butuh tambahan resistor eksternal.

---

### 3. Jika Salah Satu LED Segmen Tidak Menyala

- **Sisi Hardware:** Kabel putus/longgar, resistor rusak, LED mati, atau pin COM tidak terhubung dengan benar.
- **Sisi Software:** Salah pengaturan pin di program, kesalahan penulisan nilai pattern array, atau pin belum diatur sebagai `OUTPUT`.

---

### 4. Modifikasi: Dua Push Button (Increment & Decrement)

```cpp
#include <Arduino.h>

// =================== PIN ===================
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};
// a  b  c   d   e  f  g  dp

const int btnUp   = 3; // Tombol increment
const int btnDown = 2; // Tombol decrement

// ================= DATA ====================
// CA: 0 = ON (pin LOW), 1 = OFF
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, //0
  {0,1,1,0,0,0,0,0}, //1
  {1,1,0,1,1,0,1,0}, //2
  {1,1,1,1,0,0,1,0}, //3
  {0,1,1,0,0,1,1,0}, //4
  {1,0,1,1,0,1,1,0}, //5
  {1,0,1,1,1,1,1,0}, //6
  {1,1,1,0,0,0,0,0}, //7
  {1,1,1,1,1,1,1,0}, //8
  {1,1,1,1,0,1,1,0}, //9
  {1,1,1,0,1,1,1,0}, //A
  {0,0,1,1,1,1,1,0}, //b
  {1,0,0,1,1,1,0,0}, //C
  {0,1,1,1,1,0,1,0}, //d
  {1,0,0,1,1,1,1,0}, //E
  {1,0,0,0,1,1,1,0}  //F
};

int currentDigit = 0;

bool lastUpState   = HIGH;
bool lastDownState = HIGH;

// ============= FUNCTION ====================
void displayDigit(int num) {
  for(int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

// ================= SETUP ===================
void setup() {
  for(int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(btnUp,   INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  displayDigit(currentDigit);
}

// ================= LOOP ====================
void loop() {
  bool upState   = digitalRead(btnUp);
  bool downState = digitalRead(btnDown);

  // Increment: tekan btnUp
  if(lastUpState == HIGH && upState == LOW) {
    currentDigit++;
    if(currentDigit > 15) currentDigit = 0; // Wrap-around ke 0
    displayDigit(currentDigit);
  }

  // Decrement: tekan btnDown
  if(lastDownState == HIGH && downState == LOW) {
    currentDigit--;
    if(currentDigit < 0) currentDigit = 15; // Wrap-around ke F
    displayDigit(currentDigit);
  }

  lastUpState   = upState;
  lastDownState = downState;
}
```

**Perubahan dari kode awal:**
- Tambah `btnDown = 2` sebagai pin tombol decrement
- `pinMode(btnDown, INPUT_PULLUP)` di `setup()`
- Blok `if` baru untuk deteksi tepi turun (falling edge) pada `btnDown` → kurangi `currentDigit`
- Wrap-around dua arah: naik melewati 15 → kembali ke 0, turun melewati 0 → kembali ke 15