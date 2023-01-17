""" CardKB, Mini I2C Keyboard - simplistic read char over I2C

Author(s):
* Meurisse D for MC Hobby sprl

See Github: https://github.com/mchobby/esp8266-upy/tree/master/cardkb
"""

import machine
from machine import I2C
from cardkb import *

sda=machine.Pin(12) # PYCOM-X
scl=machine.Pin(14) # PYCOM-X
i2c=machine.I2C(0,sda=sda, scl=scl, freq=400000) #I2C channel 0,pins,400kHz max


s = ''

keyb = CardKB( i2c )
while True:
	ch = keyb.read_char( wait=True ) # Wait for a key to be pressed (by default)
	if ord(ch) == RETURN:
		print( 'Return pressed! Blank string')
		s = ''
	elif ord(ch) == BACKSPACE:
		s = s[:-1] # remove last char
	else:
		s = s + ch # Add the char to the string
	print( s )

