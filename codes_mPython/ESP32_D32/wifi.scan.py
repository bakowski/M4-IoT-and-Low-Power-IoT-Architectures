import network
station = network.WLAN(network.STA_IF)
station.active(True)
for (ssid, bssid, channel, RSSI, authmode, hidden) in station.scan():
  print("* {:s}".format(ssid))
  #print("   - Auth: {} {}".format(authmodes[authmode], '(hidden)' if hidden else ''))
  print("   - Channel: {}".format(channel))
  print("   - RSSI: {}".format(RSSI))
  print("   - BSSID: {:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}".format(*bssid))
  print()


