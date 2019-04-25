# uses python 3.7
import pygame
import asyncio
import sys
import os
import time
from bleak import BleakClient
import numpy as np

# the service and characteristic UUIDs for our bluetooth module's UART
GATT_SERVICE = "0000ffe0-0000-1000-8000-00805f9b34fb"
GATT_CHARACTERISTIC = "0000ffe1-0000-1000-8000-00805f9b34fb"
mac_addr = "f0:b5:d1:5b:e0:63"  # CHANGE THIS to your BT's MAC

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

class DS4(object):
    axis_left = None
    axis_right = None

    def init(self):
        pygame.init()
        pygame.joystick.init()
        self.controller = pygame.joystick.Joystick(0)
        self.controller.init()

    async def read(self, event_queue):
        #Axis 1: left sticks
        #Axis 3: right sticks
        while 1:
            event = await event_queue.get()
            if event.type == pygame.QUIT:
                break
            elif event.type == pygame.JOYAXISMOTION:
                axis_left = round(self.controller.get_axis(1),3)
                axis_right = round(self.controller.get_axis(3),3)


            # print (axis_data)
            # writing to vars goes here
            print("Left:")
            print(axis_left)
            print("Right:")
            print(axis_right)
            os.system('cls')

        asyncio.get_event_loop()

def io_event_loop(loop, event_queue):
    while 1:
        event = pygame.event.wait()
        asyncio.run_coroutine_threadsafe(event_queue.put(event), loop=loop)

def main(mac_addr: str, loop: asyncio.AbstractEventLoop):
    ds4 = DS4()
    ds4.init()

    async with BleakClient(mac_addr, loop=loop) as client:
        await client.connect()
        await client.get_services()
        print("connected.")

        loop = asyncio.get_event_loop()
        event_queue =asyncio.Queue()
        
        pygame_task = loop.run_in_executor(None, io_event_loop, loop, event_queue)
        ds4_task = asyncio.ensure_future(ds4.read(event_queue))
        try:
            loop.run_forever()
        except pygame.JOYAXISMOTION:
            pass
        finally:
            pygame_task.cancel()
            ds4_task.cancel()
        
        pygame.quit()

if __name__ == "__main__":
    main()