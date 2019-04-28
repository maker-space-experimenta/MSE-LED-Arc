#include "artnet.h"
#include "geq.h"
#include "globals.h"
#include "idleAnimation.h"
#include "beat.h"
#include "ota.h"
#include <Arduino.h>
#include <FastLED.h>
#include <WiFiMulti.h>

WiFiMulti wifi;
CRGB leds[NUM_LEDS];

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

    idleAnimationSetup();
    artnetInit();
    geqInit();
}

uint32_t measure = 0;

void loop() {
    // measure = micros();
    wifi.run(); // 2us
    // Serial.printf("\nWiFi: %d us", micros() - measure);
    // measure = micros();
    loopOTA(); //100us
    // Serial.printf("\nOTA: %d us", micros() - measure);
    // measure = micros();
    // if (!artnetLoop()) { //~5ms
    //     idleAnimationLoop();
    // }
    // Serial.printf("\nAN+LED: %d us", micros() - measure);
    // measure = micros();
    // geqLoop(); //~60ms
    beatLoop();
    // Serial.println(analogRead(MIC_PIN));
    // Serial.printf("\nBeat: %d us", micros() - measure);
    // measure = micros();
}