import network
import machine, ssd1306
from machine import Pin,I2C
from esp import espnow
from time import sleep
import ssd1306

def bytes_to_int(bytes):
    result = 0
    for b in bytes:
        result = result * 256 + int(b)
    return result

def int_to_bytes(value, length):
    result = []
    for i in range(0, length):
        result.append(value >> (i * 8) & 0xff)
    result.reverse()
    return result


sda=Pin(12) # PYCOM-X
scl=Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)

r=machine.RTC()


# A WLAN interface must be active to send()/recv()
w0 = network.WLAN(network.STA_IF)  # Or network.AP_IF
w0.active(True)

e = espnow.ESPNow()
e.init()
peer = b'\x7c\x9e\xbd\x46\x32\x60'   # MAC address of peer's wifi interface
#broad_peer = b'\xff\xff\xff\xff\xff\xff'
# 7c:9e:bd:46:32:60
e.add_peer(peer)

e.send("Starting...")       # Send to all peers

print(machine.reset_cause(), r.memory())
#for i in range(100):

temp=bytes_to_int(r.memory())
print(temp)

i=0
while True:
    e.send(peer, str(i)*20, True)
    #e.send(b'end')
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text("ESP32 mPython", 0, 16)
    oled.text(str(temp), 0, 32)
    oled.show()
    sleep(1)
    temp+=1
    r.memory(bytes(int_to_bytes(temp,2)))
    machine.deepsleep(10000)               # Sleep for 10 seconds then reboot
    
