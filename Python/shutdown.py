from serial import Serial

ser = Serial('/dev/ttyUSB0', 9600, timeout=5)

LEDArray = [0] * (60 * 3)

for x in range(15):
    LEDArray[(x * 3)] = 0
    LEDArray[(x * 3) + 1] = 0
    LEDArray[(x * 3) + 2] = 255

for x in range(30):
    LEDArray[(15 * 3) + (x * 3)] = 255
    LEDArray[(15 * 3) + (x * 3) + 1] = 0
    LEDArray[(15 * 3) + (x * 3) + 2] = 0

for x in range(15):
    LEDArray[(45 * 3) + (x * 3)] = 0
    LEDArray[(45 * 3) + (x * 3) + 1] = 255
    LEDArray[(45 * 3) + (x * 3) + 2] = 0

for x in range(len(LEDArray) - 2):
    print(LEDArray[x], LEDArray[x+1], LEDArray[x+2])

ser.write(LEDArray)
