#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2 // or D2

CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i ++){
    leds[i] = CRGB(0,0,0);
  }
  FastLED.show();
}

void loop() { 
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[minusOne(i)].r = r;
    leds[minusOne(i)].g = g;
    leds[minusOne(i)].b = b;
  }
  FastLED.show();
}

int minusOne(int i) {
  if (i > 0) return i - 1;
  else return NUM_LEDS - 1;
}
