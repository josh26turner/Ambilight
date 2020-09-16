#include <errno.h>
#include <fcntl.h>
#include <libconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int leds_on_top;
int leds_on_side;

char *portname;

/*
 * Can't do project without a stackoverflow reference...
 * https://stackoverflow.com/a/6947758
 */
int set_interface_attribs (int fd, int speed, int parity) {
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0) {
		//error_message ("error %d from tcgetattr", errno);
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
									// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
									// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	//tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		//error_message ("error %d from tcsetattr", errno);
		return -1;
	}
	return 0;
}

/*
 * Loading the config from ~/.config/Ambilight/config
 */
int load_config() {
	config_t config;
	config_init(&config);

	char *filename = strcat(getenv("HOME"), "/.config/Ambilight/config");

	if (CONFIG_FALSE == config_read_file(&config, filename)) return 1;

	if (CONFIG_FALSE == config_lookup_int(&config, "leds_on_top" , &leds_on_top)) return 1;
	if (CONFIG_FALSE == config_lookup_int(&config, "leds_on_side", &leds_on_side)) return 1;

	const char *str;

	if (CONFIG_FALSE == config_lookup_string(&config, "arduino_device_name", &str)) return 1;

	portname = malloc(strlen(str) + 1);

	strncpy(portname, str, strlen(str) + 1);

	config_destroy(&config);
	return 0;
}

int main() {
	if (load_config()) {
		fprintf(stderr, "Error in config file\n");
		free(portname);
		return 1;
	}

	int fd = open(portname, O_WRONLY | O_NOCTTY | O_SYNC);

	free(portname);

	if (fd < 0) {
		fprintf(stderr, "Error opening connection to Arduino\n"); 
		return 1;
	}

	set_interface_attribs(fd, B115200, 0);

	int len = 3 * (2 * leds_on_side + leds_on_top);

	unsigned char *values __attribute__((aligned(64))) = malloc(sizeof(unsigned char) * (len + 2));
	values[0] = 'F';
	values[1] = 'u';

    memset(values + 2, 0, len);

    write(fd, values, len + 2);
}