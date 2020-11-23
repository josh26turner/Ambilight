#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xutil.h>

struct config
{
    int brightness;

    int leds_on_top;
    int leds_on_side;

    int pixels_to_process;

    int pixels_per_led_top;
    int pixels_per_led_side;

    int vertical_pixel_gap;
    int vertical_pixel_count;

    int horizontal_pixel_gap;
    int horizontal_pixel_count;
};

/**
 * Get the RGB colors for the display
 * @param d the X display to get the colors from
 * @param values the array to write the colors to
 * @param t a random seed
 * @param cnf config
 */
void get_colors(Display* d, unsigned char *values, unsigned t, struct config *cnf);

#endif
