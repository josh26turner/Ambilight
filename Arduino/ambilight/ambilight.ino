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
  int c = 0;
  
  while (Serial.available() <= 0) ;
  
  if ((c = Serial.read()) != 'F') return;

  while (Serial.available() <= 0);
  
  if ((c = Serial.read()) != 'u') return;
  
	// Read the transmission data and set LED values
	for (uint8_t i = 0; i < NUM_LEDS; i++) {
		byte r, g, b;

    while (Serial.available() <= 0);
		r = Serial.read();
    
    while (Serial.available() <= 0);
		g = Serial.read();
    
    while (Serial.available() <= 0);
		b = Serial.read();

		leds[i].r = r;
		leds[i].g = g;
		leds[i].b = b;
	}
 
	FastLED.show();
}
