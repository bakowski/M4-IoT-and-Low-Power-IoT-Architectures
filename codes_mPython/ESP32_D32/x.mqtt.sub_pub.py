from umqtt.robust import MQTTClient
import machine
import wifista
import utime as time
import gc
wifista.connect()
broker = "broker.emqx.io"
client = MQTTClient("PYCOM-X", broker)

def sub_cb(topic, msg):
    print((topic, msg))
    if topic == b'pycom-v/test' :
        print('ESP received '+ str(msg))
        
def subscribe_publish():
    count = 1
    client.set_callback(sub_cb)
    client.subscribe(b"pycom-x/test")
    while True:
        client.check_msg()
        mess="hello: " + str(count)
        client.publish(b"pycom-x/test", mess)
        count = count + 1
        time.sleep(20)
        
        
client.reconnect()
subscribe_publish()
