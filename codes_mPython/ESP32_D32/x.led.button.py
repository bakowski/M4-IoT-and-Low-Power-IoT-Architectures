import machine
import time

button = machine.Pin(4, machine.Pin.IN, machine.Pin.PULL_UP)
relay = machine.Pin(0, machine.Pin.OUT)  # BUT2 on Pycom-X board
led = machine.Pin(22, machine.Pin.OUT)   # LED on LOLIN32 board

c=0
while c<100:
    val=button.value()
    print(val)
    c+=1
    if(val==1):
        relay.off()
        led.on()
    else:
        relay.on()
        led.off()
    time.sleep(1)
    
  

