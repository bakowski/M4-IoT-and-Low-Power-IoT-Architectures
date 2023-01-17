import uasyncio as asyncio
import sys

def _handle_exception(loop, context):
    print('Global handler')
    sys.print_exception(context["exception"])
    #loop.stop()
    sys.exit()  # Drastic - loop.stop() does not work when used this way

async def bar():
    await asyncio.sleep(0)
    1/0  # Crash

async def main():
    loop = asyncio.get_event_loop()
    loop.set_exception_handler(_handle_exception)
    asyncio.create_task(bar())
    for _ in range(5):
        print('Working')
        await asyncio.sleep(0.5)

asyncio.run(main())