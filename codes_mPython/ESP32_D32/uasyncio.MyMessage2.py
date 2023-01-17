from message import Message
import uasyncio as asyncio

async def bar(msg, n):
    while True:
        res = await msg
        msg.clear()
        print(n, res)
        # Pause until other coros waiting on msg have run and before again
        # awaiting a message.
        await asyncio.sleep_ms(0)

async def main():
    msg = Message()
    for n in range(5):
        asyncio.create_task(bar(msg, n))
    k = 0
    while True:
        k += 1
        await asyncio.sleep_ms(1000)
        msg.set('Hello {}'.format(k))

asyncio.run(main())