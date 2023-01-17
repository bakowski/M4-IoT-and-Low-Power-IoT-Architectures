import time
import machine
from machine import Pin, SoftI2C
from micropyGPS import MicropyGPS
import ssd1306
import _thread
import time

WIDTH  = 128
HEIGHT = 64

def main():
    i2c = SoftI2C(scl=Pin(5), sda=Pin(4), freq=100000)
    dsp = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
    uart = machine.UART(1, rx=0, tx=1, baudrate=9600, bits=8, parity=None, stop=1, timeout=5000, rxbuf=1024)

    gps = MicropyGPS()

    while True:
      buf = uart.readline()
      for char in buf:
        gps.update(chr(char))  # Note the conversion to to chr, UART outputs ints normally
      #print('UTC Timestamp:', gps.timestamp)
      #print('Date:', gps.date_string('long'))
      #print('Latitude:', gps.latitude)
      #print('Longitude:', gps.longitude_string())
      #print('Horizontal Dilution of Precision:', gps.hdop)
      #print('Altitude:', gps.altitude)
      #print('Satellites:', gps.satellites_in_use)
      #print()     
      dsp.fill(0)
      y = 0
      dy = 10
      dsp.text("{}".format(gps.date_string('s_mdy')), 0, y)
      dsp.text("Sat:{}".format(gps.satellites_in_use), 80, y)
      y += dy
      dsp.text("{:02d}:{:02d}:{:02.0f}".format(gps.timestamp[0], gps.timestamp[1], gps.timestamp[2]),  0, y)
      y += dy
      dsp.text("Lat:{}{:3d}'{:02.4f}".format(gps.latitude[2], gps.latitude[0], gps.latitude[1]),  0, y)
      y += dy
      dsp.text("Lon:{}{:3d}'{:02.4f}".format(gps.longitude[2], gps.longitude[0], gps.longitude[1]),  0, y)
      y += dy
      dsp.text("Alt:{:0.0f}ft".format(gps.altitude * 1000 / (12*25.4)),  0, y)
      y += dy
      dsp.text("HDP:{:0.2f}".format(gps.hdop),  0, y)
      dsp.show()

 
def startGPSthread():
    _thread.start_new_thread(main, ())

if __name__ == "__main__":
  print('...running main, GPS testing')
  main()