import machine
import sys
import network
import utime, time
import urequests
import wifista

# Pin definitions
bouton = machine.Pin(25,machine.Pin.IN, machine.Pin.PULL_UP)
led = machine.Pin(22,machine.Pin.OUT)
# Network settings
# wifista.disconnect()
wifista.connect()
# wifi_ssid = "Livebox-08B0"
# wifi_password = "G79ji6dtEptVTPWmZP"
# Web page (non-SSL) to get
url = "http://www.smartcomputerlab.org"
# Create a station object to store our connection
# station = network.WLAN(network.STA_IF)
# station.active(True)
# #station.disconnect()  # try this line to clear the modem after OSError: Wifi Internal Error
# print("Connecting...")
# station.connect(wifi_ssid, wifi_password)
# print("Connected!")
# print("My IP Address:", station.ifconfig()[0])
# Continually print out HTML from web page as long as we have a connection
c=0
while c<4:
    # Display connection details
#     print("Connected!")
#     print("My IP Address:", station.ifconfig()[0])
    wifista.connect()
    # Perform HTTP GET request on a non-SSL web
    response = urequests.get(url)
    # Display the contents of the page
    print(response.text)
    time.sleep(6)
    c+=1
    
    
# If we lose connection, repeat this main.py and retry for a connection
print("Connection lost. Trying again.")

