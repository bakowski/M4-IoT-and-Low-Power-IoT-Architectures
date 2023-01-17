from machine import Pin

def connect():
    import network
    ip1        = '192.168.1.110'
    subnet1    = '255.255.255.0'
    gateway1   = '192.168.1.1'
    dns1       = '8.8.8.8'
    
    ip2        = '192.168.8.110'
    subnet2    = '255.255.255.0'
    gateway2   = '192.168.8.1'
    dns2       = '8.8.8.8'
    
    ssid1      = "Livebox-08B0"
    password1  =  "G79ji6dtEptVTPWmZP"
    ssid2      = "SmartIoTLab"
    password2  =  ""
    
    p32 = Pin(32, Pin.IN,Pin.PULL_UP)
    station = network.WLAN(network.STA_IF)

    if station.isconnected() == True:
        print("Already connected")
        return station
 
    station.active(True)
    
    #if(Pin(32).value()):
        #station.ifconfig((ip1,subnet1,gateway1,dns1))  # uncomment to set static IP
    #else:
        #station.ifconfig((ip2,subnet2,gateway2,dns2))  # uncomment to set static IP
    
    if(p32.value()==1):
        station.connect(ssid1,password1)
    else:
        station.connect(ssid2,password2)
    
    while station.isconnected() == False:
        pass
 
    print("Connection successful")
    print(station.ifconfig())
    return station

def disconnect():
    import network
    station = network.WLAN(network.STA_IF)
    station.disconnect()
    station.active(False)
    

disconnect()
connect()
 