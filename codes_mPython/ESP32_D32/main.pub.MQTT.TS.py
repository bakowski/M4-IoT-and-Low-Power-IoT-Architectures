from umqtt.robust import MQTTClient
import machine
from machine import Pin,SPI,SoftI2C
import ssd1306
import wifista
import utime as time
import gc
from sx127x import SX127x


def disp(p):
    i2c = SoftI2C(scl=Pin(0), sda=Pin(10), freq=100000)
    oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text("LoRa receiver", 0, 16)
    oled.text("Packet Nr:", 0, 32)
    oled.text("RSSI: {}".format(p), 0, 48)
    oled.show()


# radio – modulation parameters
lora_default = {
    'frequency': 434500000,   #869525000,
    'frequency_offset':0,
    'tx_power_level': 14,
    'signal_bandwidth': 125e3,
    'spreading_factor': 9,
    'coding_rate': 5,
    'preamble_length': 8,
    'implicitHeader': False,
    'sync_word': 0x12,
    'enable_CRC': True,
    'invert_IQ': False,
    'debug': False,
}

# modem – connection wires-pins on SPI bus
# pins for C3

lora_pins = {
    'dio_0':3,
    'ss':7,      # 16 on SPI-LoRa ext. card
    'reset':8,  #RST
    'sck':4,
    'miso':6,
    'mosi':5,
}
# pins for S1

# lora_pins = {
#     'dio_0':26,
#     'ss':5,      # 16 on SPI-LoRa ext. card
#     'reset':36,  #RST
#     'sck':18,
#     'miso':19,
#     'mosi':23,
# }

lora_spi = SPI(
    baudrate=10000000, polarity=0, phase=0,
    bits=8, firstbit=SPI.MSB,
    sck=Pin(lora_pins['sck'], Pin.OUT, Pin.PULL_DOWN),
    mosi=Pin(lora_pins['mosi'], Pin.OUT, Pin.PULL_UP),
    miso=Pin(lora_pins['miso'], Pin.IN, Pin.PULL_UP),
)

lora = SX127x(lora_spi, pins=lora_pins, parameters=lora_default)

wifista.connect()
broker = "broker.emqx.io"
client = MQTTClient("PYCOM-V", broker)

def only_publish():
    count = 1
    rssi =0
    while True:

        if lora.receivedPacket():
            payload = lora.readPayload().decode()
            rssi = lora.packetRssi()
            print("RX: {} | RSSI: {}".format(payload, rssi))
            mess="RSSI: " + str(rssi)
            wifista.connect()
            client.publish(b"pycom-v/test", mess)
            count = count + 1
            disp(rssi)
            time.sleep(10)

client.reconnect()
only_publish()
