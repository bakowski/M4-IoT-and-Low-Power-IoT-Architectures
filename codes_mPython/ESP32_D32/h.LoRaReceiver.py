from time import sleep
import machine, ssd1306
from machine import Pin, SoftI2C
import esp32,time

# Heltec LoRa 32 with OLED Display
oled_width = 128
oled_height = 64


def disp(c,rs):
    # OLED reset pin
    i2c_rst = Pin(16, Pin.OUT)
    # Initialize the OLED display
    i2c_rst.value(0)
    time.sleep_ms(5)
    i2c_rst.value(1) # must be held high after initialization
    # Setup the I2C lines
    i2c_scl = Pin(15, Pin.OUT, Pin.PULL_UP)
    i2c_sda = Pin(4, Pin.OUT, Pin.PULL_UP)
    # Create the bus object
    i2c = SoftI2C(scl=i2c_scl, sda=i2c_sda)
    # Create the display object
    oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)
    oled.fill(0)
    oled.text(str(c), 0, 32)
    oled.text("RSSI: "+ str(rs), 0, 48)
    oled.text('LoRa receiver   ', 0, 16)
    oled.text('SmartComputerLab', 0, 0)
    oled.show()


def receive(lora):
    print("LoRa Receiver")

    while True:
        if lora.receivedPacket():
            try:
                payload = lora.readPayload().decode()
                rssi = lora.packetRssi()
                print("RX: {} | RSSI: {}".format(payload, rssi))
                disp(payload,rssi)
            except Exception as e:
                print(e)
