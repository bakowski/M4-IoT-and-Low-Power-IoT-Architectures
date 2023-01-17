import machine
import ssd1306
STATUS_BITS_MASK = 0xFFFC
from machine import Pin,I2C,SoftI2C
import utime
sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
data = []
address = 64 # 0x40 in decimal

def disp(d1,d2,d3):
#     i2c = SoftI2C(scl=Pin(14), sda=Pin(12), freq=100000)
#     oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text(d1, 0, 16)
    oled.text(d2, 0, 32)
    oled.text(d3, 0, 48)
    oled.show()

def get_humi():
# Read humidity
    i2c.writeto(address, b'\xF5') # Trigger humidity measurement
    utime.sleep_ms(29) # Wait for it to finish (29ms max)
    data = i2c.readfrom(address, 2) # Get the 2 byte result
    adjusted = (data[0] << 8) + data[1] # convert to 16 bit value
    adjusted &= STATUS_BITS_MASK # zero the status bits
    adjusted *= 125 # scale
    adjusted /= 1 << 16 # divide by 2^16
    adjusted -= 6 # subtract 6
    #print ("Humidity = %.1f" % adjusted)
    return adjusted

def get_temp():
# Read temperature
    i2c.writeto(address, b'\xF3') # Trigger temperature measurement
    utime.sleep_ms(85) # Wait for it to finish (85ms max)
    data = i2c.readfrom(address, 2) # Get the 2 byte result
    ## Compute temperature
    adjusted = (data[0] << 8) + data[1] # convert to 16 bit value
    adjusted &= STATUS_BITS_MASK # zero the status bits
    adjusted *= 175.72 # scale
    adjusted /= 1 << 16 # divide by 2^16
    adjusted -= 46.85 # subtract offset
    #print ("Temperature = %.1f" % adjusted)
    return adjusted

c=0
disp("ESP32 mPython","Audencia-2022","WiFi/BLE/LoRa")
while c<100:
    temp=get_temp()
    print ("Temperature = %.1f" % temp)
    humi=get_humi()
    print ("Humidity = %.1f" % humi)
    c+=1
    disp("SHT21 sensor","T: "+str(temp),"H: "+str(humi))
    utime.sleep_ms(1000)

