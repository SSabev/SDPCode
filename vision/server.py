import socket
import os

from c_types import *


class Server(object):

    def __init__(self, address, pipe, stdout):
        self.address = address
        self.stdout = stdout

        try:
            os.unlink(self.address)
        except OSError:
            if os.path.exists(self.address):
                print "fock"
                raise

        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.sock.bind(self.address)
        self.sock.listen(1)
        self.pipe = pipe

        self.run()

    def run(self):
        pitch_data = self.pipe.recv()
        print 'Waiting for a connection'
        self.connection, _ = self.sock.accept()
        print 'Connection established'
        # readable, _, _ = select.select([self.sock], [], [], 100)

        while True:
            request = self.connection.recv(8)
            if request == 'a':
                print 'Sending initial data to client'
                self.connection.send(pitch_data)
            elif request == 'b':
                if self.stdout:
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
