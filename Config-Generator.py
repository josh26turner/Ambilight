import subprocess
import os

leds_on_top = 0
leds_on_side = 0

pixels_to_process = 30

pixels_per_led_top = 0
pixels_per_led_side = 0

vertical_pixel_gap = 0
vertical_pixel_count = 0

horizontal_pixel_gap = 0
horizontal_pixel_count = 0

cmd = "xdpyinfo | awk '/dimensions/{print $2}'"
ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
output = ps.communicate()[0]

dim = output.decode("utf-8").replace('\n', '')
print("Is this your resolution: {0}?".format(dim))
yes = input("[Y/n]").lower()

def res_fill(dim):
    res = dim.split("x")
    if len(res) == 2:
        res[0] = int(res[0])
        res[1] = int(res[1])
        global horizontal_pixel_count
        global vertical_pixel_count
        horizontal_pixel_count = res[0]
        vertical_pixel_count = res[1]
    else:
        print("Sorry something went wrong")
        exit()

if yes == "n" or yes == "no":
    dim = input("Input your resolution (HORIZONTxVERTICAL):")

res_fill(dim)

leds_on_top = int(input("How many LEDs on top? "))
leds_on_side = int(input("How many LEDs on the side? "))

pixels_per_led_top = horizontal_pixel_count // leds_on_top
pixels_per_led_side = vertical_pixel_count // leds_on_side

horizontal_pixel_gap = (horizontal_pixel_count - (pixels_per_led_top * leds_on_top)) // 2
vertical_pixel_gap = (vertical_pixel_count - (pixels_per_led_side * leds_on_side)) // 2

if not os.path.exists(os.environ['HOME'] + "/.config/Ambilight/"):
    os.mkdir(os.environ['HOME'] + "/.config/Ambilight/")

config = open(os.environ['HOME'] + "/.config/Ambilight/config", "w+")

config.write("leds_on_top = {0};\n".format(leds_on_top))
config.write("leds_on_side = {0};\n".format(leds_on_side))
config.write("\n")

config.write("pixels_to_process = {0};\n".format(pixels_to_process))
config.write("\n")

config.write("pixels_per_led_top = {0};\n".format(pixels_per_led_top))
config.write("pixels_per_led_side = {0};\n".format(pixels_per_led_side))
config.write("\n")

config.write("vertical_pixel_gap = {0};\n".format(vertical_pixel_gap))
config.write("vertical_pixel_count = {0};\n".format(vertical_pixel_count))
config.write("\n")

config.write("horizontal_pixel_gap = {0};\n".format(horizontal_pixel_gap))
config.write("horizontal_pixel_count = {0};\n".format(horizontal_pixel_count))
config.write("\n")

config.write("arduino_device_name = \"/dev/ttyUSB0\";\n")

print("If you have a different name for your Arduino (not /dev/ttyUSB0) put it in the config file (~/.config/Ambilight/config)")
