from machine import I2C, Pin
from ads1x15 import *

i2c = I2C( sda = Pin(12), scl=Pin(14) )
adc = ADS1115(i2c = i2c, address = 72, gain = 0)

# Mettre le potentiomètre a fond pour appliquer
# 3.3v sur l'entrée analogique A0

# Lire la valeur sur le convertisseur 
# Channel1 = 0 => entrée analogique A0
value = adc.read( rate=0, channel1=0 )

# Affichera la valeur (ex: 17549) 
print( value )

# Calculer la valeur en Volts: 
# * voir les notes ci-dessus concernant la valeur du Bit)
# * Multiplier la valeur lue par 0.1875 mv par bit
#
print( value * 0.1875 / 1000 )

# Ceci affichera 3.29025 Volts

