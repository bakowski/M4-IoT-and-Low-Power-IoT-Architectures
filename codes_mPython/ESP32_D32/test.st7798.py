import machine
import st7789py

spi_pins = {
    'dio_0':3,
    'ss':7,      # 16 on SPI-LoRa ext. card
    'reset':8,  #RST
    'sck':4,
    'miso':6,
    'mosi':5,
}

spi = machine.SoftSPI(1, baudrate=40000000, polarity=1,phase=0,
    bits=8, firstbit=SPI.MSB,
    sck=Pin(spi_pins['sck'], Pin.OUT, Pin.PULL_DOWN),
    mosi=Pin(spi_pins['mosi'], Pin.OUT, Pin.PULL_UP),
    miso=Pin(spi_pins['miso'], Pin.IN, Pin.PULL_UP),
     )
    display = st7789.ST7789(
        spi, 240, 240,
        reset=machine.Pin(8, machine.Pin.OUT),
        dc=machine.Pin(7, machine.Pin.OUT),
    )
# display.init()
# spi = machine.SPI(1, baudrate=40000000, polarity=1)
# display = st7789py.ST7789(spi, 240, 240, reset=machine.Pin(5, machine.Pin.OUT), dc=machine.Pin(4, machine.Pin.OUT))
display.init()
display.pixel(120, 120, st7789py.YELLOW)

