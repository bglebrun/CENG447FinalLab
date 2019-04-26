# uses python 3.6
import pygame
import sys
import os
import time
import asyncio

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
                sys.quit(0)
                break
            elif event.type == pygame.JOYAXISMOTION:
                axis_left = round(self.controller.get_axis(1),4)
                axis_right = round(self.controller.get_axis(3),4)


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

def main():
    loop = asyncio.get_event_loop()
    event_queue =asyncio.Queue()

    ds4 = DS4()
    ds4.init()
    
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