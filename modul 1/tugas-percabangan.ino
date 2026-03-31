// Program LED Percabangan Modifikasi
// Alur: mati → lambat → cepat → sedang → mati

const int ledPin = 11;      // Pin LED terhubung di pin digital 11
int timeDelay = 1000;        // Delay awal 1000ms (kedip lambat)
bool speedingUp = true;      // Flag arah perubahan: true = percepat, false = perlambat

void setup() {               // Fungsi setup dijalankan sekali saat Arduino menyala
  pinMode(ledPin, OUTPUT);   // Set pin 11 sebagai OUTPUT untuk mengontrol LED
}

void loop() {                        // Fungsi loop dijalankan berulang terus-menerus

  // --- Siklus Kedip LED ---
  digitalWrite(ledPin, HIGH);        // Nyalakan LED (beri tegangan HIGH)
  delay(timeDelay);                  // Tahan LED menyala selama timeDelay milidetik
  digitalWrite(ledPin, LOW);         // Matikan LED (beri tegangan LOW)
  delay(timeDelay);                  // Tahan LED mati selama timeDelay milidetik

  // --- Logika Percabangan Perubahan Kecepatan ---
  if (speedingUp == true) {                // Jika sedang dalam fase percepatan
    if (timeDelay <= 100) {                // Jika sudah mencapai kecepatan maksimum (100ms)
      speedingUp = false;                  // Ubah arah menjadi perlambatan
    } else {                               // Jika belum mencapai kecepatan maksimum
      timeDelay -= 100;                    // Kurangi delay 100ms (percepat kedipan)
    }
  } else {                                 // Jika sedang dalam fase perlambatan
    if (timeDelay >= 500) {                // Jika delay sudah mencapai 500ms (kecepatan sedang)
      digitalWrite(ledPin, LOW);           // Matikan LED sepenuhnya
      delay(3000);                         // Jeda 3 detik dalam keadaan mati
      timeDelay = 1000;                    // Reset delay ke awal (1000ms = lambat)
      speedingUp = true;                   // Set kembali ke fase percepatan
    } else {                               // Jika belum mencapai kecepatan sedang
      timeDelay += 100;                    // Tambah delay 100ms (perlambat kedipan)
    }
  }
}
