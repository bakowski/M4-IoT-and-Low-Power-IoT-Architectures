import ntptime
import wifista
import time
#wifista.scan()
wifista.disconnect()
wifista.connect()
set=1
while set:
	(year,montth,day,hour,min,sec,val1,val2)=time.localtime()
	print("hour: "+ str(hour))
	print("min: "+ str(min))
	print("sec: "+ str(sec))
	time.sleep(1)
	
	