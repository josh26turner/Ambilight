#!/bin/bash
ps -A | grep ambilight
if [[ "$(ps -A | grep ambilight | wc -l)" = "0" ]]; then
	echo "Start"
	ambilight &
else
	echo "Kill"
	killall ambilight
	sleep 1
	ambilight-color
fi