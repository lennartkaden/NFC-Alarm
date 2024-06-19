#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "password.h"

ESP8266WebServer server(80);  // Webserver auf Port 80 erstellen

const char* ssid = "FRITZ!Box Fon WLAN 7390";
bool alarm = false;
unsigned long alarm_time = 0;

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    // set LED Pint type output
    pinMode(D8, OUTPUT);

    server.on("/alarm", HTTP_POST, []() {
        alarm = true;
        alarm_time = millis() + 1000 * 5; // 5 Sekunden
        return server.send(200, "text/plain", "Alarm ausgelöst");
    });

    server.begin();  // Webserver starten
}

void  handle_alarm() {
    if (alarm) {
        digitalWrite(D8, HIGH);
    }
}

void check_alarm() {
    if (alarm && millis() > alarm_time) {
        handle_alarm();
    } else {
        digitalWrite(D8, LOW);
    }
}

void loop() {
// write your code here
    server.handleClient();
    check_alarm();

    delay(1);
}