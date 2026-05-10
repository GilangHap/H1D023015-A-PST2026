## Pertanyaan Praktikum Komunikasi Task

1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya! 
2. Apakah program ini berpotensi mengalami race condition? Jelaskan! 
3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya 
sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md. 
---

## Jawaban

### 1. Mekanisme Eksekusi Dua Task

Kedua task pada program `komunikasiTask.ino` tidak benar-benar berjalan bersamaan, melainkan berjalan bergantian dengan sangat cepat. Arduino hanya mengeksekusi satu task pada satu saat, lalu FreeRTOS scheduler mengatur kapan task lain mendapat giliran berjalan.

Pada program ini, task read_data bertugas membaca atau menyiapkan data sensor, kemudian mengirimkannya ke queue menggunakan xQueueSend(). Task display menerima data dari queue dengan xQueueReceive() lalu menampilkannya ke Serial Monitor. Karena kedua task menggunakan mekanisme blocking seperti portMAX_DELAY, task yang sedang menunggu data atau menunggu ruang queue akan berhenti sementara, sehingga CPU dapat dipakai oleh task lainnya. Inilah yang membuat sistem tampak berjalan paralel, padahal sebenarnya bergantian.

---

### 2. Potensi Race Condition

Program ini pada dasarnya tidak berpotensi besar mengalami race condition pada data sensor, karena pertukaran data dilakukan melalui queue FreeRTOS. Queue bekerja sebagai mekanisme komunikasi yang aman antar task, sehingga data dikirim dan diterima secara teratur tanpa kedua task mengakses variabel struktur yang sama secara bersamaan.

Task read_data membuat variabel lokal struct readings x, mengisi nilainya, lalu mengirim salinannya ke queue. Task display juga memakai variabel lokalnya sendiri saat menerima data dari queue. Karena data tidak dibaca dan ditulis langsung pada variabel global yang sama, kondisi race dapat dihindari. Jadi, queue berfungsi sebagai media sinkronisasi sekaligus proteksi pertukaran data antar task.

---

### 3. Modifikasi Program dengan Sensor DHT Sesungguhnya

Agar data yang ditampilkan bersifat dinamis, nilai temp dan h dapat diambil langsung dari sensor DHT, misalnya DHT11 atau DHT22. Task pembaca sensor akan membaca suhu dan kelembapan aktual, lalu mengirimkannya ke queue. Setelah itu task display menampilkan hasil pembacaan terbaru ke Serial Monitor.

Contoh program modifikasi:

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

struct readings {
  int temp;
  int h;
};

QueueHandle_t my_queue;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  my_queue = xQueueCreate(1, sizeof(struct readings));

  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

void loop() {}

void read_data(void *pvParameters) {
  struct readings x;

  for (;;) {
    x.temp = (int)dht.readTemperature();
    x.h = (int)dht.readHumidity();

    xQueueSend(my_queue, &x, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void display(void *pvParameters) {
  struct readings x;

  for (;;) {
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("temp = ");
      Serial.println(x.temp);

      Serial.print("humidity = ");
      Serial.println(x.h);
    }
  }
}
```

Hasil modifikasi ini adalah nilai suhu dan kelembapan yang tampil pada Serial Monitor menjadi berubah-ubah sesuai kondisi lingkungan sekitar. Jika suhu ruangan naik atau kelembapan berubah, data yang diterima task display juga ikut berubah. Dengan demikian, komunikasi antar task melalui queue tetap berjalan, tetapi sumber datanya kini berasal dari sensor nyata sehingga output menjadi dinamis.