import uSGP30
from machine import I2C, Pin
import machine, time

i2c = I2C(0, sda=Pin(12), scl=Pin(14))

sgp30 = uSGP30.SGP30(i2c)
c=0

while c<100:
    co2eq_ppm, tvoc_ppb = sgp30.measure_iaq()
    print(co2eq_ppm, tvoc_ppb)
    time.sleep(2)
    c+=1
