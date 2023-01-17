import network
w = network.WLAN()
w.active(True)
w.config(protocol=network.MODE_LR)

from esp import espnow

e = espnow.ESPNow()
e.init()

import struct
pfmt = "%12d %12d" + " %6d" * 7

while True:
    msg = e.recv()
    if msg:
        print(pfmt % struct.unpack(">LL7h", msg[1]))

