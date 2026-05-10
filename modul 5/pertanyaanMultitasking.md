## Pertanyaan Praktikum Multitasking

1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya! 
2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya! 
3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu 
gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan 
program pada file README.md. 

---
## Jawaban

### 1. Mekanisme Eksekusi Tiga Task

Ketiga task pada program `multitasking.ino` tidak benar-benar berjalan secara bersamaan, tetapi berjalan bergantian dengan sangat cepat. Hal ini terjadi karena Arduino hanya mengeksekusi satu instruksi pada satu waktu. FreeRTOS mengatur pembagian waktu eksekusi menggunakan scheduler sehingga setiap task memperoleh giliran untuk berjalan.

Pada program ini terdapat TaskBlink1, TaskBlink2, dan Taskprint yang semuanya dibuat dengan prioritas yang sama, yaitu 1. Masing-masing task menjalankan pekerjaannya di dalam loop tak hingga, lalu memanggil vTaskDelay(). Saat sebuah task memanggil vTaskDelay(), task tersebut masuk ke status blocked sementara, sehingga scheduler dapat menjalankan task lain. Karena perpindahan ini berlangsung sangat cepat, ketiga task terlihat seperti berjalan bersamaan, padahal sebenarnya berjalan bergantian.

---
### 2. Cara Menambahkan Task Keempat

Untuk menambahkan task keempat, langkahnya adalah:

1. Menambahkan deklarasi fungsi task baru.
2. Membuat isi fungsi task baru.
3. Menambahkan pemanggilan xTaskCreate() di setup().
4. Menentukan pin, stack size, dan prioritas task sesuai kebutuhan.

Contoh penambahan task keempat:

```cpp
#include <Arduino_FreeRTOS.h>

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);
void TaskBlink3(void *pvParameters);

void setup() {
  Serial.begin(9600);

  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink3, "task4", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {}

void TaskBlink3(void *pvParameters) {
  pinMode(7, OUTPUT);

  while (1) {
    digitalWrite(7, HIGH);
    vTaskDelay(400 / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}
```

Dengan cara tersebut, task keempat akan ikut dijadwalkan oleh FreeRTOS bersama task lain.

---
### 3. Modifikasi Program dengan Potensiometer untuk Mengontrol Kecepatan LED

Program dapat dimodifikasi dengan menambahkan potensiometer pada pin A0, lalu nilai ADC dari potensiometer digunakan untuk mengatur delay LED. Semakin besar nilai potensiometer, semakin lambat kedipan LED. Semakin kecil nilainya, semakin cepat kedipan LED.

Contoh program modifikasi:

```cpp
#include <Arduino_FreeRTOS.h>

void TaskBlinkPot(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup() {
  Serial.begin(9600);

  xTaskCreate(TaskBlinkPot, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {}

void TaskBlinkPot(void *pvParameters) {
  pinMode(9, OUTPUT);

  while (1) {
    int pot = analogRead(A0);
    int delayMs = map(pot, 0, 1023, 100, 1000);

    digitalWrite(9, HIGH);
    vTaskDelay(delayMs / portTICK_PERIOD_MS);
    digitalWrite(9, LOW);
    vTaskDelay(delayMs / portTICK_PERIOD_MS);
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(8, OUTPUT);

  while (1) {
    digitalWrite(8, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(8, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;

  while (1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```

Hasilnya, LED pada pin 9 akan berkedip dengan kecepatan yang berubah-ubah mengikuti posisi potensiometer. Sistem tetap multitasking karena task LED lain dan task pencetak counter tetap berjalan sesuai jadwal scheduler. Dengan demikian, sensor analog dapat dipakai untuk mengontrol perilaku salah satu task secara dinamis.