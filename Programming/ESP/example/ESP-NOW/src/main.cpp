#include <Arduino.h>
#include <WiFi.h>
#include <esp_err.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define MAC_ADDRESS_TOTAL 2
#define MAC_ADDRESS_LENGTH 6

esp_now_peer_info_t peer_info;

// Mac address yang tersedia
uint8_t mac_addresses[MAC_ADDRESS_TOTAL][MAC_ADDRESS_LENGTH] = {
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x11},  // ADDRESS 1
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x10},  // ADDRESS 2
};

// Definisi nama mac address
const char *mac_names[MAC_ADDRESS_TOTAL] = {
    "ESP1",  // 0
    "ESP2",  // 1
};

int cari_mac_index(const uint8_t *mac) {
    for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
        // Compare the MAC address
        if (memcmp(mac, mac_addresses[i], MAC_ADDRESS_LENGTH) == 0)
            return i;
    }

    // if not found return -1
    return -1;
}

// Fungsi callback untuk menerima data
void callback_data_esp_now(const uint8_t *mac, const uint8_t *data, int len) {
    int mac_index = cari_mac_index(mac);
    Serial.printf("Data diterima dari MAC[%s]: ", mac_names[mac_index]);
    for (int i = 0; i < MAC_ADDRESS_LENGTH; i++) Serial.printf("%02x:", mac[i]);
    Serial.println();
    Serial.println("Data diterima: ");
    for (int i = 0; i < len; i++) Serial.printf("%d ", data[i]);
}

// Fungsi callback untuk pengiriman data melalui esp-now
void callback_pengiriman_esp_now(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.printf("\n\nStatus pengiriman ESP-NOW: %s\n\n", esp_err_to_name(status));
}

// Fungsi untuk menginisiasi ESP-NOW
esp_err_t mulai_esp_now(int index_mac_address) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    /* Init ESP-NOW */
    esp_err_t result = esp_now_init();
    if (result != ESP_OK)
        return result;

    /* Set callback function to handle received data */
    result = esp_now_register_recv_cb(callback_data_esp_now);
    if (result != ESP_OK)
        return result;

    result = esp_now_register_send_cb(callback_pengiriman_esp_now);
    //     if (result != ESP_OK)
    //         return result;

    /* Set MAC Address */
    uint8_t mac[MAC_ADDRESS_LENGTH];
    memcpy(mac, mac_addresses[index_mac_address], MAC_ADDRESS_LENGTH);
    result = esp_wifi_set_mac(WIFI_IF_STA, mac);
    if (result != ESP_OK)
        return result;

    /* Initialize peer_info and set fields*/
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
    peer_info.channel = 0;
    peer_info.encrypt = false;

    /* Add All MAC to peer list  */
    for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
        memcpy(peer_info.peer_addr, mac_addresses[i], MAC_ADDRESS_LENGTH);
        result = esp_now_add_peer(&peer_info);
        if (result != ESP_OK)
            return result;
    }

    return ESP_OK;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Pengiriman data ESP-NOW");

    // Memulai esp now dengan address MAC_INDEX yang mana merupakan build_flags dalam platformio.ini sesuai environmentnya
    mulai_esp_now(MAC_INDEX);
    // Ganti environment pada project ini untuk berganti index esp atau langusng ganti MAC_INDEX dengan nilai lain

    // Definisi data dan mengirimnya
    uint8_t data_sent[5] = {1, 2, 3, 4, 5};
    esp_now_send(mac_addresses[MAC_INDEX == 0 ? 1 : 0], data_sent, sizeof(data_sent));
}

void loop() {
}