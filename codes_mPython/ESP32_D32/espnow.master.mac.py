import network
import time
from esp import espnow

wlan = network.WLAN(network.AP_IF)
wlan.active(True)
wlan.config(hidden=True)

# Prints Master's MAC
print(wlan.config('mac'))

e = espnow.ESPNow()
e.init()
# Put here the Slaves MACs printed
e.add_peer(b'|\xdf\xa1\xb1[\xb1', ifidx=network.AP_IF)

# Broadcast address to send to every Slaves
peer = b'\xff\xff\xff\xff\xff\xff'
e.add_peer(peer)

#e.clear(True)
while True:
    if e.poll():
        last = None
        for host, msg in e:
            if msg and (str(msg, 'utf8') != last):
                last = str(msg, 'utf8')
                print(msg)


