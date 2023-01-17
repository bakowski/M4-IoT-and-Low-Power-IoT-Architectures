import ntptime
import wifista
import time
import machine, neopixel
np = neopixel.NeoPixel(machine.Pin(0), 12)

def reset_clock():
    for i in range(12):
        np[i]=(0, 0, 0)

def set_clock(h,m,s,lum):
    reset_clock()
    np[s] = (0, 0, lum)  # set to blue, quarter brightness
    np[m] = (0, lum, 0) # set to green, half brightness
    np[h] = (lum, 0, 0) # set to red, full brightness
    np.write()

wifista.disconnect()
wifista.connect()
set=0
print("Local time before synchronization：%s" %str(time.localtime()))
ntptime.settime()
set=1
while set:
    #print("Local time after synchronization：%s" %str(time.localtime()))
    (year,montth,day,hour,min,sec,val1,val2)=time.localtime()
    print("hour: "+ str(hour))
    print("min: "+ str(min))
    print("sec: "+ str(sec))
    ledmin=min/5
    ledsec=sec/5
    ledhour=hour%12
    print(int(ledhour),int(ledmin),int(ledsec))
    set_clock(int(ledhour),int(ledmin),int(ledsec),64)
    time.sleep(5)
    


