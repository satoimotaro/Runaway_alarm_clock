from serial_comm import SerialComm
from core_XY import calc_displacement
import time
import math
import datetime
import pygame
from pathlib import Path

useWakeUpTimer = False

radius = 30
period = 4
speed = 200

def main():
    t=0

    if useWakeUpTimer:
        pygame.mixer.init()
        music_path1 = Path(__file__).parent.parent / "Clock-Alarm05-8(Far-Mid-Loop).mp3"
        pygame.mixer.music.load(str(music_path1))
        pygame.mixer.music.play(-1)
        print('enter your wake-up time.\nex)7:30')
        wakeUp=input(">>")
        while True:
            now=datetime.datetime.now().strftime("%H:%M")
            print(now)
            if now == wakeUp:
                break
            time.sleep(1)
        pygame.mixer.music.play(-1)
        print("wake up!!!!!")

    comm = SerialComm("COM9", 115200)
    time.sleep(2)

    while True:
        data = comm.read_data()
        if data is None:
            continue

        current_x, current_y, a,b,c,d, flag = data

        target_x, target_y, speed = calc_displacement(current_x, current_y, a,b,c,d)
        comm.send_move(target_x, target_y, speed)

        # target_x = float(input("target_x = "))
        # target_y = float(input("target_y = "))
        # speed = float(input("speed = "))
        # xy_values, sensor_values = comm.read_data()
        # target_x, target_y, speed = calc_displacement(xy_values, sensor_values)

        # target_x = radius*math.cos(2*math.pi/period*t)
        # target_y = radius*math.sin(2*math.pi/period*t)
        # comm.send_move(target_x, target_y, speed)
        # t +=0.05

        # print(target_x, target_y)
        # time.sleep(0.05)

        print('cur:',current_x,current_y,' target:',target_x,target_y, speed)

if __name__ == "__main__":
    main()
