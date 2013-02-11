from multiprocessing import Process, Pipe
from optparse import OptionParser

from vision import *
from server import *


class OptParser(OptionParser):
    """
    The default OptionParser exits with exit code 2
    if OptionParser.error() is called. Unfortunately this
    screws up our vision restart script which tries to indefinitely
    restart the vision system with bad options. This just exits with
    0 instead so everything works.
    """
    def error(self, msg):
        self.print_usage(sys.stderr)
        self.exit(0, "%s: error: %s\n" % (self.get_prog_name(), msg))

if __name__ == "__main__":

    parser = OptParser()
    parser.add_option('-p', '--pitch', dest='pitch', type='int', metavar='PITCH', default=0,
                      help='PITCH should be 0 for main pitch, 1 for the other pitch')
    parser.add_option('-f', '--file', dest='file', metavar='FILE',
                      help='Use FILE as input instead of capturing from Camera')
    parser.add_option('-s', '--stdout', action='store_true', dest='stdout', default=False,
                      help='Send output to stdout instead of using a socket')
    parser.add_option('-r', '--reset', action='store_true', dest='resetPitchSize', default=False,
                      help='Don\'t restore the last run\'s saved pitch size')
    parser.add_option('-n', '--nogui', action='store_true', dest='noGui', default=True,
                      help='Don\'t print info on camera stream')

    (options, args) = parser.parse_args()
    if options.pitch not in [0, 1]:
        parser.error('Pitch must be 0 or 1')

    SOCK_ADDRESS = '/tmp/vision_sock2'

    # TODO: perhaps a shared array may be quicker than all those pipes. Research!
    srv_parent, srv_child = Pipe()
    vis_parent, vis_child = Pipe()

    srv = Process(target=Server, args=(SOCK_ADDRESS, srv_child, ))
    vis = Process(target=Vision, args=(options.pitch, options.stdout,
                                         options.file, options.resetPitchSize, options.noGui, vis_child, ))
    print 'Starting server'
    srv.start()
    print 'Starting vision'
    vis.start()

    # Send pitch data to connected client
    pitch_size = vis_parent.recv()
    srv_parent.send(pitch_size)

    while True:
        data = vis_parent.recv()

        if data == 'q':
            print 'Terminating processes here'
            srv.join()
            vis.join()
            srv_parent.close()
            srv_child.close()
            vis_parent.close()
            vis_child.close()
            break

        request = srv_parent.poll()

        if request:
            req = srv_parent.recv()
            if req == 2:
                print 'Terminating processes'
                vis.join()
                srv.join()
            else:
                print 'Sending to server'
                srv_parent.send(data)

        del(data)
