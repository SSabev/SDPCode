import socket
import os
from ctypes import *


class InitSignal(Structure):
    _fields_ = [("pitchWidth", c_int), ("pitchHeight", c_int)]


class RegSignal(Structure):
    fields_ = [("x", c_int), ("y", c_int)]

server_address = '/tmp/vision_sock'

# Make sure the socket does not already exist
try:
    os.unlink(server_address)
except OSError:
    if os.path.exists(server_address):
        raise

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

print 'Starting up on %s' % server_address
sock.bind(server_address)
sock.listen(1)

while True:
    print 'Waiting for a connection'
    connection, _ = sock.accept()
    print 'Connection established'

    while True:
        data = connection.recv(8)
        print str(data)
        if data == 'a':
            print 'Sending initial data to client'
            data = InitSignal(600, 400)
            s = connection.send(data)
            print s
        elif data == 0xAA:
            print 'Sending data back to the client'
            connection.sendall(data)
        elif data == 0x00:
            print 'Client terminated connection'
            connection.close()
            break
        elif data:
            print data
        else:
            print 'No more data'
            break
