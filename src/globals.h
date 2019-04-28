#include <FastLED.h>

#define LED_PIN     15
#define NUM_LEDS    150
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
extern CRGB leds[];

#define ARTNET_TIMEOUT 5000 // start idle animation after 5s of Artnet missing

#define MIC_PIN 36 // GPIO36 = VP
#define BUILTIN_LED 2