from umqtt.robust import MQTTClient
import machine
import wifista
import utime as time
import gc

wifista.connect()
broker = "broker.emqx.io"
client = MQTTClient("Pomme-Pi", broker)

def sub_cb(topic, msg):
    print((topic, msg))
    if topic == b'pomme-pi/test' :
        print('Pomme-Pi received '+ str(msg))
        
def subscribe_publish():
    count = 1
    
    