
# Load necessary libraries 
import machine
import wifista
import ssd1306
import network  
import urequests
import ujson as json
import time 


# ************************************** 
# Create objects: 
led = machine.Pin(22,machine.Pin.OUT)

scl = machine.Pin(14, machine.Pin.OUT, machine.Pin.PULL_UP)
sda = machine.Pin(12, machine.Pin.OUT, machine.Pin.PULL_UP)
i2c = machine.I2C(scl=scl, sda=sda, freq=400000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c, addr=0x3C)

def print_text(msg, x, y, clr):
    if clr:
        oled.fill(0)
    oled.text(msg, x, y)
    oled.show()


# ************************************** 
# Configure the ESP32 wifi as STAtion
wifista.scan()
wifista.disconnect()
wifista.connect()

# **************************************
# Global variables and constants:
#api.openweathermap.org/data/2.5/weather?q={city name}&appid={API key}
#https://api.openweathermap.org/data/2.5/weather?q=paris&appid=ac07acbb25501a599aad09b6f6d290fb

BASE_URL = "https://api.openweathermap.org/data/2.5/weather?"
API_KEY = "d0f71269f05816fa65ebafa10f1513d5"

machine.Pin(32,machine.Pin.IN,machine.Pin.PULL_UP)


    


UPDATE_INTERVAL_ms = 5000 # in ms time unit
last_update = time.ticks_ms()


# **************************************
# Main loop:
while True:
    if machine.Pin(32).value()==0:
        CITY_NAME = "HongKong"
    else:
        CITY_NAME = "Nantes"
    URL = BASE_URL + "q=" + CITY_NAME + "&appid=" + API_KEY
    if time.ticks_ms() - last_update >= UPDATE_INTERVAL_ms:
        # send a weather data request
        response = urequests.get(URL)

        # check status code of the request 
        if response.status_code == 200: 
            # get the json format of data
            data = response.json() 

            # get the main dict block
            # it contains the weather data
            main = data['main'] 

            # get the temperature and subtract
            # 273.15 to get temp in Celcius
            # originally temp is in Kelvin
            temperature = main['temp'] - 273.15 

            # get the humidity in %
            humidity = main['humidity'] 

            # get the pressure in hPA
            pressure = main['pressure'] 

            # weather report 
            report = data['weather'] 

            ## OLED display
            print_text('**OpenWeather**', 0, 0, 1)
            print_text('City:{}' .format(CITY_NAME), 3, 16, 0)
            print_text('Temp:{} oC' .format(temperature), 3, 26, 0)
            print_text('Humi:{} %' .format(humidity), 3, 36, 0)
            print_text('Pres:{} hPa' .format(pressure), 3, 46, 0)
            print_text('"{}."' .format(report[0]['description']), 3, 56, 0)

            ## debug messages
            print('')
            print('**OpenWeather**')
            print('City:{}' .format(CITY_NAME))
            print('Temperature:{} oC' .format(temperature)) 
            print('Humidity:{} %' .format(humidity)) 
            print('Pressure:{} hPa' .format(pressure)) 
            print('Weather Report:{}.' .format(report[0]['description'])) 
        else: 
            # show error message 
            print_text('Error in HTTP request.', 3, 20, 1)
            print('Error in HTTP request.')

        led.value(not led.value())
        last_update = time.ticks_ms()