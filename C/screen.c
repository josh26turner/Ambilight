#include "screen.h"

int randint(int min, int max) {
  return min + (rand() % (max - min));
}

void fillRGB(int i, int min_x, int max_x, int min_y, int max_y, int base, unsigned char values[], XColor c, Display* d, XImage* image) {
	int totalR = 0;
	int totalG = 0;
	int totalB = 0;

	for (int j = 0; j < pixels_to_process; j++) {
		int x = randint(min_x, max_x);
		int y = randint(min_y, max_y);

		c.pixel = XGetPixel(image, x, y);
		XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

		totalR += c.red/256;
		totalG += c.green/256;
		totalB += c.blue/256;
	}

	values[base + (i * 3) + 0] = totalR / pixels_to_process * brightness / 100;
	values[base + (i * 3) + 1] = totalG / pixels_to_process * brightness / 100;
	values[base + (i * 3) + 2] = totalB / pixels_to_process * brightness / 100;
}

void im(Display *d, unsigned char values[]){
	srand((unsigned) time(NULL)); // Initialising random
	
	XColor c;

	XImage *image;
	image = XGetImage(d, RootWindow(d, DefaultScreen(d)), 0, 0, horizontal_pixel_count, vertical_pixel_count, AllPlanes, ZPixmap);

	//Filling the left side

	for (int k = 0; k < leds_on_side; k++) {
		int i = leds_on_side - k - 1;

		fillRGB(i,                                                                  //Index
			0, pixels_per_led_side, vertical_pixel_gap + (k * pixels_per_led_side), //min_x, max_x
			vertical_pixel_gap + (k+1) * pixels_per_led_side,                       //min_y, nax_y
			0, values, c, d, image);                                                //base, values, XColor, Display, image
	}

	//Filling the top side

	for (int i = 0; i < leds_on_top; i++) {
		fillRGB(i,                                                                                            //Index
			horizontal_pixel_gap + i * pixels_per_led_top, horizontal_pixel_gap + (i+1) * pixels_per_led_top, //min_x, max_x
			0, pixels_per_led_top,                                                                            //min_y, nax_y
			leds_on_side * 3, values, c, d, image);                                                           //base, values, XColor, Display, image
	}

	//Filling the right side

	for (int i = 0; i < leds_on_side; i++) {
		fillRGB(i,                                                                                          //Index
			horizontal_pixel_count - pixels_per_led_side, horizontal_pixel_count - 1,                       //min_x, max_x
			vertical_pixel_gap + i * pixels_per_led_side, vertical_pixel_gap + (i+1) * pixels_per_led_side, //min_y, nax_y
			((leds_on_side + leds_on_top) * 3), values, c, d, image);                                       //base, values, XColor, Display, image
	}

	XDestroyImage(image);
}