import machine
from machine import I2C
import mlx90614
import time

sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=100000) #I2C channel 0,pins,100kHz max

sensor = mlx90614.MLX90614(i2c)
while True:
	print(sensor.read_ambient_temp(), sensor.read_object_temp())
	time.sleep_ms(1000)
	
	