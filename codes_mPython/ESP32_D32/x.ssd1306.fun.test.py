import machine, ssd1306
from machine import Pin,I2C

sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X

data = []
address = 64 # 0x40 in decimal

def disp(d1,d1):
    i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max
    oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text("ESP32 mPython", 0, 16)
    oled.text(str(d1), 0, 32)
    oled.text(str(d2), 0, 48)
    oled.show()
    

disp(4,5)


     