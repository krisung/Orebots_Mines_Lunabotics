#---------------------------------------------------------------
#   GroundStation.py
#---------------------------------------------------------------
#   Written By: Kristen Ung, CSMOrebots
#   Email: kristen.ung23@gmail.com
#
#   The purpose of this program is to be the ground station
#   for the lunar rover for the NASA Lunabotics Competition
#   for Colorado School of Mines, sending command packets 
#   over WiFi. The program connects to the rover through the
#   socket library. The program then uses a keyboard listener
#   to track keyboard inputs to send specific commands to 
#   the rover.
#
#   Command Prompts
#   release -> 0 -> IDLE
#   w       -> 1 -> Forward
#   s       -> 2 -> Backward
#   a       -> 3 -> Left
#   d       -> 4 -> Right
#   h       -> 5 -> Rotate Auger for Deployment
#   y       -> 6 -> Undeploy Auger
#   l       -> 7 -> Begin/Stop rotating auger
#   j       -> 8 -> Lower Stage Auger Down
#   u       -> 9 -> Lower Stage Auger Up
#   k       -> A -> Upper Stage Auger Down
#   i       -> B -> Upper Stage Auger Up
#   t       -> C -> Extend Linear Actuators
#   g       -> D -> Contract Linear Actuators
#   b       -> E -> Spin Collection System Fast
#   n       -> F -> Adjust Collection System
#---------------------------------------------------------------

from pynput import keyboard
import socket

# Variable Declarations for socket
sock = socket.socket()
host = "192.168.0.103"
port = 80

# Initialize command variables
command = "0"
newCommand = "0"

# Function called when a key is pressed on the keyboard
def on_press(key):
    global command
    global newCommand
    try:
        k = key.char            # Checks to see if press was a character, not special key

        # Sets new command based on keyboard press
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
        k = key.name            # if special key pressed, just stick with old command
        newCommand = command

    if(newCommand != command):      # checks to see if it was a new key press rather than hold
        command = newCommand        # If new, the command will be set and the command is printed
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
        sock.send(bytes(command, "UTF-8"))          # Sends command when new 

# Function called when a key is released
def on_release(key):
    global command
    command = "0"
    print("Stopped")
    sock.send(bytes(command, "UTF-8"))  # Sets rover in idle state when nothing pressed


#---------------------------------------------------------------
#   MAIN PROGRAM
#---------------------------------------------------------------

print("Ground Station initialized")

# Connects to the Rover
sock.connect((host, port))

print("Rover Connected, press esc to disconnect")

# Initializes listener with the interrupt functions declared
with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()
