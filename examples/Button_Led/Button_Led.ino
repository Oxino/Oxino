#include "Oxino.h"

#define PULLUP false
#define INVERT false

#ifdef __CC3200R1M1RGC__
#define BUTTON_PIN PUSH2
#else
#define BUTTON_PIN 2
#endif

//Button myBtn(BUTTON_PIN, INVERT, PULLUP);    //Declare the button

void setup() {
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    if (digitalRead(BUTTON_PIN) == HIGH) {
        digitalWrite(LED_BUILTIN, HIGH); // on
    } else {
        digitalWrite(LED_BUILTIN, LOW); // off
    }
}