#include <FastLED.h>
#include "globals.h"
#include "idleAnimation.h"
#include "artnet.h"

typedef enum effect {
    fx_static           = 0,
    fx_artnet           = 1,
    fx_colorpalette     = 2,
    fx_rainbow          = 3,
} fx_t;


const char* effectNames[] = {
    "static",
    "artnet",
    "colorpalette",
    "rainbow",
};
int numEffects = sizeof(effectNames) / sizeof(effectNames[0]);


fx_t effectStringToFx(String effectName) {
    fx_t result = (fx_t)-1;
    for(int i = 0; i < numEffects; i++) {
        if(effectName == String(effectNames[i])) {
            result = (fx_t)i;
            break;
        }
    }
    return result;
}


typedef struct {
    bool state;
    uint8_t brightness;
    uint32_t color;
    fx_t effect;
} ledState_t;

ledState_t curLedState = {
    INIT_STATE,
    INIT_BRIGHTNESS,
    INIT_COLOR,
    fx_colorpalette
};




/*void applyLedState() {
    FastLED.setBrightness(curLedState.brightness);
    if(curLedState.effect == "static") {
        fill_solid(leds, NUM_LEDS, CRGB(curLedState.color));
        FastLED.show();
    }
}*/

void setLedState(bool state) {
    curLedState.state = state;
}

void setLedBrightness(uint8_t brightness) {
    curLedState.brightness = brightness;
    FastLED.setBrightness(brightness);
}

void setLedColor(uint32_t color) {
    curLedState.color = color;
}

void setLedEffect(String effect) {
    fx_t a = effectStringToFx(effect);
    if(a > -1) {
        curLedState.effect = a;
    }
}

void setLedState(ledState_t state) {
    curLedState.state = state.state;
    curLedState.brightness = state.brightness;
    curLedState.color = state.color;
    curLedState.effect = state.effect;

    //applyLedState();
}

ledState_t getLedState() {
    return curLedState;
}

uint8_t lastPercent = 0;

void setLedProgress(uint8_t percent) {
    if(lastPercent != percent) {
        lastPercent = percent;
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        fill_solid(leds, (NUM_LEDS * percent) / 100, CRGB::White);
        FastLED.show();
    }
}

void rainbowAnim() {
    uint32_t now = millis();
    for(uint16_t i = 0; i < NUM_LEDS; i++) {
        uint16_t timeOffset = (255UL * (now % 5000)) / 5000;
        uint16_t ledOffset = (255UL * i) / (NUM_LEDS);
        uint8_t hue = (timeOffset + ledOffset) % 256;
        fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));
    }
    FastLED.show();
}

void initLeds() {
    //leds.begin();
    idleAnimationSetup();
    //applyLedState();
}

uint32_t lastLedUpdate = 0;
uint16_t animationStep = 0;

void loopLeds() {
    if(millis() - lastLedUpdate > (1000 / UPDATES_PER_SECOND)) {
        lastLedUpdate = millis();
        // simple rainbow fade animation

        if(!curLedState.state) {
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            FastLED.show();
        }
        else {
            //Serial.print(curLedState.effect);
            switch(curLedState.effect) {
                case fx_static:
                    //Serial.println("static");
                    fill_solid(leds, NUM_LEDS, CRGB(curLedState.color));
                    FastLED.show();
                    break;
                case fx_artnet:
                    //Serial.println("artnet");
                    artnetLoop();
                    break;
                case fx_colorpalette:
                    //Serial.println("colorpalette");
                    idleAnimationLoop();
                    break;
                case fx_rainbow:
                    rainbowAnim();
                    break;
                default:
                    break;
            }
        }
    }
}