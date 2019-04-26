# uses python 3.7
import pygame
from bleak import BleakClient
import asyncio
import sys
import os
import time
import numpy as np


# the service and characteristic UUIDs for our bluetooth module's UART
GATT_SERVICE = "0000ffe0-0000-1000-8000-00805f9b34fb"
GATT_CHARACTERISTIC = "0000ffe1-0000-1000-8000-00805f9b34fb"

mac_addr = "f0:b5:d1:5b:e0:63"  # CHANGE THIS to your BT's MAC

class DS4(object):
    axis_left = None
    axis_right = None
    client = None
    quit = False

    def __init___(self):
        pygame.init()
        pygame.joystick.init()
        self.controller = pygame.joystick.Joystick(0)
        self.controller.init()

    def set_client(self, clientObj):
        if self.client:
            print("client already initialized, plz only initialize once")
        else:
            print("client initialized")
            self.client = clientObj

    def write_packet(self, left_forward, right_forward, left_speed, right_speed):
        if not self.client:
            print("please initialize client before invoking DS4")
        else:
            left_dir = left_forward.to_bytes(1, 'little')
            left_spd = left_speed.to_bytes(1, 'little')
            right_dir = right_forward.to_bytes(1, 'little')
            right_spd = right_speed.to_bytes(1, 'little')
            self.client.write_gatt_char(GATT_CHARACTERISTIC, left_dir)
            self.client.write_gatt_char(GATT_CHARACTERISTIC, left_spd)
            self.client.write_gatt_char(GATT_CHARACTERISTIC, right_dir)
            self.client.write_gatt_char(GATT_CHARACTERISTIC, right_spd)
            print("wrote packet: {0} | {1} | {2} | {3}".format(
                left_dir, left_spd, right_dir, right_spd))

    async def read(self, event_queue):
        #Axis 1: left sticks
        #Axis 3: right sticks
        print("starting read")
        #async with self.client:
        while 1:
            event = await event_queue.get()
            if event.type == pygame.QUIT:
                self.quit = True
                break
            elif event.type == pygame.JOYAXISMOTION:
                axis_left = round(self.controller.get_axis(1),3)
                axis_right = round(self.controller.get_axis(3),3)


            # print (axis_data)
            # writing to vars goes here
            print("Left:")
            print(axis_left)
            self.write_packet(1, 0, self.convert_speed(axis_left), 0)
            print("Right:")
            print(axis_right)
            self.write_packet(0, 1, 0, self.convert_speed(axis_left))
            os.system('cls')

        asyncio.get_event_loop()

    def convert_speed(self, speed):
        return (-1) * speed * 255

    def run_forever(self):
        while not self.quit:
            pass

def io_event_loop(loop, event_queue):
    while 1:
        event = pygame.event.wait()
        asyncio.run_coroutine_threadsafe(event_queue.put(event), loop=loop)

async def main(mac_Addr: str, loop: asyncio.AbstractEventLoop):
   async with BleakClient(mac_Addr, loop=loop) as client:
        print("connecting to bot")
        await client.connect()
        await client.get_services()
        print("finished connect")
        ds4 = DS4()
        ds4.set_client(client)
        ds4_task = asyncio.ensure_future(ds4.read(event_queue))
        
        pygame_task = loop.run_in_executor(None, io_event_loop, loop, event_queue)

        ds4.run_forever()
        exit_program(pygame_task, ds4_task)

def exit_program(pygame_task, ds4_task):
    pygame_task.cancel()
    ds4_task.cancel()
    pygame.quit()

if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    event_queue = asyncio.Queue()
    loop.run_until_complete(main(mac_addr, loop))
