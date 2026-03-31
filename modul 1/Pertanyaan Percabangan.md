## Pertanyaan Praktikum Percabangan 1.5.4

1. Pada kondisi apa program masuk ke blok if?
2. Pada kondisi apa program masuk ke blok else?
3. Apa fungsi dari perintah delay(timeDelay)?
4. Jika program yang dibuat memiliki alur mati → lambat → cepat → reset (mati),
ubah menjadi LED tidak langsung reset → tetapi berubah dari cepat → sedang →
mati dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

---

## Jawaban

### 1. Kondisi Program Masuk ke Blok `if`

Berikut adalah bagian kode yang mengandung blok `if`:

```cpp
if (timeDelay <= 100) {
  delay(3000);
  timeDelay = 1000;   // reset ke awal
}
```

Program akan masuk ke blok `if` ketika nilai variabel **`timeDelay` kurang dari atau sama dengan 100** (`timeDelay <= 100`).



---

### 2. Kondisi Program Masuk ke Blok `else`

Berikut adalah bagian kode yang mengandung blok `else`:

```cpp
else {
  timeDelay -= 100;   // percepatan bertahap
}
```
Program akan masuk ke blok `else` ketika kondisi pada `if` **tidak terpenuhi**, yaitu ketika nilai **`timeDelay` lebih besar dari 100** (`timeDelay > 100`).

---

### 3. Fungsi Perintah `delay(timeDelay)`

Berikut adalah bagian kode yang menggunakan `delay(timeDelay)`:

```cpp
void loop() {
  // Nyalakan LED
  digitalWrite(ledPin, HIGH);
  delay(timeDelay);
  // Matikan LED
  digitalWrite(ledPin, LOW);
  delay(timeDelay);
  ...
}
```

#### Penjelasan Detail:

Fungsi **`delay(timeDelay)`** digunakan untuk **menghentikan sementara (menunda) eksekusi program** selama **`timeDelay` milidetik**.



Karena nilai `timeDelay` **berubah-ubah** (berkurang 100ms setiap siklus), maka durasi kedipan LED menjadi **semakin cepat** seiring waktu.

---

### 4. Modifikasi Program: Cepat → Sedang → Mati

#### Kode Program Asli (Alur: Mati → Lambat → Cepat → Reset):

```cpp
const int ledPin = 11;
int timeDelay = 1000;
void setup() {
  pinMode(ledPin, OUTPUT);
}
void loop() {
  digitalWrite(ledPin, HIGH);
  delay(timeDelay);
  digitalWrite(ledPin, LOW);
  delay(timeDelay);
  if (timeDelay <= 100) {
    delay(3000);
    timeDelay = 1000;   // reset ke awal
  } else {
    timeDelay -= 100;
  }
}
```

Pada program asli, ketika `timeDelay` mencapai 100ms (kedip tercepat), LED langsung **reset** kembali ke 1000ms. Ini menghasilkan transisi yang mendadak dari cepat ke lambat.

#### Kode Program Modifikasi:

```cpp
// Program LED Percabangan Modifikasi
// Alur: mati → lambat → cepat → sedang → mati

const int ledPin = 11;      // Pin LED terhubung di pin 11
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
```

#### Penjelasan Baris per Baris:

##### Bagian Deklarasi Variabel:
```cpp
const int ledPin = 11;
```
- Mendeklarasikan konstanta **`ledPin`** dengan nilai **11**, pin digital tempat LED terhubung.

```cpp
int timeDelay = 1000;
```
- Mendeklarasikan variabel **`timeDelay`** bertipe integer dengan nilai awal **1000** milidetik  digunakan sebagai durasi delay kedipan LED.

```cpp
bool speedingUp = true;
```
- Mendeklarasikan variabel **`speedingUp`** bertipe boolean dengan nilai awal **true**. Variabel ini berfungsi sebagai **flag (penanda)** untuk menentukan arah perubahan kecepatan:
  - `true` = LED sedang dalam fase **percepatan** (delay semakin kecil).
  - `false` = LED sedang dalam fase **perlambatan** (delay semakin besar).

