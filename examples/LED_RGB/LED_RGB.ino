#include <Oxino.h>

void setup() {
    // !!! Important
    SPWM.begin();

    RGB.brightness(100);
}

unsigned long t = 0;
void loop() {
    t = millis();
    RGB.light(RGB_COLOR_RED);
    while (millis() - t < 2000) RGB.update();

    t = millis();
    RGB.light(RGB_COLOR_GREEN);
    while (millis() - t < 2000) RGB.update();

    t = millis();
    RGB.light(RGB_COLOR_BLUE);
    while (millis() - t < 2000) RGB.update();

    t = millis();
    RGB.blink(RGB_COLOR_RED);
    while (millis() - t < 2000) RGB.update();

    t = millis();
    RGB.blink(RGB_COLOR_GREEN);
    while (millis() - t < 2000) RGB.update();

    t = millis();
    RGB.blink(RGB_COLOR_BLUE);
    while (millis() - t < 2000) RGB.update();
}
