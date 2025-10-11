
#include <Arduino.h>

#include "SPIFFS.h"

const char *file_path = "/hello.txt";
const char *message = "Halo, Aku ada didalam text bernama hello.txt";

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    // Menginisiasi SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return;
    }
    Serial.println("SPIFFS mounted");

    // Mengecek pada alamat file jika ditemukan sesuatu akan dihapus
    if (SPIFFS.exists(file_path)) {
        SPIFFS.remove(file_path);
    }

    // Seperti cpp umumnya jika tidak ada file pada alamat maka akan dibuat file tersebut secara default
    File file = SPIFFS.open(file_path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
    } else {
        // Menulis pesan pada file yang dibuka
        Serial.printf("Menulis pesan: [%s] pada file [%s]\n", message, file_path);
        file.println(message);
        file.close();
        Serial.println("File written");
    }

    // Membuka kembali file untuk dibaca
    file = SPIFFS.open(file_path, FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
    } else {
        // membaca langsung nilai yang ada dibuffer file
        Serial.print("File contents: ");
        while (file.available()) {
            Serial.write(file.read());
        }
        Serial.println();
        file.close();
    }
}

void loop() {
}