import network
from esp import espnow

wlan = network.WLAN(network.AP_IF)
wlan.config(hidden=True)
wlan.config(ps_mode=network.WIFI_PS_NONE)
wlan.active(True)

# Prints Slave's MAC
print(wlan.config('mac'))

