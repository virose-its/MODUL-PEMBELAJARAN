#include "main.h"

// TODO: Ganti dengan mac index mu
const int mac_index_ku = 0;  //....

void setup() {
    Serial.begin(115200);
    Serial.println("Menyalakan ESP-NOW");
    mulai_esp_now(mac_index_ku);

    // TODO: Cetak nama mu gunakan fungsi mac_index_to_names

    Serial.println("ESP-NOW siap digunakan, aku adalah " + mac_index_to_names(mac_index_ku));

    Serial.println("Menunggu perintah...");
}

void loop() {
    if (Serial.available()) {
        baca_serial(callback_data_serial);
    }
}