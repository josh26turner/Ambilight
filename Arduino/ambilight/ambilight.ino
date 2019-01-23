#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2 // or D2

CRGB leds[NUM_LEDS];

void setup() { 
     FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}
 
void loop() { 
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::White; 
    FastLED.show(); 
  }
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Red; 
    FastLED.show(); 
  }
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(0,255,0); 
    FastLED.show(); 
  }
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Blue; 
    FastLED.show(); 
  }
    FastLED.show(); 
    delay(30); 
}
