import socket
import os

from c_types import *


def run(pipe):
    server_address = '/tmp/vision_sock'

    print pipe.recv()

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
                f_data = FrameData(2, 3, 4.0, 5, 6, 7.0, 8, 8, 1)
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
