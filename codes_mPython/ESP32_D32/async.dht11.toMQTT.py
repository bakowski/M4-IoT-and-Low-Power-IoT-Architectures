import network
from umqtt.robust import MQTTClient
from machine import Pin
from dht import DHT22
import uasyncio


wifi_ssid = "Livebox-08B0"
wifi_password = "G79ji6dtEptVTPWmZP"
broker ="broker.emqx.io"
client = MQTTClient("PYCOM-X",broker)


sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect(wifi_ssid, wifi_password)
while not sta_if.isconnected():
    print(".", end = "")

dht11 = DHT11(Pin(0))
# initialize the global variables
dht11.measure()
temperature = dht11.temperature()
humidity = dht11.humidity()

async def measure():
    global temperature, humidity
    while True:
        dht11.measure()
        temperature = dht11.temperature()
        humidity = dht11.humidity()
        await uasyncio.sleep(1)
        
async def send_message():
    global temperature, humidity
    while True:
        mess= "T:"+str(temperature) + H:" + str(humidity)
        try:
            client.publish(b"pycom-x/test",mess)
            print(r.text)
        except Exception as e:
            print(e)
        await uasyncio.sleep(60)
        
event_loop = uasyncio.get_event_loop()
event_loop.create_task(measure())
event_loop.create_task(send_message())
event_loop.run_forever()
