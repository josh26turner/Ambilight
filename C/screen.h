#ifndef SCREEN_H
#define SCREEN_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LEDS                60
#define LEDS_ON_TOP             30
#define LEDS_ON_SIDE            15
#define PIXELS_PER_LED          80
#define PIXELS_TO_PROCESS       50
#define VERTICAL_PIXEL_GAP      120
#define VERTICAL_PIXEL_COUNT    1440
#define HORIZONTAL_PIXEL_GAP    80
#define HORIZONTAL_PIXEL_COUNT  2560

void im(Display* d, unsigned char values[]);

#endif
