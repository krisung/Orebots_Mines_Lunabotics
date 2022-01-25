import serial

ser = serial.Serial('/dev/ttyACMO', 9600)
while True:
        data = ser.readline()
        print("Received: '{}'".format(data))