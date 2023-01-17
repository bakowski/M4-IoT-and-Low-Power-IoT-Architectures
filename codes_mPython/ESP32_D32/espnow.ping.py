import network
from esp import espnow

peer = b'0\xaa\xaa\xaa\xaa\xaa'        # MAC address of peer
e = espnow.ESPNow()
e.init()

w0 = network.WLAN(network.STA_IF)
w0.active(True)
w0.connect('myssid', 'myppassword')
while not w0.isconnected():            # Wait until connected...
    time.sleep(0.1)
w0.config(ps_mode=network.WIFI_PS_NONE)  # ..then disable power saving

e.add_peer(peer)                       # Register peer on STA_IF
if not e.send(peer, b'ping'):          # Message will be from STA_IF mac address
  print('Ping failed!')

print('Send me messages at:', w0.config('mac'))

