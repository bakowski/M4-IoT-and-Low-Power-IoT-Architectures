import network
from esp import espnow

wlan = network.WLAN(network.AP_IF)
wlan.active(True)
wlan.config(hidden=True)

# Prints Master's MAC
print(wlan.config('mac'))

