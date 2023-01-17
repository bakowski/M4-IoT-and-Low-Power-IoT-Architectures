from umqtt.robust import MQTTClient
import machine
import sht21
from machine import Pin,I2C
import utime as time
import gc
import wifista

sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max

#broker.hivemq.com

wifista.disconnect()
wifista.connect()
client = MQTTClient("pycom/esp32", "broker.emqx.io")

def publish():
    count = 1
    while True:
        temp = sht21.SHT21_TEMPERATURE(i2c)
        humi = sht21.SHT21_HUMIDITE(i2c)
        ftemp="{:.2f}".format(temp)
        fhumi="{:.2f}".format(humi)
        msg = "T:" + str(ftemp) + ", H:" + str(fhumi)
        client.publish(b"pycom/esp32", msg)
        count = count + 1
        print(msg)
        time.sleep(20)


client.reconnect()
print('test')
publish()

