import machine, ssd1306
from machine import Pin, SoftI2C
import esp32

print(machine.freq())

i2c = SoftI2C(scl=Pin(14), sda=Pin(12), freq=100000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
oled.fill(0)
oled.text("SmartComputerLab", 0, 0)
oled.text("RISC-X mPython", 0, 16)
oled.text("Audencia-2022", 0, 32)
oled.text("WiFi/BLE/LoRa", 0, 48)
oled.show()
