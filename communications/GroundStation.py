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
# 5 - Rotate Auger for Deployment
# 6 - Undeploy Auger
# 7 - Begin/Stop rotating auger
# 8 - Lower Stage Auger Down
# 9 - Lower Stage Auger Up
# A - Upper Stage Auger Down
# B - Upper Stage Auger Up

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
    elif keyboard.is_pressed('s'):
            newCommand = "1"
    elif keyboard.is_pressed('w'):
            newCommand = "2"
    elif keyboard.is_pressed('a'):
            newCommand = "3"
    elif keyboard.is_pressed('d'):
            newCommand = "4"
    elif keyboard.is_pressed('h'):
            newCommand = "5"
    elif keyboard.is_pressed('y'):
            newCommand = "6"
    elif keyboard.is_pressed('l'):
            newCommand = "7"
    elif keyboard.is_pressed('j'):
            newCommand = "8"
    elif keyboard.is_pressed('u'):
            newCommand = "9"
    elif keyboard.is_pressed('k'):
            newCommand = "A"
    elif keyboard.is_pressed('i'):
            newCommand = "B"
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
        elif (command == "5"):
            print("Deploying")
        elif (command == "6"):
            print("Undeploying")
        elif (command == "7"):
            print("Start/Stopping Auger")
        elif (command == "8"):
            print("Lower Top Stepper")
        elif (command == "9"):
            print("Raise Top Stepper")
        elif (command == "A"):
            print("Lower Bottom Stepper")
        elif (command == "B"):
            print("Raise Bottom Stepper")
        sock.send(bytes(command, "UTF-8")) 
sock.close()
print("Rover Disconnected")