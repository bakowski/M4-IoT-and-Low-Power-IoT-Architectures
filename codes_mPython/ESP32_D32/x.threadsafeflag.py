import uasyncio as asyncio
from pyb import Timer

tsf = asyncio.ThreadSafeFlag()

def cb(_):
    tsf.set()

async def foo():
    while True:
        await tsf.wait()
        # Could set an Event here to trigger multiple tasks
        print('Triggered')

tim = Timer(1, freq=1, callback=cb)

asyncio.run(foo())
