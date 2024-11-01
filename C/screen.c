#include "screen.h"

/**
 * Generate a random number
 */
int randint(int min, int max) {
  return min + (rand() % (max - min));
}

/**
 * Find the average color of a block
 * @param i index to write colour to in values
 * @param min_x minimum x value for a pixel
 * @param max_x maximum x value for a pixel
 * @param min_y minimum y value for a pixel
 * @param max_y maximum y value for a pixel
 * @param values array of bytes to write color to
 * @param d The X display to query
 * @param image The snapshot of the display to query
 * @param pixels_to_process The number of pixels to use for an average
 * @param brightness A percentage to scale the color by
 */
void fillRGB(int i, int min_x, int max_x, int min_y, int max_y, unsigned char values[], 
			 Display* d, XImage* image, int pixels_to_process, int brightness) {
	int total[3] = {0, 0, 0};
	int x, y;
	XColor c;

	for (int j = 0; j < pixels_to_process; j++) {
		x = randint(min_x, max_x);
		y = randint(min_y, max_y);

		c.pixel = XGetPixel(image, x, y);
		XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

		total[0] += c.red/256;
		total[1] += c.green/256;
		total[2] += c.blue/256;
	}

	for (int j = 0; j < 3; j ++) 
		values[i+j] = total[j] / pixels_to_process * brightness / 100;
}

/**
 * Get the RGB colors for the display
 * @param d the X display to get the colors from
 * @param values the array to write the colors to
 * @param t a random seed
 * @param cnf config
 */
void get_colors(Display *d, unsigned char *values, unsigned t, struct config *cnf) {
	XImage *image;

	srand(t); // Initialising random

	image = XGetImage(d, RootWindow(d, DefaultScreen(d)), 0, 0, cnf->horizontal_pixel_count, cnf->vertical_pixel_count, AllPlanes, ZPixmap);

	//Filling the left side
	for (int k = 0; k < cnf->leds_on_side; k++) {
		int i = cnf->leds_on_side - k - 1;

		fillRGB(3 * i,
				0, cnf->pixels_per_led_side,
				cnf->vertical_pixel_gap + (k * cnf->pixels_per_led_side), cnf->vertical_pixel_gap + (k+1) * cnf->pixels_per_led_side,
				values, d, image, cnf->pixels_to_process, cnf->brightness);
	}

	//Filling the top side
	for (int i = 0; i < cnf->leds_on_top; i++) {
		fillRGB(cnf->leds_on_side * 3 + 3 * i,
				cnf->horizontal_pixel_gap + i * cnf->pixels_per_led_top, cnf->horizontal_pixel_gap + (i+1) * cnf->pixels_per_led_top,
				0, cnf->pixels_per_led_top,
				values, d, image, cnf->pixels_to_process, cnf->brightness);
	}

	//Filling the right side
	for (int i = 0; i < cnf->leds_on_side; i++) {
		fillRGB((cnf->leds_on_side + cnf->leds_on_top + i) * 3,
				cnf->horizontal_pixel_count - cnf->pixels_per_led_side, cnf->horizontal_pixel_count - 1,
				cnf->vertical_pixel_gap + i * cnf->pixels_per_led_side, cnf->vertical_pixel_gap + (i+1) * cnf->pixels_per_led_side,
				values, d, image, cnf->pixels_to_process, cnf->brightness);
	}

	//Filling the bottom side
	for (int i = 0; i < cnf->leds_on_top; i++) {
		fillRGB((cnf->leds_on_side * 2 + cnf->leds_on_top * 2 - i - 1) * 3,
				cnf->horizontal_pixel_gap + i * cnf->pixels_per_led_top, cnf->horizontal_pixel_gap + (i+1) * cnf->pixels_per_led_top,
				cnf->vertical_pixel_count - cnf->pixels_per_led_top, cnf->vertical_pixel_count - 1,
				values, d, image, cnf->pixels_to_process, cnf->brightness);
	}

	XDestroyImage(image);
}