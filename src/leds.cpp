#include "leds.h"
#include "ledAnimation.h"

const char* effectNames[] = {
    "static",
    "artnet",
    "colorpalette",
};
int numEffects = sizeof(effectNames) / sizeof(effectNames[0]);

CRGB leds[NUM_LEDS];
uint16_t ledMapping[] = LED_MAPPING;
uint8_t ledGroups = sizeof(ledMapping) / sizeof(ledMapping[0]);

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



ledState_t curLedState = {
    INIT_STATE,
    INIT_BRIGHTNESS,
    INIT_COLOR,
    fx_colorpalette,
    5000
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
    setAnimationBaseColor(color);
}

void setLedAnimationTime(uint32_t transition) {
    curLedState.animationTime = transition;
}

void setLedEffect(String effect) {
    fx_t fx = effectStringToFx(effect);
    if(fx > -1) {
        curLedState.effect = fx;
    }
    else {
        uint8_t anim = animationNameToId(effect.c_str());
        if(anim > -1) {
            setAnimation(anim);
            curLedState.effect = fx_animation;
        }
    }
}

const char* getCurLedEffectName() {
    if(curLedState.effect != fx_animation) {
        return effectNames[curLedState.effect];
    }
    else {
        return getCurAnimationName();
    }
}

void setLedState(ledState_t state) {
    curLedState.state = state.state;
    curLedState.brightness = state.brightness;
    curLedState.color = state.color;
    curLedState.effect = state.effect;
    curLedState.animationTime = state.animationTime;

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

void initLeds() {
    //leds.begin();
    idleAnimationSetup();
    //applyLedState();
    initAnimation(NUM_LEDS);
}

uint32_t lastLedUpdate = 0, lastAnimationStep = 0;

void loopLeds() {
    if(millis() - lastLedUpdate > (1000 / UPDATES_PER_SECOND)) {
        lastLedUpdate = millis();
        // simple rainbow fade animation

        if(!curLedState.state) {
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            FastLED.show();
        }
        else {
            switch(curLedState.effect) {
                case fx_static:
                    fill_solid(leds, NUM_LEDS, CRGB(curLedState.color));
                    FastLED.show();
                    break;
                case fx_artnet:
                    artnetLoop();
                    break;
                case fx_colorpalette:
                    idleAnimationLoop();
                    break;
                case fx_animation:
                    if(millis() - lastAnimationStep > (curLedState.animationTime / 256)) {
                        // TODO: not sure if I want to skip animation steps, or freeze when timing is not met
                        lastAnimationStep = millis();
                        stepAnimation();
                        FastLED.show();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}