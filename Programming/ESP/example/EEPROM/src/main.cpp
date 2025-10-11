#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 128

#define LED_PIN 2

const int ADDRESS_PANJANG_NAMA = 0;
const int ADDRESS_NAMA = 1;
int PANJANG_NAMA = 5;
char namaku[5] = "BISMA";

const byte pin_button = 0;
void setup() {
    Serial.begin(115200);
    // Inisialisasi EEPROM
    EEPROM.begin(EEPROM_SIZE);

    // Menulis nilai ke memmory EEPROM
    EEPROM.writeByte(ADDRESS_PANJANG_NAMA, PANJANG_NAMA);
    for (int i = 0; i < PANJANG_NAMA; i++) {
        EEPROM.writeByte(ADDRESS_NAMA + i, namaku[i]);
    }

    // Membaca memmory EEPROM
    Serial.printf("Membaca nilai EEPROM pada alamat [%d] hingga [%d]", ADDRESS_NAMA, ADDRESS_NAMA + PANJANG_NAMA);
    for (int i = 0; i < 0; i++) {
        Serial.printf("%c", EEPROM.readByte(ADDRESS_NAMA + i));
    }
    Serial.println();
}

void loop() {
}