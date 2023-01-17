def do_connect():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('Livebox-08B0', 'G79ji6dtEptVTPWmZP')
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())

import machine, ssd1306
from machine import Pin, SoftI2C
import esp32

print(machine.freq())
#print((esp32.raw_temperature()-32)/1.8)  # temp in Celsius
do_connect()
#i2c = machine.I2C(scl=machine.Pin(14), sda=machine.Pin(12))
i2c = SoftI2C(scl=Pin(6), sda=Pin(7), freq=100000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
oled.fill(0)
oled.text("Hello World", 0, 0)
oled.text("From microPython", 0, 20)
oled.text("with SoftI2C", 0, 40)
oled.show()


