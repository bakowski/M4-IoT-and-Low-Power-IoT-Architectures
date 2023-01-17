import uasyncio as asyncio

class Foo:
    def __init__(self):
        self.data = 0

    async def acquire(self):
        await asyncio.sleep(1)
        return 42

    async def __aenter__(self):
        print('Waiting for data')
        self.data = await self.acquire()
        return self

    def close(self):
        print('Exit')

    async def __aexit__(self, *args):
        print('Waiting to quit')
        await asyncio.sleep(1)  # Can run asynchronous
        self.close()  # or synchronous methods

async def bar():
    foo = Foo()
    async with foo as f:
        print('In context manager')
        res = f.data
    print('Done', res)

asyncio.run(bar())
