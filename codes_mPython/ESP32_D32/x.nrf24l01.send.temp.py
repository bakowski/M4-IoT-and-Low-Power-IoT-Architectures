import machine
import utime
#https://github.com/micropython/micropython/tree/master/drivers/nrf24l01
import sys
import ustruct as struct
import utime
from machine import Pin, SPI
from nrf24l01 import NRF24L01
from micropython import const

# Slave pause between receiving data and checking for further packets.
_RX_POLL_DELAY = const(15)
# Slave pauses an additional _SLAVE_SEND_DELAY ms after receiving data and before
# transmitting to allow the (remote) master time to get into receive mode. The
# master may be a slow device. Value tested with Pyboard, ESP32 and ESP8266.
_SLAVE_SEND_DELAY = const(10)
cfg = {"spi": -1, "miso": 19, "mosi": 23, "sck": 18, "csn": 5, "ce": 15}
#http://electroniqueamateur.blogspot.com/2019/12/communication-nrf24l01-avec-cartes.html?m=0

# Addresses are in little-endian format. They correspond to big-endian
# 0xf0f0f0f0e1, 0xf0f0f0f0d2
pipes = (b"\xe1\xf0\xf0\xf0\xf0", b"\xd2\xf0\xf0\xf0\xf0")

###############################################################################
# Parameters and global variables

# Pin definitions
sda_pin = machine.Pin(12)
scl_pin = machine.Pin(14)

# Create an I2C object out of our SDA and SCL pin objects
i2c = machine.I2C(sda=sda_pin, scl=scl_pin)

# TMP102 address on the I2C bus
tmp102_addr = 0x48

# TMP102 register addresses
reg_temp = 0x00
reg_config = 0x01


pakketnr = 0
###############################################################################
# Functions

# Read temperature registers and calculate Celsius
def read_temp():

    return 221;

def updatePakketnr():
  global pakketnr
  pakketnr +=  1
  
  if (pakketnr >= 3600):
    pakketnr = 0
  return pakketnr

# Initialize communications with the TMP102
def init():

    # Read CONFIG register (2 bytes) and convert to integer list
    val = i2c.readfrom_mem(tmp102_addr, reg_config, 2)
    val = list(val)

    # Set to 4 Hz sampling (CR1, CR0 = 0b10)
    val1 = val1 & 0b00111111
    val1 = val1 | (0b10 << 6)

    # Write 4 Hz sampling back to CONFIG
    i2c.writeto_mem(tmp102_addr, reg_config, bytearray(val))
    
    


def readSendTemp():
    csn = Pin(cfg["csn"], mode=Pin.OUT, value=1)
    ce = Pin(cfg["ce"], mode=Pin.OUT, value=0)
    if cfg["spi"] == -1:
        spi = SPI(-1, sck=Pin(cfg["sck"]), mosi=Pin(cfg["mosi"]), miso=Pin(cfg["miso"]))
        nrf = NRF24L01(spi, csn, ce, payload_size=12)
    else:
        nrf = NRF24L01(SPI(cfg["spi"]), csn, ce, payload_size=12)
    
    nrf.open_tx_pipe(pipes[0])

    nrf.open_rx_pipe(1, pipes[1])
    nrf.start_listening()
    
    print("NRF24L01 master mode")

    while True:
        # stop listening and send packet
        nrf.stop_listening()
        temperature = read_temp()
        preamble = 0xAA
        
        counterBit = 0
        for bit in bin(temperature):
          counterBit +=1
         
        counterBit -= 2 
        
        numberOfBytes = int(counterBit / 8)
        updatePakketnr()
        
        print("sending:",preamble,numberOfBytes,int(temperature*100))
               
        
        
        try:
            nrf.send(struct.pack("iiii",preamble,numberOfBytes,pakketnr,temperature))
            print("sent")
        except OSError:
            pass

       
        utime.sleep(5)

def readout():
  while True:
    # Read temperature and print it to the console

    temperature = read_temp()
    print(round(temperature, 2))
    utime.sleep(1)

readSendTemp()