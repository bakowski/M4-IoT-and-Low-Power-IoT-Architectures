from time import sleep
import machine, ssd1306
from machine import Pin, SoftI2C
import esp32, time

# Heltec LoRa 32 with OLED Display
oled_width = 128
oled_height = 64


def disp(c):
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
    oled.text(str(c), 0, 45)
    oled.text('HELLO LoRa      ', 0, 25)
    oled.text('smartcomputerlab', 0, 0)
    oled.show()

  
# def disp(c):
#     i2c = SoftI2C(scl=Pin(0), sda=Pin(10), freq=100000)
#     oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
#     oled.fill(0)
#     oled.text("SmartComputerLab", 0, 0)
#     oled.text("LoRa sender", 0, 16)
#     oled.text("Packet Nr:", 0, 32)
#     oled.text(str(c), 0, 48)
#     oled.show()


def send(lora):
    print("LoRa Sender")
    counter = 0
    while True:
        payload = 'Long long Hello ({0})'.format(counter)
        print('TX: {}'.format(payload))
        lora.println(payload)
        counter += 1
        disp(counter)
        sleep(5)
