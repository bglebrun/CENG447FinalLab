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

    def __init__(self):
        print("init controller")
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

    def read(self, events):
        for event in events:
            if event.type == pygame.QUIT:
                self.quit = True
                break
            elif event.type == pygame.JOYAXISMOTION:
                axis_left = round(self.controller.get_axis(1), 3)
                axis_right = round(self.controller.get_axis(3), 3)

            speed_left = self.convert_speed(axis_left)
            speed_right = self.convert_speed(axis_right)
            dir_left = 1
            dir_right = 1
            if speed_left < 0:
                dir_left = 1
                speed_left = -speed_left
            if speed_right < 0:
                dir_right = 1
                speed_right = -speed_right

            self.write_packet(dir_left, dir_right, speed_left, speed_right)
            # print("Left:")
            # print(speed_left)
            # if speed_left > 0:
            #     self.write_packet(1, 0,abs(speed_left), 0)
            # else:
            #     self.write_packet(0, 0, abs(speed_left), 0)
            # print("Right:")
            # print(speed_right)
            # if speed_right > 0:
            #     self.write_packet(0, 1, 0, abs(speed_right))
            # else:
            #     self.write_packet(0, 0, 0, abs(speed_right))
            os.system('cls')

    def convert_speed(self, speed):
        return int((-1) * speed * 255)


async def main(mac_addr: str, loop: asyncio.AbstractEventLoop, ds4: DS4):
    async with BleakClient(mac_addr, loop=loop) as client:
        print("connecting to bot")
        await client.connect()
        await client.get_services()
        print("finished connect")
        ds4.set_client(client)
        while True:
            ds4.read(pygame.event.get())


if __name__ == "__main__":
   # loop = asyncio.get_event_loop()
   # event_queue = asyncio.Queue()
   # loop.run_until_complete(main(mac_addr, loop))
    ds4 = DS4()
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main(mac_addr, loop, ds4))
   # main(loop)
