#include "Oxino.h"

Timo timo;

void setup() {
    Serial.begin(115200);
    Serial.println("Timo expire example");

    Serial.println("Waiting for expire");

    unsigned long t = millis();

    Serial.println();

    timo.delay(1200);
    while (!timo.expired()) {
        Serial.print("Left: ");
        Serial.print(timo.left());
        Serial.println("ms");
        delay(300);
    }

    Serial.println();
    Serial.print("Elapse: ");
    Serial.print(millis() - t);
    Serial.println("ms");

}

void loop() {

}