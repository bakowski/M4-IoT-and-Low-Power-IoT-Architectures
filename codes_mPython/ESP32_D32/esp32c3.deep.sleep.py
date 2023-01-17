import machine,esp32
from machine import Pin
import time

wake1 = Pin(9, mode = Pin.IN)
#level parameter can be: esp32.WAKEUP_ANY_HIGH or esp32.WAKEUP_ALL_LOW
esp32.wake_on_ext0(pin = wake1, level = esp32.WAKEUP_ALL_LOW)

print('Im awake. Going to sleep in 2 seconds')
time.sleep(2)
print('Going to sleep now')
time.sleep_ms(100)
machine.lightsleep()

