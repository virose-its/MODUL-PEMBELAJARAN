#include "main.h"

esp_now_peer_info_t peer_info;

uint8_t mac_addresses[MAC_ADDRESS_TOTAL][MAC_ADDRESS_LENGTH] = {
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x11},  // BISMA
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x10},  // JSON
    {0x24, 0x0A, 0xC4, 0x0A, 0x20, 0x11},  // FARUG
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x10},  // Fauzan Firdaus
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x11},  // Africha Sekar wangi
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x10},  // Rafaina Erin Sadia
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x11},  // Antonius Michael Yordanis Hartono
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x10},  // Dinda Sofi Azzahro
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x11},  // Muhammad Fahmi Ilmi
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x10},  // Dhanishara Zaschya Putri Syamsudin
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x11},  // Irsa Fairuza
    {0x24, 0x0A, 0xC4, 0x0A, 0x14, 0x10},  // Revalinda Bunga Nayla Laksono

};

const char *mac_names[MAC_ADDRESS_TOTAL] = {
    "BISMA",                               // 0
    "JSON",                                // 1
    "FARUG",                               // 2
    "Fauzan Firdaus",                      // 3
    "Africha Sekar Wangi",                 // 4
    "Rafaina Erin Sadia",                  // 5
    "Antonius Michael Yordanis Hartono",   // 6
    "Dinda Sofi Azzahro",                  // 7
    "Muhammad Fahmi Ilmi",                 // 8
    "Dhanishara Zaschya Putri Syamsudin",  // 9
    "Irsa Fairuza",                        // 10
    "Revalinda Bunga Nayla Laksono",       // 11
};

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

int cari_mac_index(const uint8_t *mac) {
    for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
        // Compare the MAC address
        if (memcmp(mac, mac_addresses[i], MAC_ADDRESS_LENGTH) == 0)
            return i;
    }

    // if not found return -1
    return -1;
}

String mac_index_to_names(int mac_index) {
    if ((mac_index < 0 || mac_index >= MAC_ADDRESS_TOTAL) || (mac_index == -1)) {
        return "Unknown";
    }
    return mac_names[mac_index];
}

void callback_data_esp_now(const uint8_t *mac, const uint8_t *data, int len) {
    int index_mac_asal = cari_mac_index(mac);
    process_perintah(data, len, index_mac_asal);
}
void callback_pengiriman_esp_now(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.printf("\nStatus pengiriman ESP-NOW: %s\n", esp_err_to_name(status));
}
void callback_data_serial(const uint8_t *data, int len) {
    process_perintah(data, len);
}
void baca_serial(void (*callback)(const uint8_t *data, int len)) {
    // TODO: implementasi kode buat nerima perintah dari serial

    // TODO: panggil callback sesuai dengan yang parameter yang diminta CLUE: ada di definisi parameter fungsi baca serial
    // callback(....)

    if (Serial.read() != 0xFF) {
        return;
    }
    if (Serial.read() != 0xAA) {
        return;
    }
    int len = Serial.read();
    uint8_t data[len];
    Serial.readBytes(data, len);
    callback(data, len);
}

