# TASK-4 Cross Platform File Transfer

> **DEADLINE SENIN, 20 OKT 2025 PUKUL 18.00**

Untuk task ini program yang akan kalian buat diharuskan untuk memecah-mecah file `.json` yang ada di laptop kalian. Dari hasil pecahan tersebut kalian harus mengirimkan data hasil tersebut ke esp-bridge atau esp yang terhubung ke laptop kalian melalui **serial communication**. Saat esp-bridge kalian menerima data dari serial communication, esp-bridge tersebut kemudian meneruskan ke esp-receiver melalui **ESP-NOW**. Esp-receiver di lain sisi ketika menerima data terusan dari **ESP-NOW**, akan menyusun kembali tiap pecahan data yang dikirim ke **SPIFFS** esp-receiver. Saat pengiriman dan penyusunan data sudah selesai, maka esp receiver akan memberikan output melalui serial monitor sesuai dengan format yang akan dijelaskan di README ini.

### KETENTUAN 

Setelah esp-receiver selesai menyusun data dari ESP-NOW, esp-receiver harus mengoutputkan hasil data yang sudah disusun ke serial monitor dengan format:

```
[DATA INPUT FROM ESP-NOW]
MOTION_MOVIE_ID: Z
MOTION_FRAME_ID: X
TIME_FRAME: Y
DATA:
670 500 130 400 ... ... ...
```
Z sebagai id dari motion movienya. mekanisme masih sama seperti task-2. Perbedaannya untuk output dari esp-receiver berpedoman pada file motion_bucket.
Dengan X, sebagai ID dari motion frame. 
**Perhatikan properties dari motion_movie untuk motion_unitnya seperti, loop, speed, dan time frame**. Y sebagai berapa lama motion_frame yang dikirim harus dijalankan. Sebagai contoh, suatu motion frame memiliki nilai sudut-sudut dengan time frame 1000 dan speed 1.5, maka sudut-sudut akan maka TIME_FRAME nilainya 1000 / 1.5 = ....

#### Catatan
Nilai Plus Jika Program Pada Laptop:
- Mengimplementasikan OOP
- Menggunakan CMakeLists

Nilai Plus Jika Program ESP:
- Mengimplementasikan header
- Mengimplementasikan protokol pada tiap pengiriman data.