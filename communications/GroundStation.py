import keyboard
import time
import socket

sock = socket.socket()
host = "192.168.0.103"
port = 80

# Command Prompts
# 0 - IDLE
# 1 - Forward
# 2 - Backward
# 3 - Left
# 4 - Right
# 5 - Deploy Systems
# 6 - Begin/End Auger
# 7 - Begin/End Conveyor

command = "0"
newCommand = "0"
augerState = 0
conveyorState = 0


print("Ground Station initialized")

sock.connect((host, port))

print("Rover Connected, press esc to disconnect")

while True:
    if keyboard.is_pressed('esc'):
        break        
    elif keyboard.is_pressed('w'):
            newCommand = "1"
    elif keyboard.is_pressed('s'):
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

        sock.send(bytes(command, "UTF-8")) 

sock.close()

print("Rover Disconnected")