#include "Oxino.h"

void setup() {
    Serial.begin(115200);
    Serial.println("SPWM simple");
    SPWM.begin(500);
    SPWM.write(RED_LED, 10);
    SPWM.write(YELLOW_LED, 50);
    SPWM.write(GREEN_LED, 100);

    Serial.println(SPWM.size());

    SPWM.printInterruptLoad();
}

void loop() {

}