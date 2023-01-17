from machine import Pin
import wifista
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


wifista.connect()
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
            
        print("Attente connexion d'un client")
        connexionClient, adresse = socketServeur.accept()
        connexionClient.settimeout(4.0)
        print("Connecté avec le client", adresse)

        print("Attente requete du client")
        requete = connexionClient.recv(1024)     #requête du client
        requete = str(requete)
        print("Requete du client = ", requete)
        connexionClient.settimeout(None)
        
        #analyse de la requête, recherche de led=on ou led=off
        if "GET /?led=green" in requete:
            print("LED GREEN")
            pixels[0] = (0x00, 0xff, 0x00)  # RGB GREEN max
            pixels.write()
        if "GET /?led=red" in requete:
            print("LED RED")
            pixels[0] = (0xff, 0x0, 0x00)  # RGB RED max
            pixels.write()
        if "GET /?led=blue" in requete:
            print("LED BLUE")
            pixels[0] = (0x00, 0x0, 0xff)  # RGB BLUE max
            pixels.write()
        if "GET /?led=bblack" in requete:
            print("LED BLACK")
            pixels[0] = (0x00, 0x0, 0x00)  # RGB BLACK max
            pixels.write()
            
        print("Envoi reponse du serveur : code HTML a afficher")
        connexionClient.send('HTTP/1.1 200 OK\n')
        connexionClient.send('Content-Type: text/html\n')
        connexionClient.send("Connection: close\n\n")
        reponse = web_page()
        connexionClient.sendall(reponse)
        connexionClient.close()  
        print("Connexion avec le client fermee")
        
    except:
        connexionClient.close()  
        print("Connexion avec le client fermee, le programme a declenché une erreur")
   