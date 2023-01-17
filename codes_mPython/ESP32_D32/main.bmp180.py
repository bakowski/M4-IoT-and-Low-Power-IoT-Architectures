from bmp180 import BMP180
from machine import I2C, Pin                        # create an I2C bus object accordingly to the port you are using
bus =  I2C(scl=Pin(14), sda=Pin(12), freq=100000)   # on mini
bmp180 = BMP180(bus)
bmp180.oversample_sett = 2
bmp180.baseline = 102325  # voir sur carte 

temp = bmp180.temperature
p = bmp180.pressure
altitude = bmp180.altitude
print(temp, p, altitude)

