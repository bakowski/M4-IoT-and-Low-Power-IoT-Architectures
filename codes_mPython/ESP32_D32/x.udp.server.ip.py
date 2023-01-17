import socket
import wifista

sta=wifista.connect()
print('network config:', sta.ifconfig())
udp_host=sta.ifconfig()[0][-1]
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)      # For UDP

#udp_host = sta[0]       # Host IP
udp_port = 12345               # specified port to connect

#print type(sock) ============> 'type' can be used to see type 
# of any variable ('sock' here)

sock.bind((udp_host,udp_port))

while True:
    print('Waiting for client...')
    data,addr = sock.recvfrom(1024)	        #receive data from client
    print('Received Messages:',data,' from' ,addr)