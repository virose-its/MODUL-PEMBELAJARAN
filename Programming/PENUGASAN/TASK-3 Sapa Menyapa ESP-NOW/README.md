# TASK-3 Sapa Menyapa

### Penugasan 3 Modul ESP-Programming

> Deadline Rabu 15 Oktober 2025

#### **TODO 1**: Buatlah perintah untuk mencetak teks yang menyatakan identitas mu.

https://github.com/virose-its/MODUL-PEMBELAJARAN/blob/e137fed29e4db7e655b02d499a70e7c17d62d43a/Programming/PENUGASAN/TASK-3%20Sapa%20Menyapa%20ESP-NOW/TEMPLATE%20TASK-3%20Sapa%20Menyapa/src/main.cpp#L6-L14

#### **TODO 2**: Buatlah perintah pemrosesan data perintah yang akan diterima serial dari laptop.
https://github.com/virose-its/MODUL-PEMBELAJARAN/blob/e137fed29e4db7e655b02d499a70e7c17d62d43a/Programming/PENUGASAN/TASK-3%20Sapa%20Menyapa%20ESP-NOW/TEMPLATE%20TASK-3%20Sapa%20Menyapa/src/utility.cpp#L105-L107

Ketentuan penerimaan data dari serial memiliki Struktur sebagai berikut:  
| HEADER | LENGTH | DATA |  
|--------|----------|-----------|
| 3 byte | 1 byte | N byte |

- HEADER: 0xFF 0xFF 0x00
- LENGTH: Panjang data yang akan dikirim (N byte)
- DATA: Data yang akan dikirim berukuran N byte

**Contoh**
| HEADER | LENGTH | DATA |  
|--------|----------|-----------|
| 0xFF 0xFF 0x00 | 0x02 | 0x01 0x02 |

- HEADER: 0xFF 0xFF 0x00
- LENGTH: 0x02 (2 byte)
- DATA: 0x01 0x02 (2 byte)
  - 0x01: Perintah untuk CEK
  - 0x02: Tujuan untuk siapa yang dicek

Perintah yang dapat diproses:
| Perintah | Keterangan | Parameter |
|----------|-------------|-----------|
| 0x00 | HALO | 1 byte (Tujuan) / String |
| 0x01 | CEK | 1 byte (Tujuan) / String |
| 0x02 | JAWABAN | String |

> Untuk perintah CEK dan HALO, perintah hanya diikuti 1 parameter yaitu tujuan jika diterima dari Serial. Jika menerima dari ESP-NOW, perintah diikuti dengan String.

#### **TODO 3**: Buatlah perintah untuk memproses perintah yang diterima baik melalui Serial dan ESP-NOW.
https://github.com/virose-its/MODUL-PEMBELAJARAN/blob/e137fed29e4db7e655b02d499a70e7c17d62d43a/Programming/PENUGASAN/TASK-3%20Sapa%20Menyapa%20ESP-NOW/TEMPLATE%20TASK-3%20Sapa%20Menyapa/src/utility.cpp#L109-L111

#### **TODO 4**: Upload Project TASK-3 ini kedalam repo intership-virose mu dan buatlah Dokumentasi menggunakan github markdown yang menjelaskan cara kerja modifikasi yang kamu buat pada project ini agar dapat bekerja.

#### Ketentunan pemrosesan perintah:

Perintah HALO:

- Jika menerima perintah HALO dari Serial, maka kirimkan packet perintah ke pengirim dengan format:
  "HALO(enum), 'Halo [Nama Tujuan] Aku [Namamu]'"
- Jika menerima perintah HALO dari ESP-NOW, maka kirimkan perintah JAWABAN ke tujuan pengirim dengan format "JAWAB(enum), 'Halo Juga [Nama Pengirim] Aku [Namamu]'".

Perintah CEK:

- Jika menerima perintah CEK dari Serial, maka kirimkan packet perintah ke pengirim dengan format:
  "CEK(enum), '[Nama tujuan] ini [Namamu] apa kamu disana?'"
- Jika menerima perintah CEK dari ESP-NOW, maka kirimkan perintah JAWABAN ke tujuan pengirim dengan format "JAWAB(enum), 'Iya Aku [Nama pengirim] Disini - [Namamu]'".

Perintah JAWAB:

- Perintah JAWAB cuma menerima dari ESP-NOW, cetak ke Serial Monitor String yang diterima".

## Keterangan

#### Pembagian MAC_ADDRESS Pake aja namanya

https://github.com/virose-its/MODUL-PEMBELAJARAN/blob/206a0e37ef614928b82e5d92ff3f648a5731d0fe/Programming/PENUGASAN/TASK-3%20Sapa%20Menyapa%20ESP-NOW/TEMPLATE%20TASK-3%20Sapa%20Menyapa/src/main.h#L15-L28
https://github.com/virose-its/MODUL-PEMBELAJARAN/blob/206a0e37ef614928b82e5d92ff3f648a5731d0fe/Programming/PENUGASAN/TASK-3%20Sapa%20Menyapa%20ESP-NOW/TEMPLATE%20TASK-3%20Sapa%20Menyapa/src/utility.cpp#L5-L34

#### Enum Perintah

https://github.com/virose-its/MODUL-PEMBELAJARAN/blob/206a0e37ef614928b82e5d92ff3f648a5731d0fe/Programming/PENUGASAN/TASK-3%20Sapa%20Menyapa%20ESP-NOW/TEMPLATE%20TASK-3%20Sapa%20Menyapa/src/main.h#L30-L34
