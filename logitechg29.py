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


async def write_packet(client: BleakClient, left_forward:int, left_speed:int, right_forward:int, right_speed:int):
    left_dir = int(left_forward).to_bytes(1, 'little')
    left_spd = int(left_speed).to_bytes(1, 'little')
    right_dir = int(right_forward).to_bytes(1, 'little')
    right_spd = int(right_speed).to_bytes(1, 'little')
    await client.write_gatt_char(GATT_CHARACTERISTIC, '\n'.encode('ascii'))
    await client.write_gatt_char(GATT_CHARACTERISTIC, left_dir)
    await client.write_gatt_char(GATT_CHARACTERISTIC, left_spd)
    await client.write_gatt_char(GATT_CHARACTERISTIC, right_dir)
    await client.write_gatt_char(GATT_CHARACTERISTIC, right_spd)
    print("wrote packet: %4d | %4d | %4d | %4d" %
          (left_forward, left_speed, right_forward, right_speed))


class DS4(object):
    controller = None
    accel: float
    brake: float
    wheel: float
    speed_left: int
    speed_right: int
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
        self.accel = 0.0
        self.brake = 0.0
        self.wheel = 0.0

    async def read(self, events):
        for event in events:
            if event.type == pygame.QUIT:
                self.quit = True
                break
            elif event.type == pygame.JOYAXISMOTION:
                self.accel = round(np.interp((-1) * self.controller.get_axis(2), [-1.0,1.0], [0.0, 1.0]), 3)
                self.brake = round(np.interp((-1) * self.controller.get_axis(3), [-1.0,1.0], [0.0, 1.0]), 3)
                self.wheel = round(self.controller.get_axis(0), 3)

        if self.accel > 0:
            self.dir_left = 1
            self.dir_right = 1
            #positive wheel->right
            if self.wheel > 0:
                self.speed_left = max(self.convert_speed(self.accel) - (self.wheelRate() * 255), 0)
                self.speed_right = self.convert_speed(self.accel)
            #negative wheel->left
            elif self.wheel < 0:
                self.speed_left = self.convert_speed(self.accel)
                self.speed_right = max(self.convert_speed(self.accel) - (self.wheelRate() * 255), 0)
            #no turn, raw speed
            else:
                self.speed_left = self.convert_speed(self.accel)
                self.speed_right = self.convert_speed(self.accel)
        elif self.brake > 0:
            self.dir_left = 0
            self.dir_right = 0
            #positive wheel->right
            if self.wheel > 0:
                self.speed_left = max(self.convert_speed(self.brake) - (self.wheelRate() * 255), 0)
                self.speed_right = self.convert_speed(self.brake)
            #negative wheel->left
            elif self.wheel < 0:
                self.speed_left = self.convert_speed(self.brake)
                self.speed_right = max(self.convert_speed(self.brake) - (self.wheelRate() * 255), 0)
            #no turn, pedal to the metal
            else:
                self.speed_left = self.convert_speed(self.brake)
                self.speed_right = self.convert_speed(self.brake)

    def convert_speed(self, speed: float):
        return abs(int(speed * 255))
    
    def wheelRate(self):
        if abs(self.wheel) > 0.20:
            return 1
        else:
            return (abs(self.wheel)/.20)


async def main(mac_addr: str, loop: asyncio.AbstractEventLoop, ds4: DS4):
    async with BleakClient(mac_addr, loop=loop) as client:
        print("connecting to bot")
        await client.connect()
        await client.get_services()
        print("finished connect")
        while not ds4.quit:
            await ds4.read(pygame.event.get())
            await write_packet(client, ds4.dir_right,  ds4.speed_right, ds4.dir_left,  ds4.speed_left)
            time.sleep(0.1)


if __name__ == "__main__":
    ds4 = DS4()
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main(mac_addr, loop, ds4))
