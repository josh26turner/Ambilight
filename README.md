# Ambillight
An ambient lighting solution for the X11 window enviroment in Linux using an Arduino to control the LEDs.

## Hardware
* An Arduino Nano
* 60 WS2812b LEDs on a flexible sticky strip
* A 5V 4A powersupply for the LEDs
* A USB cable is required to run to the Arduino from your computer too

## Software
Dependencies:
* Arduino IDE - with the `FastLED` library
* `gcc` 
* An X11 window enviroment
* A Linux distribution for the naming of the devices

1. Change the resolution, number of LEDs and other configurations specific to your build in `C/main.c`
1. Add your user to `uucp` so they have access to `/dev/ttyUSB1`: `# usermod -a -G uucp "$USERNAME"`
1. Copy the udev rule into the config folder: `# cp udev-rules/52-arduino.rules /etc/udev/rules.d`
1. With the Arduino now being named `/dev/ttyUSBX` (after a reboot) the rest of the code should work if it is the only Arduino you are using. If you use multiple, or have a different naming system, you will need to adjust the device name line 58 of `C/main.c` to the correct device name. Load up the Arduino IDE and install the `FastLED` library
1. Upload the `Arduino/ambilight/ambilight.ino` program onto the Arduino
1. Compile the C program `gcc C/main.c -l X11 -o ambilight` and run it using `./ambilight`

