from machine import Pin,SPI
from sx127x import SX127x
from time import sleep
import LoRaSender
# import LoRaReceiver
# import LoRaPing
# import LoRaReceiverCallback
# radio – modulation parameters



lora_default = {
    'frequency': 8685e5,
    'frequency_offset':0,
    'tx_power_level': 17,
    'signal_bandwidth': 250e3,
    'spreading_factor': 11,
    'coding_rate': 8,
#     'preamble_length': 8,
#     'implicitHeader': False,
#     'sync_word': 0x12,
#     'enable_CRC': False,
#     'invert_IQ': False,
#     'debug': False,
}


# pins for Heltec

lora_pins = {
    'dio_0':26,
    'ss':18,      # 16 on SPI-LoRa ext. card
    'reset':14,  #RST
    'sck':5,
    'miso':19,
    'mosi':27,
}

lora_spi = SPI(
    baudrate=10000000, polarity=0, phase=0,
    bits=8, firstbit=SPI.MSB,
    sck=Pin(lora_pins['sck'], Pin.OUT, Pin.PULL_DOWN),
    mosi=Pin(lora_pins['mosi'], Pin.OUT, Pin.PULL_UP),
    miso=Pin(lora_pins['miso'], Pin.IN, Pin.PULL_UP),
)

lora = SX127x(lora_spi, pins=lora_pins, parameters=lora_default)


# type = 'receiver'
# type = 'ping_master'
# type = 'ping_slave'
type = 'sender'     # let us select sender method

if __name__ == '__main__':
    if type == 'sender':
        LoRaSender.send(lora)
#     if type == 'receiver':
#         LoRaReceiver.receive(lora)
#     if type == 'ping_master':
#         LoRaPing.ping(lora, master=True)
#     if type == 'ping_slave':
#         LoRaPing.ping(lora, master=False)
#     if type == 'receiver_callback':
#         LoRaReceiverCallback.receiveCallback(lora)
        
        
        

