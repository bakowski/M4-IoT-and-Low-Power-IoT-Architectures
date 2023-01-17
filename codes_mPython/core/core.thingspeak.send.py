import machine
import time
import wifista
import thingspeak
from thingspeak import ThingSpeakAPI, Channel, ProtoHTTP
channel_living_room = "1538804"
field_temperature = "Temperature"
field_humidity = "Humidity"
thing_speak = ThingSpeakAPI([
    Channel(channel_living_room , 'YOX31M0EDKO0JATK', [field_temperature, field_humidity])],
    protocol_class=ProtoHTTP, log=True)

wifista.connect()
active_channel = channel_living_room
temperature = 2.0
humidity=3.0
c=0
while c<20:
    thing_speak.send(active_channel, {
        field_temperature: temperature,
        field_humidity: humidity
    })
    temperature=temperature+1.0
    humidity=humidity+2.0
    c=c+1
    time.sleep(thing_speak.free_api_delay)
    
    