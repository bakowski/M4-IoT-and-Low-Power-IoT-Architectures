import uasyncio as asyncio
from delay_ms import Delay_ms

async def my_app():
    d = Delay_ms(callback, ('Callback running',))
    print('Holding off callback')
    for _ in range(10):  # Hold off for 5 secs
        await asyncio.sleep_ms(500)
        d.trigger()
    print('Callback will run in 1s')
    await asyncio.sleep(2)
    print('Done')

def callback(v):
    print(v)

try:
    asyncio.run(my_app())
finally:
    asyncio.new_event_loop()  # Clear retained state
    
    