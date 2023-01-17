import machine
import sys
import network
import utime, time
import urequests
import wifista

# Pin definitions
led = machine.Pin(12,machine.Pin.OUT)  // LED 12, 13

# Network settings
wifista.disconnect()
wifista.connect()

# Web page (non-SSL) to get
url = "http://www.smartcomputerlab.org"
# Continually print out HTML from web page as long as we have a connection
c=0
while c<4:
    wifista.connect()
    # Perform HTTP GET request on a non-SSL web
    response = urequests.get(url)
    # Display the contents of the page
    print(response.text)
    c+=1
    time.sleep(6)

print("End of program.")

