import machine
import sys
import network
import utime, time
import urequests

# Pin definitions
repl_button = machine.Pin(9,machine.Pin.IN, machine.Pin.PULL_UP)
repl_led = machine.Pin(2,machine.Pin.OUT)

# Network settings
wifi_ssid = "Livebox-08B0"
wifi_password = "G79ji6..EptVTPWmZP"

# Web page (non-SSL) to get
url = "http://www.smartcomputerlab.org"

# Create a station object to store our connection
station = network.WLAN(network.STA_IF)
station.active(True)

print("Connecting...")
station.connect(wifi_ssid, wifi_password)
print("Connected!")
print("My IP Address:", station.ifconfig()[0])
# Check to see if our REPL button is pressed over 10 seconds
#     for i in range(100):
#         if repl_button.value() == 0:
#             print("Dropping to REPL")
#             repl_led.value(1)
#             sys.exit()
#         utime.sleep_ms(100)

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
    
    # Check to see if our REPL button is pressed over 10 seconds
#     for i in range(100):
#         if repl_button.value() == 0:
#             print("Dropping to REPL")
#             repl_led.value(1)
#             sys.exit()
#         utime.sleep_ms(100)

# If we lose connection, repeat this main.py and retry for a connection
print("Connection lost. Trying again.")
