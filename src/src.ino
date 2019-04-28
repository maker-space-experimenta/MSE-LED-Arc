#include <Arduino.h>
#include <WiFiMulti.h>
#include <FastLED.h>
#include "ota.h"

WiFiMulti wifi;

void setup() {

    wifi.addAP(WIFI_SSID, WIFI_PASS);

    Serial.println("Connecting Wifi...");
    if(wifi.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }

    initOTA();
}

void loop() {
    loopOTA();
    wifi.run();
}