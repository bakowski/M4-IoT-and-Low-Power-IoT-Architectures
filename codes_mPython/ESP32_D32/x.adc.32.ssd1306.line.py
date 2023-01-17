import time 
from machine import Pin, ADC, SoftI2C
import ssd1306

pot=ADC(Pin(34))
i2c = SoftI2C(sda=Pin(12),scl=Pin(14), freq=40000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

counttime=0
val=500
try:
    while True:
        pot.atten(ADC.ATTN_11DB)
        pot.width(ADC.WIDTH_10BIT)
        pot_value = pot.read()
        val = int(0.6*val+0.4*pot_value)
        oled.line(counttime,10,counttime,int((val-400)/1.5),1)
        oled.show()
        print(val, val-400)
        time.sleep_ms(1)
        counttime+=2
        if(counttime>127):
            counttime=0
            oled.fill(0)
            #Clear the screen
            oled.show()
except KeyboardInterrupt:
    pass