void process_perintah(const uint8_t *data, int len, int index_mac_address_asal) {
    // TODO: implementasi kode buat processing perintah

    COMMAND cmd = (COMMAND)data[0];
    if (cmd == HALO) {
        if (index_mac_address_asal == -1) {
            uint8_t tujuan = data[1];
            Serial.printf("Menyapa %s\n", mac_index_to_names(tujuan).c_str());

            uint8_t *data;
            String sapaan = "Halo " + mac_index_to_names(cari_mac_index(mac_addresses[tujuan])) + " Aku " + mac_index_to_names(mac_index_ku);
            int len_sapaan = sapaan.length() + 1;
            data = new uint8_t[len_sapaan];
            data[0] = HALO;
            memcpy(data + 1, sapaan.c_str(), sapaan.length());
            esp_now_send(mac_addresses[tujuan], data, len_sapaan);
            delete[] data;
            Serial.println();
        } else {
            Serial.printf("Menerima sapaan dari %s\n", mac_index_to_names(index_mac_address_asal).c_str());
            char sapaan[len];
            sapaan[len] = '\0';
            memcpy(sapaan, data + 1, len - 1);
            Serial.println(sapaan);

            uint8_t tujuan = index_mac_address_asal;
            uint8_t *data;
            String jawaban = "Halo juga " + mac_index_to_names(cari_mac_index(mac_addresses[tujuan])) + " Aku " + mac_index_to_names(mac_index_ku);
            int len_jawaban = jawaban.length() + 1;
            data = new uint8_t[len_jawaban];
            data[0] = JAWAB;
            memcpy(data + 1, jawaban.c_str(), jawaban.length());
            esp_now_send(mac_addresses[tujuan], data, len_jawaban);
            delete[] data;
            Serial.println();
        }

    } else if (cmd == CEK) {
        if (index_mac_address_asal == -1) {
            uint8_t tujuan = data[1];

            uint8_t *data;

            Serial.printf("Mencari %s\n", mac_index_to_names(tujuan).c_str());
            String cek = mac_index_to_names(tujuan) + " ini " + mac_index_to_names(mac_index_ku) + " apa kamu di sana?";
            int len_cek = cek.length() + 1;
            data = new uint8_t[len_cek];
            data[0] = CEK;
            memcpy(data + 1, cek.c_str(), cek.length());
            esp_now_send(mac_addresses[tujuan], data, len_cek);
            delete[] data;
            Serial.println();

        } else {
            Serial.printf("%s Mencariku\n", mac_index_to_names(index_mac_address_asal).c_str());
            char cek[len];
            cek[len] = '\0';
            memcpy(cek, data + 1, len - 1);
            Serial.println(cek);

            uint8_t tujuan = index_mac_address_asal;
            uint8_t *data;
            String jawaban = "Iya aku di sini " + mac_index_to_names(cari_mac_index(mac_addresses[tujuan])) + " - " + mac_index_to_names(mac_index_ku);
            int len_jawaban = jawaban.length() + 1;
            data = new uint8_t[len_jawaban];
            data[0] = JAWAB;
            memcpy(data + 1, jawaban.c_str(), jawaban.length());
            esp_now_send(mac_addresses[tujuan], data, len_jawaban);
            delete[] data;
            Serial.println();
        }

    } else if (cmd == JAWAB) {
        Serial.printf("Menerima pesan dari %s:\n", mac_index_to_names(index_mac_address_asal).c_str());
        char jawaban[len];
        jawaban[len] = '\0';
        memcpy(jawaban, data + 1, len - 1);
        Serial.println(jawaban);
        Serial.println();

        recap_response(index_mac_address_asal, jawaban);

    } else if (cmd == BROADCAST) {
        Serial.println("Mulai Broadcast");
        COMMAND command = (COMMAND)data[1];
        byte start_index = data[2];
        byte end_index = data[3];
    }
}

void recap_response(int8_t origin, char *msg, int len, byte check_index_start, byte check_index_end) {
    static std::vector<message_recap_t> received_messages;
    static byte index_start = 0;
    static byte index_end = MAC_ADDRESS_TOTAL - 1;
    if ((origin == -1 && len == 0) && (check_index_start == -1 && check_index_end == -1)) {
        // Print Verdict
        Serial.println("Recap Summary:");
        for (int i = index_start; i <= index_end; i++) {
            bool found = false;
            for (const auto &msg : received_messages) {
                if (msg.origin == i) {
                    Serial.printf("- %s: %s\n", mac_index_to_names(msg.origin).c_str(), msg.msg);
                    found = true;
                    break;
                }
            }
            if (!found) {
                Serial.printf("- %s: No Response\n", mac_index_to_names(i).c_str());
            }
        }

        received_messages.clear();
        index_start = 0;
        index_end = MAC_ADDRESS_TOTAL - 1;
        return;
    } else if ((origin == -1 && len == 0) && (check_index_start != -1 && check_index_end != -1)) {
        // Set index range
        index_start = check_index_start;
        index_end = check_index_end;
        received_messages.clear();
    } else {
        received_message new_message = {origin, ""};
        if (len > 0 && len < sizeof(new_message.msg)) {
            memcpy(new_message.msg, msg, len);
        }
        received_messages.push_back(new_message);
    }
}

void broadcast_message(COMMAND command, byte start_index, byte end_index) {
    byte *data_sent;
    recap_response(-1, "", 0, start_index, end_index);
    if (command == CEK) {
        Serial.printf("Mencari %s\n", mac_index_to_names(tujuan).c_str());
        String cek = mac_index_to_names(tujuan) + " ini " + mac_index_to_names(mac_index_ku) + " apa kamu di sana?";
        for (int i = start_index; i <= end_index; i++) {
            int len_cek = cek.length() + 1;
            data_sent = new byte[len_cek];
            data_sent[0] = CEK;
            memcpy(data_sent + 1, cek.c_str(), cek.length());
            esp_now_send(mac_addresses[i], data_sent, len_cek);
            delete[] data_sent;
            Serial.println();
        }
    } else if (command == HALO) {
        Serial.printf("Menyapa %s\n", mac_index_to_names(tujuan).c_str());
        String sapaan = "Halo " + mac_index_to_names(cari_mac_index(mac_addresses[tujuan])) + " Aku " + mac_index_to_names(mac_index_ku);
        for (int i = start_index; i <= end_index; i++) {
            int len_sapaan = sapaan.length() + 1;
            data_sent = new byte[len_sapaan];
            data_sent[0] = HALO;
            memcpy(data_sent + 1, sapaan.c_str(), sapaan.length());
            esp_now_send(mac_addresses[i], data_sent, len_sapaan);
            delete[] data_sent;
            Serial.println();
        }
    }

    recap_response(-1, "", 0);
}