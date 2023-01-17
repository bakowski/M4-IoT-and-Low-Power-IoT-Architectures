from umqtt.robust import MQTTClient
import machine
import utime as time
import gc
import wifista

#broker.hivemq.com

wifista.disconnect()
wifista.connect()
client = MQTTClient("pycom/esp32", "broker.emqx.io")

def publish():
    count = 1
    while True:
        msg = "hello" + str(count)
        client.publish(b"pycom/esp32", msg)
        count = count + 1
        print(msg)
        time.sleep(20)


client.reconnect()
print('test')
publish()

