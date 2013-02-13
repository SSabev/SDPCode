import socket
import os

from c_types import *


class Server(object):

    def __init__(self, address, pipe):
        self.address = address

        try:
            os.unlink(self.address)
        except OSError:
            if os.path.exists(self.address):
                raise

        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.sock.bind(self.address)
        self.sock.listen(1)
        self.pipe = pipe

        # TODO: Dunno, kinda seems redundant :?
        self.run()

    def run(self):
        pitch_data = self.pipe.recv()
        print 'Waiting for a connection'
        self.connection, _ = self.sock.accept()
        print 'Connection established'

        while True:
            request = self.connection.recv(8)
            print str(request)
            if request == 'a':
                print 'Sending initial data to client'
                self.connection.send(pitch_data)
            elif request == 'b':
                print 'Sending data to the client'
                self.pipe.send(1)
                data = self.pipe.recv()
                self.connection.send(data)
                del(data)
            elif request == 'c':
                print 'Client terminated connection'
                self.pipe.send(2)
                self.connection.close()
                break
            elif request:
                print request
            else:
                print 'No more data'
                break
