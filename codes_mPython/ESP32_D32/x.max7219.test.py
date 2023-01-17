import max7219
from machine import Pin, SPI
import time

cfg = {"spi": -1, "miso": 19, "mosi": 23, "sck": 18, "csn": 5, "ce": 15}

spi = SPI(1, baudrate=10000000, polarity=1, phase=0, sck=Pin(18), mosi=Pin(23))
ss = Pin(5, Pin.OUT)
display = max7219.Matrix8x8(spi, ss, 4)
c=36
while c>0:
    count=str(c)
    display.fill(0)
    #display.text('1234',0,0,1)
    display.text(count,0,0,1)
    display.show()
    time.sleep(1)
    c-=1
 
display.fill(0)
display.text('FINI',0,0,1)
display.show()

    

