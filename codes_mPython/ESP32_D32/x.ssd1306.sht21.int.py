import machine
import ssd1306
import sht21

from machine import Pin,I2C
import utime
sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)


def disp(d1,d2,d3):
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text(d1, 0, 16)
    oled.text(d2, 0, 32)
    oled.text(d3, 0, 48)
    oled.show()


c=0
disp("ESP32 mPython","Audencia-2022","WiFi/BLE/LoRa")
while c<100:
    temp = sht21.SHT21_TEMPERATURE(i2c)
    humi = sht21.SHT21_HUMIDITE(i2c)
    print ("Temperature = %.1f" % temp)
    print ("Humidity = %.1f" % humi)
    c+=1
    disp("SHT21 sensor","T: "+str(temp),"H: "+str(humi))
    utime.sleep_ms(1000)

