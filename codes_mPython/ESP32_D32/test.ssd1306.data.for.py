import machine, ssd1306
from machine import Pin, SoftI2C
import esp32
import time
print(machine.freq())
i2c = SoftI2C(scl=Pin(14), sda=Pin(12), freq=100000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)

d1=0
d2=0

for i in range(10):
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text("ESP32 mPython", 0, 16)
    oled.text(str(d1), 0, 32)
    oled.text(str(d2), 0, 48)
    oled.show()
    d1+=1
    d2+=2
    time.sleep(2)

