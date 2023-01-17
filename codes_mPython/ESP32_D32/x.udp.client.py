import socket
import wifista

sta=wifista.connect()
print('network config:', sta.ifconfig())
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)      # For UDP

# udp_host = socket.gethostname() # Host IP
udp_host ='192.168.1.37'  # server IP

udp_port = 12345       # specified port to connect on server

msg = "Hello Python!"
print "UDP target IP:", udp_host
print "UDP target Port:", udp_port

sock.sendto(msg,(udp_host,udp_port))

