import socket
import os

from c_types import *


class RunServer(object):

    def __init__(self, address, pipe):
        try:
            os.unlink(server_address)
        except OSError:
            if os.path.exists(server_address):
                raise
        self.address = address
        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.sock.bind(self.address)
        self.sock.listen(1)

    def run(self):
        while True:
        print 'Waiting for a connection'
        connection, _ = sock.accept()
        print 'Connection established'

        while True:
            data = pipe.recv()
            request = connection.recv(8)
            print str(request)
            if request == 'a':
                print 'Sending initial data to client'
                pitch_data = InitSignal(600, 400)
                s = connection.send(pitch_data)
                print s
                del(pitch_data)
            elif request == 'b':
                print 'Sending data back to the client'
                # this is navigation data
                # data = FrameData(2, 3, 4.0, 5, 6, 7.0, 8, 8, 1)
                connection.send(data)
                del(data)
            elif request == 'c':
                print 'Client terminated connection'
                connection.close()
                break
            elif request:
                print request
            else:
                print 'No more data'
                break