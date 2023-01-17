import machine
from machine import Pin
from time import sleep
led = Pin (22, Pin.OUT)      #GPIO14 as output for LED
led.value(1)                 #LED is ON
sleep(1)                     #delay of 1 second
led.value(0)                 #LED is OFF
sleep(1)                     #delay of 1 second

# add a 10 second delay before ESP32 goes into deep sleep mode. 
# it is just see the message on serial console
# You should remove this line in your real microPython project script

sleep(10)                    #delay of 10 seconds
print('Setting to Deep Sleep Mode')
machine.deepsleep(10000)     #10000ms sleep time

