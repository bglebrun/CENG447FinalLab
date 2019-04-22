import asyncio
from bleak import discover

## use this procedure to discover all of the bluetooth devices nearby and
## and get their mac's.
async def run():
	devs = await discover()
	for d in devs:
		print(d)

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
