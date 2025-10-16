#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <esp_err.h>
#include <esp_now.h>
#include <esp_wifi.h>

#include <vector>

#define MAC_ADDRESS_INDEX 0
#define EEPROM_SIZE 128
#define MAC_ADDRESS_TOTAL 12
#define MAC_ADDRESS_LENGTH 6
#define BUFFER_SIZE 250

typedef enum {
    BISMA = 0,
    JSON,
    FARUG,
    FAUZAN_FIRDAUS,
    AFRICHA_SEKAR_WANGI,
    RAFAINA_ERIN_SADIA,
    ANTONIUS_MICHAEL_YORDANIS_HARTONO,
    DINDA_SOFI_AZZAHRO,
    MUHAMMAD_FAHMI_ILMI,
    DHANISHARA_ZASCHYA_PUTRI_SYAMSUDIN,
    IRSA_FAIRUZA,
    REVALINDA_BUNGA_NAYLA_LAKSONO,
} ADDRESS_ASSIGNMENT;

typedef enum {
    HALO,
    CEK,
    JAWAB,
    BROADCAST,
} COMMAND;

// Buat cek penugasan
typedef struct {
    int8_t origin;
    char msg[250];
} message_recap_t;

extern const int mac_index_ku;
extern uint8_t mac_addresses[MAC_ADDRESS_TOTAL][MAC_ADDRESS_LENGTH];
extern const char *mac_names[MAC_ADDRESS_TOTAL];
extern esp_now_peer_info_t peer_info;

/**
 * @brief Menginisialisasi ESP-NOW
 * @param index_mac_address nilai untuk akses mac address pada array mac_addresses untuk jadi identitas

*/
esp_err_t mulai_esp_now(int index_mac_address);

/**
 * @brief fungsi untuk melakukan pembacaan data dari serial
 * @param callback fungsi callback yang akan dipanggil saat data diterima
 */
void baca_serial(void (*callback)(const uint8_t *data, int len));

/**
 * @brief fungsi callback untuk menangani data yang diterima melalui ESP-NOW
 * @param mac alamat MAC pengirim
 * @param data pointer ke data yang diterima
 * @param len panjang data yang diterima
 */
void callback_data_esp_now(const uint8_t *mac, const uint8_t *data, int len);

/**
 * @brief fungsi callback untuk menangani status pengiriman data melalui ESP-NOW
 * @param mac_addr alamat MAC pengirim
 * @param status status pengiriman
 */
void callback_pengiriman_esp_now(const uint8_t *mac_addr, esp_now_send_status_t status);

/**
 * @brief fungsi untuk menangani hasil pembacaan data dari serial
 * @param data pointer ke data yang diterima
 * @param len panjang data yang diterima
 * @param mac_index_asal index MAC pengirim (sesuai dengan array mac_addresses)
 */
void callback_data_serial(const uint8_t *data, int len);

/**
 * @brief fungsi untuk memproses perintah yang diterima
 * @param command perintah yang diterima
 * @param data pointer ke data yang diterima
 * @param len panjang data yang diterima
 * @param index_mac_address index MAC pengirim (sesuai dengan array mac_addresses), default -1 (jika dari serial)
 */
void process_perintah(const uint8_t *data, int len, int index_mac_address_asal = -1);

/**
 * @brief fungsi untuk mencari index MAC address
 * @param mac pointer ke alamat MAC yang dicari
 * @return index MAC address jika ditemukan, -1 jika tidak ditemukan
 */
int cari_mac_index(const uint8_t *mac);

/**
 * @brief fungsi untuk mengubah index MAC address menjadi nama
 * @param mac_index index MAC address yang akan diubah
 * @return nama MAC address jika ditemukan, "Unknown" jika tidak ditemukan
 */
String mac_index_to_names(int mac_index);

void recap_response(int8_t origin, char *msg, int len, byte check_index_start = -1, byte check_index_end = -1);
void broadcast_message(COMMAND command, byte start_index, byte end_index);
