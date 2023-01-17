# code for micropython 1.10 on esp8266

import random
#import machine
from machine import Pin,SPI
import st7789py as st7789
import time

disp_pins = {
    'dio_0':3,
    'ss':7,      # 16 on SPI-LoRa ext. card
    'reset':8,  #RST
    'sck':4,
    'miso':6,
    'mosi':5,
}


def dmain():
   # spi = machine.SPI(1, baudrate=40000000, polarity=1)
    spi = SPI(
    baudrate=10000000, polarity=1, 
    sck=Pin(disp_pins['sck'], Pin.OUT, Pin.PULL_DOWN),
    mosi=Pin(disp_pins['mosi'], Pin.OUT, Pin.PULL_UP),
    miso=Pin(disp_pins['miso'], Pin.IN, Pin.PULL_UP),
)
    display = st7789.ST7789(
        spi, 240, 240,
        reset=Pin(8, Pin.OUT),
        dc=Pin(7, Pin.OUT),
    )
    display.init()

    while True:
        display.fill(
            st7789.color565(
                random.getrandbits(8),
                random.getrandbits(8),
                random.getrandbits(8),
            ),
        )
        # Pause 2 seconds.
        time.sleep(2)
        
dmain()

