import max44009
from machine import I2C, Pin
from time import sleep

i2c = I2C(scl=Pin(0), sda=Pin(10))

sensor = max44009.MAX44009(i2c)
sensor.continuous

while True: 
    lux=sensor.lux
    print(str(lux) +"lux")
    sleep(1)

