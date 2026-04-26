## Pertanyaan Praktikum SerialMonitor

1) Jelaskan proses dari input keyboard hingga LED menyala/mati! 
2) Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika 
baris tersebut dihilangkan? 
3) Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi 
jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan 
berikan penjelasan disetiap baris kode nya dalam bentuk README.md! 
4) Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap 
sistem 
---

## Jawaban

### 1. Proses dari Input Keyboard hingga LED Menyala/Mati

Pengguna mengetik karakter di Serial Monitor, data dikirim ke Arduino lewat USB (UART) pada baudrate 9600. Karakter masuk ke buffer serial, lalu dibaca oleh `Serial.read()`. Program mengecek isi karakter: bila 1 maka LED menyala (HIGH), bila 0 maka LED mati (LOW). Arduino kemudian mengirim balik pesan status ke Serial Monitor sebagai konfirmasi.

---

### 2. Mengapa Menggunakan `Serial.available()` Sebelum Membaca Data?

`Serial.available()` dipakai untuk memastikan ada data di buffer sebelum dibaca. Jika baris ini dihilangkan, `Serial.read()` tetap berjalan walaupun buffer kosong dan mengembalikan nilai `-1`. Akibatnya program akan terus memicu pesan error meskipun pengguna tidak mengetik apa-apa.

---

### 3. Modifikasi: LED Berkedip saat Menerima Input 2

```cpp
#include <Arduino.h>

const int PIN_LED = 12;

// Mode LED saat ini: 0 = mati, 1 = nyala, 2 = blink
char mode = '0';

// Variabel untuk blink non-blocking
unsigned long lastToggle = 0;
const unsigned long intervalBlink = 500; // ms
bool ledState = LOW;

void setup() {
  Serial.begin(9600);                                // Inisialisasi komunikasi serial 9600 bps
  Serial.println("Ketik 1 ON, 0 OFF, 2 BLINK"); // Petunjuk perintah
  pinMode(PIN_LED, OUTPUT);                          // Set pin LED sebagai output
}

void loop() {
  // ===== Cek input dari Serial =====
  if (Serial.available() > 0) {                      // Jika ada data di buffer serial
    char data = Serial.read();                       // Baca 1 karakter

    if (data == '1') {                               // Perintah 1 → LED ON
      mode = '1';
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED ON");
    }
    else if (data == '0') {                          // Perintah 0 → LED OFF
      mode = '0';
      digitalWrite(PIN_LED, LOW);
      Serial.println("LED OFF");
    }
    else if (data == '2') {                          // Perintah 2 → mode BLINK aktif
      mode = '2';
      lastToggle = millis();                         // Reset waktu acuan blink
      Serial.println("LED BLINK");
    }
    else if (data != '\n' && data != '\r') {         // Abaikan ENTER, selain itu invalid
      Serial.println("Perintah tidak dikenal");
    }
  }

  // ===== Jalankan blink jika mode == '2' =====
  if (mode == '2') {                                 // Hanya berkedip saat mode blink
    unsigned long now = millis();                    // Waktu sekarang (ms)
    if (now - lastToggle >= intervalBlink) {         // Cek apakah sudah lewat interval
      lastToggle = now;                              // Update waktu acuan
      ledState = !ledState;                          // Toggle state LED
      digitalWrite(PIN_LED, ledState);               // Terapkan ke pin LED
    }
  }
}
```

---

### 4. Menggunakan `delay()` atau `millis()`?

Dipilih `millis()` karena bersifat non-blocking. Dengan `delay()`, selama LED berkedip Arduino berhenti dan tidak bisa membaca input serial baru, sehingga perintah berikutnya tertunda. `millis()` membuat loop tetap berjalan, sehingga Arduino bisa berkedip sambil tetap responsif terhadap perintah dari Serial Monitor.