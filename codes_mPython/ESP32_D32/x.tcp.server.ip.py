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
tcp_port = 12346               # specified port to connect

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
# Bind the socket to the port
server_address = (tcp_host, tcp_port)
sock.bind(server_address)
# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('waiting for a connection')
    disp("waiting","for","connection")
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)
        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(16)
            print('received "%s"' % data)
            disp("received","data",str(data))
            if data:
                print('sending data back to the client')
                connection.sendall(data)
            else:
                print('no more data from', client_address)
                break
            
    finally:
        # Clean up the connection
        connection.close()
        