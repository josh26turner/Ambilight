//
// Created by josh on 2/15/19.
//

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LEDS 60
#define PIXELS_TO_PROCESS 50
#define PIXELS_PER_LED 80
#define HORIZONTAL_PIXEL_GAP 80
#define VERTICAL_PIXEL_GAP 120
#define HORIZONTAL_PIXEL_COUNT 2560

int randint(int min, int max) {
    return min + (rand() % (max - min));
}

unsigned char * im(Display *d){
    unsigned char * values = malloc(sizeof(char) * (NUM_LEDS * 3));
    srand((unsigned) time(NULL));


    XColor c;

    XImage *image;
    image = XGetImage(d, RootWindow(d, DefaultScreen(d)), 0, 0, 2560, 1440, AllPlanes, XYPixmap);

    //Filling the left side

    for (int k = 0; k < 15; k++) {
        int i = 15 - k -1;
        int totalR = 0;
        int totalG = 0;
        int totalB = 0;

        for (int j = 0; j < PIXELS_TO_PROCESS; j++) {
            int x = randint(0, PIXELS_PER_LED);
            int y = randint(VERTICAL_PIXEL_GAP + (k * PIXELS_PER_LED), VERTICAL_PIXEL_GAP + (k+1) * PIXELS_PER_LED);

            c.pixel = XGetPixel(image, x, y);
            XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

            totalR += c.red/256;
            totalG += c.green/256;
            totalB += c.blue/256;
        }

        values[(i * 3) + 0] = totalR / PIXELS_TO_PROCESS;
        values[(i * 3) + 1] = totalG / PIXELS_TO_PROCESS;
        values[(i * 3) + 2] = totalB / PIXELS_TO_PROCESS;
    }

    //Filling the top side

    for (int i = 0; i < 30; i++) {
        int totalR = 0;
        int totalG = 0;
        int totalB = 0;

        for (int j = 0; j < PIXELS_TO_PROCESS; j++) {
            int x = randint(HORIZONTAL_PIXEL_GAP + i * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED);
            int y = randint(0, PIXELS_PER_LED);

            c.pixel = XGetPixel(image, x, y);
            XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

            totalR += c.red/256;
            totalG += c.green/256;
            totalB += c.blue/256;
        }

        values[(15 * 3) + (i * 3) + 0] = totalR / PIXELS_TO_PROCESS;
        values[(15 * 3) + (i * 3) + 1] = totalG / PIXELS_TO_PROCESS;
        values[(15 * 3) + (i * 3) + 2] = totalB / PIXELS_TO_PROCESS;
    }

    //Filling the right side

    for (int i = 0; i < 15; i++) {
        int totalR = 0;
        int totalG = 0;
        int totalB = 0;

        for (int j = 0; j < PIXELS_TO_PROCESS; j++) {
            int x = randint(HORIZONTAL_PIXEL_COUNT - PIXELS_PER_LED, HORIZONTAL_PIXEL_COUNT - 1);
            int y = randint(VERTICAL_PIXEL_GAP + i * PIXELS_PER_LED, VERTICAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED);

            c.pixel = XGetPixel(image, x, y);
            XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

            totalR += c.red/256;
            totalG += c.green/256;
            totalB += c.blue/256;
        }

        values[(45 * 3) + (i * 3) + 0] = totalR / PIXELS_TO_PROCESS;
        values[(45 * 3) + (i * 3) + 1] = totalG / PIXELS_TO_PROCESS;
        values[(45 * 3) + (i * 3) + 2] = totalB / PIXELS_TO_PROCESS;
    }

    XFree(image);

    return values;
}