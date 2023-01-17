from umqtt.simple import MQTTClient
import wifista
import time
server = "mqtt.thingspeak.com"
client = MQTTClient("umqtt_client", server)
CHANNEL_ID = "1626377"
WRITE_API_KEY = "3IN09682SQX3PT4Z"
topic = "channels/" + CHANNEL_ID + "/publish/" + WRITE_API_KEY

temp =21.5
hum =55.7

wifista.disconnect()
wifista.connect()
client.connect()
   
for i in range(60):
    wifista.connect()
    payload = "field1="+str(temp)+"&field2="+str(hum)
    client.connect()
    client.publish(topic, payload)
    client.disconnect()
    print(payload)
    temp=temp+1.0
    hum=hum+2.0
    wifista.disconnect()
    time.sleep(15)
    