//#include "beat.h"
//#include "geq.h"
#include "globals.h"
#include "leds.h"
#include "ota.h"
#include "mqttLogic.h"
#include <Arduino.h>
#include <FastLED.h>
#include <WiFiMulti.h>

WiFiMulti wifi;

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    Serial.begin(921600);
    wifi.addAP(WIFI_SSID, WIFI_PASS);

    Serial.println("Connecting Wifi...");
    if (wifi.run() == WL_CONNECTED) {
        WiFi.setHostname(MY_HOSTNAME);
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }

    initOTA();

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    initLeds();
    initMqtt();
    artnetInit();
}

uint32_t measure = 0;

void loop() {
    wifi.run(); // 2us
    loopOTA(); // 100us
    if(!otaRunning) {
        loopLeds();
        loopMqtt();
    }
}