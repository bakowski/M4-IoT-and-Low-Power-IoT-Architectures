import time
import machine, neopixel
np = neopixel.NeoPixel(machine.Pin(0), 8)

def reset_ring():
    for i in range(12):
        np[i]=(0, 0, 0)
    np.write()
    
def set_all_red():
    for i in range(12):
        np[i]=(255, 0, 0)
    np.write()
    
def set_all_green():
    for i in range(12):
        np[i]=(0, 255, 0)
    np.write()
    
def set_all_blue():
    for i in range(12):
        np[i]=(0, 0, 255)
    np.write()

c=0   
while c<60:
    reset_ring()
    time.sleep(1)
    set_all_red()
    time.sleep(1)
    set_all_green()
    time.sleep(1)
    set_all_blue()
    time.sleep(1)
    c+=1