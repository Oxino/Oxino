#include <WiFi/WiFiClient.h>
#include "Oxino.h"
#include "WiFi.h"

ATOS atos;

void setup() {
    Serial.begin(115200);
    Serial.println("ATOS Reset Example");
}

void loop() {
//    WiFi.disconnect();
    WiFi.begin();
    Serial.println("Wating connect to wifi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }

    WiFiClient client;
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect("energia.nu", 80)) {
        Serial.println("\nConnected");
    }

    WiFi.off();

    delay(2000);
    Serial.println("Reseting board");
    atos.reset();
}
