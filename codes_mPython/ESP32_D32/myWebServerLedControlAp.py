from machine import Pin
import network
import usocket as socket
import neopixel
 
pixels = neopixel.NeoPixel(Pin(8, Pin.OUT), 1)
pixels[0] = (0xff, 0x00, 0x22)
pixels.write()

def web_page():
    html = """
    <!DOCTYPE html>
    <html>
        <head 
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>ESP32 Serveur Web</title>
            <style>
                p { font-size: 36px; }
            </style>
        </head>
        <body>
            <h1>Commande LED</h1>
            <p><a href="/?led=green">LED GREEN</a></p>
            <P><a href="/?led=red">LED RED</a></p>
            <p><a href="/?led=blue">LED BLUE</a></p>
            <p><a href="/?led=black">LED BLACK</a></p>
        </body>
    </html>
       """
    return html

ssid="MyAP"
password="smartlab"
ap = network.WLAN(network.AP_IF)
ap.active(True)
ap.config(essid=ssid, password=password)
print(ap.ifconfig())

pixels = neopixel.NeoPixel(Pin(8, Pin.OUT), 1)
pixels[0] = (0xff, 0xff, 0xff)  # blanc
pixels.write()
socketServeur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socketServeur.bind(('', 80))
socketServeur.listen(5)

while True:
    try:    
        if gc.mem_free() < 102000:
            gc.collect()
            
        print("Waiting for client")
        connexionClient, adresse = socketServeur.accept()
        connexionClient.settimeout(4.0)
        print("Connected to client", adresse)

        print("Waiting for request from  client")
        request = connexionClient.recv(1024)     # client request
        request = str(request)
        print("Client request = ", request)
        connexionClient.settimeout(None)
        
        # request analyzis led=on or led=off
        if "GET /?led=green" in request:
            print("LED GREEN")
            pixels[0] = (0x00, 0xff, 0x00)  # RGB GREEN max
            pixels.write()
        if "GET /?led=red" in request:
            print("LED RED")
            pixels[0] = (0xff, 0x0, 0x00)  # RGB RED max
            pixels.write()
        if "GET /?led=blue" in request:
            print("LED BLUE")
            pixels[0] = (0x00, 0x0, 0xff)  # RGB BLUE max
            pixels.write()
        if "GET /?led=black" in request:
            print("LED BLACK")
            pixels[0] = (0x00, 0x0, 0x00)  # RGB BLACK max
            pixels.write()
            
        print("Server response : HTML code to display")
        connexionClient.send('HTTP/1.1 200 OK\n')
        connexionClient.send('Content-Type: text/html\n')
        connexionClient.send("Connection: close\n\n")
        response = web_page()
        connexionClient.sendall(response)
        connexionClient.close()  
        print("Connection to client closed")
        
    except:
        connexionClient.close()  
        print("Connection to client closed, error reporting")
        
        