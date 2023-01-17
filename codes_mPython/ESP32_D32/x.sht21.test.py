import machine
STATUS_BITS_MASK = 0xFFFC
from machine import Pin,I2C
import utime
sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max
data = []
address = 64 # 0x40 in decimal

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
temp=get_temp()
print ("Temperature = %.1f" % temp)
humi=get_humi()
print ("Humidity = %.1f" % humi)


