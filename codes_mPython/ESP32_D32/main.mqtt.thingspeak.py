from umqtt.simple import MQTTClient
import wifista

server = "mqtt.thingspeak.com"
client = MQTTClient("umqtt_client", server)
CHANNEL_ID = "1626377"
WRITE_API_KEY = "3IN09682SQX3PT4Z"

topic = "channels/" + CHANNEL_ID + "/publish/" + WRITE_API_KEY
temp =21.5
hum =55.7

wifista.connect()
payload = "field1="+str(temp)+"&field2="+str(hum)
client.connect()
client.publish(topic, payload)
client.disconnect()



