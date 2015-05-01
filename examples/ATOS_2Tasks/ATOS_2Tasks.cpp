#include "Oxino.h"
#include "WiFi.h"
ATOS atos; //create a new istance

//variables to manage the LEDs
const byte LED1 = RED_LED;
const byte LED2 = GREEN_LED;
byte LED1status = 1;
byte LED2status = 1;

//this task will flash the 1st LED
int flashLed1(void *params) {
    LED1status ^= 1;
    digitalWrite(LED1, LED1status);
    atos.sleep(1000);
}


//this task will flash the 2nd LED
int flashLed2(void *params) {
    LED2status ^= 1;
    digitalWrite(LED2, LED2status);
    atos.sleep(250);
}


String _ID;

String &GetUniqueID() {

    if (_ID == NULL) {
        _ID.reserve(12);

        uint8_t buf[6];
        // Using mac address as id
        WiFi.macAddress(buf);

        char hex_digit;
        for (int i = 0; i < 6; ++i) {
            hex_digit = (char) (48 + (buf[i] >> 4));
            if (57 < hex_digit)
                hex_digit += 39;
            _ID += hex_digit;
            hex_digit = (char) (48 + (buf[i] & 0xf));
            if (57 < hex_digit)
                hex_digit += 39;
            _ID += hex_digit;
        }
    }

    return _ID;

}

//setup routine
void setup() {
    GetUniqueID();
    atos.begin(); //initialize the scheduler
    //pins as output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    //add the tasks
    atos.createTask(flashLed1);
    atos.createTask(flashLed2);
}


//main loop - it's empty
void loop() {}