#include "Oxino.h"

ATOS atos;

void setup() {
    Serial.begin(115200);
    Serial.println("ATOS Reset");
}

void loop() {
    delay(2000);
    Serial.println("Reseting board");
    atos.reset();
}
