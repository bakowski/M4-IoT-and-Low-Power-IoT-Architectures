5.1.2 Keyboard interrupts

There is a "gotcha" illustrated by the following code sample. If allowed to run to completion it works as expected.

import uasyncio as asyncio
async def foo():
    await asyncio.sleep(3)
    print('About to throw exception.')
    1/0

async def bar():
    try:
        await foo()
    except ZeroDivisionError:
        print('foo was interrupted by zero division')  # Happens
        raise  # Force shutdown to run by propagating to loop.
    except KeyboardInterrupt:
        print('foo was interrupted by ctrl-c')  # NEVER HAPPENS
        raise

async def shutdown():
    print('Shutdown is running.')  # Happens in both cases
    await asyncio.sleep(1)
    print('done')

try:
    asyncio.run(bar())
except ZeroDivisionError:
    asyncio.run(shutdown())
except KeyboardInterrupt:
    print('Keyboard interrupt at loop level.')
    asyncio.run(shutdown())
