#include "config.h"
#define FASTLED_INTERNAL // disable the FastLED pragma messages
#include <FastLED.h>

#define DEBUG Serial

// #define DEVICE_ARC
#define DEVICE_PALMTREE

#ifdef DEVICE_ARC
    #define LED_PIN     12
    #define NUM_LEDS    150
    #define LED_MAPPING {72, 71} // maximum 170 LEDs per ArtNet universe
    #define INIT_STATE  false
    #define MY_HOSTNAME "MSE-LED-Arc"
    #define COLOR_ORDER BRG
#endif
#ifdef DEVICE_PALMTREE
    #define LED_PIN     12
    #define NUM_LEDS    26
    #define LED_MAPPING {NUM_LEDS}
    #define INIT_STATE true
    #define MY_HOSTNAME "MSE-LED-Palmtree"
    #define COLOR_ORDER RGB
#endif

#define BRIGHTNESS  255
#define LED_TYPE    WS2811
extern CRGB leds[];
#define INIT_BRIGHTNESS  255
#define INIT_COLOR  0xFFFFFF
#define UPDATES_PER_SECOND 60

#define ANIMATION_USE_FASTLED

extern uint16_t ledMapping[];
extern uint8_t ledGroups;

#define ARTNET_TIMEOUT 5000 // start idle animation after 5s of Artnet missing

#define MIC_PIN 36 // GPIO36 = VP
#define BUILTIN_LED 2

#define BROKER_IP "mse-pi.makerspace.experimenta.science"

extern bool otaRunning;


#if defined(DEVICE_ARC) && defined(DEVICE_PALMTREE)
    #error Please only choose one device configuration at a time
#endif