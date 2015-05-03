#include <Arduino.h>
#include <driverlib/prcm.h>
#include "Oxino.h"

void setup() {

    Serial.begin(115200);
    Serial.println("Watchdog begin");
    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, LOW);
    delay(1000);

    Watchdog.enable(1000);

    unsigned long cause = PRCMSysResetCauseGet();
    Serial.print("Reset Cause: ");
    Serial.println(cause);
}

void loop() {
    digitalWrite(RED_LED, HIGH);
    for (int i = 0; i < 5; ++i) {
        Watchdog.reset();
        Serial.println(i);
        delay(1000);
    }
    while(1);

}