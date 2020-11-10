#!/bin/bash

echo "Adding this user to the UUCP group"
sudo usermod -a -G uucp "$USER"

echo "Copying the naming rules to udev"
sudo cp udev-rule/52-arduino.rules /etc/udev/rules.d

echo "Generating the configuration files"
python3 Config-Generator.py

echo "Checking required libraries are there"

if [[ ! $(ls /usr/include/libconfig.h 2> /dev/null | wc -l) = "1" ]]; then
    echo "libconfig.h is missing, please install libconfig-dev if you are using Debian based systems"
    exit 1
fi

if [[ $(ls /usr/include/X11 2> /dev/null | wc -l) = "0" ]]; then
    echo "X11 libraries are missing, please install libx11-dev if you are using Debian based systems"
    exit 1
fi

echo "Compiling the code"
./update.sh

echo "A reboot is required to rename the Arduino in the required way"