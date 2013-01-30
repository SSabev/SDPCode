import socket
import os

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
        data = connection.recv(64)
        if data == '1':
            print 'Sending data back to the client'
            data = "The big brown fox jumped over..."
            connection.sendall(data)
        elif data == '0':
            print 'Client terminated connection'
            connection.close()
            break
        else:
            print 'No more data'
            break
