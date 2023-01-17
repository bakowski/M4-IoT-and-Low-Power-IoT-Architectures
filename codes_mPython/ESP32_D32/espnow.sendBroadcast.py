import network
w = network.WLAN(network.STA_IF)
w.active(True)
w.config(protocol=network.MODE_LR)

from esp import espnow

e = espnow.ESPNow()
e.init()

espnow_bcast = b'\xFF' * 6
e.add_peer(w, espnow_bcast)


import time
import struct

seq = 0
while True:
    msg = "hello: "+str(seq)
    e.send(espnow_bcast, msg)
    time.sleep(4)
    seq += 1
