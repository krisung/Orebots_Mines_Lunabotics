import socket

sock = socket.socket()
host = "192.168.0.103"
port = 80

while(True):
    command = input("Enter a message to send, or enter 'exit'")
    if (command == "exit\n"):
        break
    sock.connect((host, port))
    sock.send(bytes("test", "UTF-8"))

sock.close()


