import uasyncio as asyncio

async def bar():
    await asyncio.sleep(0)
    1/0  # Crash

async def foo():
    await asyncio.sleep(0)
    print('Running bar')
    await bar()
    print('Does not print')  # Because bar() raised an exception

async def main():
    asyncio.create_task(foo())
    for _ in range(5):
        print('Working')  # Carries on after the exception
        await asyncio.sleep(0.5)
    1/0  # Stops the scheduler
    await asyncio.sleep(0)
    print('This never happens')
    await asyncio.sleep(0)

asyncio.run(main())
