import esp32
from machine import Pin
from machine import deepsleep
from time import sleep

p0 = Pin(25, Pin.IN, Pin.PULL_UP)

#level parameter can be: esp32.WAKEUP_ANY_HIGH or esp32.WAKEUP_ALL_LOW
# esp32.wake_on_ext0(pin = wake1, level = esp32.WAKEUP_ALL_LOW)

while True:
    print(p0.value())
    if p0.value()==0:
        break
    sleep(2)



