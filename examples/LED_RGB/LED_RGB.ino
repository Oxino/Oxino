#include <Oxino.h>

RGBLED RGB(2, 3, 4);

void setup() {
    // !!! Important
    SPWM.begin();

    RGB.brightness(100);
    RGB.breath(RGB_COLOR_ORANGE);
}

void loop() {
    RGB.update();
}
