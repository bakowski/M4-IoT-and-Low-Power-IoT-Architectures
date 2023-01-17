from machine import Pin
import usocket as socket
import wifista

def web_page():
    pot = 55
    print("CAN =", pot)
    html = """
    <!DOCTYPE html>
    <html>
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>My Web Server</title>
            <style>
                p { font-size: 36px; }
            </style>
        </head>
        <body>  
            <h2>variable= </h2>                
            <p><span>""" + str(pot) + """</span></p>
        </body>
    </html>
    """
    return html

wifista.connect()
socketServeur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socketServeur.bind(('', 80))
socketServeur.listen(5)


while True:
    try:

   if gc.mem_free() < 102000:
        gc.collect()
        print("Waiting connection request from client")
        connexionClient, adresse = socketServeur.accept()
        connexionClient.settimeout(4.0)
        print("Connected to client", adresse)
        print("Waiting for request from client")
        requete = connexionClient.recv(1024)     #requête du client
        requete = str(requete)
        print("Requete du client = ", requete)
        connexionClient.settimeout(None)
        print("Server sending response  : HTML code to print")
        connexionClient.send('HTTP/1.1 200 OK\n')
        connexionClient.send('Content-Type: text/html\n')
        connexionClient.send("Connection: close\n\n")
        reponse = web_page()
        connexionClient.sendall(reponse)
        connexionClient.close()  
        print("Closing connection with client")
       
    except:
        connexionClient.close()  
        print("Connection with client closed, communication error")