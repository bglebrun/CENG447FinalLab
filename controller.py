import asyncio
from bleak import BleakClient
from readchar import readkey
import numpy as np
from time import sleep

# the service and characteristic UUIDs for our bluetooth module's UART
GATT_SERVICE = "0000ffe0-0000-1000-8000-00805f9b34fb"
GATT_CHARACTERISTIC = "0000ffe1-0000-1000-8000-00805f9b34fb"


async def write_packet(client: BleakClient, left_forward, right_forward, left_speed, right_speed):
    left_dir = left_forward.to_bytes(1, 'little')
    left_spd = left_speed.to_bytes(1, 'little')
    right_dir = right_forward.to_bytes(1, 'little')
    right_spd = right_speed.to_bytes(1, 'little')
    await client.write_gatt_char(GATT_CHARACTERISTIC, left_dir)
    await client.write_gatt_char(GATT_CHARACTERISTIC, left_spd)
    await client.write_gatt_char(GATT_CHARACTERISTIC, right_dir)
    await client.write_gatt_char(GATT_CHARACTERISTIC, right_spd)
    print("wrote packet: {0} | {1} | {2} | {3}".format(
        left_dir, left_spd, right_dir, right_spd))

# connects to and writes to the bluetooth device listed


async def main(mac_addr: str, loop: asyncio.AbstractEventLoop):
    async with BleakClient(mac_addr, loop=loop) as client:
        await client.connect()
        await client.get_services()
        print("connected.")
        while True:
            await write_packet(client, 1, 1, 64, 64)
            sleep(5)
            await write_packet(client, 0, 0, 64, 64)
            sleep(5)
            await write_packet(client, 0, 1, 64, 128)
            sleep(5)
        # print('Connected. Type to send keystrokes. Press ESC to quit.')
        # k = await getkey()
        # while ord(k) != 27:
        #     await client.write_gatt_char(GATT_CHARACTERISTIC, k.encode('ascii'))
        #     k = await getkey()


async def getkey():
    return readkey()

# mac_addr = "f0:b5:d1:5b:e0:63"  # CHANGE THIS to your BT's MAC
mac_addr = "88:3f:4a:f4:70:e8"
loop = asyncio.get_event_loop()
loop.run_until_complete(main(mac_addr, loop))
