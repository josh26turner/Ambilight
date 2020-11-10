#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xutil.h>

extern int brightness;

extern int leds_on_top;
extern int leds_on_side;

extern int pixels_to_process;

extern int pixels_per_led_top;
extern int pixels_per_led_side;

extern int vertical_pixel_gap;
extern int vertical_pixel_count;

extern int horizontal_pixel_gap;
extern int horizontal_pixel_count;

void im(Display* d, unsigned char values[], unsigned t);

#endif
