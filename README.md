# Ambillight
An ambient lighting solution for the X11 window enviroment in Linux using an Arduino to control the LEDs.

## Hardware
* An Arduino - I use a nano
* WS2812b LEDs on a flexible sticky strip - I use 60: 15 for the 2 sides and 30 on top.
* A powersupply for the LEDs - I use a 5V 4A one
* A USB cable is required to run to the Arduino from your computer too

## Software
Dependencies:
* Arduino IDE - with the `FastLED` library
* `gcc` 
  * `libconfig-dev` package will need to be installed if it doesn't come with packaged in `gcc` (such as Debian based distros - like Raspbian)
* An X11 window enviroment
  * `libx11-dev` package will need to be installed if it doesn't come with packaged in X11 (such as Debian based distros - like Raspbian)
* A Linux distribution for the naming of the devices

1. Run `Config-Generator.py` to create a config file. Also change the data pin and number of LEDs in `Arduino/ambilight/ambilight.ino` to the correct values.
1. Add your user to `uucp` so they have access to `/dev/ttyUSB[0-9]+`: 
    ```shell
    sudo usermod -a -G uucp "$USERNAME"
    ```
1. Copy the udev rule into the config folder: 
    ```shell
    sudo cp udev-rules/52-arduino.rules /etc/udev/rules.d
    ```
1. With the Arduino now being named `/dev/ttyUSB0` (after a reboot) the rest of the code should work if it is the only Arduino you are using. If you use multiple, or have a different naming system, you will need to adjust the device name in the config file. Load up the Arduino IDE and install the `FastLED` library
1. Upload the `Arduino/ambilight/ambilight.ino` program onto the Arduino
1. Compile the C program: 
    ```shell
    gcc C/main.c C/screen.c -l X11 -l config -o ambilight
    ``` 
    and run it using 
    ```shell
    ./ambilight
    ```

# Demo

![](https://github.com/josh26turner/ambilight/raw/master/Demo/Ambilight-1.jpg)

![](https://github.com/josh26turner/ambilight/raw/master/Demo/Ambilight-2.mp4)

![Rockstar Games, Inc](https://github.com/josh26turner/ambilight/raw/master/Demo/Ambilight-1.mp4 "GTA V splash screen")

# Bad delay  

If you're getting bad delay then go to the config file (`~/.config/Ambilight/config`) and decrease the `pixels_to_process` value. Find the highest value where you can't notice the delay, this should give good results.  
