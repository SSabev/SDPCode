import socket
import os
from ctypes import *


class InitSignal(Structure):
    _fields_ = [("pitchWidth", c_int), ("pitchHeight", c_int)]


class RegSignal(Structure):
    fields_ = [("x", c_int), ("y", c_int)]


class FrameData(Structure):
    _fields_ = [("yellow_x", c_int), ("yellow_y", c_int), ("yellow_angle", c_float), \
            ("blue_x", c_int), ("blue_y", c_int), ("blue_angle", c_float),
            ("ball_x", c_int), ("ball_y", c_int), ("timestamp", c_uint)]

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
            del(data)
        elif data == 'b':
            print 'Sending data back to the client'
            # this is navigation data
            f_data = FrameData(2, 3, 4, 5, 6, 7, 8, 8, 1)
            connection.send(f_data)
            del(f_data)
        elif data == 'c':
            print 'Client terminated connection'
            connection.close()
            break
        elif data:
            print data
        else:
            print 'No more data'
            break
