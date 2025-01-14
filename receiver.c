#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define RECEIVE_PIN 2  // Broche GPIO 27 (wiringPi pin 2) pour la réception de données

// Fonction pour lire un octet
unsigned char read_byte() {
    unsigned char byte = 0;
    for (int i = 0; i < 8; i++) {
        byte |= (digitalRead(RECEIVE_PIN) << i);
        delay(1);
    }
    return byte;
}

// Fonction pour calculer le CRC simple (somme des octets)
unsigned char calculate_crc(unsigned char *data, int length) {
    unsigned char crc = 0;
    for (int i = 0; i < length; i++) {
        crc += data[i];
    }
    return crc;
}

int main(void) {
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Unable to setup wiringPi: %s\n", strerror(errno));
        return 1;
    }

    pinMode(RECEIVE_PIN, INPUT);

    while (1) {
        // Lecture de l'en-tête
        unsigned char header = read_byte();
        if (header == 0xAA) {
            // Lecture de la longueur
            unsigned char length = read_byte();
            unsigned char data[length];

            // Lecture des données utiles
            for (int i = 0; i < length; i++) {
                data[i] = read_byte();
            }

            // Lecture du CRC
            unsigned char received_crc = read_byte();
            unsigned char calculated_crc = calculate_crc(data, length);

            // Vérification du CRC
            if (received_crc == calculated_crc) {
                printf("Received data: ");
                for (int i = 0; i < length; i++) {
                    printf("%02X ", data[i]);
                }
                printf("\n");
            } else {
                printf("CRC error\n");
            }
        }
    }

    return 0;
}
