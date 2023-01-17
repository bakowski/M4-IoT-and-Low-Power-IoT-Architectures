
import tsl2561
from machine import I2C, Pin
i2c = I2C(scl=Pin(14), sda=Pin(12), freq=10000)
sensor = tsl2561.TSL2561(i2c)
print(sensor.read())
