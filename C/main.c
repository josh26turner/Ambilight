#include <fcntl.h>
#include <libconfig.h>
#include <string.h>
#include <termios.h>

#include "screen.h"
	
struct config_value
{
	char *name;
	int *value;
};

/**
 * Setting interface attributes - https://stackoverflow.com/a/6947758
 * @param fd - file descriptor
 * @param speed - speed of interface
 * @return - success: 0 or failure: -1
 */
int set_interface_attribs (int fd, int speed) {
	struct termios tty;

	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0) {
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
	
	tty.c_iflag &= ~IGNBRK; // disable break processing
	tty.c_lflag = 0; // no signaling chars, no echo, no canonical processing
	tty.c_oflag = 0; // no remapping, no delays
	tty.c_cc[VMIN]  = 0; // read doesn't block
	tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls, enable reading
	tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
	tty.c_cflag &= ~CSTOPB; // 1 stop bit

	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		return -1;
	}
	return 0;
}

/**
 * Loading the config from ~/.config/Ambilight/config
 * @param cnf - config to read in to
 * @return - success: 0 or failure: -1
 */
int load_config(struct config *cnf, char **portname) {
	char *filename;
	const char *str;
	
	config_t config;
	config_init(&config);

	filename = strcat(getenv("HOME"), "/.config/Ambilight/config");

	if (CONFIG_FALSE == config_read_file(&config, filename)) return 1;
	
	struct config_value cvs[10] = {
		{"brightness",				&cnf->brightness},
		{"leds_on_top", 			&cnf->leds_on_top},
		{"leds_on_side", 			&cnf->leds_on_side},
		{"pixels_to_process", 		&cnf->pixels_to_process},
		{"pixels_per_led_top", 		&cnf->pixels_per_led_top},
		{"pixels_per_led_side", 	&cnf->pixels_per_led_side},
		{"vertical_pixel_gap", 		&cnf->vertical_pixel_gap},
		{"vertical_pixel_count", 	&cnf->vertical_pixel_count},
		{"horizontal_pixel_gap", 	&cnf->horizontal_pixel_gap},
		{"horizontal_pixel_count", 	&cnf->horizontal_pixel_count}
	};

	for (int i = 0; i < 10; i ++) {
		if (CONFIG_FALSE == config_lookup_int(&config, cvs[i].name, cvs[i].value)) {
			fprintf(stderr, "Error in config file: %s\n", cvs[i].name);
			return -1;
		}
	}

	if (CONFIG_FALSE == config_lookup_string(&config, "arduino_device_name", &str)) return -1;

	*portname = malloc(strlen(str) + 1);

	strcpy(*portname, str);

	config_destroy(&config);
	return 0;
}

int main() {
	int fd;
	Display *d;
	unsigned t;
	unsigned char *values;
	int len;
	struct config cnf;
	char *portname;

	if (load_config(&cnf, &portname)) {
		return 1;
	}

	if (cnf.brightness > 100) {
		fprintf(stderr, "Can't have brightness greater than 100\n");
		return 1;
	}

	fd = open(portname, O_WRONLY | O_NOCTTY | O_SYNC);

	free(portname);

	if (fd < 0) {
		fprintf(stderr, "Error opening connection to Arduino\n"); 
		return 1;
	}

	set_interface_attribs(fd, B115200);

	len = 3 * (2 * cnf.leds_on_side + cnf.leds_on_top);
	t = (unsigned) time(NULL);

	d = XOpenDisplay((char *) NULL);
	values = malloc(sizeof(unsigned char) * (len + 2));
	values[0] = 'F';
	values[1] = 'u';

	while (True) {
		get_colors(d, values + 2, t, &cnf);
		write(fd, values, len + 2);
	}
}
