import ntptime
import wifista_mult
import time
import machine, neopixel
np = neopixel.NeoPixel(machine.Pin(0), 12)

def reset_clock():
    for i in range(12):
        np[i]=(0, 0, 0)

def set_clock(h,m,s,lum):  # lum brightness
    reset_clock()
    np[s] = (0, 0, lum) # set to blue
    np[m] = (0, lum, 0) # set to green
    np[h] = (lum, 0, 0) # set to red
    np.write()

wifista_mult.disconnect()
time.sleep(1)
wifista_mult.connect()
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
    ledmin=(min/5+7)%12  # +7 ring shift
    ledsec=(sec/5+7)%12  # +7 ring shift
    ledhour=(hour+9)%12  # +7 ring shift +2 GMT time
    print(int(ledhour),int(ledmin),int(ledsec))
    set_clock(int(ledhour),int(ledmin),int(ledsec),100)
    time.sleep(5)
    


