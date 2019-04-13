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
  image = XGetImage(d, RootWindow(d, DefaultScreen(d)), 0, 0, horizontal_pixel_count, vertical_pixel_count, AllPlanes, ZPixmap);

  //Filling the left side

  for (int k = 0; k < leds_on_side; k++) {
    int i = 15 - k - 1;

    int totalR = 0;
    int totalG = 0;
    int totalB = 0;

    for (int j = 0; j < pixels_to_process; j++) {
      int x = randint(0, pixels_per_led_side);
      int y = randint(vertical_pixel_gap + (k * pixels_per_led_side), vertical_pixel_gap + (k+1) * pixels_per_led_side);

      c.pixel = XGetPixel(image, x, y);
      XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

      totalR += c.red/256;
      totalG += c.green/256;
      totalB += c.blue/256;
    }

    values[(i * 3) + 0] = totalR / pixels_to_process;
    values[(i * 3) + 1] = totalG / pixels_to_process;
    values[(i * 3) + 2] = totalB / pixels_to_process;
  }

  //Filling the top side

  for (int i = 0; i < leds_on_top; i++) {
    int totalR = 0;
    int totalG = 0;
    int totalB = 0;

    for (int j = 0; j < pixels_to_process; j++) {
      int x = randint(horizontal_pixel_gap + i * pixels_per_led_top, horizontal_pixel_gap + (i+1) * pixels_per_led_top);
      int y = randint(0, pixels_per_led_top);

      c.pixel = XGetPixel(image, x, y);
      XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

      totalR += c.red/256;
      totalG += c.green/256;
      totalB += c.blue/256;
    }

    values[(leds_on_side * 3) + (i * 3) + 0] = totalR / pixels_to_process;
    values[(leds_on_side * 3) + (i * 3) + 1] = totalG / pixels_to_process;
    values[(leds_on_side * 3) + (i * 3) + 2] = totalB / pixels_to_process;
  }

  //Filling the right side

  for (int i = 0; i < leds_on_side; i++) {
    int totalR = 0;
    int totalG = 0;
    int totalB = 0;

    for (int j = 0; j < pixels_to_process; j++) {
      int x = randint(horizontal_pixel_count - pixels_per_led_side, horizontal_pixel_count - 1);
      int y = randint(vertical_pixel_gap + i * pixels_per_led_side, vertical_pixel_gap + (i+1) * pixels_per_led_side);

      c.pixel = XGetPixel(image, x, y);
      XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

      totalR += c.red/256;
      totalG += c.green/256;
      totalB += c.blue/256;
    }

    values[((leds_on_side + leds_on_top) * 3) + (i * 3) + 0] = totalR / pixels_to_process;
    values[((leds_on_side + leds_on_top) * 3) + (i * 3) + 1] = totalG / pixels_to_process;
    values[((leds_on_side + leds_on_top) * 3) + (i * 3) + 2] = totalB / pixels_to_process;
  }

  XDestroyImage(image);
}