import machine # la bibliothèque principale
from machine import Pin
from time import sleep
led=Pin(22,Pin.OUT) # le numéro du Pin LED est 2
while True:
    led.value(not led.value()) # la valeur à afficher est complémenté
    sleep(1.1) # le temps d’attente en sécondes
