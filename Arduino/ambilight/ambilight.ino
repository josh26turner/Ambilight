#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2 // or D2

CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < 15; i ++){
    leds[i] = CRGB(0,0,255);
  }
  for (int i = 15; i < 45; i ++){
    leds[i] = CRGB(255,0,0);
  }
  for (int i = 45; i < 60; i ++){
    leds[i] = CRGB(0,255,0);
  }
  FastLED.show();
}

void loop() { 
}

void serialEvent() {
  int len = NUM_LEDS * 3;
  byte rawRGB [len];
  
  int b = Serial.readBytes(rawRGB, len + 5);
  
  for (int i = 0; i < NUM_LEDS; i ++) {
    leds[i] = CRGB(rawRGB[(i * 3) + rawRGB[0]], rawRGB[(i * 3) + rawRGB[0] + 1], rawRGB[(i * 3) + rawRGB[0] + 2]);
  }
  FastLED.show();
}
