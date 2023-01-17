import machine, ssd1306
from machine import Pin, SoftI2C

import socket
import wifista

def disp(p0,p1,p2):
    i2c = SoftI2C(scl=Pin(14), sda=Pin(12), freq=100000)
    oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0x3c)
    oled.fill(0)
    oled.text("SmartComputerLab", 0, 0)
    oled.text(p0, 0, 16)
    oled.text(p1, 0, 32)
    oled.text(p2, 0, 48)
    #oled.text("{}".format(p), 0, 48)
    oled.show()

sta=wifista.connect()
print('network config:', sta.ifconfig())
tcp_host=sta.ifconfig()[0]
tcp_port = 12346
tcp_server_IP = '192.168.1.34' # specified server IP to connect

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 

# Connect the socket to the port where the server is listening
server_address = (tcp_server_IP, tcp_port)
print('connecting to %s port %s' % server_address)
sock.connect(server_address)

try:
    
    # Send data
    message = "This is the message.  It will be repeated."
    print('sending "%s"' % message)
    sock.sendall(message)

    # Look for the response
    amount_received = 0
    amount_expected = len(message)
    
    while amount_received < amount_expected:
        data = sock.recv(16)
        amount_received += len(data)
        print('received "%s"' % data)

finally:
    print('closing socket')
    sock.close()
    