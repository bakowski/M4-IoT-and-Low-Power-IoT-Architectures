import time
from machine import Pin
from machine import I2C
import VL53L0X


i2c= I2C(0, sda=Pin(12), scl=Pin(14))

# Create a VL53L0X object
tof = VL53L0X.VL53L0X(i2c)

while True:
# Start ranging
    tof.start()
    tof.read()
    print(tof.read())
    tof.stop()






    #q = tof.set_signal_rate_limit(0.1)
    #
    # time.sleep(0.1)
