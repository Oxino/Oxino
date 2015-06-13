#include "Oxino.h"

ATOS atos;
const byte LED1 = RED_LED;
const byte LED2 = GREEN_LED;
const byte LED3 = BLUE_LED;

byte status = 0;

void start() {
    for (int i = 0; i < 2; ++i) {
        digitalWrite(LED3, HIGH);
        delay(125);
        digitalWrite(LED3, LOW);
        delay(125);
    }
}

int flashLed(void *params) {
    digitalWrite(LED1, status);
    status ^= 1;
    atos.sleep(150);
}

bool flag = false;
int freeze(void *params) {
    if (!flag) {
        atos.sleep(2000);
        flag = true;
    } else {
        digitalWrite(LED1, LOW); // off led1
        while (1);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("ATOS reset after timout");

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);

    atos.begin(2000);
    atos.createTask(flashLed);
    atos.createTask(freeze);
    start();
}

void loop() {
    digitalWrite(LED2, HIGH);
    delay(150);
    digitalWrite(LED2, LOW);
    delay(150);
}
