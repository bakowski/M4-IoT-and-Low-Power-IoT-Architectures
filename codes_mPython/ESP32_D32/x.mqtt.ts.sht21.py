from umqtt.simple import MQTTClient
import wifista, time
import machine, sht21
from machine import Pin,I2C
sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max

server = "mqtt.thingspeak.com"
client = MQTTClient("umqtt_client", server)
CHANNEL_ID = "1538804"
WRITE_API_KEY = "YOX31M0EDKO0JATK"
topic = "channels/" + CHANNEL_ID + "/publish/" + WRITE_API_KEY
temp =21.5
hum =55.7


#wifista.scan()

wifista.connect()
client.connect()

for i in range(60):
    wifista.connect()
    client.connect()
    temp = sht21.SHT21_TEMPERATURE(i2c)
    humi = sht21.SHT21_HUMIDITE(i2c)
    ftemp= "{:.2f}".format(temp)
    fhumi= "{:.2f}".format(humi)
    payload = "field1="+str(ftemp)+"&field2="+str(fhum)
    client.publish(topic, payload)
    client.disconnect()
    temp=temp+1.0
    hum=hum+2.0
    #wifista.disconnect()
    time.sleep(20)
    