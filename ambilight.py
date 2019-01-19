# import subprocess
import random
from PIL import Image

# subprocess.call('./scrot.sh')

im = Image.open('screenshots/img.png') # Can be many different formats.
pix = im.load()

totalr = 0
totalg = 0
totalb = 0

for i in range(10):
    x = random.randint(1, 48+1)
    y = random.randint(1, 48+1)
    print(x, ", ", y, ": ", pix[x, y])
    totalr += pix[x, y][0]
    totalg += pix[x, y][1]
    totalb += pix[x, y][2]

totalr /= 10
totalg /= 10
totalb /= 10

print(totalr, totalg, totalb)