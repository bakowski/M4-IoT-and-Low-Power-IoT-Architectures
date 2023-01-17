import machine, ssd1306
from machine import Pin, SoftI2C
import esp32

print(machine.freq())

i2c = SoftI2C(scl=Pin(0), sda=Pin(10), freq=100000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
oled.fill(0)
oled.text("SmartComputerLab", 0, 0)
oled.text("RISC-V mPython", 0, 16)
oled.text("PYCOM-V.io", 0, 32)
oled.text("WiFi/BLE/LoRa", 0, 48)
oled.show()