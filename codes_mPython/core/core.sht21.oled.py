import machine, time, ssd1306
import sht21 # librairie capteur SHT21

i2c = machine.SoftI2C(scl=machine.Pin(5), sda=machine.Pin(4), freq=100000)

def disp(p1,p2):
    oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
    oled.fill(0)
    oled.text("SmartComputerLab",0,0)   # colonne 0 et ligne 0 
    oled.text("max: 16 car/line",0,16)  # colonne 0 et ligne 16
    oled.text(p1,0,32)
    oled.text(p2,0,48)
    oled.show()


c=0
while(c<10):
    if (sht21.SHT21_DETECT(i2c)):
        sht21.SHT21_RESET(i2c)
        resolution = 2
    #sht21.SHT21_SET_RESOLUTION(i2c, resolution)
    #serial_number_sht21 = sht21.SHT21_SERIAL(i2c)
        temperature = sht21.SHT21_TEMPERATURE(i2c)
        humidity = sht21.SHT21_HUMIDITE(i2c)
        print("T: {:.2f}".format(temperature))
        print("H: {:.2f}".format(humidity))
        c=c+1
        disp(str(temperature),str(humidity))
        time.sleep(2)

