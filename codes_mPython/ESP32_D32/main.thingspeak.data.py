import machine
import time
import wifista

try:
    from thingspeak import ThingSpeakAPI, Channel, ProtoHTTP
except ImportError:
    from .lib.thingspeak import ThingSpeakAPI, Channel, ProtoHTTP


channel_living_room = "1626377"
#channel_bedroom = "bedroom"

field_temperature = "Temperature"
field_humidity = "Humidity"

thing_speak = ThingSpeakAPI([
    #Channel(channel_living_room, '3IN09682SQX3PT4Z', [field_temperature, field_humidity]),
    Channel(channel_living_room , '3IN09682SQX3PT4Z', [field_temperature, field_humidity])],
    protocol_class=ProtoHTTP, log=True)

wifista.connect()
active_channel = channel_living_room

temperature = 21.4
humidity=33.7

while True:
    thing_speak.send(active_channel, {
        field_temperature: temperature,
        field_humidity: humidity
    })

    time.sleep(thing_speak.free_api_delay)

