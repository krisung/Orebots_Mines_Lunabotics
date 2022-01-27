import socket

sock = socket.socket()
host = "192.168.0.103"
port = 80

sock.connect((host, port))
sock.send(bytes("test", "UTF-8"))

input("Push a key to exit")

sock.close()