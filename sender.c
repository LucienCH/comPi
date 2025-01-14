#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SEND_PIN 0  // Broche GPIO 17 (wiringPi pin 0) pour l'envoi de données

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

    pinMode(SEND_PIN, OUTPUT);

    unsigned char header = 0xAA;  // En-tête de la trame
    unsigned char length = 2;     // Longueur des données utiles
    unsigned char data[] = {0x12, 0x34};  // Données utiles
    unsigned char crc = calculate_crc(data, length);  // Calcul du CRC

    while (1) {
        // Envoi de l'en-tête
        for (int i = 0; i < 8; i++) {
            digitalWrite(SEND_PIN, (header >> i) & 1);
            delay(1);
        }

        // Envoi de la longueur
        for (int i = 0; i < 8; i++) {
            digitalWrite(SEND_PIN, (length >> i) & 1);
            delay(1);
        }

        // Envoi des données utiles
        for (int j = 0; j < length; j++) {
            for (int i = 0; i < 8; i++) {
                digitalWrite(SEND_PIN, (data[j] >> i) & 1);
                delay(1);
            }
        }

        // Envoi du CRC
        for (int i = 0; i < 8; i++) {
            digitalWrite(SEND_PIN, (crc >> i) & 1);
            delay(1);
        }

        // delay(1000);  // Attendre 1 seconde avant d'envoyer la prochaine trame
    }

    return 0;
}
