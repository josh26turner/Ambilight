#include <fcntl.h>
#include <libconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/*
 * https://stackoverflow.com/a/6947758
 */
int set_interface_attribs (int fd, int speed) {
	struct termios tty;

	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0) {
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	
	tty.c_iflag &= ~IGNBRK; // disable break processing
	tty.c_lflag = 0; // no signaling chars, no echo, no canonical processing
	tty.c_oflag = 0; // no remapping, no delays
	tty.c_cc[VMIN]  = 0; // read doesn't block
	tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls, enable reading
	tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
	tty.c_cflag &= ~CSTOPB; // 1 stop bit

	// This option stops the reseting of the Arduino after the program finishes
	tty.c_cflag &= ~HUPCL; // don't hang up after process shuts down
	

	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		return -1;
	}
	return 0;
}

/*
 * Loading the config from ~/.config/Ambilight/config
 */
int load_config(unsigned char *rgb, char **portname) {
	char *filename;
	const char *str;
	config_t config;
	config_setting_t *setting;
	int count;

	config_init(&config);

	filename = strcat(getenv("HOME"), "/.config/Ambilight/config");

	if (CONFIG_FALSE == config_read_file(&config, filename)) return 1;

	if (CONFIG_FALSE == config_lookup_string(&config, "arduino_device_name", &str)) return 1;

	setting = config_lookup(&config, "background");
	if (setting != NULL) {
		count = config_setting_length(setting);

		if (count != 3) {
			return 1;
		}
		
		for (int i = 0; i < 3; i ++) rgb[i] = config_setting_get_int_elem(setting, i);
	}

	*portname = malloc(strlen(str) + 1);

	strcpy(*portname, str);

	config_destroy(&config);
	return 0;
}

void send_values(int fd, unsigned char *rgb) {
	unsigned char *values = malloc(sizeof(unsigned char) * 5);

	values[0] = 'B';
	values[1] = 'k';
	memcpy(values + 2, rgb, 3);

	char buf[10];
	while (1) {
		write(fd, values, 5);

		if (read(fd, buf, 2) == 2) {
			if (strncmp(buf, "Do", 2) == 0) {
				return;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	unsigned char rgb[3];
	int fd;
	char *portname;

	if (load_config(rgb, &portname)) {
		fprintf(stderr, "Error in config file\n");
		free(portname);
		return 1;
	}

	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);

	free(portname);

	if (fd < 0) {
		fprintf(stderr, "Error opening connection to Arduino\n"); 
		return 1;
	}

	if (argc == 4) {
		for (int i = 0; i < 3; i ++) rgb[i] = atoi(argv[i+1]);
	}

	set_interface_attribs(fd, B115200);

	send_values(fd, rgb);
}