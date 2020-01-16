#include "config.h"
#include <FastLED.h>

#define DEBUG Serial

#define LED_PIN     12
#define NUM_LEDS    150
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
extern CRGB leds[];
#define INIT_STATE false
#define INIT_BRIGHTNESS  255
#define INIT_COLOR  0xFFFFFF
#define UPDATES_PER_SECOND 40

#define LED_MAPPING {72, 71} // maximum 170 LEDs per ArtNet universe
extern uint16_t ledMapping[];
extern uint8_t ledGroups;

#define ARTNET_TIMEOUT 5000 // start idle animation after 5s of Artnet missing

#define MIC_PIN 36 // GPIO36 = VP
#define BUILTIN_LED 2

#define BROKER_IP "mse-pi.lan"

extern bool otaRunning;