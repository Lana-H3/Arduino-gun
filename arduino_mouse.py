import serial
import mouse
import time 
from pynput.keyboard import Controller, Key

PORT = "COM3"
BAUD_RATE = 9600

keyboard = Controller()

ser = serial.Serial(PORT, BAUD_RATE)
time.sleep(2)

print("connected to arduino")

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        print("Received: ", line)

        parts = line.split()

        if len(parts) == 0:
            continue

        command = parts[0]

        if command == "MOVE" and len(parts) == 3:
            x = int(parts[1])
            y = int(parts[2])
            mouse.move(x, y, absolute=False)

        elif command == "SHOOT":
            mouse.click()

        elif command == "RELOAD":
            keyboard.press('r')
            keyboard.release('r')