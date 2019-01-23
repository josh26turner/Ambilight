import subprocess
import random
import time
from serial import Serial
from PIL import Image

PIXELS_TO_PROCESS = 80
PIXELS_PER_LED = 80
HORIZONTAL_PIXEL_GAP = 80
VERTICAL_PIXEL_GAP = 120
HORIZONTAL_PIXEL_COUNT = 2560
VERTICAL_PIXEL_COUNT = 1440
NUMBER_LEDS = 60

leftLEDArray = [(0, 0, 0)] * 15
topLEDArray = [(0, 0, 0)] * 30
rightLEDArray = [(0, 0, 0)] * 15


start = time.time()

while True:
    t1 = time.time()
    subprocess.call('./scrot.sh')  # Taking a screenshot
    t2 = time.time()
    print(t2 - t1)

    im = Image.open('screenshots/img.png')  # Opening the screenshot
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

        totalR //= PIXELS_TO_PROCESS
        totalG //= PIXELS_TO_PROCESS
        totalB //= PIXELS_TO_PROCESS

        topLEDArray[i] = (totalR, totalG, totalB)

    # FILLING IN THE LEFT LEDS

    for i in range(15):
        totalR = 0
        totalG = 0
        totalB = 0

        for j in range(PIXELS_TO_PROCESS):
            x = random.randint(0, PIXELS_PER_LED)
            y = random.randint(HORIZONTAL_PIXEL_GAP + i * PIXELS_PER_LED, HORIZONTAL_PIXEL_GAP + (i+1) * PIXELS_PER_LED)
            totalR += pix[x, y][0]
            totalG += pix[x, y][1]
            totalB += pix[x, y][2]

        totalR //= PIXELS_TO_PROCESS
        totalG //= PIXELS_TO_PROCESS
        totalB //= PIXELS_TO_PROCESS

        leftLEDArray[i] = (totalR, totalG, totalB)

    leftLEDArray.reverse()

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

        totalR //= PIXELS_TO_PROCESS
        totalG //= PIXELS_TO_PROCESS
        totalB //= PIXELS_TO_PROCESS

        rightLEDArray[i] = (totalR, totalG, totalB)

    LEDArray = leftLEDArray + topLEDArray + rightLEDArray

    ser = Serial('/dev/ttyUSB0', 9600)

    for i in range(NUMBER_LEDS):
        ser.write((LEDArray[i][0]).to_bytes(1, byteorder='big'))
        ser.write((LEDArray[i][1]).to_bytes(1, byteorder='big'))
        ser.write((LEDArray[i][2]).to_bytes(1, byteorder='big'))

    break

end = time.time()

print((end - start) ** -1)
