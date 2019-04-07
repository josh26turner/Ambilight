#ifndef SCREEN_H
#define SCREEN_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int leds_on_top;
int leds_on_side;
int pixels_per_led;
int pixels_to_process;
int vertical_pixel_gap;
int vertical_pixel_count;
int horizontal_pixel_gap;
int horizontal_pixel_count;

void im(Display* d, unsigned char values[]);

#endif
