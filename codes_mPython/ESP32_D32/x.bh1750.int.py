import machine
from bh1750 import BH1750
import time

sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max

s = BH1750(i2c)

c=0

while c<100:
    lumi=s.luminance(BH1750.ONCE_HIRES_1)
    c+=1
    print(int(lumi))
    time.sleep(2)
    
    