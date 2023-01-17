from machine import Pin, SoftSPI


spi = SoftSPI(baudrate=100000, polarity=1, phase=0, sck=Pin(0), mosi=Pin(2), miso=Pin(4))


spi.init(baudrate=200000) # 

spi.read(10)            # 
spi.read(10, 0xff)      # 

buf = bytearray(50)     # 
spi.readinto(buf)       # 
spi.readinto(buf, 0xff) # 

spi.write(b'12345')     # 

buf = bytearray(4)      # 
spi.write_readinto(b'1234', buf) # 
spi.write_readinto(buf, buf) # 