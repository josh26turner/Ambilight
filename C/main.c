//
// Created by josh on 2/16/19.
//

#include "main.h"

const char *portname;

/*
 * Can't do project without a stackoverflow reference...
 * https://stackoverflow.com/a/6947758
 */
int set_interface_attribs (int fd, int speed, int parity)
{
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0)
  {
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
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
  {
    //error_message ("error %d from tcsetattr", errno);
    return -1;
  }
  return 0;
}

/*
 * Loading the config
 */
int load_config() {
  config_t config;
  config_init(&config);
  char *filename = strcat(getenv("HOME"), "/.config/Ambilight/config");

  int success = config_read_file(&config, filename);

  if (success == CONFIG_FALSE) {
    printf("Error in reading config file: %s\n", config_error_file(&config));
    printf(config_error_text(&config));
    printf("\nOn line: %d", config_error_line(&config));
    printf("\n");
    return 1;
  }

  config_lookup_int(&config, "leds_on_top", &leds_on_top);
  config_lookup_int(&config, "leds_on_side", &leds_on_side);

  config_lookup_int(&config, "pixels_to_process", &pixels_to_process);
  
  config_lookup_int(&config, "pixels_per_led_top", &pixels_per_led_top);
  config_lookup_int(&config, "pixels_per_led_side", &pixels_per_led_side);

  config_lookup_int(&config, "vertical_pixel_gap", &vertical_pixel_gap);
  config_lookup_int(&config, "vertical_pixel_count", &vertical_pixel_count);

  config_lookup_int(&config, "horizontal_pixel_gap", &horizontal_pixel_gap);
  config_lookup_int(&config, "horizontal_pixel_count", &horizontal_pixel_count);

  config_lookup_string(&config, "arduino_device_name", &portname);

  return 0;
}

int main() {
  if (load_config()) return 1;

  Display *d = XOpenDisplay((char *) NULL);

  int fd = open(portname, O_WRONLY | O_NOCTTY | O_SYNC);

  if (fd < 0) {
    printf("ERROR"); 
    return 1;
  }

  set_interface_attribs(fd, B115200, 0);

  int len = 3 * (2 * leds_on_side + leds_on_top);

  while (True) {
    unsigned char *values = malloc(sizeof(char) * len);
    im(d, values);
    write(fd, values, len);
    free(values);
  }
}