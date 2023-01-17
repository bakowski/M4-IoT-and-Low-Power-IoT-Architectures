# More details can be found in TechToTinker.blogspot.com 
# George Bantique | tech.to.tinker@gmail.com

from machine import Pin
from machine import I2C
from bmp280 import BMP280

i2c_bus = I2C(0, sda=Pin(12), scl=Pin(14))
bmp = BMP280(i2c_bus)

# The following lines of code should be tested in the REPL:
# 1. To get envirment temperature (^C):
print(bmp.getTemp())
#
# 2. To get Pressure (hPa):
print(bmp.getPress())
#
# 3. To calculate absolute altitude (m):
#bmp.getAlti()


