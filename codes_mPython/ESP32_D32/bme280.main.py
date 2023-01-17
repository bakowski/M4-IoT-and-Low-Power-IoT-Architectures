import machine
import bme280

i2c = machine.I2C("X")
bme = bme280.BME280(i2c=i2c)

print(bme.temperature(), bme.pressure(), bme.humidity())

