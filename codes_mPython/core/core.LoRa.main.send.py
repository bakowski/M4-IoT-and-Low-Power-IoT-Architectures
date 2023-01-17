from machine import Pin,SPI
from sx127x import SX127x
from time import sleep

import LoRaSender
# import LoRaReceiver
# import LoRaReceiverCallback
# radio – modulation parameters

lora_default = {                    # our settings
    'frequency': 434500000,   #869525000,
    'frequency_offset':0,
    'tx_power_level': 14,
    'signal_bandwidth': 125e3,	# 125 KHz
    'spreading_factor': 9,		# 2 to power 9
    'coding_rate': 5,  		# 4 data bits over a symbol with 5 bits
    'preamble_length': 8,
    'implicitHeader': False,
    'sync_word': 0x12,
    'enable_CRC': False,
    'invert_IQ': False,
    'debug': False,
}

# modem – connection wires-pins on SPI bus – case Pomme-Pi Zero Core
lora_pins = {
    'dio_0':11,
    'ss':7,      # 16 on SPI-LoRa ext. card
    'reset':6,   #RST
    'sck':2,
    'miso':10,
    'mosi':3,
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