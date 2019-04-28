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

mac_addr = "f0:b5:d1:5b:e0:63"  # Ben LeBrun's bot
# mac_addr = "88:3f:4a:f4:70:e8"  # Ben Garcia's bot

async def write_packet(client: BleakClient, left_forward, right_forward, left_speed, right_speed):
    lft_spd_enc = left_speed.to_bytes(1, 'little')
    rt_spd_enc = right_speed.to_bytes(1, 'little')
    lft_dir_enc = left_forward.to_bytes(1, 'little')
    rt_dir_enc = right_forward.to_bytes(1, 'little')
    packet = chr(left_forward) + chr(right_forward) + chr(left_speed) + chr(right_speed)
    checksumResult = checksum(packet)
    await client.write_gatt_char(GATT_CHARACTERISTIC, lft_dir_enc)
    await client.write_gatt_char(GATT_CHARACTERISTIC, rt_dir_enc)
    await client.write_gatt_char(GATT_CHARACTERISTIC, lft_spd_enc)
    await client.write_gatt_char(GATT_CHARACTERISTIC, rt_spd_enc)
    await client.write_gatt_char(GATT_CHARACTERISTIC, checksumResult.to_bytes(1, 'little'))
    await client.write_gatt_char(GATT_CHARACTERISTIC, ord('\n').to_bytes(1, 'little'))

    print(("wrote packet: %d, %d, %d, %d, %d"%(left_forward, right_forward, left_speed, right_speed, checksumResult)))

def checksum(data):
    check = 0
    for c in data:
        check = (check + ord(c) )% 255 
    return check


class DS4(object):
    controller = None
    axis_left : float
    axis_right : float
    speed_left : int
    speed_right : int
    dir_left = 1
    dir_right = 1
    quit = False

    def __init__(self):
        print("init controller")
        pygame.init()
        pygame.joystick.init()
        self.controller = pygame.joystick.Joystick(0)
        self.controller.init()
        self.speed_left = 0
        self.speed_right = 0
        self.axis_left = 0
        self.axis_right = 0

    async def read(self, events):
        for event in events:
            if event.type == pygame.QUIT:
                self.quit = True
                break
            elif event.type == pygame.JOYAXISMOTION:
                 self.axis_left = round(self.controller.get_axis(1), 3)
                 self.axis_right = round(self.controller.get_axis(3), 3)

        self.speed_left = self.convert_speed(self.axis_left)
        self.speed_right = self.convert_speed(self.axis_right)
        if self.speed_left < 0:
            self.dir_left = 0
            self.speed_left = -self.speed_left
        else:
            self.dir_left = 1
        if self.speed_right < 0:
            self.dir_right = 0
            self.speed_right = -self.speed_right
        else:
            self.dir_right = 1

    def convert_speed(self, speed : float):
        return int((-1) * speed * 255)


async def main(mac_addr: str, loop: asyncio.AbstractEventLoop, ds4: DS4):
    async with BleakClient(mac_addr, loop=loop) as client:
        print("connecting to bot")
        await client.connect()
        await client.get_services()
        print("finished connect")
        while not ds4.quit:
            await ds4.read(pygame.event.get())
            await write_packet(client, ds4.dir_left, ds4.dir_right, ds4.speed_left, ds4.speed_right)
            time.sleep(0.1)


if __name__ == "__main__":
    ds4 = DS4()
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main(mac_addr, loop, ds4))
