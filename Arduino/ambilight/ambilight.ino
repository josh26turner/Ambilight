#include <FastLED.h>
#define NUM_LEDS 62
#define DATA_PIN 2 // or D2

CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS - 2; i ++){
    leds[i] = CRGB(255,0,0);
  }
  FastLED.show();
}
 
void loop() { 
  if (Serial.available() > 0){
    for (int i = 0; i < NUM_LEDS; i ++){
      if (Serial.available() > 0){
        char r = Serial.read();
        char g = Serial.read();
        char b = Serial.read();
        int index = i - 2;
        if (index < 0) index += NUM_LEDS;
        if (index < NUM_LEDS - 2) leds[index] = CRGB(r, g, b);
        FastLED.show();
      }
    }
  }
}
