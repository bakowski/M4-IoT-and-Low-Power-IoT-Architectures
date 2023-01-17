import socket
import wifista
import time

def http_get(url):
    import socket
    _, _, host, path = url.split('/', 3)
    print(path)
    print(host)
    addr = socket.getaddrinfo(host, 80)[0][-1]
    s = socket.socket()
    s.connect(addr)
    s.send(bytes('GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n' % (path, host), 'utf8'))
    while True:
               data = s.recv(100)
               if data:
                 print(str(data, 'utf8'), end='')
               else:
                 break
    s.close()
    t=2.2
h=4.4
c=0
while c<10:
    wifista.connect()
    urlkey='https://api.thingspeak.com/update?api_key=YOX31M0EDKO0JATK'
    fields='&field1='+str(t)+'&field2='+str(h)
    http_get(urlkey+fields)
    time.sleep(15)
    http_get('https://api.thingspeak.com/channels/1538804/fields/2/last.json?api_key=20E9AQVFW7Z6XXOM')
    t=t+1.0
    h=h+2.0
    c=c+1
    time.sleep(25)
    
    