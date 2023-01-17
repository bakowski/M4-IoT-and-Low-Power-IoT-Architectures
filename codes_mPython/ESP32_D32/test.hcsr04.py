from hcsr04 import HCSR04

sensor = HCSR04(trigger_pin=16, echo_pin=17)
distance = sensor.distance_cm()
print('Distance:', distance, 'cm')

