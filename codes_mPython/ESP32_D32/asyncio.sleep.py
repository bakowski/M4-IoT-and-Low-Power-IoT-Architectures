import asyncio
import sys
import time

async def print_time():
    while True:
        print(time.time())
        await asyncio.sleep(5)

def echo_input():
    print(input().upper())

async def main():
    asyncio.get_event_loop().add_reader(
        sys.stdin,
        echo_input
    )
    await print_time()

asyncio.run(main())

