from pynput import keyboard

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
    except:
        k = key.name
        newCommand = command

    if(newCommand != command):
        command = newCommand
        print(command)

def on_release(key):
    global command
    command = "0"
    print(command)

with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()

    


# while True:
#     if keyboard.is_pressed('esc'):
#         break        
#     elif keyboard.is_pressed('s'):
#             newCommand = "1"
#     elif keyboard.is_pressed('w'):
#             newCommand = "2"
#     elif keyboard.is_pressed('a'):
#             newCommand = "3"
#     elif keyboard.is_pressed('d'):
#             newCommand = "4"
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