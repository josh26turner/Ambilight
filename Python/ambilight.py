import subprocess
import random
from serial import Serial
from PIL import Image

PIXELS_TO_PROCESS = 200
PIXELS_PER_LED = 80
HORIZONTAL_PIXEL_GAP = 80
VERTICAL_PIXEL_GAP = 120
HORIZONTAL_PIXEL_COUNT = 2560
VERTICAL_PIXEL_COUNT = 1440
NUMBER_LEDS = 60

leftLEDArray = [0] * (15 * 3)
topLEDArray = [0] * (30 * 3)
rightLEDArray = [0] * (15 * 3)

ser = Serial('/dev/ttyUSB0', 9600, timeout=5)

while True:
    subprocess.call('/code/Ambilight/Python/scrot.sh')  # Taking a screenshot

    im = Image.open('/tmp/Ambilight/img.png')  # Opening the screenshot
    pix = im.load()  # Loading the screenshot into local memory

    # FILLING IN THE TOP LEDS

    for i in range(30):
        totalR = 0
        totalG = 0
        totalB = 0

        for j in range(PIXELS_TO_PROCESS):
            x = random.randint(HORIZONTAL_PIXEL_GAP + i * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED)
            y = random.randint(0, PIXELS_PER_LED)
            totalR += pix[x, y][0]
            totalG += pix[x, y][1]
            totalB += pix[x, y][2]

        topLEDArray[(i * 3) + 0] = totalR // PIXELS_TO_PROCESS
        topLEDArray[(i * 3) + 1] = totalG // PIXELS_TO_PROCESS
        topLEDArray[(i * 3) + 2] = totalB // PIXELS_TO_PROCESS

    # FILLING IN THE LEFT LEDS

    for k in range(15):
        i = 15 - k - 1
        totalR = 0
        totalG = 0
        totalB = 0

        for j in range(PIXELS_TO_PROCESS):
            x = random.randint(0, PIXELS_PER_LED)
            y = random.randint(HORIZONTAL_PIXEL_GAP + k * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (k+1) * PIXELS_PER_LED)
            totalR += pix[x, y][0]
            totalG += pix[x, y][1]
            totalB += pix[x, y][2]

        leftLEDArray[(i * 3) + 0] = totalR // PIXELS_TO_PROCESS
        leftLEDArray[(i * 3) + 1] = totalG // PIXELS_TO_PROCESS
        leftLEDArray[(i * 3) + 2] = totalB // PIXELS_TO_PROCESS

    # FILLING IN THE RIGHT LEDS

    for i in range(15):
        totalR = 0
        totalG = 0
        totalB = 0

        for j in range(PIXELS_TO_PROCESS):
            x = random.randint(HORIZONTAL_PIXEL_COUNT - PIXELS_PER_LED, HORIZONTAL_PIXEL_COUNT - 1)
            y = random.randint(HORIZONTAL_PIXEL_GAP + i * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED)
            totalR += pix[x, y][0]
            totalG += pix[x, y][1]
            totalB += pix[x, y][2]

        rightLEDArray[(i * 3) + 0] = totalR // PIXELS_TO_PROCESS
        rightLEDArray[(i * 3) + 1] = totalG // PIXELS_TO_PROCESS
        rightLEDArray[(i * 3) + 2] = totalB // PIXELS_TO_PROCESS

    LEDArray = leftLEDArray + topLEDArray + rightLEDArray

    prefix = [5, 4, 3, 2, 1]

    ser.write(prefix + LEDArray)
