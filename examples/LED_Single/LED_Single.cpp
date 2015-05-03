#include <Oxino.h>

#ifdef __CC3200R1M1RGC__
uint8_t LED_1_PIN = RED_LED;
uint8_t LED_2_PIN = YELLOW_LED;
#else
uint8_t LED_1_PIN = 13;
uint8_t LED_2_PIN = 20;
#endif

SingleLED led1(LED_1_PIN);    // Use onboard LED
SingleLED led2(LED_2_PIN);    // Use onboard LED
unsigned long time = 0;
int ledState = 0;

void setup() {
    // !!! Important
    SPWM.begin();
}

void loop() {
    if (millis() > time + 5000) {    // Every 5 seconds change between states
        time = millis();
        if (ledState == 0) {
            led1.on();
            led2.breath(15);
            ledState = 1;
        } else if (ledState == 1) {
            led1.breath(30);
            led2.fadeDown(15);
            ledState = 2;
        } else if (ledState == 2) {
            led1.fadeDown(30);
            led2.fadeUp(30);
            ledState = 3;
        } else if (ledState == 3) {
            led1.fadeUp(10);
            led2.blink(125);
            ledState = 4;
        } else if (ledState == 4) {
            led1.blink(250);
            led2.off();
            ledState = 5;
        } else if (ledState == 5) {
            led1.off();
            led2.on();
            ledState = 6;
        } else {
            led2.on();
            led1.off();
            ledState = 0;
        }
    }
    led1.update();
    led2.update();
}
