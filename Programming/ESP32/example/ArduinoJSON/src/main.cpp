
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// Nilai didalam test.json
// {
//     "nama" : "Bisma",
//     "umur" : 20,
//     "jurusan" : "Teknik Informatika",
//     "hobi" : "ngelamun"
// }

const char* file_path = "/test.json";

void setup() {
    Serial.begin(115200);

    // Menginisiasi SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS");
        return;
    }
    Serial.println("SPIFFS mounted");

    // Buat dan Tulis nilai di contoh json diatas kedalam file test.json

    // Membuat buffer file json
    StaticJsonDocument<256> write_doc;
    write_doc["nama"] = "Bisma";
    write_doc["umur"] = 20;
    write_doc["jurusan"] = "Teknik Informatika";
    write_doc["hobi"] = "ngelamun";

    // Membuka file test.json untuk ditulis sesuatu
    File file = SPIFFS.open(file_path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open /test.json for writing");
    } else {
        String json;
        if (serializeJson(write_doc, json) == 0) {
            Serial.println("Failed to serialize JSON to file");
        } else {
            file.print(json);
            Serial.println("Wrote JSON to /test.json");
        }
        file.close();
    }

    // Membuka file test.json untuk dibaca
    file = SPIFFS.open(file_path, FILE_READ);
    if (!file) {
        Serial.println("Failed to open /test.json");
        return;
    }

    // membuat variable buffer untuk file json (static, no dynamic allocation)
    StaticJsonDocument<256> read_doc;  // adjust size if your JSON grows
    // Membaca file json
    DeserializationError err = deserializeJson(read_doc, file);
    file.close();
    if (err) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(err.f_str());
        return;
    }

    // mengekstrak nilai json
    // Note "|" adalah sebagai OR gate untuk jika nilai yang diambil dengan contoh doc["nama"] tidak ada maka akan diganti dengan nilai di samping "|"
    const char* nama = read_doc["nama"] | "Tidak ada";
    int umur = read_doc["umur"] | 0;
    const char* jurusan = read_doc["jurusan"] | "Tidak ada";
    const char* hobi = read_doc["hobi"] | "Tidak ada";

    // Print nilai yang dibaca
    Serial.println("Contents of /test.json:");
    Serial.print("nama: ");
    Serial.println(nama);
    Serial.print("umur: ");
    Serial.println(umur);
    Serial.print("jurusan: ");
    Serial.println(jurusan);
    Serial.print("hobi: ");
    Serial.println(hobi);
}

void loop() {
}