##### Fungsi `setup()`:
```cpp
void setup() {
  pinMode(ledPin, OUTPUT);
}
```
- **`void setup()`** → Fungsi yang dijalankan **satu kali** saat Arduino pertama kali dinyalakan atau di-reset.
- **`pinMode(ledPin, OUTPUT)`** → Mengatur pin 11 sebagai **output**, sehingga Arduino dapat mengirimkan sinyal HIGH/LOW untuk menyalakan/mematikan LED.

##### Fungsi `loop()`  Siklus Kedip:
```cpp
digitalWrite(ledPin, HIGH);
delay(timeDelay);
digitalWrite(ledPin, LOW);
delay(timeDelay);
```
- **`digitalWrite(ledPin, HIGH)`** → Menyalakan LED dengan memberikan tegangan **HIGH** ke pin 11.
- **`delay(timeDelay)`** → Menahan LED tetap menyala selama `timeDelay` milidetik.
- **`digitalWrite(ledPin, LOW)`** → Mematikan LED dengan memberikan tegangan **LOW** ke pin 11.
- **`delay(timeDelay)`** → Menahan LED tetap mati selama `timeDelay` milidetik.

##### Fungsi `loop()`  Logika Percabangan:

**Percabangan utama**  menentukan fase saat ini:
```cpp
if (speedingUp == true) {
```
- Memeriksa apakah program sedang dalam **fase percepatan**. Jika `speedingUp` bernilai `true`, masuk ke blok ini.

**Fase Percepatan (speedingUp = true):**
```cpp
if (timeDelay <= 100) {
  speedingUp = false;
} else {
  timeDelay -= 100;
}
```
- **`if (timeDelay <= 100)`** → Jika delay sudah mencapai **100ms** (kedip tercepat):
  - **`speedingUp = false`** → Mengubah flag menjadi `false`, sehingga siklus berikutnya masuk ke **fase perlambatan**.
- **`else`** → Jika delay masih lebih dari 100ms:
  - **`timeDelay -= 100`** → Mengurangi delay sebesar 100ms, membuat LED berkedip **lebih cepat**.
  - Urutan: 1000 → 900 → 800 → 700 → 600 → 500 → 400 → 300 → 200 → 100.

**Fase Perlambatan (speedingUp = false):**
```cpp
if (timeDelay >= 500) {
  digitalWrite(ledPin, LOW);
  delay(3000);
  timeDelay = 1000;
  speedingUp = true;
} else {
  timeDelay += 100;
}
```
- **`if (timeDelay >= 500)`** → Jika delay sudah mencapai **500ms** (kecepatan sedang):
  - **`digitalWrite(ledPin, LOW)`** → Memastikan LED **mati sepenuhnya**.
  - **`delay(3000)`** → Memberikan jeda **3 detik** dalam keadaan mati sebagai penanda siklus selesai.
  - **`timeDelay = 1000`** → Mereset delay ke **1000ms** (kembali ke awal).
  - **`speedingUp = true`** → Mengatur flag kembali ke fase percepatan untuk siklus berikutnya.
- **`else`** → Jika delay masih kurang dari 500ms:
  - **`timeDelay += 100`** → Menambah delay sebesar 100ms, membuat LED berkedip **lebih lambat**.
  - Urutan: 100 → 200 → 300 → 400 → 500.

##### Alur Keseluruhan Program:

| Fase | Urutan `timeDelay` (ms) | Keterangan |
|------|-------------------------|------------|
| Percepatan | 1000 → 900 → 800 → 700 → 600 → 500 → 400 → 300 → 200 → 100 | LED berkedip semakin cepat |
| Perlambatan | 100 → 200 → 300 → 400 → 500 | LED berkedip semakin lambat (sedang) |
| Mati | LED mati, jeda 3 detik | Penanda akhir siklus |
| Reset | `timeDelay` kembali ke 1000 | Siklus dimulai kembali |

Program akan terus **berulang tanpa henti** (karena berada di dalam fungsi `loop()`), sehingga menghasilkan efek LED yang berkedip dari **lambat → cepat → melambat ke sedang → mati → dan mengulang dari awal** secara terus-menerus.