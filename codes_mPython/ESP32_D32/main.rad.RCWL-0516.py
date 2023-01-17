from machine import Pin
import time
 
ldr = Pin(1, Pin.IN)     # create input pin on GPIO2
 
while True:
	if ldr.value():
            print('OBJECT DETECTED')
        else:
            print('ALL CLEAR')
	time.sleep(1)
	
	