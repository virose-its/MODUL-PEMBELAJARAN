#include <Arduino.h>
#define BAUDRATE 115200

// Setup cuma
void setup() {
    // Memulai Serial
    Serial.begin(BAUDRATE);
    Serial.println("Serial dimulai dengan kecepatan %d", BAUDRATE);

    Serial.println("Mulai membaca serial...");
}

void loop() {
    // Mengecek jika terdapat nilai pada buffer serial
    if (Serial.available()) {
        // Menerima semua input sebagai char dan dicetak
        // while (Serial.available()) Serial.printf("%c", Serial.read());
        // Serial.println();

        // Menerima input dengan input filtering (sesuai protokol komunikasi kustom)
        // Hanya menerima data yang dimulai dengan 0xFF 0xFD 0x00
        if (Serial.read() != 0xFF) return;
        if (Serial.read() != 0xFD) return;
        if (Serial.read() != 0x00) return;

        // Dilanjut dengan menerima panjang data dan datanya
        uint8_t panjang_data = Serial.read();
        uint8_t data[panjang_data];
        Serial.readBytes(data, panjang_data);

        Serial.printf("Data diterima: ");
        for (int i = 0; i < panjang_data; i++) {
            Serial.printf("%d ", data[i]);
        }
        // Cuma cetak newline biar enak dilihat
        Serial.printf("\n\n");
    }
}