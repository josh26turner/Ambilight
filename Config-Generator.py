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
print(f"Is this your resolution: {dim}?")
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

config.write(f"leds_on_top = {leds_on_top};\n")
config.write(f"leds_on_side = {leds_on_side};\n")
config.write("\n")

config.write(f"pixels_to_process = {pixels_to_process};\n")
config.write("\n")

config.write(f"pixels_per_led_top = {pixels_per_led_top};\n")
config.write(f"pixels_per_led_side = {pixels_per_led_side};\n")
config.write("\n")

config.write(f"vertical_pixel_gap = {vertical_pixel_gap};\n")
config.write(f"vertical_pixel_count = {vertical_pixel_count};\n")
config.write("\n")

config.write(f"horizontal_pixel_gap = {horizontal_pixel_gap};\n")
config.write(f"horizontal_pixel_count = {horizontal_pixel_count};\n")
config.write("\n")

config.write("arduino_device_name = \"/dev/ttyUSB0\";\n")

print("If you have a different name for your Arduino (not /dev/ttyUSB0) put it in the config file (~/.config/Ambilight/config)")
