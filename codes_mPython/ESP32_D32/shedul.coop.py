import time

def print_time():
    while True:
        print(time.ctime())
        time.sleep(5)

def echo_input():
    while True:
        message = input()
        print(message.upper())