import time
import machine
from micropyGPS import MicropyGPS
import ssd1306
import _thread
import time

i2c = machine.I2C(scl=machine.Pin(14), sda=machine.Pin(12))
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c, False)
uart = machine.UART(1, rx=16, tx=17, baudrate=9600, bits=8, parity=None, stop=1, timeout=5000, rxbuf=1024)

def disp(d1,d2,d3):
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text(d1, 0, 16)
    oled.text(d2, 0, 32)
    oled.text(d3, 0, 48)
    oled.show()

def recv():
    while True:
      buf=uart.readline()
      print('Line: {}'.format(buf))
      disp("UART receiver ",'{}'.format(buf),"   ")
      
      
recv()


  