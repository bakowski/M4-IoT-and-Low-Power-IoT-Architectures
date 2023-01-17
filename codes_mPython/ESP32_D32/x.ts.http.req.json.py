import socket
import wifista
import time
import json
import gc

def http_get(url):
    scheme, _, host, path = url.split('/', 3)
    wifista.connect()
    s = socket.socket()
    s.connect((host, 80))
    request=bytes('GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n' % (path, host), 'utf8')    # request header string into bytes 
    print("Requesting /%s from host %s\n" % (path, host))
    s.send(request)
    val=0
    c=0
    while True:
         data = s.recv(500)
         if data:
             val=data
             if(c<2):
                 c+=1
                 if (c==2):
                     s.close()
                     return val 
         else:
            break
    s.close()
    return val

c=0

gc.enable()
wifista.scan()
wifista.disconnect()
wifista.connect()

while c<1000:

    try:
        print(gc.mem_free())
        gc.collect()
        #ret=http_get("https://api.thingspeak.com/channels/1538804/fields/1/last.json")
        ret=http_get("https://api.thingspeak.com/channels/1538804/fields/2/last.json")
        #ret=http_get("https://api.thingspeak.com/channels/1538804/fields/2.json?results=3")
        mstr=str(ret, 'utf8')
        print(mstr[-69:])            # to show the received string
        data=json.loads(mstr[-69:])  # string into list; depends on the requested json: fields, ..
        #print(data["field1"])        # printing the field from  list
        print(data["field2"])
        print(data["entry_id"])
        print(data["created_at"])
        c+=1
        time.sleep(20)
    except Exception as e:
        print("ERROR: "+str(e))
        if (str(e) == "-202" or str(e) == "23"):  # page or socket errors  
            wifista.scan()
            wifista.disconnect()
            wifista.connect()
        time.sleep(1)
    


