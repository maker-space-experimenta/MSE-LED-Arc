#include "artnet.h"
#include "geq.h"
#include "globals.h"
#include "idleAnimation.h"
#include "ota.h"
#include <Arduino.h>
#include <FastLED.h>
#include <WiFiMulti.h>

WiFiMulti wifi;
CRGB leds[NUM_LEDS];

void setup() {
    Serial.begin(115200);
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

    idleAnimationSetup();
    artnetInit();
    geqInit();
}

void loop() {
    wifi.run();
    loopOTA();
    if (!artnetLoop()) {
        idleAnimationLoop();
    }
    geqLoop();
}