#
# Utilisation de librairie du capteur de gestes PAJ7620
#

import machine, time, ssd1306, paj7620

# Activation ecran OLED 
i2c = machine.I2C(scl=machine.Pin(14), sda=machine.Pin(12))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)  # 128 x 64 pixels
# Declaration I2C pour la communication avec le capteur PAJ7620
# GPIO21 --> SDA, GPIO22 --> SCL
g = paj7620.PAJ7620(i2c = i2c)

while True:
    geste = g.gesture()
    # geste peut contenir les valeurs suivantes
	# 	0 : nothing
	# 	1 : Forward
	# 	2 : Backward
	# 	3 : Right
	# 	4 : Left
	# 	5 : Up
	# 	6 : Down
	# 	7 : Clockwise
	# 	8 : anti-clockwise
	# 	9 : wave
    oled.fill(0)  # efface l'ecran
    if geste == 1: 
        oled.text("Avance", 0, 0)
    elif geste == 2:
        oled.text("Recule", 0, 0)
    elif geste == 3:
        oled.text("Droite", 0, 0)
    elif geste == 4:
        oled.text("Gauche", 0, 0)
    elif geste == 5:
        oled.text("Haut", 0, 0)
    elif geste == 6:
        oled.text("Bas", 0, 0)
    elif geste == 7:
        oled.text("Sens horaire", 0, 0)
    elif geste == 8:
        oled.text("Sens anti-horaire", 0, 0)
    elif geste == 9:
        oled.text("Vague", 0, 0)
    else:
        oled.text("Pas de geste", 0, 0)
    oled.show()
    time.sleep(.5)
    