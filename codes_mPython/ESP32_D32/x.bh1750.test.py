import time
import machine
OP_SINGLE_HRES1 = 0x20
OP_SINGLE_HRES2 = 0x21
OP_SINGLE_LRES = 0x23
DELAY_HMODE = 180 # 180ms in H-mode
DELAY_LMODE = 24 # 24ms in L-mode

def sample(i2c, mode=OP_SINGLE_HRES1, i2c_addr=0x23):
    i2c.writeto(i2c_addr, b"\x00") # make sure device is in a clean state
    i2c.writeto(i2c_addr, b"\x01") # power up
    i2c.writeto(i2c_addr, bytes([mode])) # set measurement mode
    time.sleep_ms(DELAY_LMODE if mode == OP_SINGLE_LRES else DELAY_HMODE)
    raw = i2c.readfrom(i2c_addr, 2)
    i2c.writeto(i2c_addr, b"\x00") # power down again
    # we must divide the end result by 1.2 to get the lux
    return ((raw[0] << 24) | (raw[1] << 16)) // 78642

sda=machine.Pin(21)
scl=machine.Pin(22)
i2c=machine.I2C(0,sda=sda,scl=scl,freq=400000) #I2C channel 0,pins,400kHz max
lum=sample(i2c,mode=OP_SINGLE_HRES1, i2c_addr=0x23)
print('lum')
print(lum)


    
    