from umqtt.robust import MQTTClient
import machine
import wifista
import utime as time
import gc

wifista.connect()
broker = "broker.emqx.io"
client = MQTTClient("PYCOM-V", broker)

def sub_cb(topic, msg):
  print((topic, msg))
  if topic == b'pycom-v/test' :
    print('ESP received '+ str(msg))

def subscribe_publish():
    count = 1
    client.set_callback(sub_cb)
    client.subscribe(b"pycom-v/test")
    while True:
        client.check_msg()
        mess="hello: " + str(count)
        client.publish(b"pycom-v/test", mess)
        #led.value(1)
        count = count + 1
        time.sleep(20)

client.reconnect()
subscribe_publish()

