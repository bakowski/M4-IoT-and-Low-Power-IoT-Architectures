import time
import machine
from micropyGPS import MicropyGPS
import ssd1306
import _thread
import time

WIDTH  = 128
HEIGHT = 64

def main():
    i2c = machine.I2C(scl=machine.Pin(14), sda=machine.Pin(12))
    dsp = ssd1306.SSD1306_I2C(WIDTH, HEIGHT, i2c, 0x3c, False)

    uart = machine.UART(1, rx=16, tx=17, baudrate=9600, bits=8, parity=None, stop=1, timeout=5000, rxbuf=1024)
    
    
while True:
      buf = uart.readline()
      
      
if __name__ == "__main__":
  print('...running main, GPS testing')
  main()