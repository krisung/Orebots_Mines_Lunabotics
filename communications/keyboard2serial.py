import keyboard
import time
import socket

command = "0"
newCommand = "0"

while True:
    if keyboard.is_pressed('esc'):
        break        
    elif keyboard.is_pressed('s'):
            newCommand = "1"
    elif keyboard.is_pressed('w'):
            newCommand = "2"
    elif keyboard.is_pressed('a'):
            newCommand = "3"
    elif keyboard.is_pressed('d'):
            newCommand = "4"
    else:
            newCommand = "0"

    if (newCommand != command):
        command = newCommand
        if (command == "0"):
            print("Stopped")
        elif (command == "1"):
            print("Moving Forward")
        elif (command == "2"):
            print("Moving Backwards")
        elif (command == "3"):
            print("Rotating Left")
        elif (command == "4"):
            print("Rotating Right")