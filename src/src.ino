#include <Arduino.h>
#include <WiFiMulti.h>
#include <FastLED.h>
#include "globals.h"
#include "ota.h"
#include "idleAnimation.h"


WiFiMulti wifi;
CRGB leds[NUM_LEDS];

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

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    idleAnimationSetup();
}

void loop() {
    loopOTA();
    wifi.run();
    idleAnimationLoop();
}