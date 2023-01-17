import max44009
rom machine import Pin, SoftI2C
import esp32

i2c = SoftI2C(scl=Pin(14), sda=Pin(12), freq=100000)
sensor = max44009.MAX44009(i2c)
sensor.illuminance_lux()
