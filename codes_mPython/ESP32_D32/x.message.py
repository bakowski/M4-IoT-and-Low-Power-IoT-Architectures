import uasyncio as asyncio
from primitives.message import Message

async def waiter(msg):
    print('Waiting for message')
    res = await msg
    print('waiter got', res)
    msg.clear()

async def main():
    msg = Message()
    asyncio.create_task(waiter(msg))
    await asyncio.sleep(1)
    msg.set('Hello')  # Optional arg
    await asyncio.sleep(1)

asyncio.run(main())

