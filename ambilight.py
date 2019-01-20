import subprocess
import random
import time
from serial import Serial
from PIL import Image

PIXELS_TO_PROCESS = 100
PIXELS_PER_LED = 80
HORIZONTAL_PIXEL_GAP = 80
VERTICAL_PIXEL_GAP = 120
HORIZONTAL_PIXEL_COUNT = 2560
VERTICAL_PIXEL_COUNT = 1440

leftLEDArray = [(0, 0, 0)] * 15
topLEDArray = [(0, 0, 0)] * 30
rightLEDArray = [(0, 0, 0)] * 15

start = time.time()

while True:
    subprocess.call('./scrot.sh')

    im = Image.open('screenshots/img.png')
    pix = im.load()

    # FILLING IN THE TOP LEDS

    totalR = 0
    totalG = 0
    totalB = 0

    for i in range(30):
        for j in range(PIXELS_TO_PROCESS):
            x = random.randint(HORIZONTAL_PIXEL_GAP + i * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED)
            y = random.randint(0, PIXELS_PER_LED)
            totalR += pix[x, y][0]
            totalG += pix[x, y][1]
            totalB += pix[x, y][2]

        totalR //= 10
        totalG //= 10
        totalB //= 10

        topLEDArray[i] = (totalR, totalG, totalB)

    # FILLING IN THE LEFT LEDS

    totalR = 0
    totalG = 0
    totalB = 0

    for i in range(15):
        for j in range(PIXELS_TO_PROCESS):
            x = random.randint(0, PIXELS_PER_LED)
            y = random.randint(HORIZONTAL_PIXEL_GAP + i * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED)
            totalR += pix[x, y][0]
            totalG += pix[x, y][1]
            totalB += pix[x, y][2]

        totalR //= 10
        totalG //= 10
        totalB //= 10

        leftLEDArray[i] = (totalR, totalG, totalB)

    leftLEDArray.reverse()

    # FILLING IN THE RIGHT LEDS

    totalR = 0
    totalG = 0
    totalB = 0

    for i in range(15):
        for j in range(PIXELS_TO_PROCESS):
            x = random.randint(HORIZONTAL_PIXEL_COUNT - PIXELS_PER_LED, HORIZONTAL_PIXEL_COUNT - 1)
            y = random.randint(HORIZONTAL_PIXEL_GAP + i * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED)
            totalR += pix[x, y][0]
            totalG += pix[x, y][1]
            totalB += pix[x, y][2]

        totalR //= 10
        totalG //= 10
        totalB //= 10

        rightLEDArray[i] = (totalR, totalG, totalB)

    ser = Serial('/dev/tty.usbserial', 9600)

    end = time.time()
    break

print(end - start)
