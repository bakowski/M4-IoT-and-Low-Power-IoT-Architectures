from umqtt.simple import MQTTClient
import wifista, time
import sht21
from machine import Pin,I2C

server = "mqtt.thingspeak.com"
client = MQTTClient("umqtt_client", server)
CHANNEL_ID = "1538804"
WRITE_API_KEY = "YOX31M0EDKO0JATK"
topic = "channels/" + CHANNEL_ID + "/publish/" + WRITE_API_KEY
temp =21.5
hum =55.7


#wifista.scan()
#wifista.disconnect()
wifista.connect()
client.connect()

for i in range(60):
    wifista.connect()
    client.connect()
    payload = "field1="+str(temp)+"&field2="+str(hum)
    client.publish(topic, payload)
    client.disconnect()
    temp=temp+1.0
    hum=hum+2.0
    #wifista.disconnect()
    time.sleep(20)
    