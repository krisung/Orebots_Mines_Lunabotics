from pynput import keyboard
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
# C - Extend Linear Actuators
# D - Contract Linear Actuators
# E - YEET Collection System
# F - Adjust Collection System

command = "0"
newCommand = "0"

def on_press(key):
    global command
    global newCommand
    try:
        k = key.char

        if k == 'w':
            newCommand = "1"
        elif k == 's':
            newCommand = "2"
        elif k == 'a':
            newCommand = "3"
        elif k == 'd':
            newCommand = "4"
        elif k == 'h':
            newCommand = "5"
        elif k == 'y':
            newCommand = "6"
        elif k == 'l':
            newCommand = "7"
        elif k == 'j':
            newCommand = "8"
        elif k == 'u':
            newCommand = "9"
        elif k == 'k':
            newCommand = "A"
        elif k == 'i':
            newCommand = "B"
        elif k == 't':
            newCommand = "C"
        elif k == 'g':
            newCommand = "D"
        elif k == 'b':
            newCommand = "E"
        elif k == 'n':
            newCommand = "F"
    except:
        k = key.name
        newCommand = command

    if(newCommand != command):
        command = newCommand
        if (command == "1"):
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
            print("Lower Bottom Stepper")
        elif (command == "9"):
            print("Raise Bottom Stepper")
        elif (command == "A"):
            print("Lower Top Stepper")
        elif (command == "B"):
            print("Raise Top Stepper")
        elif (command == "C"):
            print("Extending Linear Actuators")
        elif (command == "D"):
            print("Contracting Linear Actuators")
        elif (command == "E"):
            print("Rotating Collection Belt -- YEET SPEED")
        elif (command == "F"):
            print("Rotating Collection Belt -- Adjust Speed")
        sock.send(bytes(command, "UTF-8")) # Sends command when new 

def on_release(key):
    global command
    command = "0"
    print("Stopped")
    sock.send(bytes(command, "UTF-8")) # Sends command when new 


print("Ground Station initialized")

# Connects to the Rover
sock.connect((host, port))

print("Rover Connected, press esc to disconnect")

with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()



# sock.close()
# print("Rover Disconnected")

# while True:
#     if keyboard.is_pressed('esc'):
#         break        
#     elif keyboard.is_pressed('w'):
#             newCommand = "1"
#     elif keyboard.is_pressed('s'):
#             newCommand = "2"
#     elif keyboard.is_pressed('a'):
#             newCommand = "3"
#     elif keyboard.is_pressed('d'):
#             newCommand = "4"
#     elif keyboard.is_pressed('h'):
#             newCommand = "5"
#     elif keyboard.is_pressed('y'):
#             newCommand = "6"
#     elif keyboard.is_pressed('l'):
#             newCommand = "7"
#     elif keyboard.is_pressed('j'):
#             newCommand = "8"
#     elif keyboard.is_pressed('u'):
#             newCommand = "9"
#     elif keyboard.is_pressed('k'):
#             newCommand = "A"
#     elif keyboard.is_pressed('i'):
#             newCommand = "B"
#     else:
#             newCommand = "0"
#     if (newCommand != command):
#         command = newCommand
#         if (command == "0"):
#             print("Stopped")
#         elif (command == "1"):
#             print("Moving Forward")
#         elif (command == "2"):
#             print("Moving Backwards")
#         elif (command == "3"):
#             print("Rotating Left")
#         elif (command == "4"):
#             print("Rotating Right")
#         elif (command == "5"):
#             print("Deploying")
#         elif (command == "6"):
#             print("Undeploying")
#         elif (command == "7"):
#             print("Start/Stopping Auger")
#         elif (command == "8"):
#             print("Lower Bottom Stepper")
#         elif (command == "9"):
#             print("Raise Bottom Stepper")
#         elif (command == "A"):
#             print("Lower Top Stepper")
#         elif (command == "B"):
#             print("Raise Top Stepper")
#         sock.send(bytes(command, "UTF-8")) # Sends command when new 