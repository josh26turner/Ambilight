gcc C/main.c C/screen.c -l X11 -l config -o ambilight
gcc -l config C/background.c -o ambilight-color

sudo cp ambilight /usr/bin/
sudo cp ambilight-color /usr/bin/