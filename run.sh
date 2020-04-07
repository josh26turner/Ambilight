#!/bin/bash

if [[ "$(ps -A -f | grep ambilight | wc -l)" = "1" ]]; then
	ambilight &
else
	killall ambilight
    sleep 1
	ambilight_off
fi