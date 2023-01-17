from time import sleep
from ulora import LoRa, ModemConfig, SPIConfig

# Lora Parameters - pycom-v
RFM95_RST = 8
RFM95_SPIBUS = SPIConfig.pycom-v
RFM95_CS = 7
RFM95_INT = 3
RF95_FREQ = 434.0
RF95_POW = 20
CLIENT_ADDRESS = 1
SERVER_ADDRESS = 2

# initialise radio
lora = LoRa(RFM95_SPIBUS, RFM95_INT, CLIENT_ADDRESS, RFM95_CS, reset_pin=RFM95_RST, freq=RF95_FREQ, tx_power=RF95_POW, acks=True)


# loop and send data
while True:
    lora.send_to_wait("This is a test message", SERVER_ADDRESS)
    print("sent")
    sleep(10)
