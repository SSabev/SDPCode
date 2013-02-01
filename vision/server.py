import socket
import os

from c_types import *


def run(pipe):
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
