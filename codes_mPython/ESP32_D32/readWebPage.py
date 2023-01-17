import machine
import sys
import wifista
import utime, time
import urequests

# Pin definitions
bouton = machine.Pin(9,machine.Pin.IN, machine.Pin.PULL_UP)
led = machine.Pin(2,machine.Pin.OUT)

# Web page (non-SSL) to get
url = "http://www.smartcomputerlab.org"

wifista.connect()
# Continually print out HTML from web page as long as we have a connection
while station.isconnected():
    # Display connection details
    print("Connected!")
    print("My IP Address:", station.ifconfig()[0])
    # Perform HTTP GET request on a non-SSL web
    response = urequests.get(url)
    # Display the contents of the page
    print(response.text)
    time.sleep(6)

# If we lose connection, repeat this main.py and retry for a connection
print("Connection lost. Trying again.")

