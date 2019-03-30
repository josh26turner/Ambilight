//
// Created by josh on 2/15/19.
//
#include "screen.h"

int randint(int min, int max) {
  return min + (rand() % (max - min));
}

void im(Display *d, unsigned char values[]){
  srand((unsigned) time(NULL));

  XColor c;

  XImage *image;
  image = XGetImage(d, RootWindow(d, DefaultScreen(d)), 0, 0, HORIZONTAL_PIXEL_COUNT, VERTICAL_PIXEL_COUNT, AllPlanes, ZPixmap);

  //Filling the left side

  for (int k = 0; k < LEDS_ON_SIDE; k++) {
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

  for (int i = 0; i < LEDS_ON_TOP; i++) {
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

  for (int i = 0; i < LEDS_ON_SIDE; i++) {
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

  XDestroyImage(image);
}