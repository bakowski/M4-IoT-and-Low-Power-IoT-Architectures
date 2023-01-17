from esp_ble_uart import *
import time

nom = 'ESP32-ble-uart-gcworks'
UUID_UART = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_TX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_RX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

uart = Bleuart(nom, UUID_UART, UUID_TX, UUID_RX)
uart.close()

def rcp_rx():
    global val_rx
    val_rx = uart.read().decode().strip()
    print('sur rx: ', val_rx)               # Interruption : affichage données reçues

uart.irq(handler=rcp_rx)

def env_tx(val_tx):
    uart.write(str(val_tx) + '\n')
    print("tx", val_tx)

while True:
    env_tx("xxx")       # xxx données à envoyer au format String
    time.sleep_ms(1000)
