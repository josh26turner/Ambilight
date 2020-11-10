#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

char *portname;
char r=0, g=0, b=0;

//
int serialport_read_until(int fd, char* buf, char until, int buf_max, int timeout)
{
    char b[1];  // read expects an array, so we give it a 1-byte array
    int i=0;
    do { 
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 1 * 1000 );  // wait 1 msec try again
            timeout--;
            if( timeout==0 ) return -2;
            continue;
        }
        // printf("serialport_read_until: i=%d, n=%d b='%c'\n",i,n,b[0]); // debug
        buf[i] = b[0]; 
        i++;
    } while( b[0] != until && i < buf_max && timeout>0 );

    buf[i] = 0;  // null terminate the string
    return 0;
}

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
	config_setting_t *setting;
	config_init(&config);

	char *filename = strcat(getenv("HOME"), "/.config/Ambilight/config");

	if (CONFIG_FALSE == config_read_file(&config, filename)) return 1;

	const char *str;

	if (CONFIG_FALSE == config_lookup_string(&config, "arduino_device_name", &str)) return 1;
	setting = config_lookup(&config, "background");
	if (setting != NULL) {
		int count = config_setting_length(setting);
		if (count != 3) {
			return 1;
		}
		r = config_setting_get_int_elem(setting, 0);
		g = config_setting_get_int_elem(setting, 1);
		b = config_setting_get_int_elem(setting, 2);
	}
	// if (CONFIG_FALSE == config_lookup(&config, "arduino_device_name", &str)) return 1;

	portname = malloc(strlen(str) + 1);

	strncpy(portname, str, strlen(str) + 1);

	config_destroy(&config);
	return 0;
}

int main(int argc, char *argv[]) {
	if (load_config()) {
		fprintf(stderr, "Error in config file\n");
		free(portname);
		return 1;
	}

	int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);

	free(portname);

	if (fd < 0) {
		fprintf(stderr, "Error opening connection to Arduino\n"); 
		return 1;
	}

	if (argc == 4) {
		r = atoi(argv[1]);
		g = atoi(argv[2]);
		b = atoi(argv[3]);
	}
	if (r > 254 || g > 254 || b > 254) {
		printf("Cannot set lights to value > 254\n");
		exit(1);
	}

	set_interface_attribs(fd, B115200, 0);

	unsigned char *values = malloc(sizeof(unsigned char) * 5);
	values[0] = 'B';
	values[1] = 'k';
	values[2] = r;
	values[3] = g;
	values[4] = b;

	char buf[10];
	while (1) {
		int n = write(fd, values, 5);

		n = read(fd, buf, 2);
		if (n == 2) {
			if (strncmp(buf, "Do", 2) == 0) {
				exit(0);
			}
		}
	}
}