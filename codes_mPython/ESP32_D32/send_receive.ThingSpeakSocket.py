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
    
wifista.connect()
t=22.2
h=44.4
urlkey='https://api.thingspeak.com/update?api_key=3IN09682SQX3PT4Z'
fields='&field1='+str(t)+'&field2='+str(h)
#http_get('https://api.thingspeak.com/update?api_key=3IN09682SQX3PT4Z&field1=0')
http_get(urlkey+fields)
time.sleep(15)
http_get('https://api.thingspeak.com/channels/1626377/fields/2/last.json?api_key=9JVTP8ZHVTB9G4TT')

