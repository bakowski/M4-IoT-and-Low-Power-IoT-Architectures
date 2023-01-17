import machine
import sys
import network
import utime
import urequests
import wifista

# Pin definitions
led = machine.Pin(22, machine.Pin.OUT)

# Web page (non-SSL) to get
url = "http://www.smartcomputerlab.org"

c=0
# Continually try to connect to WiFi access point
while c<4:
    wifista.connect()
    led.value(not led.value())
    utime.sleep(3)
    # Perform HTTP GET request on a non-SSL web
    response = urequests.get(url)
    # Display the contents of the page
    print(response.text)
    led.value(not led.value())
    utime.sleep(5)
   
    c+=1


# If we lose connection, repeat this main.py and retry for a connection
print("Connection lost. Trying again.")
