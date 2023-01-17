import network

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect("Livebox-08B0", "G79ji6dtEptVTPWmZP")
station.status()
print(station.ifconfig())




