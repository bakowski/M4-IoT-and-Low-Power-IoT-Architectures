# Complete project details at https://RandomNerdTutorials.com

import network
import urequests

import esp
esp.osdebug(None)

import gc
gc.collect()

ssid = 'REPLACE_WITH_YOUR_SSID'
password = 'REPLACE_WITH_YOUR_PASSWORD'

api_key = 'REPLACE_WITH_YOUR_THING_SPEAK_API_KEY'

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())

temp = '12'
hum = '70'

sensor_readings = {'field1':temp, 'field2':hum}
print(sensor_readings)

request_headers = {'Content-Type': 'application/json'}

request = urequests.post(
  'http://api.thingspeak.com/update?api_key=' + api_key,
  json=sensor_readings,
  headers=request_headers)
print(request.text)
request.close()
