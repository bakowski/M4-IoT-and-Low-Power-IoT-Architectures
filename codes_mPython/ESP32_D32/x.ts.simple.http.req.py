import socket
import wifista
import time
import json

def http_get(url):
    scheme, _, host, path = url.split('/', 3)
    s = socket.socket()
    s.connect((host, 80))
    request=bytes('GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n' % (path, host), 'utf8')     
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
                     return val 
         else:
            break
    s.close()
    return val

c=0

while c<100:
    wifista.scan()
    wifista.disconnect()
    wifista.connect()
    try:
        ret=http_get("https://api.thingspeak.com/channels/1538804/fields/1/last.json")
        mstr=str(ret, 'utf8')
        print(mstr[-69:])            # to show the received string
        data=json.loads(mstr[-69:])  # depends on the requested json: fields, ..
        print(data["field1"])
        print(data["entry_id"])
        print(data["created_at"])
        c+=1
        time.sleep(30)
    except Exception as e:
        print("ERROR: "+str(e))
        time.sleep(30)
    


